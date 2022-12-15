// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_vector_jac_sparsity.cpp}

Atomic Vector Sparsity Patterns Example
#######################################

f(u, v)
*******
For this example,
:math:`f : \B{R}^{3m} \rightarrow \B{R}^m`
is defined by :math:`f(u, v, w) = - u * v * w`.
where *u* , *v* , and *w* are in :math:`\B{R}^m`.

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_vector_jac_sparsity.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <cppad/example/atomic_four/vector/vector.hpp>
bool jac_sparsity(void)
{  bool ok = true;
   using CppAD::NearEqual;
   using CppAD::AD;
   //
   // vec_op
   // atomic vector_op object
   CppAD::atomic_vector<double> vec_op("atomic_vector");
   //
   // m
   // size of u, v, and w
   size_t m = 6;
   //
   // n
   size_t n = 3 * m;
   //
   // mul_op, neg_op
   typedef CppAD::atomic_vector<double>::op_enum_t op_enum_t;
   op_enum_t mul_op = CppAD::atomic_vector<double>::mul_enum;
   op_enum_t neg_op = CppAD::atomic_vector<double>::neg_enum;
   // -----------------------------------------------------------------------
   // Record f(u, v, w) = - u * v * w
   // -----------------------------------------------------------------------
   // Independent variable vector
   CPPAD_TESTVECTOR( CppAD::AD<double> ) auvw(n);
   for(size_t j = 0; j < n; ++j)
      auvw[j] = AD<double>(1 + j);
   CppAD::Independent(auvw);
   //
   // au, av, aw
   CPPAD_TESTVECTOR( CppAD::AD<double> ) au(m), av(m), aw(m);
   for(size_t i = 0; i < m; ++i)
   {  au[i] = auvw[0 * m + i];
      av[i] = auvw[1 * m + i];
      aw[i] = auvw[2 * m + i];
   }
   //
   // ax = (au, av)
   CPPAD_TESTVECTOR( CppAD::AD<double> ) ax(2 * m);
   for(size_t i = 0; i < m; ++i)
   {  ax[i]     = au[i];
      ax[m + i] = av[i];
   }
   //
   // ay = u * v
   CPPAD_TESTVECTOR( CppAD::AD<double> ) ay(m);
   vec_op(mul_op, ax, ay);
   //
   // ax = (ay, aw)
   for(size_t i = 0; i < m; ++i)
   {  ax[i]     = ay[i];
      ax[m + i] = aw[i];
   }
   //
   // az = ay * w
   CPPAD_TESTVECTOR( CppAD::AD<double> ) az(m);
   vec_op(mul_op, ax, az);
   //
   // ay = - az
   vec_op(neg_op, az, ay);
   //
   // f
   CppAD::ADFun<double> f(auvw, ay);
   //
   // size_vector, sparsity_pattern
   typedef CPPAD_TESTVECTOR(size_t) size_vector;
   typedef CppAD::sparse_rc<size_vector> sparsity_pattern;
   // -----------------------------------------------------------------------
   // Jacobian sparsity
   // -----------------------------------------------------------------------
   for(size_t direction = 0; direction < 2; ++direction)
   {  sparsity_pattern pattern_out;
      bool transpose     = false;
      bool dependency    = false;
      bool internal_bool = false;
      if( direction == 0 )
      {  // Forward direction
         //
         // pattern_in
         // sparsity pattern for identity matrix
         sparsity_pattern pattern_in(n, n, n);
         for(size_t k = 0; k < n; ++k)
            pattern_in.set(k, k, k);
         //
         // pattern_out
         f.for_jac_sparsity(
            pattern_in, transpose, dependency, internal_bool, pattern_out
         );
      }
      else
      {  // Reverse direction
         //
         // pattern_in
         // sparsity pattern for identity matrix
         sparsity_pattern pattern_in(m, m, m);
         for(size_t k = 0; k < m; ++k)
            pattern_in.set(k, k, k);
         //
         // pattern_out
         f.rev_jac_sparsity(
            pattern_in, transpose, dependency, internal_bool, pattern_out
         );
      }
      //
      // ok
      ok &= pattern_out.nnz() == 3 * m;
      ok &= pattern_out.nr()  == m;
      ok &= pattern_out.nc()  == n;
      //
      // row, col, row_major
      const size_vector& row = pattern_out.row();
      const size_vector& col = pattern_out.col();
      size_vector row_major  = pattern_out.row_major();
      //
      // ok
      size_t ell = 0;
      for(size_t i = 0; i < m; ++i)
      {  // first non-zero in row i
         size_t k = row_major[ell++];
         ok      &= row[k] == i;
         ok      &= col[k] == i;
         // second non-zero in row i
         k        = row_major[ell++];
         ok      &= row[k] == i;
         ok      &= col[k] == m + i;
         // third non-zero in row i
         k        = row_major[ell++];
         ok      &= row[k] == i;
         ok      &= col[k] == 2 * m + i;
      }
   }
   //
   return ok;
}
// END C++
