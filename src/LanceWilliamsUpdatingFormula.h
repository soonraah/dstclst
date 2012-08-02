/**************************************************************************//**
 *	@file LanceWilliamsUpdatingFormula.h
 *****************************************************************************/


#ifndef _LANCE_WILLIAMS_UPDATING_FORMULA_H_
#define _LANCE_WILLIAMS_UPDATING_FORMULA_H_


namespace dstclst
{
	//! Lance-Williamsの更新式を表す抽象クラス（ファンクタ）
	/*!
		@brief クラスタC1 = C1a∪C1b のとき、クラスタC1とC2距離をマージ前の距離情報から求めることができる。
		       参考 http://ibisforest.org/index.php?Lance-Williams%20updating%20formula
	*/
	class LanceWilliamsUpdatingFormula
	{
	public:
		//! 関数オブジェクトとして扱うための()演算子オーバーロード
		/*!
			@param[in] dist_1a2 C1aとC2間の距離
			@param[in] dist_1b2 C1bとC2間の距離
			@param[in] dist_1a1b C1aとC1b間の距離
			@param[in] num_1a C1aのメンバ数
			@param[in] num_1b C1bのメンバ数
			@param[in] num_2 C2のメンバ数
			@return C1-C2間の距離
		*/
		virtual float operator()(
			float dist_1a2,
			float dist_1b2,
			float dist_1a1b,
			unsigned int num_1a,
			unsigned int num_1b,
			unsigned int num_2
		) const = 0;

	protected:
		//! 更新式の本体
		/*!
			@param[in] dist_1a2 C1aとC2間の距離
			@param[in] dist_1b2 C1bとC2間の距離
			@param[in] dist_1a1b C1aとC1b間の距離
			@param[in] alpha_a 係数αa
			@param[in] alpha_b 係数αb
			@param[in] beta 係数β
			@param[in] gamma 係数γ
			@return C1-C2間の距離
		*/
		inline float formula(
			float dist_1a2,
			float dist_1b2,
			float dist_1a1b,
			float alpha_a,
			float alpha_b,
			float beta,
			float gamma
		) const;
	};

	//! 最短距離法
	class FormulaNearest : public LanceWilliamsUpdatingFormula
	{
	public:
		//! 関数オブジェクトとして扱うための()演算子オーバーロード
		/*!
			@param[in] dist_1a2 C1aとC2間の距離
			@param[in] dist_1b2 C1bとC2間の距離
			@param[in] dist_1a1b C1aとC1b間の距離
			@param[in] num_1a C1aのメンバ数
			@param[in] num_1b C1bのメンバ数
			@param[in] num_2 C2のメンバ数
			@return C1-C2間の距離
		*/
		float operator()(
			float dist_1a2,
			float dist_1b2,
			float dist_1a1b,
			unsigned int num_1a,
			unsigned int num_1b,
			unsigned int num_2
		) const;
	};

	//! 最長距離法
	class FormulaFurthest : public LanceWilliamsUpdatingFormula
	{
	public:
		//! 関数オブジェクトとして扱うための()演算子オーバーロード
		/*!
			@param[in] dist_1a2 C1aとC2間の距離
			@param[in] dist_1b2 C1bとC2間の距離
			@param[in] dist_1a1b C1aとC1b間の距離
			@param[in] num_1a C1aのメンバ数
			@param[in] num_1b C1bのメンバ数
			@param[in] num_2 C2のメンバ数
			@return C1-C2間の距離
		*/
		float operator()(
			float dist_1a2,
			float dist_1b2,
			float dist_1a1b,
			unsigned int num_1a,
			unsigned int num_1b,
			unsigned int num_2
		) const;
	};

	//! 群平均法
	class FormulaGroupAvg : public LanceWilliamsUpdatingFormula
	{
	public:
		//! 関数オブジェクトとして扱うための()演算子オーバーロード
		/*!
			@param[in] dist_1a2 C1aとC2間の距離
			@param[in] dist_1b2 C1bとC2間の距離
			@param[in] dist_1a1b C1aとC1b間の距離
			@param[in] num_1a C1aのメンバ数
			@param[in] num_1b C1bのメンバ数
			@param[in] num_2 C2のメンバ数
			@return C1-C2間の距離
		*/
		float operator()(
			float dist_1a2,
			float dist_1b2,
			float dist_1a1b,
			unsigned int num_1a,
			unsigned int num_1b,
			unsigned int num_2
		) const;
	};

	//! Ward法（推奨）
	class FormulaWard : public LanceWilliamsUpdatingFormula
	{
	public:
		//! 関数オブジェクトとして扱うための()演算子オーバーロード
		/*!
			@param[in] dist_1a2 C1aとC2間の距離
			@param[in] dist_1b2 C1bとC2間の距離
			@param[in] dist_1a1b C1aとC1b間の距離
			@param[in] num_1a C1aのメンバ数
			@param[in] num_1b C1bのメンバ数
			@param[in] num_2 C2のメンバ数
			@return C1-C2間の距離
		*/
		float operator()(
			float dist_1a2,
			float dist_1b2,
			float dist_1a1b,
			unsigned int num_1a,
			unsigned int num_1b,
			unsigned int num_2
		) const;
	};
}



#endif	// _LANCE_WILLIAMS_UPDATING_FORMULA_H_
