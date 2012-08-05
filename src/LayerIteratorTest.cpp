/**************************************************************************//**
 *	@file LayerIteratorTest.cpp
 *	@brief 
 *****************************************************************************/


#include "LayerIteratorTest.h"
#include "LayerIterator.h"


using namespace std;
using namespace dstclst;


//! �e�X�g�O�Ɏ��s
void LayerIteratorTest::SetUp()
{
	// �ł���߂ȃe�X�g�p�N���X�^�؍\�����쐬
	// �����o�֌W���ς����ǁA�؂����ǂ邽�߂̃e�X�g�Ȃ̂ŋC�ɂ��Ȃ��B
	//
	//           cost
	//	   X      300
	//	   |
	//	 +-+-+
	//	 |   |
	//	 X   |    200
	//	 |   |
	//	+++  X    100
	//	| |  |
	//	| | +++
	//	| | | |
	//	1 2 3 4     0

	ClusterInfo*	p1 = NULL;
	ClusterInfo*	p2 = NULL;
	ClusterInfo*	p3 = NULL;
	
	p1 = new ClusterInfo();
	p1->addMember( 1 );

	p2 = new ClusterInfo();
	p2->addMember( 2 );

	p3 = new ClusterInfo( p1, p2, 200.0f, 5 );

	p1 = new ClusterInfo();
	p1->addMember( 3 );

	p2 = new ClusterInfo();
	p2->addMember( 4 );

	p1 = new ClusterInfo( p1, p2, 100.0f, 6 );

	p2 = new ClusterInfo( p3, p1, 300.0f, 7 );

	layer_.clear();
	layer_.push_back( p2 );
}


//! �e�X�g��Ɏ��s
void LayerIteratorTest::TearDown()
{
	const ClusterInfo *p = layer_.at( 0 );

	delete p->getChild1()->getChild1();
	delete p->getChild1()->getChild2();
	delete p->getChild2()->getChild1();
	delete p->getChild2()->getChild2();
	delete p->getChild1();
	delete p->getChild2();
	delete p;

	layer_.clear();
}


//! ���낢��e�X�g
TEST_F( LayerIteratorTest, test_driveIt )
{
	LayerIterator	it( layer_ );
	LayerIterator	itEnd;	// �I�[

	ASSERT_TRUE( it != itEnd );
	ASSERT_FALSE( it == itEnd );
	ASSERT_EQ( 1, it->size() );
	ASSERT_EQ( 4, it->at( 0 )->getMembers()->size() );
	ASSERT_EQ( 1, it->at( 0 )->getMembers()->at( 0 ) );
	ASSERT_EQ( 2, it->at( 0 )->getMembers()->at( 1 ) );
	ASSERT_EQ( 3, it->at( 0 )->getMembers()->at( 2 ) );
	ASSERT_EQ( 4, it->at( 0 )->getMembers()->at( 3 ) );
	ASSERT_EQ( 300.0f, it.getLayerCost() );

	// �O�i
	ASSERT_EQ( 2, ( ++it )->size() );
	ASSERT_TRUE( it != itEnd );
	ASSERT_FALSE( it == itEnd );
	ASSERT_EQ( 2, it->at( 0 )->getMembers()->size() );
	ASSERT_EQ( 1, it->at( 0 )->getMembers()->at( 0 ) );
	ASSERT_EQ( 2, it->at( 0 )->getMembers()->at( 1 ) );
	ASSERT_EQ( 2, it->at( 1 )->getMembers()->size() );
	ASSERT_EQ( 3, it->at( 1 )->getMembers()->at( 0 ) );
	ASSERT_EQ( 4, it->at( 1 )->getMembers()->at( 1 ) );
	ASSERT_EQ( 200.0f, it.getLayerCost() );

	// �O�i
	ASSERT_EQ( 2, ( it++ )->size() );
	ASSERT_TRUE( it != itEnd );
	ASSERT_FALSE( it == itEnd );
	ASSERT_EQ( 3, it->size() );
	ASSERT_EQ( 2, it->at( 0 )->getMembers()->size() );
	ASSERT_EQ( 3, it->at( 0 )->getMembers()->at( 0 ) );
	ASSERT_EQ( 4, it->at( 0 )->getMembers()->at( 1 ) );
	ASSERT_EQ( 1, it->at( 1 )->getMembers()->size() );
	ASSERT_EQ( 1, it->at( 1 )->getMembers()->at( 0 ) );
	ASSERT_EQ( 1, it->at( 2 )->getMembers()->size() );
	ASSERT_EQ( 2, it->at( 2 )->getMembers()->at( 0 ) );
	ASSERT_EQ( 100.0f, it.getLayerCost() );

	// �O�i
	++it;
	ASSERT_TRUE( it != itEnd );
	ASSERT_FALSE( it == itEnd );
	ASSERT_EQ( 4, ( *it ).size() );
	ASSERT_EQ( 1, ( *it ).at( 0 )->getMembers()->size() );
	ASSERT_EQ( 1, ( *it ).at( 0 )->getMembers()->at( 0 ) );
	ASSERT_EQ( 1, ( *it ).at( 1 )->getMembers()->size() );
	ASSERT_EQ( 2, ( *it ).at( 1 )->getMembers()->at( 0 ) );
	ASSERT_EQ( 1, ( *it ).at( 2 )->getMembers()->size() );
	ASSERT_EQ( 3, ( *it ).at( 2 )->getMembers()->at( 0 ) );
	ASSERT_EQ( 1, ( *it ).at( 3 )->getMembers()->size() );
	ASSERT_EQ( 4, ( *it ).at( 3 )->getMembers()->at( 0 ) );
	ASSERT_EQ( 0.0f, it.getLayerCost() );

	// �O�i�i�I���j
	it++;
	ASSERT_FALSE( it != itEnd );
	ASSERT_TRUE( it == itEnd );
	ASSERT_EQ( 0, ( *it ).size() );
	ASSERT_EQ( -1.0f, it.getLayerCost() );
}

