// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin jacobian.cpp}

Jacobian: Example and Test
##########################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end jacobian.cpp}
*/
// BEGIN C++

# include <cppad/cppad.hpp>
namespace { // ---------------------------------------------------------
// define the template function JacobianCases<Vector> in empty namespace
template <class Vector>
bool JacobianCases()
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

   // a calculation between the domain and range values
   AD<double> Square = X[0] * X[0];

   // range space vector
   size_t m = 3;
   CPPAD_TESTVECTOR(AD<double>)  Y(m);
   Y[0] = Square * exp( X[1] );
   Y[1] = Square * sin( X[1] );
   Y[2] = Square * cos( X[1] );

   // create f: X -> Y and stop tape recording
   CppAD::ADFun<double> f(X, Y);

   // new value for the independent variable vector
   Vector x(n);
   x[0] = 2.;
   x[1] = 1.;

   // compute the derivative at this x
   Vector jac( m * n );
   jac = f.Jacobian(x);

   /*
   F'(x) = [ 2 * x[0] * exp(x[1]) ,  x[0] * x[0] * exp(x[1]) ]
         [ 2 * x[0] * sin(x[1]) ,  x[0] * x[0] * cos(x[1]) ]
         [ 2 * x[0] * cos(x[1]) , -x[0] * x[0] * sin(x[i]) ]
   */
   ok &=  NearEqual( 2.*x[0]*exp(x[1]), jac[0*n+0], eps99, eps99);
   ok &=  NearEqual( 2.*x[0]*sin(x[1]), jac[1*n+0], eps99, eps99);
   ok &=  NearEqual( 2.*x[0]*cos(x[1]), jac[2*n+0], eps99, eps99);

   ok &=  NearEqual( x[0] * x[0] *exp(x[1]), jac[0*n+1], eps99, eps99);
   ok &=  NearEqual( x[0] * x[0] *cos(x[1]), jac[1*n+1], eps99, eps99);
   ok &=  NearEqual(-x[0] * x[0] *sin(x[1]), jac[2*n+1], eps99, eps99);

   return ok;
}
} // End empty namespace
# include <vector>
# include <valarray>
bool Jacobian(void)
{  bool ok = true;
   // Run with Vector equal to three different cases
   // all of which are Simple Vectors with elements of type double.
   ok &= JacobianCases< CppAD::vector  <double> >();
   ok &= JacobianCases< std::vector    <double> >();
   ok &= JacobianCases< std::valarray  <double> >();
   return ok;
}
// END C++
