# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_HES_SPARSITY_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_HES_SPARSITY_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_mat_mul_hes_sparsity.hpp}

Atomic Matrix Multiply Jacobian Sparsity Pattern: Example Implementation
########################################################################

Purpose
*******
The ``hes_sparsity`` routine overrides the virtual functions
used by the atomic_four base class for Jacobian sparsity calculations; see
:ref:`hes_sparsity<atomic_four_hes_sparsity-name>` .

Example
*******
The file :ref:`atomic_four_mat_mul_sparsity.cpp-name`
contains an example and test using this operator.

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_mat_mul_hes_sparsity.hpp}
*/
// BEGIN C++
# include <cppad/example/atomic_four/mat_mul/mat_mul.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// hes_sparsity override
template <class Base>
bool atomic_mat_mul<Base>::hes_sparsity(
   size_t                                         call_id      ,
   const CppAD::vector<bool>&                     ident_zero_x ,
   const CppAD::vector<bool>&                     select_x     ,
   const CppAD::vector<bool>&                     select_y     ,
   CppAD::sparse_rc< CppAD::vector<size_t> >&     pattern_out  )
{
   // n_left, n_middle, n_right
   size_t n_left, n_middle, n_right;
   get(call_id, n_left, n_middle, n_right);
   //
   // n
   size_t n     = select_x.size();
   //
   // check sizes
# ifndef NDEBUG
   size_t m     = select_y.size();
   assert( n == n_middle * ( n_left +  n_right ) );
   assert( m == n_left * n_right );
# endif
   //
   // offset
   size_t offset = n_left * n_middle;
   //
   // pattern_out
   pattern_out.resize(n, n, 0);
   for(size_t i = 0; i < n_left; ++i)
   {  for(size_t j = 0; j < n_right; ++j)
      {  size_t ij = i * n_right + j;               // C_{i,j} = y[ij]
         if( select_y[ij] ) for(size_t k = 0; k < n_middle; ++k)
         {  size_t ik = i * n_middle + k;          // A_{i,k} = x[ik]
            size_t kj = offset + k * n_right + j;  // B_{k,j} = x[kj]
            if( select_x[ik] && select_x[kj] )
            {  // an (ik, kj) pair can only occur once in this loop
               pattern_out.push_back(ik, kj);
               pattern_out.push_back(kj, ik);
            }
         }
      }
   }
# ifndef NDEBUG
   // sorting checks hat there are no duplicate entries
   pattern_out.row_major();
# endif
   //
   return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
