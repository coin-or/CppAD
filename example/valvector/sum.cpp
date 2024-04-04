// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin valvector_sum.cpp}

Example and Test of valvector Size
##################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end valvector_sum.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/example/valvector/class.hpp>
//
bool sum(void)
{  // scalar_type
   typedef valvector::scalar_type scalar_type;
   //
   //
   // ok
   bool ok = true;
   //
   // v
   valvector v( {1.0, 2.0, 3.0} );
   //
   // ok
   scalar_type sum = v.sum();
   ok &= sum == scalar_type(6.0);
   //
   return ok;
}
// END C++
