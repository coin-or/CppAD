// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin for_hes_sparsity.cpp}

Forward Mode Hessian Sparsity: Example and Test
###############################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end for_hes_sparsity.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>

bool for_hes_sparsity(void)
{  bool ok = true;
   using CppAD::AD;
   typedef CPPAD_TESTVECTOR(size_t)     SizeVector;
   typedef CppAD::sparse_rc<SizeVector> sparsity;
   //
   // domain space vector
   size_t n = 3;
   CPPAD_TESTVECTOR(AD<double>) ax(n);
   ax[0] = 0.;
   ax[1] = 1.;
   ax[2] = 2.;

   // declare independent variables and start recording
   CppAD::Independent(ax);

   // range space vector
   size_t m = 2;
   CPPAD_TESTVECTOR(AD<double>) ay(m);
   ay[0] = sin( ax[2] );
   ay[1] = ax[0] * ax[1];

   // create f: x -> y and stop tape recording
   CppAD::ADFun<double> f(ax, ay);

   // include all x components in sparsity pattern
   CPPAD_TESTVECTOR(bool) select_domain(n);
   for(size_t j = 0; j < n; j++)
      select_domain[j] = true;

   // compute sparsity pattern for H(x) = F_1''(x)
   CPPAD_TESTVECTOR(bool) select_range(m);
   select_range[0]    = false;
   select_range[1]    = true;
   bool internal_bool = true;
   sparsity pattern_out;
   f.for_hes_sparsity(
      select_domain, select_range, internal_bool, pattern_out
   );
   size_t nnz = pattern_out.nnz();
   ok        &= nnz == 2;
   ok        &= pattern_out.nr() == n;
   ok        &= pattern_out.nc() == n;
   {  // check results
      const SizeVector& row( pattern_out.row() );
      const SizeVector& col( pattern_out.col() );
      SizeVector row_major = pattern_out.row_major();
      //
      ok &= row[ row_major[0] ] ==  0  && col[ row_major[0] ] ==  1;
      ok &= row[ row_major[1] ] ==  1  && col[ row_major[1] ] ==  0;
   }
   //
   // compute sparsity pattern for H(x) = F_0''(x)
   select_range[0] = true;
   select_range[1] = false;
   f.for_hes_sparsity(
      select_domain, select_range, internal_bool, pattern_out
   );
   nnz = pattern_out.nnz();
   ok &= nnz == 1;
   ok &= pattern_out.nr() == n;
   ok &= pattern_out.nc() == n;
   {  // check results
      const SizeVector& row( pattern_out.row() );
      const SizeVector& col( pattern_out.col() );
      //
      ok &= row[0] ==  2  && col[0] ==  2;
   }
   return ok;
}
// END C++
