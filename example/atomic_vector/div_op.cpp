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
$begin atomic_vector_div_op.cpp$$

$section Atomic Vector Divide Operator: Example Implementation$$

$head Forward Mode$$
see theory for forward mode
$cref/division/ForwardTheory/Binary Operators/Division/$$.

$head Example$$
The file $cref atomic_vector_div.cpp$$ contains an example
and test for this operator.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include "atomic_vector.hpp"

void atomic_vector::forward_div(
    size_t                                           m,
    size_t                                           p,
    size_t                                           q,
    const CppAD::vector<double>&                     tx,
    CppAD::vector<double>&                           ty)
{
    for(size_t i = 0; i < m; ++i)
    {   for(size_t k = p; k <= q; ++k)
        {   size_t y_index  = i *       (q+1) + k;
            size_t u_index  = (1 + i) * (q+1) + k;
            // y^k = u^k
            ty[y_index]     = tx[u_index];
            for(size_t d = 1; d <= k; d++)
            {   size_t y_other      = i       * (q+1) + (k-d);
                size_t v_index  = (1 + m + i) * (q+1) + d;
                // y^k -= y^{k-d} * v^d
                ty[y_index] -= ty[y_other] * tx[v_index];
            }
            size_t v_index = (1 + m + i ) * (q+1) + 0;
            // y^k /= v^0
            ty[y_index] /= tx[v_index];
        }
    }
}
void atomic_vector::forward_div(
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
    // atu, atv
    const CppAD::AD<double>* atu = atx.data() + (q+1);
    const CppAD::AD<double>* atv = atu + m * (q+1);
    //
    // ax_div
    CppAD::vector< CppAD::AD<double> > ax_div(n);
    ax_div[0] = CppAD::AD<double>( div_enum );
    CppAD::AD<double>* au_div = ax_div.data() + 1;
    CppAD::AD<double>* av_div = ax_div.data() + 1 + m;
    //
    // ax_mul
    CppAD::vector< CppAD::AD<double> > ax_mul(n);
    ax_mul[0] = CppAD::AD<double>( mul_enum );
    CppAD::AD<double>* au_mul = ax_mul.data() + 1;
    CppAD::AD<double>* av_mul = ax_mul.data() + 1 + m;
    //
    // ax_sub
    CppAD::vector< CppAD::AD<double> > ax_sub(n);
    ax_sub[0] = CppAD::AD<double>( sub_enum );
    CppAD::AD<double>* au_sub = ax_sub.data() + 1;
    CppAD::AD<double>* av_sub = ax_sub.data() + 1 + m;
    //
    // ay
    CppAD::vector< CppAD::AD<double> > ay(m);
    //
    for(size_t k = p; k <= q; ++k)
    {   // u_sub = u^k
        copy_mat_to_vec(m, q, k, atu, au_sub);
        for(size_t d = 1; d <= k; d++)
        {   // u_mul = y^{k-d}
            copy_mat_to_vec(m, q, k-d, aty.data(), au_mul);
            // v_mul = v^d
            copy_mat_to_vec(m, q, d, atv, av_mul);
            // ay = u_mul * v_mul
            (*this)(ax_mul, ay); // atomic vector multiply
            // v_sub = ay
            for(size_t i = 0; i < m; ++i)
                av_sub[i] = ay[i];
            // ay = u_sub - v_sub
            (*this)(ax_sub, ay); // atomic vector subtract
            // u_sub = ay
            for(size_t i = 0; i < m; ++i)
                au_sub[i] = ay[i];
        }
        // u_div = u_sub
        for(size_t i = 0; i < m; ++i)
            au_div[i] = au_sub[i];
        // v_div = v^0
        copy_mat_to_vec(m, q, 0, atv, av_div);
        // ay = u_div / v_div
        (*this)(ax_div, ay); // atomic vector divide
        // y^k = ay
        copy_vec_to_mat(m, q, k, ay.data(), aty.data());
    }
}
// END C++
