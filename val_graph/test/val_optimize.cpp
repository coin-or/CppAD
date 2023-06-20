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
   // ax, ap
   size_t n = 2;
   vector< AD<double> > ax(n), ap(n);
   for(size_t j = 0; j < n; ++j)
   {  ax[j] = 0.0;
      ap[j] = 0.0;
   }
   Independent(ax, ap);
   //
   // asum
   AD<double> asum = 0.0;
   for(size_t j = 0; j < n; ++j)
   {  asum += ax[j];
      asum += ap[j];
   }
   AD<double> aplus  = ax[0] + ap[0];
   AD<double> aminus = ax[0] - ap[0];
   asum             += CondExpLe(ax[0], ap[0], aplus, aminus);
   //
   // f
   vector< AD<double> > ay(1);
   ay[0] = asum * asum;
   CppAD::ADFun<double> f(ax, ay);
   //
   // val_optimize
   f.val_optimize("val_graph no_conditional_skip");
   //
   // x, p, y, check, ok
   // zero order forward
   vector<double> x(n), p(n), y(1);
   double sum = 0.0;
   for(size_t j = 0; j < n; ++j)
   {  p[j]  = double(j + 1);
      x[j]  = double(n + j + 1);
      sum  += x[j];
      sum  += p[j];
   }
   if( x[0] <= p[0] )
      sum += x[0] + p[0];
   else
      sum += x[0] - p[0];
   //
   double check = sum * sum;
   f.new_dynamic(p);
   y     = f.Forward(0, x);
   ok &= y[0] == check;
   //
   // val_optimize, y, ok
   f.val_optimize("val_graph no_conditional_skip");
   f.new_dynamic(p);
   y     = f.Forward(0, x);
   ok &= y[0] == check;
   //
   return ok;
}
// ----------------------------------------------------------------------------
} // END_EMPTY_NAMESPACE
bool test_val_optimize(void)
{  bool ok = true;
   ok     &= csum_op();
   return ok;
}
