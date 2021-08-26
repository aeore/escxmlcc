#ifndef UT_TESTSET_HPP
#define UT_TESTSET_HPP

/****************************************************************************
 * Project         ESCXMLCC UT
 * (c) copyright   2014
 * Company         Yuriy Gurin RnD
 *                 All rights reserved
 ****************************************************************************/
/**
 * @file    TestSet.hpp
 * @ingroup SCXMLCC::UT
 * @author  YGurin
 *
 * @brief   
 */

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include <cppunit/extensions/HelperMacros.h>
#include "Sequence.hpp"

// --------------------------------------------------------------------------
// Code definition
// --------------------------------------------------------------------------

class TestSet : public CppUnit::TestFixture
{
   CPPUNIT_TEST_SUITE( TestSet );
	   CPPUNIT_TEST( testOneState );
      CPPUNIT_TEST( testTwoStatesUncond );
      CPPUNIT_TEST( testTwoStatesEvent );
      CPPUNIT_TEST( testThreeDirectEvent );
      CPPUNIT_TEST( testThreeBicyclicEvents );
      CPPUNIT_TEST( testTwoBicyclicEventsWithInit );
      CPPUNIT_TEST( testTwoBicyclicEventsWithInitDiffEvents );
      CPPUNIT_TEST( testSinglenodeSelftransitionEvent );
      CPPUNIT_TEST( testThreeFork );
      CPPUNIT_TEST( testThreeTriangleWithBack );
      CPPUNIT_TEST( testThreeRepeatQuiteAndReturn );
      CPPUNIT_TEST( testSquareEvents );
      CPPUNIT_TEST( testStarAndCycling );
      CPPUNIT_TEST( testSemieventAndConditions );
      CPPUNIT_TEST( testCounter );
      CPPUNIT_TEST( testCounterTransIncr );
      CPPUNIT_TEST( testDiffCond );
      CPPUNIT_TEST( testClusterOneNode );
      CPPUNIT_TEST( testClusterTwoNodes );
      CPPUNIT_TEST( testClusterWithExit );
      CPPUNIT_TEST( testClusterTwoNodesEventsAndEventExit );
      CPPUNIT_TEST( testClusterMultinodes );
      CPPUNIT_TEST( testClusterMultinodes2 );
      CPPUNIT_TEST( testClusterMultinodes3 );
      CPPUNIT_TEST( testClusterMultinodes4 );
      CPPUNIT_TEST( testCluster2Multinodes );
      CPPUNIT_TEST( testClusterRec2Multinodes );
      CPPUNIT_TEST( testClusterRec2Multinodes_1 );
      CPPUNIT_TEST( testClusterRec2Multinodes_2 );
      CPPUNIT_TEST( testClusterRec2Multinodes_3 );
      CPPUNIT_TEST( testClusterRec2Multinodes_4 );
      CPPUNIT_TEST( testClusterRec2Multinodes_5 );
      CPPUNIT_TEST( testInclude_1 );
      CPPUNIT_TEST( testTHSimple );
      CPPUNIT_TEST( testTHSimpleParam );
      CPPUNIT_TEST( testTHCluster1 );
      CPPUNIT_TEST( testTHCluster2 );
      CPPUNIT_TEST( testTHCluster3 );
      CPPUNIT_TEST( testTHCluster4 );
      CPPUNIT_TEST( testTHTwoWays );
      CPPUNIT_TEST( testAsyncNameTransition_1 );
      CPPUNIT_TEST( testAsyncNameTransition_2 );
      CPPUNIT_TEST( testAsyncNameTransition_3 );
      CPPUNIT_TEST( testAsyncNameTransition_4 );
      CPPUNIT_TEST( testAsyncNameTransition_5 );
      CPPUNIT_TEST( testAsyncNameTransition_6 );
      CPPUNIT_TEST( testAsyncNameTransition_7 );
      CPPUNIT_TEST( testAsyncNameTransition_8 );
      CPPUNIT_TEST( testAsyncNameTransition_9 );
      CPPUNIT_TEST( testAsyncNameTransition_10 );
      CPPUNIT_TEST( testAsyncNameTransition_11 );
      CPPUNIT_TEST( testAsyncNameTransition_12 );
      CPPUNIT_TEST( testAsyncNameTransition_13 );
      CPPUNIT_TEST( testAsyncNameTransition_14 );
      CPPUNIT_TEST( testAsyncNameTransition_15 );
      CPPUNIT_TEST( testAsyncNameTransition_16 );
	CPPUNIT_TEST_SUITE_END();

public:
	virtual void setUp();
	virtual void tearDown();

	void testOneState( void );
   void testTwoStatesUncond( void );
   void testTwoStatesEvent( void );
   void testThreeDirectEvent( void );
   void testThreeBicyclicEvents( void );
   void testTwoBicyclicEventsWithInit( void );
   void testTwoBicyclicEventsWithInitDiffEvents( void );
   void testSinglenodeSelftransitionEvent( void );
   void testThreeFork( void );
   void testThreeTriangleWithBack( void );
   void testThreeRepeatQuiteAndReturn( void );
   void testSquareEvents( void );
   void testStarAndCycling( void );
   void testSemieventAndConditions( void );
   void testCounter( void );
   void testCounterTransIncr( void );
   void testDiffCond( void );
   void testClusterOneNode( void );
   void testClusterTwoNodes( void );
   void testClusterWithExit( void );
   void testClusterTwoNodesEventsAndEventExit( void );
   void testClusterMultinodes( void );
   void testClusterMultinodes2( void );
   void testClusterMultinodes3( void );
   void testClusterMultinodes4( void );
   void testCluster2Multinodes( void );
   void testClusterRec2Multinodes( void );
   void testClusterRec2Multinodes_1( void );
   void testClusterRec2Multinodes_2( void );
   void testClusterRec2Multinodes_3( void );
   void testClusterRec2Multinodes_4( void );
   void testClusterRec2Multinodes_5( void );
   void testInclude_1( void );
   void testTHSimple( void );
   void testTHSimpleParam( void );
   void testTHCluster1( void );
   void testTHCluster2( void );
   void testTHCluster3( void );
   void testTHCluster4( void );
   void testTHTwoWays( void );
   void testAsyncNameTransition_1( void );
   void testAsyncNameTransition_2( void );
   void testAsyncNameTransition_3( void );
   void testAsyncNameTransition_4( void );
   void testAsyncNameTransition_5( void );
   void testAsyncNameTransition_6( void );
   void testAsyncNameTransition_7( void );
   void testAsyncNameTransition_8( void );
   void testAsyncNameTransition_9( void );
   void testAsyncNameTransition_10( void );
   void testAsyncNameTransition_11( void );
   void testAsyncNameTransition_12( void );
   void testAsyncNameTransition_13( void );
   void testAsyncNameTransition_14( void );
   void testAsyncNameTransition_15( void );
   void testAsyncNameTransition_16( void );

   static Sequence sequence;
};

Sequence TestSet::sequence = Sequence();

CPPUNIT_TEST_SUITE_REGISTRATION( TestSet );

#endif // UT_TESTSET_HPP