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
$begin json_atom_op.cpp$$
$spell
    atom
    Json
$$

$section Json Atomic Function Operator: Example and Test$$

$head Source Code$$
$srcfile%example/json/atom_op.cpp%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>

bool atom_op(void)
{   bool ok = true;
    using CppAD::vector;
    using CppAD::AD;
    // use single quote in graphs to avoid having to escape double quote
    //
    // -----------------------------------------------------------------------
    // Define f_0 (x_0, x_1; p) = p_0 * x_0
    //        f_1 (x_0, x_1; p) = p_1 * x_1
    //
    // This Function does not have a checkpoint operator
    // node_1 : p[0]
    // node_2 : p[1]
    // node_3 : x[0]
    // node_4 : x[1]
    // node_5 : p[0] * x[0]
    // node_6 : p[1] * x[1]
    // y[0]   = p[0] * x[0]
    // y[1]   = p[1] * x[1]
    std::string graph =
        "{\n"
        "   'function_name'  : 'f(x; p)',\n"
        "   'op_define_vec'  : [ 1, [\n"
        "       { 'op_code':1, 'name':'mul', 'n_arg':2 } ]\n"
        "   ],\n"
        "   'n_dynamic_ind'  : 2,\n"         // p[0], p[1]
        "   'n_independent'  : 2,\n"         // x[0], x[1]
        "   'constant_vec'   : 0, [ ],\n"
        "   'op_usage_vec'   : 2, [\n"
        "       [ 1, 1, 3 ] ,\n" // p[0] * x[0]
        "       [ 1, 2, 4 ] ]\n" // p[1] * x[1]
        "   ,\n"
        "   'dependent_vec' : 2, [5, 6]\n"
        "}\n";
    // Convert the single quote to double quote
    for(size_t i = 0; i < graph.size(); ++i)
        if( graph[i] == '\'' ) graph[i] = '"';
    //
    CppAD::ADFun<double> f;
    f.from_json(graph);
    ok &= f.Domain() == 2;
    ok &= f.Range() == 2;
    ok &= f.size_dyn_ind() == 2;
    //
    // We could set the dynamic parameters in f to value nans during
    // construction of its checkpoint function. For the purpose of this example
    // we turn off checking for nan.
    f.check_for_nan(false);
    //
    // Create a ckhpoint_two with name f(x; p).
    // This also creates an atomic_three fucntion with name f(x; p).
    bool internal_bool    = false;
    bool use_hes_sparsity = false;
    bool use_base2ad      = false;
    bool use_in_parallel  = false;
    CppAD::chkpoint_two<double> chk_f(f, "f(x; p)",
        internal_bool, use_hes_sparsity, use_base2ad, use_in_parallel
    );
    // -----------------------------------------------------------------------
    // Define g_0 (u_0, u_1; q) = f_0 (u_0 + q_0, u_0 + q_1, p)
    //                          + f_1 (u_0 + q_0, u_0 + q_1, p)
    //                          = p_0 * (u_0 + q_0) + p_1 * (u_1 + q_1)
    //
    // This Function has a checkpoint operator
    // node_1 : q[0]
    // node_2 : q[1]
    // node_3 : u[0]
    // node_4 : u[1]
    // node_5 : u[0] + q[0]
    // node_6 : u[1] + q[1]
    // node_7 : f_0 ( u[0] + q[0], u[1] + q[1]; p)
    // node_8 : f_1 ( u[0] + q[0], u[1] + q[1]; p)
    // node_9 : p_0 * (u_0 + q_0) + p_1 * (u_1 + q_1)
    // y[0]   = p_0 * (u_0 + q_0) + p_1 * (u_1 + q_1)
    graph =
        "{\n"
        "   'function_name'  : 'g(u; p, q)',\n"
        "   'op_define_vec'  : [ 2, [\n"
        "       { 'op_code':1, 'name':'atom'           } ,\n"
        "       { 'op_code':2, 'name':'add', 'n_arg':2 } ]\n"
        "   ],\n"
        "   'n_dynamic_ind'  : 2,\n"              // q[0], q[1]
        "   'n_independent'  : 2,\n"              // u[0], u[1]
        "   'constant_vec'   : 0, [ ],\n"
        "   'op_usage_vec'   : 4, [\n"
        "       [ 2, 3, 1 ]                      ,\n" // x[0] = u[0] + q[0]
        "       [ 2, 4, 2 ]                      ,\n" // x[1] = u[1] + q[1]
        "       [ 1, 'f(x; p)', 2, 2, [ 5, 6 ] ] ,\n" // f(x; p)
        "       [ 2, 7, 8 ]                      ]\n" // f_0(x; p) + f_1(x; p)
        "   ,\n"
        "   'dependent_vec' : 1, [9]\n"
        "}\n";
    // Convert the single quote to double quote
    for(size_t i = 0; i < graph.size(); ++i)
        if( graph[i] == '\'' ) graph[i] = '"';
    //
    CppAD::ADFun<double> g;
    g.from_json(graph);
    ok &= g.Domain() == 2;
    ok &= g.Range() == 1;
    ok &= g.size_dyn_ind() == 2;
    //
    // set p in g(u; p, q)
    vector<double> p(2);
    p[0] = 2.0;
    p[1] = 3.0;
    chk_f.new_dynamic(p);
    //
    // set q in g(u; p, q)
    vector<double> q(2);
    q[0] = 4.0;
    q[1] = 5.0;
    g.new_dynamic(q);
    //
    // evalute g(u; p, q)
    vector<double> u(2), y(1);
    u[0] = 6.0;
    u[1] = 7.0;
    y    = g.Forward(0, u);
    //
    // check value
    double check = p[0] * (u[0] + q[0]) + p[1] * (u[1] + q[1]);
    ok &= y[0] == check;
    //
    return ok;
}
// END C++
