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
$begin graph_comp_op.cpp$$
$spell
    abs
$$

$section C++ AD Graph Comparison Operators: Example and Test$$

$head Source Code$$
$srcfile%test_more/graph/comp_op.cpp%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>

bool comp_op(void)
{   bool ok = true;
    using CppAD::vector;
    using CppAD::AD;
    using std::string;
    typedef CppAD::local::graph::graph_op_struct graph_op_struct;
    double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
    //
    // AD graph example
    // node_1 : p[0]
    // node_2 : x[0]
    //        : x[0] < p[0]
    // node_3 : p[0] - x[0]
    // node_4 : log( p[0] - x[0] )
    // y[0]   = log( p[0] - x[0] )
    //
    // initialize vectors as empty
    vector<string>          atomic_name_vec;
    vector<double>          constant_vec;
    vector<graph_op_struct> operator_vec;
    vector<size_t>          operator_arg;
    vector<size_t>          dependent_vec;
    //
    // structure corresponding to one operator
    graph_op_struct         op_usage;
    //
    // size_t value that is not used
    //
    // set scalars
    string function_name = "comp_op example";
    size_t n_dynamic_ind = 1;
    size_t n_independent = 1;
    //
    // x[0] < p[0]
    op_usage.op_enum          = CppAD::local::graph::comp_lt_graph_op;
    op_usage.n_result         = 0;
    op_usage.n_arg            = 2;
    op_usage.start_arg        = operator_arg.size();
    operator_vec.push_back(op_usage);
    operator_arg.push_back(2);
    operator_arg.push_back(1);
    //
    // node_3 : p[0] - x[0]
    op_usage.op_enum          = CppAD::local::graph::sub_graph_op;
    op_usage.n_result         = 1;
    op_usage.start_arg        = operator_arg.size();
    operator_vec.push_back(op_usage);
    operator_arg.push_back(1);
    operator_arg.push_back(2);
    //
    // node_4 : log( p[0] - x[0] )
    op_usage.op_enum          = CppAD::local::graph::log_graph_op;
    op_usage.n_arg            = 1;
    op_usage.start_arg        = operator_arg.size();
    operator_vec.push_back(op_usage);
    operator_arg.push_back(3);
    //
    // y[0]   = log( p[0] - x[0] )
    dependent_vec.push_back(4);
    //
    // f(x, p) = log( p[0] - x[0] )
    CppAD::ADFun<double> f;
    f.from_graph(
        function_name,
        atomic_name_vec,
        n_dynamic_ind,
        n_independent,
        constant_vec,
        operator_vec,
        operator_arg,
        dependent_vec
    );
    ok &= f.Domain() == 1;
    ok &= f.Range() == 1;
    ok &= f.size_dyn_ind() == 1;
    //
    // set independent variables and parameters
    vector<double> p(1), x(1);
    p[0] = 0.3;
    x[0] = 0.2;
    //
    // compute y = f(x, p)
    f.new_dynamic(p);
    vector<double> y = f.Forward(0, x);
    //
    //  x[0] < p[0] so comparison should not have changed
    ok &= f.compare_change_number() == 0;
    //
    // check result
    double check = std::log( p[0] - x[0] );
    ok &= CppAD::NearEqual(y[0], check, eps99, eps99);
    //
    // case where comparison is false
    f.check_for_nan(false); // suppress checking for nan for this test
    x[0] = 0.4;
    y = f.Forward(0, x);
    ok &= f.compare_change_number() == 1;
    //
    // -----------------------------------------------------------------------
    // Convert to Graph graph and back again
    f.to_graph(
        function_name,
        atomic_name_vec,
        n_dynamic_ind,
        n_independent,
        constant_vec,
        operator_vec,
        operator_arg,
        dependent_vec
    );
    // std::cout << graph;
    f.from_graph(
        function_name,
        atomic_name_vec,
        n_dynamic_ind,
        n_independent,
        constant_vec,
        operator_vec,
        operator_arg,
        dependent_vec
    );
    // -----------------------------------------------------------------------
    //
    // compute y = f(x, p)
    f.new_dynamic(p);
    x[0] = 0.2;
    y = f.Forward(0, x);
    //
    // check result
    ok &= CppAD::NearEqual(y[0], check, eps99, eps99);
    //
    // case where comparison is false
    x[0] = 0.4;
    y = f.Forward(0, x);
    ok &= f.compare_change_number() == 1;
    //
    return ok;
}
// END C++
