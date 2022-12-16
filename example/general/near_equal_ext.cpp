// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin near_equal_ext.cpp}

Compare AD with Base Objects: Example and Test
##############################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end near_equal_ext.cpp}
*/
// BEGIN C++

# include <cppad/cppad.hpp>
# include <complex>

bool near_equal_ext(void)
{  bool ok = true;
   using CppAD::AD;
   using CppAD::NearEqual;

   // double
   double x    = 1.00000;
   double y    = 1.00001;
   double a    =  .00005;
   double r    =  .00005;
   double zero = 0.;

   // AD<double>
   AD<double> ax(x);
   AD<double> ay(y);

   ok &= NearEqual(ax, ay, zero, a);
   ok &= NearEqual(ax, y,  r, zero);
   ok &= NearEqual(x, ay,  r,    a);

   // std::complex<double>
   AD<double> cx(x);
   AD<double> cy(y);

   // AD< std::complex<double> >
   AD<double> acx(x);
   AD<double> acy(y);

   ok &= NearEqual(acx, acy, zero, a);
   ok &= NearEqual(acx,  cy, r, zero);
   ok &= NearEqual(acx,   y, r,    a);
   ok &= NearEqual( cx, acy, r,    a);
   ok &= NearEqual(  x, acy, r,    a);

   return ok;
}

// END C++
