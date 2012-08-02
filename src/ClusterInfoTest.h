/**************************************************************************//**
 *	@file ClusterInfoTest.h
 *****************************************************************************/

#ifndef _CLUSTER_INFO_TEST_H_
#define _CLUSTER_INFO_TEST_H_


#include <gtest/gtest.h>


namespace dstclst
{

	//! テストクラス
	class ClusterInfoTest : public ::testing::Test
	{
	protected:
		void SetUp();		//! テスト前に実行
		void TearDown();	//! テスト後に実行
	};

}


#endif	// _CLUSTER_INFO_TEST_H_
