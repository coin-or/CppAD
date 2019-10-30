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
$begin graph_sub_op.cpp$$
$spell
    sub
$$

$section C++ AD Graph sub Operator: Example and Test$$

$head Source Code$$
$srcfile%test_more/graph/sub_op.cpp%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>

bool sub_op(void)
{   bool ok = true;
    using CppAD::vector;
    using CppAD::AD;
    using std::string;
    typedef CppAD::local::graph::graph_op_struct graph_op_struct;
    typedef CppAD::local::graph::cpp_graph       cpp_graph;
    //
    // AD graph example
    // node_1 : p[0]
    // node_2 : p[1]
    // node_3 : x[0]
    // node_4 : p[0] - p[1]
    // node_5 : x[0] - ( p[0] - p[1] )
    // y[0]   = x[0] - ( p[0] - p[1] )
    //
    //
    // C++ graph object
    cpp_graph graph_obj;
    //
    vector<graph_op_struct>& operator_vec(    graph_obj.operator_vec() );
    vector<size_t>&          operator_arg(    graph_obj.operator_arg() );
    vector<size_t>&          dependent_vec(   graph_obj.dependent_vec() );
    //
    // structure corresponding to one operator
    graph_op_struct         op_usage;
    //
    // size_t value that is not used
    //
    // set scalars
    graph_obj.set_function_name("sub_op example");
    size_t n_dynamic_ind = 2;
    graph_obj.set_n_dynamic_ind(n_dynamic_ind);
    size_t n_independent = 1;
    graph_obj.set_n_independent(n_independent);
    //
    // node_4 : p[0] - p[1]
    op_usage.op_enum          = CppAD::local::graph::sub_graph_op;
    op_usage.start_arg        = operator_arg.size();
    operator_vec.push_back(op_usage);
    operator_arg.push_back(1);
    operator_arg.push_back(2);
    //
    // node_5 : x[0] - ( p[0] - p[1] )
    op_usage.start_arg        = operator_arg.size();
    operator_vec.push_back(op_usage);
    operator_arg.push_back(3);
    operator_arg.push_back(4);
    //
    // y[0]   = x[0] - ( p[0] - p[1] )
    dependent_vec.push_back(5);
    //
    // f(x, p) = x_0 - ( p_0 - p_1 )
    CppAD::ADFun<double> f;
    f.from_graph(graph_obj);
    //
    ok &= f.Domain() == 1;
    ok &= f.Range() == 1;
    ok &= f.size_dyn_ind() == 2;
    //
    // set independent variables and parameters
    vector<double> p(2), x(1);
    p[0] = 2.0;
    p[1] = 3.0;
    x[0] = 4.0;
    //
    // compute y = f(x, p)
    f.new_dynamic(p);
    vector<double> y = f.Forward(0, x);
    //
    // check result
    ok &= y[0] == x[0] - ( p[0] - p[1] );
    // -----------------------------------------------------------------------
    // Convert to Graph graph and back again
    f.to_graph(graph_obj);
    // std::cout << "json = " << json;
    f.from_graph(graph_obj);
    // -----------------------------------------------------------------------
    ok &= f.Domain() == 1;
    ok &= f.Range() == 1;
    ok &= f.size_dyn_ind() == 2;
    //
    // set independent variables and parameters
    p[0] = 2.0;
    p[1] = 3.0;
    x[0] = 4.0;
    //
    // compute y = f(x, p)
    f.new_dynamic(p);
    y = f.Forward(0, x);
    //
    // check result
    ok &= y[0] == x[0] - ( p[0] - p[1] );
    //
    return ok;
}
// END C++
