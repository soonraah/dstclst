/**************************************************************************//**
 *	@file LayerIterator.cpp
 *****************************************************************************/


#include <algorithm>
#include "LayerIterator.h"


using namespace std;
using namespace dstclst;


//! curLayer_�̏󋵂�����O�i
/*!
	@param[in,out] cur �X�V�Ώۂ̌��݂̃��C���[
	@return ���Ƀ{�g���܂ł��ǂ蒅���Ă���ꍇ��false
*/
bool frontStep( ClusterArray& cur )
{
	if( cur.empty() ) return false;

	const ClusterInfo*	p1 = cur[ 0 ]->getChild1();
	const ClusterInfo*	p2 = cur[ 0 ]->getChild2();

	if( p1 == NULL || p2 == NULL )
	{
		cur.clear();
		return false;
	}

	cur.erase( cur.begin() );	// �e���폜
	cur.push_back( p1 );		// �q��ǉ�
	cur.push_back( p2 );		// �q��ǉ�

	// �}�[�W�R�X�g�~���Ń\�[�g
	sort( cur.begin(), cur.end(), PredicateClusterPtr() );

	return true;
}


//! �R���X�g���N�^�i�I�[�j
LayerIterator::LayerIterator() : curLayer_( 0 )
{
}


//! �R���X�g���N�^
LayerIterator::LayerIterator( const ClusterArray& topLayer ) : curLayer_( topLayer )
{
	sort( curLayer_.begin(), curLayer_.end(), PredicateClusterPtr() );
}


//! �R�s�[�R���X�g���N�^
LayerIterator::LayerIterator( const LayerIterator& other )
{
	*this = other;
}


//! �f�X�g���N�^
LayerIterator::~LayerIterator()
{
	curLayer_.clear();
}


//! ������Z�q
LayerIterator& LayerIterator::operator=( const LayerIterator& other )
{
	curLayer_.clear();
	curLayer_.reserve( other->size() );
	for( ClusterArray::const_iterator it = other->begin(); it != other->end(); ++it )
	{
		curLayer_.push_back( *it );
	}

	return *this;
}


//! ���ԎQ��
ClusterArray& LayerIterator::operator *()
{
	return curLayer_;
}


//! �����o�A�N�Z�X���Z�q
ClusterArray* LayerIterator::operator->()
{
	return &curLayer_;
}


//! �����o�A�N�Z�X���Z�q
const ClusterArray* LayerIterator::operator->() const
{
	return &curLayer_;
}


//! �O�u�C���N�������g
LayerIterator& LayerIterator::operator ++()
{
	frontStep( curLayer_ );
	return *this;
}


//! ��u�C���N�������g
LayerIterator LayerIterator::operator ++( int )
{
	LayerIterator temp( *this );
	frontStep( curLayer_ );
	return temp;
}


//! ��r���Z�q
const bool LayerIterator::operator!=( const LayerIterator& other ) const
{
	return !( *this == other );
}


//! ��r���Z�q
const bool LayerIterator::operator==( const LayerIterator& other ) const
{
	if( curLayer_.size() != other->size() ) return false;

	ClusterArray::const_iterator itThis = ( *this )->begin();
	ClusterArray::const_iterator itOther = other->begin();

	for( ; itThis != ( *this )->end(); ++itThis, ++itOther )
	{
		if( *itThis != *itOther ) return false;
	}

	return true;
}


//! �C�^���[�^���w�������؍\���̌��݂̊K�w�̃}�[�W�R�X�g���擾
float LayerIterator::getLayerCost() const
{
	if( curLayer_.empty() )
	{
		return -1.0f;
	}
	else
	{
		return curLayer_[ 0 ]->getMergeCost();
	}
}


