#ifndef AUTOFSM_CLUSTER_REC2_MULTINODES_HPP
#define AUTOFSM_CLUSTER_REC2_MULTINODES_HPP

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

class ICluster_rec2_multinodesActionHandler;
class CCluster_rec2_multinodesFSM
{
public:
	struct data_model {
		friend class CCluster_rec2_multinodesFSM;
		data_model( void ) { /* nop */ }
	private:
		ICluster_rec2_multinodesActionHandler* actionHandler;
	} model;

public:
	class state
	{
	public:
		virtual state* event_q( CCluster_rec2_multinodesFSM& ) { return 0; }
		virtual state* event_r( CCluster_rec2_multinodesFSM& ) { return 0; }
		virtual state* event_t( CCluster_rec2_multinodesFSM& ) { return 0; }
		virtual state* unconditional( CCluster_rec2_multinodesFSM& ) { return 0; }
		virtual state* initial( CCluster_rec2_multinodesFSM& ) { return 0; }

		template<class T> void enter( data_model&, ... ) {}
		template<class T> void exit( data_model&, ... ) {}
		virtual void exit( data_model&, const std::type_info& ) {}
		virtual ~state( void ) {}
	};

	state *cur_state;
	typedef state* ( state::*event )( CCluster_rec2_multinodesFSM& );

	template<class C> class state_actions
	{
	protected:
		void inline enter( data_model& ) { /* default enter action */ }
		void inline exit( data_model& ) { /* default exit action */ }
	};

	template<class C, class P> class composite: public P, public state_actions<C>
	{
		virtual state* initial( CCluster_rec2_multinodesFSM& ) { return 0; }
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
		state* operator()( S *s, D &d, CCluster_rec2_multinodesFSM &sc )
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
		state* operator()( S *s, D &d, CCluster_rec2_multinodesFSM &sc )
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
		S* operator()( S *s, CCluster_rec2_multinodesFSM &sc )
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
	CCluster_rec2_multinodesFSM( ICluster_rec2_multinodesActionHandler* pActionHandler ) : cur_state( &m_scxml )
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
		state* initial( CCluster_rec2_multinodesFSM&sc ) { return transition<0, &state::initial, scxml, state_init, internal>()( this, sc.m_state_init, sc ); }
	} m_scxml;

	class state_init: public composite<state_init, scxml>
	{
		state* event_q( CCluster_rec2_multinodesFSM &sc ) {
			if ( true ) return transition<1, &state::event_q, state_init, state_A>()( this, sc.m_state_A, sc );
			else return 0;
		}
		virtual state* unconditional( CCluster_rec2_multinodesFSM &sc ) { return 0; }
	} m_state_init;

	class state_A: public composite<state_A, scxml>
	{
		virtual state* unconditional( CCluster_rec2_multinodesFSM &sc ) {
			if ( true ) return transition<2, &state::unconditional, state_A, state_Cluster>()( this, sc.m_state_Cluster, sc );
			else return 0;
		}
	} m_state_A;

	class state_Cluster: public composite<state_Cluster, scxml>
	{
		state* initial( CCluster_rec2_multinodesFSM &sc ) { return transition<0, &state::initial, state_Cluster, state_B, internal>()( this, sc.m_state_B, sc ); }
		state* event_t( CCluster_rec2_multinodesFSM &sc ) {
			if ( true ) return transition<3, &state::event_t, state_Cluster, state_A>()( this, sc.m_state_A, sc );
			else return 0;
		}
		virtual state* unconditional( CCluster_rec2_multinodesFSM &sc ) { return 0; }
	} m_state_Cluster;

	class state_B: public composite<state_B, state_Cluster>
	{
		virtual state* unconditional( CCluster_rec2_multinodesFSM &sc ) {
			if ( true ) return transition<4, &state::unconditional, state_B, state_Cluster_2>()( this, sc.m_state_Cluster_2, sc );
			else return 0;
		}
	} m_state_B;

	class state_Cluster_2: public composite<state_Cluster_2, state_Cluster>
	{
		state* initial( CCluster_rec2_multinodesFSM &sc ) { return transition<0, &state::initial, state_Cluster_2, state_i, internal>()( this, sc.m_state_i, sc ); }
		virtual state* unconditional( CCluster_rec2_multinodesFSM &sc ) {
			if ( true ) return transition<5, &state::unconditional, state_Cluster_2, state_D>()( this, sc.m_state_D, sc );
			else return 0;
		}
	} m_state_Cluster_2;

	class state_i: public composite<state_i, state_Cluster_2>
	{
		virtual state* unconditional( CCluster_rec2_multinodesFSM &sc ) {
			if ( true ) return transition<6, &state::unconditional, state_i, state_R>()( this, sc.m_state_R, sc );
			else return 0;
		}
	} m_state_i;

	class state_R: public composite<state_R, state_Cluster_2>
	{
	} m_state_R;

	class state_D: public composite<state_D, state_Cluster>
	{
		state* event_r( CCluster_rec2_multinodesFSM &sc ) {
			if ( true ) return transition<7, &state::event_r, state_D, state_B>()( this, sc.m_state_B, sc );
			else return 0;
		}
		virtual state* unconditional( CCluster_rec2_multinodesFSM &sc ) { return 0; }
	} m_state_D;

};

class ICluster_rec2_multinodesActionHandler
{
public:
	/* state handlers */
	virtual void stateInitEnter( CCluster_rec2_multinodesFSM::data_model &m ) = 0;
	virtual void stateInitExit( CCluster_rec2_multinodesFSM::data_model &m ) = 0;
	virtual void stateAEnter( CCluster_rec2_multinodesFSM::data_model &m ) = 0;
	virtual void stateAExit( CCluster_rec2_multinodesFSM::data_model &m ) = 0;
	virtual void stateClusterEnter( CCluster_rec2_multinodesFSM::data_model &m ) = 0;
	virtual void stateClusterExit( CCluster_rec2_multinodesFSM::data_model &m ) = 0;
	virtual void stateBEnter( CCluster_rec2_multinodesFSM::data_model &m ) = 0;
	virtual void stateBExit( CCluster_rec2_multinodesFSM::data_model &m ) = 0;
	virtual void stateCluster_2Enter( CCluster_rec2_multinodesFSM::data_model &m ) = 0;
	virtual void stateCluster_2Exit( CCluster_rec2_multinodesFSM::data_model &m ) = 0;
	virtual void stateIEnter( CCluster_rec2_multinodesFSM::data_model &m ) = 0;
	virtual void stateIExit( CCluster_rec2_multinodesFSM::data_model &m ) = 0;
	virtual void stateREnter( CCluster_rec2_multinodesFSM::data_model &m ) = 0;
	virtual void stateRExit( CCluster_rec2_multinodesFSM::data_model &m ) = 0;
	virtual void stateDEnter( CCluster_rec2_multinodesFSM::data_model &m ) = 0;
	virtual void stateDExit( CCluster_rec2_multinodesFSM::data_model &m ) = 0;

	/* transition handlers */

protected:
	virtual ~ICluster_rec2_multinodesActionHandler( void ) { /* nop */ }
};

class CCluster_rec2_multinodesActionHandlerDefault: public ICluster_rec2_multinodesActionHandler
{
public:
	/* state handlers */
	virtual void stateInitEnter( CCluster_rec2_multinodesFSM::data_model &m ) { /* default */ }
	virtual void stateInitExit( CCluster_rec2_multinodesFSM::data_model &m ) { /* default */ }
	virtual void stateAEnter( CCluster_rec2_multinodesFSM::data_model &m ) { /* default */ }
	virtual void stateAExit( CCluster_rec2_multinodesFSM::data_model &m ) { /* default */ }
	virtual void stateClusterEnter( CCluster_rec2_multinodesFSM::data_model &m ) { /* default */ }
	virtual void stateClusterExit( CCluster_rec2_multinodesFSM::data_model &m ) { /* default */ }
	virtual void stateBEnter( CCluster_rec2_multinodesFSM::data_model &m ) { /* default */ }
	virtual void stateBExit( CCluster_rec2_multinodesFSM::data_model &m ) { /* default */ }
	virtual void stateCluster_2Enter( CCluster_rec2_multinodesFSM::data_model &m ) { /* default */ }
	virtual void stateCluster_2Exit( CCluster_rec2_multinodesFSM::data_model &m ) { /* default */ }
	virtual void stateIEnter( CCluster_rec2_multinodesFSM::data_model &m ) { /* default */ }
	virtual void stateIExit( CCluster_rec2_multinodesFSM::data_model &m ) { /* default */ }
	virtual void stateREnter( CCluster_rec2_multinodesFSM::data_model &m ) { /* default */ }
	virtual void stateRExit( CCluster_rec2_multinodesFSM::data_model &m ) { /* default */ }
	virtual void stateDEnter( CCluster_rec2_multinodesFSM::data_model &m ) { /* default */ }
	virtual void stateDExit( CCluster_rec2_multinodesFSM::data_model &m ) { /* default */ }

	/* transition handlers */

protected:
	virtual ~CCluster_rec2_multinodesActionHandlerDefault( void ) { /* nop */ }
};


// --------------------------------------------------------------------------
template<> inline void CCluster_rec2_multinodesFSM::state_actions<CCluster_rec2_multinodesFSM::state_init>::enter( CCluster_rec2_multinodesFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateInitEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CCluster_rec2_multinodesFSM::state_actions<CCluster_rec2_multinodesFSM::state_init>::exit( CCluster_rec2_multinodesFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateInitExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CCluster_rec2_multinodesFSM::state_actions<CCluster_rec2_multinodesFSM::state_A>::enter( CCluster_rec2_multinodesFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateAEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CCluster_rec2_multinodesFSM::state_actions<CCluster_rec2_multinodesFSM::state_A>::exit( CCluster_rec2_multinodesFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateAExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CCluster_rec2_multinodesFSM::state_actions<CCluster_rec2_multinodesFSM::state_Cluster>::enter( CCluster_rec2_multinodesFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateClusterEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CCluster_rec2_multinodesFSM::state_actions<CCluster_rec2_multinodesFSM::state_Cluster>::exit( CCluster_rec2_multinodesFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateClusterExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CCluster_rec2_multinodesFSM::state_actions<CCluster_rec2_multinodesFSM::state_B>::enter( CCluster_rec2_multinodesFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateBEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CCluster_rec2_multinodesFSM::state_actions<CCluster_rec2_multinodesFSM::state_B>::exit( CCluster_rec2_multinodesFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateBExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CCluster_rec2_multinodesFSM::state_actions<CCluster_rec2_multinodesFSM::state_Cluster_2>::enter( CCluster_rec2_multinodesFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateCluster_2Enter( m );
}

// --------------------------------------------------------------------------
template<> inline void CCluster_rec2_multinodesFSM::state_actions<CCluster_rec2_multinodesFSM::state_Cluster_2>::exit( CCluster_rec2_multinodesFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateCluster_2Exit( m );
}

// --------------------------------------------------------------------------
template<> inline void CCluster_rec2_multinodesFSM::state_actions<CCluster_rec2_multinodesFSM::state_i>::enter( CCluster_rec2_multinodesFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateIEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CCluster_rec2_multinodesFSM::state_actions<CCluster_rec2_multinodesFSM::state_i>::exit( CCluster_rec2_multinodesFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateIExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CCluster_rec2_multinodesFSM::state_actions<CCluster_rec2_multinodesFSM::state_R>::enter( CCluster_rec2_multinodesFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateREnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CCluster_rec2_multinodesFSM::state_actions<CCluster_rec2_multinodesFSM::state_R>::exit( CCluster_rec2_multinodesFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateRExit( m );
}

// --------------------------------------------------------------------------
template<> inline void CCluster_rec2_multinodesFSM::state_actions<CCluster_rec2_multinodesFSM::state_D>::enter( CCluster_rec2_multinodesFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateDEnter( m );
}

// --------------------------------------------------------------------------
template<> inline void CCluster_rec2_multinodesFSM::state_actions<CCluster_rec2_multinodesFSM::state_D>::exit( CCluster_rec2_multinodesFSM::data_model &m )
// --------------------------------------------------------------------------
{
	m.actionHandler->stateDExit( m );
}

/* ************************************************************************ */
#endif // AUTOFSM_CLUSTER_REC2_MULTINODES_HPP
