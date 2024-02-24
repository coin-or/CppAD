// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ---------------------------------------------------------------------------
# include <cppad/example/valvector/class.hpp>
# include <cppad/example/valvector/sum_ad.hpp>
# include <cppad/cppad.hpp>
//
// sum_ad
bool sum_ad(void)
{  // ok
   bool ok = true;
   //
   // ad_valvector
   typedef CppAD::AD<valvector> ad_valvector;
   //
   // ad_sum
   sum_ad_valvector asum("sum_ad_valvector");
   //
   // ax
   CPPAD_TESTVECTOR( ad_valvector ) ax(2);
   ax[0] = valvector(1.0);
   ax[1] = valvector(2.0);
   CppAD::Independent(ax);
   //
   // y = f(x)
   // y[0] = ax[0]_0 * ax[1]_0 + ax[0]_1 * ax[1]_1 + ax[0]_2 + ay[0]_2
   CPPAD_TESTVECTOR( ad_valvector ) ay(1), ap(1);
   ap[0] = ax[0] * ax[1];
   asum(ap, ay);
   CppAD::ADFun<valvector> f(ax, ay);
   //
   // x
   CPPAD_TESTVECTOR( valvector ) x(2);
   x[0] = valvector( {1.0, 2.0, 3.0} );
   x[1] = valvector( {4.0, 3.0, 2.0} );
   //
   // y
   CPPAD_TESTVECTOR( valvector ) y(1);
   y = f.Forward(0, x);
   //
   // ok
   valvector check;
   check = valvector( 4.0 + 6.0 + 6.0 );
   ok   &= y[0] == check;
   //
   //  dy
   CPPAD_TESTVECTOR( valvector ) dx(2);
   dx[0] = valvector(1.0);
   dx[1] = valvector(0.0);
   //
   // dy
   CPPAD_TESTVECTOR( valvector ) dy(1);
   dy = f.Forward(1, dx);
   //
   // ok
   ok &= dy[0] == valvector( 4.0 + 3.0 + 2.0 );
   //
   return ok;
}
