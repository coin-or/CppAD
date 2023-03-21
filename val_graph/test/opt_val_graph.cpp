// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/cppad.hpp>
namespace { // BEIGN_EMPTY_NAMESPACE
// ----------------------------------------------------------------------------
// optimize_csum
bool csum_op(void)
{  bool ok = true;
   using CppAD::AD;
   using CppAD::vector;
   //
   size_t n = 4;
   vector< AD<double> > ax(n);
   for(size_t j = 0; j < n; ++j)
      ax[j] = 0.0;
   Independent(ax);
   //
   AD<double> asum   = 0.0;
   for(size_t j = 0; j < n; ++j)
      asum += ax[j];
   //
   vector< AD<double> > ay(1);
   ay[0] = asum * asum;
   CppAD::ADFun<double> f(ax, ay);
   //
   f.opt_val_graph();
   //
   // zero order forward
   vector<double> x(n), y(1);
   double sum = 0.0;
   for(size_t j = 0; j < n; ++j)
   {  x[j]  = double(j + 1);
      sum  += x[j];
   }
   double check = sum * sum;
   y     = f.Forward(0, x);
   ok &= y[0] == check;
   //
   /*
   Not yet working
   f.opt_val_graph();
   y     = f.Forward(0, x);
   ok &= y[0] == check;
   */
   //
   return ok;
}
// ----------------------------------------------------------------------------
} // END_EMPTY_NAMESPACE
bool test_opt_val_graph(void)
{  bool ok = true;
   ok     &= csum_op();
   return ok;
}
