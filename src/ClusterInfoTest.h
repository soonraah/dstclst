/**************************************************************************//**
 *	@file ClusterInfoTest.h
 *****************************************************************************/

#ifndef _CLUSTER_INFO_TEST_H_
#define _CLUSTER_INFO_TEST_H_


#include <gtest/gtest.h>


namespace dstclst
{

	//! �e�X�g�N���X
	class ClusterInfoTest : public ::testing::Test
	{
	protected:
		void SetUp();		//! �e�X�g�O�Ɏ��s
		void TearDown();	//! �e�X�g��Ɏ��s
	};

}


#endif	// _CLUSTER_INFO_TEST_H_
