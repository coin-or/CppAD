# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_SET_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_SET_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_mat_mul_set.hpp}

atomic_mat_mul Set Routine: Example Implementation
##################################################

Syntax
******
| *call_id* = *mat_mul* . ``set`` ( *n_left* , *n_middle* , *n_right* )

Prototype
*********
{xrst_literal
   // BEGIN PROTOTYPE
   // END PROTOTYPE
}

Purpose
*******
Stores the dimension information for a an atomic operation that computes
the matrix product *R* = *A* * *B* .

n_left
******
This argument is the row dimension of the matrices *A* and *R* .

n_middle
********
This argument is the column dimension of the matrix *A*
and row dimension of the matrix *B* .

n_right
*******
This argument is the column dimension of the matrices *B* and *R* .

call_id
*******
This return value identifies the dimension information above.

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_mat_mul_set.hpp}
*/
// BEGIN C++
# include <cppad/example/atomic_four/mat_mul/mat_mul.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
// BEGIN PROTOTYPE
template <class Base>
size_t atomic_mat_mul<Base>::set(
   size_t n_left, size_t n_middle, size_t n_right
)
// END PROTOTYPE
{
   // thread
   size_t thread = thread_alloc::thread_num();
   //
   // work_[thread]
   if( work_[thread] == nullptr )
      work_[thread] = new call_vector;
   //
   // call_id
   size_t call_id = work_[thread]->size();
   //
   // call
   call_struct call;
   call.n_left   = n_left;
   call.n_middle = n_middle;
   call.n_right  = n_right;
   call.thread   = thread;
   //
   // work_[thread]
   work_[thread]->push_back( call );
   //
   return call_id;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
