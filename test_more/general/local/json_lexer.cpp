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
    using CppAD::local::json::op_enum2fixed_n_arg;
    using CppAD::local::json::op_name2enum;
    using CppAD::local::json::operator_struct;
    //
    // match_any_string
    std::string match_any_string = "";
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
    // y[0] = (p[0] + x[0] + x[1]) * (p[0] + x[0] + x[1])
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
        "   'string_vec'     : [ 2, [ 'x', 'y' ] ],\n"
        "   'constant_vec'   : [ 1, [ -2.0 ] ],\n"
        "   'op_usage_vec'   : [ 2, [\n"
        "       [ 3, 1, 2, 3 ] ,\n"
        "       [ 2, 7, 7 ] ] \n"
        "   ],\n"
        "   'dependent_vec'   : [ 1, [8] ]\n"
        "}\n";
    // Convert the single quote to double quote
    for(size_t i = 0; i < graph.size(); ++i)
        if( graph[i] == '\'' ) graph[i] = '"';
    //
    // json_lexer constructor checks for { at beginning
    CppAD::local::json::lexer json_lexer(graph);
    // -----------------------------------------------------------------------
    // op_define_vec
    json_lexer.check_next_string("op_define_vec");
    json_lexer.check_next_char(':');
    json_lexer.check_next_char('[');
    //
    // n_define
    json_lexer.next_non_neg_int();
    size_t n_define = json_lexer.token2size_t();
    json_lexer.check_next_char(',');
    json_lexer.check_next_char('[');
    CppAD::vector<operator_enum> op_code2enum(1);
    for(size_t i = 0; i < n_define; ++i)
    {   json_lexer.check_next_char('{');
        //
        // op_code
        json_lexer.check_next_string("op_code");
        json_lexer.check_next_char(':');
        json_lexer.next_non_neg_int();
        size_t op_code = json_lexer.token2size_t();
        assert( op_code == op_code2enum.size() );
        json_lexer.check_next_char(',');
        //
        // name
        json_lexer.check_next_string("name");
        json_lexer.check_next_char(':');
        json_lexer.check_next_string(match_any_string);
        std::string   name   = json_lexer.token();
        operator_enum op_enum = op_name2enum[name];
        json_lexer.check_next_char(',');
        //
        // op_code2enum
        op_code2enum.push_back(op_enum);
        //
        // n_arg
        json_lexer.check_next_string("n_arg");
        json_lexer.check_next_char(':');
        json_lexer.next_non_neg_int();
        size_t n_arg = json_lexer.token2size_t();
        assert( n_arg == op_enum2fixed_n_arg[op_enum] );
        //
        json_lexer.check_next_char('}');
        if( i + 1 == n_define )
            json_lexer.check_next_char(']');
        else
            json_lexer.check_next_char(',');
    }
    json_lexer.check_next_char(']');
    json_lexer.check_next_char(',');
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
    for(size_t i = 0; i < n_string; ++i)
    {   json_lexer.check_next_string(match_any_string);
        string_vec[i] = json_lexer.token();
        //
        if( i + 1 == n_string )
            json_lexer.check_next_char(']');
        else
            json_lexer.check_next_char(',');
    }
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
    // op_usage_vec
    json_lexer.check_next_string("op_usage_vec");
    //
    json_lexer.check_next_char(':');
    json_lexer.check_next_char('[');
    //
    json_lexer.next_non_neg_int();
    size_t n_usage = json_lexer.token2size_t();
    CppAD::vector<operator_struct> operator_vec(n_usage);
    CppAD::vector<size_t>          operator_arg(0);
    //
    json_lexer.check_next_char(',');
    //
    // [ first_operator, ... , last_operator ]
    json_lexer.check_next_char('[');
    for(size_t i = 0; i < n_usage; ++i)
    {   // start next operator
        json_lexer.check_next_char('[');
        operator_struct op;
        //
        // code
        json_lexer.next_non_neg_int();
        size_t op_code = json_lexer.token2size_t();
        op.op_enum     = op_code2enum[op_code];
        json_lexer.check_next_char(',');
        //
        // n_result
        op.n_result = 1;
        //
        // n_arg
        op.n_arg = op_enum2fixed_n_arg[op.op_enum];
        //
        // first_arg_node, ... , last_arg_node
        op.start_arg = operator_arg.size();
        for(size_t j = 0; j < op.n_arg; ++j)
        {   // next argument node
            json_lexer.next_non_neg_int();
            size_t argument_node = json_lexer.token2size_t();
            operator_arg.push_back( argument_node );
            //
            if( j + 1 == op.n_arg )
                json_lexer.check_next_char(']');
            else
                json_lexer.check_next_char(',');
        }
        //
        // end of this operator
        operator_vec[i] = op;
        //
        if( i + 1 == n_usage )
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
    ok &= operator_vec[0].op_enum == CppAD::local::json::add_operator;
    ok &= operator_vec[0].n_result == 1;
    ok &= operator_vec[0].n_arg == 2;
    size_t start_arg = operator_vec[0].start_arg;
    ok &= operator_arg[start_arg + 0] == 1;
    ok &= operator_arg[start_arg + 1] == 2;
    //
    ok &= operator_vec[1].op_enum == CppAD::local::json::mul_operator;
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
