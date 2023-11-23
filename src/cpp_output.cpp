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
#include <iostream>
#include <vector>
#include <set>
#include <map>

//todo std::set private/public/protected 
 
std::string cpp_output::classname() { 
   return "C" + UpperFirstLetter(sc.sc().name) + "FSM"; 
}

std::string cpp_output::state_t() { 
   return "state"; 
}

std::string cpp_output::state_composite_t() { 
   return "composite"; 
}

std::string cpp_output::state_parallel_t() { 
   return "parallel"; 
}

std::string cpp_output::state_actions_t() { 
   return "state_actions";
}

void cpp_output::gen_transition_base() {
   out << tab << "class no_state { /* dup */ };" << std::endl; 
   out << tab << "enum transition_type { external, internal };" << std::endl; 
   out << std::endl;

   const int max_targets = sc.parallel_target_sizes.size() ? *sc.parallel_target_sizes.rbegin() : 1;
   out << tab << "template<int index, event E, class S";
   for (int i = 0; i < max_targets; ++i) {
      out << ", class D" << i << " = no_state";
   }
   out << "> class transition_actions {" << std::endl;
   out << tab << "protected:" << std::endl;
   out << tab << tab << "void enter(data_model&) { /* default enter action */ }" << std::endl;
   out << tab << "};" << std::endl;
   out << std::endl;

   // external transition
   out << tab << "// external transition" << std::endl;
   out << tab << "template<int index, event E, class S, class D = no_state, transition_type T = external> class transition: public transition_actions<index, E, S, D> {" << std::endl;
   out << tab << "public:" << std::endl;
   // exit/enter is called here without parameter, which forces the action to always exit/enter at least current state
   out << tab << tab << "state* operator()( S *s, D &d, " << classname() << " &sc ) {" << std::endl;
   if ( sc.using_parallel ) {
      out << tab << tab << tab << "s->exit_parallel( sc, s, &d );" << std::endl;
   }
   out << tab << tab << tab << "s->exit( sc.model, typeid(S) );" << std::endl;
   out << tab << tab << tab << "s->template exit<D>( sc.model );" << std::endl;
   out << tab << tab << tab << "transition_actions<index, E, S, D>::enter( sc.model );" << std::endl;
   out << tab << tab << tab << "d.template enter<S>( sc.model );" << std::endl;
   if ( sc.using_parallel ) {
      out << tab << tab << tab << "return d.template enter_parallel<S>( sc, &d, s );" << std::endl;
   } else {
      out << tab << tab << tab << "return &d;" << std::endl;
   }
   out << tab << tab << "}" << std::endl;
   out << tab << "};" << std::endl;
   out << std::endl;

   // todo veryfy how these work at down transition
   out << tab << "// internal transition" << std::endl;
   out << tab << "template<int index, event E, class S, class D> class transition<index, E, S, D, internal>: public transition_actions<index, E, S, D> {" << std::endl;
   out << tab << "public:" << std::endl;
   out << tab << tab << "state* operator()( S *s, D &d, " << classname() << " &sc ) {" << std::endl; 
   if (sc.using_parallel) { 
      out << tab << tab << tab << "s->exit_parallel( sc, s, &d );" << std::endl;
   }
   out << tab << tab << tab << "s->exit( sc.model, typeid(S) );" << std::endl;
   out << tab << tab << tab << "s->template exit<D>( sc.model, (D*)0 );" << std::endl;
   out << tab << tab << tab << "transition_actions<index, E, S, D>::enter( sc.model );" << std::endl;
   out << tab << tab << tab << "d.template enter<S>( sc.model, (S*)0 );" << std::endl;
   if (sc.using_parallel) {
      out << tab << tab << tab << "return d.template enter_parallel<S>( sc, &d, s );" << std::endl;
   } else {
      out << tab << tab << tab << "return &d;" << std::endl;
   }
   out << tab << tab << "}" << std::endl;
   out << tab << "};" << std::endl;
   out << std::endl;

   // transition without target
   out << tab << "// transition with no target" << std::endl;
   out << tab << "template<int index, event E, class S> class transition<index, E, S, no_state>: public transition_actions<index, E, S, no_state> {" << std::endl;
   out << tab << "public:" << std::endl;
   out << tab << tab << "S* operator()( S *s, " << classname() << " &sc ) {" << std::endl;
   out << tab << tab << tab << "transition_actions<index, E, S, no_state>::enter( sc.model );" << std::endl;
   out << tab << tab << tab << "return s;" << std::endl;
   out << tab << tab << "}" << std::endl;
   out << tab << "};" << std::endl;
   out << std::endl;

   for (std::set<int>::const_iterator i = sc.parallel_target_sizes.begin(); i != sc.parallel_target_sizes.end(); ++i) {
      const int sz = *i;

      out << tab << "template<event E, class S";
      for (int i = 0; i < sz; ++i) {
         out << ", class D" << i;
      }
      out << ", transition_type T = external> class transition" << sz << ';' << std::endl;

      out << tab << "template<event E, class S";
      for (int i = 0; i < sz; ++i) {
         out << ", class D" << i;
      }
      out << "> class transition" << sz << "<E, S";
      for (int i = 0; i < sz; ++i) {
         out << ", D" << i;
      }
      out << ", internal> : public transition_actions<E, S, D0, D1>" << std::endl;

      out << tab << '{' << std::endl;
      out << tab << tab << "public:" << std::endl;

      //todo: for now, all targets must have same parallel parent
      out << tab << tab << "state* operator ()(S *s";
      for (int i = 0; i < sz; ++i) {
         out << ", D" << i << " &d" << i;
      }
      out << ", sc_test576 &sc)" << std::endl;

      out << tab << tab << '{' << std::endl;

      out << tab << tab << tab << "if (!transition_actions<E, S";
      for (int i = 0; i < sz; ++i) {
         out << ", D" << i;
      }
      out << ">::condition(sc.model)) return 0;" << std::endl;

      out << tab << tab << tab << "s->exit_parallel(sc, s, &d0);" << std::endl;
      out << tab << tab << tab << "s->exit(sc.model, typeid(S));" << std::endl;
      out << tab << tab << tab << "s->template exit<D0>(sc.model, (D0*)0);" << std::endl;

      out << tab << tab << tab << "transition_actions<E, S";
      for (int i = 0; i < sz; ++i) {
         out << ", D" << i;
      }
      out << ">::enter( sc.model );" << std::endl;

      out << tab << tab << tab << "d0.template enter<S>( sc.model, (S*)0 );" << std::endl;
      out << tab << tab << tab << "return d0.template enter_parallel<S>( sc, &d0, s";
      for (int i = 1; i < sz; ++i) {
         out << ", d" << i;
      }
      out << " );" << std::endl;

      out << tab << tab << '}' << std::endl;
      out << tab << "};" << std::endl;

      out << std::endl;
   }
}

void cpp_output::gen_state_actions_base() {
   out << "private:" << std::endl;
   out << tab << "template<class C> class " << state_actions_t() << " {" << std::endl;
   // enter/exit
   out << tab << "protected:" << std::endl;
   out << tab << tab << "void inline enter( data_model& ) { /* default enter action */ }" << std::endl;
   out << tab << tab << "void inline exit( data_model& ) { /* default exit action */ }" << std::endl;
   out << tab << "};" << std::endl;
   out << std::endl;
}

void cpp_output::gen_state_composite_base() {
   out << tab << "template<class C, class P> class " << state_composite_t() << ": public P, public state_actions<C> {" << std::endl;

   out << tab << tab << "virtual " << state_t() << "* initial( " << classname() << "& ) { return 0; }" << std::endl;
   out << std::endl;

   // lca calculation
   out << tab << "public:" << std::endl;
   out << tab << tab << "// LCA calculation" << std::endl;
   out << tab << tab << "template<class T> void enter( data_model&, " << state_composite_t() << "* ) {}" << std::endl;

   out << tab << tab << "template<class T> void enter( data_model &m, ... ) { P::template enter<T>( m, (T*)0 );";
   out << " state_actions<C>::enter( m ); }" << std::endl;

   out << tab << tab << "template<class T> void exit( data_model&, " << state_composite_t() << "* ) {}" << std::endl;

   out << tab << tab << "template<class T> void exit( data_model &m, ... ) {";
   out << " state_actions<C>::exit( m ); P::template exit<T>( m, (T*)0 ); }" << std::endl;

   out << tab << tab << "virtual void exit( data_model &m, const std::type_info &sti ) { if ( typeid(C) == sti ) return;";
   out << " state_actions<C>::exit( m ); P::exit( m, sti ); }" << std::endl;

   out << tab << "};" << std::endl;
   out << std::endl;
}

void cpp_output::gen_state_parallel_base() {
   if ( !sc.using_parallel ) {
      return;
   }

   if ( sc.parallel_sizes.size() == 0 ) {
      //todo make composite, if children < 2
      std::cerr << "error: parallel state with < 2 states is currently not supported" << std::endl;
      exit(1);
   }

   const int min_c = *sc.parallel_sizes.begin();
   const int max_c = *sc.parallel_sizes.rbegin();
   if ( min_c < 2 ) {
      //todo make composite, if children < 2
      std::cerr << "error: parallel state with < 2 states is currently not supported" << std::endl;
      exit(1);
   }

   //todo combine with no_class
   if (min_c < max_c) out << tab << "class no_class {};" << std::endl;

   for (std::set<int>::reverse_iterator i = sc.parallel_sizes.rbegin(); i != sc.parallel_sizes.rend(); ++i) {
      const int children = *i;
      out << tab << "template<class C, class P";
      for (int c = 0; c < children; ++c) {
         out << ", class C" << c;
         if ((children == max_c) && (c >= min_c)) out << " = no_class";
      }
      out << "> class " << state_parallel_t();
      if (children < max_c) {
         out << " <C, P";
         for (int c = 0; c < children; ++c) out << ", C" << c;
         out << '>';
      }
      out << " : public composite<C, P>" << std::endl;
      out << tab << '{' << std::endl;
      out << tab << tab << "public:" << std::endl;
      for (int c = 0; c < children; ++c) {
         out << tab << tab << "template<class S> " << state_t() << "* enter_parallel(" << classname() << " &sc, C" << c << "*, C*) { return this; }" << std::endl;
         out << tab << tab << "template<class S> " << state_t() << "* enter_parallel(" << classname() << " &sc, C" << c << " *d, " << state_t() << "*)" << std::endl;
         out << tab << tab << '{' << std::endl;
         out << tab << tab << tab << "// parallel state entered with C" << c << " or child of as target" << std::endl;
         out << tab << tab << tab << "P::template enter_parallel<S>(sc, d, (S*)0);" << std::endl;
         for (int n = 0; n < children; ++n) {
            if (n == c) continue;
            out << tab << tab << tab << "sc.cur_state.push_back(d->init_child(sc, (C" << n << "*)0));" << std::endl;
         }
         out << tab << tab << tab << "return this;" << std::endl;
         out << tab << tab << "};" << std::endl;
         out << tab << tab << std::endl;

         //todo to continue
      }

      out << tab << tab << "template<class S> " << state_t() << "* enter_parallel(" << classname() << " &sc, C*, C*) { return this; }" << std::endl;
      out << tab << tab << "template<class S> " << state_t() << "* enter_parallel(" << classname() << " &sc, C *d, " << state_t() << "*)" << std::endl;
      out << tab << tab << '{' << std::endl;
      out << tab << tab << tab << "// parallel state entered with parallel as target" << std::endl;
      out << tab << tab << tab << "P::template enter_parallel<S>(sc, d, (S*)0);" << std::endl;
      for (int n = 0; n < children - 1; ++n) {
         out << tab << tab << tab << "sc.cur_state.push_back(d->init_child(sc, (C" << n << "*)0));" << std::endl;
      }
      out << tab << tab << tab << "return d->init_child(sc, (C" << children - 1 << "*)0);" << std::endl;
      out << tab << tab << "};" << std::endl;
      out << std::endl;

      // handle transition with all children given
      // todo, only add if needed
      out << tab << tab << "template<class S";
      for (int c = 1; c < children; ++c) {
         out << ", class D" << c;
      }
      out << "> " << state_t() << "* enter_parallel(" << classname() << " &sc, C*, C*";
      for (int c = 1; c < children; ++c) {
         out << ", D" << c << "&";
      }
      out << ") { return this; }" << std::endl;
      out << tab << tab << "template<class S";
      for (int c = 1; c < children; ++c) {
         out << ", class D" << c;
      }
      out << "> " << state_t() << "* enter_parallel(" << classname() << " &sc, C *d, " << state_t() << '*';
      for (int c = 1; c < children; ++c) {
         out << ", D" << c << " &d" << c;
      }
      out << ")" << std::endl;
      out << tab << tab << '{' << std::endl;
      out << tab << tab << tab << "// handle transition with all children given" << std::endl;
      out << tab << tab << tab << "P::template enter_parallel<S>(sc, d, (S*)0);" << std::endl;
      for (int c = 1; c < children; ++c) {
         out << tab << tab << tab << 'd' << c << ".template enter<C>(sc.model, (C*)0), sc.cur_state.push_back(&d" << c << ");" << std::endl;
      }

      // todo call init_child where Cn is not a child of Dn or *d
      // if (false) sc.cur_state.push_back(d->init_child(sc, (C0*)0));
      // if (false) sc.cur_state.push_back(d->init_child(sc, (C1*)0));

      out << tab << tab << tab << "return this;" << std::endl;
      out << tab << tab << '}' << std::endl;
      out << std::endl;

      out << tab << tab << "bool parallel_parent(const std::type_info& pti) { return typeid(C) == pti; }" << std::endl;
      out << tab << tab << "void exit_parallel(" << classname() << " &sc, C*, C*) {}" << std::endl;
      out << tab << tab << "void exit_parallel(" << classname() << " &sc, C *s, state *d)" << std::endl;
      out << tab << tab << '{' << std::endl;
      out << tab << tab << tab << "// parallel state exited from C or child" << std::endl;
      out << tab << tab << tab << "for(" << classname() << "::cur_state_l::iterator i = sc.cur_state.begin(); (i != sc.cur_state.end()) && *i; ++i) {" << std::endl;
      out << tab << tab << tab << tab << "if(this == *i) continue;" << std::endl;
      out << tab << tab << tab << tab << "if(!(*i)->parallel_parent(typeid(C))) continue;" << std::endl;
      out << tab << tab << tab << tab << "(*i)->exit(sc.model, typeid(C));" << std::endl;
      out << tab << tab << tab << tab << "*i = 0;" << std::endl;
      out << tab << tab << tab << '}' << std::endl;
      out << tab << tab << tab << "P::exit_parallel(sc, s, d);" << std::endl;
      out << tab << tab << '}' << std::endl;


      out << tab << "};" << std::endl;
      out << std::endl;
   }
}

void cpp_output::gen_model_base() {
   out << "public:" << std::endl;
   out << tab << "class ActionHandlerInterface;" << std::endl;
   out << tab << "struct data_model {" << std::endl;
   out << tab << tab << "friend class " << classname() << ";" << std::endl;
   const std::vector<std::pair<std::string, std::string> >& dm = sc.sc().datamodel;

   out << tab << tab << "data_model( void )";

   std::string inits = "";
   for ( const auto& p: dm ) {
      if ( !inits.empty() ) {
         inits += ", ";
      }
      inits += p.second + "()";
   }
   if ( !inits.empty() ) {
      out << ": " << inits;
   }
   out << " { /* nop */ }" << std::endl;

   for ( const auto& p: dm ) {
      out << tab << tab << p.first << " " << p.second << ";" << std::endl;
   }

   out << std::endl;
   out << tab << "private:" << std::endl;
   out << tab << tab << "ActionHandlerInterface* actionHandler;" << std::endl;
   out << tab << "};" << std::endl;
   out << std::endl;
   
   gen_ahi_header();

   if ( dm.size() > 0 ) {
      out << std::endl << "public:" << std::endl;
   }
   for( const auto& p: dm ) {
      std::string var_name = p.second;
      var_name[0] = toupper(var_name[0]);
      out << tab << "void set" << var_name << "( const " << p.first + "& value ) {" << std::endl;
      out << tab << tab << "model." << p.second << " = value;" << std::endl;
      out << tab << "}" << std::endl;
   }

   out << std::endl;
}

void cpp_output::gen_state_base() {
   const scxml_parser::state_list &states = sc.sc().states;

   // state actions class
   out << "public:" << std::endl;
   out << tab << "class " << state_t() << " {" << std::endl;
   out << tab << "public:" << std::endl;

   // events
   // pass through std::set, to sort out dublicates
   std::set<std::string> event_set;
   for ( scxml_parser::state_list::const_iterator s = states.begin(); s != states.end(); ++s ) {
      for ( scxml_parser::transition_list::const_iterator i = s->get()->transitions.begin(); i != s->get()->transitions.end(); ++i ) {
         if ( i->get()->event) event_set.insert(*i->get()->event );
      }
   }
   for ( std::set<std::string>::const_iterator i = event_set.begin(); i != event_set.end(); ++i ) {
      std::string trig_event_name = *i;
      bool been = std::find_if(trig_event_name.begin(), trig_event_name.end(), [](char c)->bool{ return c == '@'; }) != trig_event_name.end();
      trig_event_name.erase(std::remove(trig_event_name.begin(), trig_event_name.end(), '@'), trig_event_name.end());
      out << tab << tab << "virtual " << state_t() << "* trigger_" << trig_event_name << "( " << classname() << "& ) { return 0; }" << std::endl;
      if ( been ) {
         out << tab << tab << "virtual " << state_t() << "* event_" << trig_event_name << "( " << classname() << "& ) { return 0; }" << std::endl;
      }
   }
   out << tab << tab << "virtual " << state_t() << "* unconditional( " << classname() << "& ) { return 0; }" << std::endl;
   out << tab << tab << "virtual " << state_t() << "* initial( " << classname() << "& ) { return 0; }" << std::endl;

   out << std::endl;

   out << tab << tab << "template<class T> void enter( data_model&, ... ) {}" << std::endl;
   out << tab << tab << "template<class T> void exit( data_model&, ... ) {}" << std::endl;
   out << tab << tab << "virtual void exit( data_model&, const std::type_info& ) {}" << std::endl;
   if ( sc.using_parallel ) {
      out << tab << tab << "template<class S> " << state_t() << "* enter_parallel( " << classname() << "&, " << state_t() << "*, " << state_t() << "* ) { return this; }" << std::endl;
      out << tab << tab << "virtual void exit_parallel( " << classname() << "&, " << state_t() << "*, " << state_t() << "* ) {}" << std::endl;
      out << tab << tab << "virtual bool parallel_parent( const std::type_info& ) { return false; }" << std::endl;
   }

   out << tab << tab << "virtual ~" << state_t() << "( void ) {}" << std::endl;

   out << tab << "};" << std::endl;
   out << std::endl;
   out << "private:" << std::endl;
   if ( sc.using_parallel ) {
      out << tab << "typedef std::vector<" << state_t() << "*> cur_state_l;" << std::endl;
      out << tab << "cur_state_l cur_state;" << std::endl;
   } else {
      out << tab << state_t() << " *cur_state;" << std::endl;
   }
   out << tab << "data_model model;" << std::endl;
   out << tab << "typedef " << state_t() << "* ( " << state_t() << "::*event )( " << classname() << "& );" << std::endl;
   out << std::endl;
}

scxml_parser::state_list cpp_output::children(const scxml_parser::state &state) {
   scxml_parser::state_list states;
   for ( scxml_parser::state_list::const_iterator i = sc.sc().states.begin(); i != sc.sc().states.end(); ++i ) {
      if ( (*i)->parent && (*i)->parent->id == state.id ) {
         states.push_back(*i);
      }
   }
   return states;
}

void cpp_output::gen_state(const scxml_parser::state &state) {
   std::string parent, prefix;
   if ( state.type && *state.type == "inter" ) {
      prefix = "inter";
   }

   // inner states
   parent = state.parent ? ("state_" + state.parent->id) : "scxml";
   std::string state_classname = prefix + "state_" + state.id;

   out << tab << "class " << state_classname << ": public ";
   out << state_composite_t();
   out << '<' << state_classname << ", " << parent << "> {";
   out << std::endl;

   // there is an initial transition in this state
   if ( state.initial.size() ) {
      std::string target = "sc.m_state_" + state.initial.front();
      std::string target_classname = "state_" + state.initial.front();
      out << tab << tab << state_t() << "* " << "initial" << "( " << classname() << " &sc ) { return transition<0, &state::initial, " << state_classname << ", " << target_classname << ", internal>()( this, " << target << ", sc ); }" << std::endl;
   }

   // One event can owned many transitions (with different conditions)
   std::map< std::string, std::vector<std::string> > mth;
   bool isUnconditionalTransitions = false;
   bool isAutotransitional = true;

   // t is iterator to [shared_ptr of transition]
   for ( scxml_parser::transition_list::const_iterator t = state.transitions.begin(); t != state.transitions.end(); ++t ) {
      std::string target;
      std::string target_classname = state_classname;
      std::string transitionName;

      if ( t->get()->target.size() ) {
         target = "sc.m_state_" + t->get()->target.front(); //todo handle multiple targets
         target_classname = "state_" + t->get()->target.front(); //todo handle multiple targets
      }
      
      if ( t->get()->event ) {
         std::string trig_event_name = *t->get()->event;
         trig_event_name.erase(std::remove(trig_event_name.begin(), trig_event_name.end(), '@'), trig_event_name.end());
         
         isAutotransitional = false;

         transitionName = "trigger_" + trig_event_name;
      } else {
         isUnconditionalTransitions = true;
         
         transitionName = "unconditional";
      }

      std::string s = t->get()->condition;
      if ( s != "" ) {
         Condition c(s);
         c.fixCondition(sc.sc().datamodel);
         s = "if ( " + c.getCondition() + " ) ";
      } else {
         s = "if ( true ) ";
      }
      
      mTransitionIndex++;
      t->get()->index = mTransitionIndex;
      if ( target.size() ) {
         // normal transition
         mth[transitionName].push_back(s + "return transition<" + std::to_string(mTransitionIndex) + ", &state::" + transitionName + ", " + state_classname + ", " + target_classname + ">()( this, " + target + ", sc );");
         t->get()->event_fmt = "state::" + transitionName;
         t->get()->source_fmt = state_classname;
         t->get()->dest_fmt = target_classname;
      } else {
         // transition without target
         mth[transitionName].push_back(s + "return transition<" + std::to_string(mTransitionIndex) + ", &state::" + transitionName + ", " + state_classname + ">()( this, sc );");
      }
   }
   
   for ( std::map< std::string, std::vector<std::string> >::iterator it = mth.begin(); it != mth.end(); it ++ ) {
      std::vector<std::string>& v = it->second;

      out << tab << tab << "inline " << (isUnconditionalTransitions ? "virtual " : "") << state_t() << "* " << it->first << "( " << classname() << " &sc ) override {" << std::endl;
      for ( std::vector<std::string>::iterator i = v.begin(); i != v.end(); i++ ) {
         out << tab << tab << tab << *i << std::endl;
      }
      if ( v.size() > 0 ) {
         out << tab << tab << tab << "else return 0;" << std::endl;
      }
      out << tab << tab << "}" << std::endl;
   }

   if ( !isAutotransitional && !isUnconditionalTransitions ) {
      out << tab << tab << "virtual state* unconditional( " << classname() << "& ) override { return 0; }" << std::endl;
   }
      
   out << tab << "} m_" << prefix << "state_" << state.id << ";" << std::endl;
}

void cpp_output::gen_sc() {
   const scxml_parser::state_list &states = sc.sc().states;

   out << "class " << classname() << " {" << std::endl;

   gen_model_base();
   gen_state_base();
   gen_state_actions_base();
   gen_state_composite_base();
   gen_state_parallel_base();
   gen_transition_base();

   // dispatch
   if (sc.using_parallel) {
      out << tab << "private: bool dispatch_event(event e)" << std::endl;
      out << tab << '{' << std::endl;
      out << tab << tab << "bool cont = false;" << std::endl;
      out << tab << tab << "state *next_state;" << std::endl;
      out << tab << tab << "for(unsigned i = 0; i < cur_state.size();) if(cur_state[i]) {" << std::endl;
      out << tab << tab << tab << "if ((next_state = (cur_state[i]->*e)(*this))) cur_state[i] = next_state, cont = true;" << std::endl;
      out << tab << tab << tab << "++i;" << std::endl;
      out << tab << tab << '}' << std::endl;
      out << tab << tab << "else cur_state.erase(cur_state.begin() + i);" << std::endl;
      out << tab << tab << "return cont;" << std::endl;
      out << tab << '}' << std::endl;
   } else {
      out << "private:" << std::endl;
      out << tab << "bool dispatch_event( const event e ) {" << std::endl;
      out << tab << tab << "state *next_state;" << std::endl;
      out << tab << tab << "if ( (next_state = (cur_state->*e)(*this)) ) cur_state = next_state;" << std::endl;
      out << tab << tab << "return next_state;" << std::endl;
      out << tab << '}' << std::endl;
   }
   out << std::endl;
   out << "public:" << std::endl;
   out << tab << "void dispatch( const event e ) {" << std::endl;
   out << tab << tab << "bool cont = dispatch_event( e );" << std::endl;
   out << tab << tab << "while ( cont ) {" << std::endl;
   out << tab << tab << tab << "if ( (cont = dispatch_event(&state::initial)) );" << std::endl;
   out << tab << tab << tab << "else if ( (cont = dispatch_event(&state::unconditional)) );" << std::endl;
   out << tab << tab << tab << "else break;" << std::endl;
   out << tab << tab << "}" << std::endl;
   out << tab << "}" << std::endl;
   out << std::endl;

   // constructor
   out << tab << classname() << "( " << "ActionHandlerInterface* pActionHandler" << " )";
   if ( !sc.using_parallel ) {
      out << ": cur_state( &m_scxml ), model() {";
   }
   out << std::endl;

   if(sc.using_parallel) out << tab << tab << "cur_state.push_back(&m_scxml);" << std::endl;
   out << tab << tab << "model.actionHandler = pActionHandler;" << std::endl;
   out << tab << "}" << std::endl;
   out << std::endl;

   out << tab << "void init( void ) {" << std::endl;
   out << tab << tab << "dispatch( &state::initial );" << std::endl;
   out << tab << "}" << std::endl;
   out << std::endl;

   out << "private: " << std::endl;
   //m_scxml
   out << tab << "class scxml : public composite<scxml, state> {" << std::endl;

   const int sz = sc.sc().initial.size();
   out << tab << tab << "inline " << state_t() << "* initial( " << classname() << "& sc ) override {" << std::endl;
   out << tab << tab << tab << "return transition";
   if(sz > 1) out << sz;
   out << "<0, &state::initial, scxml";
   for (int i = 0; i < sz; ++i) {
      out << ", state_" << sc.sc().initial[i];
   }
   out << ", internal>()( this";
   for (int i = 0; i < sz; ++i) {
      out << ", sc.m_state_" << sc.sc().initial[i];
   }
   out << ", sc );" << std::endl;
   out << tab << tab << "}" << std::endl;

   out << tab << "} m_scxml;" << std::endl;
   out << std::endl;

   // states
   for (auto& state: states) {
      gen_state( *state.get() );
      out << std::endl;
   }

   out << "};" << std::endl << std::endl;
}

void cpp_output::trim() {
   const scxml_parser::state_list &states = sc.sc().states;

   // replace '-' with '_' in event names
   // replace '*' with 'A' in event names, todo: not currently supported
   // replace '.' with '_' in event names todo: see '3.12.1 event descriptors how to handle event tokens
   for (scxml_parser::state_list::const_iterator s = states.begin(); s != states.end(); ++s) {
      for (scxml_parser::transition_list::const_iterator i = s->get()->transitions.begin(); i != s->get()->transitions.end(); ++i) {
         if (i->get()->event) {
            if (i->get()->event->find('.') != std::string::npos) { 
               std::cerr << "warning: event tokens not currently supported" << std::endl;
            }
            if (i->get()->event->find('*') != std::string::npos) {
               std::cerr << "warning: event asteriks not currently supported" << std::endl;
            }
            replace(i->get()->event->begin(), i->get()->event->end(), '.', '_');
            replace(i->get()->event->begin(), i->get()->event->end(), '-', '_');
            replace(i->get()->event->begin(), i->get()->event->end(), '*', 'A');
         }
      }
   }
}

std::string str_toupper(const std::string& s)
{
   std::string up = s;
   std::transform(up.begin(), up.end(), up.begin(), [](unsigned char c) { return std::toupper(c); });
   return up;
}

void cpp_output::gen() {
   if (sc.using_parallel) {
      std::cerr << "warning: parallel support is not fully implemented/tested" << std::endl;
   }
   
   const std::string guardDefine = "AUTOFSM_" + str_toupper(sc.sc().name) + "_HPP";

   out << "#ifndef " << guardDefine << std::endl;
   out << "#define " << guardDefine << std::endl;
   out << std::endl;

   out << "/*" << std::endl;
   out << " *                                     escxmlcc product" << std::endl;
   out << " *                                           fsm" << std::endl;
   out << " *" << std::endl;
   out << " * @ingroup Autogenerated::FSM" << std::endl;
   out << " * @author  escxmlcc" << std::endl;
   out << " * " << std::endl;
   out << " * @brief   This program was generated by escxmlcc fsm compiler written by Yuriy Gurin. This is" << std::endl;
   out << " *          an extended version of original scxmlcc compiler written by Jan Pedersen. For more" << std::endl;
   out << " *          information see project website https://github.com/aeore/escxmlcc" << std::endl;
   out << " */" << std::endl;
   out << std::endl;

   out << "#include <typeinfo>" << std::endl;
   out << std::endl;

   out << "// --------------------------------------------------------------------------" << std::endl;
   out << "// User space" << std::endl;
   out << "// --------------------------------------------------------------------------" << std::endl;
   out << std::endl;

   std::string userHeader = sc.sc().header;
   if ( userHeader == "" ) {
      out << "/* no user data */" << std::endl;
   } else {
      out << sc.sc().header;
   }
   out << std::endl;

   out << "// --------------------------------------------------------------------------" << std::endl;
   out << "// Code definition" << std::endl;
   out << "// --------------------------------------------------------------------------" << std::endl;
   out << std::endl;

   trim();
   gen_sc();
   gen_template_calls();
  
   // end of include guard
   out << "/* ************************************************************************ */" << std::endl;
   out << "#endif // " << guardDefine << std::endl;
}

void cpp_output::gen_ahi_header() {
   out << tab << "class ActionHandlerInterface {" << std::endl;
   out << tab << "public:" << std::endl;
   out << tab << tab << "// state handlers" << std::endl;

   const scxml_parser::state_list &states = sc.sc().states;
   for (scxml_parser::state_list::const_iterator s = states.begin(); s != states.end(); ++s) {
      scxml_parser::state state = *s->get();
      std::string stateName = UpperFirstLetter(state.id);

      out << tab << tab << "virtual void state" << stateName << "Enter( " << classname() << "::data_model& ) { /* default */ }" << std::endl;
      out << tab << tab << "virtual void state" << stateName << "Exit( " << classname() << "::data_model& ) { /* default */ }" << std::endl;
   }
   out << std::endl;

   const std::vector<scxml_parser::transition_action>& transitionHandlers = sc.sc().transition_actions;
   if ( transitionHandlers.size() > 0 ) {
      out << tab << tab << "// transition handlers" << std::endl;
      for ( std::vector<scxml_parser::transition_action>::const_iterator it = transitionHandlers.begin(); it != transitionHandlers.end(); ++it ) {
         const scxml_parser::transition_action& t_action = *it;

         std::string params = "";
         for ( std::vector<scxml_parser::param>::const_iterator arg_it = t_action.params.begin(); arg_it != t_action.params.end(); ++arg_it ) {
            scxml_parser::param p = *arg_it;
            if ( params != "" ) params += ", ";
            params += p.type + " " + p.name;
         }
         if ( params != "" ) {
            params = ", " + params;
         }

         out << tab << tab << "virtual void on" << UpperFirstLetter(t_action.name) << "( " << classname() << "::data_model&" << params << " ) {" << std::endl;
         for (std::vector<scxml_parser::param>::const_iterator arg_it = t_action.params.begin(); arg_it != t_action.params.end(); ++arg_it) {
            scxml_parser::param p = *arg_it;
            out << tab << tab << tab << "(void)" + p.name + ";" << std::endl;
         }
         out << tab << tab << tab << "/* default */" << std::endl;
         out << tab << tab << "}" << std::endl;
      }
      out << std::endl;
   }

   out << tab << "protected:" << std::endl;
   out << tab << tab << "virtual ~ActionHandlerInterface( void ) { /* nop */ }" << std::endl;

   out << tab << "};" << std::endl;
}

std::string getMethodPartFromExecutableContent( std::string& content ) {
   std::string result = "";
   for ( std::string::const_iterator it = content.begin(); it != content.end(); ++it ) {
      if ( std::isalnum(*it, std::locale()) || *it == '_' ) {
         result += *it;
      } else {
         break;
      }
   }
   return result;
}

std::string trim(std::string str) {
   str.erase(str.begin(), find_if(str.begin(), str.end(),
      [](char& ch)->bool { return !isspace(ch); }));
   str.erase(find_if(str.rbegin(), str.rend(),
      [](char& ch)->bool { return !isspace(ch); }).base(), str.end());
   return str;
}  

void parseExecutableMethodCall( std::string content, std::string& method, std::string& params ) {
   method = getMethodPartFromExecutableContent( content );

   std::string::size_type first_quote = content.find('(' );
   std::string::size_type last_quote = content.rfind( ')' );

   const std::string subst = content.substr( first_quote + 1, last_quote - first_quote - 1  );
   params = trim( subst );
}

std::vector<std::string> split_string(const std::string& str, const std::string& delimiter) {
   std::vector<std::string> strings;

   std::string::size_type pos = 0;
   std::string::size_type prev = 0;
   while ((pos = str.find(delimiter, prev)) != std::string::npos) {
      strings.push_back(str.substr(prev, pos - prev));
      prev = pos + 1;
   }

   // To get the last substring (or only, if delimiter is not found)
   strings.push_back(str.substr(prev));

   return strings;
}

void cpp_output:: gen_template_calls() {
   const scxml_parser::state_list &states = sc.sc().states;

   for (scxml_parser::state_list::const_iterator s = states.begin(); s != states.end(); ++s) {
      scxml_parser::state state = *s->get();

      out << "template<> inline void " << classname() << "::state_actions<" << classname() << "::" << "state_" << state.id << ">::enter( " << classname() << "::data_model &m ) {" << std::endl;
      out << tab << "m.actionHandler->state" << UpperFirstLetter(state.id) << "Enter( m );" << std::endl;
      out << "}" << std::endl;
      out << std::endl;
      out << "template<> inline void " << classname() << "::state_actions<" << classname() << "::" << "state_" << state.id << ">::exit( " << classname() << "::data_model &m ) {" << std::endl;
      out << tab << "m.actionHandler->state" << UpperFirstLetter(state.id) << "Exit( m );" << std::endl;
      out << "}" << std::endl;
      out << std::endl;

      for ( scxml_parser::transition_list::const_iterator it = state.transitions.begin(); it != state.transitions.end(); ++it ) {
         std::string execContent = it->get()->executable_content;
         if ( execContent != "" ) {
            std::string method;
            std::string params;
            std::vector<std::string> exe = split_string(it->get()->executable_content, "\n");

            out << "template<> inline void " << classname() << "::transition_actions<" << it->get()->index << ", &" << classname() << "::" << it->get()->event_fmt << ", " << classname() << "::" << it->get()->source_fmt << ", " <<
               classname() << "::" << it->get()->dest_fmt << ">::enter(" << classname() << "::data_model& m ) {" << std::endl;
            for (std::string m : exe) {
               parseExecutableMethodCall(m, method, params);
               out << tab << "m.actionHandler->on" << UpperFirstLetter(method) << "( m";
               if (params == "") {
                  out << " );" << std::endl;
               }
               else {
                  out << ", " << params << " );" << std::endl;
               }
            }
            out << "}" << std::endl;
            out << std::endl;
         }
      }
   }
}

std::string cpp_output::UpperFirstLetter(std::string word) {
   std::string::iterator it( word.begin() );
   if ( it != word.end() ) {
      word[0] = toupper( (unsigned char)word[0] );
   }
   return word;
}
