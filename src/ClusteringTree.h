/**************************************************************************//**
 *	@file ClusteringTree.h
 *****************************************************************************/


#ifndef _CLUSTERING_TREE_H_
#define _CLUSTERING_TREE_H_


#include <list>
#include <deque>

#include "DistanceTableAccessor.h"
#include "LanceWilliamsUpdatingFormula.h"
#include "ClusterInfo.h"


namespace dstclst
{
	// テスト用クラス（ここでは宣言のみ）
	class ClusteringTreeTest;

	//! ボトムアップ階層クラスタリングの木構造を扱うクラス
	class ClusteringTree
	{
		friend class ClusteringTreeTest;	// friend指定

	private:
		// コピー禁止
		ClusteringTree( const ClusteringTree &other ) {};
		ClusteringTree & operator = ( const ClusteringTree &other ) {};

	public:
		//! コンストラクタ
		ClusteringTree();

		//! デストラクタ
		~ClusteringTree();

		//! クラスタ間距離テーブルに初期値を設定
		/*!
			@param[in] pDtAccessor データ間距離へのアクセス
			@exception std::invalid_argument 引数の値が正しくない場合
			@exception std::bad_alloc メモリ確保に失敗
			@brief この処理が終わるとデータ間距離へのアクセスは不要。削除してよい。
		*/
		void setInitialDistance( const DistanceTableAccessor* pDtAccessor );

		//! 更新式（=クラスタ間距離の定義）を設定
		/*!
			@param[in] pUpdatingFormula 更新式ファンクタへのポインタ
		*/
		void setUpdatingFormula( LanceWilliamsUpdatingFormula* pUpdatingFormula );

		//! クラスタ間距離を調べ、次のクラスタをマージして木構造を1ステップ育てる
		/*!
			@param[in] killChildren trueの場合、マージ後にマージ前のクラスタを削除する
			@return マージコスト
		*/
		float buildOneStep( bool killChildren );

		//! 現在の最上層クラスタを取得
		/*!
			@return 現在の最上層クラスタ
		*/
		const ClusterArray* getTopLayer() const;

		//! クラスタ、クラスタ間距離の情報を削除
		void clear();

	protected:
		//! 最小のクラスタ間距離を探す
		/*!
			@param[out] rowMin 最小クラスタ間距離を示すクラスタの組み合わせの行番号
			@param[out] columnMin 最小クラスタ間距離を示すクラスタの組み合わせの列番号
			@param[out] distanceMin 最小のクラスタ間距離
		*/
		void findMinDistance( unsigned int& rowMin, unsigned int& columnMin, float& distanceMin );

		//! 最上層からクラスタを削除
		/*!
			@param[in] clusterIndex 削除対象のクラスタインデックス
			@exception std::out_of_range 入力インデックスが範囲外
		*/
		void deleteClusterFromTopLayer( unsigned int clusterIndex );

		//! 最上層にクラスタを追加
		/*!
			@param[in] pCluster 追加するクラスタへのポインタ
			@param[in] child1Index 子クラスタ1のインデックス（追加クラスタの距離計算に使用） child1Index < child2Index
			@param[in] child2Index 子クラスタ2のインデックス（追加クラスタの距離計算に使用） child1Index < child2Index
			@exception std::invalid_argument 入力クラスタが無効
			@exception std::bad_alloc メモリ確保失敗
		*/
		void addClusterIntoTopLayer( const ClusterInfo* pCluster, unsigned int child1Index, unsigned int child2Index );

	protected:
		ClusterArray						topLayer_;				//! 最上層のクラスタリスト（クラスタ間距離テーブルの並び順）
		//const DistanceTableAccessor*		pDtAccessor_;			//! データ間距離テーブルへのアクセス機構

		std::vector< std::vector< float > >	interClusterDistance_;	//! 最上層のクラスタ間距離テーブル
		//ClusterArray						tableOrder_;			//! interClusterDistance_の並び順と同期

		LanceWilliamsUpdatingFormula*		pUpdatingFormula_;		//! 更新式ファンクタへのポインタ

		bool								memorySavingMode_;		//! メモリ節約優先の場合true
	};
}


#endif	// _CLUSTERING_TREE_H_
