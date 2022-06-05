# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_GET_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_GET_HPP
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
$begin atomic_four_mat_mul_get.hpp$$
$spell
    mul
$$

$section atomic_mat_mul Get Routine: Example Implementation$$

$head Syntax$$
$icode%%mat_mul%.get(%call_id%, %n_left%, %n_middle%, %n_right%)%$$

$head Prototype$$
$srcthisfile%0%// BEGIN PROTOTYPE%// END PROTOTYPE%1%$$

$head Purpose$$
Retrieves the dimension information for a an atomic operation that computes
the matrix product $icode%R% = %A% * %B%$$.

$head call_id$$
This argument identifies the dimension information for this matrix product.

$head n_left$$
This result is the row dimension of the matrices $icode A$$ and $icode R$$.

$head n_middle$$
This result is the column dimension of the matrix $icode A$$
and row dimension of the matrix $icode B$$.

$head n_right$$
This result is the column dimension of the matrices $icode B$$ and $icode R$$.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
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
