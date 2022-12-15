# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_JAC_SPARSITY_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_JAC_SPARSITY_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_vector_jac_sparsity.hpp}

Atomic Vector Jacobian Sparsity Pattern: Example Implementation
###############################################################

Purpose
*******
The ``jac_sparsity`` routine overrides the virtual functions
used by the atomic_four base class for Jacobian sparsity calculations; see
:ref:`jac_sparsity<atomic_four_jac_sparsity-name>` .

Example
*******
The file :ref:`atomic_four_vector_jac_sparsity.cpp-name`
contains an example and test using this operator.

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_vector_jac_sparsity.hpp}
*/
// BEGIN C++
# include <cppad/example/atomic_four/vector/vector.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// jac_sparsity override
template <class Base>
bool atomic_vector<Base>::jac_sparsity(
   size_t                                         call_id      ,
   bool                                           dependency   ,
   const CppAD::vector<bool>&                     ident_zero_x ,
   const CppAD::vector<bool>&                     select_x     ,
   const CppAD::vector<bool>&                     select_y     ,
   CppAD::sparse_rc< CppAD::vector<size_t> >&     pattern_out  )
{
   size_t n = select_x.size();
   size_t m = select_y.size();
   assert( n == m || n == 2 * m );
   //
   // nnz
   // number of non-zeros in sparsity pattern
   size_t nnz = 0;
   for(size_t i = 0; i < m; ++i) if( select_y[i] )
   {  size_t j = i;
      if( select_x[j] )
         ++nnz;
      if( n != m )
      {  // binary operator
         j = m + i;
         if( select_x[j] )
            ++nnz;
      }
   }
   //
   // pattern_out
   pattern_out.resize(m, n, nnz);
   size_t k = 0;
   for(size_t i = 0; i < m; ++i) if( select_y[i] )
   {  size_t j = i;
      if( select_x[j] )
         pattern_out.set(k++, i, j);
      if( n != m )
      {  // binary operator
         j = m + i;
         if( select_x[j] )
            pattern_out.set(k++, i, j);
      }
   }
   assert( k == nnz);
   //
   return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
