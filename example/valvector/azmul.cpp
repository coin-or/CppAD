// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin valvector_azmul.cpp}

Example and Test of valvector azmul Function
############################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end valvector_azmul.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++
# include <sstream>
# include <string>
# include <cppad/example/valvector/class.hpp>
# include <cppad/utility/near_equal.hpp>
//
bool azmul(void)
{  typedef valvector::scalar_type scalar_type;
   //
   // ok
   bool ok = true;
   //
   // nan
   scalar_type nan   = CppAD::numeric_limits<scalar_type>::quiet_NaN();
   //
   // x, y, z
   valvector x = valvector( {0, 1, 2} );
   valvector y = valvector( {nan, nan, 5} );;
   valvector z = CppAD::azmul(x, y);
   //
   // ok
   ok &= z[0] == scalar_type(0);
   ok &= CppAD::isnan( z[1] );
   ok &= z[2] == x[2] * y[2];
   //
   return ok;
}
// END C++
