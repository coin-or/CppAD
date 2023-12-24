// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin tanh.cpp}

The AD tanh Function: Example and Test
######################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end tanh.cpp}
*/
// BEGIN C++

# include <cppad/cppad.hpp>
# include <cmath>
# include <limits>

bool Tanh(void)
{  bool ok = true;

   using CppAD::AD;
   using CppAD::NearEqual;
   double eps = 10. * CppAD::numeric_limits<double>::epsilon();

   // domain space vector
   size_t n  = 1;
   double x0 = 0.5;
   CPPAD_TESTVECTOR(AD<double>) x(n);
   x[0]      = x0;

   // declare independent variables and start tape recording
   CppAD::Independent(x);

   // range space vector
   size_t m = 1;
   CPPAD_TESTVECTOR(AD<double>) y(m);
   y[0] = CppAD::tanh(x[0]);

   // create f: x -> y and stop tape recording
   CppAD::ADFun<double> f(x, y);

   // check value
   double check = std::tanh(x0);
   ok &= NearEqual(y[0] , check,  eps, eps);

   // forward computation of first partial w.r.t. x[0]
   CPPAD_TESTVECTOR(double) dx(n);
   CPPAD_TESTVECTOR(double) dy(m);
   dx[0] = 1.;
   dy    = f.Forward(1, dx);
   check = 1. - std::tanh(x0) * std::tanh(x0);
   ok   &= NearEqual(dy[0], check, eps, eps);

   // reverse computation of derivative of y[0]
   CPPAD_TESTVECTOR(double)  w(m);
   CPPAD_TESTVECTOR(double) dw(n);
   w[0]  = 1.;
   dw    = f.Reverse(1, w);
   ok   &= NearEqual(dw[0], check, eps, eps);

   // use a VecAD<Base>::reference object with tan
   CppAD::VecAD<double> v(1);
   AD<double> zero(0);
   v[zero]           = x0;
   AD<double> result = CppAD::tanh(v[zero]);
   check = std::tanh(x0);
   ok   &= NearEqual(result, check, eps, eps);

   return ok;
}

// END C++
