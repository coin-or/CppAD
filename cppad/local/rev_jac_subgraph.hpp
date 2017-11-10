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
Note that identical arguments only appear once; e.g., x * x only
has one operator corresponding to x (assuming x is a variable).

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
		{	size_t j_var = op_arg[j];
			variable.push_back(j_var);
		}
	}
	return;
}

/*!
Compute dependency sparsity pattern for an ADFun<Base> function.

\tparam Base
the operation sequence was recorded using AD<Base>.

\tparam BoolVector
a simple vector class with elements of type bool.

\param play
is the operation sequence corresponding to the ADFun<Base> function.

\param ind_taddr
mapping from user independent variable index to variable index in play.

\param dep_taddr
mapping from user pendent variable index to variable index in play.

\param select_domain
only the selected independent variables will be included in the sparsity
pattern (must have same size as ind_taddr).

\param select_range
only the selected dependent variables will be included in the sparsity pattern
(must have same size as dep_taddr).

\param row_out
The input size and elements of row_out do not matter.
We use number of non-zeros (nnz) to denote the number of elements
in row_out. For k = 0 , ... , nnz-1, row_out[k] is the row index
of the k-th no-zero element of the dependency sparsitiy pattern for
the function corresponding to the recording.
\code
	0 <= row_out[k] < dep_taddr.size()
	select_range[ row_out[k] ] == true
\endcode

\param col_out
The input size and elements of col_out do not matter.
Upon return is has the same size as row_out; i.e., nnz.
For k = 0 , ... , nnz-1, col_out[k] is the column index
of the k-th no-zero element of the dependency sparsitiy pattern for
the function corresponding to the recording.
\code
	0 <= col_out[k] < ind_taddr.size()
	select_domain[ col_out[k] ] == true
\endcode
*/

template <typename Base, typename BoolVector>
void rev_jac_subgraph(
	const player<Base>*        play          ,
	const vector<size_t>&      ind_taddr     ,
	const vector<size_t>&      dep_taddr     ,
	const BoolVector&          select_domain ,
	const BoolVector&          select_range  ,
	pod_vector<size_t>&        row_out       ,
	pod_vector<size_t>&        col_out       )
{
	// number of independent variables
	size_t n_ind = ind_taddr.size();
	CPPAD_ASSERT_UNKNOWN( size_t( select_domain.size() ) == n_ind );

	// number of dependent variables
	size_t n_dep = dep_taddr.size();
	CPPAD_ASSERT_UNKNOWN( size_t( select_range.size() ) == n_dep );

	// number of operators in the tape
	size_t num_op  = play->num_op_rec();

	// subgraph of variables that are not independent and are connected
	// to the dependent variable
	pod_vector<size_t> subgraph;

	// start with an empty sparsity pattern
	row_out.erase();
	col_out.erase();

	// variables that are arguments to a particular user function call
	pod_vector<size_t> argument_variable;

	// work space used by get_argument_variable
	pod_vector<bool> work;
	//
	// Map user function call operators to the UserOp at the beginning
	// at the beginning of the corresponding function call
	// Other operators are left as is.
	pod_vector<size_t> map_user_op(num_op);
	for(size_t i_op = 0; i_op < num_op; ++i_op)
	{	map_user_op[i_op] = i_op;
		OpCode op = play->GetOp(i_op);
		if( op == UserOp )
		{	size_t begin = i_op;
			op           = play->GetOp(++i_op);
			while( op != UserOp )
			{	CPPAD_ASSERT_UNKNOWN(
					op==UsrapOp || op==UsravOp || op==UsrrpOp || op==UsrrvOp
				);
				map_user_op[i_op] = begin;
			}
			map_user_op[i_op] = begin;
		}
	}
	// If in_subgraph[i_op] == n_dep + 1, this operator is not
	// connected to the selected independent variables.
	//
	// If in_subgraph[i_op] == i_dep < n_dep, this operator is in the
	// subgraph for the correpsonding dependent variable.
	pod_vector<size_t> in_subgraph(num_op);
	//
	// Are all the independent variables selected
	bool entire_domain = true;
	for(size_t j = 0; j < n_ind; j++)
		entire_domain &= select_domain[j];
	//
	// initialize in_subgraph
	if( entire_domain )
	{	// initialize in_subgraph to n_dep; i.e., all operators depend on
		// the independent variables
		for(size_t i_op = 0; i_op < num_op; ++i_op)
			in_subgraph[i_op] = n_dep;
	}
	else
	{	// initialize in_subgraph to n_dep + 1
		// We will use a forward pass to determine which operators depend
		// on the selected independent variables.
		for(size_t i_op = 0; i_op < num_op; ++i_op)
			in_subgraph[i_op] = n_dep + 1;
		//
		// Change to n_dep for each operator connected to selected domain.
		// Only need to modify UserOp and operators that have NumRes(op) > 0
		for(size_t i_op = 0; i_op < num_op; ++i_op)
		{	OpCode op = play->GetOp(i_op);
			switch(op)
			{	case InvOp:
				CPPAD_ASSERT_UNKNOWN( NumRes(op) > 0 );
				CPPAD_ASSERT_UNKNOWN( i_op > 0 );
				// get user index for this independent variable
				{	size_t j = i_op - 1;
					CPPAD_ASSERT_UNKNOWN( play->var2op(ind_taddr[j]) == i_op );
					//
					// change if this independent has been selected
					if( select_domain[j] )
						in_subgraph[i_op] = n_dep;
				}
				break;

				// this will mark both first and last UserOp
				// but that does not matter
				case UserOp:
				get_argument_variable(play, i_op, argument_variable, work);
				for(size_t j = 0; j < argument_variable.size(); j++)
				{	size_t j_var = argument_variable[j];
					size_t j_op  = play->var2op(j_var);
					j_op         = map_user_op[j_op];
					if( in_subgraph[j_op] == n_dep )
						in_subgraph[i_op] = n_dep;
				}
				break;

				default:
				if( NumRes(op) > 0 )
				{	get_argument_variable(play, i_op, argument_variable, work);
					for(size_t j = 0; j < argument_variable.size(); j++)
					{	size_t j_var = argument_variable[j];
						size_t j_op  = play->var2op(j_var);
						j_op         = map_user_op[j_op];
						if( in_subgraph[j_op] == n_dep )
							in_subgraph[i_op] = n_dep;
					}
				}
				break;
			}
		}
	}

	// for each of the selected dependent variables
	for(size_t i_dep = 0; i_dep < n_dep; ++i_dep) if( select_range[i_dep] )
	{
		// start with an empty subgraph for this dependent variable
		subgraph.erase();

		// tape index corresponding to this dependent variable
		size_t i_var = dep_taddr[i_dep];

		// operator corresponding to this dependent variable
		size_t i_op = play->var2op(i_var);
		i_op        = map_user_op[i_op];

		// if this variable depends on the selected indepent variables
		// start processing its subgraph
		if( in_subgraph[i_op] <= n_dep )
			subgraph.push_back(i_op);

		// scan all the variables in this subgraph
		size_t sub_index = 0;
		while(sub_index < subgraph.size() )
		{	// scan this operator to see which variables are connected to it
			i_op      = subgraph[sub_index];
			OpCode op = play->GetOp(i_op);
			CPPAD_ASSERT_UNKNOWN( in_subgraph[i_op] <= n_dep );
			//
			// There must be a result for this operator
			CPPAD_ASSERT_UNKNOWN( op == UserOp || NumRes(op) > 0 );
			//
			if( in_subgraph[i_op] !=  i_dep )
			{	// mark this operator so we do not repeat this calculation.
				in_subgraph[i_op] = i_dep;
				//
				// special case of independent variable operator
				if( op == InvOp )
				{	CPPAD_ASSERT_NARG_NRES(op, 0, 1);
					i_var        = i_op;
					size_t i_ind = i_var - 1;
					CPPAD_ASSERT_UNKNOWN( play->var2op(i_var) == i_op );
					CPPAD_ASSERT_UNKNOWN( i_var == ind_taddr[i_ind] );
					CPPAD_ASSERT_UNKNOWN( select_domain[i_ind] );
					row_out.push_back(i_dep);
					col_out.push_back(i_ind);
				}
				else
				{	// which variables are connected to this operator
					get_argument_variable(play, i_op, argument_variable, work);
					//
					// add the corresponding operators to the subgraph
					for(size_t j = 0; j < argument_variable.size(); ++j)
					{	size_t j_var = argument_variable[j];
						size_t j_op  = play->var2op(j_var);
						j_op         = map_user_op[j_op];
						if( in_subgraph[j_op] <= n_dep )
							subgraph.push_back(j_op);
					}
				}
			}
			++sub_index;
		}
	}
}

} } // END_CPPAD_LOCAL_NAMESPACE

# endif
