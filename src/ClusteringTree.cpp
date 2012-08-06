/**************************************************************************//**
 *	@file ClusteringTree.cpp
 *****************************************************************************/


#include <cfloat>
#include <sstream>

#include "ClusteringTree.h"


using namespace std;
using namespace dstclst;


// �������ߖ񃂁[�h�̂Ƃ��Avector��size()��capacity()�ɑ΂��Ă��̊����ȉ��ɂȂ��swap�Z�@�ɂ��̈挸�������{����
static const float VecSizeRatioThreshold = 0.75f;


//! swap�Z�@�ɂ��reserve�̈�����炷���������ׂ����ǂ����̔���
template< typename T >
bool shouldBeReducedArea( const vector< T >& v )
{
	if( v.capacity() == 0 ) return false;

	if( static_cast< float >( v.size() ) / v.capacity() < VecSizeRatioThreshold ) return true;

	return false;
}


//! �N���X�^�����ċA�I�ɍ폜
void deleteClusterRecursively( const ClusterInfo* pCluster )
{
	if( pCluster == NULL ) return;

	deleteClusterRecursively( pCluster->getChild1() );
	deleteClusterRecursively( pCluster->getChild2() );

	//pCluster->setChildren( NULL, NULL );
	delete pCluster;
}



//! �R���X�g���N�^
ClusteringTree::ClusteringTree() : topLayer_(), interClusterDistance_(), pUpdatingFormula_( NULL ), memorySavingMode_( false ), lastClusterId_( 0 )
{
}


//! �f�X�g���N�^
ClusteringTree::~ClusteringTree()
{
	// ��ɂ���
	this->clear();
}


//! �N���X�^�ԋ����e�[�u���ɏ����l��ݒ�
void ClusteringTree::setInitialDistance( const DistanceTableAccessor* pDtAccessor )
{
	// NULL�`�F�b�N
	if( pDtAccessor == NULL ) throw std::invalid_argument( "Distance table accessor is NULL pointer." );

	// ���̓e�[�u���T�C�Y�̃`�F�b�N
	unsigned int size = pDtAccessor->getDistanceTableSize();
	if( size == 0 ) throw std::invalid_argument( "Distance table is empty." );

	// ��ɂ���
	this->clear();

	// �����̃N���X�^�����e�[�u�����쐬�i�����͌ʃf�[�^�ԋ����Ɠ����j
	// �����������������I
	try {
		interClusterDistance_.reserve( size + 1 );
		interClusterDistance_.resize( size );
		for( unsigned int i = 0; i < size; ++i )
		{
			interClusterDistance_[ i ].resize( i );
			for( unsigned int j = 0; j < i; ++j )
			{
				interClusterDistance_[ i ][ j ] = pDtAccessor->getDistanceAt( i, j );
				if( interClusterDistance_[ i ][ j ] < 0.0f )	// ���������̒l�̏ꍇ
				{
					ostringstream oss;
					oss << "Distance[" << i << "][" << j << "] is negative value (" << interClusterDistance_[ i ][ j ] << ").";
					throw std::domain_error( oss.str() );
				}
			}
		}
	}
	catch( std::bad_alloc& e )
	{
		// �������m�ێ��s
		interClusterDistance_.clear();
		vector< vector< float > >( interClusterDistance_ ).swap( interClusterDistance_ );
		topLayer_.clear();
		ClusterArray( topLayer_ ).swap( topLayer_ );
		throw std::bad_alloc( e.what() );
	}
	catch( std::domain_error& e )
	{
		// ���������̒l�̏ꍇ
		interClusterDistance_.clear();
		vector< vector< float > >( interClusterDistance_ ).swap( interClusterDistance_ );
		topLayer_.clear();
		ClusterArray( topLayer_ ).swap( topLayer_ );
		throw std::domain_error( e.what() );
	}

	// �؍\���̒�ӕ����i�����_�ł�topLayer�j�̃N���X�^���쐬
	// �����������������I
	try {
		topLayer_.reserve( size + 1 );	// +1�̓N���X�^�}�[�W���ɐ�Ƀ}�[�W�N���X�^��ǉ����Ă���q�N���X�^2���폜���邽��
		topLayer_.resize( size, NULL );
		ClusterInfo*	p = NULL;
		for( DataIndex i = 0; i < size; ++i )
		{
			p = new ClusterInfo();
			p->addMember( i );
			p->setId( ++lastClusterId_ );
			topLayer_[ i ] = p;
		}
	}
	catch( std::bad_alloc& e )
	{
		// �������m�ێ��s
		interClusterDistance_.clear();
		vector< vector< float > >( interClusterDistance_ ).swap( interClusterDistance_ );
		topLayer_.clear();
		ClusterArray( topLayer_ ).swap( topLayer_ );
		throw std::bad_alloc( e.what() );
	}
}


//! �X�V���i=�N���X�^�ԋ����̒�`�j��ݒ�
void ClusteringTree::setUpdatingFormula( LanceWilliamsUpdatingFormula* pUpdatingFormula )
{
	pUpdatingFormula_ = pUpdatingFormula;
}


//! �N���X�^�ԋ����𒲂ׁA���̃N���X�^���}�[�W���Ė؍\����1�X�e�b�v��Ă�
float ClusteringTree::buildOneStep( bool killChildren )
{
	// �c��N���X�^���̃`�F�b�N
	if( topLayer_.size() <= 1 ) return -1.0f;

	// �X�V���t�@���N�^���ݒ肳��Ă��邩
	if( pUpdatingFormula_ == NULL ) return -1.0f;

	// �����ŏ��̃N���X�^�g�ݍ��킹��T��
	unsigned int clusterIndex1 = 0;
	unsigned int clusterIndex2 = 0;
	float distMin = FLT_MAX;
	findMinDistance( clusterIndex2, clusterIndex1, distMin );	// �K�� clusterIndex1 < clusterIndex2 �ƂȂ�
	if( distMin == FLT_MAX ) return -1.0f;						// �ŏ��l���������Ȃ��ꍇ

	// �����ŏ��̃N���X�^���}�[�W���ĐV�����N���X�^���쐬
	ClusterInfo* pMerged = new ClusterInfo( topLayer_[ clusterIndex1 ],
											topLayer_[ clusterIndex2 ],
											distMin,						// �}�[�W�R�X�g = �N���X�^�ԋ���
											++lastClusterId_ );

	// �}�[�W��̃N���X�^��ǉ�
	addClusterIntoTopLayer( pMerged, clusterIndex1, clusterIndex2 );

	// �K�v�Ȃ�}�[�W�ΏۃN���X�^�̃C���X�^���X���폜
	if( killChildren )
	{
		if( topLayer_[ clusterIndex1 ] )
		{
			delete topLayer_[ clusterIndex1 ];
			topLayer_[ clusterIndex1 ] = NULL;
		}
		if( topLayer_[ clusterIndex2 ] )
		{
			delete topLayer_[ clusterIndex2 ];
			topLayer_[ clusterIndex2 ] = NULL;
		}
		pMerged->setChildren( NULL, NULL );
	}
	
	// �}�[�W�O�̎q�N���X�^���ŏ�w����폜
	deleteClusterFromTopLayer( clusterIndex2 );
	deleteClusterFromTopLayer( clusterIndex1 );

	return pMerged->getMergeCost();
}


//! ���݂̍ŏ�w�N���X�^���擾
const ClusterArray* ClusteringTree::getTopLayer() const
{
	return &topLayer_;
}


//! �N���X�^�A�N���X�^�ԋ����̏����폜
void ClusteringTree::clear()
{
	// ClusterInfo�C���X�^���X��delete
	for( ClusterArray::iterator it = topLayer_.begin(); it != topLayer_.end(); ++it )
	{
		deleteClusterRecursively( *it );
		*it = NULL;
	}

	// swap�Z�@�ŗ̈�폜
	topLayer_.clear();
	ClusterArray( topLayer_ ).swap( topLayer_ );
	interClusterDistance_.clear();
	vector< vector< float > >( interClusterDistance_ ).swap( interClusterDistance_ );

	return;
}


//! �ŏ��̃N���X�^�ԋ�����T��
void ClusteringTree::findMinDistance( unsigned int& rowMin, unsigned int& columnMin, float& distanceMin )
{
	unsigned int tableSize = interClusterDistance_.size();

	// �ŏ������𒲂ׂ�
	unsigned int iMin = 0;
	unsigned int jMin = 0;
	float distMin = FLT_MAX;
	for( unsigned int i = 0; i < tableSize; ++i )
	{
		for( unsigned int j = 0; j < i; ++j )
		{
			if( interClusterDistance_[ i ][ j ] < distMin )
			{
				iMin = i;
				jMin = j;
				distMin = interClusterDistance_[ i ][ j ];
			}
		}
	}
	rowMin		= iMin;
	columnMin	= jMin;
	distanceMin	= distMin;
	return;
}


//! �ŏ�w����N���X�^���폜
void ClusteringTree::deleteClusterFromTopLayer( unsigned int clusterIndex )
{
	// �C���f�b�N�X���`�F�b�N
	if( clusterIndex < 0 || topLayer_.size() <= clusterIndex ) throw std::out_of_range( "Given cluster index is out of range." );

	// �N���X�^�񂩂�폜
	topLayer_.erase( topLayer_.begin() + clusterIndex );
	if( memorySavingMode_ && shouldBeReducedArea( topLayer_ ) )
		ClusterArray( topLayer_ ).swap( topLayer_ );	// swap�Z�@�ɂ��reserve�̈�œK��

	// �N���X�^�ԋ����e�[�u������폜
	vector< vector< float > >::iterator itRow = interClusterDistance_.begin() + clusterIndex;
	interClusterDistance_.erase( itRow );	// �s�폜
	for( itRow = interClusterDistance_.begin() + clusterIndex; itRow != interClusterDistance_.end(); ++itRow )
	{
		itRow->erase( itRow->begin() + clusterIndex );
		// if( memorySavingMode_ && shouldBeReducedArea( *itRow ) )
		// {
		// 	vector< float >( *itRow ).swap( *itRow );	
		// }
	}
	if( memorySavingMode_ && shouldBeReducedArea( interClusterDistance_ ) )
		vector< vector< float > >( interClusterDistance_ ).swap( interClusterDistance_ );	// swap�Z�@�ɂ��reserve�̈�œK��
}


//! �ŏ�w�ɃN���X�^��ǉ�
void ClusteringTree::addClusterIntoTopLayer( const ClusterInfo* pCluster, unsigned int child1Index, unsigned int child2Index )
{
	if( pCluster == NULL ) throw std::invalid_argument( "Given pointer to ClusterInfo instance is NULL." );

	// �N���X�^��ɒǉ�
	topLayer_.push_back( pCluster );

	// �N���X�^�ԋ����e�[�u���ɒǉ�
	vector< float >	newRow;
	unsigned int size = interClusterDistance_.size();
	newRow.resize( size );
	float d1i = 0.0f;	// �}�[�W�O�̃N���X�^1�ichild1Index�j�ƔC�ӂ̃N���X�^�ii�j�Ƃ̋���
	float d2i = 0.0f;	// �}�[�W�O�̃N���X�^2�ichild2Index�j�ƔC�ӂ̃N���X�^�ii�j�Ƃ̋���
	float d12 = interClusterDistance_[ child2Index ][ child1Index ];	// �}�[�W�O�̃N���X�^1, 2�̋���
	unsigned int n1 = topLayer_[ child1Index ]->getMembers()->size();	// �}�[�W�O�̃N���X�^1�ichild1Index�j�̃����o��
	unsigned int n2 = topLayer_[ child2Index ]->getMembers()->size();	// �}�[�W�O�̃N���X�^2�ichild2Index�j�̃����o��
	unsigned int ni = 0;												// �C�ӂ̃N���X�^�ii�j�̃����o��

	for( unsigned int i = 0; i < size; ++i )
	{
		if( i == child1Index || i == child2Index )
		{
			newRow[ i ] = -1.0f;	// �ǂ��������ɏ�������
		}
		else
		{
			d1i = i < child1Index ? interClusterDistance_[ child1Index ][ i ] : interClusterDistance_[ i ][ child1Index ];
			d2i = i < child2Index ? interClusterDistance_[ child2Index ][ i ] : interClusterDistance_[ i ][ child2Index ];
			ni = topLayer_[ i ]->getMembers()->size();
			newRow[ i ] = ( *pUpdatingFormula_ )( d1i, d2i, d12, n1, n2, ni );
		}
	}

	interClusterDistance_.push_back( newRow );
}

