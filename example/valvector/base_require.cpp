// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ---------------------------------------------------------------------------
/*
{xrst_begin valvector_base_require.cpp}

Example and Test of valvector Base Requirements
###############################################

Scope
*****
This only covers that valvector
:ref:`valvector_base_require@Features Implemented Here` .

Source Code
***********
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end valvector_base_require.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/example/valvector/class.hpp>
//
// base_require
bool base_require(void)
{  // ok
   bool ok = true;
   //
   // x, y
   valvector x, y;
   //
   // ok
   x = valvector(2.5);
   int integer_x = CppAD::Integer(x);
   ok &= integer_x == 2;
   //
   // ok
   valvector nan = CppAD::numeric_limits<valvector>::quiet_NaN();
   ok &= CppAD::isnan( nan );
   //
   // ok
   x = valvector( {1, 2} );
   std::string string_x = CppAD::to_string( x );
   std::string s;
   for(auto itr = string_x.begin(); itr != string_x.end(); ++itr)
      if( *itr != ' ')
         s.push_back( *itr );
   ok &= s == "{1,2}";
   //
   // ok
   x   = valvector( {1, 2} );
   y   = valvector( {1.0, 2.0} );
   ok &= CppAD::EqualOpSeq(x, y);
   //
   // ok
   ok &= CppAD::IdenticalCon(x);
   ok &= ! CppAD::IdenticalZero(x);
   ok &= ! CppAD::IdenticalOne(x);
   ok &= CppAD::IdenticalEqualCon(x, y);
   //
   x   = valvector( {0.0, 0.0} );
   ok &= CppAD::IdenticalZero(x);
   //
   return ok;
}
// END C++
