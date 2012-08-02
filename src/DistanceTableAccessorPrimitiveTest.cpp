/**************************************************************************//**
 *	@file DistanceTableAccessorPrimitiveTest.cpp
 *****************************************************************************/


#include <gtest/gtest.h>

#include "DistanceTableAccessorPrimitiveTest.h"
#include "DistanceTableAccessorPrimitive.h"


using namespace dstclst;
using namespace std;


//! テスト前に実行
void DistanceTableAccessorPrimitiveTest::SetUp()
{
	// N = 5
	// i==jの箇所は距離0なのでデータを持たない
	// 
	//		0			1			2			3			4
	// 0	
	// 1	6.598335462
	// 2	1.050711254	4.844143443
	// 3	3.130328996	1.891655770	0.512448735
	// 4	3.452318159	8.023714750	5.968798042	5.340709171

	distTblCorrect_.resize( 5, vector< float >() );
	distTblCorrect_[ 1 ].push_back( 6.598335462f );
	distTblCorrect_[ 2 ].push_back( 1.050711254f );
	distTblCorrect_[ 2 ].push_back( 4.844143443f );
	distTblCorrect_[ 3 ].push_back( 3.130328996f );
	distTblCorrect_[ 3 ].push_back( 1.891655770f );
	distTblCorrect_[ 3 ].push_back( 0.512448735f );
	distTblCorrect_[ 4 ].push_back( 3.452318159f );
	distTblCorrect_[ 4 ].push_back( 8.023714750f );
	distTblCorrect_[ 4 ].push_back( 5.968798042f );
	distTblCorrect_[ 4 ].push_back( 5.340709171f );
}


//! テスト後に実行
void DistanceTableAccessorPrimitiveTest::TearDown()
{
	distTblCorrect_.clear();
}


//! 距離テーブルをセットするテスト
TEST_F( DistanceTableAccessorPrimitiveTest, test_setDistanceTable )
{
	DistanceTableAccessor* pAccessor = new DistanceTableAccessorPrimitive();

	vector< vector< float > > distTblError;

	// 空のテーブル
	distTblError.clear();
	ASSERT_FALSE( pAccessor->setDistanceTable( &distTblError, 0, 0 ) );

	// 正方形のテーブル
	distTblError.clear();
	distTblError.resize( 10 );
	for( vector< vector< float > >::iterator it = distTblError.begin(); it != distTblError.end(); ++it )
	{
		it->resize( 10, 1.234f );
	}
	ASSERT_FALSE( pAccessor->setDistanceTable( &distTblError, 0, 0 ) );

	// 三角形からはずれた形のテーブル
	distTblError.clear();
	distTblError = distTblCorrect_;
	distTblError.at( 4 ).push_back( 1.234f );
	ASSERT_FALSE( pAccessor->setDistanceTable( &distTblError, 0, 0 ) );

	// 三角形だが、対角線（i==j）に値を持つテーブル
	distTblError.clear();
	distTblError = distTblCorrect_;
	for( vector< vector< float > >::iterator it = distTblError.begin(); it != distTblError.end(); ++it )
	{
		it->push_back( 0.0f );	// i == j
	}
	ASSERT_FALSE( pAccessor->setDistanceTable( &distTblError, 0, 0 ) );

	// 正しいテーブル
	ASSERT_TRUE( pAccessor->setDistanceTable( &distTblCorrect_, 0, 0 ) );
}


//! テーブルの1辺の長さを取得（要素数）するテスト
TEST_F( DistanceTableAccessorPrimitiveTest, test_getDistanceTableSize )
{
	DistanceTableAccessor* pAccessor = new DistanceTableAccessorPrimitive();

	pAccessor->setDistanceTable( &distTblCorrect_, 0, 0 );

	ASSERT_EQ( 5, pAccessor->getDistanceTableSize() );
}


//! i番目の要素とj番目の要素の距離を取得するテスト
TEST_F( DistanceTableAccessorPrimitiveTest, test_getDistanceAt )
{
	DistanceTableAccessor* pAccessor = new DistanceTableAccessorPrimitive();

	pAccessor->setDistanceTable( &distTblCorrect_, 0, 0 );

	ASSERT_EQ( 0.000000000f, pAccessor->getDistanceAt( 0, 0 ) );
	ASSERT_EQ( 6.598335462f, pAccessor->getDistanceAt( 0, 1 ) );
	ASSERT_EQ( 1.050711254f, pAccessor->getDistanceAt( 0, 2 ) );
	ASSERT_EQ( 3.130328996f, pAccessor->getDistanceAt( 0, 3 ) );
	ASSERT_EQ( 3.452318159f, pAccessor->getDistanceAt( 0, 4 ) );
	ASSERT_EQ( 6.598335462f, pAccessor->getDistanceAt( 1, 0 ) );
	ASSERT_EQ( 0.000000000f, pAccessor->getDistanceAt( 1, 1 ) );
	ASSERT_EQ( 4.844143443f, pAccessor->getDistanceAt( 1, 2 ) );
	ASSERT_EQ( 1.891655770f, pAccessor->getDistanceAt( 1, 3 ) );
	ASSERT_EQ( 8.023714750f, pAccessor->getDistanceAt( 1, 4 ) );
	ASSERT_EQ( 1.050711254f, pAccessor->getDistanceAt( 2, 0 ) );
	ASSERT_EQ( 4.844143443f, pAccessor->getDistanceAt( 2, 1 ) );
	ASSERT_EQ( 0.000000000f, pAccessor->getDistanceAt( 2, 2 ) );
	ASSERT_EQ( 0.512448735f, pAccessor->getDistanceAt( 2, 3 ) );
	ASSERT_EQ( 5.968798042f, pAccessor->getDistanceAt( 2, 4 ) );
	ASSERT_EQ( 3.130328996f, pAccessor->getDistanceAt( 3, 0 ) );
	ASSERT_EQ( 1.891655770f, pAccessor->getDistanceAt( 3, 1 ) );
	ASSERT_EQ( 0.512448735f, pAccessor->getDistanceAt( 3, 2 ) );
	ASSERT_EQ( 0.000000000f, pAccessor->getDistanceAt( 3, 3 ) );
	ASSERT_EQ( 5.340709171f, pAccessor->getDistanceAt( 3, 4 ) );
	ASSERT_EQ( 3.452318159f, pAccessor->getDistanceAt( 4, 0 ) );
	ASSERT_EQ( 8.023714750f, pAccessor->getDistanceAt( 4, 1 ) );
	ASSERT_EQ( 5.968798042f, pAccessor->getDistanceAt( 4, 2 ) );
	ASSERT_EQ( 5.340709171f, pAccessor->getDistanceAt( 4, 3 ) );
	ASSERT_EQ( 0.000000000f, pAccessor->getDistanceAt( 4, 4 ) );

	// インデックスが範囲外
	ASSERT_THROW( pAccessor->getDistanceAt( 4, 5 ), std::out_of_range );
	ASSERT_THROW( pAccessor->getDistanceAt( -1, 0 ), std::out_of_range );
}

