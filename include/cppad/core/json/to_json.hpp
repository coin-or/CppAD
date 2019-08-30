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
# include <cppad/local/json/operator.hpp>

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
    if( local::json::op_name2enum.size() == 0 )
    {   CPPAD_ASSERT_KNOWN( ! thread_alloc::in_parallel() ,
            "call to set_operator_info in parallel mode"
        );
        local::json::set_operator_info();
    }
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
            // ---------------------------------------------------------------
            // unary operators

            case local::abs_dyn:
            is_json_op_used[local::json::abs_json_op] = true;
            break;

            case local::acosh_dyn:
            is_json_op_used[local::json::acosh_json_op] = true;
            break;

            case local::asinh_dyn:
            is_json_op_used[local::json::asinh_json_op] = true;
            break;

            case local::atanh_dyn:
            is_json_op_used[local::json::atanh_json_op] = true;
            break;

            case local::erf_dyn:
            is_json_op_used[local::json::erf_json_op] = true;
            break;

            case local::erfc_dyn:
            is_json_op_used[local::json::erfc_json_op] = true;
            break;

            case local::expm1_dyn:
            is_json_op_used[local::json::expm1_json_op] = true;
            break;

            case local::log1p_dyn:
            is_json_op_used[local::json::log1p_json_op] = true;
            break;

            case local::acos_dyn:
            is_json_op_used[local::json::acos_json_op] = true;
            break;

            case local::asin_dyn:
            is_json_op_used[local::json::asin_json_op] = true;
            break;

            case local::atan_dyn:
            is_json_op_used[local::json::atan_json_op] = true;
            break;

            case local::cosh_dyn:
            is_json_op_used[local::json::cosh_json_op] = true;
            break;

            case local::cos_dyn:
            is_json_op_used[local::json::cos_json_op] = true;
            break;

            case local::exp_dyn:
            is_json_op_used[local::json::exp_json_op] = true;
            break;

            case local::log_dyn:
            is_json_op_used[local::json::log_json_op] = true;
            break;

            case local::sign_dyn:
            is_json_op_used[local::json::sign_json_op] = true;
            break;

            case local::sinh_dyn:
            is_json_op_used[local::json::sinh_json_op] = true;
            break;

            case local::sin_dyn:
            is_json_op_used[local::json::sin_json_op] = true;
            break;

            case local::sqrt_dyn:
            is_json_op_used[local::json::sqrt_json_op] = true;
            break;

            case local::tanh_dyn:
            is_json_op_used[local::json::tanh_json_op] = true;
            break;

            case local::tan_dyn:
            is_json_op_used[local::json::tan_json_op] = true;
            break;

            // ---------------------------------------------------------------
            // binary operators

            case local::add_dyn:
            is_json_op_used[local::json::add_json_op] = true;
            break;

            case local::mul_dyn:
            is_json_op_used[local::json::mul_json_op] = true;
            break;

            case local::sub_dyn:
            is_json_op_used[local::json::sub_json_op] = true;
            break;

            case local::div_dyn:
            is_json_op_used[local::json::div_json_op] = true;
            break;

            // ---------------------------------------------------------------
            case local::call_dyn:
            is_json_op_used[local::json::atom_json_op] = true;
            break;

            // ---------------------------------------------------------------
            default:
            error_message += op_name_dyn(dyn_op);
            CPPAD_ASSERT_KNOWN( false, error_message.c_str() );
            break;
        }
    }
    size_t n_usage = n_dynamic_op;
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
    bool in_atomic_call  = false;
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

            // ---------------------------------------------------------------
            // unary operators

            case local::AbsOp:
            is_json_op_used[local::json::abs_json_op] = true;
            ++n_usage;
            break;

            case local::AcoshOp:
            is_json_op_used[local::json::acosh_json_op] = true;
            ++n_usage;
            break;

            case local::AsinhOp:
            is_json_op_used[local::json::asinh_json_op] = true;
            ++n_usage;
            break;

            case local::AtanhOp:
            is_json_op_used[local::json::atanh_json_op] = true;
            ++n_usage;
            break;

            case local::ErfOp:
            is_json_op_used[local::json::erf_json_op] = true;
            ++n_usage;
            break;

            case local::ErfcOp:
            is_json_op_used[local::json::erfc_json_op] = true;
            ++n_usage;
            break;

            case local::Expm1Op:
            is_json_op_used[local::json::expm1_json_op] = true;
            ++n_usage;
            break;

            case local::Log1pOp:
            is_json_op_used[local::json::log1p_json_op] = true;
            ++n_usage;
            break;

            case local::AcosOp:
            is_json_op_used[local::json::acos_json_op] = true;
            ++n_usage;
            break;

            case local::AsinOp:
            is_json_op_used[local::json::asin_json_op] = true;
            ++n_usage;
            break;

            case local::AtanOp:
            is_json_op_used[local::json::atan_json_op] = true;
            ++n_usage;
            break;

            case local::CoshOp:
            is_json_op_used[local::json::cosh_json_op] = true;
            ++n_usage;
            break;

            case local::CosOp:
            is_json_op_used[local::json::cos_json_op] = true;
            ++n_usage;
            break;

            case local::ExpOp:
            is_json_op_used[local::json::exp_json_op] = true;
            ++n_usage;
            break;

            case local::LogOp:
            is_json_op_used[local::json::log_json_op] = true;
            ++n_usage;
            break;

            case local::SignOp:
            is_json_op_used[local::json::sign_json_op] = true;
            ++n_usage;
            break;

            case local::SinhOp:
            is_json_op_used[local::json::sinh_json_op] = true;
            ++n_usage;
            break;

            case local::SinOp:
            is_json_op_used[local::json::sin_json_op] = true;
            ++n_usage;
            break;

            case local::SqrtOp:
            is_json_op_used[local::json::sqrt_json_op] = true;
            ++n_usage;
            break;

            case local::TanhOp:
            is_json_op_used[local::json::tanh_json_op] = true;
            ++n_usage;
            break;

            case local::TanOp:
            is_json_op_used[local::json::tan_json_op] = true;
            ++n_usage;
            break;

            // ---------------------------------------------------------------
            // binary operators

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

            case local::DivpvOp:
            case local::DivvpOp:
            case local::DivvvOp:
            is_json_op_used[local::json::div_json_op] = true;
            ++n_usage;
            break;

            case local::MulpvOp:
            case local::MulvvOp:
            is_json_op_used[local::json::mul_json_op] = true;
            ++n_usage;
            break;

            // -------------------------------------------------------------
            case local::CSumOp:
            is_json_op_used[local::json::sum_json_op] = true;
            if( (arg[1] != arg[2]) | (arg[3] != arg[4]) )
                n_usage += 3;
            else
                n_usage += 1;
            break;
            // -------------------------------------------------------------
            // atomic funciton operators

            case local::AFunOp:
            in_atomic_call = ! in_atomic_call;
            if( in_atomic_call )
            {   is_json_op_used[local::json::atom_json_op] = true;
                n_usage += 1;
            }
            break;

            case local::FunapOp:
            case local::FunavOp:
            case local::FunrpOp:
            case local::FunrvOp:
            CPPAD_ASSERT_UNKNOWN( in_atomic_call );
            break;

            // -------------------------------------------------------------
            // EndOp:
            case local::EndOp:
            more_operators = false;
            break;

            // -------------------------------------------------------------
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
    // function_name
    result += "'function_name' : '" + function_name_ + "',\n";

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
            size_t n_arg     = local::json::op_enum2fixed_n_arg[i];
            result += "{ 'op_code':" + to_string(op_code);
            result += ", 'name':'" + name + "'";
            if( n_arg != 0 )
                result += ", 'n_arg':" + to_string(n_arg);
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
    result += "'constant_vec' : " + to_string(n_constant) + ", [\n";
    for(size_t i = 0; i < n_constant; ++i)
    {   result += to_string( constant_vec[i] );
        if( i + 1 < n_constant )
            result += ",\n";
    }
    result += " ],\n";
    // ----------------------------------------------------------------------
    // Json operators is dynamic operators plus variables operators.
    // Skip BeginOp, EndOp, and independent variables.
    result += "'op_usage_vec' : " + to_string(n_usage) + ", [\n";
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
            // ---------------------------------------------------------------
            // unary operators

            case local::abs_dyn:
            op_code = graph_code[ local::json::abs_json_op ];
            break;

            case local::acosh_dyn:
            op_code = graph_code[ local::json::acosh_json_op ];
            break;

            case local::asinh_dyn:
            op_code = graph_code[ local::json::asinh_json_op ];
            break;

            case local::atanh_dyn:
            op_code = graph_code[ local::json::atanh_json_op ];
            break;

            case local::erf_dyn:
            op_code = graph_code[ local::json::erf_json_op ];
            break;

            case local::erfc_dyn:
            op_code = graph_code[ local::json::erfc_json_op ];
            break;

            case local::expm1_dyn:
            op_code = graph_code[ local::json::expm1_json_op ];
            break;

            case local::log1p_dyn:
            op_code = graph_code[ local::json::log1p_json_op ];
            break;

            case local::acos_dyn:
            op_code = graph_code[ local::json::acos_json_op ];
            break;

            case local::asin_dyn:
            op_code = graph_code[ local::json::asin_json_op ];
            break;

            case local::atan_dyn:
            op_code = graph_code[ local::json::atan_json_op ];
            break;

            case local::cosh_dyn:
            op_code = graph_code[ local::json::cosh_json_op ];
            break;

            case local::cos_dyn:
            op_code = graph_code[ local::json::cos_json_op ];
            break;

            case local::exp_dyn:
            op_code = graph_code[ local::json::exp_json_op ];
            break;

            case local::log_dyn:
            op_code = graph_code[ local::json::log_json_op ];
            break;

            case local::sign_dyn:
            op_code = graph_code[ local::json::sign_json_op ];
            break;

            case local::sinh_dyn:
            op_code = graph_code[ local::json::sinh_json_op ];
            break;

            case local::sin_dyn:
            op_code = graph_code[ local::json::sin_json_op ];
            break;

            case local::sqrt_dyn:
            op_code = graph_code[ local::json::sqrt_json_op ];
            break;

            case local::tanh_dyn:
            op_code = graph_code[ local::json::tanh_json_op ];
            break;

            case local::tan_dyn:
            op_code = graph_code[ local::json::tan_json_op ];
            break;

            // ---------------------------------------------------------------
            // binary operators

            case local::add_dyn:
            op_code = graph_code[ local::json::add_json_op ];
            break;

            case local::mul_dyn:
            op_code = graph_code[ local::json::mul_json_op ];
            break;

            case local::sub_dyn:
            op_code = graph_code[ local::json::sub_json_op ];
            break;

            case local::div_dyn:
            op_code = graph_code[ local::json::div_json_op ];
            break;

            // ---------------------------------------------------------------
            case local::call_dyn:
            op_code = graph_code[ local::json::atom_json_op ];
            break;

            case local::result_dyn: // place holder for atomic function results
            break;

            // ---------------------------------------------------------------
            default:
            // This error should have been reported above
            CPPAD_ASSERT_UNKNOWN( false );
            break;
        }
        CPPAD_ASSERT_UNKNOWN( dyn_op == local::call_dyn || op_code != 0 );
        if( n_arg == 1 )
        {   result += "[ " + to_string(op_code) + ", ";
            result += to_string(node_arg[0]) + " ]";
        }
        else if( n_arg == 2 )
        {   result += "[ " + to_string(op_code) + ", ";
            result += to_string(node_arg[0]) + ", ";
            result += to_string(node_arg[1]) + " ]";
        }
        else if( dyn_op == local::result_dyn )
        {   CPPAD_ASSERT_UNKNOWN( op_code == 0 );
            CPPAD_ASSERT_UNKNOWN( n_arg == 0 );
        }
        else
        {   CPPAD_ASSERT_UNKNOWN( dyn_op == local::call_dyn );
            // arg[0]: atomic function index
            size_t atom_index  = size_t( dyn_par_arg[i_arg + 0] );
            // arg[1]: number of arguments to function
            size_t n_arg_fun   = size_t( dyn_par_arg[i_arg + 1] );
            // arg[2]: number of results from function
            size_t n_result    = size_t( dyn_par_arg[i_arg + 2] );
            //
            // get the name for this atomic function
            std::string     name;
            {   bool        set_null = false;
                size_t      type;
                void*       ptr;
                CppAD::local::atomic_index<double>(
                    set_null, atom_index, type, &name, ptr
                );
            }
            //
            // Convert to Json
            result += "[ " + to_string(op_code) + ", "; // [ op_code,
            result += "\"" + name + "\", ";             // name,
            result += to_string(n_result) + ",";        // n_result,
            result += to_string(n_arg_fun) + ", [";     // n_arg_fun, [
            for(size_t j = 0; j < n_arg_fun; ++j)
            {   // arg[4 + j]: j-th argument to function
                size_t node_j = par2node[ dyn_par_arg[i_arg + 4 + j] ];
                CPPAD_ASSERT_UNKNOWN( node_j < i_par );
                result += to_string(node_j); // node_j
                if( j + 1 < n_arg_fun )
                    result += ", ";          // ,
                else
                    result += " ]";          // ]
            }
            result += " ]";
            // number of arguments to operator
            n_arg = 5 + n_arg_fun + n_result;
            CPPAD_ASSERT_UNKNOWN(
                n_arg == size_t(dyn_par_arg[i_arg + 4 + n_arg_fun + n_result])
            );
        }
        i_arg  += n_arg;
        ++count_usage;
        if( count_usage < n_usage )
            result += ",\n";
    }
    CPPAD_ASSERT_UNKNOWN( count_usage == n_dynamic_op );
    CPPAD_ASSERT_UNKNOWN( in_atomic_call == false );
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
    pod_vector<size_t> atom_node_arg;
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
            // -------------------------------------------------------------
            // unary operators
            case local::AbsOp:
            fixed_n_arg = 1;
            is_var[0] = true;
            break;

            case local::AcoshOp:
            fixed_n_arg = 1;
            is_var[0] = true;
            break;

            case local::AsinhOp:
            fixed_n_arg = 1;
            is_var[0] = true;
            break;

            case local::AtanhOp:
            fixed_n_arg = 1;
            is_var[0] = true;
            break;

            case local::ErfOp:
            fixed_n_arg = 1;
            is_var[0] = true;
            break;

            case local::ErfcOp:
            fixed_n_arg = 1;
            is_var[0] = true;
            break;

            case local::Expm1Op:
            fixed_n_arg = 1;
            is_var[0] = true;
            break;

            case local::Log1pOp:
            fixed_n_arg = 1;
            is_var[0] = true;
            break;

            case local::AcosOp:
            fixed_n_arg = 1;
            is_var[0] = true;
            break;

            case local::AsinOp:
            fixed_n_arg = 1;
            is_var[0] = true;
            break;

            case local::AtanOp:
            fixed_n_arg = 1;
            is_var[0] = true;
            break;

            case local::CoshOp:
            fixed_n_arg = 1;
            is_var[0] = true;
            break;

            case local::CosOp:
            fixed_n_arg = 1;
            is_var[0] = true;
            break;

            case local::ExpOp:
            fixed_n_arg = 1;
            is_var[0] = true;
            break;

            case local::LogOp:
            fixed_n_arg = 1;
            is_var[0] = true;
            break;

            case local::SignOp:
            fixed_n_arg = 1;
            is_var[0] = true;
            break;

            case local::SinhOp:
            fixed_n_arg = 1;
            is_var[0] = true;
            break;

            case local::SinOp:
            fixed_n_arg = 1;
            is_var[0] = true;
            break;

            case local::SqrtOp:
            fixed_n_arg = 1;
            is_var[0] = true;
            break;

            case local::TanhOp:
            fixed_n_arg = 1;
            is_var[0] = true;
            break;

            case local::TanOp:
            fixed_n_arg = 1;
            is_var[0] = true;
            break;

            // ---------------------------------------------------------------
            // binary operators
            // ---------------------------------------------------------------

            // first argument a parameter, second argument a variable
            case local::AddpvOp:
            case local::MulpvOp:
            case local::SubpvOp:
            case local::DivpvOp:
            fixed_n_arg = 2;
            is_var[0]   = false;
            is_var[1]   = true;
            break;

            // first argument a variable, second argument a parameter
            case local::SubvpOp:
            case local::DivvpOp:
            fixed_n_arg = 2;
            is_var[0]   = true;
            is_var[1]   = false;
            break;

            // first argument a variable, second argument a variable
            case local::AddvvOp:
            case local::MulvvOp:
            case local::SubvvOp:
            case local::DivvvOp:
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
                // ----------------------------------------------------------
                // unary operators

                case local::AbsOp:
                op_code     = graph_code[ local::json::abs_json_op ];
                break;

                case local::AcoshOp:
                op_code     = graph_code[ local::json::acosh_json_op ];
                break;

                case local::AsinhOp:
                op_code     = graph_code[ local::json::asinh_json_op ];
                break;

                case local::AtanhOp:
                op_code     = graph_code[ local::json::atanh_json_op ];
                break;

                case local::ErfOp:
                op_code     = graph_code[ local::json::erf_json_op ];
                break;

                case local::ErfcOp:
                op_code     = graph_code[ local::json::erfc_json_op ];
                break;

                case local::Expm1Op:
                op_code     = graph_code[ local::json::expm1_json_op ];
                break;

                case local::Log1pOp:
                op_code     = graph_code[ local::json::log1p_json_op ];
                break;

                case local::AcosOp:
                op_code     = graph_code[ local::json::acos_json_op ];
                break;

                case local::AsinOp:
                op_code     = graph_code[ local::json::asin_json_op ];
                break;

                case local::AtanOp:
                op_code     = graph_code[ local::json::atan_json_op ];
                break;

                case local::CoshOp:
                op_code     = graph_code[ local::json::cosh_json_op ];
                break;

                case local::CosOp:
                op_code     = graph_code[ local::json::cos_json_op ];
                break;

                case local::ExpOp:
                op_code     = graph_code[ local::json::exp_json_op ];
                break;

                case local::LogOp:
                op_code     = graph_code[ local::json::log_json_op ];
                break;

                case local::SignOp:
                op_code     = graph_code[ local::json::sign_json_op ];
                break;

                case local::SinhOp:
                op_code     = graph_code[ local::json::sinh_json_op ];
                break;

                case local::SinOp:
                op_code     = graph_code[ local::json::sin_json_op ];
                break;

                case local::SqrtOp:
                op_code     = graph_code[ local::json::sqrt_json_op ];
                break;

                case local::TanhOp:
                op_code     = graph_code[ local::json::tanh_json_op ];
                break;

                case local::TanOp:
                op_code     = graph_code[ local::json::tan_json_op ];
                break;

                // -----------------------------------------------------------
                // binary operators

                case local::AddpvOp:
                case local::AddvvOp:
                op_code = graph_code[ local::json::add_json_op ];
                break;

                case local::MulpvOp:
                case local::MulvvOp:
                op_code = graph_code[ local::json::mul_json_op ];
                break;

                case local::SubpvOp:
                case local::SubvpOp:
                case local::SubvvOp:
                op_code = graph_code[ local::json::sub_json_op ];
                break;

                case local::DivpvOp:
                case local::DivvpOp:
                case local::DivvvOp:
                op_code = graph_code[ local::json::div_json_op ];
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
            case local::FunapOp:
            atom_node_arg.push_back( par2node[arg[0]] );
            break;

            case local::FunavOp:
            CPPAD_ASSERT_UNKNOWN( var2node[arg[0]] <= i_var );
            atom_node_arg.push_back( var2node[arg[0]] );
            break;

            case local::FunrpOp:
            par2node[arg[0]] = ++previous_node;
            break;

            case local::FunrvOp:
            var2node[i_var] = ++previous_node;
            break;

            case local::AFunOp:
            in_atomic_call = ! in_atomic_call;
            if( in_atomic_call )
            {   atom_node_arg.resize(0);
            }
            else
            {   // This is the AFunOp at the end of the call
                op_code             = graph_code[ local::json::atom_json_op ];
                size_t atom_index   = size_t( arg[0] );
                size_t n_arg_fun    = size_t( arg[2] );
                size_t n_result     = size_t( arg[3] );
                CPPAD_ASSERT_UNKNOWN( atom_node_arg.size() == n_arg_fun );
                //
                // get the name for this atomic function
                std::string     name;
                {   bool        set_null = false;
                    size_t      type;
                    void*       ptr;
                    CppAD::local::atomic_index<double>(
                        set_null, atom_index, type, &name, ptr
                    );
                }
                //
                // Convert to Json
                result += "[ " + to_string(op_code) + ", "; // [ op_code,
                result += "\"" + name + "\", ";             // name,
                result += to_string(n_result) + ",";        // n_result,
                result += to_string(n_arg_fun) + ", [";     // n_arg_fun, [
                for(size_t j = 0; j < n_arg_fun; ++j)
                {   result += to_string(atom_node_arg[j]);  // next argument
                    if( j + 1 < n_arg_fun )
                        result += ", ";          // ,
                    else
                        result += " ]";          // ]
                }
                result += " ]";
                ++count_usage;
                if( count_usage < n_usage )
                    result += ",\n";
            }
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
    result += " ]\n,\n";
    // ----------------------------------------------------------------------
    // dependent_vec
    size_t n_dependent = dep_taddr_.size();
    result += "'dependent_vec' : " + to_string(n_dependent) + ", [ ";
    for(size_t i = 0; i < n_dependent; ++i)
    {   result += to_string( var2node[ dep_taddr_[i] ] );
        if( i + 1 < n_dependent )
            result += ", ";
    }
    result += " ]\n";
    result += "}\n";
    // ----------------------------------------------------------------------
    // Convert the single quote to double quote
    for(size_t i = 0; i < result.size(); ++i)
        if( result[i] == '\'' ) result[i] = '"';
    //
    return result;
}

# endif
