/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

# include <cppad/local/json/parser.hpp>

bool json_parser(void)
{   bool ok = true;
    using CppAD::local::json::operator_struct;
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
    // parser return values
    size_t                             n_dynamic_ind;
    size_t                             n_independent;
    CppAD::vector<std::string>         string_vec;
    CppAD::vector<double>              constant_vec;
    CppAD::vector<operator_struct>     operator_vec;
    CppAD::vector<size_t>              operator_arg;
    CppAD::vector<size_t>              dependent_vec;
    //
    // call parser
    CppAD::local::json::parser(
        graph,
        n_dynamic_ind,
        n_independent,
        string_vec,
        constant_vec,
        operator_vec,
        operator_arg,
        dependent_vec
    );
    //
    ok &= n_dynamic_ind == 0;
    ok &= n_independent == 2;
    //
    ok &= string_vec.size() == 2;
    ok &= string_vec[0] == "x";
    ok &= string_vec[1] == "y";
    //
    ok &= constant_vec.size() == 1;
    ok &= constant_vec[0] == -2.0;
    //
    ok &= operator_vec.size() == 2;
    //
    ok &= operator_vec[0].code == CppAD::local::json::add_operator;
    ok &= operator_vec[0].n_result == 1;
    ok &= operator_vec[0].n_arg == 2;
    size_t start_arg = operator_vec[0].start_arg;
    ok &= operator_arg[start_arg + 0] == 1;
    ok &= operator_arg[start_arg + 1] == 2;
    //
    ok &= operator_vec[1].code == CppAD::local::json::mul_operator;
    ok &= operator_vec[1].n_result == 1;
    ok &= operator_vec[1].n_arg == 2;
    start_arg = operator_vec[1].start_arg;
    ok &= operator_arg[start_arg + 0] == 6;
    ok &= operator_arg[start_arg + 1] == 6;
    //
    ok &= dependent_vec.size() == 1;
    ok &= dependent_vec[0] == 7;
    // -----------------------------------------------------------------------
    //
    return ok;
}
