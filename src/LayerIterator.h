/**************************************************************************//**
 *	@file LayerIterator.h
 *****************************************************************************/

#ifndef _HIERARCHICAL_ITERATOR_H_
#define _HIERARCHICAL_ITERATOR_H_


#include <iterator>
#include <list>

#include "ClusterInfo.h"


namespace dstclst
{
	//! クラスタリングの木構造を上（クラスタ数 = 1）から順に見ていくイタレータクラス
	class LayerIterator : public std::iterator< std::input_iterator_tag, ClusterArray, void >
	{
	public:
		//! コンストラクタ（終端）
		LayerIterator();

		//! コンストラクタ
		LayerIterator( const ClusterArray& topLayer );

		//! コピーコンストラクタ
		LayerIterator( const LayerIterator& other );

		//! デストラクタ
		~LayerIterator();

		//! 代入演算子
		LayerIterator& operator=( const LayerIterator& other );

		//! 実態参照
		ClusterArray& operator *();

		//! メンバアクセス演算子
		ClusterArray* operator->();

		//! メンバアクセス演算子
		const ClusterArray* operator->() const;

		//! 前置インクリメント
		LayerIterator& operator ++();

		//! 後置インクリメント
		LayerIterator operator ++( int );

		//! 比較演算子
		const bool operator!=( const LayerIterator& other ) const;

		//! 比較演算子
		const bool operator==( const LayerIterator& other ) const;

		//! イタレータが指し示す木構造の現在の階層のマージコストを取得
		/*!
			@return マージコスト
		*/
		float getLayerCost() const;

	protected:
		ClusterArray	curLayer_;
	};

	//const bool operator!=( const LayerIterator& it1, const LayerIterator& it2 );
}


#endif	// _HIERARCHICAL_ITERATOR_H_
