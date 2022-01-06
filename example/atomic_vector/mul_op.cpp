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

// --------------------------------------------------------------------------
// forward_mul
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
    assert( atx.size() == n * (q+1) );
    assert( aty.size() == m * (q+1) );
    //
    // atu, atv
    ad_vector::const_iterator atu = atx.begin() + (q+1);
    ad_vector::const_iterator atv = atu + m * (q+1);
    //
    // ax_mul
    ad_vector ax_mul(n);
    ax_mul[0] = CppAD::AD<double>( mul_enum );
    ad_vector::iterator au_mul = ax_mul.begin() + 1;
    ad_vector::iterator av_mul = ax_mul.begin() + 1 + m;
    //
    // ax_add
    ad_vector ax_add(n);
    ax_add[0] = CppAD::AD<double>( add_enum );
    ad_vector::iterator au_add = ax_add.begin() + 1;
    ad_vector::iterator av_add = ax_add.begin() + 1 + m;
    //
    // ay
    ad_vector ay(m);
    //
    for(size_t k = p; k <= q; ++k)
    {   // ay = 0
        for(size_t i = 0; i < m; ++i)
            ay[i] = 0.0;
        for(size_t d = 0; d <= k; d++)
        {   // u_add = ay
            for(size_t i = 0; i < m; ++i)
                au_add[i] = ay[i];
            //
            // au_mul = u^{k-d}
            copy_mat_to_vec(m, q, k-d, atu, au_mul);
            //
            // av_mul =  v^d
            copy_mat_to_vec(m, q, d, atv, av_mul);
            //
            // ay = au_mul * av_mul
            (*this)(ax_mul, ay); // atomic vector multiply
            //
            // v_add = ay
            for(size_t i = 0; i < m; ++i)
                av_add[i] = ay[i];
            //
            // ay = u_add + v_add
            (*this)(ax_add, ay); // atomic vector add
        }
        // y^k = ay
        copy_vec_to_mat(m, q, k, ay.begin(), aty.begin());
    }
}
// --------------------------------------------------------------------------
// reverse_mul
void atomic_vector::reverse_mul(
    size_t                                           m,
    size_t                                           q,
    const CppAD::vector<double>&                     tx,
    const CppAD::vector<double>&                     ty,
    CppAD::vector<double>&                           px,
    const CppAD::vector<double>&                     py)
{   size_t n = 1 + 2 * m;
    assert( tx.size() == n * (q+1) );
    assert( ty.size() == m * (q+1) );
    assert( px.size() == n * (q+1) );
    assert( py.size() == m * (q+1) );
    //
    // px
    for(size_t j = 0; j < n; ++j)
    {   for(size_t k = 0; k <= q; ++k)
            px[j * (q+1) + k] = 0.0;
    }
    //
    // px
    for(size_t i = 0; i < m; ++i)
    {   // k
        size_t k = q + 1;
        while(k)
        {   --k;
            //
            // y_index
            size_t y_index = i * (q+1) + k;
            //
            // px
            for(size_t d = 0; d <= k; ++d)
            {   size_t u_index  = (1 + i)     * (q+1) + (k-d);
                size_t v_index  = (1 + m + i) * (q+1) + d;
                //
                // must use azmul becasue py[y_index] = 0 may mean that this
                // component of the function was not selected.
                px[u_index]    += CppAD::azmul( py[y_index] , tx[v_index] );
                px[v_index]    += CppAD::azmul( py[y_index] , tx[u_index] );
            }
        }
    }
}
void atomic_vector::reverse_mul(
    size_t                                           m,
    size_t                                           q,
    const CppAD::vector< CppAD::AD<double> >&        atx,
    const CppAD::vector< CppAD::AD<double> >&        aty,
    CppAD::vector< CppAD::AD<double> >&              apx,
    const CppAD::vector< CppAD::AD<double> >&        apy)
{   size_t n = 1 + 2 * m;
    assert( atx.size() == n * (q+1) );
    assert( aty.size() == m * (q+1) );
    assert( apx.size() == n * (q+1) );
    assert( apy.size() == m * (q+1) );
    //
    // atu, atv, apu, apv
    ad_vector::const_iterator atu = atx.begin() + (q+1);
    ad_vector::const_iterator atv = atu + m * (q+1);
    ad_vector::iterator       apu = apx.begin() + (q+1);
    ad_vector::iterator       apv = apu + m * (q+1);
    //
    // ax_mul
    // need azmul_op but it is not yet available
    ad_vector ax_mul(n);
    ax_mul[0] = CppAD::AD<double>( mul_enum );
    ad_vector::iterator au_mul = ax_mul.begin() + 1;
    ad_vector::iterator av_mul = ax_mul.begin() + 1 + m;
    //
    // ax_add
    ad_vector ax_add(n);
    ax_add[0] = CppAD::AD<double>( add_enum );
    ad_vector::iterator au_add = ax_add.begin() + 1;
    ad_vector::iterator av_add = ax_add.begin() + 1 + m;
    //
    // ay
    ad_vector ay(m);
    //
    // px
    // assigning to the value zero does not create operators on the tape
    for(size_t j = 0; j < n; ++j)
    {   for(size_t k = 0; k <= q; ++k)
            apx[j * (q+1) + k] = 0.0;
    }
    //
    // k
    size_t k = q + 1;
    while(k)
    {   --k;
        //
        // au_mul = apy^k
        copy_mat_to_vec(m, q, k, apy.begin(), au_mul);
        //
        // d
        for(size_t d = 0; d <=k; ++d)
        {   // -------------------------------------------------------------
            // reverse:
            //  px[v_index] += CppAD::azmul( py[y_index] , tx[u_index] );
            // -------------------------------------------------------------

            // av_mul = atu^{k-d}
            copy_mat_to_vec(m, q, k-d, atu, av_mul);
            //
            // ay = au_mul * av_mul
            (*this)(ax_mul, ay);
            //
            // au_add = ay
            for(size_t i = 0; i < m; ++i)
                au_add[i] = ay[i];
            //
            // av_add = apv^d
            copy_mat_to_vec(m, q, d, apv, av_add);
            //
            // ay = au_add + av_add
            (*this)(ax_add, ay);
            //
            // apv^d =  ay
            copy_vec_to_mat(m, q, d, ay.begin(), apv);
            // -------------------------------------------------------------
            // reverse:
            //  px[u_index] += CppAD::azmul( py[y_index] , tx[v_index] );
            // -------------------------------------------------------------
            // av_mul = atv^{k-d}
            copy_mat_to_vec(m, q, k-d, atv, av_mul);
            //
            // ay = au_mul * av_mul
            (*this)(ax_mul, ay);
            //
            // au_add = ay
            for(size_t i = 0; i < m; ++i)
                au_add[i] = ay[i];
            //
            // av_add = apu^d
            copy_mat_to_vec(m, q, d, apu, av_add);
            //
            // ay = au_add + av_add
            (*this)(ax_add, ay);
            //
            // apu^d =  ay
            copy_vec_to_mat(m, q, d, ay.begin(), apu);
        }
    }
}

// END C++
