/**************************************************************************//**
 *	@file ClusterInfo.cpp
 *****************************************************************************/


#include <algorithm>
#include "ClusterInfo.h"


using namespace std;
using namespace dstclst;


//! コンストラクタ
ClusterInfo::ClusterInfo() : members_( 0 ), child1_( NULL ), child2_( NULL ), mergeCost_( 0.0f ), id_( 0 )
{
}


//! コンストラクタ（マージ）
ClusterInfo::ClusterInfo( const ClusterInfo* child1, const ClusterInfo* child2, float mergeCost, unsigned short id )
	: members_( 0 ), child1_( child1 ), child2_( child2 ), mergeCost_( mergeCost ), id_( id )
{
	// 子クラスタのポインタチェック
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

	// メンバ追加
	addMembers( *( child1_->getMembers() ) );
	addMembers( *( child2_->getMembers() ) );
	sort( members_.begin(), members_.end() );
}


//! デストラクタ
ClusterInfo::~ClusterInfo()
{
	// 子クラスタは同時に削除しないことにする
	child1_ = NULL;
	child2_ = NULL;
}


//! メンバ追加
void ClusterInfo::addMember( DataIndex dataIndex )
{
	members_.push_back( dataIndex );
}


//! メンバ追加
void ClusterInfo::addMembers( const std::vector< DataIndex >& dataIndexes )
{
	members_.insert( members_.end(), dataIndexes.begin(), dataIndexes.end() );
}


//! メンバ取得
const std::vector< DataIndex >* ClusterInfo::getMembers() const
{
	return &members_;
}


//! マージコストの設定
void ClusterInfo::setMergeCost( float mergeCost )
{
	mergeCost_ = mergeCost;
}


//! マージコストの取得
float ClusterInfo::getMergeCost() const
{
	return mergeCost_;
}


//! マージ前の子クラスタ1を取得
const ClusterInfo* ClusterInfo::getChild1() const
{
	return child1_;
}


//! マージ前の子クラスタ2を取得
const ClusterInfo* ClusterInfo::getChild2() const
{
	return child2_;
}


//! 子クラスタを設定
void ClusterInfo::setChildren( const ClusterInfo* child1, const ClusterInfo* child2 )
{
	child1_ = child1;
	child2_ = child2;

	// どちらもNULLの場合はメンバの更新を行わない
	if( child1 == NULL && child2 == NULL ) return;

	// メンバ更新
	members_.clear();
	if( child1 ) addMembers( *( child1->getMembers() ) );
	if( child2 ) addMembers( *( child2->getMembers() ) );
	sort( members_.begin(), members_.end() );
	return;
}


//! ID取得
unsigned short ClusterInfo::getId() const
{
	return id_;
}


//! IDを設定
void ClusterInfo::setId( unsigned short id )
{
	id_ = id;
}


//! 比較用
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
