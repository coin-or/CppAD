# ifndef CPPAD_LOCAL_SUBGRAPH_INFO_HPP
# define CPPAD_LOCAL_SUBGRAPH_INFO_HPP
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

// BEGIN_CPPAD_LOCAL_SUBGRAPH_NAMESPACE
namespace CppAD { namespace local { namespace subgraph {
/*!
\file info.hpp
subgraph information attached to a operation sequence
*/

/// class for maintaining subgraph information attached to on ADFun object.
class subgraph_info {
private:
	// -----------------------------------------------------------------------
	// private member data
	// -----------------------------------------------------------------------
	/// number of independent variables for this function
	size_t             n_ind_;

	/// number of dependent variables for this function
	size_t             n_dep_;

	/// number of operatros in operation sequence
	size_t             n_op_;

	/// the entire operation sequence as a subgraph (size n_op_).
	pod_vector<addr_t> entire_graph_;

	/// mapping atomic call operators to UserOp that begins call sequence
	/// (size zero or n_op_).
	pod_vector<addr_t> map_user_op_;

	/// flags which operatiors are in subgraph
	/// (size zero or n_op_).
	pod_vector<addr_t> in_subgraph_;

public:
	// -----------------------------------------------------------------------
	// const public functions
	// -----------------------------------------------------------------------
	/// number of independent variables
	size_t n_ind(void) const
	{	return n_ind_; }

	/// number of dependent variables
	size_t n_dep(void) const
	{	return n_dep_; }

	/// number of operators
	size_t n_op(void) const
	{	return n_op_; }

	/// entire graph represented as a sorted subgraph
	const pod_vector<addr_t>& entire_graph(void) const
	{	return entire_graph_; }

	/// map user atomic function calls to first operator in the call
	const pod_vector<addr_t>& map_user_op(void) const
	{	return map_user_op_; }

	/// amount of memory corresonding to this object
	size_t memory(void) const
	{	CPPAD_ASSERT_UNKNOWN( entire_graph_.size() == n_op_ );
		size_t sum_addr_t = entire_graph_.size();
		sum_addr_t       += map_user_op_.size();
		sum_addr_t       += in_subgraph_.size();
		return sum_addr_t * sizeof(addr_t);
	}
	// -----------------------------------------------------------------------
	/*!
	check that the value of map_user_op is OK for this operation sequence

	\param play
	is the player for this operation sequence.

	\return
	is true, if map_user_op has the correct value for this operation sequence
	(is the same as it would be after a set_map_user_op).
	*/
	template <typename Base>
	bool check_map_user_op(const player<Base>* play) const
	{	if( map_user_op_.size() != n_op_ )
			return false;
		bool   ok   = true;
		size_t i_op = 0;
		while( i_op < n_op_ )
		{	OpCode op = play->GetOp(i_op);
			ok       &= map_user_op_[i_op] == addr_t( i_op );
			if( op == UserOp )
			{	addr_t begin = addr_t( i_op );
				op           = play->GetOp(++i_op);
				while( op != UserOp )
				{	CPPAD_ASSERT_UNKNOWN(
					op==UsrapOp || op==UsravOp || op==UsrrpOp || op==UsrrvOp
					);
					ok  &= map_user_op_[i_op] == begin;
					op   = play->GetOp(++i_op);
				}
				ok  &= map_user_op_[i_op] == begin;
			}
			++i_op;
		}
		return ok;
	}
	// -----------------------------------------------------------------------
	// non const public functions
	// -----------------------------------------------------------------------

	/// flag which operators that are in the subgraph
	pod_vector<addr_t>& in_subgraph(void)
	{	return in_subgraph_; }


	/// default constructor (all sizes are zero)
	subgraph_info(void)
	: n_ind_(0), n_dep_(0), n_op_(0)
	{	CPPAD_ASSERT_UNKNOWN( entire_graph_.size() == n_op_ );
		CPPAD_ASSERT_UNKNOWN( map_user_op_.size() == n_op_ );
		CPPAD_ASSERT_UNKNOWN( in_subgraph_.size() == n_op_ );
	}
	// -----------------------------------------------------------------------
	/// assignment operator
	void operator=(const subgraph_info& info)
	{	n_ind_            = info.n_ind_;
		n_dep_            = info.n_dep_;
		n_op_             = info.n_op_;
		entire_graph_     = info.entire_graph_;
		map_user_op_      = info.map_user_op_;
		in_subgraph_      = info.in_subgraph_;
		return;
	}
	// -----------------------------------------------------------------------
	/*!
	set sizes for this object (the default sizes are zero)

	\param n_ind
	number of indepent variables.

	\param n_dep
	number of dependent variables.

	\param n_op
	number of operators.

	\par entire_graph_
	This member funcition is set the sorted subgraph corresponding to the
	entire operation sequence; i.e., entire_graph_[i_op] == i_op for
	i_op = 0 , ... , n_op -1.

	\par map_user_op_
	is resized to zero.

	\par in_subgraph_
	is resized to zero.
	*/
	void resize(size_t n_ind, size_t n_dep, size_t n_op)
	{	CPPAD_ASSERT_UNKNOWN(
			n_op <= size_t( std::numeric_limits<addr_t>::max() )
		);
		// n_ind_
		n_ind_ = n_ind;
		// n_dep_
		n_dep_ = n_dep;
		// n_op_
		n_op_  = n_op;
		//
		// entire_graph_
		size_t old_size = entire_graph_.size();
		size_t old_cap  = entire_graph_.capacity();
		entire_graph_.resize(n_op);
		if( old_cap < n_op )
		{	for(size_t i_op = 0; i_op < n_op; ++i_op)
				entire_graph_[i_op] = addr_t( i_op );
		}
		else if( old_size < n_op )
		{	for(size_t i_op = old_size; i_op < n_op; ++i_op)
				entire_graph_[i_op] = addr_t( i_op );
		}
		//
		// map_user_op_
		map_user_op_.resize(0);
		//
		// in_subgraph_
		in_subgraph_.resize(0);
		//
		return;
	}
	// -----------------------------------------------------------------------
	/*!
	set the value of map_user_op for this operation sequence

	\param play
	is the player for this operation sequence. It must have size
	equal to n_op_.

	\par map_user_op_
	This size of map_user_op must be zero when this function is called
	(which it is after a resize operation).
	This function sets its size to the number of operations in play.
	We use the term user OpCocde for the any one of the following:
	UserOp, UsrapOp, UsravOp, UsrrpOp, or UsrrvOp. Suppose
	\code
		OpCodce op_i = play->GetOp(i_op);
		size_t  j_op = map_user_op[i_op];
		OpCode  op_j = play->GetOP(j_op);
	\endcode
	If op is a user OpCode, j_op is the index of the first operator
	in the corresponding atomic function call and op_j == UserOp.
	Otherwise j_op == i_op;
	*/
	template <typename Base>
	void set_map_user_op(const player<Base>* play)
	{	CPPAD_ASSERT_UNKNOWN( map_user_op_.size() == 0 );
		CPPAD_ASSERT_UNKNOWN( n_op_ == play->num_op_rec() );
		map_user_op_.resize(n_op_);
		for(size_t i_op = 0; i_op < n_op_; ++i_op)
		{	map_user_op_[i_op] = addr_t( i_op );
			OpCode op = play->GetOp(i_op);
			if( op == UserOp )
			{	addr_t begin = addr_t( i_op );
				op           = play->GetOp(++i_op);
				while( op != UserOp )
				{	CPPAD_ASSERT_UNKNOWN(
					op==UsrapOp || op==UsravOp || op==UsrrpOp || op==UsrrvOp
					);
					map_user_op_[i_op] = begin;
					op                 = play->GetOp(++i_op);
				}
				map_user_op_[i_op] = begin;
			}
		}
		return;
	}
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
	We use depend_yes (depend_no) for the value n_dep_, n_dep_+1.
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
	*/
	template <typename Base, typename BoolVector>
	void init_rev_in_subgraph(
		const player<Base>*  play                ,
		const BoolVector&    select_domain       )
	{
		// number of operators in the recording
		CPPAD_ASSERT_UNKNOWN( map_user_op_.size() == n_op_ );
		CPPAD_ASSERT_UNKNOWN( play->num_op_rec()  == n_op_ );

		// depend_yes and depend_no
		addr_t depend_yes = addr_t( n_dep_ );
		addr_t depend_no  = addr_t( n_dep_ + 1 );

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
};

} } } // END_CPPAD_LOCAL_SUBGRAPH_NAMESPACE

# endif
