// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin valvector_output.cpp}

Example and Test of Outputting a valvector
##########################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end valvector_output.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++
# include <sstream>
# include <string>
# include <cppad/example/valvector/class.hpp>
//
bool output(void)
{  //
   // ok
   bool ok = true;
   //
   // x
   valvector x( {4, 6, 8} );
   //
   // s
   std::stringstream ss;
   ss << x;
   std::string s = ss.str();
   //
   // q
   std::string q;
   for(auto itr = s.begin(); itr != s.end(); ++itr)
      if( *itr != ' ')
         q.push_back( *itr ); 
   // 
   ok &= q == "{4,6,8}";
   //
   return ok;
}
// END C++
