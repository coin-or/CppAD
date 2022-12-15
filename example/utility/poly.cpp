// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin poly.cpp}

Polynomial Evaluation: Example and Test
#######################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end poly.cpp}
*/
// BEGIN C++

# include <cppad/cppad.hpp>
# include <cmath>

bool poly(void)
{  bool ok = true;

   // degree of the polynomial
   size_t deg = 3;

   // set the polynomial coefficients
   CPPAD_TESTVECTOR(double)   a(deg + 1);
   size_t i;
   for(i = 0; i <= deg; i++)
      a[i] = 1.;

   // evaluate this polynomial
   size_t k = 0;
   double z = 2.;
   double p = CppAD::Poly(k, a, z);
   ok      &= (p == 1. + z + z*z + z*z*z);

   // evaluate derivative
   k = 1;
   p = CppAD::Poly(k, a, z);
   ok &= (p == 1 + 2.*z + 3.*z*z);

   return ok;
}

// END C++
