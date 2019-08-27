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
{   typedef local::json::json_op_struct json_op_struct;
    using CppAD::isnan;
    //
    // json parser return values
    std::string                function_name;
    size_t                     n_dynamic_ind;
    size_t                     n_independent;
    vector<std::string>        atomic_name_vec;
    vector<std::string>        string_vec;
    vector<double>             constant_vec;
    vector<json_op_struct>     operator_vec;
    vector<size_t>             operator_arg;
    vector<size_t>             dependent_vec;
    //
    // call json parser
    local::json::parser(
        graph,
        function_name,
        n_dynamic_ind,
        n_independent,
        atomic_name_vec,
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
    {   node_type[i] = number_ad_type_enum; // invalid value
        node2fun[i]  = 0;                   // invalid value
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
    const local::pod_vector_maybe<Base>& parameter( rec.all_par_vec() );
    addr_t i_par = rec.put_con_par(nan);
    CPPAD_ASSERT_UNKNOWN( isnan( parameter[0] ) );
    //
    // Place the variable with index 0 in the tape
    CPPAD_ASSERT_NARG_NRES(local::BeginOp, 1, 1);
    rec.PutOp(local::BeginOp);
    rec.PutArg(0);
    //
    // Next come the independent dynamic parameters in the recording
    for(size_t i = 0; i < n_dynamic_ind; ++i)
    {   i_par = rec.put_dyn_par(nan, local::ind_dyn );
        CPPAD_ASSERT_UNKNOWN( isnan( parameter[start_dynamic_ind + i] ) );
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
    {   Base par = Base( constant_vec[i] );
        i_par = rec.put_con_par(par);
        CPPAD_ASSERT_UNKNOWN( parameter[i_par] == par );
        //
        node_type[start_constant + i ] = constant_enum;;
        node2fun[ start_constant + i ] = i_par;
    }

    //
    // local arrays used to avoid reallocating memory
    local::pod_vector<addr_t>       arg;
    vector<ad_type_enum>            type_x;
    local::pod_vector<addr_t>       temporary;
    //
    // arrays only used by atom_json_op
    vector<Base>                    parameter_x, taylor_y;
    vector<ad_type_enum>            type_y;
    vector< AD<Base> >              ax, ay;
    //
    // loop over operators in the recording
    size_t start_result = start_operator;
    for(size_t i_json = 0; i_json < n_usage; ++i_json)
    {   // information for this operator usage
        const json_op_struct&     json_op = operator_vec[i_json];
        local::json::json_op_enum op_enum = json_op.op_enum;
        size_t n_arg        = json_op.n_arg;
        size_t n_result     = json_op.n_result;
        size_t start_arg    = json_op.start_arg;
        //
        if( n_arg > arg.size() )
        {   arg.resize( n_arg );
            type_x.resize(n_arg);
        }
# ifndef NDEBUG
        addr_t n_con_arg  = 0;
# endif
        addr_t n_dyn_arg  = 0;
        addr_t n_var_arg  = 0;
        for(size_t j = 0; j < n_arg; ++j)
        {   //
            // argument to graph operator
            arg[j]  = addr_t( operator_arg[start_arg + j] );
            CPPAD_ASSERT_KNOWN( size_t(arg[j]) < start_result,
                "from_json graph op argument index is less that or equal\n"
                "the starting index for its results"
            );
            CPPAD_ASSERT_UNKNOWN( node2fun[ arg[j] ] != 0 );
            //
            // type of argument
            type_x[j] = node_type[ arg[j] ];
            CPPAD_ASSERT_KNOWN(
                type_x[j] != string_enum,
                "from_json AD graph op argument is a string node index\n"
                "and so far no string operators have been implemented"
            );
            //
            // argument to function operator
            arg[j]  = node2fun[ arg[j] ];
            //
            // count number of arguments of different types
# ifndef NDEBUG
            n_con_arg += addr_t( type_x[j] == constant_enum );
# endif
            n_dyn_arg += addr_t( type_x[j] == dynamic_enum  );
            n_var_arg += addr_t( type_x[j] == variable_enum );
        }
        CPPAD_ASSERT_UNKNOWN(
            n_arg == size_t(n_con_arg + n_dyn_arg + n_var_arg)
        );
        //
        addr_t i_result = 0; // invalid value
        // -------------------------------------------------------------------
        // sum operator
        // -------------------------------------------------------------------
        if( op_enum == local::json::sum_json_op )
        {
            CPPAD_ASSERT_KNOWN( n_result == 1 ,
                "Json: sum operator: n_result is not 1"
            );
            if( n_var_arg == 0 )
            {   // result of the sum is a parameter
                Base sum_constant = 0.0;
                temporary.resize(0);
                for(size_t j = 0; j < n_arg; j++)
                {   if( type_x[j] == constant_enum )
                        sum_constant += parameter[ arg[j] ];
                    else
                    {   CPPAD_ASSERT_UNKNOWN( type_x[j] == dynamic_enum );
                        temporary.push_back( arg[j] );
                    }
                }
                CPPAD_ASSERT_UNKNOWN( temporary.size() == size_t(n_dyn_arg) );
                //
                // start with constant parameter
                i_result = rec.put_con_par(sum_constant);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == sum_constant );
                //
                // sum the dynamic parameters
                for(addr_t j = 0; j < n_dyn_arg; ++j)
                {   i_result = rec.put_dyn_par(
                        nan, local::add_dyn, i_result, temporary[j]
                    );
                    CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                }
            }
            else
            {   // result of the sum is a variable
                size_t n_temporary = 6 + size_t(n_var_arg + n_dyn_arg);
                if( temporary.size() < n_temporary )
                    temporary.resize( n_temporary );
                Base sum_constant = 0.0;
                addr_t j_variable = 5 ;
                addr_t j_dynamic  = 5 + n_var_arg;
                for(size_t j = 0; j < n_arg; j++)
                {   if( type_x[j] == constant_enum )
                        sum_constant += parameter[ arg[j] ];
                    if( type_x[j] == variable_enum )
                        temporary[ j_variable++ ] = arg[j];
                    if( type_x[j] == dynamic_enum )
                        temporary[ j_dynamic++ ]  = arg[j];
                }
                temporary[j_dynamic] = j_dynamic;
                //
                temporary[0] = rec.put_con_par(sum_constant);
                CPPAD_ASSERT_UNKNOWN(parameter[temporary[0]] == sum_constant);
                //
                temporary[1] = 5 + n_var_arg;
                temporary[2] = 5 + n_var_arg;
                temporary[3] = temporary[2] + n_dyn_arg;
                temporary[4] = temporary[2] + n_dyn_arg;
                //
                i_result = rec.PutOp(local::CSumOp);
                for(size_t j = 0; j < n_temporary; ++j)
                    rec.PutArg( temporary[j] );
                CPPAD_ASSERT_UNKNOWN( local::NumRes(local::CSumOp) == 1 );
            }
        }
        // -------------------------------------------------------------------
        // atomic operator
        // -------------------------------------------------------------------
        else if( op_enum == local::json::atom_json_op )
        {   //
            // atomic_index
            size_t atomic_index = json_op.atomic_index;
            CPPAD_ASSERT_UNKNOWN( atomic_index != 0 );
            //
            // afun
            bool         set_null = false;
            size_t       type;
            std::string* name = CPPAD_NULL;
            void*        v_ptr;
            CppAD::local::atomic_index<double>(
                set_null, atomic_index, type, name, v_ptr
            );
            CPPAD_ASSERT_UNKNOWN( type == 3 );
            atomic_three<RecBase>* afun =
                reinterpret_cast< atomic_three<RecBase>* >( v_ptr );
            //
            // parameter_x
            parameter_x.resize(n_arg);
            for(size_t j = 0; j < n_arg; ++j)
            {   if( type_x[j] == constant_enum )
                    parameter_x[j] = parameter[ arg[j] ];
                else
                    parameter_x[j] = nan;
            }
            //
            // type_y
            type_y.resize(n_result);
            afun->for_type(parameter_x, type_x, type_y);
            //
            // taylor_y
            size_t need_y    = size_t(constant_enum);
            size_t order_low = 0;
            size_t order_up  = 0;
            taylor_y.resize(n_result);
            afun->forward(
                parameter_x ,
                type_x      ,
                need_y      ,
                order_low   ,
                order_up    ,
                parameter_x ,
                taylor_y
            );
            //
            // record_dynamic, record_variable
            bool record_dynamic  = false;
            bool record_variable = false;
            for(size_t i = 0; i < n_result; ++i)
            {   CPPAD_ASSERT_UNKNOWN( type_y[i] <= variable_enum );
                record_dynamic  |= type_y[i] == dynamic_enum;
                record_variable |= type_y[i] == variable_enum;
            }
            // tape_id is zero because not a true recording
            tape_id_t tape_id = 0;
            //
            // ax, ay
            if( record_dynamic || record_variable )
            {   // tape_id (not a recording AD<Base> operations)
                // ax
                ax.resize(n_arg);
                for(size_t j = 0; j < n_arg; ++j)
                {   ax[j].value_ = parameter_x[j];
                    ax[j].taddr_ = arg[j];
                }
                // ay
                ay.resize(n_result);
                for(size_t i = 0; i < n_result; ++i)
                {   ay[i].value_ = taylor_y[i];
                    ay[i].taddr_ = 0;
                }
            }
            if( record_dynamic ) rec.put_dyn_atomic(
                    tape_id, atomic_index, type_x, type_y, ax, ay
            );
            if( record_variable ) rec.put_var_atomic(
                    tape_id, atomic_index, type_x, type_y, ax, ay
            );
            //
            // node_type, node2fun
            for(size_t i = 0; i < n_result; ++i)
            {   node_type[start_result + i] = type_y[i];
                 switch( type_y[i] )
                {   case constant_enum:
                    node2fun[start_result + i] = rec.put_con_par( taylor_y[i] );
                    break;

                    case dynamic_enum:
                    case variable_enum:
                    node2fun[start_result + i] = ay[i].taddr_;
                    break;

                    default:
                    CPPAD_ASSERT_UNKNOWN(false);
                    break;
                }
            }
        }
        // -------------------------------------------------------------------
        // unary operators
        // -------------------------------------------------------------------
        else if( n_arg == 1 )
        {   CPPAD_ASSERT_UNKNOWN( n_arg == 1 && n_result == 1 );
            Base result; // used in cases argument is a constant
            if( type_x[0] == variable_enum ) switch( op_enum )
            {
                case local::json::abs_json_op:
                i_result = rec.PutOp(local::AbsOp);
                rec.PutArg( arg[0] );
                CPPAD_ASSERT_UNKNOWN( NumArg(local::AbsOp) == 1 );
                break;

                case local::json::acosh_json_op:
                i_result = rec.PutOp(local::AcoshOp);
                rec.PutArg( arg[0] );
                CPPAD_ASSERT_UNKNOWN( NumArg(local::AcoshOp) == 1 );
                break;

                case local::json::asinh_json_op:
                i_result = rec.PutOp(local::AsinhOp);
                rec.PutArg( arg[0] );
                CPPAD_ASSERT_UNKNOWN( NumArg(local::AsinhOp) == 1 );
                break;

                case local::json::atanh_json_op:
                i_result = rec.PutOp(local::AtanhOp);
                rec.PutArg( arg[0] );
                CPPAD_ASSERT_UNKNOWN( NumArg(local::AtanhOp) == 1 );
                break;

                case local::json::erf_json_op:
                i_result = rec.PutOp(local::ErfOp);
                CPPAD_ASSERT_UNKNOWN( NumArg(local::ErfOp) == 3 );
                //
                // arg[0] = variable index for function argument
                rec.PutArg( arg[0] );
                //
                // parameter[ arg[1] ] = 0.0
                i_par = rec.put_con_par( Base(0.0) );
                rec.PutArg( i_par );
                //
                // parameter[ arg[2] ] = 2 / sqrt(pi)
                i_par = rec.put_con_par(Base(
                    1.0 / std::sqrt( std::atan(1.0) )
                ));
                rec.PutArg( i_par );
                //
                break;

                case local::json::erfc_json_op:
                i_result = rec.PutOp(local::ErfcOp);
                CPPAD_ASSERT_UNKNOWN( NumArg(local::ErfcOp) == 3 );
                //
                // arg[0] = variable index for function argument
                rec.PutArg( arg[0] );
                //
                // parameter[ arg[1] ] = 0.0
                i_par = rec.put_con_par( Base(0.0) );
                rec.PutArg( i_par );
                //
                // parameter[ arg[2] ] = 2 / sqrt(pi)
                i_par = rec.put_con_par(Base(
                    1.0 / std::sqrt( std::atan(1.0) )
                ));
                rec.PutArg( i_par );
                //
                break;

                case local::json::acos_json_op:
                i_result = rec.PutOp(local::AcosOp);
                rec.PutArg( arg[0] );
                CPPAD_ASSERT_UNKNOWN( NumArg(local::AcosOp) == 1 );
                break;

                case local::json::asin_json_op:
                i_result = rec.PutOp(local::AsinOp);
                rec.PutArg( arg[0] );
                CPPAD_ASSERT_UNKNOWN( NumArg(local::AsinOp) == 1 );
                break;

                case local::json::atan_json_op:
                i_result = rec.PutOp(local::AtanOp);
                rec.PutArg( arg[0] );
                CPPAD_ASSERT_UNKNOWN( NumArg(local::AtanOp) == 1 );
                break;

                case local::json::cosh_json_op:
                i_result = rec.PutOp(local::CoshOp);
                rec.PutArg( arg[0] );
                CPPAD_ASSERT_UNKNOWN( NumArg(local::CoshOp) == 1 );
                break;

                case local::json::cos_json_op:
                i_result = rec.PutOp(local::CosOp);
                rec.PutArg( arg[0] );
                CPPAD_ASSERT_UNKNOWN( NumArg(local::CosOp) == 1 );
                break;

                case local::json::exp_json_op:
                i_result = rec.PutOp(local::ExpOp);
                rec.PutArg( arg[0] );
                CPPAD_ASSERT_UNKNOWN( NumArg(local::ExpOp) == 1 );
                break;

                case local::json::log_json_op:
                i_result = rec.PutOp(local::LogOp);
                rec.PutArg( arg[0] );
                CPPAD_ASSERT_UNKNOWN( NumArg(local::LogOp) == 1 );
                break;

                case local::json::sign_json_op:
                i_result = rec.PutOp(local::SignOp);
                rec.PutArg( arg[0] );
                CPPAD_ASSERT_UNKNOWN( NumArg(local::SignOp) == 1 );
                break;

                case local::json::sinh_json_op:
                i_result = rec.PutOp(local::SinhOp);
                rec.PutArg( arg[0] );
                CPPAD_ASSERT_UNKNOWN( NumArg(local::SinhOp) == 1 );
                break;

                case local::json::sin_json_op:
                i_result = rec.PutOp(local::SinOp);
                rec.PutArg( arg[0] );
                CPPAD_ASSERT_UNKNOWN( NumArg(local::SinOp) == 1 );
                break;

                case local::json::sqrt_json_op:
                i_result = rec.PutOp(local::SqrtOp);
                rec.PutArg( arg[0] );
                CPPAD_ASSERT_UNKNOWN( NumArg(local::SqrtOp) == 1 );
                break;

                case local::json::tanh_json_op:
                i_result = rec.PutOp(local::TanhOp);
                rec.PutArg( arg[0] );
                CPPAD_ASSERT_UNKNOWN( NumArg(local::TanhOp) == 1 );
                break;

                case local::json::tan_json_op:
                i_result = rec.PutOp(local::TanOp);
                rec.PutArg( arg[0] );
                CPPAD_ASSERT_UNKNOWN( NumArg(local::TanOp) == 1 );
                break;

                default:
                CPPAD_ASSERT_UNKNOWN( false );
                break;
            }
            else if( type_x[0] == dynamic_enum ) switch( op_enum )
            {
                case local::json::abs_json_op:
                i_result = rec.put_dyn_par(nan, local::abs_dyn, arg[0] );
                CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                break;

                case local::json::acosh_json_op:
                i_result = rec.put_dyn_par(nan, local::acosh_dyn, arg[0] );
                CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                break;

                case local::json::asinh_json_op:
                i_result = rec.put_dyn_par(nan, local::asinh_dyn, arg[0] );
                CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                break;

                case local::json::atanh_json_op:
                i_result = rec.put_dyn_par(nan, local::atanh_dyn, arg[0] );
                CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                break;

                case local::json::erf_json_op:
                i_result = rec.put_dyn_par(nan, local::erf_dyn, arg[0] );
                CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                break;

                case local::json::erfc_json_op:
                i_result = rec.put_dyn_par(nan, local::erfc_dyn, arg[0] );
                CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                break;

                case local::json::acos_json_op:
                i_result = rec.put_dyn_par(nan, local::acos_dyn, arg[0] );
                CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                break;

                case local::json::asin_json_op:
                i_result = rec.put_dyn_par(nan, local::asin_dyn, arg[0] );
                CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                break;

                case local::json::atan_json_op:
                i_result = rec.put_dyn_par(nan, local::atan_dyn, arg[0] );
                CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                break;

                case local::json::cosh_json_op:
                i_result = rec.put_dyn_par(nan, local::cosh_dyn, arg[0] );
                CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                break;

                case local::json::cos_json_op:
                i_result = rec.put_dyn_par(nan, local::cos_dyn, arg[0] );
                CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                break;

                case local::json::exp_json_op:
                i_result = rec.put_dyn_par(nan, local::exp_dyn, arg[0] );
                CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                break;

                case local::json::log_json_op:
                i_result = rec.put_dyn_par(nan, local::log_dyn, arg[0] );
                CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                break;

                case local::json::sign_json_op:
                i_result = rec.put_dyn_par(nan, local::sign_dyn, arg[0] );
                CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                break;

                case local::json::sinh_json_op:
                i_result = rec.put_dyn_par(nan, local::sinh_dyn, arg[0] );
                CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                break;

                case local::json::sin_json_op:
                i_result = rec.put_dyn_par(nan, local::sin_dyn, arg[0] );
                CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                break;

                case local::json::sqrt_json_op:
                i_result = rec.put_dyn_par(nan, local::sqrt_dyn, arg[0] );
                CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                break;

                case local::json::tanh_json_op:
                i_result = rec.put_dyn_par(nan, local::tanh_dyn, arg[0] );
                CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                break;

                case local::json::tan_json_op:
                i_result = rec.put_dyn_par(nan, local::tan_dyn, arg[0] );
                CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                break;

                default:
                CPPAD_ASSERT_UNKNOWN( false );
                break;
            }
            else switch( op_enum )
            {
                case local::json::abs_json_op:
                result    = CppAD::abs( parameter[ arg[0] ] );
                i_result  = rec.put_con_par(result);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
                break;

                case local::json::acosh_json_op:
                result    = CppAD::acosh( parameter[ arg[0] ] );
                i_result  = rec.put_con_par(result);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
                break;

                case local::json::asinh_json_op:
                result    = CppAD::asinh( parameter[ arg[0] ] );
                i_result  = rec.put_con_par(result);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
                break;

                case local::json::atanh_json_op:
                result    = CppAD::atanh( parameter[ arg[0] ] );
                i_result  = rec.put_con_par(result);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
                break;

                case local::json::erf_json_op:
                result    = CppAD::erf( parameter[ arg[0] ] );
                i_result  = rec.put_con_par(result);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
                break;

                case local::json::erfc_json_op:
                result    = CppAD::erfc( parameter[ arg[0] ] );
                i_result  = rec.put_con_par(result);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
                break;

                case local::json::acos_json_op:
                result    = CppAD::acos( parameter[ arg[0] ] );
                i_result  = rec.put_con_par(result);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
                break;

                case local::json::asin_json_op:
                result    = CppAD::asin( parameter[ arg[0] ] );
                i_result  = rec.put_con_par(result);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
                break;

                case local::json::atan_json_op:
                result    = CppAD::atan( parameter[ arg[0] ] );
                i_result  = rec.put_con_par(result);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
                break;

                case local::json::cosh_json_op:
                result    = CppAD::cosh( parameter[ arg[0] ] );
                i_result  = rec.put_con_par(result);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
                break;

                case local::json::cos_json_op:
                result    = CppAD::cos( parameter[ arg[0] ] );
                i_result  = rec.put_con_par(result);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
                break;

                case local::json::exp_json_op:
                result    = CppAD::exp( parameter[ arg[0] ] );
                i_result  = rec.put_con_par(result);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
                break;

                case local::json::log_json_op:
                result    = CppAD::log( parameter[ arg[0] ] );
                i_result  = rec.put_con_par(result);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
                break;

                case local::json::sign_json_op:
                result    = CppAD::sign( parameter[ arg[0] ] );
                i_result  = rec.put_con_par(result);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
                break;

                case local::json::sinh_json_op:
                result    = CppAD::sinh( parameter[ arg[0] ] );
                i_result  = rec.put_con_par(result);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
                break;

                case local::json::sin_json_op:
                result    = CppAD::sin( parameter[ arg[0] ] );
                i_result  = rec.put_con_par(result);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
                break;

                case local::json::sqrt_json_op:
                result    = CppAD::sqrt( parameter[ arg[0] ] );
                i_result  = rec.put_con_par(result);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
                break;

                case local::json::tanh_json_op:
                result    = CppAD::tanh( parameter[ arg[0] ] );
                i_result  = rec.put_con_par(result);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
                break;

                case local::json::tan_json_op:
                result    = CppAD::tan( parameter[ arg[0] ] );
                i_result  = rec.put_con_par(result);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
                break;

                default:
                CPPAD_ASSERT_UNKNOWN( false );
                break;
            }
        }
        // -------------------------------------------------------------------
        // binary operators
        // -------------------------------------------------------------------
        else
        {   CPPAD_ASSERT_UNKNOWN( n_arg == 2 && n_result == 1 );
            Base result; // used in cases where both arguments are constants
            if( type_x[0] == variable_enum && type_x[1] == variable_enum )
            switch( op_enum )
            {
                case local::json::add_json_op:
                i_result = rec.PutOp(local::AddvvOp);
                rec.PutArg( arg[0], arg[1] );
                CPPAD_ASSERT_NARG_NRES(local::AddvvOp, 2, 1);
                break;

                case local::json::mul_json_op:
                i_result = rec.PutOp(local::MulvvOp);
                rec.PutArg( arg[0], arg[1] );
                CPPAD_ASSERT_NARG_NRES(local::MulvvOp, 2, 1);
                break;

                case local::json::sub_json_op:
                i_result = rec.PutOp(local::SubvvOp);
                rec.PutArg( arg[0], arg[1] );
                CPPAD_ASSERT_NARG_NRES(local::SubvvOp, 2, 1);
                break;

                case local::json::div_json_op:
                i_result = rec.PutOp(local::DivvvOp);
                rec.PutArg( arg[0], arg[1] );
                CPPAD_ASSERT_NARG_NRES(local::DivvvOp, 2, 1);
                break;

                default:
                CPPAD_ASSERT_UNKNOWN( false );
                break;
            }
            else if( type_x[0] == variable_enum ) switch( op_enum )
            {
                // addition is communitative, so use Addpv
                case local::json::add_json_op:
                i_result = rec.PutOp(local::AddpvOp);
                rec.PutArg( arg[1], arg[0] );
                CPPAD_ASSERT_NARG_NRES(local::AddpvOp, 2, 1);
                break;

                // multiplication is communitative, so use Mulpv
                case local::json::mul_json_op:
                i_result = rec.PutOp(local::MulpvOp);
                rec.PutArg( arg[1], arg[0] );
                CPPAD_ASSERT_NARG_NRES(local::MulpvOp, 2, 1);
                break;

                case local::json::sub_json_op:
                i_result = rec.PutOp(local::SubvpOp);
                rec.PutArg( arg[0], arg[1] );
                CPPAD_ASSERT_NARG_NRES(local::SubvpOp, 2, 1);
                break;

                case local::json::div_json_op:
                i_result = rec.PutOp(local::DivvpOp);
                rec.PutArg( arg[0], arg[1] );
                CPPAD_ASSERT_NARG_NRES(local::DivvpOp, 2, 1);
                break;

                default:
                CPPAD_ASSERT_UNKNOWN( false );
                break;
            }
            else if( type_x[1] == variable_enum ) switch( op_enum )
            {
                case local::json::add_json_op:
                i_result = rec.PutOp(local::AddpvOp);
                rec.PutArg( arg[0], arg[1] );
                CPPAD_ASSERT_NARG_NRES(local::AddpvOp, 2, 1);
                break;

                case local::json::mul_json_op:
                i_result = rec.PutOp(local::MulpvOp);
                rec.PutArg( arg[0], arg[1] );
                CPPAD_ASSERT_NARG_NRES(local::MulpvOp, 2, 1);
                break;

                case local::json::sub_json_op:
                i_result = rec.PutOp(local::SubpvOp);
                rec.PutArg( arg[0], arg[1] );
                CPPAD_ASSERT_NARG_NRES(local::SubpvOp, 2, 1);
                break;

                case local::json::div_json_op:
                i_result = rec.PutOp(local::DivpvOp);
                rec.PutArg( arg[0], arg[1] );
                CPPAD_ASSERT_NARG_NRES(local::DivpvOp, 2, 1);
                break;

                default:
                CPPAD_ASSERT_UNKNOWN( false );
                break;
            }
            else if( type_x[0] == dynamic_enum || type_x[1] == dynamic_enum )
            switch( op_enum )
            {
                case local::json::add_json_op:
                i_result = rec.put_dyn_par(nan, local::add_dyn, arg[0], arg[1]);
                CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                break;

                case local::json::mul_json_op:
                i_result = rec.put_dyn_par(nan, local::mul_dyn, arg[0], arg[1]);
                CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                break;

                case local::json::sub_json_op:
                i_result = rec.put_dyn_par(nan, local::sub_dyn, arg[0], arg[1]);
                CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                break;

                case local::json::div_json_op:
                i_result = rec.put_dyn_par(nan, local::div_dyn, arg[0], arg[1]);
                CPPAD_ASSERT_UNKNOWN( isnan( parameter[i_result] ) );
                break;

                default:
                CPPAD_ASSERT_UNKNOWN( false );
                break;
            }
            else switch( op_enum )
            {
                case local::json::add_json_op:
                result = parameter[ arg[0] ] + parameter[ arg[1] ];
                i_result = rec.put_con_par(result);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
                break;

                case local::json::mul_json_op:
                result = parameter[ arg[0] ] * parameter[ arg[1] ];
                i_result = rec.put_con_par(result);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
                break;

                case local::json::sub_json_op:
                result = parameter[ arg[0] ] - parameter[ arg[1] ];
                i_result = rec.put_con_par(result);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
                break;

                case local::json::div_json_op:
                result = parameter[ arg[0] ] / parameter[ arg[1] ];
                i_result = rec.put_con_par(result);
                CPPAD_ASSERT_UNKNOWN( parameter[i_result] == result );
                break;

                default:
                CPPAD_ASSERT_UNKNOWN( false );
                break;

            }
        }
        // case where node_type and node2fun for the results are set above
        if( op_enum != local::json::atom_json_op )
        {   // set node_type and node2fun for result
            //
            CPPAD_ASSERT_UNKNOWN( i_result != 0 );
            CPPAD_ASSERT_UNKNOWN( n_result == 1 );
            if( n_var_arg > 0 )
                node_type[start_result] = variable_enum;
            else if( n_dyn_arg > 0 )
                node_type[start_result] = dynamic_enum;
            else
                node_type[start_result] = constant_enum;
            node2fun[start_result] = i_result;
        }
        //
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
    // set the function name
    function_name_ = function_name;
    //
    return;
}

# endif
