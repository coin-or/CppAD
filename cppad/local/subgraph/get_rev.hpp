# ifndef CPPAD_LOCAL_SUBGRAPH_GET_REV_HPP
# define CPPAD_LOCAL_SUBGRAPH_GET_REV_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/local/pod_vector.hpp>
# include <cppad/local/subgraph/arg_variable.hpp>
# include <cppad/local/subgraph/info.hpp>

// BEGIN_CPPAD_LOCAL_SUBGRAPH_NAMESPACE
namespace CppAD { namespace local { namespace subgraph {
/*!
\file get_rev.hpp
Get subgraph corresponding to a dependent variable.
*/

// ===========================================================================
/*!
Get the subgraph corresponding to a dependent variables
(and a selected set of independent variables).

\tparam Base
this operation sequence was recording using AD<Base>.

\param play
is the operation sequence corresponding to the ADFun<Base> function.

\param dep_taddr
is the vector mapping user dependent variable indices
to the correpsonding variable in he recording.

\param map_user_op
has size is equal to the number of operators in play.
We use the term user OpCocde for the any one of the following:
UserOp, UsrapOp, UsravOp, UsrrpOp, or UsrrvOp.
Suppose
\code
	OpCodce op_i = play->GetOp(i_op);
	size_t  j_op = map_user_op[i_op];
	OpCode  op_j = play->GetOP(j_op);
\endcode
If op is a user OpCode, j_op is the index of the first operator
in the corresponding atomic function call and op_j == UserOp.
Otherwise j_op == i_op;

\param depend_yes
is the value used to signify that an operator depends on the selected
independent variables.

\param i_dep
is the user index for his dependent variable;
that i_dep < depend_yes.
Furthermore, in_subgraph[i_op] is not eqaul to i_dep for any i_op.

\param in_subgraph
has size equal to the number of operators in play.
If in_subgraph[i_op] <= depend_yes,
the result for this operator depends on the selected independent variables.
In addition, upon input, there is no i_op such that in_subgraph[i_op] == i_dep.
Note that for user function call operators i_op,
\code
	depend_yes < in_subgraph[i_op]
\endcode
except for the first UserOp in the atomic function call sequence.
For the first UserOp,
\code
	in_subgraph[i_op] <= depend_yes
\endcode
if any result for the user function call
depends on the selected independent variables.
Except for UserOP, only operators with NumRes(op) > 0 are included
in the dependency; e.g., comparision operators are not included.
Upon return, some of the i_op for which in_subgraph[i_op] <= depend_yes,
will be changed to in_subgraph[i_op] = i_dep.

\param subgraph
the input size and contents of this vector do not matter.
Repeated calls with the same subgraph vector should reduce
the amount of memory allocation.
Upon return it contains the operator indices for the subgraph
corresponding to the dependent and the selected independent variables.
Furthermore the operator indices in subgraph are unique; i.e.,
if i != j then subgraph[i] != subgraph[k].
*/
template <typename Base>
void get_rev_subgraph(
	const player<Base>*       play         ,
	const vector<size_t>&     dep_taddr    ,
	const pod_vector<addr_t>& map_user_op  ,
	addr_t                    depend_yes   ,
	addr_t                    i_dep        ,
	pod_vector<addr_t>&       in_subgraph  ,
	pod_vector<addr_t>&       subgraph     )
{
	// assumption on i_dep
	CPPAD_ASSERT_UNKNOWN( i_dep < depend_yes );

	// variables that are arguments to a particular operator
	pod_vector<size_t> argument_variable;

	// work space used by get_argument_variable
	pod_vector<bool> work;

	// start with an empty subgraph for this dependent variable
	subgraph.resize(0);

	// tape index corresponding to this dependent variable
	size_t i_var = dep_taddr[i_dep];

	// operator corresponding to this dependent variable
	size_t i_op = play->var2op(i_var);
	i_op        = map_user_op[i_op];

	// if this variable depends on the selected indepent variables
	// process its subgraph
	CPPAD_ASSERT_UNKNOWN( in_subgraph[i_op] != i_dep )
	if( in_subgraph[i_op] <= depend_yes )
	{	subgraph.push_back( addr_t(i_op) );
		in_subgraph[i_op] = i_dep;
	}

	// scan all the operators in this subgraph
	size_t sub_index = 0;
	while(sub_index < subgraph.size() )
	{	// this operator connected to this dependent and selected independent
		i_op = subgraph[sub_index];
		CPPAD_ASSERT_UNKNOWN( in_subgraph[i_op] <= i_dep );
		//
		// There must be a result for this operator
# ifndef NDEBUG
		OpCode op = play->GetOp(i_op);
		CPPAD_ASSERT_UNKNOWN(op == UserOp || NumRes(op) > 0 );
# endif
		//
		// which variables are connected to this operator
		get_argument_variable(play, i_op, argument_variable, work);
		//
		// add the corresponding operators to the subgraph
		for(size_t j = 0; j < argument_variable.size(); ++j)
		{	size_t j_var = argument_variable[j];
			size_t j_op  = play->var2op(j_var);
			j_op         = map_user_op[j_op];
			bool  depend = in_subgraph[j_op] <= depend_yes;
			bool  in     = in_subgraph[j_op] == i_dep;
			if( depend & (! in) )
			{	subgraph.push_back( addr_t(j_op) );
				in_subgraph[j_op] = i_dep;
			}
		}
		// we are done scaning this subgraph operator
		++sub_index;
	}
}

} } } // END_CPPAD_LOCAL_SUBGRAPH_NAMESPACE

# endif
