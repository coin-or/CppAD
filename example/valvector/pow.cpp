// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin valvector_pow.cpp}

Example and Test of valvector pow Function
##########################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end valvector_pow.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++
# include <sstream>
# include <string>
# include <cppad/example/valvector/class.hpp>
# include <cppad/utility/near_equal.hpp>
//
bool pow(void)
{  typedef valvector::scalar_type scalar_type;
   //
   // ok
   bool ok = true;
   //
   // eps99
   scalar_type eps99 = CppAD::numeric_limits<scalar_type>::epsilon();
   eps99            *= scalar_type(99);
   //
   // x, y, z
   valvector x = valvector( {2, 3, 4} );
   valvector y = valvector( {4, 3, 2} );;
   valvector z = CppAD::pow(x, y);
   //
   // ok
   ok &= x.size() == 3;
   for(size_t i = 0; i < x.size(); ++i)
   {  scalar_type check = pow( x[i], y[i] ); 
      ok &= CppAD::NearEqual( z[i], check, eps99, eps99); 
   }
   return ok;
}
// END C++
