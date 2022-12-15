// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin integer.cpp}

Convert From AD to Integer: Example and Test
############################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end integer.cpp}
*/
// BEGIN C++

# include <cppad/cppad.hpp>

bool Integer(void)
{  bool ok = true;
   using CppAD::AD;
   using CppAD::Integer;

   // domain space vector
   size_t n = 2;
   CPPAD_TESTVECTOR(AD<double>) x(n);
   x[0] = 3.5;
   x[1] = 4.5;

   // check integer before recording
   ok &= (Integer(x[0]) == 3);
   ok &= (Integer(x[1]) == 4);

   // start recording

   // declare independent variables and start tape recording
   CppAD::Independent(x);

   // check integer during recording
   ok &= (Integer(x[0]) == 3);
   ok &= (Integer(x[1]) == 4);

   // check integer for VecAD element
   CppAD::VecAD<double> v(1);
   AD<double> zero(0);
   v[zero] = 2;
   ok &= (Integer(v[zero]) == 2);

   // range space vector
   size_t m = 1;
   CPPAD_TESTVECTOR(AD<double>) y(m);
   y[0] = - x[1];

   // create f: x -> y and stop recording
   CppAD::ADFun<double> f(x, y);

   // check integer after recording
   ok &= (Integer(x[0]) ==  3.);
   ok &= (Integer(x[1]) ==  4.);
   ok &= (Integer(y[0]) == -4.);

   return ok;
}
// END C++
