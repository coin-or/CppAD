// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin near_equal.cpp}

NearEqual Function: Example and Test
####################################

File Name
*********
This file is called ``near_equal.cpp`` instead of
``NearEqual.cpp``
to avoid a name conflict with ``../lib/NearEqual.cpp``
in the corresponding Microsoft project file.

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end near_equal.cpp}
*/
// BEGIN C++

# include <cppad/utility/near_equal.hpp>

# include <complex>

bool Near_Equal(void)
{  bool ok = true;
   typedef std::complex<double> Complex;
   using CppAD::NearEqual;

   // double
   double x    = 1.00000;
   double y    = 1.00001;
   double a    =  .00003;
   double r    =  .00003;
   double zero = 0.;
   double inf  = 1. / zero;
   double nan  = 0. / zero;

   ok &= NearEqual(x, y, zero, a);
   ok &= NearEqual(x, y, r, zero);
   ok &= NearEqual(x, y, r, a);

   ok &= ! NearEqual(x, y, r / 10., a / 10.);
   ok &= ! NearEqual(inf, inf, r, a);
   ok &= ! NearEqual(-inf, -inf, r, a);
   ok &= ! NearEqual(nan, nan, r, a);

   // complex
   Complex X(x, x / 2.);
   Complex Y(y, y / 2.);
   Complex Inf(inf, zero);
   Complex Nan(zero, nan);

   ok &= NearEqual(X, Y, zero, a);
   ok &= NearEqual(X, Y, r, zero);
   ok &= NearEqual(X, Y, r, a);

   ok &= ! NearEqual(X, Y, r / 10., a / 10.);
   ok &= ! NearEqual(Inf, Inf, r, a);
   ok &= ! NearEqual(-Inf, -inf, r, a);
   ok &= ! NearEqual(Nan, Nan, r, a);

   return ok;
}

// END C++
