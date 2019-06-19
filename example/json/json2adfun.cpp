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
    // node_1 : x[0]
    // node_2 : x[1]
    // node_3 : "x"
    // node_4 : "y"
    // node_5 : -2.0
    // node_6 : x[0] + x[1]
    // node_7 : (x[0] + x[1]) * (x[0] + x[1])
    // use single quote to avoid having to escape double quote
    std::string graph =
        "{\n"
        "   'n_dynamic_ind'  : 0,\n"
        "   'n_independent'  : 2,\n"
        "   'string_vec'     : [ 2, [ 'x', 'y' ] ],\n"
        "   'constant_vec'   : [ 1, [ -2.0 ] ],\n"
        "   'operator_vec'   : [ 2, [\n"
        "       [ 0, 1, 2, [1, 2], 'add' ] ,\n"
        "       [ 1, 1, 2, [6, 6], 'mul' ] ] \n"
        "   ],\n"
        "   'dependent_vec'   : [ 1, [7] ]\n"
        "}\n";
    // Convert the single quote to double quote
    for(size_t i = 0; i < graph.size(); ++i)
        if( graph[i] == '\'' ) graph[i] = '"';
    //
    CppAD::ADFun<double> fun(graph);
    //
    // Compute function value
    vector<double> x(2);
    x[0] = 2.0;
    x[1] = 3.0;
    vector<double> y = fun.Forward(0, x);
    ok  &= y[0] ==  (x[0] + x[1]) * (x[0] + x[1]);
    //
    // Conpute derivative value
    vector<double> jac = fun.Jacobian(x);
    ok &= jac[0] == 2.0 * (x[0] + x[1]);
    ok &= jac[1] == 2.0 * (x[0] + x[1]);
    //
    return ok;
}
// END C++
