// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin con_dyn_var.cpp}

AD Parameter and Variable Functions: Example and Test
#####################################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end con_dyn_var.cpp}
*/
// BEGIN C++

# include <cppad/cppad.hpp>

bool con_dyn_var(void)
{  bool ok = true;

   using CppAD::AD;
   using CppAD::VecAD;
   using CppAD::Parameter;
   using CppAD::Variable;

   // No recording active
   CPPAD_TESTVECTOR(AD<double>) x(1), y(1), dynamic(1);
   x[0]        = 0.0;
   y[0]        = 1.0;
   dynamic[0]  = 2.0;
   //
   ok &= Constant(x[0]);
   ok &= Parameter(x[0]);
   ok &= ! Dynamic(x[0]);
   ok &= ! Variable(x[0]);
   //
   ok &= Constant(y[0]);
   ok &= Parameter(y[0]);
   ok &= ! Dynamic(y[0]);
   ok &= ! Variable(y[0]);
   //
   ok &= Constant(dynamic[0]);
   ok &= Parameter(dynamic[0]);
   ok &= ! Dynamic(dynamic[0]);
   ok &= ! Variable(dynamic[0]);

   // declare independent variables and start recording
   CppAD::Independent(x, dynamic);
   //
   ok &= Variable(x[0]);
   ok &= ! Constant(x[0]);
   ok &= ! Dynamic(x[0]);
   ok &= ! Parameter(x[0]);
   //
   ok &= Constant(y[0]);
   ok &= Parameter(y[0]);
   ok &= ! Dynamic(y[0]);
   ok &= ! Variable(y[0]);
   //
   ok &= Dynamic(dynamic[0]);
   ok &= Parameter(dynamic[0]);
   ok &= ! Constant(dynamic[0]);
   ok &= ! Variable(dynamic[0]);

   // a dependent variable
   y[0] = fabs(x[0]) * dynamic[0];
   ok  &= Variable(y[0]);
   ok  &= ! Constant(y[0]);
   ok  &= ! Dynamic(y[0]);
   ok  &= ! Parameter(y[0]);

   // create f: x -> y and stop tape recording
   CppAD::ADFun<double> f(x, y);
   //
   ok &= Constant(x[0]);
   ok &= Parameter(x[0]);
   ok &= ! Dynamic(x[0]);
   ok &= ! Variable(x[0]);
   //
   ok &= Constant(y[0]);
   ok &= Parameter(y[0]);
   ok &= ! Dynamic(y[0]);
   ok &= ! Variable(y[0]);
   //
   ok &= Constant(dynamic[0]);
   ok &= Parameter(dynamic[0]);
   ok &= ! Dynamic(dynamic[0]);
   ok &= ! Variable(dynamic[0]);

   return ok;
}

// END C++
