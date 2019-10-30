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
$begin graph_cexp_op.cpp$$
$spell
    abs
$$

$section C++ AD Graph Conditional Expressions: Example and Test$$

$head Source Code$$
$srcfile%test_more/graph/cexp_op.cpp%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>

bool cexp_op(void)
{   bool ok = true;
    using CppAD::vector;
    using CppAD::AD;
    using std::string;
    typedef CppAD::local::graph::graph_op_struct graph_op_struct;
    typedef CppAD::local::graph::cpp_graph       cpp_graph;
    double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
    //
    // AD graph example
    // node_1 : p[0]
    // node_2 : x[0]
    // node_3 : c[0]
    // node_4 : cexp_le(p[0], x[0], p[0], x[0])
    // y[0]   = cexp_le(p[0], x[0], p[0], x[0])
    //
    //
    // C++ graph object
    cpp_graph graph_obj;
    //
    std::string&             function_name(   graph_obj.function_name() );
    size_t&                  n_dynamic_ind(   graph_obj.n_dynamic_ind() );
    size_t&                  n_independent(   graph_obj.n_independent() );
    vector<double>&          constant_vec(    graph_obj.constant_vec() );
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
    function_name = "cexp_op example";
    n_dynamic_ind = 1;
    n_independent = 1;
    constant_vec.push_back(-0.1);
    //
    // node_4 : cexp_le(p[0], x[0], p[0], x[0])
    op_usage.op_enum     = CppAD::local::graph::cexp_le_graph_op;
    op_usage.start_arg   = operator_arg.size();
    operator_vec.push_back(op_usage);
    operator_arg.push_back(1);
    operator_arg.push_back(2);
    operator_arg.push_back(1);
    operator_arg.push_back(2);
    //
    // y[0]   = cexp_le(p[0], x[0], p[0], x[0])
    dependent_vec.push_back(4);
    //
    // f(x, p) = cexp_le(p[0], x[0], p[0], x[0])
    CppAD::ADFun<double> f;
    f.from_graph(graph_obj);
    ok &= f.Domain() == 1;
    ok &= f.Range() == 1;
    ok &= f.size_dyn_ind() == 1;
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
    double check;
    if( p[0] <= x[0] )
        check= p[0];
    else
        check = x[0];
    //
    // check result
    ok &= CppAD::NearEqual(y[0], check, eps99, eps99);
    // ----------------------------------------------------------------------
    // Convert to Graph graph and back again
    f.to_graph(graph_obj);
    f.from_graph(graph_obj);
    // ----------------------------------------------------------------------
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
