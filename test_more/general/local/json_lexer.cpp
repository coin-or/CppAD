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

bool json_lexer(void)
{   bool ok = true;
    typedef CppAD::local::json::operator_enum operator_enum;
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
    // json_lexer constructor checks for { at beginning
    CppAD::local::json::lexer json_lexer(graph);
    // -----------------------------------------------------------------------
    // n_dynamic_ind
    json_lexer.check_next_string("n_dynamic_ind");
    //
    json_lexer.check_next_char(':');
    //
    json_lexer.next_non_neg_int();
    size_t n_dynamic_ind = json_lexer.token2size_t();
    //
    json_lexer.check_next_char(',');
    //
    ok &= n_dynamic_ind == 0;
    // -----------------------------------------------------------------------
    // n_independent
    json_lexer.check_next_string("n_independent");
    //
    json_lexer.check_next_char(':');
    //
    json_lexer.next_non_neg_int();
    size_t n_independent = json_lexer.token2size_t();
    //
    json_lexer.check_next_char(',');
    //
    ok &= n_independent == 2;
    // -----------------------------------------------------------------------
    // string_vec
    json_lexer.check_next_string("string_vec");
    //
    json_lexer.check_next_char(':');
    json_lexer.check_next_char('[');
    //
    json_lexer.next_non_neg_int();
    size_t n_string = json_lexer.token2size_t();
    CppAD::vector<std::string> string_vec(n_string);
    //
    json_lexer.check_next_char(',');
    //
    // [ first_string, ... , last_string ]
    json_lexer.check_next_char('[');
    std::string match_any_string = "";
    for(size_t i = 0; i < n_string; ++i)
    {   json_lexer.check_next_string(match_any_string);
        string_vec[i] = json_lexer.token();
        //
        if( i + 1 == n_string )
            json_lexer.check_next_char(']');
        else
            json_lexer.check_next_char(',');
    }
    //
    json_lexer.check_next_char(']');
    json_lexer.check_next_char(',');
    //
    ok &= string_vec.size() == 2;
    ok &= string_vec[0] == "x";
    ok &= string_vec[1] == "y";
    // -----------------------------------------------------------------------
    // constant_vec
    json_lexer.check_next_string("constant_vec");
    //
    json_lexer.check_next_char(':');
    json_lexer.check_next_char('[');
    //
    json_lexer.next_non_neg_int();
    size_t n_constant = json_lexer.token2size_t();
    CppAD::vector<double> constant_vec(n_constant);
    //
    json_lexer.check_next_char(',');
    //
    // [ first_constant, ... , last_constant ]
    json_lexer.check_next_char('[');
    for(size_t i = 0; i < n_constant; ++i)
    {   json_lexer.next_float();
        constant_vec[i] = json_lexer.token2double();
        //
        if( i + 1 == n_constant )
            json_lexer.check_next_char(']');
        else
            json_lexer.check_next_char(',');
    }
    //
    json_lexer.check_next_char(']');
    json_lexer.check_next_char(',');
    //
    ok &= constant_vec.size() == 1;
    ok &= constant_vec[0] == -2.0;
    // -----------------------------------------------------------------------
    // operator_vec
    json_lexer.check_next_string("operator_vec");
    //
    json_lexer.check_next_char(':');
    json_lexer.check_next_char('[');
    //
    json_lexer.next_non_neg_int();
    size_t n_operator = json_lexer.token2size_t();
    using CppAD::local::json::operator_struct;
    CppAD::vector<operator_struct> operator_vec(n_operator);
    CppAD::vector<size_t>          operator_arg(0);
    //
    json_lexer.check_next_char(',');
    //
    // [ first_operator, ... , last_operator ]
    json_lexer.check_next_char('[');
    for(size_t i = 0; i < n_operator; ++i)
    {   // start next operator
        json_lexer.check_next_char('[');
        operator_struct op;
        //
        // code
        json_lexer.next_non_neg_int();
        op.code = operator_enum( json_lexer.token2size_t() );
        //
        json_lexer.check_next_char(',');
        //
        // n_result
        json_lexer.next_non_neg_int();
        op.n_result = json_lexer.token2size_t();
        //
        json_lexer.check_next_char(',');
        //
        // n_arg
        json_lexer.next_non_neg_int();
        size_t n_arg = json_lexer.token2size_t();
        op.n_arg = n_arg;
        json_lexer.check_next_char(',');
        //
        // [ first_arg_node, ... , last_arg_node ],
        json_lexer.check_next_char('[');
        op.start_arg = operator_arg.size();
        for(size_t j = 0; j < n_arg; ++j)
        {   // next argument node
            json_lexer.next_non_neg_int();
            size_t argument_node = json_lexer.token2size_t();
            operator_arg.push_back( argument_node );
            //
            if( j + 1 == n_arg )
                json_lexer.check_next_char(']');
            else
                json_lexer.check_next_char(',');
        }
        json_lexer.check_next_char(',');
        //
        // name
        using CppAD::local::json::operator_name;
        json_lexer.check_next_string( operator_name[op.code] );
        json_lexer.check_next_char(']');
        //
        // end of this operator
        operator_vec[i] = op;
        //
        if( i + 1 == n_operator )
            json_lexer.check_next_char(']');
        else
            json_lexer.check_next_char(',');
    }
    json_lexer.check_next_char(']');
    //
    json_lexer.check_next_char(',');
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
    // -----------------------------------------------------------------------
    // dependent_vec
    json_lexer.check_next_string("dependent_vec");
    //
    json_lexer.check_next_char(':');
    json_lexer.check_next_char('[');
    //
    json_lexer.next_non_neg_int();
    size_t n_dependent = json_lexer.token2size_t();
    CppAD::vector<size_t> dependent_vec(n_dependent);
    //
    json_lexer.check_next_char(',');
    //
    // [ first_dependent, ... , last_dependent ]
    json_lexer.check_next_char('[');
    for(size_t i = 0; i < n_dependent; ++i)
    {   json_lexer.next_float();
        dependent_vec[i] = json_lexer.token2size_t();
        //
        if( i + 1 == n_dependent )
            json_lexer.check_next_char(']');
        else
            json_lexer.check_next_char(',');
    }
    //
    json_lexer.check_next_char(']');
    //
    ok &= dependent_vec.size() == 1;
    ok &= dependent_vec[0] == 7;
    // -----------------------------------------------------------------------
    // }
    json_lexer.check_next_char('}');
    //
    return ok;
}
