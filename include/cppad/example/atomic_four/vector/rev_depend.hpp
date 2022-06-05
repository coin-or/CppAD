# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_REV_DEPEND_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_REV_DEPEND_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin atomic_four_vector_rev_depend.hpp$$
$spell
    Jacobian
    jac
$$

$section Atomic Vector Forward Type Calculation: Example Implementation$$

$head Purpose$$
The $code rev_depend$$ routine overrides the virtual functions
used by the atomic_four base class for Jacobian sparsity calculations; see
$cref/rev_depend/atomic_four_rev_depend/$$.

$head Example$$
The file $cref atomic_four_vector_rev_depend.cpp$$
contains an example and test that uses this member function.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/example/atomic_four/vector/vector.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// rev_depend override
template <class Base>
bool atomic_vector<Base>::rev_depend(
    size_t                         call_id     ,
    CppAD::vector<bool>&           depend_x    ,
    const CppAD::vector<bool>&     depend_y    )
{
    // n, m
    size_t n     = depend_x.size();
    size_t m     = depend_y.size();
    //
    // type_y
    if( n == m  )
    {   // unary operator
        for(size_t i = 0; i < m; ++i)
            depend_x[i] = depend_y[i];
    }
    else
    {   // binary operator
        for(size_t i = 0; i < m; ++i)
        {   depend_x[i]     = depend_y[i];
            depend_x[m + i] = depend_y[i];
        }
    }
    return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
