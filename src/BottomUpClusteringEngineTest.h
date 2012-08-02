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
	//! テストクラス
	class BottomUpClusteringEngineTest : public ::testing::Test
	{
	protected:
		void SetUp();		//! テスト前に実行
		void TearDown();	//! テスト後に実行

	protected:
		BottomUpClusteringEngine*	pEngine_;

	};
}


#endif	// _BOTTOM_UP_CLUSTERING_ENGINE_TEST_H_



