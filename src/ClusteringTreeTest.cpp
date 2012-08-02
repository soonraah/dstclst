/**************************************************************************//**
 *	@file ClusteringTreeTest.cpp
 *****************************************************************************/


#include "ClusteringTreeTest.h"
#include "DistanceTableAccessorPrimitive.h"


using namespace std;
using namespace dstclst;


//! テスト前に実行
void ClusteringTreeTest::SetUp()
{
	// データ間距離（doc/TestDataMaking.xls の "Data_Small" で作ったデータ）
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


//! テスト後に実行
void ClusteringTreeTest::TearDown()
{
	dataDistanceTable_.clear();

	if( pTree_ ) delete pTree_;
}


//! クラスタ間距離テーブルへの初期値設定のテスト
TEST_F( ClusteringTreeTest, test_setInitialDistance )
{
	DistanceTableAccessor*	pAccessor = NULL;

	// 無効なポインタ（NULL）
	pAccessor = NULL;
	ASSERT_THROW( pTree_->setInitialDistance( pAccessor ), std::invalid_argument );
	EXPECT_EQ( 0, pTree_->getTopLayer()->size() );
	EXPECT_EQ( 0, getInterClusterDistance()->size() );
	// EXPECT_EQ( 0, getTableOrder()->size() );

	// 有効なポインタ
	pAccessor = pDtAccessor_;
	pTree_->setInitialDistance( pAccessor );
	EXPECT_EQ( 6, pTree_->getTopLayer()->size() );
	EXPECT_EQ( 6, getInterClusterDistance()->size() );
	// EXPECT_EQ( 6, getTableOrder()->size() );
}


//! クラスタ間距離を調べ、次のクラスタをマージして木構造を1ステップ育てる関数のテスト
TEST_F( ClusteringTreeTest, test_buildOneStep )
{
	// クラスタ間距離テーブルへ初期値設定
	pTree_->setInitialDistance( pDtAccessor_ );
	ASSERT_EQ( 6, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 6, getInterClusterDistance()->size() );
	// ASSERT_EQ( 6, getTableOrder()->size() );

	pTree_->setUpdatingFormula( new FormulaWard() );

	// 1回目
	pTree_->buildOneStep( false );
	ASSERT_EQ( 5, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 5, getInterClusterDistance()->size() );
	// ASSERT_EQ( 5, getTableOrder()->size() );

	ASSERT_EQ( 2, pTree_->getTopLayer()->back()->getMembers()->size() );
	ASSERT_EQ( 0, pTree_->getTopLayer()->back()->getMembers()->at( 0 ) );
	ASSERT_EQ( 1, pTree_->getTopLayer()->back()->getMembers()->at( 1 ) );
	ASSERT_TRUE( pTree_->getTopLayer()->back()->getChild1() != NULL );	// マージ前の子クラスタは残っている
	ASSERT_TRUE( pTree_->getTopLayer()->back()->getChild2() != NULL );
	
	// 2回目
	pTree_->buildOneStep( true );
	ASSERT_EQ( 4, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 4, getInterClusterDistance()->size() );
	// ASSERT_EQ( 4, getTableOrder()->size() );

	ASSERT_EQ( 2, pTree_->getTopLayer()->back()->getMembers()->size() );
	ASSERT_EQ( 2, pTree_->getTopLayer()->back()->getMembers()->at( 0 ) );
	ASSERT_EQ( 3, pTree_->getTopLayer()->back()->getMembers()->at( 1 ) );
	ASSERT_TRUE( pTree_->getTopLayer()->back()->getChild1() == NULL );	// マージ前の子クラスタは殺された
	ASSERT_TRUE( pTree_->getTopLayer()->back()->getChild2() == NULL );
}


//! 連続的に木構造を成長させるテスト
TEST_F( ClusteringTreeTest, test_buildContinuously )
{
	// クラスタ間距離テーブルへ初期値設定
	pTree_->setInitialDistance( pDtAccessor_ );
	ASSERT_EQ( 6, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 6, getInterClusterDistance()->size() );
	// ASSERT_EQ( 6, getTableOrder()->size() );

	pTree_->setUpdatingFormula( new FormulaWard() );

	// 1回目
	pTree_->buildOneStep( false );
	ASSERT_EQ( 5, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 5, getInterClusterDistance()->size() );
	// ASSERT_EQ( 5, getTableOrder()->size() );

	ASSERT_EQ( 2, pTree_->getTopLayer()->back()->getMembers()->size() );
	ASSERT_EQ( 0, pTree_->getTopLayer()->back()->getMembers()->at( 0 ) );
	ASSERT_EQ( 1, pTree_->getTopLayer()->back()->getMembers()->at( 1 ) );
	
	// 2回目
	pTree_->buildOneStep( false );
	ASSERT_EQ( 4, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 4, getInterClusterDistance()->size() );
	// ASSERT_EQ( 4, getTableOrder()->size() );

	ASSERT_EQ( 2, pTree_->getTopLayer()->back()->getMembers()->size() );
	ASSERT_EQ( 2, pTree_->getTopLayer()->back()->getMembers()->at( 0 ) );
	ASSERT_EQ( 3, pTree_->getTopLayer()->back()->getMembers()->at( 1 ) );
	
	// 3回目
	pTree_->buildOneStep( false );
	ASSERT_EQ( 3, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 3, getInterClusterDistance()->size() );
	// ASSERT_EQ( 3, getTableOrder()->size() );

	ASSERT_EQ( 2, pTree_->getTopLayer()->back()->getMembers()->size() );
	ASSERT_EQ( 4, pTree_->getTopLayer()->back()->getMembers()->at( 0 ) );
	ASSERT_EQ( 5, pTree_->getTopLayer()->back()->getMembers()->at( 1 ) );
	
	// 4回目
	pTree_->buildOneStep( false );
	ASSERT_EQ( 2, pTree_->getTopLayer()->size() );
	ASSERT_EQ( 2, getInterClusterDistance()->size() );
	// ASSERT_EQ( 2, getTableOrder()->size() );

	ASSERT_EQ( 4, pTree_->getTopLayer()->back()->getMembers()->size() );
	ASSERT_EQ( 0, pTree_->getTopLayer()->back()->getMembers()->at( 0 ) );
	ASSERT_EQ( 1, pTree_->getTopLayer()->back()->getMembers()->at( 1 ) );
	ASSERT_EQ( 2, pTree_->getTopLayer()->back()->getMembers()->at( 2 ) );
	ASSERT_EQ( 3, pTree_->getTopLayer()->back()->getMembers()->at( 3 ) );
	
	// 5回目
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
	
	// 6回目（余分。木構造は維持）
	ASSERT_EQ( -1.0f, pTree_->buildOneStep( false ) );	// -1を返す
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


//! クラスタ、クラスタ間距離の情報を削除するテスト
TEST_F( ClusteringTreeTest, test_clear )
{
	pTree_->setUpdatingFormula( new FormulaWard() );

	// 最後まで作る
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

	// 途中まで作る
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

	// 途中まで作る（子は残さない）
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

