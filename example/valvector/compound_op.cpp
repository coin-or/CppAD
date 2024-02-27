// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin valvector_compound_op.cpp}

Example and Test of valvector Compound Assignment Operators
###########################################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end valvector_compound_op.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/example/valvector/class.hpp>
//
bool compound_op(void)
{  //
   // ok
   bool ok = true;
   //
   // u, v, w
   valvector u(2);
   valvector v( {4, 6, 8} );
   valvector w;
   ok &= u.size() == 1;
   ok &= v.size() == 3;
   ok &= w.size() == 1;
   //
   // ok
   w  = v;
   w += u;
   ok &= w.size() == 3;
   for(size_t i = 0; i < w.size(); ++i)
      ok &= w[i] == v[i] + u[i];
   //
   // ok
   w  = v;
   w -= u;
   ok &= w.size() == 3;
   for(size_t i = 0; i < w.size(); ++i)
      ok &= w[i] == v[i] - u[i];
   //
   // ok
   w  = v;
   w *= u;
   ok &= w.size() == 3;
   for(size_t i = 0; i < w.size(); ++i)
      ok &= w[i] == v[i] * u[i];
   //
   // ok
   w  = v;
   w /= u;
   ok &= w.size() == 3;
   for(size_t i = 0; i < w.size(); ++i)
      ok &= w[i] == v[i] / u[i];
   //
   return ok;
}
// END C++
