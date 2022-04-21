# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_GET_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_GET_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
--------------------------------------------------------------------------- */
/*
$begin atomic_four_lin_ode_get.hpp$$
$spell
    mul
    lin
$$

$section atomic_lin_ode Get Routine: Example Implementation$$

$head Syntax$$
$icode%lin_ode%.get(%call_id%, %r%, %pattern%, %transpose%)%$$

$head Prototype$$
$srcfile%include/cppad/example/atomic_four/lin_ode/lin_ode.hpp%
    0%// BEGIN sparse_rc_type%// END sparse_rc_type%0
%$$
$srcthisfile%0%// BEGIN PROTOTYPE%// END PROTOTYPE%1%$$

$head Purpose$$
Retrieves the auxillary information for a an atomic operation that computes
the solution of a linear ODE.

$head call_id$$
This input argument identifies the auxillary information for this ODE.

$head r$$
This output argument is the final value for the variable that the ODE is with
respect to.

$head pattern$$
This output argument is a sparsity pattern.

$head transpose$$
If this output argument is true (false) the sparsity pattern is for
$latex A(x)^\R{T}$$ ($latex A(x)$$).

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/example/atomic_four/lin_ode/lin_ode.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
// BEGIN PROTOTYPE
template <class Base>
void atomic_lin_ode<Base>::get(
    size_t call_id, Base& r, sparse_rc& pattern, bool& transpose
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
    pattern   = pattern_vec[call.pattern_index];
    transpose = call.transpose;
    //
    return;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
