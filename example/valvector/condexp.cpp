// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin valvector_condexp.cpp}

Example and Test of valvector Conditional Expressions
#####################################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end valvector_condexp.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++
# include <sstream>
# include <string>
# include <cppad/example/valvector/class.hpp>
# include <cppad/utility/near_equal.hpp>
//
bool condexp(void)
{  typedef valvector::scalar_type scalar_type;
   //
   // ok
   bool ok = true;
   //
   // eps99
   scalar_type eps99 = CppAD::numeric_limits<scalar_type>::epsilon();
   eps99            *= scalar_type(99);
   //
   // left, right, if_true, if_false
   valvector left     = valvector( {0, 1, 2, 3, 4} );
   valvector right    = valvector( 2 );
   valvector if_true  = left;
   valvector if_false = right;
   //
   // result
   valvector result = CppAD::CondExpGe(left, right, if_true, if_false);
   //
   // ok
   for(size_t i = 0; i < result.size(); ++i)
      ok &= result[i] == std::max( left[i], right[i] );
   //
   return ok;
}
// END C++
