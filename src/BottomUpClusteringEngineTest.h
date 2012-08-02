/**************************************************************************//**
 *	@file BottomUpClusteringEngineTest.h
 *	@brief 
 *****************************************************************************/

#ifndef _BOTTOM_UP_CLUSTERING_ENGINE_TEST_H_
#define _BOTTOM_UP_CLUSTERING_ENGINE_TEST_H_


#include <gtest/gtest.h>

#include "BottomUpClusteringEngine.h"


namespace dstclst
{
	//! �e�X�g�N���X
	class BottomUpClusteringEngineTest : public ::testing::Test
	{
	protected:
		void SetUp();		//! �e�X�g�O�Ɏ��s
		void TearDown();	//! �e�X�g��Ɏ��s

	protected:
		BottomUpClusteringEngine*	pEngine_;

	};
}


#endif	// _BOTTOM_UP_CLUSTERING_ENGINE_TEST_H_



