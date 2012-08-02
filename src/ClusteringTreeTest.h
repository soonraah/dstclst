/**************************************************************************//**
 *	@file ClusteringTreeTest.h
 *****************************************************************************/

#ifndef _CLUSTERING_TREE_TEST_H_
#define _CLUSTERING_TREE_TEST_H_


#include <gtest/gtest.h>

#include "ClusteringTree.h"


namespace dstclst
{
	//! テストクラス
	class ClusteringTreeTest : public ::testing::Test
	{
	protected:
		void SetUp();		//! テスト前に実行
		void TearDown();	//! テスト後に実行

		//! クラスタ間距離テーブルへのポインタを取得
		std::vector< std::vector< float > >* getInterClusterDistance()
		{
			return &( pTree_->interClusterDistance_ );
		};


	protected:
		ClusteringTree*						pTree_;
		DistanceTableAccessor*				pDtAccessor_;
		std::vector< std::vector< float > >	dataDistanceTable_;
	};
}


#endif	// _CLUSTERING_TREE_TEST_H_
