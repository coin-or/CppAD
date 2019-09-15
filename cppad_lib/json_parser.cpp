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
# include <cppad/local/atomic_index.hpp>
# include <cppad/utility/to_string.hpp>

CPPAD_LIB_EXPORT void CppAD::local::json::parser(
    const std::string&                        graph                  ,
    std::string&                              function_name          ,
    size_t&                                   n_dynamic_ind          ,
    size_t&                                   n_independent          ,
    CppAD::vector<std::string>&               atomic_name_vec        ,
    CppAD::vector<double>&                    constant_vec           ,
    CppAD::vector<json_op_struct>&            operator_vec           ,
    CppAD::vector<size_t>&                    operator_arg           ,
    CppAD::vector<size_t>&                    dependent_vec          )
{   using std::string;
    const std::string match_any_string = "";
    size_t max_size_t = std::numeric_limits<size_t>::max();
    //
    // atomic_name_vec
    bool        set_null = true;
    size_t      index_in = 0;
    size_t      type;
    string      name;
    void*       ptr;
    size_t n_atomic = CppAD::local::atomic_index<double>(
        set_null, index_in, type, &name, ptr
    );
    atomic_name_vec.resize(n_atomic + 1 );
    atomic_name_vec[0] = "";
    set_null = false;
    for(index_in = 1; index_in <= n_atomic; ++index_in)
    {   CppAD::local::atomic_index<double>(
            set_null, index_in, type, &name, ptr
        );
        if( type == 3 )
            atomic_name_vec[index_in] = name;
        else
            atomic_name_vec[index_in] = "";
    }
    //
    // The values in this vector will be set while parsing op_devine_vec.
    // Note that the values in op_code2enum[0] are not used.
    CppAD::vector<json_op_enum> op_code2enum(1);
    //
    // -----------------------------------------------------------------------
    // json_lexer constructor checks for { at beginning
    CppAD::local::json::lexer json_lexer(graph);
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
        assert( op_code == op_code2enum.size() );
# endif
        json_lexer.check_next_char(',');
        //
        // "name" : name
        json_lexer.check_next_string("name");
        json_lexer.check_next_char(':');
        json_lexer.check_next_string(match_any_string);
        name                 = json_lexer.token();
        json_op_enum op_enum = op_name2enum[name];
# if ! CPPAD_USE_CPLUSPLUS_2011
        switch( op_enum )
        {
            case local::json::acosh_json_op:
            case local::json::asinh_json_op:
            case local::json::atanh_json_op:
            case local::json::erf_json_op:
            case local::json::erfc_json_op:
            case local::json::expm1_json_op:
            case local::json::log1p_json_op:
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
    // "constant_vec": n_constant, [ first_constant, ..., last_constant ],
    json_lexer.check_next_string("constant_vec");
    json_lexer.check_next_char(':');
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
    json_lexer.check_next_char(',');
    // -----------------------------------------------------------------------
    // "op_usage_vec": n_usage, [ first_op_usage, ..., last_op_usage ],
    json_lexer.check_next_string("op_usage_vec");
    json_lexer.check_next_char(':');
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
        json_op_enum op_enum    = op_code2enum[ json_lexer.token2size_t() ];
        operator_vec[i].op_enum = op_enum;
        operator_vec[i].extra   = max_size_t;
        json_lexer.check_next_char(',');
        //
        size_t n_result = 1;
        size_t n_arg    = op_enum2fixed_n_arg[op_enum];
        //
        // check if number of arguments is fixed
        bool fixed      = n_arg > 0;
        if( ! fixed )
        {   if( op_enum == atom_json_op )
            {   // name,
                json_lexer.check_next_string(match_any_string);
                name = json_lexer.token();
                for(size_t index = 1; index < atomic_name_vec.size(); ++index)
                {   if( atomic_name_vec[index] == name )
                    {   if( operator_vec[i].extra != max_size_t )
                        {   string expected = "unique atomic function name";
                            string found    = name;
                            json_lexer.report_error(expected, found);
                        }
                        operator_vec[i].extra = index;
                    }
                }
                if( operator_vec[i].extra == max_size_t )
                {   string expected = "a valid atomic function name";
                    string found    = name;
                    json_lexer.report_error(expected, found);
                }
                json_lexer.check_next_char(',');
            }
            else if( op_enum == comp_eq_json_op ||
                op_enum == comp_le_json_op ||
                op_enum == comp_lt_json_op )
            {   // flag
                json_lexer.next_non_neg_int();
                size_t flag = json_lexer.token2size_t();
                if( flag != 0 && flag != 1 )
                {   string expected = "flag not 0 or 1 in compare operator";
                    string found    = json_lexer.token();
                    json_lexer.report_error(expected, found);
                }
                json_lexer.check_next_char(',');
            }
            else
            {   assert( op_enum == sum_json_op );
            }
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
        operator_vec[i].n_result = n_result;
        operator_vec[i].n_arg    = n_arg;
        //
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
    json_lexer.check_next_char(',');
    // -----------------------------------------------------------------------
    // "dependent_vec": [ n_dependent, [first_dependent, ..., last_dependent] ]
    json_lexer.check_next_string("dependent_vec");
    json_lexer.check_next_char(':');
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
    // -----------------------------------------------------------------------
    // end of Json object
    json_lexer.check_next_char('}');
    //
    return;
}
