/**************************************************************************//**
 *	@file DistanceTableAccessorPrimitive.h
 *****************************************************************************/


#include "DistanceTableAccessorPrimitive.h"


using namespace std;
using namespace dstclst;


//! �R���X�g���N�^
DistanceTableAccessorPrimitive::DistanceTableAccessorPrimitive() : DistanceTableAccessor(), pDistanceTable_( NULL )
{
}


//! �f�X�g���N�^
DistanceTableAccessorPrimitive::~DistanceTableAccessorPrimitive()
{
	pDistanceTable_ = NULL;
}


//! �����e�[�u�����Z�b�g
bool DistanceTableAccessorPrimitive::setDistanceTable( void* pDistanceTable, int param1, int param2 )
{
	const vector< vector< float > >*	p = reinterpret_cast< vector< vector< float > >* >( pDistanceTable );

	// �T�C�Y�`�F�b�N�i�O�p�`�ɂȂ��Ă��邩�j
	try {
		unsigned int size = p->size();
		if( size == 0 ) return false;

		for( unsigned int i = 0; i < size; ++i )
		{
			if( p->at( i ).size() != i ) return false;
		}
	}
	catch( ... )
	{
		return false;
	}

	pDistanceTable_ = p;
	return true;
}


//! �e�[�u����1�ӂ̒������擾�i�v�f���j
unsigned int DistanceTableAccessorPrimitive::getDistanceTableSize() const
{
	return pDistanceTable_ == NULL ? 0 : pDistanceTable_->size();
}


//! i�Ԗڂ̗v�f��j�Ԗڂ̗v�f�̋������擾
float DistanceTableAccessorPrimitive::getDistanceAt( unsigned int i, unsigned int j ) const
{
	// �T�C�Y�`�F�b�N
	if( i < 0 || getDistanceTableSize() <= i )	throw std::out_of_range( "invalid index" );
	if( j < 0 || getDistanceTableSize() <= j )	throw std::out_of_range( "invalid index" );

	if( i > j )
	{
		return ( *pDistanceTable_ )[ i ][ j ];
	}
	else if( i < j )
	{
		return ( *pDistanceTable_ )[ j ][ i ];
	}
	else	// i == j
	{
		return 0.0f;
	}
}

