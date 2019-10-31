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
    using CppAD::local::graph::graph_op_struct;
    using CppAD::local::graph::cpp_graph;
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
    std::string json =
        "{\n"
        "   'function_name' : 'json_parser test',\n"
        "   'op_define_vec'  : [ 3, [\n"
        "       { 'op_code':1, 'name':'add', 'n_arg':2 } ,\n"
        "       { 'op_code':2, 'name':'mul', 'n_arg':2 } ,\n"
        "       { 'op_code':3, 'name':'sum'            } ]\n"
        "   ],\n"
        "   'n_dynamic_ind'  : 1,\n"
        "   'n_independent'  : 2,\n"
        "   'constant_vec'   : [ 1, [ -2.0 ] ],\n"
        "   'op_usage_vec'   : [ 2, [\n"
        "       [ 3, 1, 3, [1, 2, 3 ] ] ,\n"
        "       [ 2, 5, 5             ] ] \n"
        "   ],\n"
        "   'dependent_vec'   : [ 1, [6] ] \n"
        "}\n";
    // Convert the single quote to double quote
    for(size_t i = 0; i < json.size(); ++i)
        if( json[i] == '\'' ) json[i] = '"';
    //
    // C++ graph object
    cpp_graph graph_obj;
    //
    const std::string&         function_name(    graph_obj.function_name() );
    const size_t&              n_dynamic_ind(    graph_obj.n_dynamic_ind() );
    const size_t&              n_independent(    graph_obj.n_independent() );
    const vector<size_t>&      operator_arg(     graph_obj.operator_arg() );
    const vector<size_t>&      dependent_vec(    graph_obj.dependent_vec() );
    //
    // call parser
    CppAD::local::graph::json_parser( json, graph_obj );
    //
    ok &= function_name == "json_parser test";
    ok &= n_dynamic_ind == 1;
    ok &= n_independent == 2;
    ok &= graph_obj.atomic_name_vec_size() == 0;
    //
    ok &= graph_obj.constant_vec_size() == 1;
    ok &= graph_obj.constant_vec_get(0) == -2.0;
    //
    ok &= graph_obj.operator_vec_size() == 2;
    //
    size_t name_index, n_result;
    CppAD::vector<CppAD::local::graph::addr_t> arg;
    //
    CppAD::local::graph::graph_op_enum op_enum = graph_obj.operator_vec_get(0).op_enum;
    ok &= op_enum == CppAD::local::graph::sum_graph_op;
    size_t start_arg = graph_obj.operator_vec_get(0).start_arg;
    CppAD::local::graph::get_operator_info(
        op_enum, start_arg, operator_arg, name_index, n_result, arg
    );

    ok &= arg[0] == 1;
    ok &= arg[1] == 2;
    ok &= arg[2] == 3;
    //
    op_enum = graph_obj.operator_vec_get(1).op_enum;
    start_arg = graph_obj.operator_vec_get(1).start_arg;
    ok &= op_enum == CppAD::local::graph::mul_graph_op;
    CppAD::local::graph::get_operator_info(
        op_enum, start_arg, operator_arg, name_index, n_result, arg
    );
    ok &= arg.size() == 2;
    ok &= arg[0] == 5;
    ok &= arg[1] == 5;
    //
    ok &= dependent_vec.size() == 1;
    ok &= dependent_vec[0] == 6;
    // -----------------------------------------------------------------------
    //
    return ok;
}
