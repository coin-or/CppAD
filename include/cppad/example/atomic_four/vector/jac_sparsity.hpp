# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_JAC_SPARSITY_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_JAC_SPARSITY_HPP
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
$begin atomic_four_vector_jac_sparsity.hpp$$
$spell
    Jacobian
    jac
$$

$nospell
$bold This is old CppAD documentation:$$ Here is a link to its
$href%http://coin-or.github.io/CppAD%current documentation%$$.
$$
$section Atomic Vector Jacobian Sparsity Pattern: Example Implementation$$

$head Purpose$$
The $code jac_sparsity$$ routine overrides the virtual functions
used by the atomic_four base class for Jacobian sparsity calculations; see
$cref/jac_sparsity/atomic_four_jac_sparsity/$$.

$head Example$$
The file $cref atomic_four_vector_jac_sparsity.cpp$$
contains an example and test using this operator.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/example/atomic_four/vector/vector.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// jac_sparsity override
template <class Base>
bool atomic_vector<Base>::jac_sparsity(
    size_t                                         call_id      ,
    bool                                           dependency   ,
    const CppAD::vector<bool>&                     ident_zero_x ,
    const CppAD::vector<bool>&                     select_x     ,
    const CppAD::vector<bool>&                     select_y     ,
    CppAD::sparse_rc< CppAD::vector<size_t> >&     pattern_out  )
{
    size_t n = select_x.size();
    size_t m = select_y.size();
    assert( n == m || n == 2 * m );
    //
    // nnz
    // number of non-zeros in sparsity pattern
    size_t nnz = 0;
    for(size_t i = 0; i < m; ++i) if( select_y[i] )
    {   size_t j = i;
        if( select_x[j] )
            ++nnz;
        if( n != m )
        {   // binary operator
            j = m + i;
            if( select_x[j] )
                ++nnz;
        }
    }
    //
    // pattern_out
    pattern_out.resize(m, n, nnz);
    size_t k = 0;
    for(size_t i = 0; i < m; ++i) if( select_y[i] )
    {   size_t j = i;
        if( select_x[j] )
            pattern_out.set(k++, i, j);
        if( n != m )
        {   // binary operator
            j = m + i;
            if( select_x[j] )
                pattern_out.set(k++, i, j);
        }
    }
    assert( k == nnz);
    //
    return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
