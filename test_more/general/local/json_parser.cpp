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
        "   \"string_vec\"     : [ 0, [] ],\n"
        "   \"constant_vec\"   : [ 0, [] ],\n"
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
    ok &= n_dynamic == 0;
    //
    ok &= json_parser.next_char();
    ok &= json_parser.token() == ",";
    // -----------------------------------------------------------------------
    // n_independent
    ok &= json_parser.next_string();
    ok &= json_parser.token() == "n_independent";
    //
    ok &= json_parser.next_char();
    ok &= json_parser.token() == ":";
    //
    ok &= json_parser.next_non_neg_int();
    size_t n_independent = std::atoi( json_parser.token().c_str() );
    ok &= n_independent == 2;
    //
    ok &= json_parser.next_char();
    ok &= json_parser.token() == ",";
    //
    //
    return ok;
}
