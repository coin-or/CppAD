/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

# include <cppad/local/graph/json_parser.hpp>

bool json_parser(void)
{   bool ok = true;
    using CppAD::local::graph::json_op_struct;
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
        "   'function_name' : 'json_parser test',\n"
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
    // parser return values
    std::string                        function_name;
    size_t                             n_dynamic_ind;
    size_t                             n_independent;
    CppAD::vector<std::string>         atomic_name_vec;
    CppAD::vector<double>              constant_vec;
    CppAD::vector<json_op_struct>      operator_vec;
    CppAD::vector<size_t>              operator_arg;
    CppAD::vector<size_t>              dependent_vec;
    //
    // call parser
    CppAD::local::graph::parser(
        graph,
        function_name,
        n_dynamic_ind,
        n_independent,
        atomic_name_vec,
        constant_vec,
        operator_vec,
        operator_arg,
        dependent_vec
    );
    //
    ok &= function_name == "json_parser test";
    ok &= n_dynamic_ind == 1;
    ok &= n_independent == 2;
    ok &= atomic_name_vec.size() == 0;
    //
    ok &= constant_vec.size() == 1;
    ok &= constant_vec[0] == -2.0;
    //
    ok &= operator_vec.size() == 2;
    //
    ok &= operator_vec[0].op_enum == CppAD::local::graph::sum_json_op;
    ok &= operator_vec[0].n_result == 1;
    ok &= operator_vec[0].n_arg == 3;
    size_t start_arg = operator_vec[0].start_arg;
    ok &= operator_arg[start_arg + 0] == 1;
    ok &= operator_arg[start_arg + 1] == 2;
    ok &= operator_arg[start_arg + 2] == 3;
    //
    ok &= operator_vec[1].op_enum == CppAD::local::graph::mul_json_op;
    ok &= operator_vec[1].n_result == 1;
    ok &= operator_vec[1].n_arg == 2;
    start_arg = operator_vec[1].start_arg;
    ok &= operator_arg[start_arg + 0] == 5;
    ok &= operator_arg[start_arg + 1] == 5;
    //
    ok &= dependent_vec.size() == 1;
    ok &= dependent_vec[0] == 6;
    // -----------------------------------------------------------------------
    //
    return ok;
}
