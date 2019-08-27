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
$begin json_expm1_op.cpp$$
$spell
    Json
$$

$section Using The Json Addition Operator: Example and Test$$

$head Source Code$$
$srcfile%example/json/expm1_op.cpp%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>

bool expm1_op(void)
{   bool ok = true;
    using CppAD::vector;
    using CppAD::AD;
    double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
    //
    // AD graph example
    // node_1 : p[0]
    // node_2 : x[0]
    // node_3 : c[0]
    // node_4 : expm1(p[0])
    // node_5 : expm1(x[0])
    // node_6 : expm1(c[0])
    // node_7 : expm1(p[0]) + expm1(x[0]) + expm1(c[0])
    // y[0]   = expm1(p[0]) + expm1(x[0]) + expm1(c[0])
    // use single quote to avoid having to escape double quote
    std::string graph =
        "{\n"
        "   'function_name'  : 'expm1_op example',\n"
        "   'op_define_vec'  : [ 2, [\n"
        "       { 'op_code':1, 'name':'expm1', 'n_arg':1 } ,\n"
        "       { 'op_code':2, 'name':'sum'              } ]\n"
        "   ],\n"
        "   'n_dynamic_ind'  : 1,\n"
        "   'n_independent'  : 1,\n"
        "   'string_vec'     : 0, [ ],\n"
        "   'constant_vec'   : 1, [ 0.3 ],\n" // c[0]
        "   'op_usage_vec'   : 4, [\n"
        "       [ 1, 1]                ,\n" // expm1(p0)
        "       [ 1, 2]                ,\n" // expm1(x0)
        "       [ 1, 3]                ,\n" // expm1(c0)
        "       [ 2, 1, 3, [4, 5, 6] ] ]\n" // expm1(p0)+expm1(x0)+expm1(c0)
        "   ,\n"
        "   'dependent_vec' : 1, [7]\n"
        "}\n";
    // Convert the single quote to double quote
    for(size_t i = 0; i < graph.size(); ++i)
        if( graph[i] == '\'' ) graph[i] = '"';
    //
    // f(x, p) = expm1(p0) + expm1(x0) + expm1(c0)
    CppAD::ADFun<double> f;
    f.from_json(graph);
    ok &= f.Domain() == 1;
    ok &= f.Range() == 1;
    ok &= f.size_dyn_ind() == 1;
    //
    // value of constant in function
    vector<double> c(1);
    c[0] = 0.3;
    //
    // set independent variables and parameters
    vector<double> p(1), x(1);
    p[0] = -0.1;
    x[0] = 0.2;
    //
    // compute y = f(x, p)
    f.new_dynamic(p);
    vector<double> y = f.Forward(0, x);
    //
    // check result
    double check = CppAD::expm1(p[0]) + CppAD::expm1(x[0]) + CppAD::expm1(c[0]);
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
