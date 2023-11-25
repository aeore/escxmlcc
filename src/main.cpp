/*************************************************************************
 ** Copyright (C) 2013 Jan Pedersen <jp@jp-embedded.com>
 ** Copyright (C) 2015 Yuriy Gurin <ygurin@outlook.com>
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

#include <iostream>
#include "scxml_parser.h"
#include "cpp_output.h"
#include "options.h"
#include "version.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <algorithm>
#include <cctype>

//using namespace boost::property_tree;
namespace fs = boost::filesystem;

bool c_pred( char c ) {
   return !isalnum(c);
}

void scxmlcc( const options &opt ) {
   boost::property_tree::ptree pt;
   read_xml( opt.input.string(), pt );

   std::string sc_name = opt.input.stem().string();
   std::replace_if( sc_name.begin(), sc_name.end(), c_pred, '_' );

   scxml_parser sc( sc_name.c_str(), pt );
   std::ofstream ofs( opt.output.c_str() );
   if (ofs) {
      cpp_output out( ofs, sc, opt );
      out.gen();
   } else {
      throw std::runtime_error( std::string("Can't write generated results using path '") + opt.output.string() + "'. Specified location can't be opened" );
   }
}

int main(int argc, char *argv[]) {
   using namespace boost::program_options;

   options_description desc( "Options" );
   desc.add_options()
      ( "help,h", "This help message" )
      ( "input,i", value<std::string>(), "Input file" )
      ( "output,o", value<std::string>(), "Output directory" )
      ( "version,v", "Version information" );

   positional_options_description pdesc;
   pdesc.add( "input", -1 );

   variables_map vm;
   try {
      store( parse_command_line(argc, argv, desc), vm );
      store( command_line_parser(argc, argv).options(desc).positional(pdesc).run(), vm );
      notify( vm );
   } catch ( error& e ) {
      std::cerr << "Error: " << e.what() << std::endl;
      return -1;
   }

   options opt;
   if ( vm.count("input") ) {
      opt.input = vm["input"].as<std::string>();
   }

   if ( vm.count("output") ) {
      opt.output = vm["output"].as<std::string>();
      opt.output = opt.output / boost::filesystem::path( opt.input ).stem().replace_extension(".hpp");
   }

   if ( !opt.input.empty() && opt.output.empty() ) {
      opt.output = opt.input;
      opt.output.replace_extension( ".hpp" );
   }

   if ( vm.count("version") ) {
      std::cout << "scxmlcc version: " << version() << std::endl;
      std::cout << std::endl << "  <!> This is not an original scxmlcc FSM compiler <!>" << std::endl << std::endl;
      std::cout << "For more information about original version, see http://scxmlcc.org" << std::endl;
      std::cout << std::endl; 
      std::cout << "  Copyright (C) 2013 Jan Pedersen <jp@jp-embedded.com>" << std::endl;
      std::cout << "  Copyright (C) 2014 Yuriy Gurin <ygurin@outlook.com>" << std::endl;
      std::cout << std::endl; 
      std::cout << "  This program is free software: you can redistribute it and/or modify" << std::endl;
      std::cout << "  it under the terms of the GNU General Public License as published by" << std::endl;
      std::cout << "  the Free Software Foundation, either version 3 of the License, or" << std::endl;
      std::cout << "  (at your option) any later version." << std::endl;
      std::cout << std::endl;  
      std::cout << "  This program is distributed in the hope that it will be useful," << std::endl;
      std::cout << "  but WITHOUT ANY WARRANTY; without even the implied warranty of" << std::endl;
      std::cout << "  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" << std::endl;
      std::cout << "  GNU General Public License for more details." << std::endl;
      std::cout << std::endl; 
      std::cout << "  You should have received a copy of the GNU General Public License" << std::endl;
      std::cout << "  along with this program. If not, see <http://www.gnu.org/licenses/>." << std::endl;
      std::cout << std::endl;  
      return 0;
   }

   if ( vm.count("help") || opt.input.empty() ) {
      std::cout << "Extended scxml FSM compiler, version " << version() << std::endl;
      std::cout << "  Copyright (C) 2013 Jan Pedersen <jp@jp-embedded.com>" << std::endl;
      std::cout << "  Copyright (C) 2014 Yuriy Gurin <ygurin@outlook.com>" << std::endl;
      std::cout << "" << std::endl;
      std::cout << "Usage: " << argv[0] << " [options] [input]" << std::endl;
      std::cout << desc << std::endl;
      return 0;
   }

   try {
      scxmlcc( opt );
   } catch (const boost::property_tree::xml_parser::xml_parser_error& e) {
      std::cerr << "Parsing error: " << e.message() << std::endl;
      return 1;
   } catch ( const std::runtime_error& e ) {
      std::cerr << "Error: " << e.what() << std::endl;
      return 1;
   } catch (...) {
      std::cerr << "Unandled error!" << std::endl;
      return 1;
   }
   return 0;
}
