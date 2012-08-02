/**************************************************************************//**
 *	@file ClusterInfoTest.cpp
 *****************************************************************************/


#include "ClusterInfoTest.h"
#include "ClusterInfo.h"


using namespace dstclst;
using namespace std;


//! �e�X�g�O�Ɏ��s
void ClusterInfoTest::SetUp()
{
}


//! �e�X�g��Ɏ��s
void ClusterInfoTest::TearDown()
{
}


//! �����o�ǉ��̃e�X�g
TEST_F( ClusterInfoTest, test_addMember ) 
{
	ClusterInfo					ci;
	const vector< DataIndex >*	pMembers;
	
	ci.addMember( 1 );
	pMembers = ci.getMembers();
	ASSERT_EQ( 1, pMembers->size() );
	EXPECT_EQ( 1, pMembers->at( 0 ) );

	ci.addMember( 100 );
	pMembers = ci.getMembers();
	ASSERT_EQ( 2, pMembers->size() );
	EXPECT_EQ( 1, pMembers->at( 0 ) );
	EXPECT_EQ( 100, pMembers->at( 1 ) );
}


//! �����o�ǉ��̃e�X�g�i�����j
TEST_F( ClusterInfoTest, test_addMembers ) 
{
	ClusterInfo					ci;
	const vector< DataIndex >*	pMembers;
	vector< DataIndex >			addedData;

	addedData.push_back( 5 );
	addedData.push_back( 7 );
	addedData.push_back( 9 );

	ci.addMembers( addedData );
	pMembers = ci.getMembers();
	ASSERT_EQ( 3, pMembers->size() );
	EXPECT_EQ( 5, pMembers->at( 0 ) );
	EXPECT_EQ( 7, pMembers->at( 1 ) );
	EXPECT_EQ( 9, pMembers->at( 2 ) );

	addedData.clear();
	addedData.push_back( 4 );
	addedData.push_back( 6 );
	addedData.push_back( 8 );
	addedData.push_back( 10 );

	ci.addMembers( addedData );
	pMembers = ci.getMembers();
	ASSERT_EQ( 7, pMembers->size() );
	EXPECT_EQ( 5, pMembers->at( 0 ) );		// ���\�[�g���Ȃ�
	EXPECT_EQ( 7, pMembers->at( 1 ) );
	EXPECT_EQ( 9, pMembers->at( 2 ) );
	EXPECT_EQ( 4, pMembers->at( 3 ) );
	EXPECT_EQ( 6, pMembers->at( 4 ) );
	EXPECT_EQ( 8, pMembers->at( 5 ) );
	EXPECT_EQ( 10, pMembers->at( 6 ) );
}


//! �����o�ǉ��̃e�X�g�i�����j
TEST_F( ClusterInfoTest, test_setAndGetmergeCost ) 
{
	ClusterInfo					ci;

	ci.setMergeCost( 0.5f );
	EXPECT_EQ( 0.5f, ci.getMergeCost() );

	ci.setMergeCost( 1234.567f );
	EXPECT_EQ( 1234.567f, ci.getMergeCost() );
}


//! �R���X�g���N�^�i�}�[�W�j�̃e�X�g
TEST_F( ClusterInfoTest, test_mergeConstructor )
{
	ClusterInfo	*pC1 = new ClusterInfo();
	
	pC1->addMember( 3 );
	pC1->addMember( 5 );
	pC1->addMember( 1 );

	ClusterInfo	*pC2 = new ClusterInfo();

	pC2->addMember( 2 );
	pC2->addMember( 4 );
	pC2->addMember( 6 );

	const vector< DataIndex >* pMembers = NULL;

	ClusterInfo *pCm = new ClusterInfo( pC1, pC2, 100.0f );

	pMembers = pCm->getMembers();
	ASSERT_EQ( 6, pMembers->size() );
	EXPECT_EQ( 1, pMembers->at( 0 ) );	// ���\�[�g�����
	EXPECT_EQ( 2, pMembers->at( 1 ) );
	EXPECT_EQ( 3, pMembers->at( 2 ) );
	EXPECT_EQ( 4, pMembers->at( 3 ) );
	EXPECT_EQ( 5, pMembers->at( 4 ) );
	EXPECT_EQ( 6, pMembers->at( 5 ) );
	ASSERT_EQ( 100.0f, pCm->getMergeCost() );

	delete pCm;
	delete pC1;
	delete pC2;

	return;
}


//! �q�N���X�^�̎擾�A�ݒ�̃e�X�g
TEST_F( ClusterInfoTest, test_setAndGetChildren )
{
	ClusterInfo c;

	ClusterInfo c1;
	c1.addMember( 1 );
	c1.addMember( 100 );

	ClusterInfo c2;
	c2.addMember( 10 );
	c2.addMember( 11 );
	c2.addMember( 12 );

	// �������
	ASSERT_EQ( NULL, c.getChild1() );
	ASSERT_EQ( NULL, c.getChild2() );
	ASSERT_EQ( 0, c.getMembers()->size() );

	// �q�N���X�^��ݒ�
	c.setChildren( &c1, &c2 );
	ASSERT_EQ( &c1, c.getChild1() );
	ASSERT_EQ( &c2, c.getChild2() );
	ASSERT_EQ( 5, c.getMembers()->size() );
	ASSERT_EQ( 1, c.getMembers()->at( 0 ) );
	ASSERT_EQ( 10, c.getMembers()->at( 1 ) );
	ASSERT_EQ( 11, c.getMembers()->at( 2 ) );
	ASSERT_EQ( 12, c.getMembers()->at( 3 ) );
	ASSERT_EQ( 100, c.getMembers()->at( 4 ) );

	// ����NULL��ݒ�i�����o�͕ς��Ȃ��j
	c.setChildren( NULL, NULL );
	ASSERT_EQ( NULL, c.getChild1() );
	ASSERT_EQ( NULL, c.getChild2() );
	ASSERT_EQ( 5, c.getMembers()->size() );
	ASSERT_EQ( 1, c.getMembers()->at( 0 ) );
	ASSERT_EQ( 10, c.getMembers()->at( 1 ) );
	ASSERT_EQ( 11, c.getMembers()->at( 2 ) );
	ASSERT_EQ( 12, c.getMembers()->at( 3 ) );
	ASSERT_EQ( 100, c.getMembers()->at( 4 ) );

	// �Е�NULL�݂̂�ݒ�i�����o�͕ς��j
	c.setChildren( NULL, &c1 );
	ASSERT_EQ( NULL, c.getChild1() );
	ASSERT_EQ( &c1, c.getChild2() );
	ASSERT_EQ( 2, c.getMembers()->size() );
	ASSERT_EQ( 1, c.getMembers()->at( 0 ) );
	ASSERT_EQ( 100, c.getMembers()->at( 1 ) );

	return;
}

