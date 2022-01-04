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
$begin atomic_vector_add.cpp$$

$section Atomic Vector Addition Example$$

$head f(u, v, w)$$
For this example,
$latex f : \B{R}^{3m} \rightarrow \B{R}^m$$
is defined by $latex f(u, v, w) = u + v + w$$.
where $icode u$$, $icode v$$, and $icode w$$ are in $latex \B{R}^m$$.

$head g(u, v, w)$$
For this example $latex g : \B{R}^{3m} \rightarrow \B{R}^m$$
is defined by $latex g_i (u, v, w) = \partial_{v[i]}  f_i (u, v, w)$$

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include "atomic_vector.hpp"
bool add(void)
{   bool ok = true;
    using CppAD::NearEqual;
    using CppAD::AD;
    double eps99 = 99.0 * CppAD::numeric_limits<double>::epsilon();
    //
    // vec_op
    // atomic vector_op object
    atomic_vector vec_op("atomic_vector");
    //
    // m
    // size of u, v, and w
    size_t m = 5;
    //
    // add_op
    typedef atomic_vector::op_enum_t op_enum_t;
    op_enum_t add_op = atomic_vector::add_enum;
    // -----------------------------------------------------------------------
    // Record f(u, v, w) = u + v + w
    // -----------------------------------------------------------------------
    // Independent variable vector
    CPPAD_TESTVECTOR( CppAD::AD<double> ) auvw(3 * m);
    for(size_t j = 0; j < 3 * m; ++j)
        auvw[j] = AD<double>(1 + j);
    CppAD::Independent(auvw);
    //
    // au, av, aw
    CPPAD_TESTVECTOR( CppAD::AD<double> ) au(m), av(m), aw(m);
    for(size_t i = 0; i < m; ++i)
    {   au[i] = auvw[0 * m + i];
        av[i] = auvw[1 * m + i];
        aw[i] = auvw[2 * m + i];
    }
    //
    // ax = (add_op, au, av)
    CPPAD_TESTVECTOR( CppAD::AD<double> ) ax(1 + 2 * m);
    ax[0] = CppAD::AD<double>(add_op);
    for(size_t i = 0; i < m; ++i)
    {   ax[1 + i]     = au[i];
        ax[1 + m + i] = av[i];
    }
    //
    // ay = u + v
    CPPAD_TESTVECTOR( CppAD::AD<double> ) ay(m);
    vec_op(ax, ay);
    //
    // ax = (add_op, ay, aw)
    for(size_t i = 0; i < m; ++i)
    {   ax[1 + i]     = ay[i];
        ax[1 + m + i] = aw[i];
    }
    //
    // az = ay + v
    CPPAD_TESTVECTOR( CppAD::AD<double> ) az(m);
    vec_op(ax, az);
    //
    // f
    CppAD::ADFun<double> f(auvw, az);
    // -----------------------------------------------------------------------
    // check forward mode on f
    // -----------------------------------------------------------------------
    //
    // uvw, duvw
    CPPAD_TESTVECTOR(double) uvw(3 * m), duvw(3 * m);
    for(size_t j = 0; j < 3 * m; ++j)
    {   uvw[j]  = double(1 + j);
        duvw[j] = double(j);
    }
    //
    // z, dz
    CPPAD_TESTVECTOR(double) z(m), dz(m);
    z  = f.Forward(0, uvw);
    dz = f.Forward(1, duvw);
    //
    // ok
    for(size_t i = 0; i < m; ++i)
    {   double check_z  = uvw[0 * m + i] + uvw[1 * m + i] + uvw[2 * m + i];
        ok             &= NearEqual( z[i] ,  check_z,  eps99, eps99);
        double check_dz = double( (0 * m + i)  + (1 * m + i) + (2 * m + i) );
        ok             &= NearEqual( dz[i] ,  check_dz,  eps99, eps99);
    }
    // -----------------------------------------------------------------------
    // Record g_i (u, v, w) = \partial d/dv[i] f_i (u + v + w)
    // -----------------------------------------------------------------------
    //
    // af
    CppAD::ADFun< AD<double>, double > af = f.base2ad();
    //
    // auvw
    CppAD::Independent(auvw);
    //
    // aduvw
    CPPAD_TESTVECTOR( AD<double> ) aduvw(3 * m);
    for(size_t i = 0; i < m; ++i)
    {   aduvw[0 * m + i]  = 0.0; // du[i]
        aduvw[1 * m + i]  = 1.0; // dv[i]
        aduvw[2 * m + i]  = 0.0; // dw[i]
    }
    //
    // az
    // use the fact that d_v[i] f_k (u, v, w) is zero when i != k
    af.Forward(0, auvw);
    az = af.Forward(1, aduvw);
    CppAD::ADFun<double> g(auvw, az);
    // -----------------------------------------------------------------------
    // check forward mode on g
    // -----------------------------------------------------------------------
    //
    // z
    z = g.Forward(0, uvw);
    //
    // ok
    for(size_t i = 0; i < m; ++i)
    {   double check_z  = 1.0;
        ok             &= NearEqual( z[i] ,  check_z,  eps99, eps99);
    }
    return ok;
}
// END C++
