# ifndef CPPAD_LOCAL_OPTIMIZE_GET_PAR_USAGE_HPP
# define CPPAD_LOCAL_OPTIMIZE_GET_PAR_USAGE_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*!
\file get_cexp_info.hpp
Create operator information tables
*/
# include <cppad/local/optimize/get_op_usage.hpp>

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize {

/*!
Use reverse activity analysis to get usage for each parameters.

\tparam Base
Base type for the operator; i.e., this operation was recorded
using AD<Base> and computations by this routine are done using type Base.

\tparam Addr
Type used by random iterator for the player.

\param play
This is the operation sequence.

\param random_itr
This is a random iterator for the operation sequence.

\param op_usage
This argument has size equal to the number of operators
in the operation sequence; i.e., num_op = play->nun_var_rec().
The value op_usage[i] have been set to the usage for
the i-th operator in the operation sequence.

\param vecad_used
This argument has size equal to the number of VecAD vectors
in the operations sequences; i.e., play->num_vecad_vec_rec().
The VecAD vectors are indexed in the order that thier indices apprear
in the one large play->GetVecInd that holds all the VecAD vectors.

\param par_usage
Upon return it has size equal to the number of parameters
in the operation sequence; i.e., play->num_par_rec();
The value par_usage[i] is true if an only if
the i-th parameter is used.
(Independent dynamic parameters are always used.)
*/

template <class Addr, class Base>
void get_par_usage(
    const player<Base>*                         play                ,
    const play::const_random_iterator<Addr>&    random_itr          ,
    const pod_vector<usage_t>&                  op_usage            ,
    pod_vector<bool>&                           vecad_used          ,
    pod_vector<bool>&                           par_usage           )
{
    CPPAD_ASSERT_UNKNOWN( op_usage.size()   == play->num_op_rec() );
    CPPAD_ASSERT_UNKNOWN( par_usage.size()  == 0 );
    //
    // number of operators in the tape
    const size_t num_op = play->num_op_rec();
    //
    // number of parameters in the tape
    const size_t num_par = play->num_par_rec();
    //
    // number of dynamic parameters
    const size_t num_dynamic_par = play->num_dynamic_par();
    //
    // number of independent dynamic parameters
    size_t num_dynamic_ind = play->num_dynamic_ind();
    //
    // number of VecAD vectors
    size_t num_vecad_vec = play->num_vecad_vec_rec();
    //
    // dynamic parameter information
    const pod_vector<bool>&      dyn_par_is( play->dyn_par_is() );
    const pod_vector<opcode_t>&  dyn_par_op( play->dyn_par_op() );
    const pod_vector<addr_t>&    dyn_par_arg( play->dyn_par_arg() );
    const pod_vector<addr_t>&    dyn_ind2par_ind( play->dyn_ind2par_ind() );
    // -----------------------------------------------------------------------
    // initialize par_usage
    par_usage.resize(num_par);
    for(size_t i_par = 0; i_par < num_dynamic_ind; ++i_par)
        par_usage[i_par] = true;  // true for independent dynamic parameters
    for(size_t i_par = num_dynamic_ind; i_par < num_par; ++i_par)
        par_usage[i_par] = false; // initialize as false for other parameters
    //
    // -----------------------------------------------------------------------
    // set usage to true for VecAD parameters that get used
    size_t start_this_vector = 0;
    for(size_t i_vec = 0; i_vec < num_vecad_vec; ++i_vec)
    {   // length of this vector (note length is not a parameter)
        size_t length = play->GetVecInd(start_this_vector);
        //
        if( vecad_used[i_vec] )
        {   // this vector gets used
            for(size_t k = 1; k <= length; ++k)
            {   // index of parameter used by this VecAD vector
                size_t i_par = play->GetVecInd(start_this_vector + k);
                // must not be a dynamic parameter
                CPPAD_ASSERT_UNKNOWN( ! dyn_par_is[i_par] );
                // set usage for this parameter
                par_usage[i_par] = true;
            }
        }
        start_this_vector += length + 1;
    }
    CPPAD_ASSERT_UNKNOWN( start_this_vector == play->num_vec_ind_rec() );
    //
    // -----------------------------------------------------------------------
    // forward pass to mark which parameters are used by necessary operators
    for(size_t i_op = 0; i_op < num_op; ++i_op)
    {
        // information about current operator
        OpCode        op;     // operator
        const addr_t* arg;    // arguments
        size_t        i_var;  // variable index of first result
        random_itr.op_info(i_op, op, arg, i_var);
        //
        if( op_usage[i_op] != no_usage ) switch( op )
        {
            // add or subtract with left a parameter and right a variable
            case AddpvOp:
            case SubpvOp:
            if( dyn_par_is[ arg[0] ] )
                par_usage[ arg[0] ] = true;
            else
            {   // determine if this parameter will be absorbed by csum
                if( ! (op_usage[i_op] == csum_usage) )
                {   // determine operator corresponding to variable
                    size_t j_op = random_itr.var2op(size_t(arg[1]));
                    CPPAD_ASSERT_UNKNOWN( op_usage[j_op] != no_usage );
                    if( op_usage[j_op] != csum_usage )
                        par_usage[ arg[0] ] = true;
                }
            }
            break;

            // subtract with left a variable and right a parameter
            case SubvpOp:
            if( dyn_par_is[ arg[1] ] )
                par_usage[ arg[1] ] = true;
            else
            {   // determine if this parameter will be absorbed by csum
                if( ! (op_usage[i_op] == csum_usage) )
                {   // determine operator corresponding to variable
                    size_t j_op = random_itr.var2op(size_t(arg[0]));
                    CPPAD_ASSERT_UNKNOWN( op_usage[j_op] != no_usage );
                    if( op_usage[j_op] != csum_usage )
                        par_usage[ arg[1] ] = true;
                }
            }
            break;



            // cases with no parameter arguments
            case AbsOp:
            case AcosOp:
            case AcoshOp:
            case AddvvOp:
            case AsinOp:
            case AsinhOp:
            case AtanOp:
            case AtanhOp:
            case BeginOp:
            case CosOp:
            case CoshOp:
            case CSkipOp:
            case DisOp:
            case DivvvOp:
            case EndOp:
            case EqvvOp:
            case ExpOp:
            case Expm1Op:
            case InvOp:
            case LdpOp:
            case LdvOp:
            case LevvOp:
            case LogOp:
            case Log1pOp:
            case LtvvOp:
            case MulvvOp:
            case NevvOp:
            case PowvvOp:
            case SignOp:
            case SinOp:
            case SinhOp:
            case SqrtOp:
            case StpvOp:
            case StvvOp:
            case SubvvOp:
            case TanOp:
            case TanhOp:
            case AFunOp:
            case FunavOp:
            case FunrvOp:
            case ZmulvvOp:
            break;

            // cases where first and second arguments are parameters
            case EqppOp:
            case LeppOp:
            case LtppOp:
            case NeppOp:
            CPPAD_ASSERT_UNKNOWN( 2 <= NumArg(op) )
            par_usage[arg[0]] = true;
            par_usage[arg[1]] = true;
            break;


            // cases where only first argument is a parameter
            case CSumOp:
            case EqpvOp:
            case DivpvOp:
            case LepvOp:
            case LtpvOp:
            case MulpvOp:
            case NepvOp:
            case ParOp:
            case PowpvOp:
            case FunapOp:
            case FunrpOp:
            case ZmulpvOp:
            CPPAD_ASSERT_UNKNOWN( 1 <= NumArg(op) )
            par_usage[arg[0]] = true;
            break;

            // cases where only second argument is a parameter
            case DivvpOp:
            case LevpOp:
            case LtvpOp:
            case PowvpOp:
            case ZmulvpOp:
            CPPAD_ASSERT_UNKNOWN( 2 <= NumArg(op) )
            par_usage[arg[1]] = true;
            break;

            // cases where only third argument is a parameter
            case StppOp:
            case StvpOp:
            par_usage[arg[2]] = true;
            break;

            // conditional expression operator
            case CExpOp:
            CPPAD_ASSERT_UNKNOWN( 6 == NumArg(op) )
            if( (arg[1] & 1) == 0 )
                par_usage[arg[2]] = true;
            if( (arg[1] & 2) == 0 )
                par_usage[arg[3]] = true;
            if( (arg[1] & 4) == 0 )
                par_usage[arg[4]] = true;
            if( (arg[1] & 8) == 0 )
                par_usage[arg[5]] = true;
            break;

            // erf function is special
            case ErfOp:
            CPPAD_ASSERT_UNKNOWN( 3 == NumArg(op) )
            par_usage[arg[1]] = true;
            par_usage[arg[2]] = true;
            break;

            // print function
            case PriOp:
            if( (arg[0] & 1) == 0 )
                par_usage[arg[1]] = true;
            if( (arg[0] & 2) == 0 )
                par_usage[arg[3]] = true;
            CPPAD_ASSERT_UNKNOWN( 5 == NumArg(op) )
            break;

            default:
            CPPAD_ASSERT_UNKNOWN(false);
        }
    }
    // -----------------------------------------------------------------------
    // reverse pass to determine which dynamic parameters are necessary
    size_t i_arg = dyn_par_arg.size(); // index in dyn_par_arg
    size_t i_dyn = num_dynamic_par;    // index in dyn_ind2par_ind
    while(i_dyn)
    {   // next dynamic parameter in reverse order
        --i_dyn;
        //
        // corresponding parameter index
        size_t i_par = size_t( dyn_ind2par_ind[i_dyn] );
        CPPAD_ASSERT_UNKNOWN( dyn_par_is[i_par] );
        //
        // next dynamic parameter in reverse order
        op_code_dyn op = op_code_dyn( dyn_par_op[i_dyn] );
        //
        // number of argumens to this operator
        size_t n_arg = num_arg_dyn(op);
        //
        // index of first argument for this operator
        i_arg -= n_arg;
        //
        // if this dynamic parameter is needed
        if( par_usage[i_par] )
        {   // neeed dynamic parameters that are used to generate this one
            if( op == cond_exp_dyn )
            {   // special case
                CPPAD_ASSERT_UNKNOWN( n_arg == 5 );
                for(size_t i = 1; i < 5; ++i)
                    par_usage[ dyn_par_arg[i_arg + i] ] = true;
            }
            else
            {   for(size_t i = 0; i < n_arg; ++i)
                par_usage[ dyn_par_arg[i_arg + i] ] = true;
            }
        }
    }
    CPPAD_ASSERT_UNKNOWN( i_arg == 0 );
    //
    return;
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
