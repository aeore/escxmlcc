/*************************************************************************
 ** Copyright (C) 2013 Jan Pedersen <jp@jp-embedded.com>
 ** Copyright (C) 2014 Yuriy Gurin <ygurin@outlook.com>
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
 *      + datamodel            26.07.14 ygurin
 *      + selftransitions      27.07.14 ygurin
 *      + conditions parsing   03.08.14 ygurin
 *      + includes             04.08.14 ygurin
 */

#include "scxml_parser.h"
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/erase.hpp>
#include <boost/algorithm/string.hpp>
#include <functional> 
#include <algorithm> 
#include <cctype>
#include <locale>

using namespace boost::property_tree;
using namespace std;

namespace ltrim{
// trim from start
static inline std::string &ltrim( std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
}

// trim from end
static inline std::string &rtrim( std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
}

// trim from both ends
static inline std::string &trim( std::string &s) {
        return ltrim(rtrim(s));
}
};

// --------------------------------------------------------------------------
scxml_parser::scxml_parser( const char *name, const ptree &pt ) 
: using_parallel( false )
// --------------------------------------------------------------------------
{
   m_scxml.name = name;
   parse( pt );
}

// --------------------------------------------------------------------------
void scxml_parser::parse( const ptree &pt )
// --------------------------------------------------------------------------
{
	for ( ptree::const_iterator it = pt.begin(); it != pt.end(); ++it ) {
      if ( it->first == "<xmlcomment>" ) {
         /* ignore comments */
      }
      else if ( it->first == "scxml" ) {
         parse_scxml( it->second );
      }
      else cerr << "warning: unknown scxml item '" << it->first << "'" << endl;
   }
}

void scxml_parser::parse_scxml( const ptree &pt )
{
	try 
   {
		using namespace boost::algorithm;
		const ptree &xmlattr = pt.get_child("<xmlattr>");
		boost::optional<string> initial(xmlattr.get_optional<string>("initial"));
		if ( initial ) split( m_scxml.initial, *initial, is_any_of(" "), token_compress_on );
		//if ( m_scxml.initial.size() > 1 ) parallel_target_sizes.insert( m_scxml.initial.size() );
		m_scxml.name = xmlattr.get<string>("name", m_scxml.name);

		for (ptree::const_iterator it = pt.begin(); it != pt.end(); ++it) {
			if (it->first == "<xmlcomment>") ; // ignore comments
			else if (it->first == "<xmlattr>") ; // ignore, parsed above
			else if (it->first == "state") parse_state(it->second, boost::shared_ptr<state>());
			else if (it->first == "history") parse_state(it->second, boost::shared_ptr<state>());
			else if (it->first == "parallel") parse_parallel(it->second, boost::shared_ptr<state>());
			else if (it->first == "initial") m_scxml.initial = parse_initial(it->second);
         else if (it->first == "datamodel") parse_datamodel( it->second );
         else if (it->first == "header") parse_header( it->second );
         else if( it->first == "handlers" ) parse_handlers( it->second );
         else cerr << "warning: unknown item '" << it->first << "' in <scxml>" << endl;
		}

		// if initial state is not set, use first state in document order
		if(m_scxml.initial.empty()) {
			if(m_scxml.states.size()) {
				m_scxml.initial.push_back((*m_scxml.states.begin())->id);
			}
			else {
				cerr << "error: could not set initial state" << endl;
				exit(1);
			}
		}
	}
	catch (ptree_error e) {
		cerr << "error: scxml: " << e.what() << endl;
		exit(1);
	}
}

void scxml_parser::parse_handlers( const boost::property_tree::ptree &pt )
{
   for ( ptree::const_iterator it = pt.begin(); it != pt.end(); ++it ) {
      if (it->first == "method")  {
          const ptree& xmlattr = it->second.get_child( "<xmlattr>" );
          std::string method_name = xmlattr.get<string>( "name" );

          transition_action t_action;
          t_action.name = method_name;
          for ( ptree::const_iterator it_arg = it->second.begin(); it_arg != it->second.end(); ++it_arg ) {
             if ( it_arg->first == "<xmlcomment>" ) ; // ignore comments
             else if ( it_arg->first == "<xmlattr>" ) ; // ignore, parsed above
             else if ( it_arg->first == "arg" ) {
                const ptree& arg_attr = it_arg->second.get_child("<xmlattr>");
                std::string name = arg_attr.get<string>("name");
                std::string type = arg_attr.get<string>("type");

                param p = { name, type };
                t_action.params.push_back( p );
             } else {
                cerr << "error: unknown arg type" << endl;
                exit( 1 );
             }
          }
          m_scxml.transition_actions.push_back( t_action );
      } else {
         cerr << "error: unknown handler type" << endl;
         exit( 1 );
      }
   }
}

void scxml_parser::parse_datamodel( const boost::property_tree::ptree &pt )
{
   string datamodel = pt.data();

   boost::trim( datamodel );
   boost::erase_all( datamodel, "\n" );

   if ( !datamodel.empty() )
   {
      if ( *datamodel.rbegin() == ';' )
      {
         datamodel.pop_back();
      }

      slist sptb;
      boost::algorithm::split( sptb, datamodel, boost::algorithm::is_any_of(";") );

      BOOST_FOREACH( std::string s, sptb )
      {
         slist parts;
         boost::algorithm::split( parts, s, boost::algorithm::is_any_of(" ") );

         m_scxml.datamodel.push_back( std::make_pair(parts[0], parts[1]) );
      }
   }
}

void scxml_parser::parse_header( const boost::property_tree::ptree &pt )
{
   string headerContext = pt.data();
   m_scxml.header = headerContext;
}

void scxml_parser::parse_parallel(const ptree &pt, const boost::shared_ptr<state> &parent)
{
   cerr << "error: parallels do not supported" << endl;
   exit(1);
}

scxml_parser::slist scxml_parser::parse_initial(const ptree &pt)
{
	// todo initial transitian may have actions
	scxml_parser::slist initial;

	try {

		for (ptree::const_iterator it = pt.begin(); it != pt.end(); ++it) {
			if (it->first == "<xmlcomment>") ; // ignore comments
			else if (it->first == "transition") initial = parse_transition(it->second, "")->target;
		}

	}
	catch (ptree_error e) {
		cerr << "error: initial: " << e.what() << endl;
		exit(1);
	}

	return initial;
}

void scxml_parser::parse_state(const ptree &pt, const boost::shared_ptr<state> &parent)
{
	try {
		using namespace boost::algorithm;
		const ptree &xmlattr = pt.get_child("<xmlattr>");
		boost::shared_ptr<state> st = boost::make_shared<state>();
		st->id = xmlattr.get<string>("id");
		if ( parent ) st->parent = parent;
		boost::optional<string> initial(xmlattr.get_optional<string>("initial"));
		if ( initial ) split(st->initial, *initial, is_any_of(" "), token_compress_on);
		//if(st->initial.size() > 1) parallel_target_sizes.insert(st->initial.size());
		st->type = xmlattr.get_optional<string>("type");
		m_scxml.states.push_back(st);
		state_list::iterator state_i = --m_scxml.states.end();

		for (ptree::const_iterator it = pt.begin(); it != pt.end(); ++it) {
			if (it->first == "<xmlcomment>") ; // ignore comments
			else if (it->first == "<xmlattr>") ; // ignore, parsed above
			else if (it->first == "state") parse_state(it->second, st);
			else if (it->first == "history") parse_state(it->second, st);
			//else if (it->first == "parallel") parse_parallel(it->second, st);
			else if (it->first == "transition") state_i->get()->transitions.push_back(parse_transition(it->second, st->id));
			else if (it->first == "onentry") state_i->get()->entry_actions = parse_entry(it->second);
			else if (it->first == "onexit") state_i->get()->exit_actions = parse_entry(it->second);
			else if (it->first == "initial") state_i->get()->initial = parse_initial(it->second);
			else cerr << "warning: unknown item '" << it->first << "' in <state>" << endl;
		}

		// if initial state is not set, use first state in document order
		// if parent is parallel put all states in initial
		if(parent && (parent->initial.empty() || (parent->type && *parent->type == "parallel"))) {
			parent->initial.push_back(st->id);
		}
	}
	catch (ptree_error e) {
		cerr << "error: state: " << e.what() << endl;
		exit(1);
	}
}

scxml_parser::plist<scxml_parser::action> scxml_parser::parse_entry(const ptree &pt)
{
	plist<action> l_ac;
	try {
		for (ptree::const_iterator it = pt.begin(); it != pt.end(); ++it) {
			if (it->first == "<xmlcomment>") ; // ignore comments
			else if (it->first == "script") l_ac.push_back(parse_script(it->second));
			else if (it->first == "log") l_ac.push_back(parse_log(it->second));
			else cerr << "warning: unknown item '" << it->first << "' in <onentry> or <onexit>" << endl;
		}
	}
	catch (ptree_error e) {
		cerr << "error: onentry/onexit: " << e.what() << endl;
		exit(1);
	}
	return l_ac;
}

boost::shared_ptr<scxml_parser::action> scxml_parser::parse_log(const ptree &pt)
{
	boost::shared_ptr<action> ac = boost::make_shared<action>();
	try {
		const ptree &xmlattr = pt.get_child("<xmlattr>");

		const string label = xmlattr.get<string>("label");
		const string expr = xmlattr.get<string>("expr");
		ac->expr = "std::clog << \"[" + label + "] \" << " + expr + " << std::endl;";

		for (ptree::const_iterator it = pt.begin(); it != pt.end(); ++it) {
			if (it->first == "<xmlcomment>") ; // ignore comments
			else if (it->first == "<xmlattr>") ; // ignore, parsed above
			else cerr << "warning: unknown item '" << it->first << "' in <log>" << endl;
		}
	}
	catch (ptree_error e) {
		cerr << "error: log: " << e.what() << endl;
		exit(1);
	}
	return ac;
}

boost::shared_ptr<scxml_parser::action> scxml_parser::parse_script(const ptree &pt)
{
	boost::shared_ptr<action> ac = boost::make_shared<action>();
	try {
		ac->expr = pt.get_value<string>();

		for (ptree::const_iterator it = pt.begin(); it != pt.end(); ++it) {
			if (it->first == "<xmlcomment>") ; // ignore comments
			else cerr << "warning: unknown item '" << it->first << "' in <script>" << endl;
		}
	}
	catch (ptree_error e) {
		cerr << "error: action: " << e.what() << endl;
		exit(1);
	}
	return ac;
}

boost::shared_ptr<scxml_parser::transition> scxml_parser::parse_transition(const ptree &pt, const std::string state)
{
	const ptree &xmlattr = pt.get_child("<xmlattr>");
	boost::shared_ptr<transition> tr = boost::make_shared<transition>();
	try
   {
		using namespace boost::algorithm;
		boost::optional<string> target(xmlattr.get_optional<string>("target"));
      if ( !target.is_initialized() || target.get() == "" ) target = state;
		if ( target ) split(tr->target, *target, is_any_of(" "), token_compress_on);
		//if ( tr->target.size() > 1 ) parallel_target_sizes.insert( tr->target.size() );
		tr->event = xmlattr.get_optional<string>( "event" );

		for ( ptree::const_iterator it = pt.begin(); it != pt.end(); ++it ) {
			if ( it->first == "<xmlcomment>" ) ; // ignore comments
			else if ( it->first == "<xmlattr>" )
         {
            tr->condition = it->second.get<std::string>( "cond", "" );
         }
			else if (it->first == "script") tr->actions.push_back(parse_script(it->second));
			else if (it->first == "log") tr->actions.push_back(parse_log(it->second));
			else cerr << "warning: unknown item '" << it->first << "' in <transition>" << endl;
		}

      string s = pt.data();
      std::string content = ltrim::trim(s);
      if (content != "")
      {
         tr->executable_content = content;
      }
	}
	catch (ptree_error e) {
		cerr << "error: transition: " << e.what() << endl;
		exit(1);
	}
	return tr;
}

