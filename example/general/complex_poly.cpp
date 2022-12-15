// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin complex_poly.cpp}

Complex Polynomial: Example and Test
####################################

Poly
****
Select this link to view specifications for :ref:`Poly-name` :

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end complex_poly.cpp}
*/
// BEGIN C++
// Complex examples should supppress conversion warnings
# include <cppad/wno_conversion.hpp>

# include <cppad/cppad.hpp>
# include <complex>

bool complex_poly(void)
{  bool ok    = true;
   size_t deg = 4;

   using CppAD::AD;
   using CppAD::Poly;
   typedef std::complex<double> Complex;

   // polynomial coefficients
   CPPAD_TESTVECTOR( Complex )   a   (deg + 1); // coefficients for p(z)
   CPPAD_TESTVECTOR(AD<Complex>) A   (deg + 1);
   size_t i;
   for(i = 0; i <= deg; i++)
      A[i] = a[i] = Complex(double(i), double(i));

   // independent variable vector
   CPPAD_TESTVECTOR(AD<Complex>) Z(1);
   Complex z = Complex(1., 2.);
   Z[0]      = z;
   Independent(Z);

   // dependent variable vector and indices
   CPPAD_TESTVECTOR(AD<Complex>) P(1);

   // dependent variable values
   P[0] = Poly(0, A, Z[0]);

   // create f: Z -> P and vectors used for derivative calculations
   CppAD::ADFun<Complex> f(Z, P);
   CPPAD_TESTVECTOR(Complex) v( f.Domain() );
   CPPAD_TESTVECTOR(Complex) w( f.Range() );

   // check first derivative w.r.t z
   v[0]      = 1.;
   w         = f.Forward(1, v);
   Complex p = Poly(1, a, z);
   ok &= ( w[0]  == p );

   // second derivative w.r.t z is 2 times its second order Taylor coeff
   v[0] = 0.;
   w    = f.Forward(2, v);
   p    = Poly(2, a, z);
   ok &= ( 2. * w[0]  == p );

   return ok;
}

// END C++
