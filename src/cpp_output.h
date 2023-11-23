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

#ifndef __CPP_OUTPUT
#define __CPP_OUTPUT

//todo store scxml in internal format. to be able to parse other input formats
#include "scxml_parser.h"
#include "options.h"
#include <cctype>
#include <ostream>

class cpp_output {
   std::ostream &out;
   const scxml_parser &sc;
   const options &opt;
   std::string tab;
   int mTransitionIndex;

public:
   cpp_output(std::ostream &ofs, const scxml_parser &sc, const options &op) : out(ofs), sc(sc), opt(op), tab("\t"), mTransitionIndex(0) {};

   void gen();

protected:
   std::string classname();
   std::string state_t();
   std::string state_actions_t();
   std::string state_composite_t();
   std::string state_parallel_t();
   void gen_model_base();
   void gen_state_base();
   void gen_state_composite_base();
   void gen_state_parallel_base();
   void gen_state_actions_base();
   void gen_transition_base();
   void gen_state(const scxml_parser::state &state);
   void gen_sc();
   void trim();

private:
   scxml_parser::state_list children(const scxml_parser::state &state);

   void gen_ahi_header();
   void gen_template_calls();

   std::string UpperFirstLetter(std::string word);
};

class Condition {
   unsigned int _index;
   std::string _condition;

   char getNextChar() {
      char result = '\0';
      _index++;
      if ( _index < _condition.length() ) {
         result = _condition[_index];
      }
      return result;
   }

   char currentChar() {
      char result = '\0';
      if ( _index < _condition.length() ) {
         result = _condition[_index];
      }
      return result;
   }

   std::string getId(int& startPos, bool &isword) {
      std::string result;
      isword = false;
      _index = startPos;

      bool process = true;
      while ( process ) {
         char c = currentChar();
         startPos = _index;
         if (std::isalpha(c) || c == '_') {
            do {
               result += c;
               c = getNextChar();
            } while ( (std::isalpha(c) || std::isdigit(c) || c == '_') && c != 0 );
            isword = true;
            process = false;
         } else {
            switch ( c ) {
            case ' ':
               getNextChar();
               break;
            case '"':
               do {
                  c = getNextChar();
               } while ( c != '"' && c != 0 );
               c = getNextChar();
               break;
            case '\'':
               c = getNextChar();
               c = getNextChar();
               c = getNextChar();
               break;
            case 0:
               process = false;
               break;
            default:
               result = c;
               process = false;
               break;
            }
         }
      }
      return result;
   }

public:
   void fixCondition( const std::vector<std::pair<std::string, std::string>>& datamodel ) {
      std::string word;
      int position = 0;
      bool prohibited = false;
      do {
         bool isword;
         word = getId(position, isword);
         bool found = false;
         if (!prohibited && isword) {
            for (auto& p : datamodel) {
               if (p.second == word) {
                  _condition.insert(position, "sc.model.");
                  found = true;
                  break;
               }
            }
         }
         if (found) {
            position += word.length() + 9;
         } else {
            position += word.length();
         }
         prohibited = word == ":" || word == ".";
      } while (word != "");
   }

   std::string getCondition() const {
      return _condition;
   }

public:
   Condition( const std::string& condition )
      : _condition( condition ) {
        _index = 0;
   }
};

#endif // __CPP_OUTPUT
