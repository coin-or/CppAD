// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin ad_ctor.cpp}

AD Constructors: Example and Test
#################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end ad_ctor.cpp}
*/
// BEGIN C++

# include <cppad/cppad.hpp>

bool ad_ctor(void)
{  bool ok = true;   // initialize test result flag
   using CppAD::AD;  // so can use AD in place of CppAD::AD

   // default constructor
   AD<double> a;
   a = 0.;
   ok &= a == 0.;
   ok &= Constant(a);

   // constructor from base type
   AD<double> b(1.);
   ok &= b == 1.;
   ok &= Constant(b);

   // constructor from another type that converts to the base type
   AD<double> c(2);
   ok &= c == 2.;
   ok &= Constant(c);

   // constructor from AD<Base>
   AD<double> d(c);
   ok &= d == c;

   // constructor from a VecAD<Base> element
   CppAD::VecAD<double> v(1);
   v[0] = 3.;
   AD<double> e( v[0] );
   ok &= e == 3.;

   return ok;
}

// END C++
