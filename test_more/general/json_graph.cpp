/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <cppad/cppad.hpp>

namespace { // BEGIN_EMPTY_NAMESPACE
// ---------------------------------------------------------------------------
// Test transforming to Json and back to a function
bool to_json_and_back(void)
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
        "   'string_vec'     : 2, [ 'x', 'y' ],\n"
        "   'constant_vec'   : 1, [ -2.0 ],\n"
        "   'op_usage_vec'   : 2, [\n"
        "       [ 3, 1, 3, [1, 2, 3 ] ] ,\n"
        "       [ 2, 7, 7             ] ] \n"
        "   ,\n"
        "   'dependent_vec'   : 1, [8]\n"
        "}\n";
    // Convert the single quote to double quote
    for(size_t i = 0; i < graph.size(); ++i)
        if( graph[i] == '\'' ) graph[i] = '"';
    //
    CppAD::ADFun<double> fun;
    fun.from_json(graph);
    graph = fun.to_json();
    // For debugging
    // std::cout << "graph = " << graph;
    fun.from_json(graph);
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
    // Uncomment statement below to see the graph
    // std::cout << graph;
    //
    return ok;
}
// ---------------------------------------------------------------------------
// Test binary operators that should be implemented
bool binary_operators(void)
{   bool ok   = true;
    using CppAD::AD;
    //
    size_t np = 1;
    size_t nx = 2;
    size_t ny = 10;
    CPPAD_TESTVECTOR(double)       p(np),   x(nx);
    CPPAD_TESTVECTOR( AD<double> ) ap(np), ax(nx), ay(ny);
    for(size_t i = 0; i < np; ++i)
    {   ap[i] = 0.5;
        p[i]  = double(i + 1);
    }
    for(size_t i = 0; i < nx; ++i)
    {   ax[i] = 0.25;
        x[i]  = double(2 * i + 1);
    }
    size_t abort_op_index = 0;
    bool   record_compare = true;
    CppAD::Independent(ax, abort_op_index, record_compare, ap);
    //
    size_t j = 0;
    ay[j++] = ap[0] + 2.0;    // dynamic + constant (and ParOp)
    ay[j++] = 2.0 + ap[0];    // constant + dynamic (and ParOp)
    ay[j++] = ax[0] + ap[0];  // variable + dynamic
    ay[j++] = ap[0] + ax[0];  // dynamic + variable
    ay[j++] = ax[0] + ax[1];  // variable + variable
    //
    ay[j++] = ap[0] * 2.0;    // dynamic * constant (and ParOp)
    ay[j++] = 2.0 * ap[0];    // constant * dynamic (and ParOp)
    ay[j++] = ax[0] * ap[0];  // variable * dynamic
    ay[j++] = ap[0] * ax[0];  // dynamic * variable
    ay[j++] = ax[0] * ax[1];  // variable * variable
    //
    ok &= j == ny;
    //
    // Create function
    CppAD::ADFun<double> f(ax, ay);
    //
    // Evaluate function at x before
    f.new_dynamic(p);
    CPPAD_TESTVECTOR(double) y_before = f.Forward(0, x);
    //
    // Convert to Json and back again
    std::string graph = f.to_json();
    // std::cout << graph;
    f.from_json(graph);
    //
    // Evaluate function at x after
    f.new_dynamic(p);
    CPPAD_TESTVECTOR(double) y_after = f.Forward(0, x);
    //
    double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
    for(size_t i = 0; i < ny; ++i)
        ok &= CppAD::NearEqual( y_before[i], y_after[i], eps99, eps99 );
    //
    // Uncomment statement below to see the graph
    // std::cout << graph;
    return ok;
}
// ---------------------------------------------------------------------------
// Test cumulative sum operator
bool cumulative_sum(void)
{   bool ok   = true;
    using CppAD::AD;
    //
    size_t np = 2;
    size_t nx = 2;
    size_t ny = 1;
    CPPAD_TESTVECTOR(double)       p(np),   x(nx);
    CPPAD_TESTVECTOR( AD<double> ) ap(np), ax(nx), ay(ny);
    for(size_t i = 0; i < np; ++i)
    {   ap[i] = 0.5;
        p[i]  = double(i + 1);
    }
    for(size_t i = 0; i < nx; ++i)
    {   ax[i] = 0.25;
        x[i]  = double(2 * i + 1);
    }
    size_t abort_op_index = 0;
    bool   record_compare = true;
    CppAD::Independent(ax, abort_op_index, record_compare, ap);
    //
    AD<double> asum = 0.0;
    asum +=  1.0 + ap[0];
    asum += ap[1] + 1.0;
    asum -= ap[1] + ap[0];
    //
    asum +=  1.0 + ax[0];
    asum += ax[1] + 1.0;
    asum -= ax[1] + ax[0];
    //
    asum += ap[0] + ax[0];
    asum += ax[1] + ap[1];
    //
    ay[0] = asum;
    //
    // Create function
    CppAD::ADFun<double> f(ax, ay);
    f.optimize();
    //
    // Evaluate function at x before
    f.new_dynamic(p);
    CPPAD_TESTVECTOR(double) y_before = f.Forward(0, x);
    //
    // Convert to Json and back again
    std::string graph = f.to_json();
    // std::cout << graph;
    f.from_json(graph);
    //
    // Evaluate function at x after
    f.new_dynamic(p);
    CPPAD_TESTVECTOR(double) y_after = f.Forward(0, x);
    //
    double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
    for(size_t i = 0; i < ny; ++i)
        ok &= CppAD::NearEqual( y_before[i], y_after[i], eps99, eps99 );
    //
    // Uncomment statement below to see the graph
    // std::cout << graph;
    return ok;
}

// ---------------------------------------------------------------------------
} // END_EMPTY_NAMESPACE


bool json_graph(void)
{   bool ok = true;
    ok     &= to_json_and_back();
    ok     &= binary_operators();
    ok     &= cumulative_sum();
    //
    return ok;
}
