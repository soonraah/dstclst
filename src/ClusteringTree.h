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
	// �e�X�g�p�N���X�i�����ł͐錾�̂݁j
	class ClusteringTreeTest;

	//! �{�g���A�b�v�K�w�N���X�^�����O�̖؍\���������N���X
	class ClusteringTree
	{
		friend class ClusteringTreeTest;	// friend�w��

	private:
		// �R�s�[�֎~
		ClusteringTree( const ClusteringTree &other ) {};
		ClusteringTree & operator = ( const ClusteringTree &other ) {};

	public:
		//! �R���X�g���N�^
		ClusteringTree();

		//! �f�X�g���N�^
		~ClusteringTree();

		//! �N���X�^�ԋ����e�[�u���ɏ����l��ݒ�
		/*!
			@param[in] pDtAccessor �f�[�^�ԋ����ւ̃A�N�Z�X
			@exception std::invalid_argument �����̒l���������Ȃ��ꍇ
			@exception std::bad_alloc �������m�ۂɎ��s
			@brief ���̏������I���ƃf�[�^�ԋ����ւ̃A�N�Z�X�͕s�v�B�폜���Ă悢�B
		*/
		void setInitialDistance( const DistanceTableAccessor* pDtAccessor );

		//! �X�V���i=�N���X�^�ԋ����̒�`�j��ݒ�
		/*!
			@param[in] pUpdatingFormula �X�V���t�@���N�^�ւ̃|�C���^
		*/
		void setUpdatingFormula( LanceWilliamsUpdatingFormula* pUpdatingFormula );

		//! �N���X�^�ԋ����𒲂ׁA���̃N���X�^���}�[�W���Ė؍\����1�X�e�b�v��Ă�
		/*!
			@param[in] killChildren true�̏ꍇ�A�}�[�W��Ƀ}�[�W�O�̃N���X�^���폜����
			@return �}�[�W�R�X�g
		*/
		float buildOneStep( bool killChildren );

		//! ���݂̍ŏ�w�N���X�^���擾
		/*!
			@return ���݂̍ŏ�w�N���X�^
		*/
		const ClusterArray* getTopLayer() const;

		//! �N���X�^�A�N���X�^�ԋ����̏����폜
		void clear();

	protected:
		//! �ŏ��̃N���X�^�ԋ�����T��
		/*!
			@param[out] rowMin �ŏ��N���X�^�ԋ����������N���X�^�̑g�ݍ��킹�̍s�ԍ�
			@param[out] columnMin �ŏ��N���X�^�ԋ����������N���X�^�̑g�ݍ��킹�̗�ԍ�
			@param[out] distanceMin �ŏ��̃N���X�^�ԋ���
		*/
		void findMinDistance( unsigned int& rowMin, unsigned int& columnMin, float& distanceMin );

		//! �ŏ�w����N���X�^���폜
		/*!
			@param[in] clusterIndex �폜�Ώۂ̃N���X�^�C���f�b�N�X
			@exception std::out_of_range ���̓C���f�b�N�X���͈͊O
		*/
		void deleteClusterFromTopLayer( unsigned int clusterIndex );

		//! �ŏ�w�ɃN���X�^��ǉ�
		/*!
			@param[in] pCluster �ǉ�����N���X�^�ւ̃|�C���^
			@param[in] child1Index �q�N���X�^1�̃C���f�b�N�X�i�ǉ��N���X�^�̋����v�Z�Ɏg�p�j child1Index < child2Index
			@param[in] child2Index �q�N���X�^2�̃C���f�b�N�X�i�ǉ��N���X�^�̋����v�Z�Ɏg�p�j child1Index < child2Index
			@exception std::invalid_argument ���̓N���X�^������
			@exception std::bad_alloc �������m�ێ��s
		*/
		void addClusterIntoTopLayer( const ClusterInfo* pCluster, unsigned int child1Index, unsigned int child2Index );

	protected:
		ClusterArray						topLayer_;				//! �ŏ�w�̃N���X�^���X�g�i�N���X�^�ԋ����e�[�u���̕��я��j
		//const DistanceTableAccessor*		pDtAccessor_;			//! �f�[�^�ԋ����e�[�u���ւ̃A�N�Z�X�@�\

		std::vector< std::vector< float > >	interClusterDistance_;	//! �ŏ�w�̃N���X�^�ԋ����e�[�u��
		//ClusterArray						tableOrder_;			//! interClusterDistance_�̕��я��Ɠ���

		LanceWilliamsUpdatingFormula*		pUpdatingFormula_;		//! �X�V���t�@���N�^�ւ̃|�C���^

		bool								memorySavingMode_;		//! �������ߖ�D��̏ꍇtrue
	};
}


#endif	// _CLUSTERING_TREE_H_
