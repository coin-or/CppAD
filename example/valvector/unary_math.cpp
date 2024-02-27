// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin valvector_unary_math.cpp}

Example and Test of valvector Unary Math Functions
##################################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end valvector_unary_math.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++
# include <sstream>
# include <string>
# include <cppad/example/valvector/class.hpp>
# include <cppad/utility/near_equal.hpp>
//
bool math_unary(void)
{  typedef valvector::scalar_type scalar_type;
   //
   // ok
   bool ok = true;
   //
   // eps99
   scalar_type eps99 = CppAD::numeric_limits<scalar_type>::epsilon();
   eps99            *= scalar_type(99);
   //
   // u, v, ok
   valvector u = valvector( {2, 3, 4} );
   valvector v;
   ok &= u.size() == 3;
   //
   // ok
   v   = CppAD::sqrt(u);
   for(size_t i = 0; i < u.size(); ++i)
      ok &= CppAD::NearEqual( v[i], sqrt(u[i]), eps99, eps99); 
   
   //
   return ok;
}
// END C++
