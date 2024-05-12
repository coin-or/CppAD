// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

#include <cppad/cppad.hpp>

bool base_complex(void)
{  // ok
   bool ok = true;
   //
   // base, ad_base
   typedef std::complex<double> base;
   typedef CppAD::AD<base>      ad_base;
   //
   // f
   CPPAD_TESTVECTOR(ad_base) ax(1), ay(1);
   ax[0] = 0.0;
   CppAD::Independent(ax);
   ay[0] = ax[0] * ax[0];
   CppAD::ADFun<base> f(ax, ay);
   //
   // f
   f.optimize();
   //
   // dy
   std::vector<base> x(1), dx(1), dy(1);
   x[0]  = 5;
   f.Forward(0, x);
   dx[0] = 1;
   dy    = f.Forward(1, dx);
   //
   // ok
   ok &= dy[0] == 2.0 * x[0];
   //
   return ok;
}
