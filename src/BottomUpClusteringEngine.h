/**************************************************************************//**
 *	@file BottomUpClusteringEngine.h
 *	@brief 
 *****************************************************************************/

#ifndef _BOTTOM_UP_CLUSTERING_ENGINGE_H_
#define _BOTTOM_UP_CLUSTERING_ENGINGE_H_


#include "DistanceTableAccessor.h"
#include "ClusteringTree.h"
#include "LayerIterator.h"


namespace dstclst
{
	//! クラスタ間距離関数種別
	enum InterClusterDistanceType {
		NEAREST,		// 最短距離法
		FURTHEST,		// 最長距離法
		GROUP_AVG,		// 群平均法
		WARD,			// ウォード法（おすすめ）
		CENTROID,		// セントロイド法
		MEDIAN			// メジアン法
	};

	//! ボトムアップクラスタリングを管理するクラス
	class BottomUpClusteringEngine
	{
	private:
		// コピー禁止
		BottomUpClusteringEngine( const BottomUpClusteringEngine &other );
		BottomUpClusteringEngine & operator = ( const BottomUpClusteringEngine &other );

	public:
		//! コンストラクタ
		BottomUpClusteringEngine();

		//! デストラクタ
		~BottomUpClusteringEngine();

		//! データ間距離情報により木構造を初期化
		/*!
			@param[in] pDtAccessor データ間距離テーブルへのアクセスのインスタンス
			@exception BottomUpClusteringEngineException 失敗した場合
			@brief この処理が終わればデータ間距離へのアクセスは不要
		*/
		void initTreeByInterDataDistance( const DistanceTableAccessor* pDtAccessor );

		//! ボトムアップクラスタリングの木構造（デンドログラム）を作成
		/*!
			@param[in] icdType 使用するクラスタ間距離関数の種別
			@exception BottomUpClusteringEngineException 失敗した場合
		*/
		void makeClusterTree( InterClusterDistanceType icdType = WARD );

		//! 木構造を空にする
		/*!
		*/
		void clearClusterTree();

		//! 木構造を上から階層的にサーチするイタレータを生成
		/*!
			@return 先頭イタレータ（階層構造の頂上、クラスタ数は1の箇所）
		*/
		LayerIterator beginClusterTree() const;

		//! 終端イタレータを生成
		/*!
			@return 終端イタレータ（階層構造の底辺の下、クラスタ数=データ数となった後）
		*/
		LayerIterator endClusterTree() const;

	protected:
		//! クラスタリング処理
		/*!
			@param[in] icdType 使用するクラスタ間距離関数の種別
			@param[in] thresholdNumClusters クラスタ数の閾値。クラスタ数がこの数になるとクラスタリングを終了する。0のときはクラスタ数による制限なし。
			@param[in] thresholdMergeCost クラスタマージコストの閾値。マージコストがこの値以上になるとクラスタリングを終了する。0のときはマージコストによる制限なし。
			@param[in] removeMergedChildren trueだとマージ後にマージ前の2クラスタが削除される。メモリ節約になるが、木構造が作られない。
		*/
		void doClustering(
			InterClusterDistanceType icdType,
			unsigned int thresholdNumClusters = 0,
			float thresholdMergeCost = 0.0f,
			bool removeMergedChildren = false
		);
		
	protected:
		//DistanceTableAccessor	*pDtAccessor_;		//! 距離テーブルへのアクセス機構
		ClusteringTree			*pTree_;			//! 木構造管理のオブジェクトへのポインタ

	};

	//! 例外クラス
	class BottomUpClusteringEngineException : public std::exception
	{
	public:
		//! コンストラクタ
		BottomUpClusteringEngineException()													: std::exception() {};

		//! コンストラクタ
		BottomUpClusteringEngineException( const char* const& msg )							: std::exception( msg ) {};

		//! コンストラクタ
		BottomUpClusteringEngineException( const char* const& msg, int errId )				: std::exception( msg, errId ) {};

		//! コンストラクタ
		BottomUpClusteringEngineException( const BottomUpClusteringEngineException& other )	: std::exception( other ) {};
	};
}

#endif	// _BOTTOM_UP_CLUSTERING_ENGINGE_H_
