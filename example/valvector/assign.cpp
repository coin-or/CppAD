// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin valvector_assign.cpp}

Example and Test of valvector Assignment Operator
#################################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end valvector_assign.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/example/valvector/class.hpp>
//
bool assign(void)
{  // scalar_type  
   typedef valvector::scalar_type scalar_type;
   //
   //
   // ok
   bool ok = true;
   //
   // v, ok
   valvector v;
   ok &= v.size() == 1;
   //
   // w
   valvector w = valvector( {2, 3, 4} );
   ok &= w.size() == 3;
   //
   // v
   v    = w;
   v[0] = scalar_type(1);
   //
   // ok
   ok &= v.size() == w.size();
   ok &= w[0] == scalar_type(2);
   ok &= v[0] == scalar_type(1);
   for(size_t i = 1; i < w.size(); ++i)
      ok &= v[i] == w[i];
   //
   return ok;
}
// END C++
