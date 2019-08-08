# ifndef CPPAD_CORE_JSON_TO_JSON_HPP
# define CPPAD_CORE_JSON_TO_JSON_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

# include <cppad/core/ad_fun.hpp>
# include <cppad/local/op_code_dyn.hpp>

/*
$begin to_json$$
$spell
    Json
$$

$section Create a Json AD Graph Corresponding to an ADFun Object$$

$head Under Construction$$
This routine is under construction and subject to
change without backward compatibility.

$head Syntax$$
$codei%
    %graph% = %fun%.to_json()
%$$

$head Prototype$$
$srcfile%include/cppad/core/json/to_json.hpp%
    0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1
%$$

$head fun$$
is the $cref/ADFun/adfun/$$ object.

$head graph$$
The return value of $icode graph$$ is a
$cref json_ad_graph$$ representation of the corresponding function.

$head Base$$
is the type corresponding to this $cref/ADFun/adfun/$$ object;
i.e., its calculations are done using the type $icode Base$$.

$head RecBase$$
in the prototype above, $icode RecBase$$ is the same type as $icode Base$$.

$childtable%
    example/json/to_json.cpp
%$$
$head Example$$
The file $cref to_json.cpp$$ is an example and test of this operation.

$end
*/
// BEGIN_PROTOTYPE
template <class Base, class RecBase>
std::string CppAD::ADFun<Base,RecBase>::to_json(void)
// END_PROTOTYPE
{   using local::pod_vector;
    using local::opcode_t;
    // --------------------------------------------------------------------
    // some constants
    // --------------------------------------------------------------------
    // dynamic parameter information
    const pod_vector<opcode_t>& dyn_par_op ( play_.dyn_par_op()  );
    const pod_vector<addr_t>&   dyn_par_arg( play_.dyn_par_arg() );
    const pod_vector<addr_t>&   dyn_ind2par_ind ( play_.dyn_ind2par_ind() );
    const pod_vector<bool>&     dyn_par_is( play_.dyn_par_is() );
    //
    // number of dynamic parameters
    const size_t n_dynamic     = dyn_ind2par_ind.size();
    //
    // number of independent dynamic parameters
    const size_t n_dynamic_ind = play_.num_dynamic_ind();
    //
    // number of parameters
    const size_t n_parameter = play_.num_par_rec();
    //
    // number of constant parameters
    const size_t n_constant = n_parameter - n_dynamic - 1;
    //
    // number of independent variables
    const size_t n_independent = ind_taddr_.size();
    //
    // number of string constants
    const size_t n_string = 0;
    //
    // value of parameters
    const Base* parameter = play_.GetPar();
    //
    // number of dynamic parameter operators
    const size_t n_dynamic_op = n_dynamic - n_dynamic_ind;
    //
    // number of variables
    const size_t n_variable = play_.num_var_rec();
    //
    // number of json operators
    const size_t n_json_op = size_t( local::json::n_json_op );
    //
    // some checks
    CPPAD_ASSERT_UNKNOWN( n_dynamic_ind <= n_dynamic );
    CPPAD_ASSERT_UNKNOWN( dyn_par_is.size() == n_parameter );
    CPPAD_ASSERT_UNKNOWN( n_parameter > 0 );
    CPPAD_ASSERT_UNKNOWN( isnan( parameter[0] ) );
    CPPAD_ASSERT_UNKNOWN( ! dyn_par_is[0] );
    // -----------------------------------------------------------------------
    // Set n_usage and is_json_op_used corresponding to dynmaic operators
    size_t n_usage = 0;
    pod_vector<bool> is_json_op_used(n_json_op);
    for(size_t i = 0; i < n_json_op; ++i)
        is_json_op_used[i] = false;
    //
    std::string error_message =
        "to_json not yet implemented for following dynamic operator: ";
    for(size_t i_dyn = n_dynamic_ind; i_dyn < n_dynamic; ++i_dyn)
    {   // operator for this dynamic parameter
        local::op_code_dyn dyn_op = local::op_code_dyn( dyn_par_op[i_dyn] );
        //
        switch(dyn_op)
        {
            case local::add_dyn:
            is_json_op_used[local::json::add_json_op] = true;
            break;

            case local::mul_dyn:
            is_json_op_used[local::json::mul_json_op] = true;
            break;

            case local::sub_dyn:
            is_json_op_used[local::json::sub_json_op] = true;
            break;

            default:
            error_message += op_name_dyn(dyn_op);
            CPPAD_ASSERT_KNOWN( false, error_message.c_str() );
            break;
        }
    }
    n_usage += n_dynamic_op;
    // --------------------------------------------------------------------
    // Update n_usage and is_json_op_used to include variable operators
    //
    local::play::const_sequential_iterator itr = play_.begin();
    local::OpCode var_op;
    const addr_t* arg;
    size_t        i_var;
    itr.op_info(var_op, arg, i_var);
    CPPAD_ASSERT_UNKNOWN( var_op == local::BeginOp ); // skip BeginOp
    //
    bool more_operators  = true;
    error_message        =
        "to_json not yet implemented for following variable operator: ";
    while(more_operators)
    {
        // next op
        (++itr).op_info(var_op, arg, i_var);
        switch( var_op )
        {
            // -------------------------------------------------------------
            // Ignore all comparison operators (for now)
            case local::EqppOp:
            case local::EqpvOp:
            case local::EqvvOp:
            case local::NeppOp:
            case local::NepvOp:
            case local::NevvOp:
            //
            case local::LtppOp:
            case local::LtpvOp:
            case local::LtvpOp:
            case local::LtvvOp:
            case local::LeppOp:
            case local::LepvOp:
            case local::LevpOp:
            case local::LevvOp:
            //
            // other operators that do not need graph operations
            case local::InvOp:
            case local::ParOp:
            break;

            // -------------------------------------------------------------
            // operators that are implemented
            case local::AddpvOp:
            case local::AddvvOp:
            is_json_op_used[local::json::add_json_op] = true;
            ++n_usage;
            break;

            case local::SubpvOp:
            case local::SubvpOp:
            case local::SubvvOp:
            is_json_op_used[local::json::sub_json_op] = true;
            ++n_usage;
            break;

            case local::CSumOp:
            is_json_op_used[local::json::sum_json_op] = true;
            if( (arg[1] != arg[2]) | (arg[3] != arg[4]) )
                n_usage += 3;
            else
                n_usage += 1;
            break;

            case local::MulpvOp:
            case local::MulvvOp:
            is_json_op_used[local::json::mul_json_op] = true;
            ++n_usage;
            break;

            // -------------------------------------------------------------
            // EndOp:
            case local::EndOp:
            more_operators = false;
            break;

            default:
            error_message += local::OpName(var_op);
            CPPAD_ASSERT_KNOWN( false, error_message.c_str() );
            break;
        }
    }
    // --------------------------------------------------------------------
    // par2node
    pod_vector<size_t> par2node(n_parameter);
    par2node[0] = 0; // invalid value
    for(size_t i = 1; i <= n_dynamic_ind; ++i)
        par2node[i] = i; // independent dynamic parameters
    for(size_t i = n_dynamic_ind + 1; i < n_parameter; ++i)
        par2node[i] = 0; // will be set later
    // ----------------------------------------------------------------------
    // start with the opening '{' for this graph
    std::string result = "{\n";

    // ----------------------------------------------------------------------
    // op_define_vec
    size_t n_define = 0;
    pod_vector<size_t> graph_code(n_json_op);
    for(size_t i = 0; i < n_json_op; ++i)
    {   graph_code[i] = 0;
        if( is_json_op_used[i] )
            graph_code[i] = ++n_define;
    }
    result += "'op_define_vec' : [ " + to_string(n_define) + ", [\n";
    size_t count_define = 0;
    for(size_t i = 0; i < n_json_op; ++i)
    {   if( is_json_op_used[i] )
        {   ++count_define;
            const std::string name = local::json::op_enum2name[i];
            size_t op_code   = graph_code[i];
            result += "{ 'op_code':" + to_string(op_code);
            result += ", 'name':'" + name + "'";
            if( i != size_t( local::json::sum_json_op ) )
                result += ", 'n_arg':2";
            result += " }";
            if( count_define < n_define )
                result += ",\n";
        }
    }
    CPPAD_ASSERT_UNKNOWN( count_define == n_define );
    result += " ]\n] ,\n";
    //
    // initialize index of previous node in the graph
    size_t previous_node = 0;
    //
    // n_dynamic_ind
    result += "'n_dynamic_ind' : " + to_string( n_dynamic_ind ) + ",\n";
    previous_node += n_dynamic_ind;
    //
    // n_independent
    result += "'n_independent' : " + to_string( n_independent ) + ",\n";
    previous_node += n_independent;
    //
    // string_vec
    result += "'string_vec' : 0, [ ],\n";
    previous_node += n_string;
    // --------------------------------------------------------------------
    // constant_vec and par2node for constants
    local::pod_vector_maybe<Base> constant_vec;
    for(size_t i = 1; i < n_parameter; ++i)
    {   if( ! dyn_par_is[i] )
        {   // this is a constant node
            constant_vec.push_back( parameter[i] );
            par2node[i] = ++previous_node;
        }
    }
    CPPAD_ASSERT_UNKNOWN( n_constant == constant_vec.size() );
    result += "'constant_vec' : [ " + to_string(n_constant) + ", [\n";
    for(size_t i = 0; i < n_constant; ++i)
    {   result += to_string( constant_vec[i] );
        if( i + 1 < n_constant )
            result += ",\n";
    }
    result += " ] ],\n";
    // ----------------------------------------------------------------------
    // Json operators is dynamic operators plus variables operators.
    // Skip BeginOp, EndOp, and independent variables.
    result += "'op_usage_vec' : [ " + to_string(n_usage) + ", [\n";
    size_t count_usage = 0;
    // ----------------------------------------------------------------------
    // dynamic parameter operations and par2node
    // for dynamic parameters that are not constants or independent
    CPPAD_ASSERT_UNKNOWN( num_arg_dyn(local::ind_dyn) == 0 );
    size_t i_arg = 0;
    pod_vector<size_t> node_arg(2);
    for(size_t i_dyn = n_dynamic_ind; i_dyn < n_dynamic; ++i_dyn)
    {   // operator for this dynamic parameter
        local::op_code_dyn dyn_op = local::op_code_dyn( dyn_par_op[i_dyn] );
        //
        // parameter index for this dynamic parameter
        size_t i_par = size_t( dyn_ind2par_ind[i_dyn] );
        CPPAD_ASSERT_UNKNOWN( par2node[i_par] == 0 );
        par2node[i_par] = ++previous_node;
        //
        // number of arguments for operators with fixed number of arguments
        size_t n_arg = size_t( num_arg_dyn(dyn_op) );
        CPPAD_ASSERT_UNKNOWN( n_arg <= 2 );
        //
        // arguments in graph node space
        for(size_t i = 0; i < n_arg; ++i)
        {   node_arg[i] = par2node[ dyn_par_arg[i_arg + i] ];
            CPPAD_ASSERT_UNKNOWN( node_arg[i] > 0 );
        }
        //
        size_t op_code = local::json::n_json_op; // invalid value
        switch(dyn_op)
        {
            case local::add_dyn:
            op_code = graph_code[ local::json::add_json_op ];
            break;

            case local::mul_dyn:
            op_code = graph_code[ local::json::mul_json_op ];
            break;

            case local::sub_dyn:
            op_code = graph_code[ local::json::sub_json_op ];
            break;

            default:
            // This error should have been reported above
            CPPAD_ASSERT_UNKNOWN( false );
            break;
        }
        CPPAD_ASSERT_UNKNOWN( op_code != 0 );
        if( n_arg != 2 )
        {   CPPAD_ASSERT_UNKNOWN( false );
        }
        else
        {   result += "[ " + to_string(op_code) + ", ";
            result += to_string(node_arg[0]) + ", ";
            result += to_string(node_arg[1]) + " ]";
        }
        i_arg  += n_arg;
        ++count_usage;
        if( count_usage < n_usage )
            result += ",\n";
    }
    CPPAD_ASSERT_UNKNOWN( count_usage == n_dynamic_op );
    // ----------------------------------------------------------------------
    // variable operators
    pod_vector<size_t> var2node(n_variable);
    var2node[0] = 0; // invalide node value
    for(size_t i = 1; i <= n_independent; ++i)
        var2node[i] = n_dynamic_ind + i;
    for(size_t i = n_independent + 1; i < n_variable; ++i)
        var2node[i] = 0; // invalid node value
    //
    itr            = play_.begin();
    more_operators = true;
    pod_vector<bool> is_var(2);
    while(more_operators)
    {   // if non-zero, this is a fixed size operator with this many arguments
        // and implemented after the switch. In additionk, is_var is set for
        // each of the at most 2 arguments.
        size_t fixed_n_arg = 0;

        // op_code for this opertor;
        size_t op_code = 0;

        // next op
        (++itr).op_info(var_op, arg, i_var);


        // -------------------------------------------------------------------
        // Cases with fixed number of arguments, one or two arguments, and
        // operator is not ignored.
        // -------------------------------------------------------------------
        switch( var_op )
        {
            // --------------------------------------------------------------
            // first argument a parameter, second argument a variable
            case local::AddpvOp:
            case local::MulpvOp:
            case local::SubpvOp:
            fixed_n_arg = 2;
            is_var[0]   = false;
            is_var[1]   = true;
            break;

            // --------------------------------------------------------------
            // first argument a variable, second argument a parameter
            case local::SubvpOp:
            fixed_n_arg = 2;
            is_var[0]   = true;
            is_var[1]   = true;
            break;

            // --------------------------------------------------------------
            // first argument a variable, second argument a variable
            case local::AddvvOp:
            case local::MulvvOp:
            case local::SubvvOp:
            fixed_n_arg = 2;
            is_var[0]   = true;
            is_var[1]   = true;
            break;

            // --------------------------------------------------------------
            default:
            break;
        }
        if( fixed_n_arg > 0 )
        {   // Set op_code
            switch( var_op )
            {
                // -----------------------------------------------------------
                case local::AddpvOp:
                case local::AddvvOp:
                op_code     = graph_code[ local::json::add_json_op ];
                break;

                // -----------------------------------------------------------
                case local::MulpvOp:
                case local::MulvvOp:
                op_code = graph_code[ local::json::mul_json_op ];
                break;

                // -----------------------------------------------------------
                case local::SubpvOp:
                case local::SubvpOp:
                case local::SubvvOp:
                op_code = graph_code[ local::json::sub_json_op ];
                break;

                // -----------------------------------------------------------
                default:
                // This should be one of the cases above
                CPPAD_ASSERT_UNKNOWN(false);
                break;
            }
            CPPAD_ASSERT_UNKNOWN( op_code != 0 );
            //
            var2node[i_var] = ++previous_node;
            result += "[ " + to_string(op_code) + ", ";
            //
            // first argument
            if( is_var[0] )
                result += to_string( var2node[ arg[0] ] );
            else
                result += to_string( par2node[ arg[0] ] );
            if( fixed_n_arg == 1 )
                result += " ]";
            else
            {   // second argument
                CPPAD_ASSERT_UNKNOWN( fixed_n_arg == 2 );
                if( is_var[1] )
                    result += ", " + to_string( var2node[ arg[1] ] ) + " ]";
                else
                    result += ", " + to_string( par2node[ arg[1] ] ) + " ]";
            }
            ++count_usage;
            if( count_usage < n_usage )
                result += ",\n";
        }
        // -------------------------------------------------------------------
        // Other cases
        // -------------------------------------------------------------------
        else switch( var_op )
        {
            // -------------------------------------------------------------
            // Ignore all comparison operators (for now)
            case local::EqppOp:
            case local::EqpvOp:
            case local::EqvvOp:
            case local::NeppOp:
            case local::NepvOp:
            case local::NevvOp:
            //
            case local::LtppOp:
            case local::LtpvOp:
            case local::LtvpOp:
            case local::LtvvOp:
            case local::LeppOp:
            case local::LepvOp:
            case local::LevpOp:
            case local::LevvOp:
            //
            break;

            // --------------------------------------------------------------
            // CSumOp
            case local::CSumOp:
            {   // does this case have subtraction terms
                bool has_subtract = (arg[1] != arg[2]) | (arg[3] != arg[4]);
                if( has_subtract )
                {   // two cumulative sum and one subtract operators
                    var2node[i_var] = previous_node + 3;
                }
                else
                {   // one cumulative sum operator
                    var2node[i_var] = previous_node + 1;
                }
                // previous_node + 1 = sum corresponding to addition terms
                op_code = graph_code[ local::json::sum_json_op ];
                CPPAD_ASSERT_UNKNOWN( op_code != 0 );
                CPPAD_ASSERT_UNKNOWN( 5 <= arg[1] );
                CPPAD_ASSERT_UNKNOWN( arg[2] <= arg[3] );
                size_t n_arg = size_t(1 + arg[1] - 5 + arg[3] - arg[2]);
                result += "[ " + to_string(op_code) + ", 1, ";
                result += to_string(n_arg) + ", [ ";
                size_t arg_node  = par2node[ arg[0] ];
                result += to_string(arg_node) + ", ";
                size_t j_arg = 1;
                for(addr_t i = 5; i < arg[1]; ++i)
                {   arg_node    = var2node[ arg[i] ];
                    CPPAD_ASSERT_UNKNOWN( arg_node > 0 );
                    result += to_string(arg_node);
                    ++j_arg;
                    if( j_arg < n_arg )
                        result += ", ";
                }
                for(addr_t i = arg[2]; i < arg[3]; ++i)
                {   arg_node  = par2node[ arg[i] ];
                    result   += to_string(arg_node);
                    ++j_arg;
                    if( j_arg < n_arg )
                        result += ", ";
                }
                CPPAD_ASSERT_UNKNOWN( j_arg == n_arg );
                result += "] ]";
                if( has_subtract )
                {   // previous_node + 2 = sum corresponding to subtract terms
                    CPPAD_ASSERT_UNKNOWN( arg[1] <= arg[2] );
                    CPPAD_ASSERT_UNKNOWN( arg[3] <= arg[4] );
                    n_arg = size_t(arg[2] - arg[1] + arg[4] - arg[3]);
                    CPPAD_ASSERT_UNKNOWN( n_arg > 0 );
                    result += ",\n[ " + to_string(op_code) + ", 1, ";
                    result += to_string(n_arg) + ", [ ";
                    j_arg = 0;
                    for(addr_t i = arg[1]; i < arg[2]; ++i)
                    {   arg_node    = var2node[ arg[i] ];
                        CPPAD_ASSERT_UNKNOWN( arg_node > 0 );
                        result += to_string(arg_node);
                        ++j_arg;
                        if( j_arg < n_arg )
                            result += ", ";
                    }
                    for(addr_t i = arg[3]; i < arg[4]; ++i)
                    {   arg_node  = par2node[ arg[i] ];
                        result   += to_string(arg_node);
                        ++j_arg;
                        if( j_arg < n_arg )
                            result += ", ";
                    }
                    CPPAD_ASSERT_UNKNOWN( j_arg == n_arg );
                    result += "] ]";
                    //
                    // previous_node + 3 = first sum minus second sum
                    op_code = graph_code[ local::json::sub_json_op ];
                    result += ",\n[ " + to_string(op_code) + ", ";
                    result += to_string(previous_node + 1) + ", ";
                    result += to_string(previous_node + 2) + " ]";
                }
                if( has_subtract )
                {   count_usage   += 3;
                    previous_node += 3;
                }
                else
                {   count_usage   += 1;
                    previous_node += 1;
                }
            }
            itr.correct_before_increment();
            if( count_usage < n_usage )
                result += ",\n";
            break;

            // --------------------------------------------------------------
            // EndOp:
            case local::EndOp:
            more_operators = false;
            break;

            // --------------------------------------------------------------
            // InvOp: independent variables
            case local::InvOp:
            // no graph operators for independent variables
            break;

            // --------------------------------------------------------------
            // ParOp:
            case local::ParOp:
            // no need for a graph operator, just map variable to parameter
            var2node[i_var] = par2node[arg[0]];
            break;

            // --------------------------------------------------------------
            default:
            // This error should have been reported above
            CPPAD_ASSERT_UNKNOWN(false);
            break;
        }
    }
    CPPAD_ASSERT_UNKNOWN( count_usage == n_usage );
    result += " ]\n] ,\n";
    // ----------------------------------------------------------------------
    // dependent_vec
    size_t n_dependent = dep_taddr_.size();
    result += "'dependent_vec' : [ " + to_string(n_dependent) + ", [ ";
    for(size_t i = 0; i < n_dependent; ++i)
    {   result += to_string( var2node[ dep_taddr_[i] ] );
        if( i + 1 < n_dependent )
            result += ", ";
    }
    result += " ] ]\n";
    result += "}\n";
    // ----------------------------------------------------------------------
    // Convert the single quote to double quote
    for(size_t i = 0; i < result.size(); ++i)
        if( result[i] == '\'' ) result[i] = '"';
    //
    return result;
}

# endif
