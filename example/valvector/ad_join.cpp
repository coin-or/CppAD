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
   // ok
   f.optimize();
   z = f.Forward(0, x);
   for(size_t j = 0; j < n; ++j)
      ok &= z[0][j] == x[j][0] * x[j][0];
   //
   return ok;
}
// END C++
