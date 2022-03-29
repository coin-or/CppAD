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
$icode%lin_ode%.get(%call_id%, %r%, %n_step%)%$$

$head Prototype$$
$srcthisfile%0%// BEGIN PROTOTYPE%// END PROTOTYPE%1%$$

$head Purpose$$
Retrieves the auxillary information for a an atomic operation that computes
the solution of a linear ODE.

$head call_id$$
This input argument identifies the auxillary information for this ODE.

$head r$$
This output argument is the final value for the variable that the ODE is with
respect to.

$head n_step$$
This output argument is the number of steps to use when approximating
to solution of the ODE.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/example/atomic_four/lin_ode/lin_ode.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
// BEGIN PROTOTYPE
template <class Base>
void atomic_lin_ode<Base>::get(size_t call_id, Base& r, size_t& n_step)
// END PROTOTYPE
{
    // thread
    size_t thread = thread_alloc::thread_num();
    assert( work_[thread] != nullptr );
    assert( thread == (*work_[thread])[call_id].thread );
    //
    // r, n_step
    call_struct& call = (*work_[thread])[call_id];
    r      = call.r;
    n_step = call.n_step;
    //
    return;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
