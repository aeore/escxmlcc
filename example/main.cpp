/**
 * @file main.cpp
 * @author Yuriy Gurin <ygurin@outlook.com>
 */

#include <iostream>
#include "src-gen/example.hpp"

class Implementation: protected CExampleFSM::ActionHandlerInterface {
   CExampleFSM _fsm;

protected:
   // CExampleFSM::ActionHandlerInterface
   virtual inline void stateInitEnter( CExampleFSM::data_model& ) override {
      std::cout << "Init::ENTER" << std::endl;
   }

   virtual inline void stateInitExit( CExampleFSM::data_model& ) override {
      std::cout << "Init::EXIT" << std::endl;
   }

   virtual inline void stateStateAEnter( CExampleFSM::data_model& ) override {
      std::cout << "A::ENTER" << std::endl;
   }

   virtual inline void stateStateAExit( CExampleFSM::data_model& ) override {
      std::cout << "A::EXIT" << std::endl;
   }

   virtual inline void stateStateBEnter( CExampleFSM::data_model& ) override {
      std::cout << "B::ENTER" << std::endl;
   }

   virtual inline void stateStateBExit( CExampleFSM::data_model& ) override {
      std::cout << "B::EXIT" << std::endl;
   }

   virtual inline void stateStateCEnter( CExampleFSM::data_model& ) override {
      std::cout << "C::ENTER" << std::endl;
   }

   virtual inline void stateStateCExit( CExampleFSM::data_model& ) override {
      std::cout << "C::EXIT" << std::endl;
   }

   virtual inline void stateStateDEnter( CExampleFSM::data_model& ) override {
      std::cout << "D::ENTER" << std::endl;
   }

   virtual inline void stateStateDExit( CExampleFSM::data_model& ) override {
      std::cout << "D::EXIT" << std::endl;
   }

   virtual inline void stateFailedEnter( CExampleFSM::data_model& ) override {
      std::cout << "Failed::ENTER" << std::endl;
   }

   virtual inline void stateFailedExit( CExampleFSM::data_model& ) override {
      std::cout << "Failed::EXIT" << std::endl;
   }

   virtual inline void onMakeTransitionToD( CExampleFSM::data_model& m ) override {
      std::cout << "onMakeTransitionToD(" << m.value << ")" << std::endl;
   }

   virtual inline void onMakeTransitionToA( CExampleFSM::data_model& m ) override {
      std::cout << "onMakeTransitionToA(" << m.value << ")" << std::endl;
   }

   virtual inline void onMakeReport( CExampleFSM::data_model& ) override {
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
