// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin hes_lu_det.cpp}

Gradient of Determinant Using LU Factorization: Example and Test
################################################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end hes_lu_det.cpp}
*/
// BEGIN C++
// Complex examples should supppress conversion warnings
# include <cppad/wno_conversion.hpp>

# include <cppad/cppad.hpp>
# include <cppad/speed/det_by_lu.hpp>

// The AD complex case is used by this example so must
// define a specializatgion of LeqZero,AbsGeq for the AD<Complex> case
namespace CppAD {
   CPPAD_BOOL_BINARY( std::complex<double> ,  AbsGeq   )
   CPPAD_BOOL_UNARY(  std::complex<double> ,  LeqZero )
}


bool HesLuDet(void)
{  bool ok = true;

   using namespace CppAD;
   typedef std::complex<double> Complex;
   double eps99 = 99.0 * std::numeric_limits<double>::epsilon();

   size_t n = 2;

   // object for computing determinants
   det_by_lu< AD<Complex> > Det(n);

   // independent and dependent variable vectors
   CPPAD_TESTVECTOR(AD<Complex>)  X(n * n);
   CPPAD_TESTVECTOR(AD<Complex>)  D(1);

   // value of the independent variable
   size_t i;
   for(i = 0; i < n * n; i++)
      X[i] = Complex( double(i), -double(i) );

   // set the independent variables
   Independent(X);

   D[0]  = Det(X);

   // create the function object
   ADFun<Complex> f(X, D);

   // argument value
   CPPAD_TESTVECTOR(Complex)     x( n * n );
   for(i = 0; i < n * n; i++)
      x[i] = Complex( double(2 * i) , double(i) );

   // first derivative of the determinant
   CPPAD_TESTVECTOR(Complex) H( n * n * n * n );
   H = f.Hessian(x, 0);

   /*
   f(x)     = x[0] * x[3] - x[1] * x[2]
   f'(x)    = ( x[3], -x[2], -x[1], x[0] )
   */
   Complex zero(0., 0.);
   Complex one(1., 0.);
   Complex Htrue[]  = {
      zero, zero, zero,  one,
      zero, zero, -one, zero,
      zero, -one, zero, zero,
         one, zero, zero, zero
   };
   for( i = 0; i < n*n*n*n; i++)
      ok &= NearEqual( Htrue[i], H[i], eps99 , eps99 );

   return ok;
}

// END C++
