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
$begin json2adfun.cpp$$
$spell
    Json
$$

$section ADFun Object Corresponding to Json AD Graph : Example and Test$$

$head Source Code$$
$srcfile%example/json/json2adfun.cpp%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>

bool json2adfun(void)
{   bool ok = true;
    using CppAD::vector;
    //
    // An AD graph example
    // node_1 : p[0]
    // node_2 : x[0]
    // node_3 : x[1]
    // node_4 : "x"
    // node_5 : "y"
    // node_6 : -2.0
    // node_7 : p[0] + x[0] + x[1]
    // node_8 : (p[0] + x[0] + x[1]) * (p[0] + x[0] + x[1])
    // y[0]   = (p[0] + x[0] + x[1]) * (p[0] + x[0] + x[1])
    // use single quote to avoid having to escape double quote
    std::string graph =
        "{\n"
        "   'op_define_vec'  : [ 3, [\n"
        "       { 'op_code':1, 'name':'add', 'n_arg':2 } ,\n"
        "       { 'op_code':2, 'name':'mul', 'n_arg':2 } ,\n"
        "       { 'op_code':3, 'name':'sum'            } ]\n"
        "   ],\n"
        "   'n_dynamic_ind'  : 1,\n"
        "   'n_independent'  : 2,\n"
        "   'string_vec'     : [ 2, [ 'x', 'y' ] ],\n"
        "   'constant_vec'   : [ 1, [ -2.0 ] ],\n"
        "   'op_usage_vec'   : [ 2, [\n"
        "       [ 3, 1, 3, [1, 2, 3 ] ] ,\n"
        "       [ 2, 7, 7             ] ] \n"
        "   ],\n"
        "   'dependent_vec'   : [ 1, [8] ]\n"
        "}\n";
    // Convert the single quote to double quote
    for(size_t i = 0; i < graph.size(); ++i)
        if( graph[i] == '\'' ) graph[i] = '"';
    //
    CppAD::ADFun<double> fun(graph);
    //
    // Compute function value
    vector<double> p(1), x(2);
    p[0] = 1.0;
    x[0] = 2.0;
    x[1] = 3.0;
    fun.new_dynamic(p);
    vector<double> y = fun.Forward(0, x);
    ok  &= y[0] ==  (p[0] + x[0] + x[1]) * (p[0] + x[0] + x[1]);
    //
    // Conpute derivative value
    vector<double> jac = fun.Jacobian(x);
    ok &= jac[0] == 2.0 * (p[0] + x[0] + x[1]);
    ok &= jac[1] == 2.0 * (p[0] + x[0] + x[1]);
    //
    return ok;
}
// END C++
