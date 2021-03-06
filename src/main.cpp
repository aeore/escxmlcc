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

#include "scxml_parser.h"
#include "cpp_output.h"
#include "options.h"
#include "version.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <algorithm>
#include <cctype>
#include <boost/filesystem.hpp>
#include <iostream>

using namespace boost::property_tree;
using namespace std;
namespace fs = boost::filesystem;

// --------------------------------------------------------------------------
bool c_pred( char c )
// --------------------------------------------------------------------------
{
	return !isalnum(c);
};

// --------------------------------------------------------------------------
void scxmlcc( const options &opt )
// --------------------------------------------------------------------------
{
   ptree pt;
   read_xml( opt.input.string(), pt );

   string sc_name = opt.input.stem().string();
   replace_if( sc_name.begin(), sc_name.end(), c_pred, '_' );

   scxml_parser sc( sc_name.c_str(), pt );

   ofstream ofs( opt.output.c_str() );
   ofstream ofs_cpp( opt.output_cpp.c_str() );

   if (!ofs){
      cerr << "Error openning " << opt.output.c_str() << endl;
      return ;
   }
   if (!ofs_cpp){
      cerr << "Error openning " << opt.output_cpp.c_str() << endl;
      return ;
   }

   cpp_output out( ofs, ofs_cpp, sc, opt );
   out.gen();
}
// --------------------------------------------------------------------------
int main(int argc, char *argv[])
// --------------------------------------------------------------------------
{
   using namespace boost::program_options;
   
   options_description desc( "Options" );
   desc.add_options()
      ( "help,h", "This help message" )
      ( "input,i", value<string>(),	"Input file" )
      ( "output,o", value<string>(),	"Output directory" )
      ( "version,v", "Version information" );

   positional_options_description pdesc;
   pdesc.add( "input", -1 );
	
   variables_map vm;
   try {
      store( parse_command_line(argc, argv, desc), vm );
      store( command_line_parser(argc, argv).options(desc).positional(pdesc).run(), vm );
      notify( vm );
   }
   catch ( error& e ) {
      cerr << "Error: " << e.what() << endl;
      return -1;
   }

   options opt;

   if ( vm.count("input") ) {
      opt.input = vm["input"].as<string>();
   }

   if ( vm.count("output") ) {
      opt.output = vm["output"].as<string>();
      opt.output = opt.output / boost::filesystem::path( opt.input ).stem().replace_extension(".hpp");

      opt.output_cpp = vm["output"].as<string>();
      opt.output_cpp = opt.output_cpp / boost::filesystem::path( opt.input ).stem().replace_extension(".cpp");
   }

	//if (vm.count("debug")) opt.debug = true;

   if ( !opt.input.empty() && opt.output.empty() ) {
      opt.output = opt.input;
      opt.output.replace_extension( ".hpp" );
      opt.output_cpp = opt.input;
      opt.output_cpp.replace_extension( ".cpp" );
   }

   if ( vm.count("version") ) {
      std::cout << "scxmlcc version: " << version() << endl;
      cout << endl << "  <!> This is not an original scxmlcc FSM compiler <!>" << endl << endl;
      cout << "For more information about original version, see http://scxmlcc.org" << endl;
      cout << endl; 
      cout << "  Copyright (C) 2013 Jan Pedersen <jp@jp-embedded.com>" << endl;
      cout << "  Copyright (C) 2014 Yuriy Gurin <ygurin@outlook.com>" << endl;
      cout << endl; 
      cout << "  This program is free software: you can redistribute it and/or modify" << endl;
      cout << "  it under the terms of the GNU General Public License as published by" << endl;
      cout << "  the Free Software Foundation, either version 3 of the License, or" << endl;
      cout << "  (at your option) any later version." << endl;
      cout << endl;  
      cout << "  This program is distributed in the hope that it will be useful," << endl;
      cout << "  but WITHOUT ANY WARRANTY; without even the implied warranty of" << endl;
      cout << "  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" << endl;
      cout << "  GNU General Public License for more details." << endl;
      cout << endl; 
      cout << "  You should have received a copy of the GNU General Public License" << endl;
      cout << "  along with this program.  If not, see <http://www.gnu.org/licenses/>." << endl;
      cout << endl;  
      return 0;
   }

   if ( vm.count("help") || opt.input.empty() ) {
      cout << "Extended scxml FSM compiler, version " << version() << endl;
      cout << "  Copyright (C) 2013 Jan Pedersen <jp@jp-embedded.com>" << endl;
      cout << "  Copyright (C) 2014 Yuriy Gurin <ygurin@outlook.com>" << endl;
      cout << "" << endl;
      cout << "Usage: " << argv[0] << " [options] [input]" << endl;
      cout << desc << endl;
      return 0;
   }

   try {
      scxmlcc( opt );
   }
   catch (...) {
      cerr << "Unandled error!" << endl;
      return 1;
   }
   return 0;
}
