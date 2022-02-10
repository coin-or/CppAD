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
$begin atomic_four_vector_neg.cpp$$

$section Atomic Vector Negation Example$$

$head f(u, w)$$
For this example,
$latex f : \B{R}^{2m} \rightarrow \B{R}^m$$
is defined by $latex f(u, w) = u - w$$.
where $icode u$$ and $icode w$$ are in $latex \B{R}^m$$.

$head g(u, w)$$
For this example $latex g : \B{R}^{2m} \rightarrow \B{R}^m$$
is defined by $latex g_i (u, w) = \partial_{w[i]}  f_i (u, w)$$

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <cppad/example/atomic_four/vector/vector.hpp>
bool neg(void)
{   bool ok = true;
    using CppAD::NearEqual;
    using CppAD::AD;
    double eps99 = 99.0 * CppAD::numeric_limits<double>::epsilon();
    //
    // vec_op
    // atomic vector_op object
    CppAD::atomic_vector<double> vec_op("atomic_vector");
    //
    // m
    // size of u, v
    size_t m = 5;
    //
    // add_op, neg_op
    typedef CppAD::atomic_vector<double>::op_enum_t op_enum_t;
    op_enum_t add_op = CppAD::atomic_vector<double>::add_enum;
    op_enum_t neg_op = CppAD::atomic_vector<double>::neg_enum;
    // -----------------------------------------------------------------------
    // Record f(u, v) = u - v
    // -----------------------------------------------------------------------
    // Independent variable vector
    CPPAD_TESTVECTOR( CppAD::AD<double> ) auv(2 * m);
    for(size_t j = 0; j < 2 * m; ++j)
        auv[j] = AD<double>(1 + j);
    CppAD::Independent(auv);
    //
    // au, av
    CPPAD_TESTVECTOR( CppAD::AD<double> ) au(m), av(m);
    for(size_t i = 0; i < m; ++i)
    {   au[i] = auv[0 * m + i];
        av[i] = auv[1 * m + i];
    }
    //
    // ax = av
    CPPAD_TESTVECTOR( CppAD::AD<double> ) ax(m);
    for(size_t i = 0; i < m; ++i)
        ax[i] = av[i];
    //
    // ay = - av
    CPPAD_TESTVECTOR( CppAD::AD<double> ) ay(m);
    vec_op(neg_op, ax, ay);
    //
    // ax = (au, ay)
    ax.resize(2 * m);
    for(size_t i = 0; i < m; ++i)
    {   ax[i]     = au[i];
        ax[m + i] = ay[i];
    }
    //
    // az = au + ay
    CPPAD_TESTVECTOR( CppAD::AD<double> ) az(m);
    vec_op(add_op, ax, az);
    //
    // f
    CppAD::ADFun<double> f(auv, az);
    // -----------------------------------------------------------------------
    // check forward mode on f
    // -----------------------------------------------------------------------
    //
    // uv, duv
    CPPAD_TESTVECTOR(double) uv(2 * m), duv(2 * m);
    for(size_t j = 0; j < 2 * m; ++j)
    {   uv[j]  = double(1 + j);
        duv[j] = double(j);
    }
    //
    // z, dz
    CPPAD_TESTVECTOR(double) z(m), dz(m);
    z  = f.Forward(0, uv);
    dz = f.Forward(1, duv);
    //
    // ok
    for(size_t i = 0; i < m; ++i)
    {   double check_z  = uv[0 * m + i] - uv[1 * m + i];
        ok             &= NearEqual( z[i] ,  check_z,  eps99, eps99);
        double check_dz = double(0 * m + i) - double(1 * m + i);
        ok             &= NearEqual( dz[i] ,  check_dz,  eps99, eps99);
    }
    // -----------------------------------------------------------------------
    // Record g_i (u, v) = \partial d/dv[i] f_i (u , v)
    // -----------------------------------------------------------------------
    //
    // af
    CppAD::ADFun< AD<double>, double > af = f.base2ad();
    //
    // auv
    CppAD::Independent(auv);
    //
    // aduv
    CPPAD_TESTVECTOR( AD<double> ) aduv(2 * m);
    for(size_t i = 0; i < m; ++i)
    {   aduv[0 * m + i]  = 0.0; // du[i]
        aduv[1 * m + i]  = 1.0; // dv[i]
    }
    //
    // az
    // use the fact that d_v[i] f_k (u, v) is zero when i != k
    af.Forward(0, auv);
    az = af.Forward(1, aduv);
    CppAD::ADFun<double> g(auv, az);
    // -----------------------------------------------------------------------
    // check forward mode on g
    // -----------------------------------------------------------------------
    //
    // z
    z = g.Forward(0, uv);
    //
    // ok
    for(size_t i = 0; i < m; ++i)
    {   double check_z  = - 1.0;
        ok             &= NearEqual( z[i] ,  check_z,  eps99, eps99);
    }
    return ok;
}
// END C++
