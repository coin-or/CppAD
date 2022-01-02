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
$begin atomic_vector_sub.cpp$$

$section Atomic Vector Subtract Operator: Example Implementation$$

$head Forward Mode$$
see theory for forward mode
$cref/subtraction/ForwardTheory/Binary Operators/Subtraction/$$.

$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include "atomic_vector.hpp"

void atomic_vector::forward_sub(
    size_t                                           n,
    size_t                                           p,
    size_t                                           q,
    const CppAD::vector<double>&                     tx,
    CppAD::vector<double>&                           ty)
{   assert( n % 2 == 1 );
    size_t m = (n - 1) / 2;
    //
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
    size_t                                           n,
    size_t                                           p,
    size_t                                           q,
    const CppAD::vector< CppAD::AD<double> >&        atx,
    CppAD::vector< CppAD::AD<double> >&              aty)
{   assert( n % 2 == 1 );
    size_t m = (n - 1) / 2;
    //
    CppAD::vector< CppAD::AD<double> > ax(n), ay(m);
    ax[0] = CppAD::AD<double>( sub_enum );
    for(size_t k = p; k <= q; ++k)
    {   // au = u^k , av = v^k
        copy_atx_to_ax(n, q, k, k, atx, ax);
        // ay = au - av
        (*this)(ax, ay); // atomic vector sub
        // y^k = ay
        copy_ay_to_aty(n, q, k, ay, aty);
    }
}
// END C++
