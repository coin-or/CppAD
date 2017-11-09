# ifndef CPPAD_LOCAL_REV_JAC_SUBGRAPH_HPP
# define CPPAD_LOCAL_REV_JAC_SUBGRAPH_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*!
\file rev_jac_subgraph.hpp
Compute dependency sparsity pattern using subgraph technique.
*/

/*!
Determine the set of argument variables for an operator

\param play
is the player for this operation sequence.

\param i_op
is the operator index. It this operator is in a user function call,
it must be the first UserOp in the call
(there are two UserOp in each such call).

\param variable
is the set of argument variables corresponding to this operator.

\param work
this is work space used by get_argument_variable to make subsequent calls
faster. It should not be used by the calling routine. In addition,
it is better for it to not drop out of scope between calls to
get_argument_variable.
*/
template <typename Base>
void get_argument_variable(
	const player<Base>*  play        ,
	size_t               i_op        ,
	pod_vector<size_t>&  variable    ,
	pod_vector<bool>&    work        )
{
	// reset to size zero, but keep allocated memory
	variable.erase();
	//
	// operator corresponding to i_op
	OpCode        op;
	const addr_t* op_arg;
	size_t        i_var;
	play->get_op_info(i_op, op, op_arg, i_var);
	CPPAD_ASSERT_UNKNOWN(
		op != UsrapOp && op != UsravOp && op != UsrrpOp && op != UsrrvOp
	);
	if( op == UserOp )
	{	play->get_op_info(++i_op, op, op_arg, i_var);
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
				CPPAD_ASSERT_UNKNOWN(false);
				break;
			}
			play->get_op_info(++i_op, op, op_arg, i_var);
		}
		CPPAD_ASSERT_UNKNOWN( variable.size() > 0 );
		return;
	}
	// create a reference to work with a better name
	pod_vector<bool>& is_variable(work);
	size_t num_arg = arg_is_variable(op, op_arg, is_variable);
	for(size_t j = 0; j < num_arg; ++j)
	{	if( is_variable[j] )
			variable.push_back(op_arg[j]);
	}
	return;
}

/*!
Compute dependency sparsity pattern for an ADFun<Base> function.

\tparam Base
the operation sequence was recorded using AD<Base>.

\param play
is the operation sequence corresponding to the ADFun<Base> function.

\param ind_taddr
is the vector of independent variables for this ADFun<Base> function.

\param dep_taddr
is the vector of dependent variables for this ADFun<Base> function.

\param row_out
The input size and elements of row_out do not matter.
We use number of non-zeros (nnz) to denote the number of elements
in row_out. For k = 0 , ... , nnz-1, row_out[k] is the row index
of the k-th no-zero element of the dependency sparsitiy pattern for
the function corresponding to the recording.
\code
	0 <= row_out[k] < dep_taddr.size()
\endcode

\param col_out
The input size and elements of col_out do not matter.
Upon return is has the same size as row_out; i.e., nnz.
For k = 0 , ... , nnz-1, col_out[k] is the column index
of the k-th no-zero element of the dependency sparsitiy pattern for
the function corresponding to the recording.
\code
	0 <= col_out[k] < ind_taddr.size()
\endcode
*/

template <typename Base>
void rev_jac_subgraph(
	const player<Base>*        play         ,
	const vector<size_t>&      ind_taddr    ,
	const vector<size_t>&      dep_taddr    ,
	pod_vector<size_t>&        row_out      ,
	pod_vector<size_t>&        col_out      )
{
	// numver of dependent variables
	size_t n_dep = dep_taddr.size();

	// numbef of operators in the tape
	size_t num_op  = play->num_op_rec();

	// subgraph of variables that are not independent and are connected
	// to the dependent variable
	pod_vector<size_t> subgraph;

	// start with an empty sparsity pattern
	row_out.erase();
	col_out.erase();

	// variables that are arguments to a particular user function call
	pod_vector<size_t> argument_variable;

	// if sub_or_connected[i_op] == i_dep, one of the following holds:
	// 1. if i_op is an independent variable operator connected to i_dep
	// 2. otherwise it is in subgraph for dependent variable i_dep
	pod_vector<size_t> sub_or_connected(num_op);
	//
	// initilaize sub_or_connected to an impossible dependent variable index
	for(size_t i_op = 0; i_op < num_op; ++i_op)
		sub_or_connected[i_op] = n_dep;

	// work space used by get_argument_variable
	pod_vector<bool> work;

	// for each dependent variable
	for(size_t i_dep = 0; i_dep < n_dep; ++i_dep)
	{
		// start with an empty subgraph for this dependent variable
		subgraph.erase();

		// tape index corresponding to this dependent variable
		size_t i_var = dep_taddr[i_dep];

		// put this node in the subgraph for this dependent variable
		size_t i_op = play->var2op(i_var);
		if( play->GetOp(i_op) == InvOp )
		{	// this dependent variable is also an independent variable
			size_t i_ind = i_var - 1;
			CPPAD_ASSERT_UNKNOWN( i_var == ind_taddr[i_ind] );
			row_out.push_back(i_dep);
			col_out.push_back(i_ind);
		}
		else
		{	subgraph.push_back(i_op);
		}

		// check all that all the variables in the subgraph have been scanned
		size_t sub_index = 0;
		while(sub_index < subgraph.size() )
		{	// scan this operator to see which variables are connected to it
			i_op      = subgraph[sub_index];
			OpCode op = play->GetOp(i_op);
			//
			// There must be a result for this operator
			CPPAD_ASSERT_UNKNOWN( NumRes(op) > 0 );
			//
			// special case where result is part of a user function call,
			// find first UserOp for this function call.
			if( op == UsrrvOp )
			{	// 2DO: make the resulting sparsity_out more efficient using
				// sparsity pattern for the user function call.
				//
				// determine the UserOp that started this function call
				// along with the first variable result for this call.
				while( op != UserOp )
				{	CPPAD_ASSERT_UNKNOWN(
						op == UsrapOp ||
						op == UsravOp ||
						op == UsrrpOp ||
						op == UsrrvOp
					);
					CPPAD_ASSERT_UNKNOWN( i_op > 0 );
					op = play->GetOp(--i_op);
				}
			}
			if( sub_or_connected[i_op] != i_dep )
			{	// Mark as processed so we do not repeat this calculation.
				sub_or_connected[i_op] = i_dep;
				//
				// determine which variables are connected to this call
				get_argument_variable(play, i_op, argument_variable, work);
				//
				// check each of these variables
				for(size_t j = 0; j < argument_variable.size(); ++j)
				{	size_t j_var = argument_variable[j];
					size_t j_op  = play->var2op(j_var);
					//
					// has this argument already been processed
					// for this dependent variable
					if( sub_or_connected[j_op] != i_dep )
					{	// variable not yet in subgraph or connected
						//
						if( play->GetOp(j_op) == InvOp )
						{	// This is an independent variable
							size_t j_ind = j_var - 1;
							CPPAD_ASSERT_UNKNOWN( j_var == ind_taddr[j_ind] );
							row_out.push_back(i_dep);
							col_out.push_back(j_ind);
						}
						else
						{	// add to the subgraph
							subgraph.push_back(j_op);
						}
						sub_or_connected[j_op] = i_dep;
					}
				}
			}
			++sub_index;
		}
	}
}

} } // END_CPPAD_LOCAL_NAMESPACE

# endif
