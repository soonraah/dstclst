/**************************************************************************//**
 *	@file ClusteringTree.cpp
 *****************************************************************************/


#include <cfloat>
#include <sstream>

#include "ClusteringTree.h"


using namespace std;
using namespace dstclst;


// メモリ節約モードのとき、vectorのsize()がcapacity()に対してこの割合以下になるとswap技法による領域減少を実施する
static const float VecSizeRatioThreshold = 0.75f;


//! swap技法によりreserve領域を減らす処理をすべきかどうかの判定
template< typename T >
bool shouldBeReducedArea( const vector< T >& v )
{
	if( v.capacity() == 0 ) return false;

	if( static_cast< float >( v.size() ) / v.capacity() < VecSizeRatioThreshold ) return true;

	return false;
}


//! クラスタ情報を再帰的に削除
void deleteClusterRecursively( const ClusterInfo* pCluster )
{
	if( pCluster == NULL ) return;

	deleteClusterRecursively( pCluster->getChild1() );
	deleteClusterRecursively( pCluster->getChild2() );

	//pCluster->setChildren( NULL, NULL );
	delete pCluster;
}



//! コンストラクタ
ClusteringTree::ClusteringTree() : topLayer_(), interClusterDistance_(), pUpdatingFormula_( NULL ), memorySavingMode_( false ), lastClusterId_( 0 )
{
}


//! デストラクタ
ClusteringTree::~ClusteringTree()
{
	// 空にする
	this->clear();
}


//! クラスタ間距離テーブルに初期値を設定
void ClusteringTree::setInitialDistance( const DistanceTableAccessor* pDtAccessor )
{
	// NULLチェック
	if( pDtAccessor == NULL ) throw std::invalid_argument( "Distance table accessor is NULL pointer." );

	// 入力テーブルサイズのチェック
	unsigned int size = pDtAccessor->getDistanceTableSize();
	if( size == 0 ) throw std::invalid_argument( "Distance table is empty." );

	// 空にする
	this->clear();

	// 初期のクラスタ距離テーブルを作成（初期は個別データ間距離と同じ）
	// ※メモリ消費が増える！
	try {
		interClusterDistance_.reserve( size + 1 );
		interClusterDistance_.resize( size );
		for( unsigned int i = 0; i < size; ++i )
		{
			interClusterDistance_[ i ].resize( i );
			for( unsigned int j = 0; j < i; ++j )
			{
				interClusterDistance_[ i ][ j ] = pDtAccessor->getDistanceAt( i, j );
				if( interClusterDistance_[ i ][ j ] < 0.0f )	// 距離が負の値の場合
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
		// メモリ確保失敗
		interClusterDistance_.clear();
		vector< vector< float > >( interClusterDistance_ ).swap( interClusterDistance_ );
		topLayer_.clear();
		ClusterArray( topLayer_ ).swap( topLayer_ );
		throw std::bad_alloc( e.what() );
	}
	catch( std::domain_error& e )
	{
		// 距離が負の値の場合
		interClusterDistance_.clear();
		vector< vector< float > >( interClusterDistance_ ).swap( interClusterDistance_ );
		topLayer_.clear();
		ClusterArray( topLayer_ ).swap( topLayer_ );
		throw std::domain_error( e.what() );
	}

	// 木構造の底辺部分（現時点ではtopLayer）のクラスタを作成
	// ※メモリ消費が増える！
	try {
		topLayer_.reserve( size + 1 );	// +1はクラスタマージ時に先にマージクラスタを追加してから子クラスタ2つを削除するため
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
		// メモリ確保失敗
		interClusterDistance_.clear();
		vector< vector< float > >( interClusterDistance_ ).swap( interClusterDistance_ );
		topLayer_.clear();
		ClusterArray( topLayer_ ).swap( topLayer_ );
		throw std::bad_alloc( e.what() );
	}
}


//! 更新式（=クラスタ間距離の定義）を設定
void ClusteringTree::setUpdatingFormula( LanceWilliamsUpdatingFormula* pUpdatingFormula )
{
	pUpdatingFormula_ = pUpdatingFormula;
}


//! クラスタ間距離を調べ、次のクラスタをマージして木構造を1ステップ育てる
float ClusteringTree::buildOneStep( bool killChildren )
{
	// 残りクラスタ数のチェック
	if( topLayer_.size() <= 1 ) return -1.0f;

	// 更新式ファンクタが設定されているか
	if( pUpdatingFormula_ == NULL ) return -1.0f;

	// 距離最小のクラスタ組み合わせを探す
	unsigned int clusterIndex1 = 0;
	unsigned int clusterIndex2 = 0;
	float distMin = FLT_MAX;
	findMinDistance( clusterIndex2, clusterIndex1, distMin );	// 必ず clusterIndex1 < clusterIndex2 となる
	if( distMin == FLT_MAX ) return -1.0f;						// 最小値を見つけられない場合

	// 距離最小のクラスタをマージして新しいクラスタを作成
	ClusterInfo* pMerged = new ClusterInfo( topLayer_[ clusterIndex1 ],
											topLayer_[ clusterIndex2 ],
											distMin,						// マージコスト = クラスタ間距離
											++lastClusterId_ );

	// マージ後のクラスタを追加
	addClusterIntoTopLayer( pMerged, clusterIndex1, clusterIndex2 );

	// 必要ならマージ対象クラスタのインスタンスを削除
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
	
	// マージ前の子クラスタを最上層から削除
	deleteClusterFromTopLayer( clusterIndex2 );
	deleteClusterFromTopLayer( clusterIndex1 );

	return pMerged->getMergeCost();
}


//! 現在の最上層クラスタを取得
const ClusterArray* ClusteringTree::getTopLayer() const
{
	return &topLayer_;
}


//! クラスタ、クラスタ間距離の情報を削除
void ClusteringTree::clear()
{
	// ClusterInfoインスタンスをdelete
	for( ClusterArray::iterator it = topLayer_.begin(); it != topLayer_.end(); ++it )
	{
		deleteClusterRecursively( *it );
		*it = NULL;
	}

	// swap技法で領域削除
	topLayer_.clear();
	ClusterArray( topLayer_ ).swap( topLayer_ );
	interClusterDistance_.clear();
	vector< vector< float > >( interClusterDistance_ ).swap( interClusterDistance_ );

	return;
}


//! 最小のクラスタ間距離を探す
void ClusteringTree::findMinDistance( unsigned int& rowMin, unsigned int& columnMin, float& distanceMin )
{
	unsigned int tableSize = interClusterDistance_.size();

	// 最小距離を調べる
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


//! 最上層からクラスタを削除
void ClusteringTree::deleteClusterFromTopLayer( unsigned int clusterIndex )
{
	// インデックスをチェック
	if( clusterIndex < 0 || topLayer_.size() <= clusterIndex ) throw std::out_of_range( "Given cluster index is out of range." );

	// クラスタ列から削除
	topLayer_.erase( topLayer_.begin() + clusterIndex );
	if( memorySavingMode_ && shouldBeReducedArea( topLayer_ ) )
		ClusterArray( topLayer_ ).swap( topLayer_ );	// swap技法によるreserve領域最適化

	// クラスタ間距離テーブルから削除
	vector< vector< float > >::iterator itRow = interClusterDistance_.begin() + clusterIndex;
	interClusterDistance_.erase( itRow );	// 行削除
	for( itRow = interClusterDistance_.begin() + clusterIndex; itRow != interClusterDistance_.end(); ++itRow )
	{
		itRow->erase( itRow->begin() + clusterIndex );
		// if( memorySavingMode_ && shouldBeReducedArea( *itRow ) )
		// {
		// 	vector< float >( *itRow ).swap( *itRow );	
		// }
	}
	if( memorySavingMode_ && shouldBeReducedArea( interClusterDistance_ ) )
		vector< vector< float > >( interClusterDistance_ ).swap( interClusterDistance_ );	// swap技法によるreserve領域最適化
}


//! 最上層にクラスタを追加
void ClusteringTree::addClusterIntoTopLayer( const ClusterInfo* pCluster, unsigned int child1Index, unsigned int child2Index )
{
	if( pCluster == NULL ) throw std::invalid_argument( "Given pointer to ClusterInfo instance is NULL." );

	// クラスタ列に追加
	topLayer_.push_back( pCluster );

	// クラスタ間距離テーブルに追加
	vector< float >	newRow;
	unsigned int size = interClusterDistance_.size();
	newRow.resize( size );
	float d1i = 0.0f;	// マージ前のクラスタ1（child1Index）と任意のクラスタ（i）との距離
	float d2i = 0.0f;	// マージ前のクラスタ2（child2Index）と任意のクラスタ（i）との距離
	float d12 = interClusterDistance_[ child2Index ][ child1Index ];	// マージ前のクラスタ1, 2の距離
	unsigned int n1 = topLayer_[ child1Index ]->getMembers()->size();	// マージ前のクラスタ1（child1Index）のメンバ数
	unsigned int n2 = topLayer_[ child2Index ]->getMembers()->size();	// マージ前のクラスタ2（child2Index）のメンバ数
	unsigned int ni = 0;												// 任意のクラスタ（i）のメンバ数

	for( unsigned int i = 0; i < size; ++i )
	{
		if( i == child1Index || i == child2Index )
		{
			newRow[ i ] = -1.0f;	// どうせすぐに消すから
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

