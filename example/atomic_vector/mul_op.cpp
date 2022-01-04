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
$begin atomic_vector_mul_op.cpp$$

$section Atomic Vector Multiply Operator: Example Implementation$$

$head Forward Mode$$
see theory for forward mode
$cref/multiplication/ForwardTheory/Binary Operators/Multiplication/$$.

$head Example$$
The file $cref atomic_vector_mul.cpp$$ contains an example
and test for this operator.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include "atomic_vector.hpp"

void atomic_vector::forward_mul(
    size_t                                           m,
    size_t                                           p,
    size_t                                           q,
    const CppAD::vector<double>&                     tx,
    CppAD::vector<double>&                           ty)
{
    for(size_t i = 0; i < m; ++i)
    {   for(size_t k = p; k <= q; ++k)
        {   size_t y_index = i * (q+1) + k;
            // y^k = 0
            ty[y_index]    = 0.0;
            for(size_t d = 0; d <= k; d++)
            {   size_t u_index  = (1 + i)     * (q+1) + (k-d);
                size_t v_index  = (1 + m + i) * (q+1) + d;
                // y^k += u^{k-d} * v^d
                ty[y_index]    += tx[u_index] * tx[v_index];
            }
        }
    }
}
void atomic_vector::forward_mul(
    size_t                                           m,
    size_t                                           p,
    size_t                                           q,
    const CppAD::vector< CppAD::AD<double> >&        atx,
    CppAD::vector< CppAD::AD<double> >&              aty)
{
    size_t n = 2 * m + 1;
    //
    CppAD::vector< CppAD::AD<double> > ax_mul(n), ax_add(n), ay(m);
    ax_mul[0] = CppAD::AD<double>( mul_enum );
    ax_add[0] = CppAD::AD<double>( add_enum );
    for(size_t k = p; k <= q; ++k)
    {   // ay = 0
        for(size_t i = 0; i < m; ++i)
            ay[i] = 0.0;
        for(size_t d = 0; d <= k; d++)
        {   // u_add = ay
            for(size_t i = 0; i < m; ++i)
                ax_add[1 + i] = ay[i];
            //
            // au_mul = u^{k-d}
            copy_atx_to_au(m, q, k-d, atx, ax_mul);
            //
            // av_mul =  v^d
            copy_atx_to_av(m, q, d, atx, ax_mul);
            //
            // ay = au_mul * av_mul
            (*this)(ax_mul, ay); // atomic vector multiply
            //
            // v_add = ay
            for(size_t i = 0; i < m; ++i)
                ax_add[1 + m + i] = ay[i];
            //
            // ay = u_add + v_add
            (*this)(ax_add, ay); // atomic vector add
        }
        // y^k = ay
        copy_ay_to_aty(m, q, k, ay, aty);
    }
}
// END C++
