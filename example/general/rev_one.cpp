// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin rev_one.cpp}

First Order Derivative Driver: Example and Test
###############################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end rev_one.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>
namespace { // -------------------------------------------------------
// define the template function RevOneCases<Vector> in empty namespace
template <class Vector>
bool RevOneCases()
{  bool ok = true;
   using CppAD::AD;
   using CppAD::NearEqual;
   double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
   using CppAD::exp;
   using CppAD::sin;
   using CppAD::cos;

   // domain space vector
   size_t n = 2;
   CPPAD_TESTVECTOR(AD<double>)  X(n);
   X[0] = 1.;
   X[1] = 2.;

   // declare independent variables and starting recording
   CppAD::Independent(X);

   // range space vector
   size_t m = 3;
   CPPAD_TESTVECTOR(AD<double>)  Y(m);
   Y[0] = X[0] * exp( X[1] );
   Y[1] = X[0] * sin( X[1] );
   Y[2] = X[0] * cos( X[1] );

   // create f: X -> Y and stop tape recording
   CppAD::ADFun<double> f(X, Y);

   // new value for the independent variable vector
   Vector x(n);
   x[0] = 2.;
   x[1] = 1.;

   // compute and check derivative of y[0]
   Vector dw(n);
   dw  = f.RevOne(x, 0);
   ok &= NearEqual(dw[0],      exp(x[1]), eps99, eps99); // w.r.t x[0]
   ok &= NearEqual(dw[1], x[0]*exp(x[1]), eps99, eps99); // w.r.t x[1]

   // compute and check derivative of y[1]
   dw  = f.RevOne(x, 1);
   ok &= NearEqual(dw[0],      sin(x[1]), eps99, eps99);
   ok &= NearEqual(dw[1], x[0]*cos(x[1]), eps99, eps99);

   // compute and check derivative of y[2]
   dw  = f.RevOne(x, 2);
   ok &= NearEqual(dw[0],        cos(x[1]), eps99, eps99);
   ok &= NearEqual(dw[1], - x[0]*sin(x[1]), eps99, eps99);

   return ok;
}
} // End empty namespace
# include <vector>
# include <valarray>
bool RevOne(void)
{  bool ok = true;
   // Run with Vector equal to three different cases
   // all of which are Simple Vectors with elements of type double.
   ok &= RevOneCases< CppAD::vector  <double> >();
   ok &= RevOneCases< std::vector    <double> >();
   ok &= RevOneCases< std::valarray  <double> >();
   return ok;
}
// END C++
