// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin bool_fun.cpp}

AD Boolean Functions: Example and Test
######################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end bool_fun.cpp}
*/
// BEGIN C++

# include <cppad/cppad.hpp>
# include <complex>


// define abbreviation for double precision complex
typedef std::complex<double> Complex;

namespace {
   // a unary bool function with Complex argument
   static bool IsReal(const Complex &x)
   {  return x.imag() == 0.; }

   // a binary bool function with Complex arguments
   static bool AbsGeq(const Complex &x, const Complex &y)
   {  double axsq = x.real() * x.real() + x.imag() * x.imag();
      double aysq = y.real() * y.real() + y.imag() * y.imag();

      return axsq >= aysq;
   }

   // Create version of IsReal with AD<Complex> argument
   // inside of namespace and outside of any other function.
   CPPAD_BOOL_UNARY(Complex, IsReal)

   // Create version of AbsGeq with AD<Complex> arguments
   // inside of namespace and outside of any other function.
   CPPAD_BOOL_BINARY(Complex, AbsGeq)

}
bool BoolFun(void)
{  bool ok = true;

   CppAD::AD<Complex> x = Complex(1.,  0.);
   CppAD::AD<Complex> y = Complex(1.,  1.);

   ok &= IsReal(x);
   ok &= ! AbsGeq(x, y);

   return ok;
}

// END C++
