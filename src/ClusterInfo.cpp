/**************************************************************************//**
 *	@file ClusterInfo.cpp
 *****************************************************************************/


#include <algorithm>
#include "ClusterInfo.h"


using namespace std;
using namespace dstclst;


//! �R���X�g���N�^
ClusterInfo::ClusterInfo() : members_( 0 ), child1_( NULL ), child2_( NULL ), mergeCost_( 0.0f ), id_( 0 )
{
}


//! �R���X�g���N�^�i�}�[�W�j
ClusterInfo::ClusterInfo( const ClusterInfo* child1, const ClusterInfo* child2, float mergeCost, unsigned short id )
	: members_( 0 ), child1_( child1 ), child2_( child2 ), mergeCost_( mergeCost ), id_( id )
{
	// �q�N���X�^�̃|�C���^�`�F�b�N
	if( child1_ == NULL )
	{
		child2_ = NULL;
		return;
	}
	else if( child2_ == NULL )
	{
		child1_ = NULL;
		return;
	}

	// �����o�ǉ�
	addMembers( *( child1_->getMembers() ) );
	addMembers( *( child2_->getMembers() ) );
	sort( members_.begin(), members_.end() );
}


//! �f�X�g���N�^
ClusterInfo::~ClusterInfo()
{
	// �q�N���X�^�͓����ɍ폜���Ȃ����Ƃɂ���
	child1_ = NULL;
	child2_ = NULL;
}


//! �����o�ǉ�
void ClusterInfo::addMember( DataIndex dataIndex )
{
	members_.push_back( dataIndex );
}


//! �����o�ǉ�
void ClusterInfo::addMembers( const std::vector< DataIndex >& dataIndexes )
{
	members_.insert( members_.end(), dataIndexes.begin(), dataIndexes.end() );
}


//! �����o�擾
const std::vector< DataIndex >* ClusterInfo::getMembers() const
{
	return &members_;
}


//! �}�[�W�R�X�g�̐ݒ�
void ClusterInfo::setMergeCost( float mergeCost )
{
	mergeCost_ = mergeCost;
}


//! �}�[�W�R�X�g�̎擾
float ClusterInfo::getMergeCost() const
{
	return mergeCost_;
}


//! �}�[�W�O�̎q�N���X�^1���擾
const ClusterInfo* ClusterInfo::getChild1() const
{
	return child1_;
}


//! �}�[�W�O�̎q�N���X�^2���擾
const ClusterInfo* ClusterInfo::getChild2() const
{
	return child2_;
}


//! �q�N���X�^��ݒ�
void ClusterInfo::setChildren( const ClusterInfo* child1, const ClusterInfo* child2 )
{
	child1_ = child1;
	child2_ = child2;

	// �ǂ����NULL�̏ꍇ�̓����o�̍X�V���s��Ȃ�
	if( child1 == NULL && child2 == NULL ) return;

	// �����o�X�V
	members_.clear();
	if( child1 ) addMembers( *( child1->getMembers() ) );
	if( child2 ) addMembers( *( child2->getMembers() ) );
	sort( members_.begin(), members_.end() );
	return;
}


//! ID�擾
unsigned short ClusterInfo::getId() const
{
	return id_;
}


//! ID��ݒ�
void ClusterInfo::setId( unsigned short id )
{
	id_ = id;
}


//! ��r�p
bool PredicateClusterPtr::operator()( const ClusterInfo* pLeft, const ClusterInfo* pRight ) const
{
	if( pLeft->getMergeCost() > pRight->getMergeCost() )
	{
		return true;
	}
	else if( pLeft->getMergeCost() == pRight->getMergeCost() )
	{
		if( pLeft->getMembers()->size() > pRight->getMembers()->size() )
		{
			return true;
		}
		else if( pLeft->getMembers()->size() == pRight->getMembers()->size() )
		{
			return pLeft->getMembers()->at( 0 ) < pRight->getMembers()->at( 0 );
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}
