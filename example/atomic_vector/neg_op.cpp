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
$begin atomic_vector_neg_op.cpp$$

$section Atomic Vector Negative Operator: Example Implementation$$

$head Example$$
The file $cref atomic_vector_neg.cpp$$ contains an example
and test for this operator.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include "atomic_vector.hpp"

// ---------------------------------------------------------------------------
void atomic_vector::forward_neg(
    size_t                                           m,
    size_t                                           p,
    size_t                                           q,
    const CppAD::vector<double>&                     tx,
    CppAD::vector<double>&                           ty)
{
    for(size_t k = p; k <= q; ++k)
    {   for(size_t i = 0; i < m; ++i)
        {   size_t u_index  = (1 + i) * (q+1) + k;
            size_t y_index  =       i * (q+1) + k;
            // y_i^k = - u_i^k
            ty[y_index] = - tx[u_index];
        }
    }
}
void atomic_vector::forward_neg(
    size_t                                           m,
    size_t                                           p,
    size_t                                           q,
    const CppAD::vector< CppAD::AD<double> >&        atx,
    CppAD::vector< CppAD::AD<double> >&              aty)
{   size_t n = m + 1;
    assert( atx.size() == n * (q+1) );
    assert( aty.size() == m * (q+1) );
    //
    // atu
    ad_vector::const_iterator atu = atx.begin() + difference_type(q+1);
    //
    // ax
    ad_vector ax(n);
    ax[0] = CppAD::AD<double>( neg_enum );
    ad_vector::iterator au = ax.begin() + 1;
    //
    // ay
    ad_vector ay(m);
    //
    for(size_t k = p; k <= q; ++k)
    {   // au = u^k
        copy_mat_to_vec(m, q, k, atu, au);
        // ay = - au
        (*this)(ax, ay); // atomic vector neg
        // y^k = ay
        copy_vec_to_mat(m, q, k, ay.begin(), aty.begin() );
    }
}
// ---------------------------------------------------------------------------
// reverse_neg
void atomic_vector::reverse_neg(
    size_t                                           m,
    size_t                                           q,
    const CppAD::vector<double>&                     tx,
    const CppAD::vector<double>&                     ty,
    CppAD::vector<double>&                           px,
    const CppAD::vector<double>&                     py)
{
    for(size_t k = 0; k <= q; ++k)
    {   for(size_t i = 0; i < m; ++i)
        {   size_t u_index  = (1 + i) * (q+1) + k;
            size_t y_index  =       i * (q+1) + k;
            // y_i^k = - u_i^k
            px[u_index] = - py[y_index];
        }
    }
}
void atomic_vector::reverse_neg(
    size_t                                           m,
    size_t                                           q,
    const CppAD::vector< CppAD::AD<double> >&        atx,
    const CppAD::vector< CppAD::AD<double> >&        aty,
    CppAD::vector< CppAD::AD<double> >&              apx,
    const CppAD::vector< CppAD::AD<double> >&        apy)
{   size_t n = m + 1;
    assert( atx.size() == n * (q+1) );
    assert( aty.size() == m * (q+1) );
    //
    // apu
    ad_vector::iterator apu = apx.begin() + difference_type(q+1);
    //
    // ax
    ad_vector ax(n);
    ax[0] = CppAD::AD<double>( neg_enum );
    ad_vector::iterator au = ax.begin() + 1;
    //
    // ay
    ad_vector ay(m);
    //
    for(size_t k = 0; k <= q; ++k)
    {   // au = py^k
        copy_mat_to_vec(m, q, k, apy.begin(), au);
        // ay = - au
        (*this)(ax, ay); // atomic vector neg
        // pu^k = ay
        copy_vec_to_mat(m, q, k, ay.begin(), apu);
    }
}

// END C++
