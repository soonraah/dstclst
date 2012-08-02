/**************************************************************************//**
 *	@file ClusteringTreeTest.h
 *****************************************************************************/

#ifndef _CLUSTERING_TREE_TEST_H_
#define _CLUSTERING_TREE_TEST_H_


#include <gtest/gtest.h>

#include "ClusteringTree.h"


namespace dstclst
{
	//! �e�X�g�N���X
	class ClusteringTreeTest : public ::testing::Test
	{
	protected:
		void SetUp();		//! �e�X�g�O�Ɏ��s
		void TearDown();	//! �e�X�g��Ɏ��s

		//! �N���X�^�ԋ����e�[�u���ւ̃|�C���^���擾
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
