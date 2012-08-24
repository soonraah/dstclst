/**************************************************************************//**
 *	@file BottomUpClusteringEngineTest.cpp
 *	@brief 
 *****************************************************************************/


#include "BottomUpClusteringEngineTest.h"
#include "TestDataCreation.h"
#include "DistanceTableAccessorPrimitive.h"


using namespace std;
using namespace dstclst;


//! �e�X�g�O�Ɏ��s
void BottomUpClusteringEngineTest::SetUp()
{
	pEngine_ = new BottomUpClusteringEngine();
}


//! �e�X�g��Ɏ��s
void BottomUpClusteringEngineTest::TearDown()
{
	if( pEngine_ ) delete pEngine_;
}


//! �f�[�^�ԋ������ɂ��؍\�����������̃e�X�g
TEST_F( BottomUpClusteringEngineTest, test_initTreeByInterDataDistance )
{
	DistanceTableAccessor*	pDta = NULL;

	// NULL
	ASSERT_THROW( pEngine_->initTreeByInterDataDistance( pDta ), BottomUpClusteringEngineException );

	// �e�[�u�����܂���
	pDta = new DistanceTableAccessorPrimitive();
	ASSERT_THROW( pEngine_->initTreeByInterDataDistance( pDta ), BottomUpClusteringEngineException );

	// �f�[�^�ԋ������̗p��
	vector< vector< float > >	interDataDistTable;
	setDataDistance1( interDataDistTable );
	pDta->setDistanceTable( &interDataDistTable, 0, 0 );

	// �؍\��������
	pEngine_->initTreeByInterDataDistance( pDta );
	interDataDistTable.clear();	// ��������Ȃ�����e�[�u���폜
	delete pDta;

	// �f�[�^�����̃N���X�^��������
	ASSERT_EQ( 1000, pEngine_->beginClusterTree()->size() );
}


//! �؍\���쐬�̈�A�̗�����e�X�g
TEST_F( BottomUpClusteringEngineTest, test_makeTreeWithLargeData )
{
	// �f�[�^�ԋ������̗p��
	DistanceTableAccessor*	pDta = new DistanceTableAccessorPrimitive();
	vector< vector< float > >	interDataDistTable;
	setDataDistance1( interDataDistTable );
	pDta->setDistanceTable( &interDataDistTable, 0, 0 );

	// �؍\��������
	pEngine_->initTreeByInterDataDistance( pDta );
	interDataDistTable.clear();	// ��������Ȃ�����e�[�u���폜
	delete pDta;

	// �؍\���쐬
	ASSERT_THROW( pEngine_->makeClusterTree( static_cast< InterClusterDistanceType >( 99 ) ), BottomUpClusteringEngineException );
	pEngine_->makeClusterTree( WARD );

	// �؍\���̃`�F�b�N
	int n = 0;
	LayerIterator it	= pEngine_->beginClusterTree();
	LayerIterator itEnd	= pEngine_->endClusterTree();
	ASSERT_EQ( 1, it->size() );

	for( ; it != itEnd; ++it )
	{
		++n;
		if( n == 4 )	// �N���X�^����4�̂Ƃ�
		{
			ASSERT_EQ( 4, it->size() );

			// �N���X�^�����O���������s���Ă���΁A0�`249, 250�`499, 500�`749, 750�`999 ��4�̃N���X�^�ɕ�����Ă���͂�
			for( int ic = 0; ic < 4; ++ic )
			{
				ASSERT_EQ( 250, it->at( ic )->getMembers()->size() );
				if( it->at( ic )->getMembers()->at( 0 ) < 250 )
				{
					for( vector< DataIndex >::const_iterator itd = it->at( ic )->getMembers()->begin(); itd != it->at( ic )->getMembers()->end(); ++itd )
					{
						ASSERT_TRUE( 0 <= *itd && *itd < 250 );
					}
				}
				else if( it->at( ic )->getMembers()->at( 0 ) < 500 )
				{
					for( vector< DataIndex >::const_iterator itd = it->at( ic )->getMembers()->begin(); itd != it->at( ic )->getMembers()->end(); ++itd )
					{
						ASSERT_TRUE( 250 <= *itd && *itd < 500 );
					}
				}
				else if( it->at( ic )->getMembers()->at( 0 ) < 750 )
				{
					for( vector< DataIndex >::const_iterator itd = it->at( ic )->getMembers()->begin(); itd != it->at( ic )->getMembers()->end(); ++itd )
					{
						ASSERT_TRUE( 500 <= *itd && *itd < 750 );
					}
				}
				else
				{
					for( vector< DataIndex >::const_iterator itd = it->at( ic )->getMembers()->begin(); itd != it->at( ic )->getMembers()->end(); ++itd )
					{
						ASSERT_TRUE( 750 <= *itd && *itd < 1000 );
					}
				}
			}
		}
	}
	ASSERT_EQ( 1000, n );

	ASSERT_EQ( 1, pEngine_->beginClusterTree()->size() );
	ASSERT_EQ( 1999, pEngine_->beginClusterTree()->front()->getId() );

	// �؍\������ɂ���
	pEngine_->clearClusterTree();
	ASSERT_EQ( pEngine_->beginClusterTree(), pEngine_->endClusterTree() );
}


//! ����0�̊֌W���܂ރf�[�^����؍\���쐬�̈�A�̗�����e�X�g
TEST_F( BottomUpClusteringEngineTest, test_makeTreeWithZeroDistance )
{
	// �f�[�^�ԋ������̗p��
	DistanceTableAccessor*	pDta = new DistanceTableAccessorPrimitive();
	vector< vector< float > >	interDataDistTable;
	setDataDistance2( interDataDistTable );
	pDta->setDistanceTable( &interDataDistTable, 0, 0 );

	// �؍\��������
	pEngine_->initTreeByInterDataDistance( pDta );
	interDataDistTable.clear();	// ��������Ȃ�����e�[�u���폜
	delete pDta;

	// �؍\���쐬
	ASSERT_THROW( pEngine_->makeClusterTree( static_cast< InterClusterDistanceType >( 99 ) ), BottomUpClusteringEngineException );
	pEngine_->makeClusterTree( WARD );

	// �؍\���̃`�F�b�N
	int n = 0;
	LayerIterator it	= pEngine_->beginClusterTree();
	LayerIterator itEnd	= pEngine_->endClusterTree();
	ASSERT_EQ( 1, it->size() );

	for( ; it != itEnd; ++it )
	{
		++n;
		ASSERT_EQ( n, it->size() );
	}

	ASSERT_EQ( 10, n );	// 0�����ł�10��N���X�^�����O���ꂽ���Ƃ��m�F

	ASSERT_EQ( 1, pEngine_->beginClusterTree()->size() );
	ASSERT_EQ( 19, pEngine_->beginClusterTree()->front()->getId() );

	// �؍\������ɂ���
	pEngine_->clearClusterTree();
	ASSERT_EQ( pEngine_->beginClusterTree(), pEngine_->endClusterTree() );
}


