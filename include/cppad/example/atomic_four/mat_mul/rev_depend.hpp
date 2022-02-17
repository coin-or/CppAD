# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_REV_DEPEND_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_REV_DEPEND_HPP
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
$begin atomic_four_mat_mul_rev_depend.hpp$$
$spell
    Jacobian
    jac
$$

$section
Atomic Matrix Multiply Reverse Dependency Analysis: Example Implementation
$$

$head Purpose$$
The $code rev_depend$$ routine is used by $cref optimize$$
to reduce the number of variables in the recording of a function.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/example/atomic_four/mat_mul/mat_mul.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// rev_depend override
template <class Base>
bool atomic_mat_mul<Base>::rev_depend(
    size_t                         call_id  ,
    CppAD::vector<bool>&           depend_x ,
    const CppAD::vector<bool>&     depend_y )
{
    //
    // n_left, n_middle, n_right
    size_t n_left, n_middle, n_right;
    get(call_id, n_left, n_middle, n_right);
# ifndef NDEBUG
    // n, m
    size_t n     = depend_x.size();
    size_t m     = depend_y.size();
    //
    // check sizes
    assert( n == n_left * n_middle + n_middle * n_right );
    assert( m == n_left * n_right );
# endif
    //
    // offset
    size_t offset = n_left * n_middle;
    //
    // type_y
    // y[ i * n_right + j] = sum_k
    //      x[i * n_middle + k] * x[ offset + k * n_right + j]
    // type_y
    for(size_t i = 0; i < n_left; ++i)
    {   for(size_t j = 0; j < n_right; ++j)
        {   size_t ij = i * n_right + j;
            if( depend_y[ij] )
            {   for(size_t k = 0; k < n_middle; ++k)
                {   size_t ik = i * n_middle + k;
                    size_t kj = offset + k * n_right + j;
                    depend_x[ik] = true;
                    depend_x[kj] = true;
                }
            }
        }
    }
    return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
