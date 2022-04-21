# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_SET_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_SET_HPP
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
$begin atomic_four_lin_ode_set.hpp$$
$spell
    mul
    lin
    const
$$

$section atomic_lin_ode Set Routine: Example Implementation$$

$head Syntax$$
$icode%call_id% = %lin_ode%.set(%r%, %pattern%, %transpose%)%$$

$head Prototype$$
$srcfile%include/cppad/example/atomic_four/lin_ode/lin_ode.hpp%
    0%// BEGIN sparse_rc_type%// END sparse_rc_type%0
%$$
$srcthisfile%0%// BEGIN PROTOTYPE%// END PROTOTYPE%1%$$

$head Purpose$$
Stores the auxillary information for a an atomic operation that computes
the solution of a linear ODE.

$head r$$
This argument is the final value for the variable that the ODE is with
respect to.

$head pattern$$
This argument is a sparsity pattern.
It would be $code const$$ except for the fact that
$icode%pattern.set_row_major()%$$ is called so that checking for
equality is faster; see
$cref/set_row_major/sparse_rc/set_row_major/$$.

$head transpose$$
If this argument is true (false) the sparsity pattern is for
$latex A(x)\R{T}$$ ($latex A(x)$$).

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/example/atomic_four/lin_ode/lin_ode.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
// BEGIN PROTOTYPE
template <class Base>
size_t atomic_lin_ode<Base>::set(
    const Base& r, sparse_rc& pattern, const bool& transpose
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
    {   pattern_vec.push_back( pattern );
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
