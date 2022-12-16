// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin sign.cpp}

Sign Function: Example and Test
###############################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end sign.cpp}
*/
// BEGIN C++

# include <cppad/cppad.hpp>

bool sign(void)
{  bool ok = true;

   using CppAD::AD;
   using CppAD::NearEqual;

   // create f: x -> y where f(x) = sign(x)
   size_t n = 1;
   size_t m = 1;
   CPPAD_TESTVECTOR(AD<double>) ax(n), ay(m);
   ax[0]     = 0.;
   CppAD::Independent(ax);
   ay[0]     = sign(ax[0]);
   CppAD::ADFun<double> f(ax, ay);

   // check value during recording
   ok &= (ay[0] == 0.);

   // use f(x) to evaluate the sign function and its derivatives
   CPPAD_TESTVECTOR(double) x(n), y(m), dx(n), dy(m), w(m), dw(n);
   dx[0] = 1.;
   w[0] = 1.;
   //
   x[0]  = 2.;
   y     = f.Forward(0, x);
   ok   &= (y[0] == 1.);
   dy    = f.Forward(1, dx);
   ok   &= (dy[0] == 0.);
   dw   = f.Reverse(1, w);
   ok  &= (dw[0] == 0.);
   //
   x[0]  = 0.;
   y     = f.Forward(0, x);
   ok   &= (y[0] == 0.);
   dy    = f.Forward(1, dx);
   ok   &= (dy[0] == 0.);
   dw   = f.Reverse(1, w);
   ok  &= (dw[0] == 0.);
   //
   x[0]  = -2.;
   y     = f.Forward(0, x);
   ok   &= (y[0] == -1.);
   dy    = f.Forward(1, dx);
   ok   &= (dy[0] == 0.);
   dw   = f.Reverse(1, w);
   ok  &= (dw[0] == 0.);

   // use a VecAD<Base>::reference object with sign
   CppAD::VecAD<double> v(1);
   AD<double> zero(0);
   v[zero]           = 2.;
   AD<double> result = sign(v[zero]);
   ok   &= (result == 1.);

   return ok;
}

// END C++
