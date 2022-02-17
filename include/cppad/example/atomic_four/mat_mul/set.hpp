# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_SET_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_SET_HPP
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
$begin atomic_four_mat_mul_set.hpp$$
$spell
    mul
$$

$section atomic_mat_mul Set Routine: Example Implementation$$

$head Syntax$$
$icode%call_id% = %mat_mul%.set(%n_left%, %n_middle%, %n_right%)%$$

$head Prototype$$
$srcthisfile%0%// BEGIN PROTOTYPE%// END PROTOTYPE%1%$$

$head Purpose$$
Stores the dimension information for a an atomic operation that computes
the matrix product $icode%R% = %A% * %B%$$.

$head n_left$$
This argument is the row dimension of the matrices $icode A$$ and $icode R$$.

$head n_middle$$
This argument is the column dimension of the matrix $icode A$$
and row dimension of the matrix $icode B$$.

$head n_right$$
This argument is the column dimension of the matrices $icode B$$ and $icode R$$.

$head call_id$$
This return value identifies the dimension information above.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
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
