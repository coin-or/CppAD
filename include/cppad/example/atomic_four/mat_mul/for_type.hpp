# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_FOR_TYPE_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_FOR_TYPE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_mat_mul_for_type.hpp}

Atomic Matrix Multiply Forward Type Calculation: Example Implementation
#######################################################################

Purpose
*******
The ``for_type`` routine overrides the virtual functions
used by the atomic_four base; see
:ref:`for_type<atomic_four_for_type-name>` .

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_mat_mul_for_type.hpp}
*/
// BEGIN C++
# include <cppad/example/atomic_four/mat_mul/mat_mul.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// for_type override
template <class Base>
bool atomic_mat_mul<Base>::for_type(
   size_t                                     call_id     ,
   const CppAD::vector<CppAD::ad_type_enum>&  type_x      ,
   CppAD::vector<CppAD::ad_type_enum>&        type_y      )
{
   //
   // n_left, n_middle, n_right
   size_t n_left, n_middle, n_right;
   get(call_id, n_left, n_middle, n_right);
# ifndef NDEBUG
   // n, m
   size_t n     = type_x.size();
   size_t m     = type_y.size();
   //
   // check sizes
   assert( n == n_left * n_middle + n_middle * n_right );
   assert( m == n_left * n_right );
# endif
   //
   // offset
   size_t offset = n_left * n_middle;
   //
   // type_y
   // y[ i * n_right + j] = sum_k
   //      x[i * n_middle + k] * x[ offset + k * n_right + j]
   // treat multpilication by zero like absolute zero
   for(size_t i = 0; i < n_left; ++i)
   {  for(size_t j = 0; j < n_right; ++j)
      {  CppAD::ad_type_enum type_ij = CppAD::identical_zero_enum;
         for(size_t k = 0; k < n_middle; ++k)
         {  CppAD::ad_type_enum type_ik = type_x[i * n_middle + k];
            CppAD::ad_type_enum type_kj = type_x[offset + k * n_right + j];
            if( type_ik != identical_zero_enum )
            {  if( type_kj != identical_zero_enum )
               {  type_ij = std::max(type_ij, type_ik);
                  type_ij = std::max(type_ij, type_kj);
               }
            }
         }
         type_y[ i * n_right + j] = type_ij;
      }
   }
   return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
