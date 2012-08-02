/**************************************************************************//**
 *	@file DistanceTableAccessorPrimitiveTest.h
 *****************************************************************************/

#ifndef _DISTANCE_TABLE_ACCESSOR_PRIMITIVE_TEST_H_
#define _DISTANCE_TABLE_ACCESSOR_PRIMITIVE_TEST_H_


#include <gtest/gtest.h>
#include <vector>


namespace dstclst
{

	//! �e�X�g�N���X
	class DistanceTableAccessorPrimitiveTest : public ::testing::Test
	{
	protected:
		void SetUp();		//! �e�X�g�O�Ɏ��s
		void TearDown();	//! �e�X�g��Ɏ��s

	protected:
		std::vector< std::vector< float > >		distTblCorrect_;
	};

}


#endif	// _DISTANCE_TABLE_ACCESSOR_PRIMITIVE_TEST_H_
