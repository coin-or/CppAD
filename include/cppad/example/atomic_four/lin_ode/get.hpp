# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_GET_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_GET_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_lin_ode_get.hpp}
{xrst_spell
}

atomic_lin_ode Get Routine: Example Implementation
##################################################

Syntax
******
| *lin_ode* . ``get`` ( *call_id* , *r* , *step* , *pattern* , *transpose* )

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
Retrieves the auxiliary information for a an atomic operation that computes
the solution of a linear ODE.

call_id
*******
This input argument identifies the auxiliary information for this ODE.

r
*
This output argument is the final value for the variable that the ODE is with
respect to.

step
****
This is a positive maximum step size to use when solving the ODE.

pattern
*******
This output argument is a sparsity pattern.

transpose
*********
If this output argument is true (false) the sparsity pattern is for
:math:`A(x)^\R{T}` (:math:`A(x)`).

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_lin_ode_get.hpp}
*/
// BEGIN C++
# include <cppad/example/atomic_four/lin_ode/lin_ode.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
// BEGIN PROTOTYPE
template <class Base>
void atomic_lin_ode<Base>::get(
   size_t call_id, Base& r, Base& step, sparse_rc& pattern, bool& transpose
)
// END PROTOTYPE
{
   // thread
   size_t thread = thread_alloc::thread_num();
   CPPAD_ASSERT_UNKNOWN( work_[thread] != nullptr );
   //
   // pattern_vec
   CppAD::vector<sparse_rc>& pattern_vec( work_[thread]->pattern_vec );
   //
   // call_vec
   CppAD::vector<call_struct>& call_vec( work_[thread]->call_vec );
   //
   CPPAD_ASSERT_UNKNOWN( thread == call_vec[call_id].thread );
   //
   // r
   call_struct& call = call_vec[call_id];
   r         = call.r;
   step      = call.step;
   pattern   = pattern_vec[call.pattern_index];
   transpose = call.transpose;
   //
   return;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
