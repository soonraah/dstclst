/**************************************************************************//**
 *	@file ClusteringTreeTest.cpp
 *****************************************************************************/


#include "ClusteringTreeTest.h"
#include "DistanceTableAccessorPrimitive.h"


using namespace std;
using namespace dstclst;


//! �e�X�g�O�Ɏ��s
void ClusteringTreeTest::SetUp()
{
	// �f�[�^�ԋ����idoc/TestDataMaking.xls �� "Data_Small" �ō�����f�[�^�j
	dataDistanceTable_.resize( 6 );
	dataDistanceTable_[ 0 ].clear();
	dataDistanceTable_[ 1 ].resize( 1 );
	dataDistanceTable_[ 2 ].resize( 2 );
	dataDistanceTable_[ 3 ].resize( 3 );
	dataDistanceTable_[ 4 ].resize( 4 );
	dataDistanceTable_[ 5 ].resize( 5 );

	dataDistanceTable_[ 1 ][ 0 ] = 0.1f;
	dataDistanceTable_[ 2 ][ 0 ] = 8.0f;
	dataDistanceTable_[ 2 ][ 1 ] = 8.1f;
	dataDistanceTable_[ 3 ][ 0 ] = 8.015609771f;
	dataDistanceTable_[ 3 ][ 1 ] = 8.115417426f;
	dataDistanceTable_[ 3 ][ 2 ] = 0.5f;
	dataDistanceTable_[ 4 ][ 0 ] = 12.80624847f;
	dataDistanceTable_[ 4 ][ 1 ] = 12.86895489f;
	dataDistanceTable_[ 4 ][ 2 ] = 10.0f;
	dataDistanceTable_[ 4 ][ 3 ] = 9.5f;
	dataDistanceTable_[ 5 ][ 0 ] = 12.20655562f;
	dataDistanceTable_[ 5 ][ 1 ] = 12.26417547f;
	dataDistanceTable_[ 5 ][ 2 ] = 10.04987562f;
	dataDistanceTable_[ 5 ][ 3 ] = 9.552486587f;
	dataDistanceTable_[ 5 ][ 4 ] = 1.0f;

	pDtAccessor_ = new DistanceTableAccessorPrimitive();
	pDtAccessor_->setDistanceTable( &dataDistanceTable_, 0, 0 );

	pTree_ = new ClusteringTree();
}


//! �e�X�g��Ɏ��s
void ClusteringTreeTest::TearDown()
{
	dataDistanceTable_.clear();

	if( pTree_ ) delete pTree_;
}


//! �N���X�^�ԋ����e�[�u���ւ̏����l�ݒ�̃e�X�g
TEST_F( ClusteringTreeTest, test_setInitialDistance )
{
	DistanceTableAccessor*	pAccessor = NULL;

	// �����ȃ|�C���^�iNULL�j
	pAccessor = NULL;
	ASSERT_THROW( pTree_->setInitialDistance( pAccessor ), std::invalid_argument );
	EXPECT_EQ( 0, pTree_->getTopLayer()->size() );
	EXPECT_EQ( 0, getInterClusterDistance()->size() );
	// EXPECT_EQ( 0, getTableOrder()->size() );

	// �L���ȃ|�C���^
	pAccessor = pDtAccessor_;
	pTree_->setInitialDistance( pAccessor );
	EXPECT_EQ( 6, pTree_->getTopLayer()->size() );
	EXPECT_EQ( 6, getInterClusterDistance()->size() );
	// EXPECT_EQ( 6, getTableOrder()->size() );
}


//! �N���X�^�ԋ����𒲂ׁA���̃N���X�^���}�[�W���Ė؍\����1�X�e�b�v��Ă�֐��̃e�X�g
TEST_F( ClusteringTreeTest, test_buildOneStep )
{
	// �N���X�^�ԋ����e�[�u���֏����l�ݒ�
	pTree_->setInitialDistance( pDtAccessor_ );
	ASSERT_EQ( 6, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 6, getInterClusterDistance()->size() );
	// ASSERT_EQ( 6, getTableOrder()->size() );

	pTree_->setUpdatingFormula( new FormulaWard() );

	// 1���
	pTree_->buildOneStep( false );
	ASSERT_EQ( 5, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 5, getInterClusterDistance()->size() );
	// ASSERT_EQ( 5, getTableOrder()->size() );

	ASSERT_EQ( 2, pTree_->getTopLayer()->back()->getMembers()->size() );
	ASSERT_EQ( 0, pTree_->getTopLayer()->back()->getMembers()->at( 0 ) );
	ASSERT_EQ( 1, pTree_->getTopLayer()->back()->getMembers()->at( 1 ) );
	ASSERT_TRUE( pTree_->getTopLayer()->back()->getChild1() != NULL );	// �}�[�W�O�̎q�N���X�^�͎c���Ă���
	ASSERT_TRUE( pTree_->getTopLayer()->back()->getChild2() != NULL );
	
	// 2���
	pTree_->buildOneStep( true );
	ASSERT_EQ( 4, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 4, getInterClusterDistance()->size() );
	// ASSERT_EQ( 4, getTableOrder()->size() );

	ASSERT_EQ( 2, pTree_->getTopLayer()->back()->getMembers()->size() );
	ASSERT_EQ( 2, pTree_->getTopLayer()->back()->getMembers()->at( 0 ) );
	ASSERT_EQ( 3, pTree_->getTopLayer()->back()->getMembers()->at( 1 ) );
	ASSERT_TRUE( pTree_->getTopLayer()->back()->getChild1() == NULL );	// �}�[�W�O�̎q�N���X�^�͎E���ꂽ
	ASSERT_TRUE( pTree_->getTopLayer()->back()->getChild2() == NULL );
}


//! �A���I�ɖ؍\���𐬒�������e�X�g
TEST_F( ClusteringTreeTest, test_buildContinuously )
{
	// �N���X�^�ԋ����e�[�u���֏����l�ݒ�
	pTree_->setInitialDistance( pDtAccessor_ );
	ASSERT_EQ( 6, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 6, getInterClusterDistance()->size() );
	// ASSERT_EQ( 6, getTableOrder()->size() );

	pTree_->setUpdatingFormula( new FormulaWard() );

	// 1���
	pTree_->buildOneStep( false );
	ASSERT_EQ( 5, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 5, getInterClusterDistance()->size() );
	// ASSERT_EQ( 5, getTableOrder()->size() );

	ASSERT_EQ( 2, pTree_->getTopLayer()->back()->getMembers()->size() );
	ASSERT_EQ( 0, pTree_->getTopLayer()->back()->getMembers()->at( 0 ) );
	ASSERT_EQ( 1, pTree_->getTopLayer()->back()->getMembers()->at( 1 ) );
	
	// 2���
	pTree_->buildOneStep( false );
	ASSERT_EQ( 4, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 4, getInterClusterDistance()->size() );
	// ASSERT_EQ( 4, getTableOrder()->size() );

	ASSERT_EQ( 2, pTree_->getTopLayer()->back()->getMembers()->size() );
	ASSERT_EQ( 2, pTree_->getTopLayer()->back()->getMembers()->at( 0 ) );
	ASSERT_EQ( 3, pTree_->getTopLayer()->back()->getMembers()->at( 1 ) );
	
	// 3���
	pTree_->buildOneStep( false );
	ASSERT_EQ( 3, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 3, getInterClusterDistance()->size() );
	// ASSERT_EQ( 3, getTableOrder()->size() );

	ASSERT_EQ( 2, pTree_->getTopLayer()->back()->getMembers()->size() );
	ASSERT_EQ( 4, pTree_->getTopLayer()->back()->getMembers()->at( 0 ) );
	ASSERT_EQ( 5, pTree_->getTopLayer()->back()->getMembers()->at( 1 ) );
	
	// 4���
	pTree_->buildOneStep( false );
	ASSERT_EQ( 2, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 2, getInterClusterDistance()->size() );
	// ASSERT_EQ( 2, getTableOrder()->size() );

	ASSERT_EQ( 4, pTree_->getTopLayer()->back()->getMembers()->size() );
	ASSERT_EQ( 0, pTree_->getTopLayer()->back()->getMembers()->at( 0 ) );
	ASSERT_EQ( 1, pTree_->getTopLayer()->back()->getMembers()->at( 1 ) );
	ASSERT_EQ( 2, pTree_->getTopLayer()->back()->getMembers()->at( 2 ) );
	ASSERT_EQ( 3, pTree_->getTopLayer()->back()->getMembers()->at( 3 ) );
	
	// 5���
	pTree_->buildOneStep( false );
	ASSERT_EQ( 1, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 1, getInterClusterDistance()->size() );
	// ASSERT_EQ( 1, getTableOrder()->size() );

	ASSERT_EQ( 6, pTree_->getTopLayer()->back()->getMembers()->size() );
	ASSERT_EQ( 0, pTree_->getTopLayer()->back()->getMembers()->at( 0 ) );
	ASSERT_EQ( 1, pTree_->getTopLayer()->back()->getMembers()->at( 1 ) );
	ASSERT_EQ( 2, pTree_->getTopLayer()->back()->getMembers()->at( 2 ) );
	ASSERT_EQ( 3, pTree_->getTopLayer()->back()->getMembers()->at( 3 ) );
	ASSERT_EQ( 4, pTree_->getTopLayer()->back()->getMembers()->at( 4 ) );
	ASSERT_EQ( 5, pTree_->getTopLayer()->back()->getMembers()->at( 5 ) );
	
	// 6��ځi�]���B�؍\���͈ێ��j
	ASSERT_EQ( -1.0f, pTree_->buildOneStep( false ) );	// -1��Ԃ�
	ASSERT_EQ( 1, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 1, getInterClusterDistance()->size() );
	// ASSERT_EQ( 1, getTableOrder()->size() );

	ASSERT_EQ( 6, pTree_->getTopLayer()->back()->getMembers()->size() );
	ASSERT_EQ( 0, pTree_->getTopLayer()->back()->getMembers()->at( 0 ) );
	ASSERT_EQ( 1, pTree_->getTopLayer()->back()->getMembers()->at( 1 ) );
	ASSERT_EQ( 2, pTree_->getTopLayer()->back()->getMembers()->at( 2 ) );
	ASSERT_EQ( 3, pTree_->getTopLayer()->back()->getMembers()->at( 3 ) );
	ASSERT_EQ( 4, pTree_->getTopLayer()->back()->getMembers()->at( 4 ) );
	ASSERT_EQ( 5, pTree_->getTopLayer()->back()->getMembers()->at( 5 ) );
}


//! �N���X�^�A�N���X�^�ԋ����̏����폜����e�X�g
TEST_F( ClusteringTreeTest, test_clear )
{
	pTree_->setUpdatingFormula( new FormulaWard() );

	// �Ō�܂ō��
	pTree_->setInitialDistance( pDtAccessor_ );
	ASSERT_EQ( 6, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 6, getInterClusterDistance()->size() );
	while( pTree_->buildOneStep( false ) > 0.0f ) {};
	ASSERT_EQ( 1, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 1, getInterClusterDistance()->size() );
	
	// clear
	pTree_->clear();
	ASSERT_EQ( 0, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 0, getInterClusterDistance()->size() );
	ASSERT_EQ( 0, pTree_->getTopLayer()->capacity() );
	ASSERT_EQ( 0, getInterClusterDistance()->capacity() );

	// �r���܂ō��
	pTree_->setInitialDistance( pDtAccessor_ );
	ASSERT_EQ( 6, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 6, getInterClusterDistance()->size() );
	pTree_->buildOneStep( false );
	pTree_->buildOneStep( false );
	pTree_->buildOneStep( false );
	ASSERT_EQ( 3, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 3, getInterClusterDistance()->size() );
	
	// clear
	pTree_->clear();
	ASSERT_EQ( 0, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 0, getInterClusterDistance()->size() );
	ASSERT_EQ( 0, pTree_->getTopLayer()->capacity() );
	ASSERT_EQ( 0, getInterClusterDistance()->capacity() );

	// �r���܂ō��i�q�͎c���Ȃ��j
	pTree_->setInitialDistance( pDtAccessor_ );
	ASSERT_EQ( 6, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 6, getInterClusterDistance()->size() );
	pTree_->buildOneStep( true );
	pTree_->buildOneStep( true );
	pTree_->buildOneStep( true );
	ASSERT_EQ( 3, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 3, getInterClusterDistance()->size() );

	// clear
	pTree_->clear();
	ASSERT_EQ( 0, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 0, getInterClusterDistance()->size() );
	ASSERT_EQ( 0, pTree_->getTopLayer()->capacity() );
	ASSERT_EQ( 0, getInterClusterDistance()->capacity() );
}

