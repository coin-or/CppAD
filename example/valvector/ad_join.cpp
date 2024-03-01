// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ---------------------------------------------------------------------------
# include <cppad/example/valvector/ad_join.hpp>
# include <cppad/cppad.hpp>
/*
{xrst_begin valvector_ad_join.cpp}

Example and Test of Splitting a valvector
#########################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end valvector_ad_join.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++
bool ad_join(void)
{  // ok
   bool ok = true;
   //
   // scalar_type
   typedef valvector::scalar_type scalar_type;
   //
   // ad_valvector
   typedef CppAD::AD<valvector> ad_valvector;
   //
   // ajoin
   valvector_ad_join ajoin;
   //
   // n, m
   size_t n = 10;
   size_t m = 1;
   //
   // ax
   CPPAD_TESTVECTOR( ad_valvector ) ax(n);
   for(size_t j = 0; j < n; ++j)
      ax[j] = valvector(j);
   CppAD::Independent(ax);
   //
   // ay
   ad_valvector ay;
   ajoin(ax, ay);
   //
   ad_valvector ay_sq = ay * ay;
   //
   // f
   CPPAD_TESTVECTOR( ad_valvector ) az(m);
   az[0] = ay_sq;
   CppAD::ADFun<valvector> f(ax, az);
   //
   // x
   CPPAD_TESTVECTOR( valvector ) x(n);
   for(size_t j = 0; j < n; ++j)
      x[j][0] = scalar_type(j + 1);
   //
   // a
   CPPAD_TESTVECTOR( valvector ) z(m);
   z = f.Forward(0, x);
   //
   // ok
   for(size_t j = 0; j < n; ++j)
      ok &= z[0][j] == x[j][0] * x[j][0];
   //
   // dw
   CPPAD_TESTVECTOR( valvector ) w(m), dw(n);
   w[0][0] = 1.0;
   dw = f.Reverse(1, w);
   //
   // ok
   for(size_t j = 0; j < n; ++j)
      ok &= dw[0][j] == scalar_type(2) * x[0][j];
   return ok;
}
// END C++
