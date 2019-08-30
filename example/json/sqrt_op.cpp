/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin json_sqrt_op.cpp$$
$spell
    sqrt
    Json
$$

$section Json sqrt Operator: Example and Test$$

$head Source Code$$
$srcfile%example/json/sqrt_op.cpp%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>

bool sqrt_op(void)
{   bool ok = true;
    using CppAD::vector;
    using CppAD::AD;
    double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
    //
    // AD graph example
    // node_1 : p[0]
    // node_2 : x[0]
    // node_3 : c[0]
    // node_4 : sqrt(p[0])
    // node_5 : sqrt(x[0])
    // node_6 : sqrt(c[0])
    // node_7 : sqrt(p[0]) + sqrt(x[0]) + sqrt(c[0])
    // y[0]   = sqrt(p[0]) + sqrt(x[0]) + sqrt(c[0])
    // use single quote to avoid having to escape double quote
    std::string graph =
        "{\n"
        "   'function_name'  : 'sqrt_op example',\n"
        "   'op_define_vec'  : [ 2, [\n"
        "       { 'op_code':1, 'name':'sqrt', 'n_arg':1 } ,\n"
        "       { 'op_code':2, 'name':'sum'             } ]\n"
        "   ],\n"
        "   'n_dynamic_ind'  : 1,\n"
        "   'n_independent'  : 1,\n"
        "   'constant_vec'   : 1, [ +0.1 ],\n" // c[0]
        "   'op_usage_vec'   : 4, [\n"
        "       [ 1, 1]                ,\n" // sqrt(p[0])
        "       [ 1, 2]                ,\n" // sqrt(x[0])
        "       [ 1, 3]                ,\n" // sqrt(c[0])
        "       [ 2, 1, 3, [4, 5, 6] ] ]\n" // sqrt(p[0])+sqrt(x[0])+sqrt(c[0])
        "   ,\n"
        "   'dependent_vec' : 1, [7]\n"
        "}\n";
    // Convert the single quote to double quote
    for(size_t i = 0; i < graph.size(); ++i)
        if( graph[i] == '\'' ) graph[i] = '"';
    //
    // f(x, p) = sqrt(p_0) + sqrt(x_0) + sqrt(c_0)
    CppAD::ADFun<double> f;
    f.from_json(graph);
    ok &= f.Domain() == 1;
    ok &= f.Range() == 1;
    ok &= f.size_dyn_ind() == 1;
    //
    // value of constant in function
    vector<double> c(1);
    c[0] = +0.1;
    //
    // set independent variables and parameters
    vector<double> p(1), x(1);
    p[0] = 0.2;
    x[0] = 0.3;
    //
    // compute y = f(x, p)
    f.new_dynamic(p);
    vector<double> y = f.Forward(0, x);
    //
    // check result
    double check = std::sqrt(p[0]) + std::sqrt(x[0]) + std::sqrt(c[0]);
    ok &= CppAD::NearEqual(y[0], check, eps99, eps99);
    //
    // Convert to Json graph and back again
    graph = f.to_json();
    f.from_json(graph);
    //
    // compute y = f(x, p)
    f.new_dynamic(p);
    y = f.Forward(0, x);
    //
    // check result
    ok &= CppAD::NearEqual(y[0], check, eps99, eps99);
    //
    return ok;
}
// END C++
