#ifndef AUTOFSM_THREE_DR_EV_HPP
#define AUTOFSM_THREE_DR_EV_HPP

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

class IThree_dr_evActionHandler;
class CThree_dr_evFSM
{
public:
	struct data_model {
		friend class CThree_dr_evFSM;
		data_model( void ) { /* nop */ }
	private:
		IThree_dr_evActionHandler* actionHandler;
	} model;

public:
	class state
	{
	public:
		virtual state* event_ev( CThree_dr_evFSM& ) { return 0; }
		virtual state* unconditional( CThree_dr_evFSM& ) { return 0; }
		virtual state* initial( CThree_dr_evFSM& ) { return 0; }

		template<class T> void enter( data_model&, ... ) {}
		template<class T> void exit( data_model&, ... ) {}
		virtual void exit( data_model&, const std::type_info& ) {}
		virtual ~state( void ) {}
	};

	state *cur_state;
	typedef state* ( state::*event )( CThree_dr_evFSM& );

	template<class C> class state_actions
	{
	protected:
		void inline enter( data_model& ) { /* default enter action */ }
		void inline exit( data_model& ) { /* default exit action */ }
	};

	template<class C, class P> class composite: public P, public state_actions<C>
	{
		virtual state* initial( CThree_dr_evFSM& ) { return 0; }
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
		state* operator()( S *s, D &d, CThree_dr_evFSM &sc )
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
		state* operator()( S *s, D &d, CThree_dr_evFSM &sc )
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
		S* operator()( S *s, CThree_dr_evFSM &sc )
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
	CThree_dr_evFSM( IThree_dr_evActionHandler* pActionHandler ) : cur_state( &m_scxml )
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
		state* initial( CThree_dr_evFSM&sc ) { return transition<0, &state::initial, scxml, state_init, internal>()( this, sc.m_state_init, sc ); }
	} m_scxml;

	class state_init: public composite<state_init, scxml>
	{
		virtual state* unconditional( CThree_dr_evFSM &sc ) {
			if ( true ) return transition<1, &state::unconditional, state_init, state_node>()( this, sc.m_state_node, sc );
			else return 0;
		}
	} m_state_init;

	class state_node: public composite<state_node, scxml>
	{
		state* event_ev( CThree_dr_evFSM &sc ) {
			if ( true ) return transition<2, &state::event_ev, state_node, state_cnode>()( this, sc.m_state_cnode, sc );
			else return 0;
		}
		virtual state* unconditional( CThree_dr_evFSM &sc ) { return 0; }
	} m_state_node;

	class state_cnode: public composite<state_cnode, scxml>
	{
	} m_state_cnode;

};

class IThree_dr_evActionHandler
{
public:
	/* state handlers */
	virtual void stateInitEnter( CThree_dr_evFSM::data_model &m ) = 0;
	virtual void stateInitExit( CThree_dr_evFSM::data_model &m ) = 0;
	virtual void stateNodeEnter( CThree_dr_evFSM::data_model &m ) = 0;
	virtual void stateNodeExit( CThree_dr_evFSM::data_model &m ) = 0;
	virtual void stateCnodeEnter( CThree_dr_evFSM::data_model &m ) = 0;
	virtual void stateCnodeExit( CThree_dr_evFSM::data_model &m ) = 0;

	/* transition handlers */

protected:
	virtual ~IThree_dr_evActionHandler( void ) { /* nop */ }
};

class CThree_dr_evActionHandlerDefault: public IThree_dr_evActionHandler
{
public:
	/* state handlers */
	virtual void stateInitEnter( CThree_dr_evFSM::data_model &m ) { /* default */ }
	virtual void stateInitExit( CThree_dr_evFSM::data_model &m ) { /* default */ }
	virtual void stateNodeEnter( CThree_dr_evFSM::data_model &m ) { /* default */ }
	virtual void stateNodeExit( CThree_dr_evFSM::data_model &m ) { /* default */ }
	virtual void stateCnodeEnter( CThree_dr_evFSM::data_model &m ) { /* default */ }
	virtual void stateCnodeExit( CThree_dr_evFSM::data_model &m ) { /* default */ }

	/* transition handlers */

protected:
	virtual ~CThree_dr_evActionHandlerDefault( void ) { /* nop */ }
};


// --------------------------------------------------------------------------
template<> inline void CThree_dr_evFSM::state_actions<CThree_dr_evFSM::state_init>::enter( CThree_dr_evFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateInitEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CThree_dr_evFSM::state_actions<CThree_dr_evFSM::state_init>::exit( CThree_dr_evFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateInitExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CThree_dr_evFSM::state_actions<CThree_dr_evFSM::state_node>::enter( CThree_dr_evFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateNodeEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CThree_dr_evFSM::state_actions<CThree_dr_evFSM::state_node>::exit( CThree_dr_evFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateNodeExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CThree_dr_evFSM::state_actions<CThree_dr_evFSM::state_cnode>::enter( CThree_dr_evFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateCnodeEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CThree_dr_evFSM::state_actions<CThree_dr_evFSM::state_cnode>::exit( CThree_dr_evFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateCnodeExit( m );
}

/* ************************************************************************ */
#endif // AUTOFSM_THREE_DR_EV_HPP
