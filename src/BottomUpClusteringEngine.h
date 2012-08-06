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
	//! �N���X�^�ԋ����֐����
	enum InterClusterDistanceType {
		NEAREST,		// �ŒZ�����@
		FURTHEST,		// �Œ������@
		GROUP_AVG,		// �Q���ϖ@
		WARD,			// �E�H�[�h�@�i�������߁j
		CENTROID,		// �Z���g���C�h�@
		MEDIAN			// ���W�A���@
	};

	//! �{�g���A�b�v�N���X�^�����O���Ǘ�����N���X
	class BottomUpClusteringEngine
	{
	private:
		// �R�s�[�֎~
		BottomUpClusteringEngine( const BottomUpClusteringEngine &other );
		BottomUpClusteringEngine & operator = ( const BottomUpClusteringEngine &other );

	public:
		//! �R���X�g���N�^
		BottomUpClusteringEngine();

		//! �f�X�g���N�^
		~BottomUpClusteringEngine();

		//! �f�[�^�ԋ������ɂ��؍\����������
		/*!
			@param[in] pDtAccessor �f�[�^�ԋ����e�[�u���ւ̃A�N�Z�X�̃C���X�^���X
			@exception BottomUpClusteringEngineException ���s�����ꍇ
			@brief ���̏������I���΃f�[�^�ԋ����ւ̃A�N�Z�X�͕s�v
		*/
		void initTreeByInterDataDistance( const DistanceTableAccessor* pDtAccessor );

		//! �{�g���A�b�v�N���X�^�����O�̖؍\���i�f���h���O�����j���쐬
		/*!
			@param[in] icdType �g�p����N���X�^�ԋ����֐��̎��
			@exception BottomUpClusteringEngineException ���s�����ꍇ
		*/
		void makeClusterTree( InterClusterDistanceType icdType = WARD );

		//! �؍\������ɂ���
		/*!
		*/
		void clearClusterTree();

		//! �؍\�����ォ��K�w�I�ɃT�[�`����C�^���[�^�𐶐�
		/*!
			@return �擪�C�^���[�^�i�K�w�\���̒���A�N���X�^����1�̉ӏ��j
		*/
		LayerIterator beginClusterTree() const;

		//! �I�[�C�^���[�^�𐶐�
		/*!
			@return �I�[�C�^���[�^�i�K�w�\���̒�ӂ̉��A�N���X�^��=�f�[�^���ƂȂ�����j
		*/
		LayerIterator endClusterTree() const;

	protected:
		//! �N���X�^�����O����
		/*!
			@param[in] icdType �g�p����N���X�^�ԋ����֐��̎��
			@param[in] thresholdNumClusters �N���X�^����臒l�B�N���X�^�������̐��ɂȂ�ƃN���X�^�����O���I������B0�̂Ƃ��̓N���X�^���ɂ�鐧���Ȃ��B
			@param[in] thresholdMergeCost �N���X�^�}�[�W�R�X�g��臒l�B�}�[�W�R�X�g�����̒l�ȏ�ɂȂ�ƃN���X�^�����O���I������B0�̂Ƃ��̓}�[�W�R�X�g�ɂ�鐧���Ȃ��B
			@param[in] removeMergedChildren true���ƃ}�[�W��Ƀ}�[�W�O��2�N���X�^���폜�����B�������ߖ�ɂȂ邪�A�؍\��������Ȃ��B
		*/
		void doClustering(
			InterClusterDistanceType icdType,
			unsigned int thresholdNumClusters = 0,
			float thresholdMergeCost = 0.0f,
			bool removeMergedChildren = false
		);
		
	protected:
		//DistanceTableAccessor	*pDtAccessor_;		//! �����e�[�u���ւ̃A�N�Z�X�@�\
		ClusteringTree			*pTree_;			//! �؍\���Ǘ��̃I�u�W�F�N�g�ւ̃|�C���^

	};

	//! ��O�N���X
	class BottomUpClusteringEngineException : public std::exception
	{
	public:
		//! �R���X�g���N�^
		BottomUpClusteringEngineException()													: std::exception() {};

		//! �R���X�g���N�^
		BottomUpClusteringEngineException( const char* const& msg )							: std::exception( msg ) {};

		//! �R���X�g���N�^
		BottomUpClusteringEngineException( const char* const& msg, int errId )				: std::exception( msg, errId ) {};

		//! �R���X�g���N�^
		BottomUpClusteringEngineException( const BottomUpClusteringEngineException& other )	: std::exception( other ) {};
	};
}

#endif	// _BOTTOM_UP_CLUSTERING_ENGINGE_H_
