/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */


# include <cppad/local/graph/json_lexer.hpp>
# include <cppad/local/define.hpp>
# include <cppad/local/atomic_index.hpp>
# include <cppad/utility/to_string.hpp>

// documentation for this routine is in the file below
# include <cppad/local/graph/json_parser.hpp>

CPPAD_LIB_EXPORT void CppAD::local::graph::json_parser(
    const std::string&                        json                   ,
    std::string&                              function_name          ,
    CppAD::vector<std::string>&               atomic_name_vec        ,
    size_t&                                   n_dynamic_ind          ,
    size_t&                                   n_independent          ,
    CppAD::vector<double>&                    constant_vec           ,
    CppAD::vector<graph_op_struct>&           operator_vec           ,
    CppAD::vector<size_t>&                    operator_arg           ,
    CppAD::vector<size_t>&                    dependent_vec          )
{   using std::string;
    const string match_any_string = "";
    //
    // initilize atomic_name_vec
    atomic_name_vec.resize(0);
    //
    // The values in this vector will be set while parsing op_define_vec.
    // Note that the values in op_code2enum[0] are not used.
    CppAD::vector<graph_op_enum> op_code2enum(1);
    //
    // -----------------------------------------------------------------------
    // json_lexer constructor checks for { at beginning
    CppAD::local::graph::json_lexer json_lexer(json);
    //
    // "function_name" : function_name
    json_lexer.check_next_string("function_name");
    json_lexer.check_next_char(':');
    json_lexer.check_next_string(match_any_string);
    function_name = json_lexer.token();
    json_lexer.set_function_name(function_name);
    json_lexer.check_next_char(',');
    //
    // -----------------------------------------------------------------------
    // "op_define_vec" : [ n_define, [
    json_lexer.check_next_string("op_define_vec");
    json_lexer.check_next_char(':');
    json_lexer.check_next_char('[');
    //
    json_lexer.next_non_neg_int();
    size_t n_define = json_lexer.token2size_t();
    json_lexer.check_next_char(',');
    //
    json_lexer.check_next_char('[');
    for(size_t i = 0; i < n_define; ++i)
    {   // {
        json_lexer.check_next_char('{');
        //
        // "op_code" : op_code,
        json_lexer.check_next_string("op_code");
        json_lexer.check_next_char(':');
        json_lexer.next_non_neg_int();
# ifndef NDEBUG
        size_t op_code = json_lexer.token2size_t();
        CPPAD_ASSERT_UNKNOWN( op_code == op_code2enum.size() );
# endif
        json_lexer.check_next_char(',');
        //
        // "name" : name
        json_lexer.check_next_string("name");
        json_lexer.check_next_char(':');
        json_lexer.check_next_string(match_any_string);
        string name = json_lexer.token();
        graph_op_enum op_enum = op_name2enum[name];
# if ! CPPAD_USE_CPLUSPLUS_2011
        switch( op_enum )
        {
            case local::graph::acosh_graph_op:
            case local::graph::asinh_graph_op:
            case local::graph::atanh_graph_op:
            case local::graph::erf_graph_op:
            case local::graph::erfc_graph_op:
            case local::graph::expm1_graph_op:
            case local::graph::log1p_graph_op:
            {   string expected = "a C++98 function";
                string found    = name + " which is a C++11 function.";
                json_lexer.report_error(expected, found);
            }
            break;

            default:
            break;
        }

# endif
        //
        // op_code2enum for this op_code
        op_code2enum.push_back(op_enum);
        //
        size_t n_arg = op_enum2fixed_n_arg[op_enum];
        if( n_arg > 0 )
        {   // , "narg" : n_arg
            json_lexer.check_next_char(',');
            json_lexer.check_next_string("n_arg");
            json_lexer.check_next_char(':');
            json_lexer.next_non_neg_int();
            if( n_arg != json_lexer.token2size_t() )
            {   string expected = CppAD::to_string(n_arg);
                string found    = json_lexer.token();
                json_lexer.report_error(expected, found);
            }
        }
        json_lexer.check_next_char('}');
        //
        // , (if not last entry)
        if( i + 1 < n_define )
            json_lexer.check_next_char(',');
    }
    json_lexer.check_next_char(']');
    // ],
    json_lexer.check_next_char(']');
    json_lexer.check_next_char(',');
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
        if( i + 1 < n_constant )
            json_lexer.check_next_char(',');
    }
    json_lexer.check_next_char(']');
    json_lexer.check_next_char(']');
    json_lexer.check_next_char(',');
    // -----------------------------------------------------------------------
    // "op_usage_vec": [ n_usage, [ first_op_usage, ..., last_op_usage ] ],
    json_lexer.check_next_string("op_usage_vec");
    json_lexer.check_next_char(':');
    json_lexer.check_next_char('[');
    //
    json_lexer.next_non_neg_int();
    size_t n_usage = json_lexer.token2size_t();
    operator_vec.resize(n_usage);
    operator_arg.resize(0);
    //
    json_lexer.check_next_char(',');
    //
    json_lexer.check_next_char('[');
    for(size_t i = 0; i < n_usage; ++i)
    {   // [ op_code,
        json_lexer.check_next_char('[');
        //
        // op_enum
        json_lexer.next_non_neg_int();
        graph_op_enum op_enum    = op_code2enum[ json_lexer.token2size_t() ];
        operator_vec[i].op_enum = op_enum;
        json_lexer.check_next_char(',');
        //
        size_t n_result = 1;
        size_t n_arg    = op_enum2fixed_n_arg[op_enum];
        //
        // check if number of arguments is fixed
        bool fixed      = n_arg > 0;
        size_t name_index = atomic_name_vec.size();
        if( ! fixed )
        {   if( op_enum == atom_graph_op )
            {   // name,
                json_lexer.check_next_string(match_any_string);
                string name = json_lexer.token();
                json_lexer.check_next_char(',');
                //
                for(size_t index = 0; index < atomic_name_vec.size(); ++index)
                {   if( atomic_name_vec[index] == name )
                        name_index = index;
                }
                if( name_index == atomic_name_vec.size() )
                    atomic_name_vec.push_back( name );
            }
            else CPPAD_ASSERT_UNKNOWN(
                op_enum == comp_eq_graph_op ||
                op_enum == comp_le_graph_op ||
                op_enum == comp_lt_graph_op ||
                op_enum == comp_ne_graph_op ||
                op_enum == sum_graph_op
            );
            // n_result,
            json_lexer.next_non_neg_int();
            n_result = json_lexer.token2size_t();
            json_lexer.check_next_char(',');
            //
            // n_arg, [
            json_lexer.next_non_neg_int();
            n_arg = json_lexer.token2size_t();
            json_lexer.check_next_char(',');
            json_lexer.check_next_char('[');
        }
        //
        // in the atom_graph_op case, name_index, n_result, n_arg
        // come before first argument
        if( op_enum == atom_graph_op )
        {   // name_index, n_result, n_arg come before start_arg
            CPPAD_ASSERT_UNKNOWN( name_index < atomic_name_vec.size() );
            operator_arg.push_back( name_index );
            operator_arg.push_back( n_result );
            operator_arg.push_back( n_arg );
        }
        if( op_enum == sum_graph_op )
        {   // n_arg comes before start_arg
            operator_arg.push_back( n_arg );
        }
        // start_arg
        operator_vec[i].start_arg = operator_arg.size();
        for(size_t j = 0; j < n_arg; ++j)
        {   // next_arg
            json_lexer.next_non_neg_int();
            size_t argument_node = json_lexer.token2size_t();
            operator_arg.push_back( argument_node );
            //
            // , (if not last entry)
            if( j + 1 < n_arg )
                json_lexer.check_next_char(',');
        }
        json_lexer.check_next_char(']');
        if( ! fixed )
            json_lexer.check_next_char(']');
        //
        // , (if not last entry)
        if( i + 1 < n_usage )
            json_lexer.check_next_char(',');
    }
    json_lexer.check_next_char(']');
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
    //
    json_lexer.check_next_char('[');
    for(size_t i = 0; i < n_dependent; ++i)
    {   json_lexer.next_float();
        dependent_vec[i] = json_lexer.token2size_t();
        //
        if( i + 1 < n_dependent )
            json_lexer.check_next_char(',');
    }
    json_lexer.check_next_char(']');
    json_lexer.check_next_char(']');
    // -----------------------------------------------------------------------
    // end of Json object
    json_lexer.check_next_char('}');
    //
    return;
}
