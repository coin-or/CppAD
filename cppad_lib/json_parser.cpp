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
# include <cppad/local/json/lexer.hpp>
# include <cppad/local/define.hpp>

CPPAD_LIB_EXPORT void CppAD::local::json::parser(
    const std::string&                        graph                  ,
    size_t&                                   n_dynamic_ind          ,
    size_t&                                   n_independent          ,
    CppAD::vector<std::string>&               string_vec             ,
    CppAD::vector<double>&                    constant_vec           ,
    CppAD::vector<operator_struct>&           operator_vec           ,
    CppAD::vector<size_t>&                    operator_arg           ,
    CppAD::vector<size_t>&                    dependent_vec          )
{
    //
    // json_lexer constructor checks for { at beginning
    CppAD::local::json::lexer json_lexer(graph);
    //
    // -----------------------------------------------------------------------
    // "n_dynamic_ind" : n_dynamic_ind ,
    json_lexer.check_next_string("n_dynamic_ind");
    json_lexer.check_next_char(':');
    //
    json_lexer.next_non_neg_int();
    n_dynamic_ind = json_lexer.token2size_t();
    //
    json_lexer.check_next_char(',');
    // -----------------------------------------------------------------------
    // "n_independent" : n_independent ,
    json_lexer.check_next_string("n_independent");
    json_lexer.check_next_char(':');
    //
    json_lexer.next_non_neg_int();
    n_independent = json_lexer.token2size_t();
    //
    json_lexer.check_next_char(',');
    // -----------------------------------------------------------------------
    // "string_vec" : [ n_string , [ first_string, ... , last_string ] ] ,
    json_lexer.check_next_string("string_vec");
    json_lexer.check_next_char(':');
    json_lexer.check_next_char('[');
    //
    json_lexer.next_non_neg_int();
    size_t n_string = json_lexer.token2size_t();
    string_vec.resize(n_string);
    //
    json_lexer.check_next_char(',');
    json_lexer.check_next_char('[');
    //
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
    json_lexer.check_next_char(']');
    json_lexer.check_next_char(',');
    // -----------------------------------------------------------------------
    // "constant_vec": [ n_constant, [ first_constant, ..., last_constant ] ],
    json_lexer.check_next_string("constant_vec");
    json_lexer.check_next_char(':');
    json_lexer.check_next_char('[');
    //
    json_lexer.next_non_neg_int();
    size_t n_constant = json_lexer.token2size_t();
    constant_vec.resize(n_constant);
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
    // -----------------------------------------------------------------------
    // "operator_vec": [ n_operator, [ first_operator, ..., last_operator ] ],
    json_lexer.check_next_string("operator_vec");
    json_lexer.check_next_char(':');
    json_lexer.check_next_char('[');
    //
    json_lexer.next_non_neg_int();
    size_t n_operator = json_lexer.token2size_t();
    operator_vec.resize(n_operator);
    operator_arg.resize(0);
    //
    json_lexer.check_next_char(',');
    json_lexer.check_next_char('[');
    //
    for(size_t i = 0; i < n_operator; ++i)
    {   // [ code, n_result, [ n_arg, [first_arg, ..., last_arg] ], namne ]
        json_lexer.check_next_char('[');
        //
        // code ,
        json_lexer.next_non_neg_int();
        operator_vec[i].code = operator_enum( json_lexer.token2size_t() );
        json_lexer.check_next_char(',');
        //
        // n_result ,
        json_lexer.next_non_neg_int();
        operator_vec[i].n_result = json_lexer.token2size_t();
        json_lexer.check_next_char(',');
        //
        // [ n_arg, [
        json_lexer.check_next_char('[');
        json_lexer.next_non_neg_int();
        size_t n_arg = json_lexer.token2size_t();
        operator_vec[i].n_arg = n_arg;
        json_lexer.check_next_char(',');
        json_lexer.check_next_char('[');
        //
        operator_vec[i].start_arg = operator_arg.size();
        for(size_t j = 0; j < n_arg; ++j)
        {   // next_arg
            json_lexer.next_non_neg_int();
            size_t argument_node = json_lexer.token2size_t();
            operator_arg.push_back( argument_node );
            //
            // , or ] at end
            if( j + 1 == n_arg )
                json_lexer.check_next_char(']');
            else
                json_lexer.check_next_char(',');
        }
        // ], name ]
        json_lexer.check_next_char(']');
        json_lexer.check_next_char(',');
        json_lexer.check_next_string( operator_name[operator_vec[i].code] );
        json_lexer.check_next_char(']');
        //
        // , or ] at end
        if( i + 1 == n_operator )
            json_lexer.check_next_char(']');
        else
            json_lexer.check_next_char(',');
    }
    //
    json_lexer.check_next_char(']');
    json_lexer.check_next_char(',');
    // -----------------------------------------------------------------------
    // "dependent_vec": [ n_dependent, [first_dependent, ..., last_dependent] ]
    json_lexer.check_next_string("dependent_vec");
    json_lexer.check_next_char(':');
    json_lexer.check_next_char('[');
    //
    json_lexer.next_non_neg_int();
    size_t n_dependent = json_lexer.token2size_t();
    dependent_vec.resize(n_dependent);
    json_lexer.check_next_char(',');
    json_lexer.check_next_char('[');
    //
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
    // -----------------------------------------------------------------------
    // end of Json object
    json_lexer.check_next_char('}');
    //
    return;
}
