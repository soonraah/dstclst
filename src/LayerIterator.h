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
	//! �N���X�^�����O�̖؍\������i�N���X�^�� = 1�j���珇�Ɍ��Ă����C�^���[�^�N���X
	class LayerIterator : public std::iterator< std::input_iterator_tag, ClusterArray, void >
	{
	public:
		//! �R���X�g���N�^�i�I�[�j
		LayerIterator();

		//! �R���X�g���N�^
		LayerIterator( const ClusterArray& topLayer );

		//! �R�s�[�R���X�g���N�^
		LayerIterator( const LayerIterator& other );

		//! �f�X�g���N�^
		~LayerIterator();

		//! ������Z�q
		LayerIterator& operator=( const LayerIterator& other );

		//! ���ԎQ��
		ClusterArray& operator *();

		//! �����o�A�N�Z�X���Z�q
		ClusterArray* operator->();

		//! �����o�A�N�Z�X���Z�q
		const ClusterArray* operator->() const;

		//! �O�u�C���N�������g
		LayerIterator& operator ++();

		//! ��u�C���N�������g
		LayerIterator operator ++( int );

		//! ��r���Z�q
		const bool operator!=( const LayerIterator& other ) const;

		//! ��r���Z�q
		const bool operator==( const LayerIterator& other ) const;

		//! �C�^���[�^���w�������؍\���̌��݂̊K�w�̃}�[�W�R�X�g���擾
		/*!
			@return �}�[�W�R�X�g
		*/
		float getLayerCost() const;

	protected:
		ClusterArray	curLayer_;
	};

	//const bool operator!=( const LayerIterator& it1, const LayerIterator& it2 );
}


#endif	// _HIERARCHICAL_ITERATOR_H_
