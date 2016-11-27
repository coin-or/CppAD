// $Id$
# ifndef CPPAD_LOCAL_OPTIMIZE_RECORD_CSUM_HPP
# define CPPAD_LOCAL_OPTIMIZE_RECORD_CSUM_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

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

<!-- replace prototype -->
\param tape
is a vector that maps a variable index, in the old operation sequence,
to the corresponding struct_old_variable information.
Note that the old variable index must be greater than or equal zero and
less than tape.size().

\param current
is the index in the old operation sequence for
the variable corresponding to the result for the current operator.
It follows that current < tape.size() and NumRes( tape[current].op ) > 0.
Suppose i <= current, j < NumArg( tape[i] ), and k = tape[i].arg[j],
It is assumed that tape[i].arg[j] is connected to the dependent variables
and tape[k].new_var has been set to the corresponding variable.
Note that tape[i].arg[j] < i <= current and
tape[k].new_var <= k < current.

\param npar
is the number of parameters corresponding to the old operation sequence.

\param par
is a vector of length npar containing the parameters
the old operation sequence; i.e.,
given a parameter index i < npar, the corresponding parameter value is par[i].
<!-- end prototype -->

\param rec
is the object that will record the operations.

\param work
Is temporary work space. On input and output,
work.op_stack, work.add_stack, and work.sub_stack, are all empty.
These stacks are passed in so that they are created once
and then be reused with calls to record_csum.

\param var2op
mapping from old variable index to old operator index
(only used for error checking)

\param op_info
mapping from old index to operator information
(only used for error checking)

\par Assumptions
tape[i].new_var is not yet defined for any node i that is csum_connected
to the current node; i.e., tape[i].new_var = tape.size() for all such nodes.
For example; suppose j is an argument in the old operation sequence for the
current operator, i = tape[current].arg[j], and
tape[arg[j]].connect_type == csum_connected. It follows that
tape[i].new_var == tape.size().

\par Restriction:
tape[current].op
must be one of AddpvOp, AddvvOp, SubpvOp, SubvpOp, SubvvOp.
tape[current].connect_type must be yes_connected.
tape[j].connect_type == csum_connected for some index
j that is a variable argument for the current operation.
*/

template <class Base>
struct_size_pair record_csum(
	const CppAD::vector<struct struct_old_variable>&   tape           ,
	size_t                                             current        ,
	size_t                                             npar           ,
	const Base*                                        par            ,
	recorder<Base>*                                    rec            ,
	// local information passed so stacks need not be allocated for every call
	struct_csum_stacks&                                work           ,
	const vector<size_t>&                              var2op         ,
	const vector<struct_op_info>&                      op_info        )
{
	// check assumption about work space
	CPPAD_ASSERT_UNKNOWN( work.op_stack.empty() );
	CPPAD_ASSERT_UNKNOWN( work.add_stack.empty() );
	CPPAD_ASSERT_UNKNOWN( work.sub_stack.empty() );
	//
	CPPAD_ASSERT_UNKNOWN( tape[current].connect_type == yes_connected );
# ifndef NDEBUG
	{	size_t i_op = var2op[current];
		CPPAD_ASSERT_UNKNOWN( ! op_info[i_op].csum_connected );
	}
# endif

	size_t                        i;
	OpCode                        op;
	const addr_t*                 arg;
	bool                          add;
	struct struct_csum_variable var;
	//
	// information corresponding to the root node in the cummulative summation
	var.op  = tape[current].op;   // this operator
	var.arg = tape[current].arg;  // arguments for this operator
	var.add = true;               // was parrent operator positive or negative
	//
	// initialize stack as containing this one operator
	work.op_stack.push( var );
	//
	// initialize sum of parameter values as zero
	Base sum_par(0);
	//
# ifndef NDEBUG
	bool ok = false;
	if( var.op == SubvpOp )
		ok = tape[ tape[current].arg[0] ].connect_type == csum_connected;
	if( var.op == AddpvOp || var.op == SubpvOp )
		ok = tape[ tape[current].arg[1] ].connect_type == csum_connected;
	if( var.op == AddvvOp || var.op == SubvvOp )
	{	ok  = tape[ tape[current].arg[0] ].connect_type == csum_connected;
		ok |= tape[ tape[current].arg[1] ].connect_type == csum_connected;
	}
	CPPAD_ASSERT_UNKNOWN( ok );
# endif
	//
	// while there are operators left on the stack
	while( ! work.op_stack.empty() )
	{	// get this summation operator
		var     = work.op_stack.top();
		work.op_stack.pop();
		op      = var.op;
		arg     = var.arg;
		add     = var.add;
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
			// check if the first argument is csum_connected
# ifndef NDEBUG
			{	bool flag1 = tape[arg[0]].connect_type == csum_connected;
				bool flag2 = op_info[ var2op[ arg[0] ] ].csum_connected;
				CPPAD_ASSERT_UNKNOWN( flag1 == flag2 );
			}
# endif
			if( tape[arg[0]].connect_type == csum_connected )
			{	CPPAD_ASSERT_UNKNOWN(
					size_t(tape[arg[0]].new_var) == tape.size()
				);
				// push the operator corresponding to the first argument
				var.op  = tape[arg[0]].op;
				var.arg = tape[arg[0]].arg;
				// first argument has same sign as parent node
				var.add = add;
				work.op_stack.push( var );
			}
			else
			{	// there are no nodes below this one
				CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < current );
				CPPAD_ASSERT_UNKNOWN( tape[arg[0]].new_var <= arg[0] );
				if( add )
					work.add_stack.push(arg[0]);
				else	work.sub_stack.push(arg[0]);
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
			// check if the second argument is csum_connected
# ifndef NDEBUG
			{	bool flag1 = tape[arg[1]].connect_type == csum_connected;
				bool flag2 = op_info[ var2op[ arg[1] ] ].csum_connected;
				CPPAD_ASSERT_UNKNOWN( flag1 == flag2 );
			}
# endif
			if( tape[arg[1]].connect_type == csum_connected )
			{	CPPAD_ASSERT_UNKNOWN(
					size_t(tape[arg[1]].new_var) == tape.size()
				);
				// push the operator corresoponding to the second arugment
				var.op   = tape[arg[1]].op;
				var.arg  = tape[arg[1]].arg;
				var.add  = add;
				work.op_stack.push( var );
			}
			else
			{	// there are no nodes below this one
				CPPAD_ASSERT_UNKNOWN( size_t(arg[1]) < current );
				CPPAD_ASSERT_UNKNOWN( tape[arg[1]].new_var <=  arg[1] );
				if( add )
					work.add_stack.push(arg[1]);
				else	work.sub_stack.push(arg[1]);
			}
			break;

			default:
			CPPAD_ASSERT_UNKNOWN(false);
		}
	}
	// number of variables to add in this cummulative sum operator
	size_t n_add = work.add_stack.size();
	// number of variables to subtract in this cummulative sum operator
	size_t n_sub = work.sub_stack.size();
	//
	rec->PutArg(n_add);                // arg[0]
	rec->PutArg(n_sub);                // arg[1]
	size_t new_arg = rec->PutPar(sum_par);
	rec->PutArg(new_arg);              // arg[2]
	// addition arguments
	for(i = 0; i < n_add; i++)
	{	CPPAD_ASSERT_UNKNOWN( ! work.add_stack.empty() );
		size_t old_arg = work.add_stack.top();
		new_arg        = tape[old_arg].new_var;
		CPPAD_ASSERT_UNKNOWN( new_arg < current );
		rec->PutArg(new_arg);         // arg[3+i]
		work.add_stack.pop();
	}
	// subtraction arguments
	for(i = 0; i < n_sub; i++)
	{	CPPAD_ASSERT_UNKNOWN( ! work.sub_stack.empty() );
		size_t old_arg = work.sub_stack.top();
		new_arg        = tape[old_arg].new_var;
		CPPAD_ASSERT_UNKNOWN( new_arg < current );
		rec->PutArg(new_arg);      // arg[3 + arg[0] + i]
		work.sub_stack.pop();
	}
	// number of additions plus number of subtractions
	rec->PutArg(n_add + n_sub);        // arg[3 + arg[0] + arg[1]]
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
