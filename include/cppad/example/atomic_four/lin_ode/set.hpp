# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_SET_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_SET_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_lin_ode_set.hpp}
{xrst_spell
}

atomic_lin_ode Set Routine: Example Implementation
##################################################

Syntax
******
| *call_id* = *lin_ode* . ``set`` ( *r* , *step* , *pattern* , *transpose* )

Prototype
*********
{xrst_literal
   include/cppad/example/atomic_four/lin_ode/lin_ode.hpp
   // BEGIN sparse_rc_type
   // END sparse_rc_type
}
{xrst_literal
   // BEGIN PROTOTYPE
   // END PROTOTYPE
}

Purpose
*******
Stores the auxiliary information for a an atomic operation that computes
the solution of a linear ODE.

r
*
This argument is the final value for the variable that the ODE is with
respect to.

step
****
This is a positive maximum step size to use when solving the ODE.

pattern
*******
This argument is a sparsity pattern.
It would be ``const`` except for the fact that
*pattern.set_row_major* () is called so that checking for
equality is faster; see
:ref:`sparse_rc@set_row_major` .

transpose
*********
If this argument is true (false) the sparsity pattern is for
:math:`A(x)\R{T}` (:math:`A(x)`).

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_lin_ode_set.hpp}
*/
// BEGIN C++
# include <cppad/example/atomic_four/lin_ode/lin_ode.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
// BEGIN PROTOTYPE
template <class Base>
size_t atomic_lin_ode<Base>::set(
   const Base& r, const Base& step, sparse_rc& pattern, const bool& transpose
)
// END PROTOTYPE
{
   // pattern
   // set_row_major so that checking for pattern equality is faster
   pattern.set_row_major();
   //
   // thread
   size_t thread = thread_alloc::thread_num();
   //
   // work_[thread]
   if( work_[thread] == nullptr )
      work_[thread] = new thread_struct;
   //
   // pattern_vec
   CppAD::vector<sparse_rc>& pattern_vec( work_[thread]->pattern_vec );
   //
   // pattern_index
   size_t     n_pattern     = pattern_vec.size();
   size_t     pattern_index = n_pattern;
   for(size_t i = 0; i < n_pattern; ++i)
      if( pattern == pattern_vec[i] )
         pattern_index = i;
   if( pattern_index == n_pattern )
   {  pattern_vec.push_back( pattern );
      CPPAD_ASSERT_UNKNOWN( pattern_vec[pattern_index] == pattern );
   }
   //
   // call_vec
   CppAD::vector<call_struct>& call_vec( work_[thread]->call_vec );
   //
   // call_id
   size_t call_id = call_vec.size();
   //
   // call
   call_struct call;
   call.thread        = thread;
   call.r             = r;
   call.step          = step;
   call.pattern_index = pattern_index;
   call.transpose     = transpose;
   //
   // work_[thread]
   call_vec.push_back( call );
   //
   return call_id;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
