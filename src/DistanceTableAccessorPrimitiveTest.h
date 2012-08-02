/**************************************************************************//**
 *	@file DistanceTableAccessorPrimitiveTest.h
 *****************************************************************************/

#ifndef _DISTANCE_TABLE_ACCESSOR_PRIMITIVE_TEST_H_
#define _DISTANCE_TABLE_ACCESSOR_PRIMITIVE_TEST_H_


#include <gtest/gtest.h>
#include <vector>


namespace dstclst
{

	//! テストクラス
	class DistanceTableAccessorPrimitiveTest : public ::testing::Test
	{
	protected:
		void SetUp();		//! テスト前に実行
		void TearDown();	//! テスト後に実行

	protected:
		std::vector< std::vector< float > >		distTblCorrect_;
	};

}


#endif	// _DISTANCE_TABLE_ACCESSOR_PRIMITIVE_TEST_H_
