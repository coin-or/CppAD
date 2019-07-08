# ifndef CPPAD_CORE_JSON_FROM_JSON_HPP
# define CPPAD_CORE_JSON_FROM_JSON_HPP
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
# include <cppad/core/ad_type.hpp>
# include <cppad/local/json/parser.hpp>

/*
$begin from_json$$
$spell
    Json
$$

$section ADFun Object Corresponding to a Json AD Graph$$

$head Under Construction$$
This routine is under construction and subject to
change without backward compatibility.

$head Syntax$$
$codei%
    ADFun<%Base%> %fun%
    %fun%.from_json(%graph%)
%$$

$head Prototype$$
$srcfile%include/cppad/core/json/from_json.hpp%
    0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1
%$$

$head graph$$
is a $cref json_ad_graph$$.

$head Base$$
is the type corresponding to this $cref/ADFun/adfun/$$ object;
i.e., its calculations are done using the type $icode Base$$.

$head RecBase$$
in the prototype above, $icode RecBase$$ is the same type as $icode Base$$.

$children%
    example/json/from_json.cpp
%$$
$head Example$$
The file $cref from_json.cpp$$ is an example and test of this operation.

$end
*/
// BEGIN_PROTOTYPE
template <class Base, class RecBase>
void CppAD::ADFun<Base,RecBase>::from_json(const std::string& graph)
// END_PROTOTYPE
{
    // json parser return values
    size_t                                   n_dynamic_ind;
    size_t                                   n_independent;
    vector<std::string>                      string_vec;
    vector<double>                           constant_vec;
    vector<local::json::json_op_struct>      operator_vec;
    vector<size_t>                           operator_arg;
    vector<size_t>                           dependent_vec;
    //
    // call json parser
    local::json::parser(
        graph,
        n_dynamic_ind,
        n_independent,
        string_vec,
        constant_vec,
        operator_vec,
        operator_arg,
        dependent_vec
    );
    size_t n_string    = string_vec.size();
    size_t n_constant  = constant_vec.size();
    size_t n_usage     = operator_vec.size();
    size_t n_dependent = dependent_vec.size();
    //
    // Start of node indices
    size_t start_dynamic_ind = 1;
    size_t start_independent = start_dynamic_ind + n_dynamic_ind;
    size_t start_string      = start_independent + n_independent;
    size_t start_constant    = start_string      + n_string;
    size_t start_operator    = start_constant    + n_constant;
    //
    // number of nodes in the graph
    size_t n_node = start_operator;
    for(size_t i = 0; i < n_usage; ++i)
        n_node += operator_vec[i].n_result;
    //
    // initialize mapping from node index in graph to index in function
    vector<ad_type_enum>        node_type( n_node );
    local::pod_vector<addr_t>   node2fun( n_node );
    for(size_t i = 0; i < n_node; ++i)
    {   node_type[i] = number_ad_type_enum;
        node2fun[i]  = std::numeric_limits<addr_t>::max();
    }
    // ----------------------------------------------------------------------
    // Create a recording for this function
    // ----------------------------------------------------------------------

    // start a recording
    local::recorder<Base> rec;
    CPPAD_ASSERT_UNKNOWN( rec.num_op_rec() == 0 );
    rec.set_num_dynamic_ind(n_dynamic_ind);
    rec.set_abort_op_index(0);
    rec.set_record_compare(false);

    // nan
    Base nan = CppAD::numeric_limits<Base>::quiet_NaN();

    // Place the parameter with index 0 in the tape
    local::pod_vector_maybe<Base> parameter;
    addr_t i_par = rec.put_con_par(nan);
    CPPAD_ASSERT_UNKNOWN( size_t(i_par) == parameter.size() );
    parameter.push_back(nan);
    //
    // Place the variable with index 0 in the tape
    CPPAD_ASSERT_NARG_NRES(local::BeginOp, 1, 1);
    rec.PutOp(local::BeginOp);
    rec.PutArg(0);
    //
    // Next come the independent dynamic parameters in the recording
    for(size_t i = 0; i < n_dynamic_ind; ++i)
    {   i_par = rec.put_dyn_par(nan, local::ind_dyn );
        CPPAD_ASSERT_UNKNOWN( size_t(i_par) == parameter.size() )
        parameter.push_back(nan);
        //
        node_type[start_dynamic_ind + i ] = dynamic_enum;
        node2fun[ start_dynamic_ind + i ] = i_par;
        CPPAD_ASSERT_UNKNOWN( i + 1 == size_t(i_par) );
    }

    // Next come the independent variables
    CPPAD_ASSERT_NARG_NRES(local::InvOp, 0, 1);
    for(size_t i = 0; i < n_independent; ++i)
    {   addr_t i_var = rec.PutOp( local::InvOp );
        node_type[start_independent + i ] = variable_enum;;
        node2fun[ start_independent + i ]  = i_var;
        CPPAD_ASSERT_UNKNOWN( i + 1 == size_t(i_var) );
    }

    // Next come the string values
    for(size_t i = 0; i < n_string; ++i)
    {   addr_t i_txt = rec.PutTxt( string_vec[i].c_str() );
        node_type[start_string + i ] = string_enum;;
        node2fun[ start_string + i ] = i_txt;
    }

    // Next come the constant parameters
    for(size_t i = 0; i < n_constant; ++i)
    {   i_par = rec.put_con_par( Base( constant_vec[i] ) );
        if( size_t(i_par) == parameter.size() )
            parameter.push_back( Base(constant_vec[i] ) );
# ifndef NDEBUG
        else CPPAD_ASSERT_UNKNOWN(
            parameter[i_par] == Base(constant_vec[i])
        );
# endif
        node_type[start_constant + i ] = constant_enum;;
        node2fun[ start_constant + i ] = i_par;
    }

    // loop over operators in the recording
    size_t start_result = start_operator;
    local::pod_vector<addr_t> arg;
    local::pod_vector<ad_type_enum> type;
    local::pod_vector<addr_t> temporary;
    for(size_t i = 0; i < n_usage; ++i)
    {   local::json::json_op_struct json_op = operator_vec[i];
        size_t n_arg                      = json_op.n_arg;
        size_t n_result                   = json_op.n_result;
        size_t start_arg                  = json_op.start_arg;
        local::json::json_op_enum op_enum = json_op.op_enum;
        if( n_arg > arg.size() )
        {   arg.extend( n_arg - arg.size() );
            type.extend( n_arg - type.size() );
        }
        addr_t nc_arg  = 0; // number of constant arguments
        addr_t nd_arg  = 0; // number of dynamic arguments
        addr_t nv_arg  = 0; // number of vaiable arguments
        for(size_t j = 0; j < n_arg; ++j)
        {   CPPAD_ASSERT_KNOWN(
                operator_arg[start_arg + j] < start_result,
            "from_json graph op argument index >= its starting result index"
            );
            arg[j]  = addr_t( operator_arg[start_arg + j] );
            type[j] = node_type[ arg[j] ];
            arg[j]  = node2fun[ arg[j] ];
            CPPAD_ASSERT_KNOWN(
                type[j] != string_enum,
                "from_json AD graph op argument is a string node index\n"
                "and so far no string operators have been implemented"
            );
            //
            nc_arg += addr_t( type[j] == constant_enum );
            nd_arg += addr_t( type[j] == dynamic_enum  );
            nv_arg += addr_t( type[j] == variable_enum );
        }
        CPPAD_ASSERT_UNKNOWN( n_arg == size_t(nc_arg + nd_arg + nv_arg) );
        //
        // initailize to avoid compiler warning
        addr_t i_result = 0; // invalid value
        if( n_arg == 2 && type[0] == variable_enum ) switch( op_enum )
        {
            // --------------------------------------------------------------
            case local::json::add_json_op:
            if( type[1] == variable_enum )
            {   i_result = rec.PutOp(local::AddvvOp);
                rec.PutArg( arg[0], arg[1] );
                CPPAD_ASSERT_NARG_NRES(local::AddvvOp, 2, 1);
            }
            else
            {   i_result = rec.PutOp(local::AddpvOp);
                rec.PutArg( arg[1], arg[0] );
                CPPAD_ASSERT_NARG_NRES(local::AddpvOp, 2, 1);
            }
            break;

            // --------------------------------------------------------------
            case local::json::mul_json_op:
            if( type[1] == variable_enum )
            {   i_result = rec.PutOp(local::MulvvOp);
                rec.PutArg( arg[0], arg[1] );
                CPPAD_ASSERT_NARG_NRES(local::MulvvOp, 2, 1);
            }
            else
            {   i_result = rec.PutOp(local::MulpvOp);
                rec.PutArg( arg[1], arg[0] );
                CPPAD_ASSERT_NARG_NRES(local::MulpvOp, 2, 1);
            }
            break;

            // --------------------------------------------------------------
            default:
            CPPAD_ASSERT_UNKNOWN( false );
            break;
        }
        else switch( op_enum )
        {
            // --------------------------------------------------------------
            case local::json::add_json_op:
            CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
            if( type[1] == variable_enum )
            {   i_result = rec.PutOp(local::AddpvOp);
                rec.PutArg( arg[0], arg[1] );
                CPPAD_ASSERT_NARG_NRES(local::AddpvOp, 2, 1);
            }
            else if( type[0] == dynamic_enum || type[1] == dynamic_enum )
            {   i_result = rec.put_dyn_par(nan, local::add_dyn, arg[0], arg[1]);
                CPPAD_ASSERT_UNKNOWN( size_t(i_result) == parameter.size() );
                parameter.push_back( nan );
            }
            else
            {   Base result = parameter[ arg[0] ] + parameter[ arg[1] ];
                i_result = rec.put_con_par(result);
                if( size_t(i_result) == parameter.size() )
                    parameter.push_back(result);
# ifndef NDEBUG
                else CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
# endif
            }
            break;

            // --------------------------------------------------------------
            case local::json::mul_json_op:
            CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
            if( type[1] == variable_enum )
            {   i_result = rec.PutOp(local::MulpvOp);
                rec.PutArg( arg[0], arg[1] );
                CPPAD_ASSERT_NARG_NRES(local::MulpvOp, 2, 1);
            }
            else if( type[0] == dynamic_enum || type[1] == dynamic_enum )
            {   i_result = rec.put_dyn_par(
                    nan, local::mul_dyn, arg[0], arg[1]
                );
                CPPAD_ASSERT_UNKNOWN( size_t(i_result) == parameter.size() );
                parameter.push_back( nan );
            }
            else
            {   Base result = parameter[ arg[0] ] * parameter[ arg[1] ];
                i_result = rec.put_con_par(result);
                if( size_t(i_result) == parameter.size() )
                    parameter.push_back(result);
# ifndef NDEBUG
                else CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
# endif
            }
            break;

            // --------------------------------------------------------------
            case local::json::sum_json_op:
            {   size_t n_temporary = 6 + size_t(nv_arg + nd_arg);
                if( temporary.size() < n_temporary )
                    temporary.extend( n_temporary - temporary.size() );
                Base sum_constant = 0.0;
                addr_t j_variable = 5 ;
                addr_t j_dynamic  = 5 + nv_arg;
                for(size_t j = 0; j < n_arg; j++)
                {   if( type[j] == constant_enum )
                        sum_constant += parameter[ arg[j] ];
                    if( type[j] == variable_enum )
                        temporary[ j_variable++ ] = arg[j];
                    if( type[j] == dynamic_enum )
                        temporary[ j_dynamic++ ]  = arg[j];
                }
                temporary[j_dynamic] = j_dynamic;
                temporary[0] = rec.put_con_par(sum_constant);
                if( size_t(temporary[0]) == parameter.size() )
                    parameter.push_back(sum_constant);
# ifndef NDEBUG
                else CPPAD_ASSERT_UNKNOWN(
                    parameter[temporary[0]] == sum_constant
                );
# endif
                temporary[1] = 5 + nv_arg;
                temporary[2] = 5 + nv_arg;
                temporary[3] = temporary[2] + nd_arg;
                temporary[4] = temporary[2] + nd_arg;
                //
                i_result = rec.PutOp(local::CSumOp);
                for(size_t j = 0; j < n_temporary; ++j)
                    rec.PutArg( temporary[j] );
                CPPAD_ASSERT_UNKNOWN( local::NumRes(local::CSumOp) == 1 );
            }
            break;

            // --------------------------------------------------------------
            default:
            CPPAD_ASSERT_UNKNOWN( false );
            break;
        }
        CPPAD_ASSERT_UNKNOWN( n_result == 1 );
        CPPAD_ASSERT_UNKNOWN( i_result != 0 );
        if( nv_arg > 0 )
            node_type[start_result] = variable_enum;
        else if( nd_arg > 0 )
            node_type[start_result] = dynamic_enum;
        else
            node_type[start_result] = constant_enum;
        //
        node2fun[start_result] = i_result;
        start_result          += n_result;
    }
    // set this->dep_parameter_, set this->dep_taddr_
    //
    CPPAD_ASSERT_NARG_NRES(local::ParOp, 1, 1);
    dep_parameter_.resize( n_dependent );
    dep_taddr_.resize( n_dependent );
    for(size_t i = 0; i < n_dependent; ++i)
    {   CPPAD_ASSERT_KNOWN(
            node_type[ dependent_vec[i] ] != string_enum,
            "Json AD graph dependent variable node is a string"
        );
        CPPAD_ASSERT_UNKNOWN(
            node_type[ dependent_vec[i] ] != number_ad_type_enum
        );
        if( node_type[ dependent_vec[i] ] == variable_enum )
        {   dep_parameter_[i] = false;
            dep_taddr_[i]     = size_t( node2fun[ dependent_vec[i] ] );
        }
        else
        {   dep_parameter_[i] = true;
            dep_taddr_[i]     = size_t( rec.PutOp(local::ParOp) );
            rec.PutArg( node2fun[ dependent_vec[i] ] );
        }
    }
    rec.PutOp(local::EndOp);
    // ----------------------------------------------------------------------
    // End recording, set private member data except for
    // dep__parameter_ and dep_taddr_
    // ----------------------------------------------------------------------
    //
    // bool values in this object except check_for_nan_
    base2ad_return_value_      = false;
    has_been_optimized_        = false;
    //
    // size_t values in this object
    compare_change_count_      = 1;
    compare_change_number_     = 0;
    compare_change_op_index_   = 0;
    num_order_taylor_          = 0;
    cap_order_taylor_          = 0;
    num_direction_taylor_      = 0;
    num_var_tape_              = rec.num_var_rec();
    //
    // taylor_
    taylor_.resize(0);
    //
    // cskip_op_
    cskip_op_.resize( rec.num_op_rec() );
    //
    // load_op_
    load_op_.resize( rec.num_load_op_rec() );
    //
    // play_
    // Now that each dependent variable has a place in the recording,
    // and there is a EndOp at the end of the record, we can transfer the
    // recording to the player and and erase the recording.
    play_.get_recording(rec, n_independent);
    //
    // ind_taddr_
    // Note that play_ has been set, we can use it to check operators
    ind_taddr_.resize(n_independent);
    CPPAD_ASSERT_UNKNOWN( n_independent < num_var_tape_);
    for(size_t j = 0; j < n_independent; j++)
    {   CPPAD_ASSERT_UNKNOWN( play_.GetOp(j+1) == local::InvOp );
        ind_taddr_[j] = j+1;
    }
    //
    // for_jac_sparse_pack_, for_jac_sparse_set_
    for_jac_sparse_pack_.resize(0, 0);
    for_jac_sparse_set_.resize(0,0);
    //
    // resize subgraph_info_
    subgraph_info_.resize(
        ind_taddr_.size(),   // n_dep
        dep_taddr_.size(),   // n_ind
        play_.num_op_rec(),  // n_op
        play_.num_var_rec()  // n_var
    );
    //
    return;
}

# endif
