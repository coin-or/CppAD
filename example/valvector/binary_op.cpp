// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin valvector_binary_op.cpp}

Example and Test of valvector Binary Operators
##############################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end valvector_binary_op.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/example/valvector/class.hpp>
//
bool binary_op(void)
{  //
   // ok
   bool ok = true;
   //
   // u, v, w
   valvector u( {4, 6, 8} );
   valvector v(2);
   valvector w;
   //
   // w_size
   size_t w_size = std::max( u.size(), v.size() );
   //
   // ok
   w = u + v;
   ok &= w.size() == w_size;
   for(size_t i = 0; i < w_size; ++i)
      ok &= w[i] == u[i] + v[i];
   //
   // ok
   w = u - v;
   ok &= w.size() == w_size;
   for(size_t i = 0; i < w_size; ++i)
      ok &= w[i] == u[i] - v[i];
   //
   // ok
   w = u * v;
   ok &= w.size() == w_size;
   for(size_t i = 0; i < w_size; ++i)
      ok &= w[i] == u[i] * v[i];
   //
   // ok
   w = u / v;
   ok &= w.size() == w_size;
   for(size_t i = 0; i < w_size; ++i)
      ok &= w[i] == u[i] / v[i];
   //
   return ok;
}
// END C++
