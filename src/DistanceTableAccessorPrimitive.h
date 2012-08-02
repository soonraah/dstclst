/**************************************************************************//**
 *	@file DistanceTableAccessorPrimitive.h
 *****************************************************************************/


#ifndef _DISTANCE_TABLE_ACCESSOR_PRIMITIVE_H_
#define _DISTANCE_TABLE_ACCESSOR_PRIMITIVE_H_


#include <vector>

#include "DistanceTableAccessor.h"


namespace dstclst
{
	//! �����e�[�u���ɃA�N�Z�X���邽�߂̒��ۃN���X
	class DistanceTableAccessorPrimitive : public DistanceTableAccessor
	{
	public:
		//! �R���X�g���N�^
		DistanceTableAccessorPrimitive();

		//! �f�X�g���N�^
		virtual ~DistanceTableAccessorPrimitive();

	public:
		//! �����e�[�u�����Z�b�g
		/*!
			@param[in] pDistanceTable �����e�[�u���ւ̃|�C���^
			@param[in] param1 �p�����[�^����1�i�g��Ȃ��j
			@param[in] param2 �p�����[�^����2�i�g��Ȃ��j
			@return �ݒ�ɐ���������true
		*/
		virtual bool setDistanceTable( void* pDistanceTable, int param1, int param2 );

		//! �e�[�u����1�ӂ̒������擾�i�v�f���j
		/*!
			@return �e�[�u����1�ӂ̒���
		*/
		virtual unsigned int getDistanceTableSize() const;

		//! i�Ԗڂ̗v�f��j�Ԗڂ̗v�f�̋������擾
		/*!
			@param[in] i �s�ԍ�
			@param[in] j ��ԍ�
			@return i�Ԗڂ̗v�f��j�Ԗڂ̗v�f�̋���
			@exception std::out_of_range i�܂���j���e�[�u���T�C�Y�͈̔͊O�������ꍇ
		*/
		virtual float getDistanceAt( unsigned int i, unsigned int j ) const;

	protected:
		const std::vector< std::vector< float > >*	pDistanceTable_;

	};
}


#endif	// _DISTANCE_TABLE_ACCESSOR_PRIMITIVE_H_
