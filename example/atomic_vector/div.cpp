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

$section Atomic Vector Division Example$$

$head f(u, v, w)$$
For this example,
$latex f : \B{R}^{2m} \rightarrow \B{R}^m$$
is defined by $latex f(u, v) = u * v / u$$.
where $icode u$$ and $icode v$$ are in $latex \B{R}^m$$.

$head g(u, v)$$
For this example $latex g : \B{R}^{2m} \rightarrow \B{R}^m$$
is defined by $latex g_i (u, v) = \partial_{v[i]}  f_i (u, v)$$

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include "atomic_vector.hpp"
bool div(void)
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
    // size of u and v
    size_t m = 2;
    //
    // mul_op, div_op
    typedef atomic_vector::op_enum_t op_enum_t;
    op_enum_t mul_op = atomic_vector::mul_enum;
    op_enum_t div_op = atomic_vector::div_enum;
    // -----------------------------------------------------------------------
    // Record f(u, v) = u * v / u
    // -----------------------------------------------------------------------
    // Independent variable vector
    CPPAD_TESTVECTOR( CppAD::AD<double> ) auv(2 * m);
    for(size_t j = 0; j < 2 * m; ++j)
        auv[j] = AD<double>(1 + j);
    CppAD::Independent(auv);
    //
    // au, av, aw
    CPPAD_TESTVECTOR( CppAD::AD<double> ) au(m), av(m);
    for(size_t i = 0; i < m; ++i)
    {   au[i] = auv[0 * m + i];
        av[i] = auv[1 * m + i];
    }
    //
    // ax = (mul_op, au, av)
    CPPAD_TESTVECTOR( CppAD::AD<double> ) ax(1 + 2 * m);
    ax[0] = CppAD::AD<double>(mul_op);
    for(size_t i = 0; i < m; ++i)
    {   ax[1 + i]     = au[i];
        ax[1 + m + i] = av[i];
    }
    //
    // ay = u * v
    CPPAD_TESTVECTOR( CppAD::AD<double> ) ay(m);
    vec_op(ax, ay);
    //
    // ax = (div_op, ay, au)
    ax[0] = CppAD::AD<double>(div_op);
    for(size_t i = 0; i < m; ++i)
    {   ax[1 + i]     = ay[i];
        ax[1 + m + i] = au[i];
    }
    //
    // az = au / ay
    CPPAD_TESTVECTOR( CppAD::AD<double> ) az(m);
    vec_op(ax, az);
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
    {   double vi   = uv[1 * m + i];
        ok         &= NearEqual( z[i] ,  vi,  eps99, eps99);
        double dvi  = double(1 * m + i);
        ok         &= NearEqual( dz[i] ,  dvi,  eps99, eps99);
    }
    // -----------------------------------------------------------------------
    // Record g_i (u, v) = \partial d/du[i] f_i (u, v)
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
    // use the fact that d_v[i] f_k (u, v, w) is zero when i != k
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
    {   double check  = 1.0;
        ok           &= NearEqual( z[i] ,  check,  eps99, eps99);
    }
    return ok;
}
// END C++
