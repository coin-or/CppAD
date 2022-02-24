/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

# include <cppad/local/pod_vector.hpp>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/core/graph/cpp_graph.hpp>
# include <cppad/utility/to_string.hpp>

// documentation for this routine is in the file below
# include <cppad/local/graph/csrc_writer.hpp>

/* Optimizations 2DO:
1. Reduce size of v by removing x and y from the v vector.
2. Reduce size of v by reusing elemnents when they are no longer needed.
*/


namespace {
    void binary_operator(
        std::string& csrc   ,
        const char* op      ,
        size_t result_node  ,
        size_t left_node    ,
        size_t right_node   )
    {   csrc += "\tv[" + CppAD::to_string(result_node) + "] = ";
        csrc += "v[" + CppAD::to_string(left_node) + "] " + op + " ";
        csrc += "v[" + CppAD::to_string(right_node) + "];\n";
    }
}

void CppAD::local::graph::csrc_writer(
    std::string&                              csrc                   ,
    const cpp_graph&                          graph_obj              )
{   using std::string;
    using CppAD::to_string;
    //
    // --------------------------------------------------------------------
    string function_name  = graph_obj.function_name_get();
    size_t n_dynamic_ind  = graph_obj.n_dynamic_ind_get();
    size_t n_variable_ind = graph_obj.n_variable_ind_get();
    size_t n_dependent    = graph_obj.dependent_vec_size();
    size_t n_usage        = graph_obj.operator_vec_size();
    // --------------------------------------------------------------------
    CPPAD_ASSERT_KNOWN( function_name != "" ,
        "to_csrc: Cannot convert a fucntion with no name"
    );
    //
    // graph_itr
    // defined here because not using as loop index
    cpp_graph::const_iterator graph_itr;
    //
    // first_result_node
    size_t first_result_node = 1 + n_dynamic_ind + n_variable_ind;
    //
    // n_node
    size_t n_node = first_result_node;
    for(size_t op_index = 0; op_index < n_usage; ++op_index)
    {   // graph_itr
        if( op_index == 0 )
            graph_itr = graph_obj.begin();
        else
            ++graph_itr;
        //
        // nv
        cpp_graph::const_iterator::value_type itr_value = *graph_itr;
        n_node += itr_value.n_result;
    }
    //
    // initialize
    csrc = "";
    //
    csrc +=
        "// includes\n"
        "# include <stddef.h>\n"
        "# include <assert.h>\n";
    //
    csrc +=
        "\n"
        "// prototype\n"
        "void " + function_name + "(\n"
        "\tsize_t         call_id         ,\n"
        "\tsize_t         n_x             ,\n"
        "\tconst double*  x               ,\n"
        "\tsize_t         n_y             ,\n"
        "\tdouble*        y               ,\n"
        "\tsize_t*        compare_change  )\n";
    //
    csrc += "{\t// begin function body \n";
    //
    // v, i
    csrc +=
        "\t// declare variables\n"
        "\tdouble v[" + to_string(n_node) + "];\n"
        "\tsize_t i;\n"
        "\tdouble nan = 0.0 / 0.0;\n"
        "\n";
    //
    // n_x
    size_t n_x = n_dynamic_ind + n_variable_ind;
    csrc += "\tassert( n_x == " + to_string(n_x) + ");\n";
    //
    // n_y
    size_t n_y = n_dependent;
    csrc += "\tassert( n_y == " + to_string(n_y) + ");\n";
    //
    csrc +=
        "\n"
        "\t*compare_change = 0;   // initialize\n"
        "\tv[0]            = nan; // set v[0]\n";
    //
    csrc +=
        "\n"
        "\t// set v[i] for i = 1, ..., nx\n"
        "\tfor(i = 0; i < n_x; ++i)\n"
        "\t\tv[1+i] = x[i];\n";
    //
    csrc +=
        "\n"
        "\t// set v[i] for i = n_x+1, ..., " + to_string(n_node+1) + "\n";
    //
    // arg
    // defined here to avoid memory re-allocation for each operator
    vector<size_t> arg;
    //
    // result_node
    size_t result_node = first_result_node;
    //
    // op_index
    for(size_t op_index = 0; op_index < n_usage; ++op_index)
    {   //
        // graph_itr
        if( op_index == 0 )
            graph_itr = graph_obj.begin();
        else
            ++graph_itr;
        //
        // str_index, op_enum, call_id, n_result, n_arg, arg
        cpp_graph::const_iterator::value_type itr_value = *graph_itr;
        // const vector<size_t>& str_index( *itr_value.str_index_ptr );
        graph_op_enum op_enum    = itr_value.op_enum;
        // size_t        call_id    = itr_value.call_id;
        size_t        n_result   = itr_value.n_result;
        size_t        n_arg      = itr_value.arg_node_ptr->size();
        arg.resize(n_arg);
        arg                      = *(itr_value.arg_node_ptr);
        CPPAD_ASSERT_UNKNOWN( n_arg > 0 );
        //
        switch( op_enum )
        {   //
            // add
            case add_graph_op:
            CPPAD_ASSERT_UNKNOWN( n_result == 1 );
            binary_operator(csrc, "+", result_node, arg[0], arg[1]);
            break;
            //
            // div
            case div_graph_op:
            CPPAD_ASSERT_UNKNOWN( n_result == 1 );
            binary_operator(csrc, "/", result_node, arg[0], arg[1]);
            break;
            //
            // mul
            case mul_graph_op:
            CPPAD_ASSERT_UNKNOWN( n_result == 1 );
            binary_operator(csrc, "*", result_node, arg[0], arg[1]);
            break;
            //
            // sub
            case sub_graph_op:
            CPPAD_ASSERT_UNKNOWN( n_result == 1 );
            binary_operator(csrc, "-", result_node, arg[0], arg[1]);
            break;

            default:
            {   string msg = op_enum2name[op_enum];
                msg = "f.to_csrc: The " + msg + " is not yet implemented.";
                CPPAD_ASSERT_KNOWN(false, msg.c_str() );
            }
            break;
        }
        //
        // result_node
        result_node += n_result;
    }
    // ----------------------------------------------------------------------
    // dependent
    csrc +=
        "\n"
        "\t// set y[i] for i = 0, n_y-1\n";
    for(size_t i = 0; i < n_y; ++i)
    {   size_t node = graph_obj.dependent_vec_get(i);
        csrc += "\ty[" + to_string(i) + "] = ";
        csrc += "v[" + to_string( node ) + "];\n";
    }
    // ----------------------------------------------------------------------
    // end function body
    csrc += "\n";
    csrc += "\treturn;\n";
    csrc += "}\n";
    //
    return;
}
