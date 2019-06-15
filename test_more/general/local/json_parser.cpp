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

bool json_parser(void)
{   bool ok = true;
    //
    // graph
    std::string graph =
        "{\n"
        "   \"n_dynamic\"      : 0,\n"
        "   \"n_independent\"  : 2,\n"
        "   \"string_vec\"     : [ 2, [ \"x\", \"y\" ] ],\n"
        "   \"constant_vec\"   : [ 1, [ -2.0 ] ],\n"
        "   \"operator_vec\"   : [ 1,\n"
        "       [\"mul\", 1, 1, [ 2, [1, 2] ] ]\n"
        "   ],\n"
        "   \"dependent_vec\"   : [ 1, [3] ],\n"
        "}\n";
    //
    // json_parser
    CppAD::local::json::parser json_parser(graph);
    //
    // {
    ok &= json_parser.token() == "{";
    // -----------------------------------------------------------------------
    // n_dynamic
    ok &= json_parser.next_string();
    ok &= json_parser.token() == "n_dynamic";
    //
    ok &= json_parser.check_next_char(':');
    //
    ok &= json_parser.next_non_neg_int();
    size_t n_dynamic = json_parser.token2size_t();
    //
    ok &= json_parser.check_next_char(',');
    //
    ok &= n_dynamic == 0;
    // -----------------------------------------------------------------------
    // n_independent
    ok &= json_parser.next_string();
    ok &= json_parser.token() == "n_independent";
    //
    ok &= json_parser.check_next_char(':');
    //
    ok &= json_parser.next_non_neg_int();
    size_t n_independent = json_parser.token2size_t();
    //
    ok &= json_parser.check_next_char(',');
    //
    ok &= n_independent == 2;
    // -----------------------------------------------------------------------
    // string_vec
    ok &= json_parser.next_string();
    ok &= json_parser.token() == "string_vec";
    //
    ok &= json_parser.check_next_char(':');
    ok &= json_parser.check_next_char('[');
    //
    ok &= json_parser.next_non_neg_int();
    size_t n_string = json_parser.token2size_t();
    CppAD::vector<std::string> string_vec(n_string);
    //
    ok &= json_parser.check_next_char(',');
    //
    // [ first_string, ... , last_string ]
    ok &= json_parser.check_next_char('[');
    for(size_t i = 0; i < n_string; ++i)
    {   ok           &= json_parser.next_string();
        string_vec[i] = json_parser.token();
        //
        if( i + 1 == n_string )
            ok &= json_parser.check_next_char(']');
        else
            ok &= json_parser.check_next_char(',');
    }
    //
    ok &= json_parser.check_next_char(']');
    ok &= json_parser.check_next_char(',');
    //
    ok &= string_vec.size() == 2;
    ok &= string_vec[0] == "x";
    ok &= string_vec[1] == "y";
    // -----------------------------------------------------------------------
    // constant_vec
    ok &= json_parser.next_string();
    ok &= json_parser.token() == "constant_vec";
    //
    ok &= json_parser.check_next_char(':');
    ok &= json_parser.check_next_char('[');
    //
    ok &= json_parser.next_non_neg_int();
    size_t n_constant = json_parser.token2size_t();
    CppAD::vector<double> constant_vec(n_constant);
    //
    ok &= json_parser.check_next_char(',');
    //
    // [ first_constant, ... , last_constant ]
    ok &= json_parser.check_next_char('[');
    for(size_t i = 0; i < n_constant; ++i)
    {   ok             &= json_parser.next_float();
        constant_vec[i] = json_parser.token2double();
        //
        if( i + 1 == n_constant )
            ok &= json_parser.check_next_char(']');
        else
            ok &= json_parser.check_next_char(',');
    }
    //
    ok &= json_parser.check_next_char(']');
    ok &= json_parser.check_next_char(',');
    //
    ok &= constant_vec.size() == 1;
    ok &= constant_vec[0] == -2.0;
    // -----------------------------------------------------------------------
    // operator_vec
    ok &= json_parser.next_string();
    ok &= json_parser.token() == "operator_vec";
    //
    ok &= json_parser.check_next_char(':');
    ok &= json_parser.check_next_char('[');
    //
    ok &= json_parser.next_non_neg_int();
    size_t n_operator = json_parser.token2size_t();
    using CppAD::local::json::operator_struct;
    CppAD::vector<operator_struct> operator_vec(n_operator);
    CppAD::vector<size_t>          operator_argument(0);
    //
    ok &= json_parser.check_next_char(',');
    //
    // [ first_operator, ... , last_operator ]
    ok &= json_parser.check_next_char('[');
    for(size_t i = 0; i < n_operator; ++i)
    {   // start next operator
        operator_struct op;
        //
        // name
        ok &= json_parser.next_string();
        std::string name = json_parser.token();
        //
        ok &= json_parser.check_next_char(',');
        //
        // code
        ok &= json_parser.next_non_neg_int();
        op.code = json_parser.token2size_t();
        ok &= name == CppAD::local::json::operator_name[ op.code ];
        //
        ok &= json_parser.check_next_char(',');
        //
        // n_result
        ok &= json_parser.next_non_neg_int();
        op.n_result = json_parser.token2size_t();
        //
        ok &= json_parser.check_next_char(',');
        ok &= json_parser.check_next_char('[');
        //
        // n_argument
        ok &= json_parser.next_non_neg_int();
        size_t n_argument = json_parser.token2size_t();
        op.n_argument = n_argument;
        //
        ok &= json_parser.check_next_char(',');
        //
        // [ first_argument_node, ... , last_argument_node ]
        ok &= json_parser.check_next_char('[');
        op.arg_index = operator_argument.size();
        for(size_t j = 0; j < n_argument; ++j)
        {   // next argument node
            ok &= json_parser.next_non_neg_int();
            size_t argument_node = json_parser.token2size_t();
            operator_argument.push_back( argument_node );
            //
            if( j + 1 == n_argument )
                ok &= json_parser.check_next_char(']');
            else
                ok &= json_parser.check_next_char(',');
        }
        ok &= json_parser.check_next_char(']');
        operator_vec[i] = op;
        //
        if( i + 1 == n_operator )
            ok &= json_parser.check_next_char(']');
        else
            ok &= json_parser.check_next_char(',');
    }
    //
    ok &= json_parser.check_next_char(']');
    ok &= json_parser.check_next_char(',');
    //
    ok &= operator_vec.size() == 1;
    ok &= operator_vec[0].code == size_t( CppAD::local::json::mul_operator );
    ok &= operator_vec[0].n_result == 1;
    ok &= operator_vec[0].n_argument == 2;
    size_t arg_index = operator_vec[0].arg_index;
    ok &= operator_argument[arg_index + 0] == 1;
    ok &= operator_argument[arg_index + 1] == 2;
    // -----------------------------------------------------------------------
    // dependent_vec
    ok &= json_parser.next_string();
    ok &= json_parser.token() == "dependent_vec";
    //
    ok &= json_parser.check_next_char(':');
    ok &= json_parser.check_next_char('[');
    //
    ok &= json_parser.next_non_neg_int();
    size_t n_dependent = json_parser.token2size_t();
    CppAD::vector<size_t> dependent_vec(n_dependent);
    //
    ok &= json_parser.check_next_char(',');
    //
    // [ first_dependent, ... , last_dependent ]
    ok &= json_parser.check_next_char('[');
    for(size_t i = 0; i < n_dependent; ++i)
    {   ok              &= json_parser.next_float();
        dependent_vec[i] = json_parser.token2size_t();
        //
        if( i + 1 == n_dependent )
            ok &= json_parser.check_next_char(']');
        else
            ok &= json_parser.check_next_char(',');
    }
    //
    ok &= json_parser.check_next_char(']');
    ok &= json_parser.check_next_char(',');
    //
    ok &= dependent_vec.size() == 1;
    ok &= dependent_vec[0] == 3;
   // -----------------------------------------------------------------------
    return ok;
}
