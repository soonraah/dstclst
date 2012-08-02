/**************************************************************************//**
 *	@file BottomUpClusteringEngine.cpp
 *	@brief 
 *****************************************************************************/

#include <vector>
#include <string>
#ifdef _DEBUG
#include <iostream>
#endif	// _DEBUG

#include "BottomUpClusteringEngine.h"


using namespace std;
using namespace dstclst;


//! �R���X�g���N�^
BottomUpClusteringEngine::BottomUpClusteringEngine() : pTree_( NULL )
{
	pTree_ = new ClusteringTree();
}


//! �f�X�g���N�^
BottomUpClusteringEngine::~BottomUpClusteringEngine()
{
	if( pTree_ )
	{
		delete pTree_;
		pTree_ = NULL;
	}
}


//! �f�[�^�ԋ������ɂ��؍\����������
void BottomUpClusteringEngine::initTreeByInterDataDistance( const DistanceTableAccessor* pDtAccessor )
{
	try
	{
		pTree_->setInitialDistance( pDtAccessor );
	}
	catch( std::bad_alloc& e )
	{
		string	msg( "Memory allocation error. : " );
		msg += e.what();
		throw BottomUpClusteringEngineException( msg.c_str() );
	}
	catch( std::invalid_argument& e )
	{
		string	msg( "Accessor is invalid. : " );
		msg += e.what();
		throw BottomUpClusteringEngineException( msg.c_str() );
	}
}


//! �{�g���A�b�v�N���X�^�����O�̖؍\���i�f���h���O�����j���쐬
void BottomUpClusteringEngine::makeClusterTree( InterClusterDistanceType icdType )
{
	doClustering( icdType, 0, 0.0f, false );
}


//! �؍\������ɂ���
void BottomUpClusteringEngine::clearClusterTree()
{
	pTree_->clear();
}


//! �؍\�����ォ��K�w�I�ɃT�[�`����C�^���[�^�𐶐�
LayerIterator BottomUpClusteringEngine::beginClusterTree() const
{
	return LayerIterator( *( pTree_->getTopLayer() ) );
}


//! �I�[�C�^���[�^�𐶐�
LayerIterator BottomUpClusteringEngine::endClusterTree() const
{
	return LayerIterator();
}


//! �N���X�^�����O����
void BottomUpClusteringEngine::doClustering(
	InterClusterDistanceType icdType,
	unsigned int thresholdNumClusters,
	float thresholdMergeCost,
	bool removeMergedChildren
)
{
	// �N���X�^�ԋ����X�V���t�@���N�^�̃C���X�^���X�쐬
	LanceWilliamsUpdatingFormula* pUpdatingFormula = NULL;
	switch( icdType )
	{
	case NEAREST:
		pUpdatingFormula = new FormulaNearest();
		break;
	case FURTHEST:
		pUpdatingFormula = new FormulaFurthest();
		break;
	case GROUP_AVG:
		pUpdatingFormula = new FormulaGroupAvg();
		break;
	case WARD:
		pUpdatingFormula = new FormulaWard();
		break;
	default:
		throw BottomUpClusteringEngineException( "Given inter-cluster distance is not supported." );
	}

	// ClusteringTree�C���X�^���X�ɍX�V����ݒ�
	pTree_->setUpdatingFormula( pUpdatingFormula );

	while( pTree_->getTopLayer()->size() > 1 )
	{
		try
		{
			pTree_->buildOneStep( removeMergedChildren );
		}
		catch( std::bad_alloc& e )
		{
			string	msg( "Memory allocation error. : " );
			msg += e.what();
			throw BottomUpClusteringEngineException( msg.c_str() );
		}
		catch( std::invalid_argument& e )
		{
			string	msg( "Invalid imput. : " );
			msg += e.what();
			throw BottomUpClusteringEngineException( msg.c_str() );
		}

#ifdef _DEBUG
		cout << "Num Clusters = " << pTree_->getTopLayer()->size() << "\t";
		cout << "Merge Cost = " << pTree_->getTopLayer()->back()->getMergeCost() << endl;
#endif	// _DEBUG
	}
}
