/****************************************************************************
 * Project         ESCXMLCC UT
 * (c) copyright   2014
 * Company         Yuriy Gurin RnD
 *                 All rights reserved
 ****************************************************************************/
/**
 * @file    TestSet.cpp
 * @ingroup SCXMLCC::UT
 * @author  YGurin
 *
 * @brief   
 */

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "TestSet.hpp"
#include "src-gen/one_state.hpp"
#include "src-gen/two_states_uncond.hpp"
#include "src-gen/two_states_event.hpp"
#include "src-gen/three_dr_ev.hpp"
#include "src-gen/three_event_bicyclic.hpp"
#include "src-gen/two_event_bicyclic_with_init.hpp"
#include "src-gen/two_event_bicyclic_with_init_diffevents.hpp"
#include "src-gen/singlenode_selftrans_event.hpp"
#include "src-gen/three_fork.hpp"
#include "src-gen/three_thiangle_with_back.hpp"
#include "src-gen/three_rep_quiteandreturn.hpp"
#include "src-gen/square_events.hpp"
#include "src-gen/star_and_cycling.hpp"
#include "src-gen/semievent_and_conditions.hpp"
#include "src-gen/counter.hpp"
#include "src-gen/counter_transincr.hpp"
#include "src-gen/diff_cond.hpp"
#include "src-gen/cluster_onenode.hpp"
#include "src-gen/cluster_twonodes.hpp"
#include "src-gen/cluster_with_exit.hpp"
#include "src-gen/cluster_twonodes_ext_events.hpp"
#include "src-gen/cluster_multinodes.hpp"
#include "src-gen/cluster_multinodes_2.hpp"
#include "src-gen/cluster_multinodes_3.hpp"
#include "src-gen/cluster_multinodes_4.hpp"
#include "src-gen/cluster_2_multinodes.hpp"
#include "src-gen/cluster_rec2_multinodes.hpp"
#include "src-gen/cluster_rec2_multinodes_1.hpp"
#include "src-gen/cluster_rec2_multinodes_2.hpp"
#include "src-gen/cluster_rec2_multinodes_3.hpp"
#include "src-gen/cluster_rec2_multinodes_4.hpp"
#include "src-gen/cluster_rec2_multinodes_5.hpp"
#include "src-gen/include_1.hpp"
#include "src-gen/th_simple.hpp"
#include "src-gen/th_simple_param.hpp"
#include "src-gen/th_cluster_1.hpp"
#include "src-gen/th_cluster_2.hpp"
#include "src-gen/th_cluster_3.hpp"
#include "src-gen/th_cluster_4.hpp"
#include "src-gen/th_twoways.hpp"

// --------------------------------------------------------------------------
// Code implementation
// --------------------------------------------------------------------------

// -------------------------------------------------------------------------
void TestSet::setUp()
// -------------------------------------------------------------------------
{
}

// -------------------------------------------------------------------------
void TestSet::tearDown()
// -------------------------------------------------------------------------
{
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( One_state,
   STATE( One_state, Node ) 
);
void TestSet::testOneState( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( One_state );

   sequence.enter( "Node" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Two_states_uncond,
   STATE( Two_states_uncond, Init ) 
   STATE( Two_states_uncond, Node ) 
);
void TestSet::testTwoStatesUncond( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Two_states_uncond );

   sequence.enter( "Init" ).exit( "Init" ).enter( "Node" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Two_states_event,
   STATE( Two_states_event, Init )
   STATE( Two_states_event, Node )
);
void TestSet::testTwoStatesEvent( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Two_states_event );

   sequence.enter( "Init" ).check();

   chart.dispatch(&CTwo_states_eventFSM::state::trigger_e);
   sequence.exit( "Init" ).enter( "Node" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Three_dr_ev,
   STATE( Three_dr_ev, Init )
   STATE( Three_dr_ev, Node )
   STATE( Three_dr_ev, Cnode )
);
void TestSet::testThreeDirectEvent( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Three_dr_ev );
   
   sequence.enter( "Init" ).exit( "Init" ).enter( "Node" ).check();

   chart.dispatch( &CThree_dr_evFSM::state::trigger_ev );
   sequence.exit( "Node" ).enter( "Cnode" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Three_event_bicyclic,
   STATE( Three_event_bicyclic, Init )
   STATE( Three_event_bicyclic, A )
   STATE( Three_event_bicyclic, B )
);
void TestSet::testThreeBicyclicEvents( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Three_event_bicyclic );

   sequence.enter( "Init" ).exit( "Init" ).enter( "A" ).check();

   chart.dispatch( &CThree_event_bicyclicFSM::state::trigger_q );
   sequence.exit( "A" ).enter( "B" ).check();

   chart.dispatch( &CThree_event_bicyclicFSM::state::trigger_q );
   sequence.exit( "B" ).enter( "A" ).check();

   chart.dispatch( &CThree_event_bicyclicFSM::state::trigger_q );
   sequence.exit( "A" ).enter( "B" ).check();

   chart.dispatch( &CThree_event_bicyclicFSM::state::trigger_q );
   sequence.exit( "B" ).enter( "A" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Two_event_bicyclic_with_init,
   STATE( Two_event_bicyclic_with_init, Init )
   STATE( Two_event_bicyclic_with_init, S )
);
void TestSet::testTwoBicyclicEventsWithInit( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Two_event_bicyclic_with_init );

   sequence.enter( "Init" ).check();

   chart.dispatch( &CTwo_event_bicyclic_with_initFSM::state::trigger_q );
   sequence.exit( "Init" ).enter( "S" ).check();

   chart.dispatch( &CTwo_event_bicyclic_with_initFSM::state::trigger_q );
   sequence.exit( "S" ).enter( "Init" ).check();

   chart.dispatch( &CTwo_event_bicyclic_with_initFSM::state::trigger_q );
   sequence.exit( "Init" ).enter( "S" ).check();

   chart.dispatch( &CTwo_event_bicyclic_with_initFSM::state::trigger_q );
   sequence.exit( "S" ).enter( "Init" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Two_event_bicyclic_with_init_diffevents,
   STATE( Two_event_bicyclic_with_init_diffevents, Init )
   STATE( Two_event_bicyclic_with_init_diffevents, S )
);
void TestSet::testTwoBicyclicEventsWithInitDiffEvents( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Two_event_bicyclic_with_init_diffevents );

   sequence.enter( "Init" ).check();

   chart.dispatch( &CTwo_event_bicyclic_with_init_diffeventsFSM::state::trigger_q );
   sequence.exit( "Init" ).enter( "S" ).check();

   chart.dispatch( &CTwo_event_bicyclic_with_init_diffeventsFSM::state::trigger_r );
   sequence.exit( "S" ).enter( "Init" ).check();

   chart.dispatch( &CTwo_event_bicyclic_with_init_diffeventsFSM::state::trigger_q );
   sequence.exit( "Init" ).enter( "S" ).check();

   chart.dispatch( &CTwo_event_bicyclic_with_init_diffeventsFSM::state::trigger_r );
   sequence.exit( "S" ).enter( "Init" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Singlenode_selftrans_event,
   STATE( Singlenode_selftrans_event, Init )
);
void TestSet::testSinglenodeSelftransitionEvent( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Singlenode_selftrans_event );

   sequence.enter( "Init" ).check();

   chart.dispatch( &CSinglenode_selftrans_eventFSM::state::trigger_q );
   sequence.exit( "Init" ).enter( "Init" ).check();

   chart.dispatch( &CSinglenode_selftrans_eventFSM::state::trigger_q );
   sequence.exit( "Init" ).enter( "Init" ).check();

   chart.dispatch( &CSinglenode_selftrans_eventFSM::state::trigger_q );
   sequence.exit( "Init" ).enter( "Init" ).check();

   chart.dispatch( &CSinglenode_selftrans_eventFSM::state::trigger_q );
   sequence.exit( "Init" ).enter( "Init" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Three_fork,
   STATE( Three_fork, Init )
   STATE( Three_fork, A )
   STATE( Three_fork, B )
);
void TestSet::testThreeFork( void )
// --------------------------------------------------------------------------
{
   {
      RUN_TEST_FSM( Three_fork );

      sequence.enter( "Init" ).check();

      chart.dispatch( &CThree_forkFSM::state::trigger_q );
      sequence.exit( "Init" ).enter( "A" ).check();
   }
   {
      RUN_TEST_FSM( Three_fork );

      sequence.enter( "Init" ).check();

      chart.dispatch( &CThree_forkFSM::state::trigger_w );
      sequence.exit( "Init" ).enter( "B" ).check();
   }
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Three_thiangle_with_back,
   STATE( Three_thiangle_with_back, Init )
   STATE( Three_thiangle_with_back, A )
   STATE( Three_thiangle_with_back, B )
);
void TestSet::testThreeTriangleWithBack( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Three_thiangle_with_back );

   sequence.enter( "Init" ).check();

   chart.dispatch( &CThree_thiangle_with_backFSM::state::trigger_q );
   sequence.exit( "Init" ).enter( "A" ).check();

   chart.dispatch( &CThree_thiangle_with_backFSM::state::trigger_q );
   sequence.exit( "A" ).enter( "B" ).check();

   chart.dispatch( &CThree_thiangle_with_backFSM::state::trigger_q );
   sequence.exit( "B" ).enter( "Init" ).check();

   chart.dispatch( &CThree_thiangle_with_backFSM::state::trigger_w );
   sequence.exit( "Init" ).enter( "B" ).check();

   chart.dispatch( &CThree_thiangle_with_backFSM::state::trigger_q );
   sequence.exit( "B" ).enter( "Init" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Three_rep_quiteandreturn,
   STATE( Three_rep_quiteandreturn, Init )
   STATE( Three_rep_quiteandreturn, R )
   STATE( Three_rep_quiteandreturn, A )
);
void TestSet::testThreeRepeatQuiteAndReturn( void )
// ------------------------------------------------------------------------
{
    RUN_TEST_FSM( Three_rep_quiteandreturn );

    sequence.enter( "Init" ).exit( "Init" ).enter( "R" ).check();

    chart.dispatch( &CThree_rep_quiteandreturnFSM::state::trigger_r );
    sequence.exit( "R" ).enter( "R" ).check();

    chart.dispatch( &CThree_rep_quiteandreturnFSM::state::trigger_r );
    sequence.exit( "R" ).enter( "R" ).check();

    chart.dispatch( &CThree_rep_quiteandreturnFSM::state::trigger_r );
    sequence.exit( "R" ).enter( "R" ).check();

    chart.dispatch( &CThree_rep_quiteandreturnFSM::state::trigger_r );
    sequence.exit( "R" ).enter( "R" ).check();

    chart.dispatch( &CThree_rep_quiteandreturnFSM::state::trigger_q );
    sequence.exit( "R" ).enter( "A" ).exit( "A" ).enter( "R" ).check();

    chart.dispatch( &CThree_rep_quiteandreturnFSM::state::trigger_r );
    sequence.exit( "R" ).enter( "R" ).check();

    chart.dispatch( &CThree_rep_quiteandreturnFSM::state::trigger_r );
    sequence.exit( "R" ).enter( "R" ).check();

    chart.dispatch( &CThree_rep_quiteandreturnFSM::state::trigger_r );
    sequence.exit( "R" ).enter( "R" ).check();

    chart.dispatch( &CThree_rep_quiteandreturnFSM::state::trigger_r );
    sequence.exit( "R" ).enter( "R" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Square_events,
   STATE( Square_events, Init )
   STATE( Square_events, A )
   STATE( Square_events, B )
   STATE( Square_events, C )
   STATE( Square_events, D )
);
void TestSet::testSquareEvents( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Square_events );

   sequence.enter( "Init" ).exit( "Init" ).enter( "A" ).check();

   chart.dispatch( &CSquare_eventsFSM::state::trigger_ab );
   sequence.exit( "A" ).enter( "B" ).check();

   chart.dispatch( &CSquare_eventsFSM::state::trigger_bc );
   sequence.exit( "B" ).enter( "C" ).check();

   chart.dispatch( &CSquare_eventsFSM::state::trigger_cd );
   sequence.exit( "C" ).enter( "D" ).check();

   chart.dispatch( &CSquare_eventsFSM::state::trigger_da );
   sequence.exit( "D" ).enter( "A" ).check();

   chart.dispatch( &CSquare_eventsFSM::state::trigger_ab );
   sequence.exit( "A" ).enter( "B" ).check();

   chart.dispatch( &CSquare_eventsFSM::state::trigger_bd );
   sequence.exit( "B" ).enter( "D" ).check();

   chart.dispatch( &CSquare_eventsFSM::state::trigger_da );
   sequence.exit( "D" ).enter( "A" ).check();

   chart.dispatch( &CSquare_eventsFSM::state::trigger_ab );
   sequence.exit( "A" ).enter( "B" ).check();

   chart.dispatch( &CSquare_eventsFSM::state::trigger_bc );
   sequence.exit( "B" ).enter( "C" ).check();

   chart.dispatch( &CSquare_eventsFSM::state::trigger_ca );
   sequence.exit( "C" ).enter( "A" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Star_and_cycling,
   STATE( Star_and_cycling, Init )
   STATE( Star_and_cycling, 1 )
   STATE( Star_and_cycling, 2 )
   STATE( Star_and_cycling, 3 )
   STATE( Star_and_cycling, 4 )
);
void TestSet::testStarAndCycling( void )
// --------------------------------------------------------------------------
{
   {
      RUN_TEST_FSM( Star_and_cycling );
      sequence.enter( "Init" ).check();

      chart.dispatch( &CStar_and_cyclingFSM::state::trigger_q );
      sequence.exit( "Init" ).enter( "1" ).check();

      chart.dispatch( &CStar_and_cyclingFSM::state::trigger_z );
      sequence.exit( "1" ).enter( "2" ).check();

      chart.dispatch( &CStar_and_cyclingFSM::state::trigger_z );
      sequence.exit( "2" ).enter( "3" ).check();

      chart.dispatch( &CStar_and_cyclingFSM::state::trigger_z );
      sequence.exit( "3" ).enter( "4" ).check();
   }
   {
      RUN_TEST_FSM( Star_and_cycling );
      sequence.enter( "Init" ).check();

      chart.dispatch( &CStar_and_cyclingFSM::state::trigger_w );
      sequence.exit( "Init" ).enter( "2" ).check();

      chart.dispatch( &CStar_and_cyclingFSM::state::trigger_z );
      sequence.exit( "2" ).enter( "3" ).check();

      chart.dispatch( &CStar_and_cyclingFSM::state::trigger_z );
      sequence.exit( "3" ).enter( "4" ).check();

      chart.dispatch( &CStar_and_cyclingFSM::state::trigger_z );
      sequence.exit( "4" ).enter( "1" ).check();
   }
   {
      RUN_TEST_FSM( Star_and_cycling );
      sequence.enter( "Init" ).check();

      chart.dispatch( &CStar_and_cyclingFSM::state::trigger_e );
      sequence.exit( "Init" ).enter( "3" ).check();

      chart.dispatch( &CStar_and_cyclingFSM::state::trigger_z );
      sequence.exit( "3" ).enter( "4" ).check();

      chart.dispatch( &CStar_and_cyclingFSM::state::trigger_z );
      sequence.exit( "4" ).enter( "1" ).check();

      chart.dispatch( &CStar_and_cyclingFSM::state::trigger_z );
      sequence.exit( "1" ).enter( "2" ).check();
   }
   {
      RUN_TEST_FSM( Star_and_cycling );
      sequence.enter( "Init" ).check();

      chart.dispatch( &CStar_and_cyclingFSM::state::trigger_r );
      sequence.exit( "Init" ).enter( "4" ).check();

      chart.dispatch( &CStar_and_cyclingFSM::state::trigger_z );
      sequence.exit( "4" ).enter( "1" ).check();

      chart.dispatch( &CStar_and_cyclingFSM::state::trigger_z );
      sequence.exit( "1" ).enter( "2" ).check();

      chart.dispatch( &CStar_and_cyclingFSM::state::trigger_z );
      sequence.exit( "2" ).enter( "3" ).check();
   }
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Semievent_and_conditions,
   STATE( Semievent_and_conditions, Init )
   STATE( Semievent_and_conditions, A )
   STATE( Semievent_and_conditions, B )
   STATE( Semievent_and_conditions, C )
);
void TestSet::testSemieventAndConditions( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Semievent_and_conditions );
   sequence.enter( "Init" ).check();

   chart.setIndex( 5 );
   chart.dispatch( &CSemievent_and_conditionsFSM::state::trigger_q );
   sequence.exit( "Init" ).enter( "A" ).exit( "A" ).enter( "Init" ).check();

   chart.setIndex( 15 );
   chart.dispatch( &CSemievent_and_conditionsFSM::state::trigger_q );
   sequence.exit( "Init" ).enter( "B" ).exit( "B" ).enter( "Init" ).check();

   chart.setIndex( 10 );
   chart.dispatch( &CSemievent_and_conditionsFSM::state::trigger_q );
   sequence.exit( "Init" ).enter( "C" ).exit( "C" ).enter( "Init" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Counter,
   STATE( Counter, Init )
   virtual void stateCEnter( CCounterFSM::data_model& m ) {  m.value++; TestSet::sequence.action( "state_C::enter" ); }
   virtual void stateCExit( CCounterFSM::data_model& ) { TestSet::sequence.action( "state_C::exit" ); }
   STATE( Counter, F )
);
void TestSet::testCounter( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Counter );

   sequence.enter( "Init" ).exit( "Init" )
      .enter( "C" ).exit( "C" ).enter( "C" ).exit( "C" )
      .enter( "C" ).exit( "C" ).enter( "C" ).exit( "C" )
      .enter( "C" ).exit( "C" ).enter( "F" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Counter_transincr,
   STATE( Counter_transincr, Init )
   STATE( Counter_transincr, F )
);
void TestSet::testCounterTransIncr( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Counter_transincr );

   sequence.enter( "Init" ).exit( "Init" ).enter( "Init" ).exit( "Init" )
      .enter( "Init" ).exit( "Init" ).enter( "Init" ).exit( "Init" )
      .enter( "F" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Diff_cond,
   STATE( Diff_cond, Init )
   STATE( Diff_cond, A )
);
void TestSet::testDiffCond( void )
// --------------------------------------------------------------------------
{
   {
      RUN_TEST_FSM( Diff_cond );

      sequence.enter( "Init" ).check();

      chart.dispatch( &CDiff_condFSM::state::trigger_q );
      sequence.check();
   }
   
   {
      RUN_TEST_FSM( Diff_cond );

      sequence.enter( "Init" ).check();

      chart.setData( 7 );
      chart.setStr( "SET" );
      chart.setValue( 8 );
      chart.dispatch( &CDiff_condFSM::state::trigger_q );
      sequence.exit( "Init" ).enter( "A" ).exit( "A" ).enter( "Init" ).check();
   }
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Cluster_onenode,
   STATE( Cluster_onenode, Init )
   STATE( Cluster_onenode, Cluster )
   STATE( Cluster_onenode, A )
);
void TestSet::testClusterOneNode( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Cluster_onenode );

   sequence.enter( "Init" ).exit( "Init" ).enter( "Cluster" ).enter( "A" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Cluster_twonodes,
   STATE( Cluster_twonodes, Init )
   STATE( Cluster_twonodes, Cluster )
   STATE( Cluster_twonodes, A )
   STATE( Cluster_twonodes, B )
);
void TestSet::testClusterTwoNodes( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Cluster_twonodes );

   sequence.enter( "Init" ).exit( "Init" ).enter( "Cluster" ).enter( "A" ).exit( "A" ).enter( "B" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Cluster_with_exit,
   STATE( Cluster_with_exit, Init )
   STATE( Cluster_with_exit, Cluster )
   STATE( Cluster_with_exit, A )
   STATE( Cluster_with_exit, B )
   STATE( Cluster_with_exit, C )
);
void TestSet::testClusterWithExit( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Cluster_with_exit );

   sequence.enter( "Init" ).exit( "Init" ).enter( "Cluster" ).enter( "A" ).exit( "A" ).
      enter( "B" ).exit( "B" ).exit( "Cluster" ).enter( "C" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Cluster_twonodes_ext_events,
   STATE( Cluster_twonodes_ext_events, Init )
   STATE( Cluster_twonodes_ext_events, Cluster )
   STATE( Cluster_twonodes_ext_events, A )
   STATE( Cluster_twonodes_ext_events, B )
   STATE( Cluster_twonodes_ext_events, C )
);
void TestSet::testClusterTwoNodesEventsAndEventExit( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Cluster_twonodes_ext_events );

   sequence.enter( "Init" ).check();

   chart.dispatch( &CCluster_twonodes_ext_eventsFSM::state::trigger_qw );
   sequence.exit( "Init" ).enter( "Cluster" ).enter( "A" ).check();

   chart.dispatch( &CCluster_twonodes_ext_eventsFSM::state::trigger_qw );
   sequence.exit( "A" ).enter( "B" ).check();

   chart.dispatch( &CCluster_twonodes_ext_eventsFSM::state::trigger_we );
   sequence.exit( "B" ).exit( "Cluster" ).enter( "C" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Cluster_multinodes,
   STATE( Cluster_multinodes, Init )
   STATE( Cluster_multinodes, Cluster )
   STATE( Cluster_multinodes, A )
   STATE( Cluster_multinodes, B )
   STATE( Cluster_multinodes, C )
   STATE( Cluster_multinodes, D )
   STATE( Cluster_multinodes, E )
   STATE( Cluster_multinodes, F )
);
void TestSet::testClusterMultinodes( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Cluster_multinodes );

   sequence.enter( "Init" ).check();

   chart.dispatch( &CCluster_multinodesFSM::state::trigger_q );
   sequence.exit( "Init" ).enter( "A" ).exit( "A" ).enter( "Cluster" ).enter( "B" ).
      exit( "B" ).enter( "C" ).check();

   chart.dispatch( &CCluster_multinodesFSM::state::trigger_d );
   sequence.exit( "C" ).enter( "D" ).exit( "D" ).exit( "Cluster" ).enter( "F" ).check();

   chart.dispatch( &CCluster_multinodesFSM::state::trigger_w );
   sequence.exit( "F" ).enter( "A" ).exit( "A" ).enter( "Cluster" ).enter( "B" ).
      exit( "B" ).enter( "C" ).check();

   chart.dispatch( &CCluster_multinodesFSM::state::trigger_e );
   sequence.exit( "C" ).enter( "E" ).exit( "E" ).exit( "Cluster" ).enter( "F" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Cluster_multinodes_2,
   STATE( Cluster_multinodes_2, Init )
   STATE( Cluster_multinodes_2, Cluster )
   STATE( Cluster_multinodes_2, A )
   STATE( Cluster_multinodes_2, B )
   STATE( Cluster_multinodes_2, C )
   STATE( Cluster_multinodes_2, D )
   STATE( Cluster_multinodes_2, E )
   STATE( Cluster_multinodes_2, F )
);
void TestSet::testClusterMultinodes2( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Cluster_multinodes_2 );

   sequence.enter( "Init" ).check();

   chart.dispatch( &CCluster_multinodes_2FSM::state::trigger_q );
   sequence.exit( "Init" ).enter( "A" ).exit( "A" ).enter( "Cluster" ).enter( "B" ).
      exit( "B" ).enter( "C" ).check();

   chart.dispatch( &CCluster_multinodes_2FSM::state::trigger_d );
   sequence.exit( "C" ).enter( "D" ).check();

   chart.dispatch( &CCluster_multinodes_2FSM::state::trigger_yy );
   sequence.exit( "D" ).exit( "Cluster" ).enter( "F" ).check();

   chart.dispatch( &CCluster_multinodes_2FSM::state::trigger_w );
   sequence.exit( "F" ).enter( "A" ).exit( "A" ).enter( "Cluster" ).enter( "B" ).
      exit( "B" ).enter( "C" ).check();

   chart.dispatch( &CCluster_multinodes_2FSM::state::trigger_e );
   sequence.exit( "C" ).enter( "E" ).check();

   chart.dispatch( &CCluster_multinodes_2FSM::state::trigger_yy );
   sequence.exit( "E" ).exit( "Cluster" ).enter( "F" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Cluster_multinodes_3,
   STATE( Cluster_multinodes_3, Init )
   STATE( Cluster_multinodes_3, Cluster )
   STATE( Cluster_multinodes_3, A )
   STATE( Cluster_multinodes_3, B )
   STATE( Cluster_multinodes_3, C )
   STATE( Cluster_multinodes_3, D )
   STATE( Cluster_multinodes_3, F )
);
void TestSet::testClusterMultinodes3( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Cluster_multinodes_3 );

   sequence.enter( "Init" ).check();

   chart.dispatch( &CCluster_multinodes_3FSM::state::trigger_q );
   sequence.exit( "Init" ).enter( "A" ).exit( "A" ).enter( "Cluster" ).enter( "B" ).
   exit( "B" ).enter( "C" ).exit( "C" ).enter( "D" ).check();

   chart.dispatch( &CCluster_multinodes_3FSM::state::trigger_yy );
   sequence.exit( "D" ).exit( "Cluster" ).enter( "F" ).check();

   chart.dispatch( &CCluster_multinodes_3FSM::state::trigger_w );
   sequence.exit( "F" ).enter( "A" ).exit( "A" ).enter( "Cluster" ).enter( "B" ).
   exit( "B" ).enter( "C" ).exit( "C" ).enter( "D" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Cluster_multinodes_4,
   STATE( Cluster_multinodes_4, Init )
   STATE( Cluster_multinodes_4, Cluster )
   STATE( Cluster_multinodes_4, A )
   STATE( Cluster_multinodes_4, B )
   STATE( Cluster_multinodes_4, C )
   STATE( Cluster_multinodes_4, D )
   STATE( Cluster_multinodes_4, F )
);
void TestSet::testClusterMultinodes4( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Cluster_multinodes_4 );

   sequence.enter( "Init" ).check();

   chart.dispatch( &CCluster_multinodes_4FSM::state::trigger_q );
   sequence.exit( "Init" ).enter( "A" ).exit( "A" ).enter( "Cluster" ).enter( "B" ).
   exit( "B" ).enter( "C" ).exit( "C" ).enter( "D" ).check();

   chart.dispatch( &CCluster_multinodes_4FSM::state::trigger_yy );
   sequence.exit( "D" ).exit( "Cluster" ).enter( "F" ).check();

   chart.dispatch( &CCluster_multinodes_4FSM::state::trigger_w );
   sequence.exit( "F" ).enter( "A" ).exit( "A" ).enter( "Cluster" ).enter( "B" ).
   exit( "B" ).enter( "C" ).exit( "C" ).enter( "D" ).check();

   chart.dispatch( &CCluster_multinodes_4FSM::state::trigger_r );
   sequence.exit( "D" ).enter( "B" ).exit( "B" ).enter( "C" ).exit( "C" ).enter( "D" ).check();

   chart.dispatch( &CCluster_multinodes_4FSM::state::trigger_yy );
   sequence.exit( "D" ).exit( "Cluster" ).enter( "F" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Cluster_2_multinodes,
   STATE( Cluster_2_multinodes, Init )
   STATE( Cluster_2_multinodes, C_init )
   STATE( Cluster_2_multinodes, Cluster )
   STATE( Cluster_2_multinodes, Cluster_2 )
   STATE( Cluster_2_multinodes, A )
   STATE( Cluster_2_multinodes, B )
   STATE( Cluster_2_multinodes, C )
   STATE( Cluster_2_multinodes, D )
   STATE( Cluster_2_multinodes, G )
);
void TestSet::testCluster2Multinodes( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Cluster_2_multinodes );

   sequence.enter( "Init" ).check();

   chart.dispatch( &CCluster_2_multinodesFSM::state::trigger_q );
   sequence.exit( "Init" ).enter( "A" ).exit( "A" ).enter( "Cluster" ).enter( "B" ).
   exit( "B" ).enter( "C" ).exit( "C" ).enter( "D" ).check();

   chart.dispatch( &CCluster_2_multinodesFSM::state::trigger_yy );
   sequence.exit( "D" ).exit( "Cluster" ).enter( "Cluster_2" ).enter( "C_init" ).check();

   chart.dispatch( &CCluster_2_multinodesFSM::state::trigger_y );
   sequence.exit( "C_init" ).enter( "G" ).exit( "G" ).exit( "Cluster_2" ).enter( "A" )
      .exit( "A" ).enter( "Cluster" ).enter( "B" ).exit( "B" ).enter( "C" )
      .exit( "C" ).enter( "D" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Cluster_rec2_multinodes,
   STATE( Cluster_rec2_multinodes, Init )
   STATE( Cluster_rec2_multinodes, Cluster )
   STATE( Cluster_rec2_multinodes, Cluster_2 )
   STATE( Cluster_rec2_multinodes, A )
   STATE( Cluster_rec2_multinodes, I )
   STATE( Cluster_rec2_multinodes, B )
   STATE( Cluster_rec2_multinodes, R )
   STATE( Cluster_rec2_multinodes, D )
);
void TestSet::testClusterRec2Multinodes( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Cluster_rec2_multinodes );

   sequence.enter( "Init" ).check();

   chart.dispatch( &CCluster_rec2_multinodesFSM::state::trigger_q );
   sequence.exit( "Init" ).enter( "A" ).exit( "A" ).enter( "Cluster" ).enter( "B" ).exit( "B" ).enter( "Cluster_2" ).enter( "I" ).exit( "I" ).enter( "R" ).exit( "R" )
      .exit( "Cluster_2" ).enter( "D" ).check();

   chart.dispatch( &CCluster_rec2_multinodesFSM::state::trigger_r );
   sequence.exit( "D" ).enter( "B" ).exit( "B" ).enter( "Cluster_2" ).enter( "I" ).exit( "I" ).enter( "R" ).exit( "R" ).exit( "Cluster_2" ).enter( "D" ).check();

   chart.dispatch( &CCluster_rec2_multinodesFSM::state::trigger_t );
   sequence.exit( "D" ).exit( "Cluster" ).enter( "A" ).exit( "A" ).enter( "Cluster" ).enter( "B" ).exit( "B" ).enter( "Cluster_2" ).enter( "I" ).exit( "I" ).enter( "R" ).exit( "R" )
      .exit( "Cluster_2" ).enter( "D" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Cluster_rec2_multinodes_1,
   STATE( Cluster_rec2_multinodes_1, Init )
   STATE( Cluster_rec2_multinodes_1, Cluster )
   STATE( Cluster_rec2_multinodes_1, Cluster_2 )
   STATE( Cluster_rec2_multinodes_1, A )
   STATE( Cluster_rec2_multinodes_1, I )
   STATE( Cluster_rec2_multinodes_1, B )
   STATE( Cluster_rec2_multinodes_1, R )
   STATE( Cluster_rec2_multinodes_1, D )
);
void TestSet::testClusterRec2Multinodes_1( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Cluster_rec2_multinodes_1 );

   sequence.enter( "Init" ).check();

   chart.dispatch( &CCluster_rec2_multinodes_1FSM::state::trigger_q );
   sequence.exit( "Init" ).enter( "A" ).exit( "A" ).enter( "Cluster" ).enter( "B" )
      .exit( "B" ).enter( "Cluster_2" ).enter( "I" ).check();
      
   chart.dispatch( &CCluster_rec2_multinodes_1FSM::state::trigger_u );
   sequence.exit( "I" ).enter( "R" ).exit( "R" ).exit( "Cluster_2" ).enter( "D" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Cluster_rec2_multinodes_2,
   STATE( Cluster_rec2_multinodes_2, Init )
   STATE( Cluster_rec2_multinodes_2, Cluster )
   STATE( Cluster_rec2_multinodes_2, Cluster_2 )
   STATE( Cluster_rec2_multinodes_2, A )
   STATE( Cluster_rec2_multinodes_2, I )
   STATE( Cluster_rec2_multinodes_2, B )
   STATE( Cluster_rec2_multinodes_2, R )
   STATE( Cluster_rec2_multinodes_2, F )
);
void TestSet::testClusterRec2Multinodes_2( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Cluster_rec2_multinodes_2 );

   sequence.enter( "Init" ).check();

   chart.dispatch( &CCluster_rec2_multinodes_2FSM::state::trigger_q );
   sequence.exit( "Init" ).enter( "A" ).exit( "A" ).enter( "Cluster" ).enter( "B" )
      .exit( "B" ).enter( "Cluster_2" ).enter( "I" ).check();
      
   chart.dispatch( &CCluster_rec2_multinodes_2FSM::state::trigger_u );
   sequence.exit( "I" ).enter( "R" ).exit( "R" ).exit( "Cluster_2" ).exit( "Cluster" )
      .enter( "F" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Cluster_rec2_multinodes_3,
   STATE( Cluster_rec2_multinodes_3, Init )
   STATE( Cluster_rec2_multinodes_3, Cluster )
   STATE( Cluster_rec2_multinodes_3, Cluster_2 )
   STATE( Cluster_rec2_multinodes_3, A )
   STATE( Cluster_rec2_multinodes_3, I )
   STATE( Cluster_rec2_multinodes_3, B )
   STATE( Cluster_rec2_multinodes_3, R )
   STATE( Cluster_rec2_multinodes_3, F )
);
void TestSet::testClusterRec2Multinodes_3( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Cluster_rec2_multinodes_3 );

   sequence.enter( "Init" ).check();

   chart.dispatch( &CCluster_rec2_multinodes_3FSM::state::trigger_q );
   sequence.exit( "Init" ).enter( "A" ).exit( "A" ).enter( "Cluster" ).enter( "B" )
      .exit( "B" ).enter( "Cluster_2" ).enter( "I" ).check();
      
   chart.dispatch( &CCluster_rec2_multinodes_3FSM::state::trigger_u );
   sequence.exit( "I" ).enter( "R" ).check();

   chart.dispatch( &CCluster_rec2_multinodes_3FSM::state::trigger_r );
   sequence.exit( "R" ).exit( "Cluster_2" ).exit( "Cluster" ).enter( "F" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Cluster_rec2_multinodes_4,
   STATE( Cluster_rec2_multinodes_4, Init )
   STATE( Cluster_rec2_multinodes_4, Cluster )
   STATE( Cluster_rec2_multinodes_4, Cluster_2 )
   STATE( Cluster_rec2_multinodes_4, A )
   STATE( Cluster_rec2_multinodes_4, C )
   STATE( Cluster_rec2_multinodes_4, G )
   STATE( Cluster_rec2_multinodes_4, I )
   STATE( Cluster_rec2_multinodes_4, B )
   STATE( Cluster_rec2_multinodes_4, R )
   STATE( Cluster_rec2_multinodes_4, F )
);
void TestSet::testClusterRec2Multinodes_4( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Cluster_rec2_multinodes_4 );

   sequence.enter( "Init" ).check();

   chart.dispatch( &CCluster_rec2_multinodes_4FSM::state::trigger_q );
   sequence.exit( "Init" ).enter( "A" ).exit( "A" ).enter( "Cluster" ).enter( "B" )
      .exit( "B" ).enter( "Cluster_2" ).enter( "I" ).check();
      
   chart.dispatch( &CCluster_rec2_multinodes_4FSM::state::trigger_u );
   sequence.exit( "I" ).enter( "R" ).exit( "R" ).exit( "Cluster_2" ).enter( "C" ).check();

   chart.dispatch( &CCluster_rec2_multinodes_4FSM::state::trigger_j );
   sequence.exit( "C" ).enter( "Cluster_2" ).enter( "I" ).check();

   chart.dispatch( &CCluster_rec2_multinodes_4FSM::state::trigger_u );
   sequence.exit( "I" ).enter( "R" ).exit( "R" ).exit( "Cluster_2" ).enter( "C" ).check();

   chart.dispatch( &CCluster_rec2_multinodes_4FSM::state::trigger_k );
   sequence.exit( "C" ).enter( "G" ).check();

   chart.dispatch( &CCluster_rec2_multinodes_4FSM::state::trigger_d );
   sequence.exit( "G" ).exit( "Cluster" ).enter( "Cluster" ).enter( "B" )
      .exit( "B" ).enter( "Cluster_2" ).enter( "I" ).check();

   chart.dispatch( &CCluster_rec2_multinodes_4FSM::state::trigger_u );
   sequence.exit( "I" ).enter( "R" ).exit( "R" ).exit( "Cluster_2" ).enter( "C" ).check();
   
   chart.dispatch( &CCluster_rec2_multinodes_4FSM::state::trigger_k );
   sequence.exit( "C" ).enter( "G" ).check();

   chart.dispatch( &CCluster_rec2_multinodes_4FSM::state::trigger_r );
   sequence.exit( "G" ).exit( "Cluster" ).enter( "F" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Cluster_rec2_multinodes_5,
   STATE( Cluster_rec2_multinodes_5, Init )
   STATE( Cluster_rec2_multinodes_5, Cluster )
   STATE( Cluster_rec2_multinodes_5, Cluster_2 )
   STATE( Cluster_rec2_multinodes_5, C )
   STATE( Cluster_rec2_multinodes_5, G )
   STATE( Cluster_rec2_multinodes_5, I )
   STATE( Cluster_rec2_multinodes_5, B )
   STATE( Cluster_rec2_multinodes_5, R )
   STATE( Cluster_rec2_multinodes_5, F )
);
void TestSet::testClusterRec2Multinodes_5( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Cluster_rec2_multinodes_5 );

   sequence.enter( "Init" ).exit( "Init" ).enter( "Cluster" ).enter( "B" ).exit( "B" ).enter( "Cluster_2" ).enter( "I" ).exit( "I" ).enter( "R" ).check();
      
   chart.dispatch( &CCluster_rec2_multinodes_5FSM::state::trigger_y );
   sequence.exit( "R" ).exit( "Cluster_2" ).enter( "Cluster_2" ).enter( "I" ).exit( "I" ).enter( "R" ).check();

   chart.dispatch( &CCluster_rec2_multinodes_5FSM::state::trigger_k );
   sequence.exit( "R" ).exit( "Cluster_2" ).enter( "C" ).check();

   chart.dispatch( &CCluster_rec2_multinodes_5FSM::state::trigger_j );
   sequence.exit( "C" ).enter( "Cluster_2" ).enter( "I" ).exit( "I" ).enter( "R" ).check();

   chart.dispatch( &CCluster_rec2_multinodes_5FSM::state::trigger_k );
   sequence.exit( "R" ).exit( "Cluster_2" ).enter( "C" ).check();

   chart.dispatch( &CCluster_rec2_multinodes_5FSM::state::trigger_k );
   sequence.exit( "C" ).enter( "G" ).check();

   chart.dispatch( &CCluster_rec2_multinodes_5FSM::state::trigger_r );
   sequence.exit( "G" ).exit( "Cluster" ).enter( "F" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Include_1,
   STATE( Include_1, Init )
   STATE( Include_1, Full )
   STATE( Include_1, Empty )
);
void TestSet::testInclude_1( void )
// --------------------------------------------------------------------------
{
   {
      RUN_TEST_FSM( Include_1 );
      chart.init();
      sequence.enter( "Init" ).check();

      chart.dispatch( &CInclude_1FSM::state::trigger_q );
      sequence.exit( "Init" ).enter( "Empty" ).check();
   }
   
   {
      RUN_TEST_FSM( Include_1 );
      chart.init();
      sequence.enter( "Init" ).check();

      std::vector<int> k;
      k.push_back( 9 );
      chart.setV( k );

      chart.dispatch( &CInclude_1FSM::state::trigger_q );
      sequence.exit( "Init" ).enter( "Full" ).check();
   }
}
   
// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Th_simple,
   STATE( Th_simple, Init )
   STATE( Th_simple, A )
   ACTION( Th_simple, Foo )
);
void TestSet::testTHSimple( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Th_simple );
   sequence.enter( "Init" ).exit( "Init" ).tr_action( "Foo" ).enter( "A" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Th_simple_param,
   STATE( Th_simple_param, Init )
   STATE( Th_simple_param, A )
   ACTION( Th_simple_param, Foo, int )
);
void TestSet::testTHSimpleParam( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Th_simple_param );
   sequence.enter( "Init" ).exit( "Init" ).tr_action( "Foo" ).enter( "A" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Th_cluster_1,
   STATE( Th_cluster_1, Init )
   STATE( Th_cluster_1, Cluster )
   STATE( Th_cluster_1, I )
   STATE( Th_cluster_1, A )
   STATE( Th_cluster_1, B )
   ACTION( Th_cluster_1, Foo )
   ACTION( Th_cluster_1, Bar )
);
void TestSet::testTHCluster1( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Th_cluster_1 );

   sequence.enter( "Init" ).exit( "Init" ).enter( "Cluster" ).enter( "I" ).check();
   chart.dispatch( &CTh_cluster_1FSM::state::trigger_q );
   sequence.exit( "I" ).tr_action( "Foo" ).enter( "A" ).check();
   chart.dispatch( &CTh_cluster_1FSM::state::trigger_q );
   sequence.exit( "A" ).exit( "Cluster" ).tr_action( "Bar" ).enter( "B" ).check();
} 

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Th_cluster_2,
   STATE( Th_cluster_2, Init )
   STATE( Th_cluster_2, Cluster )
   STATE( Th_cluster_2, I )
   STATE( Th_cluster_2, A )
   STATE( Th_cluster_2, B )
   STATE( Th_cluster_2, C )
   ACTION( Th_cluster_2, Foo )
   ACTION( Th_cluster_2, Bar )
);
void TestSet::testTHCluster2( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Th_cluster_2 );

   sequence.enter( "Init" ).exit( "Init" ).enter( "Cluster" ).enter( "I" ).exit( "I" ).tr_action( "Foo" ).enter( "A" ).exit( "A" ).exit( "Cluster" ).tr_action( "Bar" ).enter( "B" )
      .exit( "B" ).enter( "Cluster" ).enter( "I" ).exit( "I" ).tr_action( "Foo" ).enter( "A" ).exit( "A" ).exit( "Cluster" ).enter( "C" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Th_cluster_3,
   STATE( Th_cluster_3, Init )
   STATE( Th_cluster_3, Cluster )
   STATE( Th_cluster_3, I )
   STATE( Th_cluster_3, A )
   STATE( Th_cluster_3, B )
   STATE( Th_cluster_3, C )
   STATE( Th_cluster_3, D )
   ACTION( Th_cluster_3, Foo )
   ACTION( Th_cluster_3, Bar )
   ACTION( Th_cluster_3, Gap )
   ACTION( Th_cluster_3, Sub, int )
);
void TestSet::testTHCluster3( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Th_cluster_3 );

   sequence.enter( "Init" ).exit( "Init" ).enter( "Cluster" ).enter( "I" ).exit( "I" ).tr_action( "Foo" ).enter( "A" ).exit( "A" ).exit( "Cluster" ).tr_action( "Bar" ).enter( "B" )
      .exit( "B" ).tr_action( "Gap" ).enter( "B" ).exit( "B" ).tr_action( "Gap" ).enter( "B" ).exit( "B" ).tr_action( "Sub" ).enter( "C" ).check();

   chart.dispatch( &CTh_cluster_3FSM::state::trigger_w );
   sequence.exit( "C" ).tr_action( "Gap" ).enter( "D" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Th_cluster_4,
   STATE( Th_cluster_4, Init )
   STATE( Th_cluster_4, Cluster )
   STATE( Th_cluster_4, I )
   STATE( Th_cluster_4, A )
   STATE( Th_cluster_4, B )
   STATE( Th_cluster_4, C )
   STATE( Th_cluster_4, D )
   STATE( Th_cluster_4, Ii )
   STATE( Th_cluster_4, G )
   ACTION( Th_cluster_4, Foo )
   ACTION( Th_cluster_4, Bar )
   ACTION( Th_cluster_4, Gap )
   ACTION( Th_cluster_4, Sub, int )
);
void TestSet::testTHCluster4( void )
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM( Th_cluster_4 );

   sequence.enter( "Init" ).exit( "Init" ).enter( "Cluster" ).enter( "I" ).exit( "I" ).tr_action( "Foo" ).enter( "A" ).exit( "A" ).exit( "Cluster" ).tr_action( "Bar" ).enter( "B" )
      .enter( "Ii" ).exit( "Ii" ).enter( "G" ).exit( "G" ).exit( "B" ).tr_action( "Gap" ).enter( "B" ).enter( "Ii" ).exit( "Ii" ).enter( "G" ).exit( "G" ).exit( "B" ).tr_action( "Gap" )
      .enter( "B" ).enter( "Ii" ).exit( "Ii" ).enter( "G" ).exit( "G" ).exit( "B" ).tr_action( "Sub" ).enter( "C" ).check();

   chart.dispatch( &CTh_cluster_4FSM::state::trigger_w );
   sequence.exit( "C" ).tr_action( "Gap" ).enter( "D" ).check();
}

// --------------------------------------------------------------------------
DEFINE_TEST_CASE( Th_twoways,
   STATE( Th_twoways, Init )
   STATE( Th_twoways, A )
   STATE( Th_twoways, B )
   STATE( Th_twoways, C )
   STATE( Th_twoways, D )
   ACTION( Th_twoways, Foo )
   ACTION( Th_twoways, Bar )
);
void TestSet::testTHTwoWays(void)
// --------------------------------------------------------------------------
{
   RUN_TEST_FSM(Th_twoways);

   sequence.enter("Init").exit("Init").enter("A").check();

   chart.dispatch(&CTh_twowaysFSM::state::trigger_q);
   sequence.exit("A").enter("B").exit("B").tr_action("Foo").enter("D").exit("D").enter("A").check();

   chart.dispatch(&CTh_twowaysFSM::state::trigger_w);
   sequence.exit("A").enter("C").exit("C").tr_action("Bar").enter("D").exit("D").enter("A").check();
}
