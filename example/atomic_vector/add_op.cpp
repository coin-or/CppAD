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
$begin atomic_vector_add_op.cpp$$

$section Atomic Vector Add Operator: Example Implementation$$

$head Forward Mode$$
see theory for forward mode
$cref/addition/ForwardTheory/Binary Operators/Addition/$$.

$head Example$$
The file $cref atomic_vector_add.cpp$$ contains an example
and test for this operator.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include "atomic_vector.hpp"

// ---------------------------------------------------------------------------
// BEGIN forward_add
void atomic_vector::forward_add(
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
            // y_i^k = u_i^k + v_i^k
            ty[y_index]     = tx[u_index] + tx[v_index];
        }
    }
}
void atomic_vector::forward_add(
    size_t                                           n,
    size_t                                           p,
    size_t                                           q,
    const CppAD::vector< CppAD::AD<double> >&        atx,
    CppAD::vector< CppAD::AD<double> >&              aty)
{   assert( n % 2 == 1 );
    size_t m = (n - 1) / 2;
    //
    CppAD::vector< CppAD::AD<double> > ax(n), ay(m);
    ax[0] = CppAD::AD<double>( add_enum );
    for(size_t k = p; k <= q; ++k)
    {   // au = u^k
        copy_atx_to_au(n, q, k, atx, ax);
        // av = v^k
        copy_atx_to_av(n, q, k, atx, ax);
        // ay = au + av
        (*this)(ax, ay); // atomic vector add
        // y^k = ay
        copy_ay_to_aty(n, q, k, ay, aty);
    }
}
// END forward_add
// ---------------------------------------------------------------------------
// reverse_add
void atomic_vector::reverse_add(
    size_t                                           n,
    size_t                                           q,
    const CppAD::vector<double>&                     tx,
    const CppAD::vector<double>&                     ty,
    CppAD::vector<double>&                           px,
    const CppAD::vector<double>&                     py)
{   assert( n % 2 == 1 );
    size_t m = (n - 1) / 2;
    //
    for(size_t k = 0; k <= q; ++k)
    {   for(size_t i = 0; i < m; ++i)
        {   size_t u_index  = (1 + i)     * (q+1) + k;
            size_t v_index  = (1 + m + i) * (q+1) + k;
            size_t y_index  = i *           (q+1) + k;
            // y_i^k = u_i^k + v_i^k
            px[u_index] = py[y_index];
            px[v_index] = py[y_index];
        }
    }
}

// END C++
