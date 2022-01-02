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
$begin atomic_vector_div.cpp$$

$section Atomic Vector Divide Operator: Example Implementation$$

$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include "atomic_vector.hpp"

void atomic_vector::forward_div(
    size_t                                           n,
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
    size_t                                           n,
    size_t                                           m,
    size_t                                           p,
    size_t                                           q,
    const CppAD::vector< CppAD::AD<double> >&        atx,
    CppAD::vector< CppAD::AD<double> >&              aty)
{   CppAD::vector< CppAD::AD<double> > ax_div(n), ax_mul(n), ax_sub(n), ay(m);
    ax_div[0] = CppAD::AD<double>( div_enum );
    ax_mul[0] = CppAD::AD<double>( mul_enum );
    ax_sub[0] = CppAD::AD<double>( sub_enum );
    for(size_t k = p; k <= q; ++k)
    {   // u_sub = u^k
        copy_atx_to_au(n, m, q, k, atx, ax_sub);
        for(size_t d = 1; d <= k; d++)
        {   // u_mul = y^{k-d}
            copy_aty_to_au(n, m, q, k-d, aty, ax_mul);
            // v_mul = v^d
            copy_atx_to_av(n, m, q, d, atx, ax_mul);
            // ay = u_mul * v_mul
            (*this)(ax_mul, ay); // atomic vector multiply
            // v_sub = ay
            for(size_t i = 0; i < m; ++i)
                ax_sub[1 + m + i] = ay[i];
            // ay = u_sub - v_sub
            (*this)(ax_sub, ay); // atomic vector subtract
            // u_sub = ay
            for(size_t i = 0; i < m; ++i)
                ax_sub[1 + i] = ay[i];
        }
        // u_div = u_sub
        for(size_t i = 0; i < m; ++i)
            ax_div[1 + i] = ax_sub[1 + i];
        // v_div = v^0
        copy_atx_to_av(n, m, q, 0, atx, ax_div);
        // ay = u_div / v_div
        (*this)(ax_div, ay); // atomic vector divide
        // y^k = ay
        copy_ay_to_aty(n, m, q, k, ay, aty);
    }
}
// END C++
