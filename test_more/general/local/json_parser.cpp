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
        "   \"operator_vec\"   : [ 0,\n"
        "       [\"mul\", 1, 1, 2, [1, 2] ] ],\n"
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
    ok &= json_parser.next_char();
    ok &= json_parser.token() == ":";
    //
    ok &= json_parser.next_non_neg_int();
    size_t n_dynamic = std::atoi( json_parser.token().c_str() );
    //
    ok &= json_parser.next_char();
    ok &= json_parser.token() == ",";
    //
    ok &= n_dynamic == 0;
    // -----------------------------------------------------------------------
    // n_independent
    ok &= json_parser.next_string();
    ok &= json_parser.token() == "n_independent";
    //
    ok &= json_parser.next_char();
    ok &= json_parser.token() == ":";
    //
    ok &= json_parser.next_non_neg_int();
    size_t n_independent = size_t ( std::atoi( json_parser.token().c_str() ) );
    //
    ok &= json_parser.next_char();
    ok &= json_parser.token() == ",";
    //
    ok &= n_independent == 2;
    // -----------------------------------------------------------------------
    // string_vec
    ok &= json_parser.next_string();
    ok &= json_parser.token() == "string_vec";
    //
    ok &= json_parser.next_char();
    ok &= json_parser.token() == ":";
    //
    ok &= json_parser.next_char();
    ok &= json_parser.token() == "[";
    //
    ok &= json_parser.next_non_neg_int();
    size_t n_string = size_t( std::atoi( json_parser.token().c_str() ) );
    CppAD::vector<std::string> string_vec(n_string);
    //
    ok &= json_parser.next_char();
    ok &= json_parser.token() == ",";
    //
    ok &= json_parser.next_char();
    ok &= json_parser.token() == "[";
    //
    for(size_t i = 0; i < n_string; ++i)
    {   ok           &= json_parser.next_string();
        string_vec[i] = json_parser.token();
        //
        ok &= json_parser.next_char();
        if( i + 1 == n_string )
            ok &= json_parser.token() == "]";
        else
            ok &= json_parser.token() == ",";
    }
    //
    ok &= json_parser.next_char();
    ok &= json_parser.token() == "]";
    //
    ok &= json_parser.next_char();
    ok &= json_parser.token() == ",";
    //
    ok &= string_vec.size() == 2;
    ok &= string_vec[0] == "x";
    ok &= string_vec[1] == "y";
    // -----------------------------------------------------------------------
    // constant_vec
    ok &= json_parser.next_string();
    ok &= json_parser.token() == "constant_vec";
    //
    ok &= json_parser.next_char();
    ok &= json_parser.token() == ":";
    //
    ok &= json_parser.next_char();
    ok &= json_parser.token() == "[";
    //
    ok &= json_parser.next_non_neg_int();
    size_t n_constant = size_t( std::atoi( json_parser.token().c_str() ) );
    CppAD::vector<double> constant_vec(n_constant);
    //
    ok &= json_parser.next_char();
    ok &= json_parser.token() == ",";
    //
    ok &= json_parser.next_char();
    ok &= json_parser.token() == "[";
    //
    for(size_t i = 0; i < n_constant; ++i)
    {   ok             &= json_parser.next_float();
        constant_vec[i] = std::atof( json_parser.token().c_str() );
        //
        ok &= json_parser.next_char();
        if( i + 1 == n_constant )
            ok &= json_parser.token() == "]";
        else
            ok &= json_parser.token() == ",";
    }
    //
    ok &= json_parser.next_char();
    ok &= json_parser.token() == "]";
    //
    ok &= json_parser.next_char();
    ok &= json_parser.token() == ",";
    //
    ok &= constant_vec.size() == 1;
    ok &= constant_vec[0] == -2.0;
    // -----------------------------------------------------------------------
    return ok;
}
