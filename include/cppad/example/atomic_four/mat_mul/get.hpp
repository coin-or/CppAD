# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_GET_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_GET_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_mat_mul_get.hpp}

atomic_mat_mul Get Routine: Example Implementation
##################################################

Syntax
******
| ``mat_mul`` . *get* ( ``call_id`` , ``n_left`` , ``n_middle`` , ``n_right`` )

Prototype
*********
{xrst_literal
   // BEGIN PROTOTYPE
   // END PROTOTYPE
}

Purpose
*******
Retrieves the dimension information for a an atomic operation that computes
the matrix product *R* = *A* * *B* .

call_id
*******
This argument identifies the dimension information for this matrix product.

n_left
******
This result is the row dimension of the matrices *A* and *R* .

n_middle
********
This result is the column dimension of the matrix *A*
and row dimension of the matrix *B* .

n_right
*******
This result is the column dimension of the matrices *B* and *R* .

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_mat_mul_get.hpp}
*/
// BEGIN C++
# include <cppad/example/atomic_four/mat_mul/mat_mul.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
// BEGIN PROTOTYPE
template <class Base>
void atomic_mat_mul<Base>::get(
   size_t call_id, size_t& n_left, size_t& n_middle, size_t& n_right
)
// END PROTOTYPE
{
   // thread
   size_t thread = thread_alloc::thread_num();
   assert( work_[thread] != nullptr );
   assert( thread == (*work_[thread])[call_id].thread );
   //
   // n_left, n_middle, n_right
   call_struct& call = (*work_[thread])[call_id];
   n_left   = call.n_left;
   n_middle = call.n_middle;
   n_right  = call.n_right;
   //
   return;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
