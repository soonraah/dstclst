/**************************************************************************//**
 *	@file LanceWilliamsUpdatingFormulaTest.cpp
 *****************************************************************************/


#include "LanceWilliamsUpdatingFormulaTest.h"
#include "LanceWilliamsUpdatingFormula.h"


using namespace dstclst;
using namespace std;


//! テスト前に実行
void LanceWilliamsUpdatingFormulaTest::SetUp()
{
}


//! テスト後に実行
void LanceWilliamsUpdatingFormulaTest::TearDown()
{
}


// ----------------------------------------------------------------------------
// 以下、各クラスタ間距離基準のテスト。
// Lance-Willamsの更新式から新しいクラスタ間距離を導く。
// テストデータは doc/TestDataMaking.xls の中で座標から計算して作成した。
// ----------------------------------------------------------------------------

//! 最短距離法のテスト
TEST_F( LanceWilliamsUpdatingFormulaTest, test_nearest ) 
{
	LanceWilliamsUpdatingFormula*	pFormula = new FormulaNearest();
	
	float			dist_1a2	= 10.0f;
	float			dist_1b2	= 10.44030651f;
	float			dist_1a1b	= 3.0f;
	unsigned int	num_1a		= 2;
	unsigned int	num_1b		= 2;
	unsigned int	num_2		= 3;

	float			expected	= 10.0f;
	float			actual		= ( *pFormula )( dist_1a2, dist_1b2, dist_1a1b, num_1a, num_1b, num_2 );
	
	EXPECT_EQ( expected, actual );
}


//! 最長距離法のテスト
TEST_F( LanceWilliamsUpdatingFormulaTest, test_furthest ) 
{
	LanceWilliamsUpdatingFormula*	pFormula = new FormulaFurthest();
	
	float			dist_1a2	= 12.0f;
	float			dist_1b2	= 11.70469991f;
	float			dist_1a1b	= 4.123105626f;
	unsigned int	num_1a		= 2;
	unsigned int	num_1b		= 2;
	unsigned int	num_2		= 3;

	float			expected	= 12.0f;
	float			actual		= ( *pFormula )( dist_1a2, dist_1b2, dist_1a1b, num_1a, num_1b, num_2 );
	
	EXPECT_EQ( expected, actual );
}


//! 群平均法のテスト
TEST_F( LanceWilliamsUpdatingFormulaTest, test_groupAvg ) 
{
	LanceWilliamsUpdatingFormula*	pFormula = new FormulaGroupAvg();
	
	float			dist_1a2	= 10.84920611f;
	float			dist_1b2	= 11.04462663f;
	float			dist_1a1b	= 3.571345821f;
	unsigned int	num_1a		= 2;
	unsigned int	num_1b		= 2;
	unsigned int	num_2		= 3;

	float			expected	= 10.94691637f;
	float			actual		= ( *pFormula )( dist_1a2, dist_1b2, dist_1a1b, num_1a, num_1b, num_2 );
	
	EXPECT_EQ( expected, actual );
}


//! Ward法のテスト
TEST_F( LanceWilliamsUpdatingFormulaTest, test_ward ) 
{
	LanceWilliamsUpdatingFormula*	pFormula = new FormulaWard();
	
	float			dist_1a2	= 140.9666667f;
	float			dist_1b2	= 145.7666667f;
	float			dist_1a1b	= 12.5f;
	unsigned int	num_1a		= 2;
	unsigned int	num_1b		= 2;
	unsigned int	num_2		= 3;

	float			expected	= 199.452381f;
	float			actual		= ( *pFormula )( dist_1a2, dist_1b2, dist_1a1b, num_1a, num_1b, num_2 );
	
	EXPECT_EQ( expected, actual );
	// EXPECT_NEAR( expected, actual, expected * 0.00001 );
}

