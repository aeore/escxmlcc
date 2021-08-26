#ifndef AUTOFSM_ASYNC_TRANSITION_15_HPP
#define AUTOFSM_ASYNC_TRANSITION_15_HPP

/*
 *                                     escxmlcc product
 *                                           fsm
 *
 * @ingroup Autogenerated::FSM
 * @author  escxmlcc
 * 
 * @brief   This program was generated by escxmlcc fsm compiler written by Yuriy Gurin. This is
 *          an extended version of original scxmlcc compiler written by Jan Pedersen. For more
 *          information see project website http://escxmlcc.sourceforge.net
 */

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include <typeinfo>
#include <memory>
#include "Environment/Environment.hpp"

// --------------------------------------------------------------------------
// User space
// --------------------------------------------------------------------------

/* no user data */

// --------------------------------------------------------------------------
// Code definition
// --------------------------------------------------------------------------

class IAsync_transition_15ActionHandler;
class CAsync_transition_15FSM
{
public:
	struct data_model {
		friend class CAsync_transition_15FSM;
		data_model( void ): value() { /* nop */ }
		int value;
	private:
		IAsync_transition_15ActionHandler* actionHandler;
	} model;

public:
	// --------------------------------------------------------------------------
	void setValue( const int& value )
	// --------------------------------------------------------------------------
	{
		model.value = value;
	}

public:
	class state
	{
	public:
		virtual state* trigger_e( CAsync_transition_15FSM& ) { return 0; }
		virtual state* event_e( CAsync_transition_15FSM& ) { return 0; }
		virtual state* unconditional( CAsync_transition_15FSM& ) { return 0; }
		virtual state* unconditional_async( CAsync_transition_15FSM& ) { return 0; }
		virtual state* initial( CAsync_transition_15FSM& ) { return 0; }

		template<class T> void enter( data_model&, ... ) {}
		template<class T> void exit( data_model&, ... ) {}
		virtual void exit( data_model&, const std::type_info& ) {}
		virtual ~state( void ) {}
	};

	state *cur_state;
	typedef state* ( state::*event )( CAsync_transition_15FSM& );

private: 
	class CPAsyncEventTransitionData
	{
	public:
		// --------------------------------------------------------------------------
		CPAsyncEventTransitionData( CAsync_transition_15FSM& fsm, const event trigger ): mFSM( fsm ), mTrigger( trigger ) { /* none */ }
		// --------------------------------------------------------------------------

		// --------------------------------------------------------------------------
		inline void dispatch( void ) { mFSM.dispatch( mTrigger ); }
		// --------------------------------------------------------------------------

	private:
		CAsync_transition_15FSM& mFSM;
		event mTrigger;
	};

	DECLARE_EVENT( CPAsyncEvent, CPAsyncEventTransitionData, IPAsyncEventConsumer );

	class CPAsyncTriggerInvoker : public IPAsyncEventConsumer
	{
	public:
		// --------------------------------------------------------------------------
		CPAsyncTriggerInvoker( CAsync_transition_15FSM& fsm ) : mFsm( fsm ) { /* none */ }
		// --------------------------------------------------------------------------

		// --------------------------------------------------------------------------
		inline state* makeAsyncCall( const event trigger ) {
		// --------------------------------------------------------------------------
			CPAsyncEvent* e = CPAsyncEvent::createEvent( CPAsyncEventTransitionData(mFsm, trigger) );
			e->setConsumer( this );
			e->send();
			return 0;
		}

		// --------------------------------------------------------------------------
		inline state* makeAsyncUCall( const event trigger ) {
		// --------------------------------------------------------------------------
			CPAsyncEvent* e = CPAsyncEvent::createEvent( CPAsyncEventTransitionData(mFsm, trigger) );
			e->setConsumer( this );
			e->send();
			return (state*)0xFFFFFF;
		}

	protected:
		// --------------------------------------------------------------------------
		inline virtual void processEvent( const CPAsyncEvent& event ) { event.getData().dispatch(); }
		// --------------------------------------------------------------------------

	private:
		CAsync_transition_15FSM& mFsm;
	} asyncTriggerInvoker;

public:
	template<class C> class state_actions
	{
	protected:
		void inline enter( data_model& ) { /* default enter action */ }
		void inline exit( data_model& ) { /* default exit action */ }
	};

	template<class C, class P> class composite: public P, public state_actions<C>
	{
		virtual state* initial( CAsync_transition_15FSM& ) { return 0; }
	public:
		// LCA calculation
		template<class T> void enter( data_model&, composite* ) {}
		template<class T> void enter( data_model &m, ... ) { P::template enter<T>( m, (T*)0 ); state_actions<C>::enter( m ); }
		template<class T> void exit( data_model&, composite* ) {}
		template<class T> void exit( data_model &m, ... ) { state_actions<C>::exit( m ); P::template exit<T>( m, (T*)0 ); }
		virtual void exit( data_model &m, const std::type_info &sti ) { if ( typeid(C) == sti ) return; state_actions<C>::exit( m ); P::exit( m, sti ); }
	};

	class no_state { /* dup */ };
	enum transition_type { external, internal };

	template<int index, event E, class S, class D0 = no_state> class transition_actions
	{
	protected:
		void enter(data_model&) { /* default enter action */ }
		bool condition(data_model&) { return true; } /* default condition action */
	};

	// external transition
	template<int index, event E, class S, class D = no_state, transition_type T = external> class transition: public transition_actions<index, E, S, D>
	{
	public:
		state* operator()( S *s, D &d, CAsync_transition_15FSM &sc )
		{
			if( !transition_actions<index, E, S, D>::condition(sc.model) ) return 0;
			s->exit( sc.model, typeid(S) );
			s->template exit<D>( sc.model );
			transition_actions<index, E, S, D>::enter( sc.model );
			d.template enter<S>( sc.model );
			return &d;
		}
	};

	// internal transition
	template<int index, event E, class S, class D> class transition<index, E, S, D, internal>: public transition_actions<index, E, S, D>
	{
	public:
		state* operator()( S *s, D &d, CAsync_transition_15FSM &sc )
		{
			if ( !transition_actions<index, E, S, D>::condition(sc.model) ) return 0;
			s->exit( sc.model, typeid(S) );
			s->template exit<D>( sc.model, (D*)0 );
			transition_actions<index, E, S, D>::enter( sc.model );
			d.template enter<S>( sc.model, (S*)0 );
			return &d;
		}
	};

	// transition with no target
	template<int index, event E, class S> class transition<index, E, S, no_state>: public transition_actions<index, E, S, no_state>
	{
	public:
		S* operator()( S *s, CAsync_transition_15FSM &sc )
		{
			if ( !transition_actions<index, E, S, no_state>::condition(sc.model) ) return 0;
			transition_actions<index, E, S, no_state>::enter( sc.model );
			return s;
		}
	};

private:
	// --------------------------------------------------------------------------
	bool dispatch_event( event e )
	// --------------------------------------------------------------------------
	{
		state *next_state;
		if ( (next_state = (cur_state->*e)(*this)) ) cur_state = next_state;
		return next_state;
	}
	// --------------------------------------------------------------------------
	bool dispatch_uasync( event e )
	// --------------------------------------------------------------------------
	{
		return (cur_state->*e)(*this) == (state*)0xFFFFFF;
	}

public:
	// --------------------------------------------------------------------------
	void dispatch( event e )
	// --------------------------------------------------------------------------
	{
		bool cont = dispatch_event( e );
		while ( cont ) {
			if ( (cont = dispatch_event(&state::initial)) );
			else if ( dispatch_uasync(&state::unconditional_async) );
			else if ( cont = dispatch_event(&state::unconditional) );
			else break;
		}
	}

	// --------------------------------------------------------------------------
	CAsync_transition_15FSM( IAsync_transition_15ActionHandler* pActionHandler ) : cur_state( &m_scxml ), asyncTriggerInvoker( *this )
	// --------------------------------------------------------------------------
	{
		model.actionHandler = pActionHandler;
	}

	// --------------------------------------------------------------------------
	void init( void )
	// --------------------------------------------------------------------------
	{
		dispatch( &state::initial );
	}

	class scxml : public composite<scxml, state>
	{
		// --------------------------------------------------------------------------
		state* initial( CAsync_transition_15FSM&sc ) {
		// --------------------------------------------------------------------------
			return transition<0, &state::initial, scxml, state_init, internal>()( this, sc.m_state_init, sc ); }
		} m_scxml;

	class state_A: public composite<state_A, scxml>
	{
		// --------------------------------------------------------------------------
		inline state* trigger_e( CAsync_transition_15FSM &sc ) {
		// --------------------------------------------------------------------------
			int& value = sc.model.value;
			if ( value == 8 ) return transition<1, &state::trigger_e, state_A, state_B>()( this, sc.m_state_B, sc );
			else return 0;
		}
		// --------------------------------------------------------------------------
		inline state* event_e( CAsync_transition_15FSM &sc ) { return sc.asyncTriggerInvoker.makeAsyncCall( &state::trigger_e ); }
		// --------------------------------------------------------------------------

		// --------------------------------------------------------------------------
		inline virtual void processEvent( const CPAsyncEvent& event ) { event.getData().dispatch(); }
		// --------------------------------------------------------------------------

		virtual state* unconditional( CAsync_transition_15FSM &sc ) { return 0; }
	} m_state_A;

	class state_init: public composite<state_init, scxml>
	{
		// --------------------------------------------------------------------------
		inline virtual state* unconditional( CAsync_transition_15FSM &sc ) {
		// --------------------------------------------------------------------------
			if ( true ) return transition<2, &state::unconditional, state_init, state_A>()( this, sc.m_state_A, sc );
			else return 0;
		}
	} m_state_init;

	class state_B: public composite<state_B, scxml>
	{
	} m_state_B;

};

class IAsync_transition_15ActionHandler
{
public:
	/* state handlers */
	virtual void stateAEnter( CAsync_transition_15FSM::data_model &m ) = 0;
	virtual void stateAExit( CAsync_transition_15FSM::data_model &m ) = 0;
	virtual void stateInitEnter( CAsync_transition_15FSM::data_model &m ) = 0;
	virtual void stateInitExit( CAsync_transition_15FSM::data_model &m ) = 0;
	virtual void stateBEnter( CAsync_transition_15FSM::data_model &m ) = 0;
	virtual void stateBExit( CAsync_transition_15FSM::data_model &m ) = 0;

	/* transition handlers */

protected:
	virtual ~IAsync_transition_15ActionHandler( void ) { /* nop */ }
};

class CAsync_transition_15ActionHandlerDefault: public IAsync_transition_15ActionHandler
{
public:
	/* state handlers */
	virtual void stateAEnter( CAsync_transition_15FSM::data_model &m ) { /* default */ }
	virtual void stateAExit( CAsync_transition_15FSM::data_model &m ) { /* default */ }
	virtual void stateInitEnter( CAsync_transition_15FSM::data_model &m ) { /* default */ }
	virtual void stateInitExit( CAsync_transition_15FSM::data_model &m ) { /* default */ }
	virtual void stateBEnter( CAsync_transition_15FSM::data_model &m ) { /* default */ }
	virtual void stateBExit( CAsync_transition_15FSM::data_model &m ) { /* default */ }

	/* transition handlers */

protected:
	virtual ~CAsync_transition_15ActionHandlerDefault( void ) { /* nop */ }
};


// --------------------------------------------------------------------------
template<> inline void CAsync_transition_15FSM::state_actions<CAsync_transition_15FSM::state_A>::enter( CAsync_transition_15FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateAEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_15FSM::state_actions<CAsync_transition_15FSM::state_A>::exit( CAsync_transition_15FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateAExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_15FSM::state_actions<CAsync_transition_15FSM::state_init>::enter( CAsync_transition_15FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateInitEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_15FSM::state_actions<CAsync_transition_15FSM::state_init>::exit( CAsync_transition_15FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateInitExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_15FSM::state_actions<CAsync_transition_15FSM::state_B>::enter( CAsync_transition_15FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateBEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_15FSM::state_actions<CAsync_transition_15FSM::state_B>::exit( CAsync_transition_15FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateBExit( m );
}

/* ************************************************************************ */
#endif // AUTOFSM_ASYNC_TRANSITION_15_HPP