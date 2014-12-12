#ifndef UT_SEQUENCE_HPP
#define UT_SEQUENCE_HPP

/****************************************************************************
 * Project         UT
 * (c) copyright   2014
 * Company         Yuriy Gurin RnD
 *                 All rights reserved
 ****************************************************************************/
/**
 * @file    Sequence.hpp
 * @ingroup SCXMLCC::UT
 * @author  YGurin
 *
 * @brief   
 */

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include <vector>
#include <string>
#include <iostream>

// --------------------------------------------------------------------------
// Code definition
// --------------------------------------------------------------------------

class Sequence
{
public:
   void action( std::string action );
   Sequence& check();
   Sequence& clear();
   Sequence& enter( std::string name );
   Sequence& exit( std::string name );
   inline Sequence& tr_action( std::string name );

private:
   std::vector<std::string> mActions;
   std::vector<std::string> mSequence;
};

// --------------------------------------------------------------------------
inline Sequence& Sequence::tr_action( std::string name )
// --------------------------------------------------------------------------
{
   mSequence.push_back( "action_" + name );
   return *this;
}

#endif // UT_SEQUENCE_HPP