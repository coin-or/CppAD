# ifndef CPPAD_CORE_SUBGRAPH_DEP_HPP
# define CPPAD_CORE_SUBGRAPH_DEP_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin subgraph_depend$$

$section Computing Dependency Using A Sub-graph Method$$

$head Under Construction$$

$head Syntax$$
$icode%f%.subgraph_dep(%pattern_out%)%$$

$head Purpose$$
We use $latex F : B^n \rightarrow B^m$$ to denote the
$cref/AD function/glossary/AD Function/$$ corresponding to $icode f$$.
This routine computes a
$cref/dependency pattern/dependency.cpp/Dependency Pattern/$$ for
$latex F ( x )$$ using a sub-graph technique.
To be specific, for $latex i = 0 , \ldots , m-1$$,
the dependency for $latex F_i (x)$$ is computed by traversing
the sub-graph corresponding to the $th i$$ dependent variable.

$head SizeVector$$
We use $icode SizeVector$$ to denote $cref SimpleVector$$ class
$cref/with elements of type/SimpleVector/Elements of Specified Type/$$
$code size_t$$.

$head pattern_out$$
This argument has prototype
$codei%
	sparse_rc<%SizeVector%>& %pattern_out%
%$$
This input value of $icode pattern_out$$ does not matter.
Upon return $icode pattern_out$$ is a
$cref/dependency pattern/dependency.cpp/Dependency Pattern/$$
for $latex F(x)$$.
To be specific, if $latex F_i (x)$$ depends on the value of $latex x_j$$
then the row, column pair  $latex (i, j)$$ appears in $icode pattern_out$$.

$end
*/

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*!
\file subgraph_dep.hpp
Dependency for on dependent variable .
*/
/*!
Determine the set of results and argument variables for a user function call.

\param play
is the player for this operation sequence.

\param i_op
is the operator index for the first UserOp in the user function call
(there are two in each such call).

\param argument_variable
is the set of variables corresponding to the arguments
for this user function call.
*/
template <typename Base>
void user_variables(
	const player<Base>*  play              ,
	size_t               i_op              ,
	pod_vector<addr_t>&  argument_variable )
{
	OpCode        op;
	const addr_t* op_arg;
	size_t        i_var;
	play->get_info(i_op, op, op_arg, i_var);
	CPPAD_ASSERT_UNKNOWN( op == UserOp );
	//
	argument_variable.erase();
	//
	play->get_info(++i_op, op, op_arg, i_var);
	while( op != UserOp )
	{	switch(op)
		{
			case UsravOp:
			{	CPPAD_ASSERT_NARG_NRES(op, 1, 0);
				size_t j_var = op_arg[0];
				argument_variable.push_back(j_var);
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
		play->get_info(++i_op, op, op_arg, i_var);
	}
	return;
}


template <typename Base, typename SizeVector>
void subgraph_dep(
	const player<Base>*    play         ,
	const vector<size_t>&  dep_taddr    ,
	sparse_rc<SizeVector>& pattern_out  )
{
	// number of operators in the tape
	size_t num_var = play->num_var_rec();

	// set of variables in the subgraph
	pod_vector<addr_t> subgraph;

	// set of independent variables connected to the subgraph
	pod_vector<addr_t> connected;

	// variables that are arguments to a user function call
	pod_vector<addr_t> argument_variable;

	// if sub_or_connected[i_op] == i, one of the following holds:
	// 1. operator i_op is already in sub-graph for dependent variable i
	// 2. i_op corresponds to an independent variable connected to subgraph
	pod_vector<addr_t> sub_or_connected(num_var);
	//
	// initilaize sub_or_connected to an impossible dependent variable index
	size_t n_dep = dep_taddr.size();
	for(size_t i_var = 0; i_var < num_var; ++i_var)
		sub_or_connected[i_var] = addr_t(n_dep);

	// which arguments, for one operator, are variables
	pod_vector<bool> variable;

	// for each dependent variable
	for(size_t i_dep = 0; i_dep < n_dep; ++i_dep)
	{	// start with an empty subgraph and connected set
		subgraph.erase();
		connected.erase();

		// tape index corresponding to this dependent variable
		size_t var_index = dep_taddr[i_dep];

		// put this node in the sub-graph for this independent variable
		subgraph.push_back(var_index);
		sub_or_connected[var_index] = addr_t( i_dep );

		// check all that all the variables in the subgraph have been scanned
		size_t sub_index = 0;
		while(sub_index < subgraph.size() )
		{	// variable for this node in the subgraph
			var_index    = subgraph[sub_index];
			//
			// operator for this noide
			size_t i_op  = play->var2op(var_index);
			//
			// get the information for this operator
			local::OpCode  op;
			addr_t* op_arg;
			size_t  i_var;
			play->get_info(i_op, op, op_arg, i_var);
			CPPAD_ASSERT_UNKNOWN( NumRes(op) > 0 );
			CPPAD_ASSERT_UNKNOWN( var_index == i_var );

			// special case where operator corresponds to a user function call
			if( op == UsrrvOp )
			{	// 2DO: make this more efficient using sparsity pattern
				// for the user function call.
				//
				// determine the UserOp that started this function call
				// along with the first variable result for this call.
				size_t first_result = i_var;
				while( op != UserOp )
				{	CPPAD_ASSERT_UNKNOWN( i_op > 0 );
					play->get_info(--i_op, op, op_arg, i_var);
					CPPAD_ASSERT_UNKNOWN(
						op == UsrapOp ||
						op == UsravOp ||
						op == UsrrpOp ||
						op == UsrrvOp
					);
					if( op == UsrrvOp )
						first_result = i_var;
				}
				if( sub_or_connected[first_result] != addr_t(i_dep) )
				{	// we have not yet processed this function call
					//
					sub_or_connected[first_result] = addr_t(i_dep);
					user_variables(play, i_op, argument_variable);
					for(size_t j = 0; j < argument_variable.size(); ++j)
					{	size_t j_var = argument_variable[j];
						if( sub_or_connected[j_var] != addr_t(i_dep) )
						{	// variable not yet in subgraph or connected
							size_t j_op = play->var2op( j_var );
							if( play->GetOp(j_op) == InvOp )
							{	// This is an independent variable
								CPPAD_ASSERT_UNKNOWN( j_var == j_op - 1 );
								connected.push_back( addr_t(j_var) );
							}
							else
							{	// add to the subgraph
								subgraph.push_back( addr_t( j_var ) );
							}
							sub_or_connected[j_var] = addr_t( i_dep );
						}
					}
				}
			}
			else
			{
				// variable = which operator arguments are variables
				// num_arg  = true number of arguments for this operator
				size_t num_arg = arg_is_variable(op, op_arg, variable);

				// loop through the variables
				for(size_t j = 0; j < num_arg; ++j) if( variable[j] )
				{	// index for this variables
					size_t j_var = op_arg[j];
					{	// variable not yet in subgraph or connected
						size_t j_op = play->var2op( j_var );
						if( play->GetOp(j_op) == InvOp )
						{	// This is an independent variable
							CPPAD_ASSERT_UNKNOWN( j_var == j_op - 1 );
							connected.push_back( addr_t(j_var) );
						}
						else
						{	// add to the subgraph
							subgraph.push_back( addr_t( j_var ) );
						}
						sub_or_connected[j_var] = addr_t( i_dep );
					}
				}
			}
			++sub_index;
		}
	}
}

} } // END_CPPAD_LOCAL_NAMESPACE

# endif
