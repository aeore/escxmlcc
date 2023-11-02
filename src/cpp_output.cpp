/*************************************************************************
 ** Copyright (C) 2013 Jan Pedersen <jp@jp-embedded.com>
 ** Copyright (C) 2015 Juriy Gurin <ygurin@outlook.com>
 ** 
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 ** 
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 ** 
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *************************************************************************/


/* log:
 *      + datamodel                                    26.07.14 ygurin
 *      + selftransitions                              27.07.14 ygurin
 *      + additional conditions parsing                03.08.14 ygurin
 *      + terminal virtual condition                   03.08.14 ygurin
 *      + includes                                     04.08.14 ygurin
 *      + custom header                                25.08.14 ygurin
 *      + action handlers                              26.08.14 ygurin
 */

#include "cpp_output.h"
#include "version.h"
#include <boost/algorithm/string.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <set>
#include <boost/foreach.hpp>
#include <map>
#include <iostream>
#include <vector>

using namespace std;

//todo set private/public/protected 
 
string cpp_output::classname()		{ return "C" + UpperFirstLetter(sc.sc().name) + "FSM"; }
string cpp_output::getActionHandlerName() { return  "I" + UpperFirstLetter(sc.sc().name) + "ActionHandler"; }
string cpp_output::getActionHandlerDefaultName() { return  "C" + UpperFirstLetter(sc.sc().name) + "ActionHandlerDefault"; }
string cpp_output::state_t()		{ return "state"; }
string cpp_output::state_composite_t()	{ return "composite"; }
string cpp_output::state_parallel_t()	{ return "parallel"; }
string cpp_output::state_actions_t()	{ return "state_actions"; }
	
void cpp_output::gen_transition_base()
{
	out << tab << "class no_state { /* dup */ };" << endl; 
	out << tab << "enum transition_type { external, internal };" << endl; 
	out << endl;

	const int max_targets = sc.parallel_target_sizes.size() ? *sc.parallel_target_sizes.rbegin() : 1;
	out << tab << "template<int index, event E, class S";
	for(int i = 0; i < max_targets; ++i) out << ", class D" << i << " = no_state";
	out << "> class transition_actions" << endl;
	out << tab << "{" << endl;
	out << tab << "protected:" << endl;
	out << tab << tab << "void enter(data_model&) { /* default enter action */ }" << endl;
	out << tab << tab << "bool condition(data_model&) { return true; } /* default condition action */" << endl;
	out << tab << "};" << endl;
	out << endl;

	// external transition
	out << tab << "// external transition" << endl;
	out << tab << "template<int index, event E, class S, class D = no_state, transition_type T = external> class transition: public transition_actions<index, E, S, D>" << endl;
	out << tab << "{" << endl;
	out << tab << "public:" << endl;
	// exit/enter is called here without parameter, which forces the action to always exit/enter at least current state
	out << tab << tab << "state* operator()( S *s, D &d, " << classname() << " &sc )" << endl;
	out << tab << tab << "{" << endl;
       	out << tab << tab << tab << "if( !transition_actions<index, E, S, D>::condition(sc.model) ) return 0;" << endl;
	if(opt.debug) out << tab << tab << tab << "std::clog << \"" << classname() << ": transition \" << typeid(S).name() << \" -> \" << typeid(D).name() << std::endl;" << endl;
	if(sc.using_parallel) out << tab << tab << tab << "s->exit_parallel( sc, s, &d );" << endl;
       	out << tab << tab << tab << "s->exit( sc.model, typeid(S) );" << endl;
       	out << tab << tab << tab << "s->template exit<D>( sc.model );" << endl;
       	out << tab << tab << tab << "transition_actions<index, E, S, D>::enter( sc.model );" << endl;
       	out << tab << tab << tab << "d.template enter<S>( sc.model );" << endl;
       	if(sc.using_parallel) out << tab << tab << tab << "return d.template enter_parallel<S>( sc, &d, s );" << endl;
	else out << tab << tab << tab << "return &d;" << endl;
       	out << tab << tab << "}" << endl;
	out << tab << "};" << endl;
	out << endl;
	
	// todo veryfy how these work at down transition
	out << tab << "// internal transition" << endl;
	out << tab << "template<int index, event E, class S, class D> class transition<index, E, S, D, internal>: public transition_actions<index, E, S, D>" << endl;
	out << tab << "{" << endl;
	out << tab << "public:" << endl;
	out << tab << tab << "state* operator()( S *s, D &d, " << classname() << " &sc )" << endl; 
	out << tab << tab << "{" << endl;
       	out << tab << tab << tab << "if ( !transition_actions<index, E, S, D>::condition(sc.model) ) return 0;" << endl;
	if(opt.debug) out << tab << tab << tab << "std::clog << \"" << classname() << ": transition \" << typeid(S).name() << \" -> \" << typeid(D).name() << std::endl;" << endl;
	if(sc.using_parallel) out << tab << tab << tab << "s->exit_parallel( sc, s, &d );" << endl;
       	out << tab << tab << tab << "s->exit( sc.model, typeid(S) );" << endl;
       	out << tab << tab << tab << "s->template exit<D>( sc.model, (D*)0 );" << endl;
       	out << tab << tab << tab << "transition_actions<index, E, S, D>::enter( sc.model );" << endl;
       	out << tab << tab << tab << "d.template enter<S>( sc.model, (S*)0 );" << endl;
       	if(sc.using_parallel) out << tab << tab << tab << "return d.template enter_parallel<S>( sc, &d, s );" << endl;
	else out << tab << tab << tab << "return &d;" << endl;
       	out << tab << tab << "}" << endl;
	out << tab << "};" << endl;
	out << endl;
	
	// transition without target
	out << tab << "// transition with no target" << endl;
	out << tab << "template<int index, event E, class S> class transition<index, E, S, no_state>: public transition_actions<index, E, S, no_state>" << endl;
	out << tab << "{" << endl;
	out << tab << "public:" << endl;
	out << tab << tab << "S* operator()( S *s, " << classname() << " &sc )" << endl;
       	out << tab << tab << "{" << endl;
       	out << tab << tab << tab << "if ( !transition_actions<index, E, S, no_state>::condition(sc.model) ) return 0;" << endl;
	if(opt.debug) out << tab << tab << tab << "std::clog << \"" << classname() << ": transition \" << typeid(S).name() << std::endl;" << endl;
       	out << tab << tab << tab << "transition_actions<index, E, S, no_state>::enter( sc.model );" << endl;
       	out << tab << tab << tab << "return s;" << endl;
       	out << tab << tab << "}" << endl;
	out << tab << "};" << endl;
	out << endl;

	for (set<int>::const_iterator i = sc.parallel_target_sizes.begin(); i != sc.parallel_target_sizes.end(); ++i) {
		const int sz = *i;

		out << tab << "template<event E, class S";
		for(int i = 0; i < sz; ++i) out << ", class D" << i;
		out << ", transition_type T = external> class transition" << sz << ';' << endl;
		
		out << tab << "template<event E, class S";
		for(int i = 0; i < sz; ++i) out << ", class D" << i;
		out << "> class transition" << sz << "<E, S";
		for(int i = 0; i < sz; ++i) out << ", D" << i;
		out << ", internal> : public transition_actions<E, S, D0, D1>" << endl;

		out << tab << '{' << endl;
		out << tab << tab << "public:" << endl;

		//todo: for now, all targets must have same parallel parent
		out << tab << tab << "state* operator ()(S *s";
		for(int i = 0; i < sz; ++i) out << ", D" << i << " &d" << i;
		out << ", sc_test576 &sc)" << endl;

		out << tab << tab << '{' << endl;

		out << tab << tab << tab << "if(!transition_actions<E, S";
		for(int i = 0; i < sz; ++i) out << ", D" << i;
		out << ">::condition(sc.model)) return 0;" << endl;

		if(opt.debug) {
			out << tab << tab << tab << "std::clog << \"" << classname() << ": transition \" << typeid(S).name() << \" -> \"";
			for(int i = 0; i < sz; ++i) {
				if(i) out << " << \", \"";
		       		out << " << typeid(D" << i << ").name()";
			}
			out << " << std::endl;" << endl;
		}

		out << tab << tab << tab << "s->exit_parallel(sc, s, &d0);" << endl;
		out << tab << tab << tab << "s->exit(sc.model, typeid(S));" << endl;
		out << tab << tab << tab << "s->template exit<D0>(sc.model, (D0*)0);" << endl;

		out << tab << tab << tab << "transition_actions<E, S";
		for(int i = 0; i < sz; ++i) out << ", D" << i;
		out << ">::enter( sc.model );" << endl;

		out << tab << tab << tab << "d0.template enter<S>( sc.model, (S*)0 );" << endl;
		out << tab << tab << tab << "return d0.template enter_parallel<S>( sc, &d0, s";
		for(int i = 1; i < sz; ++i) out << ", d" << i;
		out << " );" << endl;

		out << tab << tab << '}' << endl;
		out << tab << "};" << endl;
		
		out << endl;
	}
}

void cpp_output::gen_state_actions_base()
{
   out << "public:" << endl;
	out << tab << "template<class C> class " << state_actions_t() << endl;
	out << tab << "{" << endl;
	// enter/exit
    	out << tab << "protected:" << endl;
    	out << tab << tab << "void inline enter( data_model& ) { /* default enter action */ }" << endl;
	out << tab << tab << "void inline exit( data_model& ) { /* default exit action */ }" << endl;
	out << tab << "};" << endl;
	out << endl;
}

void cpp_output::gen_state_composite_base()
{
	out << tab << "template<class C, class P> class " << state_composite_t() << ": public P, public state_actions<C>" << endl;
	out << tab << "{" << endl;
	
	out << tab << tab << "virtual " << state_t() << "* initial( " << classname() << "& ) { return 0; }" << endl;

	// lca calculation
	out << tab << "public:" << endl;
	out << tab << tab << "// LCA calculation" << endl;
	out << tab << tab << "template<class T> void enter( data_model&, " << state_composite_t() << "* ) {}" << endl;

	out << tab << tab << "template<class T> void enter( data_model &m, ... ) { P::template enter<T>( m, (T*)0 );";
	if(opt.debug) out << " std::clog << \"" << classname() << ": enter \" << typeid(C).name() << std::endl;";
       	out << " state_actions<C>::enter( m ); }" << endl;

	out << tab << tab << "template<class T> void exit( data_model&, " << state_composite_t() << "* ) {}" << endl;

	out << tab << tab << "template<class T> void exit( data_model &m, ... ) {";
	if(opt.debug) out << " std::clog << \"" << classname() << ": exit \" << typeid(C).name() << std::endl;";
	out << " state_actions<C>::exit( m ); P::template exit<T>( m, (T*)0 ); }" << endl;

	out << tab << tab << "virtual void exit( data_model &m, const std::type_info &sti ) { if ( typeid(C) == sti ) return;";
	if(opt.debug) out << " std::clog << \"" << classname() << ": exit \" << typeid(C).name() << std::endl;";
       	out << " state_actions<C>::exit( m ); P::exit( m, sti ); }" << endl;

	out << tab << "};" << endl;
	out << endl;
}

void cpp_output::gen_state_parallel_base()
{
	if (!sc.using_parallel) return;

	if(sc.parallel_sizes.size() == 0) {
		//todo make composite, if children < 2
		cerr << "error: parallel state with < 2 states is currently not supported" << endl;
		exit(1);
	}

	const int min_c = *sc.parallel_sizes.begin();
	const int max_c = *sc.parallel_sizes.rbegin();
	if(min_c < 2) {
		//todo make composite, if children < 2
		cerr << "error: parallel state with < 2 states is currently not supported" << endl;
		exit(1);
	}

	//todo combine with no_class
	if(min_c < max_c) out << tab << "class no_class {};" << endl;

	for (set<int>::reverse_iterator i = sc.parallel_sizes.rbegin(); i != sc.parallel_sizes.rend(); ++i) {
		const int children = *i;
		out << tab << "template<class C, class P";
		for(int c = 0; c < children; ++c) {
			out << ", class C" << c;
			if ((children == max_c) && (c >= min_c)) out << " = no_class";
		}
		out << "> class " << state_parallel_t();
		if(children < max_c) {
			out << " <C, P";
			for (int c = 0; c < children; ++c) out << ", C" << c;
			out << '>';
		}
		out << " : public composite<C, P>" << endl;
		out << tab << '{' << endl;
		out << tab << tab << "public:" << endl;
		for(int c = 0; c < children; ++c) {
			out << tab << tab << "template<class S> " << state_t() << "* enter_parallel(" << classname() << " &sc, C" << c << "*, C*) { return this; }" << endl;
			out << tab << tab << "template<class S> " << state_t() << "* enter_parallel(" << classname() << " &sc, C" << c << " *d, " << state_t() << "*)" << endl;
			out << tab << tab << '{' << endl;
			out << tab << tab << tab << "// parallel state entered with C" << c << " or child of as target" << endl;
			out << tab << tab << tab << "P::template enter_parallel<S>(sc, d, (S*)0);" << endl;
			for(int n = 0; n < children; ++n) {
				if (n == c) continue;
				out << tab << tab << tab << "sc.cur_state.push_back(d->init_child(sc, (C" << n << "*)0));" << endl;
			}
			out << tab << tab << tab << "return this;" << endl;
			out << tab << tab << "};" << endl;
			out << tab << tab << endl;

			//todo to continue
		}

		out << tab << tab << "template<class S> " << state_t() << "* enter_parallel(" << classname() << " &sc, C*, C*) { return this; }" << endl;
		out << tab << tab << "template<class S> " << state_t() << "* enter_parallel(" << classname() << " &sc, C *d, " << state_t() << "*)" << endl;
		out << tab << tab << '{' << endl;
		out << tab << tab << tab << "// parallel state entered with parallel as target" << endl;
		out << tab << tab << tab << "P::template enter_parallel<S>(sc, d, (S*)0);" << endl;
		for(int n = 0; n < children - 1; ++n) {
			out << tab << tab << tab << "sc.cur_state.push_back(d->init_child(sc, (C" << n << "*)0));" << endl;
		}
		out << tab << tab << tab << "return d->init_child(sc, (C" << children - 1 << "*)0);" << endl;
		out << tab << tab << "};" << endl;
		out << endl;

		// handle transition with all children given
		// todo, only add if needed
		out << tab << tab << "template<class S";
		for(int c = 1; c < children; ++c) out << ", class D" << c;
		out << "> " << state_t() << "* enter_parallel(" << classname() << " &sc, C*, C*";
		for(int c = 1; c < children; ++c) out << ", D" << c << "&";
		out << ") { return this; }" << endl;
		out << tab << tab << "template<class S";
		for(int c = 1; c < children; ++c) out << ", class D" << c;
		out << "> " << state_t() << "* enter_parallel(" << classname() << " &sc, C *d, " << state_t() << '*';
		for(int c = 1; c < children; ++c) out << ", D" << c << " &d" << c;
		out << ")" << endl;
	        out << tab << tab << '{' << endl;
		out << tab << tab << tab << "// handle transition with all children given" << endl;
		out << tab << tab << tab << "P::template enter_parallel<S>(sc, d, (S*)0);" << endl;
		for(int c = 1; c < children; ++c) out << tab << tab << tab << 'd' << c << ".template enter<C>(sc.model, (C*)0), sc.cur_state.push_back(&d" << c << ");" << endl;

		// todo call init_child where Cn is not a child of Dn or *d
		// if (false) sc.cur_state.push_back(d->init_child(sc, (C0*)0));
		// if (false) sc.cur_state.push_back(d->init_child(sc, (C1*)0));
		
	        out << tab << tab << tab << "return this;" << endl;
	        out << tab << tab << '}' << endl;
		out << endl;

		out << tab << tab << "bool parallel_parent(const std::type_info& pti) { return typeid(C) == pti; }" << endl;
		out << tab << tab << "void exit_parallel(" << classname() << " &sc, C*, C*) {}" << endl;
		out << tab << tab << "void exit_parallel(" << classname() << " &sc, C *s, state *d)" << endl;
		out << tab << tab << '{' << endl;
		out << tab << tab << tab << "// parallel state exited from C or child" << endl;
		out << tab << tab << tab << "for(" << classname() << "::cur_state_l::iterator i = sc.cur_state.begin(); (i != sc.cur_state.end()) && *i; ++i) {" << endl;
		out << tab << tab << tab << tab << "if(this == *i) continue;" << endl;
		out << tab << tab << tab << tab << "if(!(*i)->parallel_parent(typeid(C))) continue;" << endl;
		out << tab << tab << tab << tab << "(*i)->exit(sc.model, typeid(C));" << endl;
		out << tab << tab << tab << tab << "*i = 0;" << endl;
		out << tab << tab << tab << '}' << endl;
		out << tab << tab << tab << "P::exit_parallel(sc, s, d);" << endl;
		out << tab << tab << '}' << endl;


		out << tab << "};" << endl;
		out << endl;
	}
}

void cpp_output::gen_model_base()
{
   out << "public:" << endl;
   out << tab << "struct " << "data_model {" << endl;
   out << tab << tab << "friend class " << classname() << ";" << endl;
   const std::vector<std::pair<std::string, std::string> >& dm = sc.sc().datamodel;

   typedef const std::pair<std::string, std::string> var_pair;

   out << tab << tab << "data_model( void )";

   std::string inits = "";
   BOOST_FOREACH( var_pair& p, dm )
   {
      if ( !inits.empty() ) inits += ", ";
      inits += p.second + "()";
   }
   if ( !inits.empty() )
   {
      out << ": " << inits;
   }
   out << " { /* nop */ }" << endl;

   BOOST_FOREACH( var_pair& p, dm )
   {
      out << tab << tab << p.first << " " << p.second << ";" << endl;
   }

   out << tab << "private:" << endl;
   out << tab << tab << "I" << UpperFirstLetter(sc.sc().name) + "ActionHandler* actionHandler;" << endl;

   out << tab << "} model;" << endl;

   if ( dm.size() > 0 ) out << endl << "public:" << endl;
   BOOST_FOREACH( var_pair& p, dm )
   {
      std::string var_name = p.second;
      var_name[0] = toupper(var_name[0]);
      out << tab << "// --------------------------------------------------------------------------" << endl;
      out << tab << "void set" << var_name << "( const " << p.first + "& value )" << endl;
      out << tab << "// --------------------------------------------------------------------------" << endl;
      out << tab << "{" << endl;
      out << tab << tab << "model." << p.second << " = value;" << endl;
      out << tab << "}" << endl;
   }

	out << endl;
}

void cpp_output::gen_state_base()
{
	const scxml_parser::state_list &states = sc.sc().states;

	// state actions class
   out << "public:" << endl;
	out << tab << "class " << state_t() << endl;
	out << tab << "{" << endl;
	out << tab << "public:" << endl;

	// events
	// pass through set, to sort out dublicates
	set<string> event_set;
	for (scxml_parser::state_list::const_iterator s = states.begin(); s != states.end(); ++s) {
		for (scxml_parser::transition_list::const_iterator i = s->get()->transitions.begin(); i != s->get()->transitions.end(); ++i) {
			if(i->get()->event) event_set.insert(*i->get()->event);
		}
	}
	for (set<string>::const_iterator i = event_set.begin(); i != event_set.end(); ++i) {
      std::string trig_event_name = *i;
      bool been = std::find_if(trig_event_name.begin(), trig_event_name.end(), [](char c)->bool{ return c == '@'; }) != trig_event_name.end();
      trig_event_name.erase(std::remove(trig_event_name.begin(), trig_event_name.end(), '@'), trig_event_name.end());
      out << tab << tab << "virtual " << state_t() << "* trigger_" << trig_event_name << "( " << classname() << "& ) { return 0; }" << endl;
      if (been)
       out << tab << tab << "virtual " << state_t() << "* event_" << trig_event_name << "( " << classname() << "& ) { return 0; }" << endl;
	}
	out << tab << tab << "virtual " << state_t() << "* unconditional( " << classname() << "& ) { return 0; }" << endl;
   out << tab << tab << "virtual " << state_t() << "* unconditional_async( " << classname() << "& ) { return 0; }" << endl;
	out << tab << tab << "virtual " << state_t() << "* initial( " << classname() << "& ) { return 0; }" << endl;

	out << endl;

	out << tab << tab << "template<class T> void enter( data_model&, ... ) {}" << endl;
	out << tab << tab << "template<class T> void exit( data_model&, ... ) {}" << endl;
	out << tab << tab << "virtual void exit( data_model&, const std::type_info& ) {}" << endl;
	if(sc.using_parallel) {
 		out << tab << tab << "template<class S> " << state_t() << "* enter_parallel( " << classname() << "&, " << state_t() << "*, " << state_t() << "* ) { return this; }" << endl;
 		out << tab << tab << "virtual void exit_parallel( " << classname() << "&, " << state_t() << "*, " << state_t() << "* ) {}" << endl;
 		out << tab << tab << "virtual bool parallel_parent( const std::type_info& ) { return false; }" << endl;
	}

	out << tab << tab << "virtual ~" << state_t() << "( void ) {}" << endl;

	out << tab << "};" << endl;
	out << endl;

	if(sc.using_parallel) {
		out << tab << "typedef std::vector<" << state_t() << "*> cur_state_l;" << endl;
		out << tab << "cur_state_l cur_state;" << endl;
	}
	else {
		out << tab << state_t() << " *cur_state;" << endl;
	}
	out << tab << "typedef " << state_t() << "* ( " << state_t() << "::*event )( " << classname() << "& );" << endl;
	out << endl;
}

scxml_parser::state_list cpp_output::children(const scxml_parser::state &state)
{
   scxml_parser::state_list states;
   for (scxml_parser::state_list::const_iterator i = sc.sc().states.begin(); i != sc.sc().states.end(); ++i) {
      if ((*i)->parent && (*i)->parent->id == state.id) {
         states.push_back(*i);
      }
   }
   return states;
}

bool isAsyncTransitions(const scxml_parser::state &state)
{
   for (scxml_parser::transition_list::const_iterator t = state.transitions.begin(); t != state.transitions.end(); ++t) {
      if (t->get()->event){
         std::string trig_event_name = *t->get()->event;
         if (std::find_if(trig_event_name.begin(), trig_event_name.end(), [](char c)->bool{ return c == '@'; }) != trig_event_name.end())
            return true;
      }
   }
   return false;
}

void cpp_output::gen_state(const scxml_parser::state &state)
{
   string parent, prefix;
	if ( state.type && *state.type == "inter" ) prefix = "inter";

   // inner states
	if ( state.parent ) parent = "state_" + state.parent->id;
	else parent = "scxml";

	string state_classname = prefix + "state_" + state.id;

	out << tab << "class " << state_classname << ": public ";

   out << state_composite_t();

   out << '<' << state_classname << ", " << parent;

   out << ">";
   
   out << endl;

	out << tab << "{" << endl;

   // there is an initial transition in this state
	if ( state.initial.size() ) {
		string target = "sc.m_state_" + state.initial.front();
		string target_classname = "state_" + state.initial.front();
		out << tab << tab << state_t() << "* " << "initial" << "( " << classname() << " &sc ) { return transition<0, &state::initial, " << state_classname << ", " << target_classname << ", internal>()( this, " << target << ", sc ); }" << endl;
	}

   // One event can owned many transitions (with different conditions)
   std::map< std::string, std::vector<std::string> > mth;
   std::map< std::string, std::string> mth_async;
   bool isUnconditionalTransitions = false;
   bool isAutotransitional = true;
	//events
   bool async_autotransition = false;
   // t is iterator to [shared_ptr of transition]
	for (scxml_parser::transition_list::const_iterator t = state.transitions.begin(); t != state.transitions.end(); ++t) {
		string target;
		string target_classname = state_classname;
		string transitionName;
      string transitionEventName;

		if ( t->get()->target.size() ) {
			target = "sc.m_state_" + t->get()->target.front(); //todo handle multiple targets
			target_classname = "state_" + t->get()->target.front(); //todo handle multiple targets
		}

      bool async = false;
      
		if (t->get()->event) {
         std::string trig_event_name = *t->get()->event;
         async = std::find_if(trig_event_name.begin(), trig_event_name.end(), [](char c)->bool{ return c == '@'; }) != trig_event_name.end();
         trig_event_name.erase(std::remove(trig_event_name.begin(), trig_event_name.end(), '@'), trig_event_name.end());


        // async_autotransition |= async && trig_event_name == "";

         if (async && trig_event_name == "") {
            async_autotransition = true;
            isUnconditionalTransitions = true;
            isAutotransitional = false;
            transitionName = "unconditional";
         }
         else {
            isAutotransitional = false;

            transitionName = "trigger_" + trig_event_name;
            transitionEventName = "event_" + trig_event_name;
         }
		}
		else {
         isUnconditionalTransitions = true;
         
         transitionName = "unconditional";
		}

      std::string s = t->get()->condition;
      if ( s != "" )
      {
         Condition c(s);
         c.fixCondition(sc.sc().datamodel);
         s = "if ( " + c.getCondition() + " ) ";
      }
      else
      {
         s = "if ( true ) ";
      }
      
      mTransitionIndex++;
      t->get()->index = mTransitionIndex;
		if( target.size() ) {
			// normal transition
         mth[transitionName].push_back(s + "return transition<" + std::to_string(mTransitionIndex) + ", &state::" + transitionName + ", " + state_classname + ", " + target_classname + ">()( this, " + target + ", sc );");
         t->get()->event_fmt = "state::" + transitionName;
         t->get()->source_fmt = state_classname;
         t->get()->dest_fmt = target_classname;

         if (async && transitionName != "unconditional") {
            mth_async[transitionEventName] = "return sc.asyncTriggerInvoker.makeAsyncCall( &state::" + transitionName + " );";
         }
		}
		else {
			// transition without target
         mth[transitionName].push_back(s + "return transition<" + std::to_string(mTransitionIndex) + ", &state::" + transitionName + ", " + state_classname + ">()( this, sc );");
		}
	}
   
   for ( std::map< std::string, std::vector<std::string> >::iterator it = mth.begin(); it != mth.end(); it ++ )
   {
      std::vector<std::string>& v = it->second;

      out << tab << tab << "// --------------------------------------------------------------------------" << endl;
      out << tab << tab << "inline " << (isUnconditionalTransitions ? "virtual " : "") << state_t() << "* " << it->first << "( " << classname() << " &sc ) {" << endl;
      out << tab << tab << "// --------------------------------------------------------------------------" << endl;

      for ( std::vector<std::string>::iterator i = v.begin(); i != v.end(); i++ )
      {
         out  << tab << tab << tab << *i << endl;
      }
      if (v.size() > 0)
         out << tab << tab << tab << "else return 0;" << endl;
      out << tab << tab << "}" << endl;
   }

   if (isUnconditionalTransitions && async_autotransition)
   {
      out << tab << tab << "// --------------------------------------------------------------------------" << endl;
      out << tab << tab << "inline virtual state* unconditional_async(" << classname() << " &sc) {" << endl;
      out << tab << tab << "// --------------------------------------------------------------------------" << endl;
      out << tab << tab << tab << "return sc.asyncTriggerInvoker.makeAsyncUCall(&state::unconditional);" << endl;
      out << tab << tab << "}" << endl;
   }

   for (std::map< std::string, std::string >::iterator it = mth_async.begin(); it != mth_async.end(); it++)
   {
      out << tab << tab << "// --------------------------------------------------------------------------" << endl;
      out << tab << tab << "inline " << (isUnconditionalTransitions ? "virtual " : "") << state_t() << "* " << it->first << "( " << classname() << " &sc ) { " << it->second << " }" << endl;
      out << tab << tab << "// --------------------------------------------------------------------------" << endl;
   }

   if ( isAsyncTransitions(state) ) {
      out << endl;
      out << tab << tab << "// --------------------------------------------------------------------------" << endl;
      out << tab << tab << "inline virtual void processEvent( const CPAsyncEvent& event ) { event.getData().dispatch(); }" << endl;
      out << tab << tab << "// --------------------------------------------------------------------------" << endl;
      out << endl;
   }

   if ( !isAutotransitional && !isUnconditionalTransitions )
   {
      out << tab << tab << "virtual state* unconditional( " << classname() << "& ) { return 0; }" << std::endl;
   }
      
	out << tab << "} m_" << prefix << "state_" << state.id << ";" << endl;
	out << endl;
}

void cpp_output::gen_async_event(void)
{
   out << "private: " << endl;
   out << tab << "class CPAsyncEventTransitionData" << endl;
   out << tab << "{" << endl;
   out << tab << "public:" << endl;
   out << tab << tab << "// --------------------------------------------------------------------------" << endl;
   out << tab << tab << "CPAsyncEventTransitionData( " << classname() << "& fsm, const event trigger ): mFSM( fsm ), mTrigger( trigger ) { /* none */ }" << endl;
   out << tab << tab << "// --------------------------------------------------------------------------" << endl << endl;

   out << tab << tab << "// --------------------------------------------------------------------------" << endl;
   out << tab << tab << "inline void dispatch( void ) { mFSM.dispatch( mTrigger ); }" << endl;
   out << tab << tab << "// --------------------------------------------------------------------------" << endl << endl;

   out << tab << "private:" << endl;
   out << tab << tab << classname() << "& mFSM;" << endl;
   out << tab << tab << "event mTrigger;" << endl;
   out << tab << "};" << endl << endl;

   out << tab << "DECLARE_EVENT( CPAsyncEvent, CPAsyncEventTransitionData, IPAsyncEventConsumer );" << endl << endl;


   out << tab << "class CPAsyncTriggerInvoker : public IPAsyncEventConsumer" << endl;
   out << tab << "{" << endl;
   out << tab << "public:" << endl;
   out << tab << tab << "// --------------------------------------------------------------------------" << endl;
   out << tab << tab << "CPAsyncTriggerInvoker( " << classname() << "& fsm ) : mFsm( fsm ) { /* none */ }" << endl;
   out << tab << tab << "// --------------------------------------------------------------------------" << endl << endl;

   out << tab << tab << "// --------------------------------------------------------------------------" << endl;
   out << tab << tab << "inline state* makeAsyncCall( const event trigger ) {" << endl;
   out << tab << tab << "// --------------------------------------------------------------------------" << endl;
   out << tab << tab << tab << "CPAsyncEvent* e = CPAsyncEvent::createEvent( CPAsyncEventTransitionData(mFsm, trigger) );" << endl;
   out << tab << tab << tab << "e->setConsumer( this );" << endl;
   out << tab << tab << tab << "e->send();" << endl;
   out << tab << tab << tab << "return 0;" << endl;
   out << tab << tab << "}" << endl << endl;

   out << tab << tab << "// --------------------------------------------------------------------------" << endl;
   out << tab << tab << "inline state* makeAsyncUCall( const event trigger ) {" << endl;
   out << tab << tab << "// --------------------------------------------------------------------------" << endl;
   out << tab << tab << tab << "CPAsyncEvent* e = CPAsyncEvent::createEvent( CPAsyncEventTransitionData(mFsm, trigger) );" << endl;
   out << tab << tab << tab << "e->setConsumer( this );" << endl;
   out << tab << tab << tab << "e->send();" << endl;
   out << tab << tab << tab << "return (state*)0xFFFFFF;" << endl;
   out << tab << tab << "}" << endl << endl;

   out << tab << "protected:" << endl;
   out << tab << tab << "// --------------------------------------------------------------------------" << endl;
   out << tab << tab << "inline virtual void processEvent( const CPAsyncEvent& event ) { event.getData().dispatch(); }" << endl;
   out << tab << tab << "// --------------------------------------------------------------------------" << endl << endl;

   out << tab << "private:" << endl;
   out << tab << tab << classname() << "& mFsm;" << endl;
   out << tab << "} asyncTriggerInvoker;" << endl << endl;

//   out << tab << "// --------------------------------------------------------------------------" << endl;
//   out << tab << "template <typename T> static state* makeAsyncCall( CTestFSM& fsm, T* consumer, const event trigger )" << endl;
//   out << tab << "// --------------------------------------------------------------------------" << endl;
//   out << tab << "{" << endl;
//   out << tab << tab << "CPAsyncEvent* e = CPAsyncEvent::createEvent( CPAsyncEventTransitionData( fsm, trigger ) );" << endl;
//   out << tab << tab << "e->setConsumer( consumer );" << endl;
//   out << tab << tab << "e->send();" << endl;
//   out << tab << tab << "return 0;" << endl;
//   out << tab << "}" << endl << endl;
}

void cpp_output::gen_sc()
{
	const scxml_parser::state_list &states = sc.sc().states;

   out << "class I" << UpperFirstLetter(sc.sc().name) + "ActionHandler;" << endl;

	out << "class " << classname() << endl;
	out << "{" << endl;

	gen_model_base();
	gen_state_base();
	if (opt.withAsyncs) {
	   gen_async_event();
	}
	gen_state_actions_base();
	gen_state_composite_base();
	gen_state_parallel_base();
	gen_transition_base();

	// dispatch
	if(sc.using_parallel) {
		out << tab << "private: bool dispatch_event(event e)" << endl;
		out << tab << '{' << endl;
		out << tab << tab << "bool cont = false;" << endl;
		out << tab << tab << "state *next_state;" << endl;
		out << tab << tab << "for(unsigned i = 0; i < cur_state.size();) if(cur_state[i]) {" << endl;
		out << tab << tab << tab << "if ((next_state = (cur_state[i]->*e)(*this))) cur_state[i] = next_state, cont = true;" << endl;
		out << tab << tab << tab << "++i;" << endl;
		out << tab << tab << '}' << endl;
		out << tab << tab << "else cur_state.erase(cur_state.begin() + i);" << endl;
		out << tab << tab << "return cont;" << endl;
		out << tab << '}' << endl;
	}
	else {
		out << "private:" << endl;
      out << tab << "// --------------------------------------------------------------------------" << endl;
      out << tab << "bool dispatch_event( event e )" << endl;
      out << tab << "// --------------------------------------------------------------------------" << endl;
		out << tab << '{' << endl;
		out << tab << tab << "state *next_state;" << endl;
		out << tab << tab << "if ( (next_state = (cur_state->*e)(*this)) ) cur_state = next_state;" << endl;
		out << tab << tab << "return next_state;" << endl;
		out << tab << '}' << endl;

      if (opt.withAsyncs) {
         out << tab << "// --------------------------------------------------------------------------" << endl;
         out << tab << "bool dispatch_uasync( event e )" << endl;
         out << tab << "// --------------------------------------------------------------------------" << endl;
         out << tab << "{" << endl;
         out << tab << tab << "return (cur_state->*e)(*this) == (state*)0xFFFFFF;" << endl;
         out << tab << "}" << endl;
      }
	}
	out << endl;
	out << "public:" << endl;
   out << tab << "// --------------------------------------------------------------------------" << endl;
   out << tab << "void dispatch( event e )" << endl;
   out << tab << "// --------------------------------------------------------------------------" << endl;
	out << tab << "{" << endl;
	out << tab << tab << "bool cont = dispatch_event( e );" << endl;
	out << tab << tab << "while ( cont ) {" << endl;
	out << tab << tab << tab << "if ( (cont = dispatch_event(&state::initial)) );" << endl;
   if (opt.withAsyncs) {
      out << tab << tab << tab << "else if ( dispatch_uasync(&state::unconditional_async) );" << endl;
   }
	out << tab << tab << tab << "else if ( (cont = dispatch_event(&state::unconditional)) );" << endl;
	//out << tab << tab << tab << "else if (event_queue.size()) cont = dispatch_event(event_queue.front()), event_queue.pop();" << endl;
	out << tab << tab << tab << "else break;" << endl;
	out << tab << tab << "}" << endl;
	out << tab << "}" << endl;
	out << endl;

	// constructor
   out << tab << "// --------------------------------------------------------------------------" << endl;
	out << tab << classname() << "( " << "I" << UpperFirstLetter(sc.sc().name) + "ActionHandler* pActionHandler" << " )";
	if(!sc.using_parallel) out << " : cur_state( &m_scxml )";
   if (opt.withAsyncs) {
      out <<  ", asyncTriggerInvoker( *this )";
   }
	out << endl;
   out << tab << "// --------------------------------------------------------------------------" << endl;

	out << tab << "{" << endl;
	if(sc.using_parallel) out << tab << tab << "cur_state.push_back(&m_scxml);" << endl;
   out << tab << tab << "model.actionHandler = pActionHandler;" << endl;
	//out << tab << tab << "model.user = user;" << endl;
//	out << tab << tab << "dispatch(&state::initial);" << endl;
	out << tab << "}" << endl;
	out << endl;

   out << tab << "// --------------------------------------------------------------------------" << endl;
   out << tab << "void init( void )" << endl;
   out << tab << "// --------------------------------------------------------------------------" << endl;
   out << tab << "{" << endl;
   out << tab << tab << "dispatch( &state::initial );" << endl;
   out << tab << "}" << endl;
   out << endl;

	//m_scxml
	out << tab << "class scxml : public composite<scxml, state>" << endl;
	out << tab << "{" << endl;

	const int sz = sc.sc().initial.size();
   out << tab << tab << "// --------------------------------------------------------------------------" << endl;
   out << tab << tab << state_t() << "* initial( " << classname() << "&sc ) {" << endl;
   out << tab << tab << "// --------------------------------------------------------------------------" << endl;
   out << tab << tab << tab << "return transition";
	if(sz > 1) out << sz;
	out << "<0, &state::initial, scxml";
	for(int i = 0; i < sz; ++i) out << ", state_" << sc.sc().initial[i];
	out << ", internal>()( this";
	for(int i = 0; i < sz; ++i) out << ", sc.m_state_" << sc.sc().initial[i];
	out << ", sc ); }" << endl;

   out << tab << tab << "} m_scxml;" << endl;
	out << endl;
	
	// states
	for (scxml_parser::state_list::const_iterator s = states.begin(); s != states.end(); ++s) {
		gen_state(*s->get());
	}

	out << "};" << endl << endl;
}

void cpp_output::trim()
{
	const scxml_parser::state_list &states = sc.sc().states;

	// replace '-' with '_' in event names
	// replace '*' with 'A' in event names, todo: not currently supported
	// replace '.' with '_' in event names todo: see '3.12.1 event descriptors how to handle event tokens
	for (scxml_parser::state_list::const_iterator s = states.begin(); s != states.end(); ++s) {
		for (scxml_parser::transition_list::const_iterator i = s->get()->transitions.begin(); i != s->get()->transitions.end(); ++i) {
			if(i->get()->event) {
				if(i->get()->event->find('.') != string::npos) cerr << "warning: event tokens not currently supported" << endl;
				if(i->get()->event->find('*') != string::npos) cerr << "warning: event asteriks not currently supported" << endl;
				replace(i->get()->event->begin(), i->get()->event->end(), '.', '_');
				replace(i->get()->event->begin(), i->get()->event->end(), '-', '_');
				replace(i->get()->event->begin(), i->get()->event->end(), '*', 'A');
			}
		}
	}
}

void cpp_output::gen()
{
	if (sc.using_parallel) cerr << "warning: parallel support is not fully implemented/tested" << endl;

   out << "#ifndef AUTOFSM_" << boost::to_upper_copy(sc.sc().name) << "_HPP" << endl;
	out << "#define AUTOFSM_" << boost::to_upper_copy(sc.sc().name) << "_HPP" << endl;
	out << endl;

   out << "/*" << endl;
   out << " *                                     escxmlcc product" << endl;
   out << " *                                           fsm" << endl;
   out << " *" << endl;
   out << " * @ingroup Autogenerated::FSM" << endl;
   out << " * @author  escxmlcc" << endl;
   out << " * " << endl;
   out << " * @brief   This program was generated by escxmlcc fsm compiler written by Yuriy Gurin. This is" << endl;
   out << " *          an extended version of original scxmlcc compiler written by Jan Pedersen. For more" << endl;
   out << " *          information see project website https://github.com/aeore/escxmlcc" << endl;
   out << " */" << endl;
	out << endl;

   out << "// --------------------------------------------------------------------------" << endl;
   out << "// Includes" << endl;
   out << "// --------------------------------------------------------------------------" << endl;
   out << endl;

	out << "#include <typeinfo>" << endl;
//	out << "#include <memory>" << endl;
   if (opt.withAsyncs) {
      out << "#include \"Environment.hpp\"" << endl;
   }
	if(opt.debug) out << "#include <iostream>" << endl;

  /* BOOST_FOREACH( const std::string& s, sc.sc().includes )
   {
      out << "#include " << s << endl;
   }*/

	out << endl;

   out << "// --------------------------------------------------------------------------" << endl;
   out << "// User space" << endl;
   out << "// --------------------------------------------------------------------------" << endl;
   out << endl;

   std::string userHeader = sc.sc().header;
   if ( userHeader == "" ) {
      out << "/* no user data */" << endl;
   } else {
      out << sc.sc().header;
   }
   out << endl;

   out << "// --------------------------------------------------------------------------" << endl;
   out << "// Code definition" << endl;
   out << "// --------------------------------------------------------------------------" << endl;
   out << endl;

	trim();
	gen_sc();
   gen_ahi_header();
   gen_ahd_header();

   out << endl;

   gen_template_calls();
  
	// end of include guard
   out << "/* ************************************************************************ */" << endl;
 	out << "#endif // AUTOFSM_" << boost::to_upper_copy(sc.sc().name) << "_HPP" << endl;
}

void cpp_output::gen_ahi_header()
{
   out << "class " << getActionHandlerName() << endl;
   out << "{" << endl;
   out << "public:" << endl;
   out << tab << "/* state handlers */" << endl;
   const scxml_parser::state_list &states = sc.sc().states;
   for (scxml_parser::state_list::const_iterator s = states.begin(); s != states.end(); ++s)
   {
      scxml_parser::state state = *s->get();
      string stateName = UpperFirstLetter(state.id);

      out << tab << "virtual void state" << stateName << "Enter( " << classname() << "::data_model &m ) = 0;" << endl;
      out << tab << "virtual void state" << stateName << "Exit( " << classname() << "::data_model &m ) = 0;" << endl;
	}

   out << endl;
   out << tab << "/* transition handlers */" << endl;
   const std::vector<scxml_parser::transition_action>& transitionHandlers = sc.sc().transition_actions;
   for ( std::vector<scxml_parser::transition_action>::const_iterator it = transitionHandlers.begin(); it != transitionHandlers.end(); ++it )
   {
      const scxml_parser::transition_action& t_action = *it;

      std::string params = "";
      for ( std::vector<scxml_parser::param>::const_iterator arg_it = t_action.params.begin(); arg_it != t_action.params.end(); ++arg_it ) {
         scxml_parser::param p = *arg_it;
         if ( params != "") params += ", ";
         params += p.type + " " + p.name;
      }
      if ( params != "" ) {
         params = ", " + params;
      }

      out << tab << "virtual void on" << UpperFirstLetter(t_action.name) << "( " << classname() << "::data_model& m" << params << " ) = 0;" << endl;
   }

   out << endl;
   out << "protected:" << endl;
   out << tab << "virtual ~" << getActionHandlerName() << "( void ) { /* nop */ }" << endl;

   out << "};" << endl;
   out << endl;
}

void cpp_output::gen_ahd_header()
{
   out << "class " << getActionHandlerDefaultName() << ": public " << getActionHandlerName() << endl;
   out << "{" << endl;
   out << "public:" << endl;
   out << tab << "/* state handlers */" << endl;

   const scxml_parser::state_list &states = sc.sc().states;
   for (scxml_parser::state_list::const_iterator s = states.begin(); s != states.end(); ++s)
   {
      scxml_parser::state state = *s->get();
      string stateName = UpperFirstLetter(state.id);

      out << tab << "virtual void state" << stateName << "Enter( " << classname() << "::data_model &m ) { (void)m; /* default */ }" << endl;
      out << tab << "virtual void state" << stateName << "Exit( " << classname() << "::data_model &m ) { (void)m; /* default */ }" << endl;
	}

   out << endl;
   out << tab << "/* transition handlers */" << endl;
   const std::vector<scxml_parser::transition_action>& transitionHandlers = sc.sc().transition_actions;
   for ( std::vector<scxml_parser::transition_action>::const_iterator it = transitionHandlers.begin(); it != transitionHandlers.end(); ++it )
   {
      const scxml_parser::transition_action& t_action = *it;

      std::string params = "";
      for ( std::vector<scxml_parser::param>::const_iterator arg_it = t_action.params.begin(); arg_it != t_action.params.end(); ++arg_it ) {
         scxml_parser::param p = *arg_it;
         if ( params != "") params += ", ";
         params += p.type + " " + p.name;
      }
      if ( params != "" ) {
         params = ", " + params;
      }

	  out << tab << "virtual void on" << UpperFirstLetter(t_action.name) << "( " << classname() << "::data_model& m" << params << " ) {" << endl;
	  out << tab << tab << "(void)m;" << endl;
	  for (std::vector<scxml_parser::param>::const_iterator arg_it = t_action.params.begin(); arg_it != t_action.params.end(); ++arg_it) {
		  scxml_parser::param p = *arg_it;
		  out << tab << tab << "(void)" + p.name + ";" << endl;
	  }
		  
      out << tab << "}" << endl;
   }

   out << endl;
   out << "protected:" << endl;
   out << tab << "virtual ~" << getActionHandlerDefaultName() << "( void ) { /* nop */ }" << endl;

   out << "};" << endl;
   out << endl;
}

std::string getMethodPartFromExecutableContent( std::string& content )
{
   std::string result = "";
   for ( std::string::const_iterator it = content.begin(); it != content.end(); ++it ) {
      if ( std::isalnum(*it, locale()) || *it == '_' ) result += *it;
      else break;
   }
   return result;
}

string trim(string str)
{
  str.erase(str.begin(), find_if(str.begin(), str.end(),
    [](char& ch)->bool { return !isspace(ch); }));
  str.erase(find_if(str.rbegin(), str.rend(),
    [](char& ch)->bool { return !isspace(ch); }).base(), str.end());
  return str;
}  

void parseExecutableMethodCall( std::string content, std::string& method, std::string& params )
{
   method = getMethodPartFromExecutableContent( content );

   std::string::size_type first_quote = content.find('(' );
   std::string::size_type last_quote = content.rfind( ')' );

   const std::string subst = content.substr( first_quote + 1, last_quote - first_quote - 1  );
   params =  trim( subst );
}

std::vector<std::string> split_string(const std::string& str,
   const std::string& delimiter)
{
   std::vector<std::string> strings;

   std::string::size_type pos = 0;
   std::string::size_type prev = 0;
   while ((pos = str.find(delimiter, prev)) != std::string::npos)
   {
      strings.push_back(str.substr(prev, pos - prev));
      prev = pos + 1;
   }

   // To get the last substring (or only, if delimiter is not found)
   strings.push_back(str.substr(prev));

   return strings;
}

void cpp_output:: gen_template_calls()
{
   const scxml_parser::state_list &states = sc.sc().states;

   for (scxml_parser::state_list::const_iterator s = states.begin(); s != states.end(); ++s)
   {
      scxml_parser::state state = *s->get();

      out << "// --------------------------------------------------------------------------" << endl;
      out << "template<> inline void " << classname() << "::state_actions<" << classname() << "::" << "state_" << state.id << ">::enter( " << classname() << "::data_model &m )" << endl;
      out << "// --------------------------------------------------------------------------" << endl;
      out << "{" << endl;
      out << tab << "m.actionHandler->state" << UpperFirstLetter(state.id) << "Enter( m );" << endl;
      out << "}" << endl;
      out << endl;
      out << "// --------------------------------------------------------------------------" << endl;
      out << "template<> inline void " << classname() << "::state_actions<" << classname() << "::" << "state_" << state.id << ">::exit( " << classname() << "::data_model &m )" << endl;
      out << "// --------------------------------------------------------------------------" << endl;
      out << "{" << endl;
      out << tab << "m.actionHandler->state" << UpperFirstLetter(state.id) << "Exit( m );" << endl;
      out << "}" << endl;
      out << endl;

      for ( scxml_parser::transition_list::const_iterator it = state.transitions.begin(); it != state.transitions.end(); ++it )
      {
         std::string execContent = it->get()->executable_content;
         if ( execContent != "" )
         {
            std::string method;
            std::string params;

            std::vector<std::string> exe = split_string(it->get()->executable_content, "\n");


            
            out << "// --------------------------------------------------------------------------" << endl;
            out << "template<> inline void " << classname() << "::transition_actions<" << it->get()->index << ", &" << classname() << "::" << it->get()->event_fmt << ", " << classname() << "::" << it->get()->source_fmt << ", " <<
               classname() << "::" << it->get()->dest_fmt << ">::enter(" << classname() << "::data_model& m )" << endl;
            out << "// --------------------------------------------------------------------------" << endl;
            out << "{" << endl;
            for (std::string m : exe) {
               parseExecutableMethodCall(m, method, params);
               out << tab << "m.actionHandler->on" << UpperFirstLetter(method) << "( m";
               if (params == "") {
                  out << " );" << endl;
               }
               else {
                  out << ", " << params << " );" << endl;
               }
            }
            out << "}" << endl;
            out << endl;
         }
      }
   }
}

std::string cpp_output::UpperFirstLetter(std::string word)
{
   string::iterator it( word.begin() );
   if ( it != word.end() )
      word[0] = toupper( (unsigned char)word[0] );
   return word;
}
