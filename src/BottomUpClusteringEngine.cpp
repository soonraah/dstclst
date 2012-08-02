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


//! コンストラクタ
BottomUpClusteringEngine::BottomUpClusteringEngine() : pTree_( NULL )
{
	pTree_ = new ClusteringTree();
}


//! デストラクタ
BottomUpClusteringEngine::~BottomUpClusteringEngine()
{
	if( pTree_ )
	{
		delete pTree_;
		pTree_ = NULL;
	}
}


//! データ間距離情報により木構造を初期化
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


//! ボトムアップクラスタリングの木構造（デンドログラム）を作成
void BottomUpClusteringEngine::makeClusterTree( InterClusterDistanceType icdType )
{
	doClustering( icdType, 0, 0.0f, false );
}


//! 木構造を空にする
void BottomUpClusteringEngine::clearClusterTree()
{
	pTree_->clear();
}


//! 木構造を上から階層的にサーチするイタレータを生成
LayerIterator BottomUpClusteringEngine::beginClusterTree() const
{
	return LayerIterator( *( pTree_->getTopLayer() ) );
}


//! 終端イタレータを生成
LayerIterator BottomUpClusteringEngine::endClusterTree() const
{
	return LayerIterator();
}


//! クラスタリング処理
void BottomUpClusteringEngine::doClustering(
	InterClusterDistanceType icdType,
	unsigned int thresholdNumClusters,
	float thresholdMergeCost,
	bool removeMergedChildren
)
{
	// クラスタ間距離更新式ファンクタのインスタンス作成
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

	// ClusteringTreeインスタンスに更新式を設定
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
