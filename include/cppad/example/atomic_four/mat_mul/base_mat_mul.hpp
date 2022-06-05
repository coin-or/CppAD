# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_BASE_MAT_MUL_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_BASE_MAT_MUL_HPP
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
$begin atomic_four_mat_mul_base_mat_mul.hpp$$
$spell
$$

$section
Atomic Multiply Base Matrices: Example Implementation
$$

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/example/atomic_four/mat_mul/mat_mul.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// base_mat_mul
template <class Base>
void atomic_mat_mul<Base>::base_mat_mul(
    size_t                         n_left      ,
    size_t                         n_middle    ,
    size_t                         n_right     ,
    const CppAD::vector<Base>&     x           ,
    CppAD::vector<Base>&           y           )
{
# ifndef NDEBUG
    // n, m
    size_t n     = x.size();
    size_t m     = y.size();
    //
    // check sizes
    assert( n == n_middle * (n_left + n_right ) );
    assert( m == n_left * n_right );
# endif
    //
    // offset
    size_t offset = n_left * n_middle;
    //
    // y
    // y[ i * n_right + j] = sum_k
    //      x[i * n_middle + k] * x[ offset + k * n_right + j]
    // type_y
    for(size_t i = 0; i < n_left; ++i)
    {   for(size_t j = 0; j < n_right; ++j)
        {   Base sum_ij = Base(0);
            for(size_t k = 0; k < n_middle; ++k)
            {   Base left_ik  = x[i * n_middle + k];
                Base right_kj = x[offset + k * n_right + j];
                sum_ij       += left_ik * right_kj;
            }
            y[i * n_right + j] = sum_ij;
        }
    }
    return;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
