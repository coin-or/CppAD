# ifndef CPPAD_LOCAL_SUBGRAPH_ARG_VARIABLE_HPP
# define CPPAD_LOCAL_SUBGRAPH_ARG_VARIABLE_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/local/pod_vector.hpp>

// BEGIN_CPPAD_LOCAL_SUBGRAPH_NAMESPACE
namespace CppAD { namespace local { namespace subgraph {
/*!
\file arg_variable.hpp
Determine arguments that are variables.
*/

/*!
Determine the set of arguments, for an operator, that are variables.

\tparam Addr
Type used for indices in random iterator.

\param random_itr
is a random iterator for this operation sequence.

\param i_op
is the operator index. If this operator is part of a user function call,
it must be the first UserOp in the call. (There is a UserOp at the
beginning and end of each call.)

\param variable
is the set of argument variables corresponding to this operator.
If the operator is a UserOp, the arguments are the variables
that are passed into the function call.

\param work
this is work space used by arg_variable to make subsequent calls
faster. It should not be used by the calling routine. In addition,
it is better if work does not drop out of scope between calls.
*/
template <class Addr>
void get_argument_variable(
	const play::const_random_iterator<Addr>& random_itr  ,
	size_t                                   i_op        ,
	pod_vector<size_t>&                      variable    ,
	pod_vector<bool>&                        work        )
{
	// reset to size zero, but keep allocated memory
	variable.resize(0);
	//
	// operator corresponding to i_op
	OpCode        op;
	const addr_t* op_arg;
	size_t        i_var;
	random_itr.op_info(i_op, op, op_arg, i_var);
	//
	// partial check of assumptions on user function calls
	CPPAD_ASSERT_UNKNOWN(
		op != UsrapOp && op != UsravOp && op != UsrrpOp && op != UsrrvOp
	);
	//
	// we assume this is the first UserOp of the call
	if( op == UserOp )
	{	random_itr.op_info(++i_op, op, op_arg, i_var);
		while( op != UserOp )
		{	switch(op)
			{
				case UsravOp:
				{	CPPAD_ASSERT_NARG_NRES(op, 1, 0);
					size_t j_var = op_arg[0];
					variable.push_back(j_var);
				}
				break;

				case UsrrvOp:
				case UsrrpOp:
				case UsrapOp:
				break;

				default:
				// cannot find second UserOp in this call
				CPPAD_ASSERT_UNKNOWN(false);
				break;
			}
			random_itr.op_info(++i_op, op, op_arg, i_var);
		}
		CPPAD_ASSERT_UNKNOWN( variable.size() > 0 );
		return;
	}
	// is_variable is a reference to work with a better name
	pod_vector<bool>& is_variable(work);
	arg_is_variable(op, op_arg, is_variable);
	size_t num_arg = is_variable.size();
	for(size_t j = 0; j < num_arg; ++j)
	{	if( is_variable[j] )
		{	size_t j_var = op_arg[j];
			variable.push_back(j_var);
		}
	}
	return;
}

} } } // END_CPPAD_LOCAL_SUBGRAPH_NAMESPACE

# endif
