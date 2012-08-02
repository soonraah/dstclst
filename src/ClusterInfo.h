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
	//! �N���X�^�̏��������N���X
	class ClusterInfo
	{
	public:
		//! �R���X�g���N�^
		ClusterInfo();

		//! �R���X�g���N�^�i�}�[�W�j
		/*!
			@param[in] child1 �q�N���X�^����1
			@param[in] child2 �q�N���X�^����2
			@param[in] mergeCost �}�[�W�R�X�g
		*/
		ClusterInfo( const ClusterInfo* child1, const ClusterInfo* child2, float mergeCost );

		//! �f�X�g���N�^
		~ClusterInfo();

		//! �����o�ǉ�
		/*!
			@param[in] dataIndex �N���X�^�ɒǉ�����f�[�^�����o
		*/
		void addMember( DataIndex dataIndex );

		//! �����o�ǉ�
		/*!
			@param[in] dataIndexes �N���X�^�ɒǉ�����f�[�^�����o�i�����j
		*/
		void addMembers( const std::vector< DataIndex >& dataIndexes );

		//! �����o�擾
		/*!
			@return �f�[�^�����o�ւ̃|�C���^
		*/
		const std::vector< DataIndex >* getMembers() const;

		//! �}�[�W�R�X�g�̐ݒ�
		/*!
			@param[in] mergeCost �N���X�^�}�[�W���̃R�X�g
		*/
		void setMergeCost( float mergeCost );

		//! �}�[�W�R�X�g�̎擾
		/*!
			@return �N���X�^�}�[�W���̃R�X�g
		*/
		float getMergeCost() const;

		//! �}�[�W�O�̎q�N���X�^1���擾
		/*!
			@return �}�[�W�O�̎q�N���X�^1�ւ̃|�C���^
		*/
		const ClusterInfo* getChild1() const;

		//! �}�[�W�O�̎q�N���X�^2���擾
		/*!
			@return �}�[�W�O�̎q�N���X�^2�ւ̃|�C���^
		*/
		const ClusterInfo* getChild2() const;

		//! �q�N���X�^��ݒ�
		/*!
			@param[in] child1 �Z�b�g����q�N���X�^1
			@param[in] child2 �Z�b�g����q�N���X�^2
		*/
		void setChildren( const ClusterInfo* child1, const ClusterInfo* child2 );

	protected:
		std::vector< DataIndex >	members_;		//! �N���X�^�̃f�[�^�����o
		const ClusterInfo*			child1_;		//! �}�[�W�O�̃N���X�^���̂P
		const ClusterInfo*			child2_;		//! �}�[�W�O�̃N���X�^���̂Q
		float						mergeCost_;		//! ���̃N���X�^�̃}�[�W�R�X�g

	public:
		void*						hook_;			//! �ǉ����i����΁j�@�����ӁFvoid�|�C���^���ƃf�X�g���N�^�͌Ă΂�Ȃ��̂ŁA�f�X�g���N�^�s�v�Ȍ^�Ɍ���B
	};


	//! �N���X�^�̏W�c
	typedef std::vector< const ClusterInfo* >		ClusterArray;


	//! ��r�p�t�@���N�^
	class PredicateClusterPtr
	{
	public:
		//! ��r�p
		/*!
			@param[in] pLeft ���ӂ̃N���X�^�|�C���^
			@param[in] pRight �E�ӂ̃N���X�^�|�C���^
			@return L<R�̂Ƃ���true���Ԃ�悤�ɂ���Ə����\�[�g�ɂȂ�
		*/
		bool operator()( const ClusterInfo* pLeft, const ClusterInfo* pRight ) const;
	};
}


#endif	// _CLUSTER_INFO_H_

