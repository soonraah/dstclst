/**************************************************************************//**
 *	@file LanceWilliamsUpdatingFormula.h
 *****************************************************************************/


#ifndef _LANCE_WILLIAMS_UPDATING_FORMULA_H_
#define _LANCE_WILLIAMS_UPDATING_FORMULA_H_


namespace dstclst
{
	//! Lance-Williams�̍X�V����\�����ۃN���X�i�t�@���N�^�j
	/*!
		@brief �N���X�^C1 = C1a��C1b �̂Ƃ��A�N���X�^C1��C2�������}�[�W�O�̋�����񂩂狁�߂邱�Ƃ��ł���B
		       �Q�l http://ibisforest.org/index.php?Lance-Williams%20updating%20formula
	*/
	class LanceWilliamsUpdatingFormula
	{
	public:
		//! �֐��I�u�W�F�N�g�Ƃ��Ĉ������߂�()���Z�q�I�[�o�[���[�h
		/*!
			@param[in] dist_1a2 C1a��C2�Ԃ̋���
			@param[in] dist_1b2 C1b��C2�Ԃ̋���
			@param[in] dist_1a1b C1a��C1b�Ԃ̋���
			@param[in] num_1a C1a�̃����o��
			@param[in] num_1b C1b�̃����o��
			@param[in] num_2 C2�̃����o��
			@return C1-C2�Ԃ̋���
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
		//! �X�V���̖{��
		/*!
			@param[in] dist_1a2 C1a��C2�Ԃ̋���
			@param[in] dist_1b2 C1b��C2�Ԃ̋���
			@param[in] dist_1a1b C1a��C1b�Ԃ̋���
			@param[in] alpha_a �W����a
			@param[in] alpha_b �W����b
			@param[in] beta �W����
			@param[in] gamma �W����
			@return C1-C2�Ԃ̋���
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

	//! �ŒZ�����@
	class FormulaNearest : public LanceWilliamsUpdatingFormula
	{
	public:
		//! �֐��I�u�W�F�N�g�Ƃ��Ĉ������߂�()���Z�q�I�[�o�[���[�h
		/*!
			@param[in] dist_1a2 C1a��C2�Ԃ̋���
			@param[in] dist_1b2 C1b��C2�Ԃ̋���
			@param[in] dist_1a1b C1a��C1b�Ԃ̋���
			@param[in] num_1a C1a�̃����o��
			@param[in] num_1b C1b�̃����o��
			@param[in] num_2 C2�̃����o��
			@return C1-C2�Ԃ̋���
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

	//! �Œ������@
	class FormulaFurthest : public LanceWilliamsUpdatingFormula
	{
	public:
		//! �֐��I�u�W�F�N�g�Ƃ��Ĉ������߂�()���Z�q�I�[�o�[���[�h
		/*!
			@param[in] dist_1a2 C1a��C2�Ԃ̋���
			@param[in] dist_1b2 C1b��C2�Ԃ̋���
			@param[in] dist_1a1b C1a��C1b�Ԃ̋���
			@param[in] num_1a C1a�̃����o��
			@param[in] num_1b C1b�̃����o��
			@param[in] num_2 C2�̃����o��
			@return C1-C2�Ԃ̋���
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

	//! �Q���ϖ@
	class FormulaGroupAvg : public LanceWilliamsUpdatingFormula
	{
	public:
		//! �֐��I�u�W�F�N�g�Ƃ��Ĉ������߂�()���Z�q�I�[�o�[���[�h
		/*!
			@param[in] dist_1a2 C1a��C2�Ԃ̋���
			@param[in] dist_1b2 C1b��C2�Ԃ̋���
			@param[in] dist_1a1b C1a��C1b�Ԃ̋���
			@param[in] num_1a C1a�̃����o��
			@param[in] num_1b C1b�̃����o��
			@param[in] num_2 C2�̃����o��
			@return C1-C2�Ԃ̋���
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

	//! Ward�@�i�����j
	class FormulaWard : public LanceWilliamsUpdatingFormula
	{
	public:
		//! �֐��I�u�W�F�N�g�Ƃ��Ĉ������߂�()���Z�q�I�[�o�[���[�h
		/*!
			@param[in] dist_1a2 C1a��C2�Ԃ̋���
			@param[in] dist_1b2 C1b��C2�Ԃ̋���
			@param[in] dist_1a1b C1a��C1b�Ԃ̋���
			@param[in] num_1a C1a�̃����o��
			@param[in] num_1b C1b�̃����o��
			@param[in] num_2 C2�̃����o��
			@return C1-C2�Ԃ̋���
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
