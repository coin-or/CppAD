# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_JAC_SPARSITY_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_JAC_SPARSITY_HPP
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
$begin atomic_four_mat_mul_jac_sparsity.hpp$$
$spell
    Jacobian
    jac
$$

$nospell
$bold This is cppad-20221105 documentation$$. Here is a link to its
$href%https://cppad.readthedocs.io%current documentation%$$.
$$
$section
Atomic Matrix Multiply Jacobian Sparsity Pattern: Example Implementation
$$

$head Purpose$$
The $code jac_sparsity$$ routine overrides the virtual functions
used by the atomic_four base class for Jacobian sparsity calculations; see
$cref/jac_sparsity/atomic_four_jac_sparsity/$$.

$head Example$$
The file $cref atomic_four_mat_mul_sparsity.cpp$$
contains an example and test using this operator.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/example/atomic_four/mat_mul/mat_mul.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// jac_sparsity override
template <class Base>
bool atomic_mat_mul<Base>::jac_sparsity(
    size_t                                         call_id      ,
    bool                                           dependency   ,
    const CppAD::vector<bool>&                     ident_zero_x ,
    const CppAD::vector<bool>&                     select_x     ,
    const CppAD::vector<bool>&                     select_y     ,
    CppAD::sparse_rc< CppAD::vector<size_t> >&     pattern_out  )
{
    // n_left, n_middle, n_right
    size_t n_left, n_middle, n_right;
    get(call_id, n_left, n_middle, n_right);
    //
    // n, m
    size_t n     = select_x.size();
    size_t m     = select_y.size();
    //
    // check sizes
    assert( n == n_middle * ( n_left +  n_right ) );
    assert( m == n_left * n_right );
    //
    // offset
    size_t offset = n_left * n_middle;
    //
    // pattern_out
    pattern_out.resize(m, n, 0);
    for(size_t i = 0; i < n_left; ++i)
    {   for(size_t j = 0; j < n_right; ++j)
        {   size_t ij = i * n_right + j;               // C_{i,j} = y[ij]
            if( select_y[ij] ) for(size_t k = 0; k < n_middle; ++k)
            {   size_t ik = i * n_middle + k;          // A_{i,k} = x[ik]
                size_t kj = offset + k * n_right + j;  // B_{k,j} = x[kj]
                if( select_x[ik] && ! ident_zero_x[kj] )
                    pattern_out.push_back(ij, ik);
                if( select_x[kj] && ! ident_zero_x[ij] )
                    pattern_out.push_back(ij, kj);
            }
        }
    }
    //
    return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
