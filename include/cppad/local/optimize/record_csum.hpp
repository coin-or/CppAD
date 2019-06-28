# ifndef CPPAD_LOCAL_OPTIMIZE_RECORD_CSUM_HPP
# define CPPAD_LOCAL_OPTIMIZE_RECORD_CSUM_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize  {

/*!
$begin optimize_record_csum$$
$spell
    iutr
    iterator
    op
    var
    itr
    NumRes
    csum
    Addpv
    Addvv
    Subpv
    Subvp
    Subvv
$$

$section Recording a Cumulative Summation Operator$$

$head Prototype$$
$srcfile%include/cppad/local/optimize/record_csum.hpp%
    0%// BEGIN_RECORD_CSUM%// END_PROROTYPE%1
%$$

$head play$$
player object corresponding to the old recording.

$head random_itr$$
is a random iterator corresponding to the old operation sequence.

$head op_usage$$
mapping from old operator index to how it is used.

$head new_par$$
mapping from old parameter index to parameter index in new recording.

$head new_var$$
mapping from old operator index to variable index in new recording.

$head current$$
is the index in the old operation sequence for
the variable corresponding to the result for the current operator.
We use the notation $icode%i_op% = %random_itr%.var2op(%current%)%$$.
It follows that  NumRes( random_itr.get_op[i_op] ) > 0.
If 0 < j_op < i_op, either op_usage[j_op] == usage_t(csum_usage),
op_usage[j_op] = usage_t(no_usage), or new_var[j_op] != 0.

$head rec$$
is the object that will record the new operations.

$head return$$
is the operator and variable indices in the new operation sequence.

$head stack$$
Is temporary work space. On input and output,
stack.op_info, stack.add_var, and stack.sub_var, are all empty.
These stacks are passed in so that they are created once
and then be reused with calls to $code record_csum$$.

$head Assumptions$$
random_itr.get_op[i_op]
must be one of AddpvOp, AddvvOp, SubpvOp, SubvpOp, SubvvOp.
op_usage[i_op] != usage_t(no_usage) and
! op_usage[i_op] == usage_type(csum_usage).
Furthermore op_usage[j_op] == usage_t(csum_usage) is true from some
j_op that corresponds to a variable that is an argument to
random_itr.get_op[i_op].

$end
*/

// BEGIN_RECORD_CSUM
template <class Addr, class Base>
struct_size_pair record_csum(
    const player<Base>*                                play           ,
    const play::const_random_iterator<Addr>&           random_itr     ,
    const pod_vector<usage_t>&                         op_usage       ,
    const pod_vector<addr_t>&                          new_par        ,
    const pod_vector<addr_t>&                          new_var        ,
    size_t                                             current        ,
    recorder<Base>*                                    rec            ,
    // local information passed so stacks need not be allocated for every call
    struct_csum_stacks&                                stack          )
// END_PROROTYPE
{
# ifndef NDEBUG
    // number of parameters corresponding to the old operation sequence.
    size_t npar = play->num_par_rec();
# endif

    // vector of length npar containing the parameters the old operation
    // sequence; i.e., given a parameter index i < npar, the corresponding
    // parameter value is par[i].
    const Base* par = play->GetPar();

    // which parameters are dynamic
    const pod_vector<bool>& dyn_par_is( play->dyn_par_is() );

    // check assumption about work space
    CPPAD_ASSERT_UNKNOWN( stack.op_info.empty() );
    CPPAD_ASSERT_UNKNOWN( stack.add_var.empty() );
    CPPAD_ASSERT_UNKNOWN( stack.sub_var.empty() );
    //
    // this operator is not csum connected to some other result
    size_t i_op = random_itr.var2op(current);
    CPPAD_ASSERT_UNKNOWN( ! ( op_usage[i_op] == usage_t(csum_usage) ) );
    //
    // information corresponding to the root node in the cummulative summation
    struct struct_csum_op_info info;
    size_t not_used;
    random_itr.op_info(i_op, info.op, info.arg, not_used);
    info.add = true;  // was parrent operator positive or negative
    //
    // initialize stack as containing this one operator
    stack.op_info.push( info );
    //
    // initialize sum of parameter values as zero
    Base sum_par(0);
    //
# ifndef NDEBUG
    // one argument of this operator must have been csum connected to it
    bool ok = false;
    i_op = random_itr.var2op(size_t(info.arg[0]));
    if( (info.op != SubpvOp) & (info.op != AddpvOp) )
        ok |= op_usage[i_op] == usage_t(csum_usage);
    i_op = random_itr.var2op(size_t(info.arg[1]));
    if( info.op != SubvpOp )
        ok |= op_usage[i_op] == usage_t(csum_usage);
    CPPAD_ASSERT_UNKNOWN( ok );
# endif
    //
    // while there are operators left on the stack
    while( ! stack.op_info.empty() )
    {   // get this summation operator
        info = stack.op_info.top();
        stack.op_info.pop();
        OpCode        op      = info.op;
        const addr_t* arg     = info.arg;
        bool          add     = info.add;
        CPPAD_ASSERT_NARG_NRES(op, 2, 1);
        //
        // is this a subtraction operator
        bool subtract = (op==SubpvOp) | (op==SubvpOp) | (op==SubvvOp);
        //
        // is the i-th arguemnt a parameter
        bool par_arg[2];
        switch(op)
        {   case SubpvOp:
            case AddpvOp:
            par_arg[0] = true;
            par_arg[1] = false;
            break;
            //
            case SubvpOp:
            par_arg[0] = false;
            par_arg[1] = true;
            break;
            //
            default:
            par_arg[0] = false;
            par_arg[1] = false;
            break;
        }
        //
        // loop over the arguments to this operator
        for(size_t i = 0; i < 2; ++i)
        {   if( subtract & (i == 1) )
                add = ! add;
            if( par_arg[i] )
            {   // case where i-th argument is a parameter
                CPPAD_ASSERT_UNKNOWN( size_t(arg[i]) < npar );
                //
                if( dyn_par_is[ arg[i] ] )
                {   // i-th argument is a dynamic parameter
                    // (can't yet be a result, so no nodes below)
                    if( add )
                        stack.add_dyn.push(arg[i]);
                    else
                        stack.sub_dyn.push(arg[i]);
                }
                else
                {   // i-th argument is constant parameter
                    if( add )
                        sum_par += par[arg[i]];
                    else
                        sum_par -= par[arg[i]];
                }
            }
            else
            {    // case where i-th argument is a variable
                //
                // check if the i-th argument has csum usage
                i_op = random_itr.var2op(size_t(arg[i]));
                if( op_usage[i_op] == usage_t(csum_usage) )
                {   // there is no result corresponding to i-th argument
                    CPPAD_ASSERT_UNKNOWN( size_t( new_var[i_op]) == 0 );

                    // push the operator corresponding to the i-th argument
                    random_itr.op_info(i_op, info.op, info.arg, not_used);
                    info.add = add;
                    stack.op_info.push( info );
                }
                else
                {   // there are no nodes below this one
                    CPPAD_ASSERT_UNKNOWN( size_t(arg[i]) < current );
                    if( add )
                        stack.add_var.push(arg[i]);
                    else
                        stack.sub_var.push(arg[i]);
                }
            }
        }
    }
    // number of variables to add in this cummulative sum operator
    size_t n_add_var = stack.add_var.size();

    // number of variables to subtract in this cummulative sum operator
    size_t n_sub_var = stack.sub_var.size();

    // number of dynamics to add in this cummulative sum operator
    size_t n_add_dyn = stack.add_dyn.size();

    // number of dynamics to subtract in this cummulative sum operator
    size_t n_sub_dyn = stack.sub_dyn.size();

    // first five arguments to cumulative sum operator
    addr_t new_arg = rec->put_con_par(sum_par);
    rec->PutArg(new_arg);            // arg[0]: initial sum
    size_t end   = n_add_var + 5;
    rec->PutArg( addr_t(end) );      // arg[1]: end for add variables
    end           += n_sub_var;
    rec->PutArg( addr_t(end) );      // arg[2]: end for sub variables
    end           += n_add_dyn;
    rec->PutArg( addr_t(end) );      // arg[3]: end for add dynamics
    end           += n_sub_dyn;
    rec->PutArg( addr_t(end) );      // arg[4]: end for sub dynamics

    // addition variable arguments
    for(size_t i = 0; i < n_add_var; i++)
    {   CPPAD_ASSERT_UNKNOWN( ! stack.add_var.empty() );
        addr_t old_arg = stack.add_var.top();
        new_arg        = new_var[ random_itr.var2op(size_t(old_arg)) ];
        CPPAD_ASSERT_UNKNOWN( 0 < new_arg && size_t(new_arg) < current );
        rec->PutArg(new_arg);         // arg[5+i]
        stack.add_var.pop();
    }

    // subtraction variable arguments
    for(size_t i = 0; i < n_sub_var; i++)
    {   CPPAD_ASSERT_UNKNOWN( ! stack.sub_var.empty() );
        addr_t old_arg = stack.sub_var.top();
        new_arg        = new_var[ random_itr.var2op(size_t(old_arg)) ];
        CPPAD_ASSERT_UNKNOWN( 0 < new_arg && size_t(new_arg) < current );
        rec->PutArg(new_arg);      // arg[arg[1] + i]
        stack.sub_var.pop();
    }

    // addition dynamic arguments
    for(size_t i = 0; i < n_add_dyn; ++i)
    {   addr_t old_arg = stack.add_dyn.top();
        new_arg        = new_par[ old_arg ];
        rec->PutArg(new_arg);      // arg[arg[2] + i]
        stack.add_dyn.pop();
    }

    // subtraction dynamic arguments
    for(size_t i = 0; i < n_sub_dyn; ++i)
    {   addr_t old_arg = stack.sub_dyn.top();
        new_arg        = new_par[ old_arg ];
        rec->PutArg(new_arg);      // arg[arg[3] + i]
        stack.sub_dyn.pop();
    }

    // number of additions plus number of subtractions
    rec->PutArg( addr_t(end) );    // arg[arg[4]] = arg[4]
    //
    // return value
    struct_size_pair ret;
    ret.i_op  = rec->num_op_rec();
    ret.i_var = size_t(rec->PutOp(CSumOp));
    //
    return ret;
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE


# endif
