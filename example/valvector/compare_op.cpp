// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin valvector_compare_op.cpp}

Example and Test of valvector Compare Operators
###############################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end valvector_compare_op.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/example/valvector/class.hpp>
//
bool compare_op(void)
{  typedef valvector::scalar_type scalar_type;
   //
   // ok
   bool ok = true;
   //
   // u, v
   valvector u( {4, 6, 8} );
   valvector v( {4.0, 6.0, 8.0} );
   //
   // ok
   ok &= u == v;
   u[0] = u[0] + scalar_type(1);
   ok &= u != v;
   //
   // ok
   ok &= valvector( 2.0 ) < valvector( 3.0 );
   ok &= valvector( 4.0 ) > valvector( 3.0 );
   //
   return ok;
}
// END C++
