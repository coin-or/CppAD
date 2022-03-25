# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_HES_SPARSITY_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_HES_SPARSITY_HPP
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
$begin atomic_four_vector_hes_sparsity.hpp$$
$spell
    Jacobian
    hes
$$

$section Atomic Vector Hessian Sparsity Pattern: Example Implementation$$

$head Purpose$$
The $code hes_sparsity$$ routine overrides the virtual functions
used by the atomic_four base class for Jacobian sparsity calculations; see
$cref/hes_sparsity/atomic_four_hes_sparsity/$$.

$head Example$$
The file $cref atomic_four_vector_hes_sparsity.cpp$$
contains an example and test using this operator.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/example/atomic_four/vector/vector.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// hes_sparsity override
template <class Base>
bool atomic_vector<Base>::hes_sparsity(
    size_t                                         call_id      ,
    const CppAD::vector<bool>&                     select_x     ,
    const CppAD::vector<bool>&                     select_y     ,
    CppAD::sparse_rc< CppAD::vector<size_t> >&     pattern_out  )
{
    size_t n = select_x.size();
    size_t m = select_y.size();
    assert( n == m || n == 2 * m );
    //
    // op
    op_enum_t op = op_enum_t( call_id );
    //
    switch(op)
    {   // linear operator cases
        case add_enum:
        case sub_enum:
        case neg_enum:
        //
        // pattern_out is empty
        pattern_out.resize(n, n, 0);
        return true;

        default:
        break;
    }
    //
    // nnz
    // number of non-zeros in sparsity pattern
    size_t nnz = 0;
    for(size_t i = 0; i < m; ++i) if( select_y[i] )
    {   size_t j = i;
        if( select_x[j] && op != mul_enum )
            ++nnz;
        if( n != m )
        {   // binary operator
            j = m + i;
            if( select_x[j] )
                nnz += 2;
        }
    }
    //
    // pattern_out
    pattern_out.resize(n, n, nnz);
    size_t k = 0;
    for(size_t i = 0; i < m; ++i) if( select_y[i] )
    {   size_t j = i;
        if( select_x[j] && op != mul_enum )
            pattern_out.set(k++, i, j);
        if( n != m )
        {   // binary operator
            j = m + i;
            if( select_x[j] )
            {   pattern_out.set(k++, i, j);
                pattern_out.set(k++, j, i);
            }
        }
    }
    assert( k == nnz);
    //
    return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
