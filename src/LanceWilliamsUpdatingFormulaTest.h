/**************************************************************************//**
 *	@file LanceWilliamsUpdatingFormulaTest.h
 *****************************************************************************/

#ifndef _LANCE_WILLIAMS_UPDATING_TEST_H_
#define _LANCE_WILLIAMS_UPDATING_TEST_H_


#include <gtest/gtest.h>


namespace dstclst
{

	//! テストクラス
	class LanceWilliamsUpdatingFormulaTest : public ::testing::Test
	{
	protected:
		void SetUp();		//! テスト前に実行
		void TearDown();	//! テスト後に実行
	};

}


#endif	// _LANCE_WILLIAMS_UPDATING_TEST_H_
