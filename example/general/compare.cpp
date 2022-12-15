// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin compare.cpp}

AD Binary Comparison Operators: Example and Test
################################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end compare.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>

bool Compare(void)
{  bool ok = true;
   using CppAD::AD;
   using CppAD::NearEqual;
   double eps99 = 99.0 * std::numeric_limits<double>::epsilon();

   // declare independent variables and start tape recording
   size_t n  = 2;
   double x0 = 0.5;
   double x1 = 1.5;
   CPPAD_TESTVECTOR(AD<double>) x(n);
   x[0]      = x0;
   x[1]      = x1;
   CppAD::Independent(x);

   // some binary comparison operations
   AD<double> p;
   if( x[0] < x[1] )
      p = x[0];   // values in x choose this case
   else
      p = x[1];
   if( x[0] <= x[1] )
      p *= x[0];  // values in x choose this case
   else
      p *= x[1];
   if( x[0] >  x[1] )
      p *= x[0];
   else
      p *= x[1];  // values in x choose this case
   if( x[0] >= x[1] )
      p *= x[0];
   else
      p *= x[1];  // values in x choose this case
   if( x[0] == x[1] )
      p *= x[0];
   else
      p *= x[1];  // values in x choose this case
   if( x[0] != x[1] )
      p *= x[0];  // values in x choose this case
   else
      p *= x[1];

   // dependent variable vector
   size_t m = 1;
   CPPAD_TESTVECTOR(AD<double>) y(m);
   y[0] = p;

   // create f: x -> y and stop tape recording
   CppAD::ADFun<double> f(x, y);

   // check value
   ok &= NearEqual(y[0] , x0*x0*x1*x1*x1*x0, eps99, eps99);

   // forward computation of partials w.r.t. x[0]
   CPPAD_TESTVECTOR(double) dx(n);
   CPPAD_TESTVECTOR(double) dy(m);
   dx[0] = 1.;
   dx[1] = 0.;
   dy    = f.Forward(1, dx);
   ok   &= NearEqual(dy[0], 3.*x0*x0*x1*x1*x1, eps99, eps99);

   // forward computation of partials w.r.t. x[1]
   dx[0] = 0.;
   dx[1] = 1.;
   dy    = f.Forward(1, dx);
   ok   &= NearEqual(dy[0], 3.*x0*x0*x1*x1*x0, eps99, eps99);

   // reverse computation of derivative of y[0]
   CPPAD_TESTVECTOR(double)  w(m);
   CPPAD_TESTVECTOR(double) dw(n);
   w[0]  = 1.;
   dw    = f.Reverse(1, w);
   ok   &= NearEqual(dw[0], 3.*x0*x0*x1*x1*x1, eps99, eps99);
   ok   &= NearEqual(dw[1], 3.*x0*x0*x1*x1*x0, eps99, eps99);

   return ok;
}

// END C++
