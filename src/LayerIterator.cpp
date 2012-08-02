/**************************************************************************//**
 *	@file LayerIterator.cpp
 *****************************************************************************/


#include <algorithm>
#include "LayerIterator.h"


using namespace std;
using namespace dstclst;


//! curLayer_の状況を一歩前進
/*!
	@param[in,out] cur 更新対象の現在のレイヤー
	@return 既にボトムまでたどり着いている場合はfalse
*/
bool frontStep( ClusterArray& cur )
{
	if( cur.empty() ) return false;

	const ClusterInfo*	p1 = cur[ 0 ]->getChild1();
	const ClusterInfo*	p2 = cur[ 0 ]->getChild2();

	if( p1 == NULL || p2 == NULL )
	{
		cur.clear();
		return false;
	}

	cur.erase( cur.begin() );	// 親を削除
	cur.push_back( p1 );		// 子を追加
	cur.push_back( p2 );		// 子を追加

	// マージコスト降順でソート
	sort( cur.begin(), cur.end(), PredicateClusterPtr() );

	return true;
}


//! コンストラクタ（終端）
LayerIterator::LayerIterator() : curLayer_( 0 )
{
}


//! コンストラクタ
LayerIterator::LayerIterator( const ClusterArray& topLayer ) : curLayer_( topLayer )
{
	sort( curLayer_.begin(), curLayer_.end(), PredicateClusterPtr() );
}


//! コピーコンストラクタ
LayerIterator::LayerIterator( const LayerIterator& other )
{
	*this = other;
}


//! デストラクタ
LayerIterator::~LayerIterator()
{
	curLayer_.clear();
}


//! 代入演算子
LayerIterator& LayerIterator::operator=( const LayerIterator& other )
{
	curLayer_.clear();
	curLayer_.reserve( other->size() );
	for( ClusterArray::const_iterator it = other->begin(); it != other->end(); ++it )
	{
		curLayer_.push_back( *it );
	}

	return *this;
}


//! 実態参照
ClusterArray& LayerIterator::operator *()
{
	return curLayer_;
}


//! メンバアクセス演算子
ClusterArray* LayerIterator::operator->()
{
	return &curLayer_;
}


//! メンバアクセス演算子
const ClusterArray* LayerIterator::operator->() const
{
	return &curLayer_;
}


//! 前置インクリメント
LayerIterator& LayerIterator::operator ++()
{
	frontStep( curLayer_ );
	return *this;
}


//! 後置インクリメント
LayerIterator LayerIterator::operator ++( int )
{
	LayerIterator temp( *this );
	frontStep( curLayer_ );
	return temp;
}


//! 比較演算子
const bool LayerIterator::operator!=( const LayerIterator& other ) const
{
	return !( *this == other );
}


//! 比較演算子
const bool LayerIterator::operator==( const LayerIterator& other ) const
{
	if( curLayer_.size() != other->size() ) return false;

	ClusterArray::const_iterator itThis = ( *this )->begin();
	ClusterArray::const_iterator itOther = other->begin();

	for( ; itThis != ( *this )->end(); ++itThis, ++itOther )
	{
		if( *itThis != *itOther ) return false;
	}

	return true;
}


//! イタレータが指し示す木構造の現在の階層のマージコストを取得
float LayerIterator::getLayerCost() const
{
	if( curLayer_.empty() )
	{
		return -1.0f;
	}
	else
	{
		return curLayer_[ 0 ]->getMergeCost();
	}
}


