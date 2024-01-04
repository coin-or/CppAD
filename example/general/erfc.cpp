// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin erfc.cpp}

The AD erfc Function: Example and Test
######################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end erfc.cpp}
*/
// BEGIN C++

# include <cppad/cppad.hpp>
# include <cmath>
# include <limits>

bool erfc(void)
{  bool ok = true;

   using CppAD::AD;
   using CppAD::NearEqual;
   double eps = 10. * CppAD::numeric_limits<double>::epsilon();

   // domain space vector
   size_t n  = 1;
   double x0 = 0.5;
   CPPAD_TESTVECTOR(AD<double>) ax(n);
   ax[0]     = x0;

   // declare independent variables and start tape recording
   CppAD::Independent(ax);

   // range space vector
   size_t m = 1;
   CPPAD_TESTVECTOR(AD<double>) ay(m);
   ay[0] = CppAD::erfc(ax[0]);

   // create f: x -> y and stop tape recording
   CppAD::ADFun<double> f(ax, ay);

   // check relative error
   double erfc_x0 = 0.4795001221869534;
   ok &= NearEqual(ay[0] , erfc_x0,  0.,    4e-4);
   double tmp = std::max(1e-15, eps);
   ok &= NearEqual(ay[0] , erfc_x0,  0.,    tmp);

   // value of derivative of erfc at x0
   double pi     = 4. * std::atan(1.);
   double factor = 2. / sqrt(pi);
   double check  = - factor * std::exp(-x0 * x0);

   // forward computation of first partial w.r.t. x[0]
   CPPAD_TESTVECTOR(double) dx(n);
   CPPAD_TESTVECTOR(double) dy(m);
   dx[0] = 1.;
   dy    = f.Forward(1, dx);
   ok   &= NearEqual(dy[0], check,  0.,  1e-3);
   ok   &= NearEqual(dy[0], check,  0.,  eps);

   // reverse computation of derivative of y[0]
   CPPAD_TESTVECTOR(double)  w(m);
   CPPAD_TESTVECTOR(double) dw(n);
   w[0]  = 1.;
   dw    = f.Reverse(1, w);
   ok   &= NearEqual(dw[0], check,  0., 1e-1);
   ok   &= NearEqual(dw[0], check,  0., eps);

   // use a VecAD<Base>::reference object with erfc
   CppAD::VecAD<double> v(1);
   AD<double> zero(0);
   v[zero]           = x0;
   AD<double> result = CppAD::erfc(v[zero]);
   ok   &= NearEqual(result, ay[0], eps, eps);

   // use a double with erfc
   ok   &= NearEqual(CppAD::erfc(x0), ay[0], eps, eps);

   return ok;
}

// END C++
