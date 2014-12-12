/****************************************************************************
 * Project         UT
 * (c) copyright   2014
 * Company         Yuriy Gurin RnD
 *                 All rights reserved
 ****************************************************************************/
/**
 * @file    Sequence.cpp
 * @ingroup SCXMLCC::UT
 * @author  YGurin
 *
 * @brief   
 */

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "Sequence.hpp"
#include <cppunit/extensions/HelperMacros.h>
#include <iostream>

// --------------------------------------------------------------------------
// Code implementation
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
Sequence& Sequence::enter( std::string name )
// --------------------------------------------------------------------------
{
   mSequence.push_back( "state_" + name + "::enter" );
   return *this;
}

// --------------------------------------------------------------------------
Sequence& Sequence::exit( std::string name )
// --------------------------------------------------------------------------
{
   mSequence.push_back( "state_" + name + "::exit" );
   return *this;
}

// --------------------------------------------------------------------------
void Sequence::action( std::string action )
// --------------------------------------------------------------------------
{
   mActions.push_back( action );
}

// --------------------------------------------------------------------------
Sequence& Sequence::check()
// --------------------------------------------------------------------------
{
   if ( mActions != mSequence )
   {
      std::cout << std::endl << std::endl << "<!> ";
      if ( mActions.size() == 0)
      {
         std::cout << "No actions from FSM received";
      }
      else
      {
         for ( std::vector<std::string>::iterator it = mActions.begin(); it != mActions.end(); it++ )
         {
            std::cout << *it << " ";
         }
      }

      std::cout << std::endl << std::endl;
   }
   clear();

   CPPUNIT_ASSERT( mActions == mSequence );
   return *this;
}

// --------------------------------------------------------------------------
Sequence& Sequence::clear()
// --------------------------------------------------------------------------
{
   mActions.clear();
   mSequence.clear();
   return *this;
}