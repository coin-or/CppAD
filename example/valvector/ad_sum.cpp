// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ---------------------------------------------------------------------------
/*
{xrst_begin valvector_ad_sum.cpp}

Example and Test of Summing a valvector
#######################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end valvector_ad_sum.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/example/valvector/sum.hpp>
# include <cppad/cppad.hpp>
bool ad_sum(void)
{  // ok
   bool ok = true;
   //
   // sparsity_type
   typedef CppAD::sparse_rc< CPPAD_TESTVECTOR(size_t) > sparsity_type;
   //
   // scalar_type
   typedef valvector::scalar_type scalar_type;
   //
   // ad_valvector
   typedef CppAD::AD<valvector> ad_valvector;
   //
   // asum
   valvector_ad_sum asum;
   //
   // n, m, p
   size_t n = 1;
   size_t m = 1;
   size_t p = 10;
   //
   // ax
   CPPAD_TESTVECTOR( ad_valvector ) ax(n);
   ax[0] = valvector(1);
   CppAD::Independent(ax);
   //
   // ax_sq
   ad_valvector ax_sq = ax[0] * ax[0];
   //
   // ay
   CPPAD_TESTVECTOR( ad_valvector ) ay(m);
   asum(ax_sq, ay[0]);
   //
   // f
   CppAD::ADFun<valvector> f(ax, ay);
   //
   // x
   CPPAD_TESTVECTOR( valvector ) x(n);
   x[0].resize(p);
   for(size_t k = 0; k < p; ++k)
      x[0][k] = scalar_type(k + 1);
   //
   // y
   CPPAD_TESTVECTOR( valvector ) y(m);
   y = f.Forward(0, x);
   //
   // ok
   scalar_type check(0);
   for(size_t k = 0; k < p; ++k)
      check += x[0][k] * x[0][k];
   ok &= check == y[0][0];
   //
   // dw
   CPPAD_TESTVECTOR( valvector ) w(m), dw(n);
   w[0][0] = 1.0;
   dw = f.Reverse(1, w);
   //
   // ok
   for(size_t k = 0; k < p; ++k)
      ok &= dw[0][k] == scalar_type(2) * x[0][k];
   //
   // jac_pattern
   sparsity_type identity_pattern(n, n, n);
   for(size_t k = 0; k < n; ++k)
      identity_pattern.set(k, k, k);
   bool transpose     = false;
   bool dependency    = false;
   bool internal_bool = false;
   sparsity_type jac_pattern;
   f.for_jac_sparsity(
      identity_pattern, transpose, dependency, internal_bool, jac_pattern
   );
   //
   // ok
   ok &= jac_pattern.nnz()    == 1;
   ok &= jac_pattern.nr()     == 1;
   ok &= jac_pattern.nc()     == 1;
   ok &= jac_pattern.row()[0] == 0;
   ok &= jac_pattern.col()[0] == 0;
   //
   // hes_pattern
   internal_bool = false;
   CPPAD_TESTVECTOR(bool) select_domain(n);
   CPPAD_TESTVECTOR(bool) select_range(m);
   select_range[0]  = true;
   select_domain[0] = true;
   sparsity_type hes_pattern;
   f.for_hes_sparsity (
      select_domain , select_range , internal_bool , hes_pattern
   );
   //
   // ok
   ok &= hes_pattern.nnz()    == n * n;
   ok &= hes_pattern.nr()     == n;
   ok &= hes_pattern.nc()     == n;
   ok &= hes_pattern.row()[0] == 0;
   ok &= hes_pattern.col()[0] == 0;
   //
   // af
   typedef CppAD::ADFun<ad_valvector, valvector> ad_fun_type;
   ad_fun_type af = ad_fun_type( f.base2ad() );
   //
   // g
   CPPAD_TESTVECTOR( ad_valvector ) adx(n), ady(m);
   adx[0] = valvector( 1.0 );
   CppAD::Independent(ax);
   af.Forward(0, ax);
   ady = af.Forward(1, adx);
   CppAD::ADFun<valvector> g(ax, ady);
   //
   // ok
   CPPAD_TESTVECTOR( valvector ) dy(m);
   dy    = g.Forward(0, x);
   check = scalar_type(0);
   for(size_t k = 0; k < p; ++k)
      check += 2.0 * x[0][k];
   ok &= dy[0][0] == check;
   //
   // h
   CPPAD_TESTVECTOR( ad_valvector ) aw(m), adw(n);
   aw[0] = valvector( 1.0 );
   CppAD::Independent(ax);
   af.Forward(0, ax);
   adw = af.Reverse(1, aw);
   CppAD::ADFun<valvector> h(ax, adw);
   //
   // ok
   dw = h.Forward(0, x);
   for(size_t k = 0; k < p; ++k)
      ok &= dw[0][k] == 2.0 * x[0][k];
   //
   // ok
   f.optimize();
   y     = f.Forward(0, x);
   check = scalar_type(0);
   for(size_t k = 0; k < p; ++k)
      check += x[0][k] * x[0][k];
   ok &= y[0][0] == check;
   //
   return ok;
}
// END C++
