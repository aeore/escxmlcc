#ifndef UT_SEQUENCE_HPP
#define UT_SEQUENCE_HPP

/****************************************************************************
 * Project         UT
 * (c) copyright   2015
 * Company         Yuriy Gurin RnD
 *                 All rights reserved
 ****************************************************************************/
/**
 * @file    Sequence.hpp
 * @ingroup SCXMLCC::UT
 * @author  YGurin
 *
 * @brief   
 */

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include <vector>
#include <string>
#include <iostream>
//#include "Environment/Environment.hpp"

// --------------------------------------------------------------------------
// Code definition
// --------------------------------------------------------------------------

class Sequence
{
public:
   void action( std::string action );
   Sequence& check();
   Sequence& clear();
   Sequence& enter( std::string name );
   Sequence& exit( std::string name );
   inline Sequence& tr_action( std::string name );

private:
   std::vector<std::string> mActions;
   std::vector<std::string> mSequence;
};

#define DEFINE_TEST_CASE( cls, ... ) \
class C##cls##AH: public C##cls##FSM::ActionHandlerInterface { __VA_ARGS__ };

#define STATE( cls, name ) \
   virtual void state##name##Enter( C##cls##FSM::data_model& ) override { TestSet::sequence.action( "state_"#name"::enter" ); } \
   virtual void state##name##Exit( C##cls##FSM::data_model& ) override { TestSet::sequence.action( "state_"#name"::exit" ); }

#define RUN_TEST_FSM( cls ) \
   C##cls##AH ah; \
   C##cls##FSM chart( &ah ); \
   chart.init();

#define ACTION( cls, name, ... ) \
   virtual void on##name( C##cls##FSM::data_model&, ##__VA_ARGS__ ) override { TestSet::sequence.action( "action_"#name ); }
/*
class D{};
DECLARE_EVENT(E, D, I);

template <typename T, typename H>
class TEFCaseMonad : public I, H
{
private:
   typedef enum { rtAction, rtData, rtFunction } eDataType;

public:
   TEFCaseMonad() : fsm(this) {}
   T fsm;

   void check(std::string s)
   {
      uRecord& r = mSequence.front();
      if (r.type != rtData)
         logme_error("Expected DATA but other type found");
      if (r.checked != s)
         logme_error("Different state obtained: Expected %s but found %s", r.checked.c_str(), s.c_str());
      mSequence.erase(mSequence.begin());
   }

   void error_unexpected_data(){
      logme_error("Expected data but other type found");
      CEnvironment::terminateAsync(1);
   }

   TEFCaseMonad& enter(std::string str)
   {
      mSequence.push_back(uRecord("state_" + str + "::enter"));
      return *this;
   }

   TEFCaseMonad& exit(std::string str)
   {
      mSequence.push_back(uRecord("state_" + str + "::exit"));

      return *this;
   }

   TEFCaseMonad& lambda(const std::function<void(T&)>& f)
   {
      mSequence.push_back(uRecord(f));
      return *this;
   }

   TEFCaseMonad& dispatch_sync(typename T::event event)
   {
      mSequence.push_back(uRecord(cSync, event));
      return *this;
   }

   TEFCaseMonad& dispatch_async(typename T::event event)
   {
      mSequence.push_back(uRecord(cAsync, event));
      return *this;
   }

   TEFCaseMonad& dispatch_async()
   {
      mSequence.push_back(uRecord(cAsyncEmpty));
      return *this;
   }

   void handle()
   {
      if (mSequence.size() > 0)
      {
         uRecord r = mSequence.front();
         mSequence.erase(mSequence.begin());

         switch (r.type)
         {
         case rtFunction:
            r.function( fsm );
            handle();
            break;

         case rtAction:
            if (r.action.async != cAsyncEmpty)
               fsm.dispatch(r.action.event);

            if (r.action.async == cAsync || r.action.async == cAsyncEmpty)
            {
               E* e = E::createEvent(D());
               e->setConsumer(this);
               e->send();
            }
            else
            {
               handle();
            }
            break;

         default:
            logme_error("Handle found unsuported action");
            break;
         }
      }
      else
      {
         CEnvironment::terminateAsync(0);
      }
   }

   inline virtual void processEvent(const E& event) {
      handle();
   }

   void run()
   {
      fsm.init();
      handle();
   }

   typedef enum { cSync, cAsync, cAsyncEmpty } eCall;
   typedef struct Event
   {
      Event(){}
      Event(eCall _async) : async(_async) {}
      Event(eCall _async, typename T::event _event) : async(_async), event(_event) {}
      eCall async;
      typename T::event event;

   } sEvent;

   typedef struct Record {
      Record(std::string s) : checked(s), type(rtData) {}
      Record(eCall async) : action(async), type(rtAction) {}
      Record(eCall async, typename T::event e) : action(async, e), type(rtAction) {}
      Record(const std::function<void(T&)>& func) : type(rtFunction), function(func){}

      eDataType type;
      std::string checked;
      sEvent action;
      std::function<void(T& fsm)> function;
   } uRecord;

   std::vector<uRecord> mSequence;
};

#define ENVIRONMENT_FSM_CASE( testcase, sequence ) \
   class Q : public IServiceLifecycle { public: C##testcase##Handler<C##testcase##FSM, C##testcase##ActionHandlerDefault> seq;\
   void processServiceStartup( IServiceParameters& params ) { params.assignTraceStrategy(new NATIVE_CONTRACER(std::cout));  seq.sequence.run(); }\
   void processServiceTeardown(void) {} };\
   CPPUNIT_ASSERT(CEnvironment::instance().run<Q>(0, 0) == 0);

#define ENV_CASE(testcase, ...) \
   template <typename F, typename H> class C##testcase##Handler : public TEFCaseMonad<C##testcase##FSM, C##testcase##ActionHandlerDefault> { __VA_ARGS__ };

#define ENV_STATE(testcase, name) \
   virtual void state##name##Enter(typename C##testcase##FSM::data_model& m) { check("state_"#name"::enter"); }\
   virtual void state##name##Exit(typename C##testcase##FSM::data_model& m) { check("state_"#name"::exit"); }
*/

// --------------------------------------------------------------------------
inline Sequence& Sequence::tr_action( std::string name )
// --------------------------------------------------------------------------
{
   mSequence.push_back( "action_" + name );
   return *this;
}

#endif // UT_SEQUENCE_HPP
