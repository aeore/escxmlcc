/**
 * @file main.cpp
 * @author Yuriy Gurin <ygurin@outlook.com>
 */

#include <iostream>
#include "src-gen/example.hpp"

class Implementation: protected IExampleActionHandler {
   CExampleFSM _fsm;

protected:
   // IExampleActionHandler
   virtual void stateInitEnter( CExampleFSM::data_model & ) {
      std::cout << "Init::ENTER" << std::endl;
   }
   
   virtual void stateInitExit( CExampleFSM::data_model & ) {
      std::cout << "Init::EXIT" << std::endl;
   }
   
   virtual void stateStateAEnter( CExampleFSM::data_model & ) {
      std::cout << "A::ENTER" << std::endl;
   }
   
   virtual void stateStateAExit( CExampleFSM::data_model & ) {
      std::cout << "A::EXIT" << std::endl;
   }
   
   virtual void stateStateBEnter( CExampleFSM::data_model & ) {
      std::cout << "B::ENTER" << std::endl;
   }
   
   virtual void stateStateBExit( CExampleFSM::data_model & ) {
      std::cout << "B::EXIT" << std::endl;
   }
   
   virtual void stateStateCEnter( CExampleFSM::data_model & ) {
      std::cout << "C::ENTER" << std::endl;
   }
   
   virtual void stateStateCExit( CExampleFSM::data_model & ) {
      std::cout << "C::EXIT" << std::endl;
   }
   
   virtual void stateStateDEnter( CExampleFSM::data_model & ) {
      std::cout << "D::ENTER" << std::endl;
   }
   
   virtual void stateStateDExit( CExampleFSM::data_model & ) {
      std::cout << "D::EXIT" << std::endl;
   }
   
   virtual void stateFailedEnter( CExampleFSM::data_model & ) {
      std::cout << "Failed::ENTER" << std::endl;
   }

   virtual void stateFailedExit( CExampleFSM::data_model & ) {
      std::cout << "Failed::EXIT" << std::endl;
   }

   virtual void onMakeTransitionToD( CExampleFSM::data_model& m ) {
      std::cout << "onMakeTransitionToD(" << m.value << ")" << std::endl;
   }

   virtual void onMakeTransitionToA( CExampleFSM::data_model& m ) {
      std::cout << "onMakeTransitionToA(" << m.value << ")" << std::endl;
   }

   virtual void onMakeReport( CExampleFSM::data_model& ) {
      std::cout << "onMakeReport()" << std::endl;
   }
   
public:
   Implementation()
      : _fsm( this ) /* setting action handler */ {
   }

   void run() {
      _fsm.setValue(1); // fsm has one field in the data model
      _fsm.init(); // run fsm

      std::cout << "  Calling 'success' trigger" << std::endl;
      _fsm.dispatch( &CExampleFSM::state::trigger_success );

      std::cout << "  Calling 'success' trigger" << std::endl;
      _fsm.dispatch( &CExampleFSM::state::trigger_success ); // after this we have one autotransition
      
      std::cout << "  Calling 'success' trigger" << std::endl;
      _fsm.dispatch( &CExampleFSM::state::trigger_success );
      
      _fsm.setValue(2); // lets choose other branch
      
      std::cout << "  Calling 'success' trigger" << std::endl;
      _fsm.dispatch( &CExampleFSM::state::trigger_success );
      
      std::cout << "  Calling 'failed' trigger" << std::endl;
      _fsm.dispatch( &CExampleFSM::state::trigger_failed );
   }
};

int main( int argc, char** argv ) {
   (void)argc; (void)argv;
   std::cout << "escxmlcc example" << std::endl;
   std::cout << std::endl;
   
   Implementation impl;
   impl.run();
   
   return 0;
}
