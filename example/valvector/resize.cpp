// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin valvector_resize.cpp}

Example and Test of valvector Resize
####################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end valvector_resize.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/example/valvector/class.hpp>
//
bool resize(void)
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
   // v
   v.resize(3);
   for(size_t i = 0; i < v.size(); ++i)
      v[i] = scalar_type(i); 
   //
   // ok
   ok &= v.size() == 3;
   for(size_t i = 0; i < v.size(); ++i)
      ok &= v[i] == scalar_type(i);
   //
   return ok;
}
// END C++
