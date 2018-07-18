# ifndef CPPAD_LOCAL_OPTIMIZE_RECORD_CSUM_HPP
# define CPPAD_LOCAL_OPTIMIZE_RECORD_CSUM_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*!
\file record_csum.hpp
Recording a cummulative cummulative summation.
*/

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize  {
/*!
Recording a cummulative cummulative summation.

\param play
player object corresponding to the old recroding.

\param random_itr
is a random iterator corresponding to the old operation sequence.

\param op_usage
mapping from old index to how it is used.

\param new_var
mapping from old operator index to variable index in new recording.

\param current
is the index in the old operation sequence for
the variable corresponding to the result for the current operator.
We use the notation i_op = random_itr.var2op(current).
It follows that  NumRes( random_itr.get_op[i_op] ) > 0.
If 0 < j_op < i_op, either op_usage[j_op] == usage_t(csum_usage),
op_usage[j_op] = usage_t(no_usage), or new_var[j_op] != 0.

\param rec
is the object that will record the new operations.

\return
is the operator and variable indices in the new operation sequence.

\param stack
Is temporary work space. On input and output,
stack.op_info, stack.add_var, and stack.sub_var, are all empty.
These stacks are passed in so that they are created once
and then be reused with calls to record_csum.

\par Assumptions
random_itr.get_op[i_op]
must be one of AddpvOp, AddvvOp, SubpvOp, SubvpOp, SubvvOp.
op_usage[i_op] != usage_t(no_usage) and ! op_usage[i_op] == usage_type(csum_usage).
Furthermore op_usage[j_op] == usage_t(csum_usage) is true from some
j_op that corresponds to a variable that is an argument to
random_itr.get_op[i_op].
*/

template <class Addr, class Base>
struct_size_pair record_csum(
	const player<Base>*                                play           ,
	const play::const_random_iterator<Addr>&           random_itr     ,
	const pod_vector<usage_t>&                         op_usage       ,
	const pod_vector<addr_t>&                          new_var        ,
	size_t                                             current        ,
	recorder<Base>*                                    rec            ,
	// local information passed so stacks need not be allocated for every call
	struct_csum_stacks&                                stack          )
{
# ifndef NDEBUG
	// number of parameters corresponding to the old operation sequence.
	size_t npar = play->num_par_rec();
# endif

	// vector of length npar containing the parameters the old operation
	// sequence; i.e., given a parameter index i < npar, the corresponding
	// parameter value is par[i].
	const Base* par = play->GetPar();

	// check assumption about work space
	CPPAD_ASSERT_UNKNOWN( stack.op_info.empty() );
	CPPAD_ASSERT_UNKNOWN( stack.add_var.empty() );
	CPPAD_ASSERT_UNKNOWN( stack.sub_var.empty() );
	//
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
	bool ok = false;
	if( info.op == SubvpOp ) ok =
		op_usage[ random_itr.var2op(info.arg[0]) ] == usage_t(csum_usage);
	if( info.op == AddpvOp || info.op == SubpvOp ) ok =
		op_usage[ random_itr.var2op(info.arg[1]) ] == usage_t(csum_usage);
	if( info.op == AddvvOp || info.op == SubvvOp )
	{	ok  =
		op_usage[ random_itr.var2op(info.arg[0]) ] == usage_t(csum_usage);
		ok |=
		op_usage[ random_itr.var2op(info.arg[1]) ] == usage_t(csum_usage);
	}
	CPPAD_ASSERT_UNKNOWN( ok );
# endif
	//
	// while there are operators left on the stack
	while( ! stack.op_info.empty() )
	{	// get this summation operator
		info = stack.op_info.top();
		stack.op_info.pop();
		OpCode        op      = info.op;
		const addr_t* arg     = info.arg;
		bool          add     = info.add;
		//
		// process first argument to this operator
		switch(op)
		{	// cases where first argument is a parameter
			case AddpvOp:
			case SubpvOp:
			CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < npar );
			// first argument has same sign as parent node
			if( add )
				sum_par += par[arg[0]];
			else	sum_par -= par[arg[0]];
			break;

			// cases where first argument is a variable
			case AddvvOp:
			case SubvpOp:
			case SubvvOp:
			//
			// check if the first argument has csum usage
			if( op_usage[random_itr.var2op(arg[0])] == usage_t(csum_usage) )
			{	CPPAD_ASSERT_UNKNOWN(
				size_t( new_var[ random_itr.var2op(arg[0]) ]) == 0
				);
				// push the operator corresponding to the first argument
				size_t i_op_tmp = random_itr.var2op(arg[0]);
				random_itr.op_info(i_op_tmp, info.op, info.arg, not_used);
				// first argument has same sign as parent node
				info.add = add;
				stack.op_info.push( info );
			}
			else
			{	// there are no nodes below this one
				CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < current );
				if( add )
					stack.add_var.push(arg[0]);
				else	stack.sub_var.push(arg[0]);
			}
			break;

			default:
			CPPAD_ASSERT_UNKNOWN(false);
		}
		// process second argument to this operator
		switch(op)
		{	// cases where second argument is a parameter
			case SubvpOp:
			CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < npar );
			// second argument has opposite sign of parent node
			if( add )
				sum_par -= par[arg[1]];
			else	sum_par += par[arg[1]];
			break;

			// cases where second argument is a variable and has opposite sign
			case SubvvOp:
			case SubpvOp:
			add = ! add;

			// cases where second argument is a variable and has same sign
			case AddvvOp:
			case AddpvOp:
			// check if the second argument has csum usage
			if( op_usage[random_itr.var2op(arg[1])] == usage_t(csum_usage) )
			{	CPPAD_ASSERT_UNKNOWN(
				size_t( new_var[ random_itr.var2op(arg[1]) ]) == 0
				);
				// push the operator corresoponding to the second arugment
				size_t i_op_tmp = random_itr.var2op(arg[1]);
				random_itr.op_info(i_op_tmp, info.op, info.arg, not_used);
				info.add  = add;
				stack.op_info.push( info );
			}
			else
			{	// there are no nodes below this one
				CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < current );
				if( add )
					stack.add_var.push(arg[1]);
				else	stack.sub_var.push(arg[1]);
			}
			break;

			default:
			CPPAD_ASSERT_UNKNOWN(false);
		}
	}
	// number of variables to add in this cummulative sum operator
	size_t n_add = stack.add_var.size();
	// number of variables to subtract in this cummulative sum operator
	size_t n_sub = stack.sub_var.size();
	//
	CPPAD_ASSERT_UNKNOWN(
		size_t( std::numeric_limits<addr_t>::max() ) >= n_add + n_sub
	);
	//
	addr_t new_arg = rec->PutPar(sum_par);
	rec->PutArg(new_arg);            // arg[0]
	size_t end   = n_add + 5;
	rec->PutArg( addr_t(end) );      // arg[1]
	end           += n_sub;
	rec->PutArg( addr_t(end) );      // arg[2]
	rec->PutArg( addr_t(end) );      // arg[3]
	rec->PutArg( addr_t(end) );      // arg[4]
	// addition arguments
	for(size_t i = 0; i < n_add; i++)
	{	CPPAD_ASSERT_UNKNOWN( ! stack.add_var.empty() );
		size_t old_arg = stack.add_var.top();
		new_arg        = new_var[ random_itr.var2op(old_arg) ];
		CPPAD_ASSERT_UNKNOWN( 0 < new_arg && size_t(new_arg) < current );
		rec->PutArg(new_arg);         // arg[5+i]
		stack.add_var.pop();
	}
	// subtraction arguments
	for(size_t i = 0; i < n_sub; i++)
	{	CPPAD_ASSERT_UNKNOWN( ! stack.sub_var.empty() );
		size_t old_arg = stack.sub_var.top();
		new_arg        = new_var[ random_itr.var2op(old_arg) ];
		CPPAD_ASSERT_UNKNOWN( 0 < new_arg && size_t(new_arg) < current );
		rec->PutArg(new_arg);      // arg[5 + n_add + i]
		stack.sub_var.pop();
	}
	// number of additions plus number of subtractions
	rec->PutArg( addr_t(end) );    // arg[end] = end
	//
	// return value
	struct_size_pair ret;
	ret.i_op  = rec->num_op_rec();
	ret.i_var = rec->PutOp(CSumOp);
	//
	return ret;
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE


# endif
