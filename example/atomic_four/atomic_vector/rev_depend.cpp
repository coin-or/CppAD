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
$begin atomic_vector_rev_depend.cpp$$

$section Example Optimizing Atomic Vector Usage$$

$head f(u, v)$$
For this example,
$latex f : \B{R}^{3m} \rightarrow \B{R}$$
is defined by $latex f(u, v, w) = - ( u_0 + v_0 ) * w_0$$.
where $icode u$$, $icode v$$, and $icode w$$ are in $latex \B{R}^m$$.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <cppad/example/atomic_four/atomic_vector.hpp>
bool rev_depend(void)
{   bool ok = true;
    using CppAD::NearEqual;
    using CppAD::AD;
    //
    // vec_op
    // atomic vector_op object
    CppAD::atomic_vector<double> vec_op("atomic_vector");
    //
    // m
    // size of u, v, and w
    size_t m = 6;
    //
    // n
    size_t n = 3 * m;
    //
    // add_op, mul_op, neg_op
    typedef CppAD::atomic_vector<double>::op_enum_t op_enum_t;
    op_enum_t add_op = CppAD::atomic_vector<double>::add_enum;
    op_enum_t mul_op = CppAD::atomic_vector<double>::mul_enum;
    op_enum_t neg_op = CppAD::atomic_vector<double>::neg_enum;
    // -----------------------------------------------------------------------
    // Record f(u, v, w) = - (u + v) * w
    // -----------------------------------------------------------------------
    // Independent variable vector
    CPPAD_TESTVECTOR( CppAD::AD<double> ) a_ind(n);
    for(size_t j = 0; j < n; ++j)
        a_ind[j] = AD<double>(1 + j);
    CppAD::Independent(a_ind);
    //
    // au, av, aw
    CPPAD_TESTVECTOR( CppAD::AD<double> ) au(m), av(m), aw(m);
    for(size_t i = 0; i < m; ++i)
    {   au[i] = a_ind[0 * m + i];
        av[i] = a_ind[1 * m + i];
        aw[i] = a_ind[2 * m + i];
    }
    //
    // ax = (au, av)
    CPPAD_TESTVECTOR( CppAD::AD<double> ) ax(2 * m);
    for(size_t i = 0; i < m; ++i)
    {   ax[i]     = au[i];
        ax[m + i] = av[i];
    }
    //
    // ay = u + v
    CPPAD_TESTVECTOR( CppAD::AD<double> ) ay(m);
    vec_op(add_op, ax, ay);
    //
    // ax = (ay, aw)
    for(size_t i = 0; i < m; ++i)
    {   ax[i]     = ay[i];
        ax[m + i] = aw[i];
    }
    //
    // az = ay * w
    CPPAD_TESTVECTOR( CppAD::AD<double> ) az(m);
    vec_op(mul_op, ax, az);
    //
    // ay = - az
    vec_op(neg_op, az, ay);
    //
    // f
    CPPAD_TESTVECTOR( CppAD::AD<double> ) a_dep(1);
    a_dep[0] = ay[0];
    CppAD::ADFun<double> f(a_ind, a_dep);
    //
    // size_var
    // phantom variable, independent variables, operator results
    ok   &= f.size_var() == 1 + n + 3 * m;
    //
    // optimize
    // The atomic funciton rev_depend routine is called by optimizer
    f.optimize();
    //
    // size_var
    // phantom variablem, independent variables, operator variables
    ok &= f.size_var() == 1 + n + 3;
    //
    //
    return ok;
}
// END C++
