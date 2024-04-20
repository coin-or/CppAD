// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ---------------------------------------------------------------------------
/*
{xrst_begin valvector_ad_join.cpp}

Example and Test of Joining a valvector
#######################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end valvector_ad_join.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/example/valvector/split_join.hpp>
# include <cppad/cppad.hpp>
bool ad_join(void)
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
   ok &= jac_pattern.nnz() == n;
   ok &= jac_pattern.nr()  == m;
   ok &= jac_pattern.nc()  == n;
   {  CPPAD_TESTVECTOR(size_t) col_major = jac_pattern.col_major();
      const CPPAD_TESTVECTOR(size_t)& row = jac_pattern.row();
      const CPPAD_TESTVECTOR(size_t)& col = jac_pattern.col();
      for(size_t k = 0; k < n; ++k)
      {  ok &= row[ col_major[k] ] == 0;
         ok &= col[ col_major[k] ] == k;
      }
   }
   //
   // hes_pattern
   internal_bool = false;
   CPPAD_TESTVECTOR(bool) select_domain(n);
   CPPAD_TESTVECTOR(bool) select_range(m);
   select_range[0] = true;
   for(size_t j = 0; j < n; ++j)
      select_domain[j] = true;
   sparsity_type hes_pattern;
   f.for_hes_sparsity (
      select_domain , select_range , internal_bool , hes_pattern
   );
   //
   // ok
   // The sparsity calculation does not distinguish between different elements
   // of a valvector so the sparsity pattern fills in (this is not efficient).
   ok &= hes_pattern.nnz() == n * n;
   ok &= hes_pattern.nr()  == n;
   ok &= hes_pattern.nc()  == n;
   {  CPPAD_TESTVECTOR(size_t) col_major = hes_pattern.col_major();
      const CPPAD_TESTVECTOR(size_t)& row = hes_pattern.row();
      const CPPAD_TESTVECTOR(size_t)& col = hes_pattern.col();
      size_t k = 0;
      for(size_t j = 0; j < n; ++j)
      {  for(size_t i = 0; i < n; ++i)
         {  ok &= row[ col_major[k] ] == i;
            ok &= col[ col_major[k] ] == j;
            ++k;
         }
      }
   }
   //
   // af
   typedef CppAD::ADFun<ad_valvector, valvector> ad_fun_type;
   ad_fun_type af = ad_fun_type( f.base2ad() );
   //
   // g
   CPPAD_TESTVECTOR( ad_valvector ) adx(n), adz(m);
   for(size_t j = 0; j < n; ++j)
      adx[j] = valvector( 1.0 );
   CppAD::Independent(ax);
   af.Forward(0, ax);
   adz = af.Forward(1, adx);
   CppAD::ADFun<valvector> g(ax, adz);
   //
   // ok
   CPPAD_TESTVECTOR( valvector ) dz(m);
   dz = g.Forward(0, x);
   for(size_t j = 0; j < n; ++j)
      ok &= dz[0][j] == 2.0 * x[j][0];
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
   for(size_t j = 0; j < n; ++j)
      ok &= dw[j][0] == 2.0 * x[j][0];
   //
   // ok
   f.optimize();
   z = f.Forward(0, x);
   for(size_t j = 0; j < n; ++j)
      ok &= z[0][j] == x[j][0] * x[j][0];
   //
   return ok;
}
// END C++
