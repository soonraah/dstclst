/**************************************************************************//**
 *	@file DistanceTableAccessor.h
 *****************************************************************************/


#ifndef _DISTANCE_TABLE_ACCESSOR_H_
#define _DISTANCE_TABLE_ACCESSOR_H_


namespace dstclst
{
	//! 距離テーブルにアクセスするための抽象クラス
	class DistanceTableAccessor
	{
	protected:
		DistanceTableAccessor() {};

	public:
		//! デストラクタ
		virtual ~DistanceTableAccessor() {};

		//! 距離テーブルをセット
		/*!
			@param[in] pDistanceTable 距離テーブルへのポインタ
			@param[in] param1 パラメータその1
			@param[in] param2 パラメータその2
			@return 設定に成功したらtrue
		*/
		virtual bool setDistanceTable( void* pDistanceTable, int param1, int param2 ) = 0;

		//! テーブルの1辺の長さを取得（要素数）
		/*!
			@return テーブルの1辺の長さ
		*/
		virtual unsigned int getDistanceTableSize() const = 0;

		//! i番目の要素とj番目の要素の距離を取得
		/*!
			@param[in] i 行番号
			@param[in] j 列番号
			@return i番目の要素とj番目の要素の距離
			@exception std::out_of_range iまたはjがテーブルサイズの範囲外だった場合
		*/
		virtual float getDistanceAt( unsigned int i, unsigned int j ) const = 0;
	};
}


#endif	// _DISTANCE_TABLE_ACCESSOR_H_
