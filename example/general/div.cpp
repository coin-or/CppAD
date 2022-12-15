// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin div.cpp}

AD Binary Division: Example and Test
####################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end div.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>

bool Div(void)
{  bool ok = true;
   using CppAD::AD;
   using CppAD::NearEqual;
   double eps99 = 99.0 * std::numeric_limits<double>::epsilon();


   // domain space vector
   size_t n  = 1;
   double x0 = 0.5;
   CPPAD_TESTVECTOR(AD<double>) x(n);
   x[0]      = x0;

   // declare independent variables and start tape recording
   CppAD::Independent(x);

   // some binary division operations
   AD<double> a = x[0] / 1.; // AD<double> / double
   AD<double> b = a  / 2;    // AD<double> / int
   AD<double> c = 3. / b;    // double     / AD<double>
   AD<double> d = 4  / c;    // int        / AD<double>

   // range space vector
   size_t m = 1;
   CPPAD_TESTVECTOR(AD<double>) y(m);
   y[0] = (x[0] * x[0]) / d;   // AD<double> / AD<double>

   // create f: x -> y and stop tape recording
   CppAD::ADFun<double> f(x, y);

   // check value
   ok &= NearEqual(y[0], x0*x0*3.*2.*1./(4.*x0), eps99, eps99);

   // forward computation of partials w.r.t. x[0]
   CPPAD_TESTVECTOR(double) dx(n);
   CPPAD_TESTVECTOR(double) dy(m);
   dx[0] = 1.;
   dy    = f.Forward(1, dx);
   ok   &= NearEqual(dy[0], 3.*2.*1./4., eps99, eps99);

   // reverse computation of derivative of y[0]
   CPPAD_TESTVECTOR(double)  w(m);
   CPPAD_TESTVECTOR(double) dw(n);
   w[0]  = 1.;
   dw    = f.Reverse(1, w);
   ok   &= NearEqual(dw[0], 3.*2.*1./4., eps99, eps99);

   // use a VecAD<Base>::reference object with division
   CppAD::VecAD<double> v(1);
   AD<double> zero(0);
   v[zero] = d;
   AD<double> result = (x[0] * x[0]) / v[zero];
   ok     &= (result == y[0]);

   return ok;
}

// END C++
