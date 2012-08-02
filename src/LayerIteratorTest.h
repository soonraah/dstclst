/**************************************************************************//**
 *	@file LayerIteratorTest.h
 *	@brief 
 *****************************************************************************/

#ifndef _LAYER_ITERATOR_TEST_H_
#define _LAYER_ITERATOR_TEST_H_


#include <gtest/gtest.h>

#include "ClusterInfo.h"


namespace dstclst
{

	//! テストクラス
	class LayerIteratorTest : public ::testing::Test
	{
	protected:
		void SetUp();		//! テスト前に実行
		void TearDown();	//! テスト後に実行
	protected:
		ClusterArray	layer_;
	};

}


#endif	// _LAYER_ITERATOR_TEST_H_
