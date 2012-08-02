/**************************************************************************//**
 *	@file ClusterInfo.h
 *****************************************************************************/

#ifndef _CLUSTER_INFO_H_
#define _CLUSTER_INFO_H_

#include <vector>
#include <list>

#include "dstclst_param.h"


namespace dstclst
{
	//! クラスタの情報を扱うクラス
	class ClusterInfo
	{
	public:
		//! コンストラクタ
		ClusterInfo();

		//! コンストラクタ（マージ）
		/*!
			@param[in] child1 子クラスタその1
			@param[in] child2 子クラスタその2
			@param[in] mergeCost マージコスト
		*/
		ClusterInfo( const ClusterInfo* child1, const ClusterInfo* child2, float mergeCost );

		//! デストラクタ
		~ClusterInfo();

		//! メンバ追加
		/*!
			@param[in] dataIndex クラスタに追加するデータメンバ
		*/
		void addMember( DataIndex dataIndex );

		//! メンバ追加
		/*!
			@param[in] dataIndexes クラスタに追加するデータメンバ（複数）
		*/
		void addMembers( const std::vector< DataIndex >& dataIndexes );

		//! メンバ取得
		/*!
			@return データメンバへのポインタ
		*/
		const std::vector< DataIndex >* getMembers() const;

		//! マージコストの設定
		/*!
			@param[in] mergeCost クラスタマージ時のコスト
		*/
		void setMergeCost( float mergeCost );

		//! マージコストの取得
		/*!
			@return クラスタマージ時のコスト
		*/
		float getMergeCost() const;

		//! マージ前の子クラスタ1を取得
		/*!
			@return マージ前の子クラスタ1へのポインタ
		*/
		const ClusterInfo* getChild1() const;

		//! マージ前の子クラスタ2を取得
		/*!
			@return マージ前の子クラスタ2へのポインタ
		*/
		const ClusterInfo* getChild2() const;

		//! 子クラスタを設定
		/*!
			@param[in] child1 セットする子クラスタ1
			@param[in] child2 セットする子クラスタ2
		*/
		void setChildren( const ClusterInfo* child1, const ClusterInfo* child2 );

	protected:
		std::vector< DataIndex >	members_;		//! クラスタのデータメンバ
		const ClusterInfo*			child1_;		//! マージ前のクラスタその１
		const ClusterInfo*			child2_;		//! マージ前のクラスタその２
		float						mergeCost_;		//! このクラスタのマージコスト

	public:
		void*						hook_;			//! 追加情報（あれば）　※注意：voidポインタだとデストラクタは呼ばれないので、デストラクタ不要な型に限る。
	};


	//! クラスタの集団
	typedef std::vector< const ClusterInfo* >		ClusterArray;


	//! 比較用ファンクタ
	class PredicateClusterPtr
	{
	public:
		//! 比較用
		/*!
			@param[in] pLeft 左辺のクラスタポインタ
			@param[in] pRight 右辺のクラスタポインタ
			@return L<Rのときにtrueが返るようにすると昇順ソートになる
		*/
		bool operator()( const ClusterInfo* pLeft, const ClusterInfo* pRight ) const;
	};
}


#endif	// _CLUSTER_INFO_H_

