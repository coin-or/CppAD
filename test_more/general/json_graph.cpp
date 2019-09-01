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
// Test conditonal expression
bool cexp_lt_variable(void)
{   bool ok = true;
    using CppAD::vector;
    //
    // An AD graph example
    // node_1 : x[0]
    // node_2 : x[1]
    // node_3 : cexp_lt(x[0], x[1], x[1], x[0])
    // y[0]   = max(x[0], x[1])
    // use single quote to avoid having to escape double quote
    std::string graph =
        "{\n"
        "   'function_name'  : 'cexp_lt test',\n"
        "   'op_define_vec'  : [ 1, [\n"
        "       { 'op_code':1, 'name':'cexp_lt', 'n_arg':4 } ]\n"
        "   ],\n"
        "   'n_dynamic_ind'  : 0,\n"
        "   'n_independent'  : 2,\n"
        "   'constant_vec'   : 0, [],\n"
        "   'op_usage_vec'   : 1, [\n"
        "       [ 1, 1, 2, 2, 1 ] ] \n"
        "   ,\n"
        "   'dependent_vec'   : 1, [3]\n"
        "}\n";
    // Convert the single quote to double quote
    for(size_t i = 0; i < graph.size(); ++i)
        if( graph[i] == '\'' ) graph[i] = '"';
    //
    CppAD::ADFun<double> f;
    f.from_json(graph);
    // ---------------------------------------------------------------------
    ok &= f.Domain() == 2;
    ok &= f.Range() == 1;
    ok &= f.size_dyn_ind() == 0;
    //
    vector<double> x(2), y(1);
    x[0] = 2.0;
    x[1] = 3.0;
    y = f.Forward(0, x);
    ok &= y[0] == std::max(x[0], x[1]);
    //
    x[0] = 3.0;
    x[1] = 2.0;
    y = f.Forward(0, x);
    ok &= y[0] == std::max(x[0], x[1]);
    //
    return ok;
}
// Test conditonal expression
bool cexp_lt_constant(void)
{   bool ok = true;
    using CppAD::vector;
    //
    // An AD graph example
    // node_1 : x[0]
    // node_2 : c[0]
    // node_3 : c[1]
    // node_4 : cexp_lt(c[0], c[1], c[1], c[0])
    // y[0]   = max(c[0], c[1])
    // use single quote to avoid having to escape double quote
    std::string graph =
        "{\n"
        "   'function_name'  : 'cexp_lt test',\n"
        "   'op_define_vec'  : [ 1, [\n"
        "       { 'op_code':1, 'name':'cexp_lt', 'n_arg':4 } ]\n"
        "   ],\n"
        "   'n_dynamic_ind'  : 0,\n"
        "   'n_independent'  : 1,\n"
        "   'constant_vec'   : 2, [ 5.0, -5.0 ],\n"
        "   'op_usage_vec'   : 1, [\n"
        "       [ 1, 2, 3,  3, 2 ] ] \n"
        "   ,\n"
        "   'dependent_vec'   : 1, [4]\n"
        "}\n";
    // Convert the single quote to double quote
    for(size_t i = 0; i < graph.size(); ++i)
        if( graph[i] == '\'' ) graph[i] = '"';
    //
    CppAD::ADFun<double> f;
    f.from_json(graph);
    // ---------------------------------------------------------------------
    ok &= f.Domain() == 1;
    ok &= f.Range() == 1;
    ok &= f.size_dyn_ind() == 0;
    //
    vector<double> c(2), x(1), y(1);
    c[0] = 5.0;
    c[1] = -5.0;
    y = f.Forward(0, x);
    ok &= y[0] == std::max(c[0], c[1]);
    //
    return ok;
}
bool cexp_lt_dynamic(void)
{   bool ok = true;
    using CppAD::vector;
    //
    // An AD graph example
    // node_1 : p[0]
    // node_2 : p[1]
    // node_3 : x[0]
    // node_4 : cexp_lt(p[0], p[1], p[1], p[0])
    // y[0]   = max(p[0], p[1])
    // use single quote to avoid having to escape double quote
    std::string graph =
        "{\n"
        "   'function_name'  : 'cexp_lt test',\n"
        "   'op_define_vec'  : [ 1, [\n"
        "       { 'op_code':1, 'name':'cexp_lt', 'n_arg':4 } ]\n"
        "   ],\n"
        "   'n_dynamic_ind'  : 2,\n"
        "   'n_independent'  : 1,\n"
        "   'constant_vec'   : 0, [ ],\n"
        "   'op_usage_vec'   : 1, [\n"
        "       [ 1, 1, 2, 2, 1 ] ] \n"
        "   ,\n"
        "   'dependent_vec'   : 1, [4]\n"
        "}\n";
    // Convert the single quote to double quote
    for(size_t i = 0; i < graph.size(); ++i)
        if( graph[i] == '\'' ) graph[i] = '"';
    //
    CppAD::ADFun<double> f;
    f.from_json(graph);
    // ---------------------------------------------------------------------
    ok &= f.Domain() == 1;
    ok &= f.Range() == 1;
    ok &= f.size_dyn_ind() == 2;
    //
    vector<double> p(2), x(1), y(1);
    p[0] = 3.0;
    p[1] = 2.0;
    f.new_dynamic(p);
    y = f.Forward(0, x);
    ok &= y[0] == std::max(p[0], p[1]);
    //
    return ok;
}
// ---------------------------------------------------------------------------
// Test atomic function that gets passed both variables and dynamic parameters
bool atomic_both(void)
{   bool ok = true;
    using CppAD::vector;
    using CppAD::AD;
    //
    // y[0] = p[0] * x[0]
    vector< AD<double> > ap(1), ax(1), ay(1);
    ap[0] = 2.0;
    ax[0] = 3.0;
    size_t abort_op_index = 0;
    bool   record_compare = false;
    CppAD::Independent(ax, abort_op_index, record_compare, ap);
    ay[0] = ap[0] * ax[0];
    CppAD::ADFun<double> f(ax, ay);
    //
    // Create a ckhpoint_two with name f(x; p).
    // (This also creates an atomic_three fucntion with same name.)
    bool internal_bool    = false;
    bool use_hes_sparsity = false;
    bool use_base2ad      = false;
    bool use_in_parallel  = false;
    CppAD::chkpoint_two<double> chk_f(f, "f(x; p)",
        internal_bool, use_hes_sparsity, use_base2ad, use_in_parallel
    );
    // -----------------------------------------------------------------------
    // g(u; p)
    vector< AD<double> > au(2), av(1);
    au[0] = 5.0;
    au[1] = 6.0;
    CppAD::Independent(au);
    ax[0] = au[0];
    chk_f(ax, av);          // v[0] = p[0] * u[0]
    ay[0] = au[1] + av[0];  // y[0] = u[1] + p[0] * u[0]
    CppAD::ADFun<double> g(au, ay);
    // ---------------------------------------------------------------------
    ok &= g.Domain() == 2;
    ok &= g.Range() == 1;
    ok &= g.size_dyn_ind() == 0;
    //
    // evalute g(u; p)
    vector<double> p(1), u(2), y(1);
    p[0] = 3.0;
    u[0] = 4.0;
    u[1] = 5.0;
    chk_f.new_dynamic(p);
    y    = g.Forward(0, u);
    //
    // check value
    double check = u[1] + p[0] * u[0];
    ok &= y[0] == check;
    // ---------------------------------------------------------------------
    std::string graph = g.to_json();
    g.from_json(graph);
    // ---------------------------------------------------------------------
    ok &= g.Domain() == 2;
    ok &= g.Range() == 1;
    ok &= g.size_dyn_ind() == 0;
    //
    // evalute g(u; p)
    p[0] = 4.0;
    u[0] = 5.0;
    u[1] = 6.0;
    chk_f.new_dynamic(p);
    y    = g.Forward(0, u);
    //
    // check value
    check = u[1] + p[0] * u[0];
    ok &= y[0] == check;

    // -----------------------------------------------------------------------
    // std::cout << graph;
    return ok;
}
// ---------------------------------------------------------------------------
// Test atomic function that only gets passed dynamic parameters
bool atomic_dynamic(void)
{   bool ok = true;
    using CppAD::vector;
    using CppAD::AD;
    //
    // y[0] = x[0] * x[1]
    vector< AD<double> > ax(2), ay(1);
    ax[0] = 2.0;
    ax[1] = 3.0;
    CppAD::Independent(ax);
    ay[0] = ax[0] * ax[1];
    CppAD::ADFun<double> f(ax, ay);
    //
    // Create a ckhpoint_two with name f(x).
    // (This also creates an atomic_three fucntion with same name.)
    bool internal_bool    = false;
    bool use_hes_sparsity = false;
    bool use_base2ad      = false;
    bool use_in_parallel  = false;
    CppAD::chkpoint_two<double> chk_f(f, "f(x)",
        internal_bool, use_hes_sparsity, use_base2ad, use_in_parallel
    );
    // -----------------------------------------------------------------------
    vector< AD<double> > au(1), aq(2), av(1);
    aq[0] = 4.0;
    aq[1] = 5.0;
    au[0] = 6.0;
    size_t abort_op_index = 0;
    bool   record_compare = false;
    CppAD::Independent(au, abort_op_index, record_compare, aq);
    chk_f(aq, av);          // v[0] = q[0] * q[1]
    ay[0] = au[0] + av[0];  // y[0] = u[0] + q[0] * q[1]
    CppAD::ADFun<double> g(au, ay);
    //
    // ---------------------------------------------------------------------
    ok &= g.Domain() == 1;
    ok &= g.Range() == 1;
    ok &= g.size_dyn_ind() == 2;
    //
    // set q in g(u; q)
    vector<double> q(2);
    q[0] = 2.0;
    q[1] = 3.0;
    g.new_dynamic(q);
    //
    // evalute g(u; q)
    vector<double> u(1), y(1);
    u[0] = 4.0;
    y    = g.Forward(0, u);
    //
    // check value
    double check = u[0] + q[0] * q[1];
    ok &= y[0] == check;
    // ---------------------------------------------------------------------
    std::string graph = g.to_json();
    g.from_json(graph);
    //
    ok &= g.Domain() == 1;
    ok &= g.Range() == 1;
    ok &= g.size_dyn_ind() == 2;
    //
    // set q in g(u; q)
    q[0] = 3.0;
    q[1] = 4.0;
    g.new_dynamic(q);
    //
    // evalute g(u; q)
    u[0] = 5.0;
    y    = g.Forward(0, u);
    //
    // check value
    check = u[0] + q[0] * q[1];
    ok &= y[0] == check;
    // ----------------------------------------------------------------------
    // std::cout << graph;
    return ok;
}
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
    // node_4 : -2.0
    // node_5 : p[0] + x[0] + x[1]
    // node_6 : (p[0] + x[0] + x[1]) * (p[0] + x[0] + x[1])
    // y[0]   = (p[0] + x[0] + x[1]) * (p[0] + x[0] + x[1])
    // use single quote to avoid having to escape double quote
    std::string graph =
        "{\n"
        "   'function_name'  : 'to_json_and_back test',\n"
        "   'op_define_vec'  : [ 3, [\n"
        "       { 'op_code':1, 'name':'add', 'n_arg':2 } ,\n"
        "       { 'op_code':2, 'name':'mul', 'n_arg':2 } ,\n"
        "       { 'op_code':3, 'name':'sum'            } ]\n"
        "   ],\n"
        "   'n_dynamic_ind'  : 1,\n"
        "   'n_independent'  : 2,\n"
        "   'constant_vec'   : 1, [ -2.0 ],\n"
        "   'op_usage_vec'   : 2, [\n"
        "       [ 3, 1, 3, [1, 2, 3 ] ] ,\n"
        "       [ 2, 5, 5             ] ] \n"
        "   ,\n"
        "   'dependent_vec'   : 1, [6]\n"
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
    ok     &= cexp_lt_variable();
    ok     &= cexp_lt_constant();
    ok     &= cexp_lt_dynamic();
    ok     &= atomic_both();
    ok     &= atomic_dynamic();
    ok     &= to_json_and_back();
    ok     &= binary_operators();
    ok     &= cumulative_sum();
    //
    return ok;
}
