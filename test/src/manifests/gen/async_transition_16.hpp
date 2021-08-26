#ifndef AUTOFSM_ASYNC_TRANSITION_16_HPP
#define AUTOFSM_ASYNC_TRANSITION_16_HPP

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

class IAsync_transition_16ActionHandler;
class CAsync_transition_16FSM
{
public:
	struct data_model {
		friend class CAsync_transition_16FSM;
		data_model( void ): value() { /* nop */ }
		int value;
	private:
		IAsync_transition_16ActionHandler* actionHandler;
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
		virtual state* trigger_( CAsync_transition_16FSM& ) { return 0; }
		virtual state* event_( CAsync_transition_16FSM& ) { return 0; }
		virtual state* trigger_e( CAsync_transition_16FSM& ) { return 0; }
		virtual state* event_e( CAsync_transition_16FSM& ) { return 0; }
		virtual state* trigger_w( CAsync_transition_16FSM& ) { return 0; }
		virtual state* event_w( CAsync_transition_16FSM& ) { return 0; }
		virtual state* trigger_x( CAsync_transition_16FSM& ) { return 0; }
		virtual state* event_x( CAsync_transition_16FSM& ) { return 0; }
		virtual state* trigger_z( CAsync_transition_16FSM& ) { return 0; }
		virtual state* event_z( CAsync_transition_16FSM& ) { return 0; }
		virtual state* trigger_t( CAsync_transition_16FSM& ) { return 0; }
		virtual state* unconditional( CAsync_transition_16FSM& ) { return 0; }
		virtual state* unconditional_async( CAsync_transition_16FSM& ) { return 0; }
		virtual state* initial( CAsync_transition_16FSM& ) { return 0; }

		template<class T> void enter( data_model&, ... ) {}
		template<class T> void exit( data_model&, ... ) {}
		virtual void exit( data_model&, const std::type_info& ) {}
		virtual ~state( void ) {}
	};

	state *cur_state;
	typedef state* ( state::*event )( CAsync_transition_16FSM& );

private: 
	class CPAsyncEventTransitionData
	{
	public:
		// --------------------------------------------------------------------------
		CPAsyncEventTransitionData( CAsync_transition_16FSM& fsm, const event trigger ): mFSM( fsm ), mTrigger( trigger ) { /* none */ }
		// --------------------------------------------------------------------------

		// --------------------------------------------------------------------------
		inline void dispatch( void ) { mFSM.dispatch( mTrigger ); }
		// --------------------------------------------------------------------------

	private:
		CAsync_transition_16FSM& mFSM;
		event mTrigger;
	};

	DECLARE_EVENT( CPAsyncEvent, CPAsyncEventTransitionData, IPAsyncEventConsumer );

	class CPAsyncTriggerInvoker : public IPAsyncEventConsumer
	{
	public:
		// --------------------------------------------------------------------------
		CPAsyncTriggerInvoker( CAsync_transition_16FSM& fsm ) : mFsm( fsm ) { /* none */ }
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
		CAsync_transition_16FSM& mFsm;
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
		virtual state* initial( CAsync_transition_16FSM& ) { return 0; }
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
		state* operator()( S *s, D &d, CAsync_transition_16FSM &sc )
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
		state* operator()( S *s, D &d, CAsync_transition_16FSM &sc )
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
		S* operator()( S *s, CAsync_transition_16FSM &sc )
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
	CAsync_transition_16FSM( IAsync_transition_16ActionHandler* pActionHandler ) : cur_state( &m_scxml ), asyncTriggerInvoker( *this )
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
		state* initial( CAsync_transition_16FSM&sc ) {
		// --------------------------------------------------------------------------
			return transition<0, &state::initial, scxml, state_init, internal>()( this, sc.m_state_init, sc ); }
		} m_scxml;

	class state_A: public composite<state_A, scxml>
	{
		// --------------------------------------------------------------------------
		inline state* trigger_e( CAsync_transition_16FSM &sc ) {
		// --------------------------------------------------------------------------
			int& value = sc.model.value;
			if ( value > 7 ) return transition<1, &state::trigger_e, state_A, state_B>()( this, sc.m_state_B, sc );
			else return 0;
		}
		// --------------------------------------------------------------------------
		inline state* event_e( CAsync_transition_16FSM &sc ) { return sc.asyncTriggerInvoker.makeAsyncCall( &state::trigger_e ); }
		// --------------------------------------------------------------------------

		// --------------------------------------------------------------------------
		inline virtual void processEvent( const CPAsyncEvent& event ) { event.getData().dispatch(); }
		// --------------------------------------------------------------------------

		virtual state* unconditional( CAsync_transition_16FSM &sc ) { return 0; }
	} m_state_A;

	class state_init: public composite<state_init, scxml>
	{
		// --------------------------------------------------------------------------
		inline virtual state* unconditional( CAsync_transition_16FSM &sc ) {
		// --------------------------------------------------------------------------
			if ( true ) return transition<2, &state::unconditional, state_init, state_A>()( this, sc.m_state_A, sc );
			else return 0;
		}
	} m_state_init;

	class state_B: public composite<state_B, scxml>
	{
		// --------------------------------------------------------------------------
		inline virtual state* unconditional( CAsync_transition_16FSM &sc ) {
		// --------------------------------------------------------------------------
			if ( true ) return transition<3, &state::unconditional, state_B, state_Cluster>()( this, sc.m_state_Cluster, sc );
			else return 0;
		}
	} m_state_B;

	class state_Cluster: public composite<state_Cluster, scxml>
	{
		state* initial( CAsync_transition_16FSM &sc ) { return transition<0, &state::initial, state_Cluster, state_C, internal>()( this, sc.m_state_C, sc ); }
	} m_state_Cluster;

	class state_Cluster_2: public composite<state_Cluster_2, state_Cluster>
	{
		state* initial( CAsync_transition_16FSM &sc ) { return transition<0, &state::initial, state_Cluster_2, state_E, internal>()( this, sc.m_state_E, sc ); }
		// --------------------------------------------------------------------------
		inline state* trigger_z( CAsync_transition_16FSM &sc ) {
		// --------------------------------------------------------------------------
			if ( true ) return transition<4, &state::trigger_z, state_Cluster_2, state_G>()( this, sc.m_state_G, sc );
			else return 0;
		}
		// --------------------------------------------------------------------------
		inline state* event_z( CAsync_transition_16FSM &sc ) { return sc.asyncTriggerInvoker.makeAsyncCall( &state::trigger_z ); }
		// --------------------------------------------------------------------------

		// --------------------------------------------------------------------------
		inline virtual void processEvent( const CPAsyncEvent& event ) { event.getData().dispatch(); }
		// --------------------------------------------------------------------------

		virtual state* unconditional( CAsync_transition_16FSM &sc ) { return 0; }
	} m_state_Cluster_2;

	class state_E: public composite<state_E, state_Cluster_2>
	{
		// --------------------------------------------------------------------------
		inline virtual state* unconditional( CAsync_transition_16FSM &sc ) {
		// --------------------------------------------------------------------------
			if ( true ) return transition<5, &state::unconditional, state_E, state_F>()( this, sc.m_state_F, sc );
			else return 0;
		}
	} m_state_E;

	class state_F: public composite<state_F, state_Cluster_2>
	{
		// --------------------------------------------------------------------------
		inline state* trigger_x( CAsync_transition_16FSM &sc ) {
		// --------------------------------------------------------------------------
			if ( true ) return transition<6, &state::trigger_x, state_F, state_H>()( this, sc.m_state_H, sc );
			else return 0;
		}
		// --------------------------------------------------------------------------
		inline state* event_x( CAsync_transition_16FSM &sc ) { return sc.asyncTriggerInvoker.makeAsyncCall( &state::trigger_x ); }
		// --------------------------------------------------------------------------

		// --------------------------------------------------------------------------
		inline virtual void processEvent( const CPAsyncEvent& event ) { event.getData().dispatch(); }
		// --------------------------------------------------------------------------

		virtual state* unconditional( CAsync_transition_16FSM &sc ) { return 0; }
	} m_state_F;

	class state_C: public composite<state_C, state_Cluster>
	{
		// --------------------------------------------------------------------------
		inline state* trigger_t( CAsync_transition_16FSM &sc ) {
		// --------------------------------------------------------------------------
			if ( true ) return transition<7, &state::trigger_t, state_C, state_D>()( this, sc.m_state_D, sc );
			else return 0;
		}
		virtual state* unconditional( CAsync_transition_16FSM &sc ) { return 0; }
	} m_state_C;

	class state_D: public composite<state_D, state_Cluster>
	{
		// --------------------------------------------------------------------------
		inline state* trigger_w( CAsync_transition_16FSM &sc ) {
		// --------------------------------------------------------------------------
			int& value = sc.model.value;
			if ( value < 4 ) return transition<8, &state::trigger_w, state_D, state_Cluster_2>()( this, sc.m_state_Cluster_2, sc );
			if ( value > 4 ) return transition<9, &state::trigger_w, state_D, state_F>()( this, sc.m_state_F, sc );
			else return 0;
		}
		// --------------------------------------------------------------------------
		inline state* event_w( CAsync_transition_16FSM &sc ) { return sc.asyncTriggerInvoker.makeAsyncCall( &state::trigger_w ); }
		// --------------------------------------------------------------------------

		// --------------------------------------------------------------------------
		inline virtual void processEvent( const CPAsyncEvent& event ) { event.getData().dispatch(); }
		// --------------------------------------------------------------------------

		virtual state* unconditional( CAsync_transition_16FSM &sc ) { return 0; }
	} m_state_D;

	class state_H: public composite<state_H, state_Cluster>
	{
		// --------------------------------------------------------------------------
		inline virtual state* unconditional( CAsync_transition_16FSM &sc ) {
		// --------------------------------------------------------------------------
			if ( true ) return transition<10, &state::unconditional, state_H, state_C>()( this, sc.m_state_C, sc );
			else return 0;
		}
		// --------------------------------------------------------------------------
		inline virtual state* unconditional_async(CAsync_transition_16FSM &sc) {
		// --------------------------------------------------------------------------
			return sc.asyncTriggerInvoker.makeAsyncUCall(&state::unconditional);
		}

		// --------------------------------------------------------------------------
		inline virtual void processEvent( const CPAsyncEvent& event ) { event.getData().dispatch(); }
		// --------------------------------------------------------------------------

	} m_state_H;

	class state_G: public composite<state_G, scxml>
	{
	} m_state_G;

};

class IAsync_transition_16ActionHandler
{
public:
	/* state handlers */
	virtual void stateAEnter( CAsync_transition_16FSM::data_model &m ) = 0;
	virtual void stateAExit( CAsync_transition_16FSM::data_model &m ) = 0;
	virtual void stateInitEnter( CAsync_transition_16FSM::data_model &m ) = 0;
	virtual void stateInitExit( CAsync_transition_16FSM::data_model &m ) = 0;
	virtual void stateBEnter( CAsync_transition_16FSM::data_model &m ) = 0;
	virtual void stateBExit( CAsync_transition_16FSM::data_model &m ) = 0;
	virtual void stateClusterEnter( CAsync_transition_16FSM::data_model &m ) = 0;
	virtual void stateClusterExit( CAsync_transition_16FSM::data_model &m ) = 0;
	virtual void stateCluster_2Enter( CAsync_transition_16FSM::data_model &m ) = 0;
	virtual void stateCluster_2Exit( CAsync_transition_16FSM::data_model &m ) = 0;
	virtual void stateEEnter( CAsync_transition_16FSM::data_model &m ) = 0;
	virtual void stateEExit( CAsync_transition_16FSM::data_model &m ) = 0;
	virtual void stateFEnter( CAsync_transition_16FSM::data_model &m ) = 0;
	virtual void stateFExit( CAsync_transition_16FSM::data_model &m ) = 0;
	virtual void stateCEnter( CAsync_transition_16FSM::data_model &m ) = 0;
	virtual void stateCExit( CAsync_transition_16FSM::data_model &m ) = 0;
	virtual void stateDEnter( CAsync_transition_16FSM::data_model &m ) = 0;
	virtual void stateDExit( CAsync_transition_16FSM::data_model &m ) = 0;
	virtual void stateHEnter( CAsync_transition_16FSM::data_model &m ) = 0;
	virtual void stateHExit( CAsync_transition_16FSM::data_model &m ) = 0;
	virtual void stateGEnter( CAsync_transition_16FSM::data_model &m ) = 0;
	virtual void stateGExit( CAsync_transition_16FSM::data_model &m ) = 0;

	/* transition handlers */

protected:
	virtual ~IAsync_transition_16ActionHandler( void ) { /* nop */ }
};

class CAsync_transition_16ActionHandlerDefault: public IAsync_transition_16ActionHandler
{
public:
	/* state handlers */
	virtual void stateAEnter( CAsync_transition_16FSM::data_model &m ) { /* default */ }
	virtual void stateAExit( CAsync_transition_16FSM::data_model &m ) { /* default */ }
	virtual void stateInitEnter( CAsync_transition_16FSM::data_model &m ) { /* default */ }
	virtual void stateInitExit( CAsync_transition_16FSM::data_model &m ) { /* default */ }
	virtual void stateBEnter( CAsync_transition_16FSM::data_model &m ) { /* default */ }
	virtual void stateBExit( CAsync_transition_16FSM::data_model &m ) { /* default */ }
	virtual void stateClusterEnter( CAsync_transition_16FSM::data_model &m ) { /* default */ }
	virtual void stateClusterExit( CAsync_transition_16FSM::data_model &m ) { /* default */ }
	virtual void stateCluster_2Enter( CAsync_transition_16FSM::data_model &m ) { /* default */ }
	virtual void stateCluster_2Exit( CAsync_transition_16FSM::data_model &m ) { /* default */ }
	virtual void stateEEnter( CAsync_transition_16FSM::data_model &m ) { /* default */ }
	virtual void stateEExit( CAsync_transition_16FSM::data_model &m ) { /* default */ }
	virtual void stateFEnter( CAsync_transition_16FSM::data_model &m ) { /* default */ }
	virtual void stateFExit( CAsync_transition_16FSM::data_model &m ) { /* default */ }
	virtual void stateCEnter( CAsync_transition_16FSM::data_model &m ) { /* default */ }
	virtual void stateCExit( CAsync_transition_16FSM::data_model &m ) { /* default */ }
	virtual void stateDEnter( CAsync_transition_16FSM::data_model &m ) { /* default */ }
	virtual void stateDExit( CAsync_transition_16FSM::data_model &m ) { /* default */ }
	virtual void stateHEnter( CAsync_transition_16FSM::data_model &m ) { /* default */ }
	virtual void stateHExit( CAsync_transition_16FSM::data_model &m ) { /* default */ }
	virtual void stateGEnter( CAsync_transition_16FSM::data_model &m ) { /* default */ }
	virtual void stateGExit( CAsync_transition_16FSM::data_model &m ) { /* default */ }

	/* transition handlers */

protected:
	virtual ~CAsync_transition_16ActionHandlerDefault( void ) { /* nop */ }
};


// --------------------------------------------------------------------------
template<> inline void CAsync_transition_16FSM::state_actions<CAsync_transition_16FSM::state_A>::enter( CAsync_transition_16FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateAEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_16FSM::state_actions<CAsync_transition_16FSM::state_A>::exit( CAsync_transition_16FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateAExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_16FSM::state_actions<CAsync_transition_16FSM::state_init>::enter( CAsync_transition_16FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateInitEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_16FSM::state_actions<CAsync_transition_16FSM::state_init>::exit( CAsync_transition_16FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateInitExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_16FSM::state_actions<CAsync_transition_16FSM::state_B>::enter( CAsync_transition_16FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateBEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_16FSM::state_actions<CAsync_transition_16FSM::state_B>::exit( CAsync_transition_16FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateBExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_16FSM::state_actions<CAsync_transition_16FSM::state_Cluster>::enter( CAsync_transition_16FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateClusterEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_16FSM::state_actions<CAsync_transition_16FSM::state_Cluster>::exit( CAsync_transition_16FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateClusterExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_16FSM::state_actions<CAsync_transition_16FSM::state_Cluster_2>::enter( CAsync_transition_16FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateCluster_2Enter( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_16FSM::state_actions<CAsync_transition_16FSM::state_Cluster_2>::exit( CAsync_transition_16FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateCluster_2Exit( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_16FSM::state_actions<CAsync_transition_16FSM::state_E>::enter( CAsync_transition_16FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateEEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_16FSM::state_actions<CAsync_transition_16FSM::state_E>::exit( CAsync_transition_16FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateEExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_16FSM::state_actions<CAsync_transition_16FSM::state_F>::enter( CAsync_transition_16FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateFEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_16FSM::state_actions<CAsync_transition_16FSM::state_F>::exit( CAsync_transition_16FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateFExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_16FSM::state_actions<CAsync_transition_16FSM::state_C>::enter( CAsync_transition_16FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateCEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_16FSM::state_actions<CAsync_transition_16FSM::state_C>::exit( CAsync_transition_16FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateCExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_16FSM::state_actions<CAsync_transition_16FSM::state_D>::enter( CAsync_transition_16FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateDEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_16FSM::state_actions<CAsync_transition_16FSM::state_D>::exit( CAsync_transition_16FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateDExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_16FSM::state_actions<CAsync_transition_16FSM::state_H>::enter( CAsync_transition_16FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateHEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_16FSM::state_actions<CAsync_transition_16FSM::state_H>::exit( CAsync_transition_16FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateHExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_16FSM::state_actions<CAsync_transition_16FSM::state_G>::enter( CAsync_transition_16FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateGEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CAsync_transition_16FSM::state_actions<CAsync_transition_16FSM::state_G>::exit( CAsync_transition_16FSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateGExit( m );
}

/* ************************************************************************ */
#endif // AUTOFSM_ASYNC_TRANSITION_16_HPP
