/**************************************************************************//**
 *	@file BottomUpClusteringEngineTest.cpp
 *	@brief 
 *****************************************************************************/


#include "BottomUpClusteringEngineTest.h"
#include "TestDataCreation.h"
#include "DistanceTableAccessorPrimitive.h"


using namespace std;
using namespace dstclst;


//! テスト前に実行
void BottomUpClusteringEngineTest::SetUp()
{
	pEngine_ = new BottomUpClusteringEngine();
}


//! テスト後に実行
void BottomUpClusteringEngineTest::TearDown()
{
	if( pEngine_ ) delete pEngine_;
}


//! データ間距離情報による木構造を初期化のテスト
TEST_F( BottomUpClusteringEngineTest, test_initTreeByInterDataDistance )
{
	DistanceTableAccessor*	pDta = NULL;

	// NULL
	ASSERT_THROW( pEngine_->initTreeByInterDataDistance( pDta ), BottomUpClusteringEngineException );

	// テーブルがまだ空
	pDta = new DistanceTableAccessorPrimitive();
	ASSERT_THROW( pEngine_->initTreeByInterDataDistance( pDta ), BottomUpClusteringEngineException );

	// データ間距離情報の用意
	vector< vector< float > >	interDataDistTable;
	setDataDistance1( interDataDistTable );
	pDta->setDistanceTable( &interDataDistTable, 0, 0 );

	// 木構造初期化
	pEngine_->initTreeByInterDataDistance( pDta );
	interDataDistTable.clear();	// もういらないからテーブル削除
	delete pDta;

	// データ数分のクラスタがある状態
	ASSERT_EQ( 1000, pEngine_->beginClusterTree()->size() );
}


//! 木構造作成の一連の流れをテスト
TEST_F( BottomUpClusteringEngineTest, test_makeTreeWithLargeData )
{
	// データ間距離情報の用意
	DistanceTableAccessor*	pDta = new DistanceTableAccessorPrimitive();
	vector< vector< float > >	interDataDistTable;
	setDataDistance1( interDataDistTable );
	pDta->setDistanceTable( &interDataDistTable, 0, 0 );

	// 木構造初期化
	pEngine_->initTreeByInterDataDistance( pDta );
	interDataDistTable.clear();	// もういらないからテーブル削除
	delete pDta;

	// 木構造作成
	ASSERT_THROW( pEngine_->makeClusterTree( static_cast< InterClusterDistanceType >( 99 ) ), BottomUpClusteringEngineException );
	pEngine_->makeClusterTree( WARD );

	// 木構造のチェック
	int n = 0;
	LayerIterator it	= pEngine_->beginClusterTree();
	LayerIterator itEnd	= pEngine_->endClusterTree();
	ASSERT_EQ( 1, it->size() );

	for( ; it != itEnd; ++it )
	{
		++n;
		if( n == 4 )	// クラスタ数が4のとき
		{
			ASSERT_EQ( 4, it->size() );

			// クラスタリングが正しく行われていれば、0～249, 250～499, 500～749, 750～999 の4つのクラスタに分かれているはず
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

	// 木構造を空にする
	pEngine_->clearClusterTree();
	ASSERT_EQ( pEngine_->beginClusterTree(), pEngine_->endClusterTree() );
}


//! 距離0の関係を含むデータから木構造作成の一連の流れをテスト
TEST_F( BottomUpClusteringEngineTest, test_makeTreeWithZeroDistance )
{
	// データ間距離情報の用意
	DistanceTableAccessor*	pDta = new DistanceTableAccessorPrimitive();
	vector< vector< float > >	interDataDistTable;
	setDataDistance2( interDataDistTable );
	pDta->setDistanceTable( &interDataDistTable, 0, 0 );

	// 木構造初期化
	pEngine_->initTreeByInterDataDistance( pDta );
	interDataDistTable.clear();	// もういらないからテーブル削除
	delete pDta;

	// 木構造作成
	ASSERT_THROW( pEngine_->makeClusterTree( static_cast< InterClusterDistanceType >( 99 ) ), BottomUpClusteringEngineException );
	pEngine_->makeClusterTree( WARD );

	// 木構造のチェック
	int n = 0;
	LayerIterator it	= pEngine_->beginClusterTree();
	LayerIterator itEnd	= pEngine_->endClusterTree();
	ASSERT_EQ( 1, it->size() );

	for( ; it != itEnd; ++it )
	{
		++n;
		ASSERT_EQ( n, it->size() );
	}

	ASSERT_EQ( 10, n );	// 0距離でも10回クラスタリングされたことを確認

	ASSERT_EQ( 1, pEngine_->beginClusterTree()->size() );
	ASSERT_EQ( 19, pEngine_->beginClusterTree()->front()->getId() );

	// 木構造を空にする
	pEngine_->clearClusterTree();
	ASSERT_EQ( pEngine_->beginClusterTree(), pEngine_->endClusterTree() );
}


