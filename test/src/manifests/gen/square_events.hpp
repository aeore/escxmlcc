#ifndef AUTOFSM_SQUARE_EVENTS_HPP
#define AUTOFSM_SQUARE_EVENTS_HPP

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

// --------------------------------------------------------------------------
// User space
// --------------------------------------------------------------------------

/* no user data */

// --------------------------------------------------------------------------
// Code definition
// --------------------------------------------------------------------------

class ISquare_eventsActionHandler;
class CSquare_eventsFSM
{
public:
	struct data_model {
		friend class CSquare_eventsFSM;
		data_model( void ) { /* nop */ }
	private:
		ISquare_eventsActionHandler* actionHandler;
	} model;

public:
	class state
	{
	public:
		virtual state* event_ab( CSquare_eventsFSM& ) { return 0; }
		virtual state* event_bc( CSquare_eventsFSM& ) { return 0; }
		virtual state* event_bd( CSquare_eventsFSM& ) { return 0; }
		virtual state* event_ca( CSquare_eventsFSM& ) { return 0; }
		virtual state* event_cd( CSquare_eventsFSM& ) { return 0; }
		virtual state* event_da( CSquare_eventsFSM& ) { return 0; }
		virtual state* unconditional( CSquare_eventsFSM& ) { return 0; }
		virtual state* initial( CSquare_eventsFSM& ) { return 0; }

		template<class T> void enter( data_model&, ... ) {}
		template<class T> void exit( data_model&, ... ) {}
		virtual void exit( data_model&, const std::type_info& ) {}
		virtual ~state( void ) {}
	};

	state *cur_state;
	typedef state* ( state::*event )( CSquare_eventsFSM& );

	template<class C> class state_actions
	{
	protected:
		void inline enter( data_model& ) { /* default enter action */ }
		void inline exit( data_model& ) { /* default exit action */ }
	};

	template<class C, class P> class composite: public P, public state_actions<C>
	{
		virtual state* initial( CSquare_eventsFSM& ) { return 0; }
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
		state* operator()( S *s, D &d, CSquare_eventsFSM &sc )
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
		state* operator()( S *s, D &d, CSquare_eventsFSM &sc )
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
		S* operator()( S *s, CSquare_eventsFSM &sc )
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

public:
	// --------------------------------------------------------------------------
	void dispatch( event e )
	// --------------------------------------------------------------------------
	{
		bool cont = dispatch_event( e );
		while ( cont ) {
			if ( (cont = dispatch_event(&state::initial)) );
			else if ( (cont = dispatch_event(&state::unconditional)) );
			else break;
		}
	}

	// --------------------------------------------------------------------------
	CSquare_eventsFSM( ISquare_eventsActionHandler* pActionHandler ) : cur_state( &m_scxml )
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
		state* initial( CSquare_eventsFSM&sc ) { return transition<0, &state::initial, scxml, state_init, internal>()( this, sc.m_state_init, sc ); }
	} m_scxml;

	class state_init: public composite<state_init, scxml>
	{
		virtual state* unconditional( CSquare_eventsFSM &sc ) {
			if ( true ) return transition<1, &state::unconditional, state_init, state_A>()( this, sc.m_state_A, sc );
			else return 0;
		}
	} m_state_init;

	class state_A: public composite<state_A, scxml>
	{
		state* event_ab( CSquare_eventsFSM &sc ) {
			if ( true ) return transition<2, &state::event_ab, state_A, state_B>()( this, sc.m_state_B, sc );
			else return 0;
		}
		virtual state* unconditional( CSquare_eventsFSM &sc ) { return 0; }
	} m_state_A;

	class state_B: public composite<state_B, scxml>
	{
		state* event_bc( CSquare_eventsFSM &sc ) {
			if ( true ) return transition<3, &state::event_bc, state_B, state_C>()( this, sc.m_state_C, sc );
			else return 0;
		}
		state* event_bd( CSquare_eventsFSM &sc ) {
			if ( true ) return transition<4, &state::event_bd, state_B, state_D>()( this, sc.m_state_D, sc );
			else return 0;
		}
		virtual state* unconditional( CSquare_eventsFSM &sc ) { return 0; }
	} m_state_B;

	class state_D: public composite<state_D, scxml>
	{
		state* event_da( CSquare_eventsFSM &sc ) {
			if ( true ) return transition<5, &state::event_da, state_D, state_A>()( this, sc.m_state_A, sc );
			else return 0;
		}
		virtual state* unconditional( CSquare_eventsFSM &sc ) { return 0; }
	} m_state_D;

	class state_C: public composite<state_C, scxml>
	{
		state* event_ca( CSquare_eventsFSM &sc ) {
			if ( true ) return transition<7, &state::event_ca, state_C, state_A>()( this, sc.m_state_A, sc );
			else return 0;
		}
		state* event_cd( CSquare_eventsFSM &sc ) {
			if ( true ) return transition<6, &state::event_cd, state_C, state_D>()( this, sc.m_state_D, sc );
			else return 0;
		}
		virtual state* unconditional( CSquare_eventsFSM &sc ) { return 0; }
	} m_state_C;

};

class ISquare_eventsActionHandler
{
public:
	/* state handlers */
	virtual void stateInitEnter( CSquare_eventsFSM::data_model &m ) = 0;
	virtual void stateInitExit( CSquare_eventsFSM::data_model &m ) = 0;
	virtual void stateAEnter( CSquare_eventsFSM::data_model &m ) = 0;
	virtual void stateAExit( CSquare_eventsFSM::data_model &m ) = 0;
	virtual void stateBEnter( CSquare_eventsFSM::data_model &m ) = 0;
	virtual void stateBExit( CSquare_eventsFSM::data_model &m ) = 0;
	virtual void stateDEnter( CSquare_eventsFSM::data_model &m ) = 0;
	virtual void stateDExit( CSquare_eventsFSM::data_model &m ) = 0;
	virtual void stateCEnter( CSquare_eventsFSM::data_model &m ) = 0;
	virtual void stateCExit( CSquare_eventsFSM::data_model &m ) = 0;

	/* transition handlers */

protected:
	virtual ~ISquare_eventsActionHandler( void ) { /* nop */ }
};

class CSquare_eventsActionHandlerDefault: public ISquare_eventsActionHandler
{
public:
	/* state handlers */
	virtual void stateInitEnter( CSquare_eventsFSM::data_model &m ) { /* default */ }
	virtual void stateInitExit( CSquare_eventsFSM::data_model &m ) { /* default */ }
	virtual void stateAEnter( CSquare_eventsFSM::data_model &m ) { /* default */ }
	virtual void stateAExit( CSquare_eventsFSM::data_model &m ) { /* default */ }
	virtual void stateBEnter( CSquare_eventsFSM::data_model &m ) { /* default */ }
	virtual void stateBExit( CSquare_eventsFSM::data_model &m ) { /* default */ }
	virtual void stateDEnter( CSquare_eventsFSM::data_model &m ) { /* default */ }
	virtual void stateDExit( CSquare_eventsFSM::data_model &m ) { /* default */ }
	virtual void stateCEnter( CSquare_eventsFSM::data_model &m ) { /* default */ }
	virtual void stateCExit( CSquare_eventsFSM::data_model &m ) { /* default */ }

	/* transition handlers */

protected:
	virtual ~CSquare_eventsActionHandlerDefault( void ) { /* nop */ }
};


// --------------------------------------------------------------------------
template<> inline void CSquare_eventsFSM::state_actions<CSquare_eventsFSM::state_init>::enter( CSquare_eventsFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateInitEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CSquare_eventsFSM::state_actions<CSquare_eventsFSM::state_init>::exit( CSquare_eventsFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateInitExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CSquare_eventsFSM::state_actions<CSquare_eventsFSM::state_A>::enter( CSquare_eventsFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateAEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CSquare_eventsFSM::state_actions<CSquare_eventsFSM::state_A>::exit( CSquare_eventsFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateAExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CSquare_eventsFSM::state_actions<CSquare_eventsFSM::state_B>::enter( CSquare_eventsFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateBEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CSquare_eventsFSM::state_actions<CSquare_eventsFSM::state_B>::exit( CSquare_eventsFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateBExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CSquare_eventsFSM::state_actions<CSquare_eventsFSM::state_D>::enter( CSquare_eventsFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateDEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CSquare_eventsFSM::state_actions<CSquare_eventsFSM::state_D>::exit( CSquare_eventsFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateDExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CSquare_eventsFSM::state_actions<CSquare_eventsFSM::state_C>::enter( CSquare_eventsFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateCEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CSquare_eventsFSM::state_actions<CSquare_eventsFSM::state_C>::exit( CSquare_eventsFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateCExit( m );
}

/* ************************************************************************ */
#endif // AUTOFSM_SQUARE_EVENTS_HPP