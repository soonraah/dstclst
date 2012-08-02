/**************************************************************************//**
 *	@file DistanceTableAccessorPrimitive.h
 *****************************************************************************/


#include "DistanceTableAccessorPrimitive.h"


using namespace std;
using namespace dstclst;


//! コンストラクタ
DistanceTableAccessorPrimitive::DistanceTableAccessorPrimitive() : DistanceTableAccessor(), pDistanceTable_( NULL )
{
}


//! デストラクタ
DistanceTableAccessorPrimitive::~DistanceTableAccessorPrimitive()
{
	pDistanceTable_ = NULL;
}


//! 距離テーブルをセット
bool DistanceTableAccessorPrimitive::setDistanceTable( void* pDistanceTable, int param1, int param2 )
{
	const vector< vector< float > >*	p = reinterpret_cast< vector< vector< float > >* >( pDistanceTable );

	// サイズチェック（三角形になっているか）
	try {
		unsigned int size = p->size();
		if( size == 0 ) return false;

		for( unsigned int i = 0; i < size; ++i )
		{
			if( p->at( i ).size() != i ) return false;
		}
	}
	catch( ... )
	{
		return false;
	}

	pDistanceTable_ = p;
	return true;
}


//! テーブルの1辺の長さを取得（要素数）
unsigned int DistanceTableAccessorPrimitive::getDistanceTableSize() const
{
	return pDistanceTable_ == NULL ? 0 : pDistanceTable_->size();
}


//! i番目の要素とj番目の要素の距離を取得
float DistanceTableAccessorPrimitive::getDistanceAt( unsigned int i, unsigned int j ) const
{
	// サイズチェック
	if( i < 0 || getDistanceTableSize() <= i )	throw std::out_of_range( "invalid index" );
	if( j < 0 || getDistanceTableSize() <= j )	throw std::out_of_range( "invalid index" );

	if( i > j )
	{
		return ( *pDistanceTable_ )[ i ][ j ];
	}
	else if( i < j )
	{
		return ( *pDistanceTable_ )[ j ][ i ];
	}
	else	// i == j
	{
		return 0.0f;
	}
}

