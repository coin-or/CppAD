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
$begin atomic_vector_sub_op.cpp$$

$section Atomic Vector Subtract Operator: Example Implementation$$

$head Forward Mode$$
see theory for forward mode
$cref/subtraction/ForwardTheory/Binary Operators/Subtraction/$$.

$head Example$$
The file $cref atomic_vector_sub.cpp$$ contains an example
and test for this operator.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include "atomic_vector.hpp"

void atomic_vector::forward_sub(
    size_t                                           m,
    size_t                                           p,
    size_t                                           q,
    const CppAD::vector<double>&                     tx,
    CppAD::vector<double>&                           ty)
{
    for(size_t k = p; k <= q; ++k)
    {   for(size_t i = 0; i < m; ++i)
        {   size_t u_index  = (1 + i)     * (q+1) + k;
            size_t v_index  = (1 + m + i) * (q+1) + k;
            size_t y_index  = i *           (q+1) + k;
            // y_i^k = u_i^k - v_i^k
            ty[y_index]     = tx[u_index] - tx[v_index];
        }
    }
}
void atomic_vector::forward_sub(
    size_t                                           m,
    size_t                                           p,
    size_t                                           q,
    const CppAD::vector< CppAD::AD<double> >&        atx,
    CppAD::vector< CppAD::AD<double> >&              aty)
{
    size_t n = 2 * m + 1;
    assert( atx.size() == n * (q+1) );
    assert( aty.size() == m * (q+1) );
    //
    // atx
    const CppAD::AD<double>* atu = atx.data() + (q+1);
    const CppAD::AD<double>* atv = atu + m * (q+1);
    //
    // ax
    CppAD::vector< CppAD::AD<double> > ax(n);
    ax[0] = CppAD::AD<double>( sub_enum );
    CppAD::AD<double>* au = ax.data() + 1;
    CppAD::AD<double>* av = ax.data() + 1 + m;
    //
    // ay
    CppAD::vector< CppAD::AD<double> > ay(m);
    //
    for(size_t k = p; k <= q; ++k)
    {   // au = u^k
        copy_mat_to_vec(m, q, k, atu, au);
        // av = v^k
        copy_mat_to_vec(m, q, k, atv, av);
        // ay = au - av
        (*this)(ax, ay); // atomic vector sub
        // y^k = ay
        copy_vec_to_mat(m, q, k, ay.data(), aty.data());
    }
}
// END C++
