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

/*!
Compute dependency sparsity pattern for a player.

\tparam Base
this operation sequence was recorded using AD<Base>.

\tparam Vector_set
is the type used for vectors of sets. It can be either
sparse_pack or sparse_list.

\param play
this is the player holding the operation sequence.

\param ind_taddr
this is the vector of independent variables for this recording.

\param dep_taddr
this is the vector of dependent variables for this recording.

\param sparsity_out
this is the dependency sparsity pattern for the player.
The input value of sparsity_out does not matter.
Upon return
sparsity_out.n_set() = dep_taddr.size(),
sparsity_out.end()   = ind_taddr.size(),
and if (i, j) is in sparsity_out,
dep_taddr[i] depends on ind_taddr[j].
*/

template <typename Base, typename Vector_set>
void subgraph_dep(
	const player<Base>*    play         ,
	const vector<size_t>&  ind_taddr    ,
	const vector<size_t>&  dep_taddr    ,
	Vector_set&            sparsity_out )
{
	// number of independent variables
	size_t n_ind = ind_taddr.size();

	// numver of dependent variables
	size_t n_dep = dep_taddr.size();

	// number of variables in the tape
	size_t num_var = play->num_var_rec();

	// numbef of operators in the tape
	size_t num_op  = play->num_op_rec();

	// subgraph of variables that are not independent and are connected
	// to the dependent variable
	pod_vector<addr_t> subgraph;

	// start with an empty set of independent variables connected
	// to each dependent variable.
	sparsity_out.resize(n_dep, n_ind);

	// variables that are arguments to a particular user function call
	pod_vector<addr_t> argument_variable;

	// if sub_or_connected[i_op] == i_dep, one of the following holds:
	// 1. if i_op is an independent variable operator connected to i_dep
	// 2. otherwise it is in subgraph for dependent variable i_dep
	pod_vector<addr_t> sub_or_connected(num_op);
	//
	// initilaize sub_or_connected to an impossible dependent variable index
	for(size_t i_op = 0; i_op < num_op; ++i_op)
		sub_or_connected[i_op] = addr_t(n_dep);

	// which arguments, for one operator, are variables
	pod_vector<bool> variable;

	// for each dependent variable
	for(size_t i_dep = 0; i_dep < n_dep; ++i_dep)
	{
		// start with an empty subgraph for this dependent variable
		subgraph.erase();

		// tape index corresponding to this dependent variable
		size_t i_var = dep_taddr[i_dep];

		// put this node in the subgraph for this independent variable
		// (var_index cannot be an independent variable so put is subgraph)
		size_t i_op = play->var2op(i_var);
		CPPAD_ASSERT_UNKNNOWN( play->GetOp(i_op) != InvOp );
		subgraph.push_back(i_op);
		sub_or_connected[i_op] = addr_t( i_dep );

		// check all that all the variables in the subgraph have been scanned
		size_t sub_index = 0;
		while(sub_index < subgraph.size() )
		{	// scan this variable to see which other variables are connected
			i_var    = subgraph[sub_index];
			//
			// operator for this variable
			i_op  = play->var2op(i_var);
			//
			// get the information for this operator
			local::OpCode  op;
			addr_t* op_arg;
			play->get_info(i_op, op, op_arg, i_var);
			CPPAD_ASSERT_UNKNOWN( NumRes(op) > 0 );
			//
			// special case where operator corresponds to a user function call
			if( op == UsrrvOp )
			{	// 2DO: make the resulting sparsity_out  more efficient using
				// sparsity pattern for the user function call.
				//
				// determine the UserOp that started this function call
				// along with the first variable result for this call.
				while( op != UserOp )
				{	CPPAD_ASSERT_UNKNOWN( i_op > 0 );
					play->get_info(--i_op, op, op_arg, i_var);
					CPPAD_ASSERT_UNKNOWN(
						op == UsrapOp ||
						op == UsravOp ||
						op == UsrrpOp ||
						op == UsrrvOp
					);
				}
				//
				// check if we have processed this function call
				if( sub_or_connected[i_op] != addr_t(i_dep) )
				{	// Mark as processed so we do not repeat this calculation.
					sub_or_connected[i_op] = addr_t(i_dep);
					//
					// Check that this is only place sub_or_connected can
					// change for this i_op
					CPPAD_ASSERT_UNKNOWN( NumRes(op) == 0 );
					//
					// determine which variables are connected to this call
					user_variables(play, i_op, argument_variable);
					//
					// check each of these variables
					for(size_t j = 0; j < argument_variable.size(); ++j)
					{	size_t j_var = argument_variable[j];
						size_t j_op  = play->var2op( j_var );
						//
						// has this variable already been processed
						// for this dependent variable
						if( sub_or_connected[j_op] != addr_t(i_dep) )
						{	// variable not yet in subgraph or connected
							//
							if( play->GetOp(j_op) == InvOp )
							{	// This is an independent variable
								size_t j_ind = j_var - 1;
								CPPAD_ASSERT_UNKNOWN(
									j_var == ind_taddr[j_ind]
								);
								sparsity_out.add_element(i_dep, j_ind);
							}
							else
							{	// add to the subgraph
								subgraph.push_back( addr_t( j_op ) );
							}
							sub_or_connected[j_op] = addr_t( i_dep );
						}
					}
				}
			}
			else
			{
				// variable = which operator arguments are variables
				// num_arg  = true number of arguments for this operator
				size_t num_arg = arg_is_variable(op, op_arg, variable);

				// loop through arguments that are variables
				for(size_t j = 0; j < num_arg; ++j) if( variable[j] )
				{	// index for this variable
					size_t j_var = op_arg[j];
					size_t j_op  = play->var2op( j_var );
					//
					// has this variable already been processed
					// for this dependent variable
					if( sub_or_connected[j_op] != addr_t(i_dep) )
					{	// variable not yet in subgraph or connected
						//
						if( play->GetOp(j_op) == InvOp )
						{	// This is an independent variable
							size_t j_ind = j_var - 1;
							CPPAD_ASSERT_UNKNOWN( j_var == ind_taddr[j_ind] );
							sparsity_out.add_element(i_dep, j_ind);
						}
						else
						{	// add to the subgraph
							subgraph.push_back( addr_t( j_op ) );
						}
						sub_or_connected[j_op] = addr_t( i_dep );
					}
				}
			}
			++sub_index;
		}
	}
}

} } // END_CPPAD_LOCAL_NAMESPACE

# endif
