# ifndef CPPAD_LOCAL_GRAPH_TO_GRAPH_HPP
# define CPPAD_LOCAL_GRAPH_TO_GRAPH_HPP

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
# include <cppad/local/graph/cpp_graph_op.hpp>

/*
------------------------------------------------------------------------------
$begin to_graph$$
$spell
    Json
    cpp
    ind
    vec
    arg
$$

$section Create a C++ AD Graph Corresponding to an ADFun Object$$

$head Under Construction$$
This routine is under construction and subject to
change without backward compatibility.

$head Syntax$$
$codei%
    %graph% = %fun%.to_graph(
        %function_name%,
        %atomic_name_vec%,
        %n_dynamic_ind%,
        %n_independent%,
        %constant_vec%,
        %operator_vec%,
        %operator_arg%,
        %dependent_vec%
    )
%$$

$head Prototype$$
$srcfile%include/cppad/local/graph/to_graph.hpp%
    0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1
%$$

$head Base$$
is the type corresponding to this $cref/ADFun/adfun/$$ object;
i.e., its calculations are done using the type $icode Base$$.

$head RecBase$$
in the prototype above, $icode RecBase$$ is the same type as $icode Base$$.

$head Arguments$$
All the arguments to this routine are all outputs
and have the following meaning:
$table
$cref/function_name/cpp_ad_graph/function_name/$$ $rnext
$cref/atomic_name_vec/cpp_ad_graph/atomic_name_vec/$$ $rnext
$cref/n_dynamic_ind/cpp_ad_graph/n_dynamic_ind/$$ $rnext
$cref/n_independent/cpp_ad_graph/n_independent/$$ $rnext
$cref/constant_vec/cpp_ad_graph/constant_vec/$$ $rnext
$cref/operator_vec/cpp_ad_graph/operator_vec/$$ $rnext
$cref/operator_arg/cpp_ad_graph/operator_arg/$$ $rnext
$cref/dependent_vec/cpp_ad_graph/dependent_vec/$$
$tend

$head Examples$$
See $cref/from_graph examples/from_graph/Examples/$$.

$end
*/
// BEGIN_PROTOTYPE
template <class Base, class RecBase>
void CppAD::ADFun<Base,RecBase>::to_graph(
    std::string&                                   function_name   ,
    CppAD::vector<std::string>&                    atomic_name_vec ,
    size_t&                                        n_dynamic_ind   ,
    size_t&                                        n_independent   ,
    CppAD::vector<double>&                         constant_vec    ,
    CppAD::vector<local::graph::graph_op_struct>&  operator_vec    ,
    CppAD::vector<size_t>&                         operator_arg    ,
    CppAD::vector<size_t>&                         dependent_vec   )
// END_PROTOTYPE
{   using local::pod_vector;
    using local::opcode_t;
    // --------------------------------------------------------------------
    if( local::graph::op_name2enum.size() == 0 )
    {   CPPAD_ASSERT_KNOWN( ! thread_alloc::in_parallel() ,
            "call to set_operator_info in parallel mode"
        );
        local::graph::set_operator_info();
    }
    // --------------------------------------------------------------------
    // some constants
    // --------------------------------------------------------------------
    //
    // output: function_name
    function_name = function_name_;
    //
    // dynamic parameter information
    const pod_vector<opcode_t>& dyn_par_op ( play_.dyn_par_op()  );
    const pod_vector<addr_t>&   dyn_par_arg( play_.dyn_par_arg() );
    const pod_vector<addr_t>&   dyn_ind2par_ind ( play_.dyn_ind2par_ind() );
    const pod_vector<bool>&     dyn_par_is( play_.dyn_par_is() );
    //
    // number of dynamic parameters
    const size_t n_dynamic     = dyn_ind2par_ind.size();
    //
    // output: n_dynamic_ind
    n_dynamic_ind = play_.num_dynamic_ind();
    //
    // number of parameters
    const size_t n_parameter = play_.num_par_rec();
    //
    // number of constant parameters
# ifndef NDEBUG
    const size_t n_constant = n_parameter - n_dynamic - 1;
# endif
    //
    // output: n_indepdendent
    n_independent = ind_taddr_.size();
    //
    // value of parameters
    const Base* parameter = play_.GetPar();
    //
    // number of variables
    const size_t n_variable = play_.num_var_rec();
    //
    // some checks
    CPPAD_ASSERT_UNKNOWN( n_dynamic_ind <= n_dynamic );
    CPPAD_ASSERT_UNKNOWN( dyn_par_is.size() == n_parameter );
    CPPAD_ASSERT_UNKNOWN( n_parameter > 0 );
    CPPAD_ASSERT_UNKNOWN( isnan( parameter[0] ) );
    CPPAD_ASSERT_UNKNOWN( ! dyn_par_is[0] );
    // --------------------------------------------------------------------
    // par2node
    pod_vector<size_t> par2node(n_parameter);
    par2node[0] = 0; // invalid value
    for(size_t i = 1; i <= n_dynamic_ind; ++i)
        par2node[i] = i; // independent dynamic parameters
    for(size_t i = n_dynamic_ind + 1; i < n_parameter; ++i)
        par2node[i] = 0; // will be set later
    // ----------------------------------------------------------------------
    //
    // initialize index of previous node in the graph
    size_t previous_node = 0;
    //
    // n_dynamic_ind
    previous_node += n_dynamic_ind;
    //
    // n_independent
    previous_node += n_independent;
    // --------------------------------------------------------------------
    //
    // output: constant_vec
    // constant_vec and par2node for constants
    constant_vec.resize(0);
    for(size_t i = 1; i < n_parameter; ++i)
    {   if( ! dyn_par_is[i] )
        {   // this is a constant node
            constant_vec.push_back( parameter[i] );
            par2node[i] = ++previous_node;
        }
    }
    CPPAD_ASSERT_UNKNOWN( n_constant == constant_vec.size() );
    // ----------------------------------------------------------------------
    //  output: initialize atomic_name_vec, operator_vec, operator_arg
    atomic_name_vec.resize(0);
    operator_vec.resize(0);
    operator_arg.resize(0);
    // temporary used for elements of operator_vec
    local::graph::graph_op_struct         op_usage;
    //
    // Json operators is dynamic operators plus variables operators.
    // Skip BeginOp, EndOp, and independent variables.
    //
    // dynamic parameter operations and par2node
    // for dynamic parameters that are not constants or independent
    CPPAD_ASSERT_UNKNOWN( num_arg_dyn(local::ind_dyn) == 0 );
    size_t i_arg = 0;
    pod_vector<size_t> node_arg;

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
        if( n_arg > node_arg.size() )
            node_arg.resize(n_arg);
        //
        // arguments in graph node space
        for(size_t i = 0; i < n_arg; ++i)
        {   node_arg[i] = par2node[ dyn_par_arg[i_arg + i] ];
            CPPAD_ASSERT_UNKNOWN(
                node_arg[i] > 0 ||
                ( dyn_op == local::cond_exp_dyn && i == 0 )
            );
        }
        //
        // invalid value
        local::graph::graph_op_enum op_code = local::graph::n_graph_op;
        switch(dyn_op)
        {
            // ---------------------------------------------------------------
            // unary operators

            case local::abs_dyn:
            op_code = local::graph::abs_graph_op;
            break;

            case local::acosh_dyn:
            op_code = local::graph::acosh_graph_op;
            break;

            case local::asinh_dyn:
            op_code = local::graph::asinh_graph_op;
            break;

            case local::atanh_dyn:
            op_code = local::graph::atanh_graph_op;
            break;

            case local::erf_dyn:
            op_code = local::graph::erf_graph_op;
            break;

            case local::erfc_dyn:
            op_code = local::graph::erfc_graph_op;
            break;

            case local::expm1_dyn:
            op_code = local::graph::expm1_graph_op;
            break;

            case local::log1p_dyn:
            op_code = local::graph::log1p_graph_op;
            break;

            case local::acos_dyn:
            op_code = local::graph::acos_graph_op;
            break;

            case local::asin_dyn:
            op_code = local::graph::asin_graph_op;
            break;

            case local::atan_dyn:
            op_code = local::graph::atan_graph_op;
            break;

            case local::cosh_dyn:
            op_code = local::graph::cosh_graph_op;
            break;

            case local::cos_dyn:
            op_code = local::graph::cos_graph_op;
            break;

            case local::exp_dyn:
            op_code = local::graph::exp_graph_op;
            break;

            case local::log_dyn:
            op_code = local::graph::log_graph_op;
            break;

            case local::sign_dyn:
            op_code = local::graph::sign_graph_op;
            break;

            case local::sinh_dyn:
            op_code = local::graph::sinh_graph_op;
            break;

            case local::sin_dyn:
            op_code = local::graph::sin_graph_op;
            break;

            case local::sqrt_dyn:
            op_code = local::graph::sqrt_graph_op;
            break;

            case local::tanh_dyn:
            op_code = local::graph::tanh_graph_op;
            break;

            case local::tan_dyn:
            op_code = local::graph::tan_graph_op;
            break;

            // ---------------------------------------------------------------
            // binary operators

            case local::add_dyn:
            op_code = local::graph::add_graph_op;
            break;

            case local::mul_dyn:
            op_code = local::graph::mul_graph_op;
            break;

            case local::sub_dyn:
            op_code = local::graph::sub_graph_op;
            break;

            case local::div_dyn:
            op_code = local::graph::div_graph_op;
            break;

            // ---------------------------------------------------------------
            case local::call_dyn:
            op_code = local::graph::atom_graph_op;
            break;

            case local::cond_exp_dyn: // op_code determined below
            case local::result_dyn:   // no graph operation necessary
            break;

            // ---------------------------------------------------------------
            default:
            // This error should have been reported above
            CPPAD_ASSERT_UNKNOWN( false );
            break;
        }
        if( n_arg == 1 || n_arg == 2 )
        {   // unary or binary
            op_usage.n_result    = 1;
            op_usage.n_arg       = n_arg;
            op_usage.start_arg   = operator_arg.size();
            op_usage.op_enum     = op_code;
            //
            operator_vec.push_back( op_usage );
            for(size_t i = 0; i < n_arg; ++i)
                operator_arg.push_back( node_arg[i] );
        }
        else if( dyn_op == local::result_dyn )
        {   // setting par2node[i_dyn] above is all that is necessary
            CPPAD_ASSERT_UNKNOWN( op_code == 0 );
            CPPAD_ASSERT_UNKNOWN( n_arg == 0 );
        }
        else if( dyn_op == local::call_dyn )
        {   // arg[0]: atomic function index
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
            // set index for this atomic function call
            size_t name_index = atomic_name_vec.size();
            for(size_t i = 0; i < atomic_name_vec.size(); ++i)
            {   if( atomic_name_vec[i] == name )
                {   if( name_index == atomic_name_vec.size() )
                        name_index = i;
                    else
                    {   std::string msg  = "The atomic function name "
                            + name + " is used for multiple functions";
                        CPPAD_ASSERT_KNOWN(false, msg.c_str() );
                    }
                }
            }
            if( name_index == atomic_name_vec.size() )
                atomic_name_vec.push_back(name);
            //
            // for atom_graph_op:
            // name_index, n_result, n_arg come before start_arg
            operator_arg.push_back(name_index);
            operator_arg.push_back(n_result);
            operator_arg.push_back(n_arg);
            //
            op_code             = local::graph::atom_graph_op;
            op_usage.n_result   = n_result;
            op_usage.n_arg      = n_arg_fun;
            op_usage.start_arg  = operator_arg.size();
            op_usage.op_enum    = op_code;
            operator_vec.push_back( op_usage );
            //
            for(size_t j  = 0; j < n_arg_fun; ++j)
            {   // arg[4 + j] is j-th argument to the function
                size_t node_j = par2node[ dyn_par_arg[i_arg + 4 + j] ];
                CPPAD_ASSERT_UNKNOWN( node_j < i_par );
                operator_arg.push_back( node_j );
            }
        }
        else
        {   CPPAD_ASSERT_UNKNOWN( dyn_op == local::cond_exp_dyn )
            CompareOp cop = CompareOp( dyn_par_arg[i_arg + 0] );
            size_t left     = node_arg[1];
            size_t right    = node_arg[2];
            size_t if_true  = node_arg[3];
            size_t if_false = node_arg[4];
            switch( cop )
            {   case CompareLt:
                op_code = local::graph::cexp_lt_graph_op;
                break;

                case CompareLe:
                op_code = local::graph::cexp_le_graph_op;
                break;

                case CompareEq:
                op_code = local::graph::cexp_eq_graph_op;
                break;

                case CompareGe:
                op_code = local::graph::cexp_lt_graph_op;
                std::swap(if_true, if_false);
                break;

                case CompareGt:
                op_code = local::graph::cexp_le_graph_op;
                std::swap(if_true, if_false);
                break;

                case CompareNe:
                op_code = local::graph::cexp_eq_graph_op;
                std::swap(if_true, if_false);
                break;

                default:
                CPPAD_ASSERT_UNKNOWN(false);
                break;
            }
            op_usage.n_result    = 1;
            op_usage.n_arg       = 4;
            op_usage.start_arg   = operator_arg.size();
            op_usage.op_enum     = op_code;
            operator_vec.push_back( op_usage );
            operator_arg.push_back( left );
            operator_arg.push_back( right );
            operator_arg.push_back( if_true );
            operator_arg.push_back( if_false );
        }
        i_arg  += n_arg;
    }
    // ----------------------------------------------------------------------
    // variable operators
    pod_vector<size_t> var2node(n_variable);
    var2node[0] = 0; // invalide node value
    for(size_t i = 1; i <= n_independent; ++i)
        var2node[i] = n_dynamic_ind + i;
    for(size_t i = n_independent + 1; i < n_variable; ++i)
        var2node[i] = 0; // invalid node value
    //
    local::play::const_sequential_iterator itr  = play_.begin();
    local::OpCode      var_op;
    const              addr_t* arg;
    size_t             i_var;
    pod_vector<bool>   is_var(2);
    vector<size_t>     atom_node_arg;
    bool in_atomic_call = false;
    bool more_operators = true;
    while(more_operators)
    {   // if non-zero, this is a fixed size operator with this many arguments
        // and implemented after the switch. In additionk, is_var is set for
        // each of the at most 2 arguments.
        size_t fixed_n_arg = 0;

        // invalid value
        local::graph::graph_op_enum op_code = local::graph::n_graph_op;

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
                op_code     = local::graph::abs_graph_op;
                break;

                case local::AcoshOp:
                op_code     = local::graph::acosh_graph_op;
                break;

                case local::AsinhOp:
                op_code     = local::graph::asinh_graph_op;
                break;

                case local::AtanhOp:
                op_code     = local::graph::atanh_graph_op;
                break;

                case local::ErfOp:
                op_code     = local::graph::erf_graph_op;
                break;

                case local::ErfcOp:
                op_code     = local::graph::erfc_graph_op;
                break;

                case local::Expm1Op:
                op_code     = local::graph::expm1_graph_op;
                break;

                case local::Log1pOp:
                op_code     = local::graph::log1p_graph_op;
                break;

                case local::AcosOp:
                op_code     = local::graph::acos_graph_op;
                break;

                case local::AsinOp:
                op_code     = local::graph::asin_graph_op;
                break;

                case local::AtanOp:
                op_code     = local::graph::atan_graph_op;
                break;

                case local::CoshOp:
                op_code     = local::graph::cosh_graph_op;
                break;

                case local::CosOp:
                op_code     = local::graph::cos_graph_op;
                break;

                case local::ExpOp:
                op_code     = local::graph::exp_graph_op;
                break;

                case local::LogOp:
                op_code     = local::graph::log_graph_op;
                break;

                case local::SignOp:
                op_code     = local::graph::sign_graph_op;
                break;

                case local::SinhOp:
                op_code     = local::graph::sinh_graph_op;
                break;

                case local::SinOp:
                op_code     = local::graph::sin_graph_op;
                break;

                case local::SqrtOp:
                op_code     = local::graph::sqrt_graph_op;
                break;

                case local::TanhOp:
                op_code     = local::graph::tanh_graph_op;
                break;

                case local::TanOp:
                op_code     = local::graph::tan_graph_op;
                break;

                // -----------------------------------------------------------
                // binary operators

                case local::AddpvOp:
                case local::AddvvOp:
                op_code = local::graph::add_graph_op;
                break;

                case local::MulpvOp:
                case local::MulvvOp:
                op_code = local::graph::mul_graph_op;
                break;

                case local::SubpvOp:
                case local::SubvpOp:
                case local::SubvvOp:
                op_code = local::graph::sub_graph_op;
                break;

                case local::DivpvOp:
                case local::DivvpOp:
                case local::DivvvOp:
                op_code = local::graph::div_graph_op;
                break;

                // -----------------------------------------------------------
                default:
                // This should be one of the cases above
                CPPAD_ASSERT_UNKNOWN(false);
                break;
            }
            //
            // var2node and previous_node for this operator
            var2node[i_var] = ++previous_node;
            //
            op_usage.n_result    = 1;
            op_usage.n_arg       = fixed_n_arg;
            op_usage.start_arg   = operator_arg.size();
            op_usage.op_enum     = op_code;
            //
            operator_vec.push_back( op_usage );
            for(size_t i = 0; i < fixed_n_arg; ++i)
            {   if( is_var[i] )
                    operator_arg.push_back( var2node[ arg[i] ] );
                else
                    operator_arg.push_back( par2node[ arg[i] ] );
            }
        }
        // -------------------------------------------------------------------
        // Other cases
        // -------------------------------------------------------------------
        else switch( var_op )
        {
            // -------------------------------------------------------------
            // comparison operators
            case local::EqppOp:
            case local::EqpvOp:
            case local::EqvvOp:
            case local::NeppOp:
            case local::NepvOp:
            case local::NevvOp:
            case local::LtppOp:
            case local::LtpvOp:
            case local::LtvpOp:
            case local::LtvvOp:
            case local::LeppOp:
            case local::LepvOp:
            case local::LevpOp:
            case local::LevvOp:
            {   // node_0, node_1
                size_t node_0, node_1;
                switch( var_op )
                {   // both nodes parameters
                    case local::EqppOp:
                    case local::NeppOp:
                    case local::LtppOp:
                    case local::LeppOp:
                    node_0 = par2node[arg[0]];
                    node_1 = par2node[arg[1]];
                    break;

                    // first node parameter, second variable
                    case local::EqpvOp:
                    case local::NepvOp:
                    case local::LtpvOp:
                    case local::LepvOp:
                    node_0 = par2node[arg[0]];
                    node_1 = var2node[arg[1]];
                    break;

                    // first node variable, second parameter
                    case local::LtvpOp:
                    case local::LevpOp:
                    node_0 = var2node[arg[0]];
                    node_1 = par2node[arg[1]];
                    break;

                    // both nodes variables
                    case local::EqvvOp:
                    case local::NevvOp:
                    case local::LtvvOp:
                    case local::LevvOp:
                    node_0 = var2node[arg[0]];
                    node_1 = var2node[arg[1]];
                    break;

                    // should never get here
                    default:
                    CPPAD_ASSERT_UNKNOWN(false);
                    node_0 = 0; // to avoid compiler warning
                    node_1 = 0;
                    break;
                }
                // op_code
                switch( var_op )
                {
                    case local::EqppOp:
                    case local::EqpvOp:
                    case local::EqvvOp:
                    op_code = local::graph::comp_eq_graph_op;
                    break;

                    case local::NeppOp:
                    case local::NepvOp:
                    case local::NevvOp:
                    op_code = local::graph::comp_ne_graph_op;
                    break;

                    case local::LtppOp:
                    case local::LtpvOp:
                    case local::LtvpOp:
                    case local::LtvvOp:
                    op_code = local::graph::comp_lt_graph_op;
                    break;

                    case local::LeppOp:
                    case local::LepvOp:
                    case local::LevpOp:
                    case local::LevvOp:
                    op_code = local::graph::comp_le_graph_op;
                    break;

                    // should never get here
                    default:
                    CPPAD_ASSERT_UNKNOWN(false);
                    op_code  = local::graph::n_graph_op; // invalid values
                    break;
                }
                op_usage.n_result    = 0;
                op_usage.n_arg       = 2;
                op_usage.start_arg   = operator_arg.size();
                op_usage.op_enum     = op_code;
                operator_vec.push_back( op_usage );
                operator_arg.push_back( node_0 );
                operator_arg.push_back( node_1 );
            }
            break;

            // --------------------------------------------------------------
            // CSumOp
            case local::CSumOp:
            {   // does this case have subtraction terms
                bool has_subtract = (arg[1] != arg[2]) | (arg[3] != arg[4]);
                //
                // var2node for this operator
                if( has_subtract )
                {   // two cumulative sum and one subtract operators
                    var2node[i_var] = previous_node + 3;
                }
                else
                {   // one cumulative sum operator
                    var2node[i_var] = previous_node + 1;
                }
                // Json
                //
                // previous_node + 1 = sum corresponding to addition terms
                op_code = local::graph::sum_graph_op;
                CPPAD_ASSERT_UNKNOWN( op_code != 0 );
                CPPAD_ASSERT_UNKNOWN( 5 <= arg[1] );
                CPPAD_ASSERT_UNKNOWN( arg[2] <= arg[3] );
                size_t n_arg = size_t(1 + arg[1] - 5 + arg[3] - arg[2]);
                op_usage.n_result    = 1;
                op_usage.n_arg       = n_arg;
                op_usage.start_arg   = operator_arg.size();
                op_usage.op_enum     = op_code;
                operator_vec.push_back( op_usage );
                //
                size_t arg_node  = par2node[ arg[0] ];
                operator_arg.push_back( arg_node );
                size_t j_arg = 1;
                for(addr_t i = 5; i < arg[1]; ++i)
                {   arg_node    = var2node[ arg[i] ];
                    CPPAD_ASSERT_UNKNOWN( arg_node > 0 );
                    operator_arg.push_back( arg_node );
                    ++j_arg;
                }
                for(addr_t i = arg[2]; i < arg[3]; ++i)
                {   arg_node  = par2node[ arg[i] ];
                    CPPAD_ASSERT_UNKNOWN( arg_node > 0 );
                    operator_arg.push_back( arg_node );
                    ++j_arg;
                }
                CPPAD_ASSERT_UNKNOWN( j_arg == n_arg );
                if( has_subtract )
                {   // previous_node + 2 = sum corresponding to subtract terms
                    CPPAD_ASSERT_UNKNOWN( arg[1] <= arg[2] );
                    CPPAD_ASSERT_UNKNOWN( arg[3] <= arg[4] );
                    n_arg = size_t(arg[2] - arg[1] + arg[4] - arg[3]);
                    op_code              = local::graph::sum_graph_op;
                    op_usage.n_result    = 1;
                    op_usage.n_arg       = n_arg;
                    op_usage.start_arg   = operator_arg.size();
                    op_usage.op_enum     = op_code;
                    operator_vec.push_back( op_usage );
                    j_arg = 0;
                    for(addr_t i = arg[1]; i < arg[2]; ++i)
                    {   arg_node    = var2node[ arg[i] ];
                        CPPAD_ASSERT_UNKNOWN( arg_node > 0 );
                        operator_arg.push_back( arg_node );
                        ++j_arg;
                    }
                    for(addr_t i = arg[3]; i < arg[4]; ++i)
                    {   arg_node  = par2node[ arg[i] ];
                        CPPAD_ASSERT_UNKNOWN( arg_node > 0 );
                        operator_arg.push_back( arg_node );
                        ++j_arg;
                    }
                    CPPAD_ASSERT_UNKNOWN( j_arg == n_arg );
                    //
                    // previous_node + 3 = first sum minus second sum
                    op_code = local::graph::sub_graph_op;
                    op_usage.n_result    = 1;
                    op_usage.n_arg       = 2;
                    op_usage.start_arg   = operator_arg.size();
                    op_usage.op_enum     = op_code;
                    operator_vec.push_back( op_usage );
                    operator_arg.push_back( previous_node + 1 );
                    operator_arg.push_back( previous_node + 2 );
                }
                // previous node
                if( has_subtract )
                    previous_node += 3;
                else
                    previous_node += 1;
            }
            itr.correct_before_increment();
            break;
            // --------------------------------------------------------------
            case local::FunapOp:
            atom_node_arg.push_back( par2node[arg[0]] );
            break;

            case local::FunavOp:
            CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) <= i_var );
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
                size_t atom_index   = size_t( arg[0] );
                size_t n_arg        = size_t( arg[2] );
                size_t n_result     = size_t( arg[3] );
                CPPAD_ASSERT_UNKNOWN( atom_node_arg.size() == n_arg );
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
                // set index for this atomic function
                size_t name_index = atomic_name_vec.size();
                for(size_t i = 0; i < atomic_name_vec.size(); ++i)
                {   if( atomic_name_vec[i] == name )
                    {   if( name_index == atomic_name_vec.size() )
                            name_index = i;
                        else
                        {   std::string msg  = "The atomic function name "
                                + name + " is used for multiple functions";
                            CPPAD_ASSERT_KNOWN(false, msg.c_str() );
                        }
                    }
                }
                if( name_index == atomic_name_vec.size() )
                    atomic_name_vec.push_back(name);
                //
                // for atom_graph_op:
                // name_index, n_result, n_arg come before start_arg
                operator_arg.push_back(name_index);
                operator_arg.push_back(n_result);
                operator_arg.push_back(n_arg);
                //
                op_code             = local::graph::atom_graph_op;
                op_usage.n_result   = n_result;
                op_usage.n_arg      = n_arg;
                op_usage.start_arg  = operator_arg.size();
                op_usage.op_enum    = op_code;
                operator_vec.push_back( op_usage );
                for(size_t i = 0; i < n_arg; ++i)
                    operator_arg.push_back( atom_node_arg[i] );
            }
            break;
            // --------------------------------------------------------------
            // CExpOp:
            case local::CExpOp:
            {   CompareOp cop = CompareOp( arg[0] );
                size_t left, right, if_true, if_false;
                if( arg[1] & 1 )
                    left = var2node[ arg[2] ];
                else
                    left = par2node[ arg[2] ];
                if( arg[1] & 2 )
                    right = var2node[ arg[3] ];
                else
                    right = par2node[ arg[3] ];
                if( arg[1] & 4 )
                    if_true = var2node[ arg[4] ];
                else
                    if_true = par2node[ arg[4] ];
                if( arg[1] & 8 )
                    if_false = var2node[ arg[5] ];
                else
                    if_false = par2node[ arg[5] ];
                switch( cop )
                {   case CompareLt:
                    op_code = local::graph::cexp_lt_graph_op;
                    break;

                    case CompareLe:
                    op_code = local::graph::cexp_le_graph_op;
                    break;

                    case CompareEq:
                    op_code = local::graph::cexp_eq_graph_op;
                    break;

                    case CompareGe:
                    op_code = local::graph::cexp_lt_graph_op;
                    std::swap(if_true, if_false);
                    break;

                    case CompareGt:
                    op_code = local::graph::cexp_le_graph_op;
                    std::swap(if_true, if_false);
                    break;

                    case CompareNe:
                    op_code = local::graph::cexp_eq_graph_op;
                    std::swap(if_true, if_false);
                    break;

                    default:
                    CPPAD_ASSERT_UNKNOWN(false);
                    break;
                }
                // var2node and previous_node for this operator
                var2node[i_var] = ++previous_node;
                //
                op_usage.n_result    = 1;
                op_usage.n_arg       = 4;
                op_usage.start_arg   = operator_arg.size();
                op_usage.op_enum     = op_code;
                operator_vec.push_back( op_usage );
                operator_arg.push_back( left );
                operator_arg.push_back( right );
                operator_arg.push_back( if_true );
                operator_arg.push_back( if_false );
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
    // ----------------------------------------------------------------------
    // output: dependent_vec
    size_t n_dependent = dep_taddr_.size();
    dependent_vec.resize(n_dependent);
    for(size_t i = 0; i < n_dependent; ++i)
        dependent_vec[i] = var2node[ dep_taddr_[i] ];
    //
    return;
}

# endif
