// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin sub_eq.cpp}

AD Compound Assignment Subtraction: Example and Test
####################################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end sub_eq.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>

bool SubEq(void)
{  bool ok = true;
   using CppAD::AD;
   using CppAD::NearEqual;
   double eps99 = 99.0 * std::numeric_limits<double>::epsilon();

   // domain space vector
   size_t  n = 1;
   double x0 = .5;
   CPPAD_TESTVECTOR(AD<double>) x(n);
   x[0]      = x0;

   // declare independent variables and start tape recording
   CppAD::Independent(x);

   // range space vector
   size_t m = 2;
   CPPAD_TESTVECTOR(AD<double>) y(m);
   y[0] = 3. * x[0];    // initial value
   y[0] -= 2;           // AD<double> -= int
   y[0] -= 4.;          // AD<double> -= double
   y[1] = y[0] -= x[0]; // use the result of a compound assignment

   // create f: x -> y and stop tape recording
   CppAD::ADFun<double> f(x, y);

   // check value
   ok &= NearEqual(y[0] , 3.*x0-(2.+4.+x0), eps99, eps99);
   ok &= NearEqual(y[1] ,             y[0], eps99, eps99);

   // forward computation of partials w.r.t. x[0]
   CPPAD_TESTVECTOR(double) dx(n);
   CPPAD_TESTVECTOR(double) dy(m);
   dx[0] = 1.;
   dy    = f.Forward(1, dx);
   ok   &= NearEqual(dy[0], 2., eps99, eps99);
   ok   &= NearEqual(dy[1], 2., eps99, eps99);

   // reverse computation of derivative of y[0]
   CPPAD_TESTVECTOR(double)  w(m);
   CPPAD_TESTVECTOR(double) dw(n);
   w[0]  = 1.;
   w[1]  = 0.;
   dw    = f.Reverse(1, w);
   ok   &= NearEqual(dw[0], 2., eps99, eps99);

   // use a VecAD<Base>::reference object with computed subtraction
   CppAD::VecAD<double> v(1);
   AD<double> zero(0);
   AD<double> result = 1;
   v[zero] = 2;
   result -= v[zero];
   ok     &= (result == -1);

   return ok;
}

// END C++
