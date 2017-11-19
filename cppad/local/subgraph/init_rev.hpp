
# ifndef CPPAD_LOCAL_SUBGRAPH_INIT_REV_HPP
# define CPPAD_LOCAL_SUBGRAPH_INIT_REV_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/local/subgraph/info.hpp>

// BEGIN_CPPAD_LOCAL_SUBGRAPH_NAMESPACE
namespace CppAD { namespace local { namespace subgraph {
/*!
\file init_rev.hpp
initialize for a reverse mode subgraph calculation
*/

// -----------------------------------------------------------------------
/*!
Initialize in_subgraph corresponding to a single dependent variable
(and a selected set of independent variables).

\tparam Base
this operation sequence was recording using AD<Base>.

\param play
is the operation sequence corresponding to the ADFun<Base> function
(it must correspond to map_user_op_).

\param select_domain
is a vector with, size equal to the number of independent variables
in the recording. It determines the selected independent variables.

\par in_subgraph_
We use depend_yes (depend_no) for the value n_dep_ (n_dep_ + 1).
The important properties are that depend_yes < depend_no and
for a valid indpendent variable index i_ind < depend_yes.
The input size and elements of in_subgraph_ do not matter.
If in_subgraph_[i_op] == depend_yes (depend_no),
the result for this operator depends (does not depend)
on the selected independent variables.
Note that for user function call operators i_op,
in_subgraph[i_op] is depend_no except for the first UserOp in the
atomic function call sequence. For the first UserOp,
it is depend_yes (depend_no) if any of the results for the call sequence
depend (do not depend) on the selected independent variables.
Except for UserOP, only operators with NumRes(op) > 0 have in_subgraph_
value depend_yes;
e.g., comparision operators have in_subgraph_ value depend_no.

\par processed_
This vector is initialized to have size zero when
init_rev_in_subgraph is called.
*/
template <typename Base, typename BoolVector>
void subgraph_info::init_rev_in_subgraph(
	const player<Base>*  play                ,
	const BoolVector&    select_domain       )
{
	// number of operators in the recording
	CPPAD_ASSERT_UNKNOWN( map_user_op_.size() == n_op_ );
	CPPAD_ASSERT_UNKNOWN( play->num_op_rec()  == n_op_ );

	// depend_yes and depend_no
	addr_t depend_yes = addr_t( n_dep_ );
	addr_t depend_no  = addr_t( n_dep_ + 1 );

	// processed_
	processed_.resize(0);

	// set in_subgraph to have proper size
	in_subgraph_.resize(n_op_);

	// variables that are arguments to a particular operator
	pod_vector<size_t> argument_variable;

	// work space used by get_argument_variable
	pod_vector<bool> work;

# ifndef NDEBUG
	size_t count_independent = 0;
# endif
	bool begin_atomic_call = false;
	for(size_t i_op = 0; i_op < n_op_; ++i_op)
	{	OpCode op = play->GetOp(i_op);
		//
		// default value for this operator
		in_subgraph_[i_op] = depend_no;
		//
		switch(op)
		{	case InvOp:
			CPPAD_ASSERT_UNKNOWN( NumRes(op) > 0 );
			CPPAD_ASSERT_UNKNOWN( i_op > 0 );
			{	// get user index for this independent variable
				size_t j = i_op - 1;
				CPPAD_ASSERT_UNKNOWN( j < size_t(select_domain.size()) );
				//
				// set in_subgraph_[i_op]
				if( select_domain[j] )
					in_subgraph_[i_op] = depend_yes;
			}
# ifndef NDEBUG
			++count_independent;
# endif
			break;

			// only mark both first UserOp for each call as depending
			// on the selected independent variables
			case UserOp:
			begin_atomic_call  = not begin_atomic_call;
			if( begin_atomic_call )
			{	get_argument_variable(play, i_op, argument_variable, work);
				for(size_t j = 0; j < argument_variable.size(); j++)
				{	size_t j_var = argument_variable[j];
					size_t j_op  = play->var2op(j_var);
					j_op         = map_user_op_[j_op];
					CPPAD_ASSERT_UNKNOWN( j_op < i_op );
					if( in_subgraph_[j_op] == depend_yes )
						in_subgraph_[i_op] =  depend_yes;
				}
			}
			break;

			// skip UsrrvOp (gets mapped to first UserOp in this call)
			case UsrrvOp:
			CPPAD_ASSERT_UNKNOWN( NumRes(op) > 0 );
			break;

			default:
			// Except for UserOp, only include when NumRes(op) > 0.
			if( NumRes(op) > 0 )
			{	get_argument_variable(play, i_op, argument_variable, work);
				for(size_t j = 0; j < argument_variable.size(); j++)
				{	size_t j_var = argument_variable[j];
					size_t j_op  = play->var2op(j_var);
					j_op         = map_user_op_[j_op];
					CPPAD_ASSERT_UNKNOWN( j_op < i_op );
					if( in_subgraph_[j_op] == depend_yes )
						in_subgraph_[i_op] =  depend_yes;
				}
			}
			break;
		}
	}
	CPPAD_ASSERT_UNKNOWN(
		count_independent == size_t(select_domain.size())
	);
	//
	return;
}

} } } // END_CPPAD_LOCAL_SUBGRAPH_NAMESPACE

# endif
