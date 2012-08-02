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

	//! �e�X�g�N���X
	class LayerIteratorTest : public ::testing::Test
	{
	protected:
		void SetUp();		//! �e�X�g�O�Ɏ��s
		void TearDown();	//! �e�X�g��Ɏ��s
	protected:
		ClusterArray	layer_;
	};

}


#endif	// _LAYER_ITERATOR_TEST_H_
