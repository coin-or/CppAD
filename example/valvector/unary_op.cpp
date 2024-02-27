// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin valvector_unary_op.cpp}

Example and Test of valvector Unary Operators
#############################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end valvector_unary_op.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/example/valvector/class.hpp>
//
bool unary_op(void)
{  //
   // ok
   bool ok = true;
   //
   // v
   valvector v( {-1, +1} );
   //
   // ok
   valvector w = +v;
   for(size_t i = 0; i < v.size(); ++i)
      ok &= w[i] == v[i];
   //
   // ok
   valvector u = -v;
   for(size_t i = 0; i < v.size(); ++i)
      ok &= u[i] == - v[i];
   //
   return ok;
}
// END C++
