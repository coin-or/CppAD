# ifndef CPPAD_LOCAL_SUBGRAPH_HPP
# define CPPAD_LOCAL_SUBGRAPH_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/local/pod_vector.hpp>

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*!
\file subgraph.hpp
Compute dependency sparsity pattern using subgraph technique.
*/

// ===========================================================================
/*!
Determine the set of argument variables for an operator

\param play
is the player for this operation sequence.

\param i_op
is the operator index. If this operator is part of a user function call,
it must be the first UserOp in the call. (There is a UserOp at the
beginning and end of each call.)

\param variable
is the set of argument variables corresponding to this operator.

\param work
this is work space used by get_argument_variable to make subsequent calls
faster. It should not be used by the calling routine. In addition,
it is better if work does not drop out of scope between calls.
*/
template <typename Base>
void get_argument_variable(
	const player<Base>*  play        ,
	size_t               i_op        ,
	pod_vector<size_t>&  variable    ,
	pod_vector<bool>&    work        )
{
	// reset to size zero, but keep allocated memory
	variable.resize(0);
	//
	// operator corresponding to i_op
	OpCode        op;
	const addr_t* op_arg;
	size_t        i_var;
	play->get_op_info(i_op, op, op_arg, i_var);
	//
	// partial check of assumptions on user function calls
	CPPAD_ASSERT_UNKNOWN(
		op != UsrapOp && op != UsravOp && op != UsrrpOp && op != UsrrvOp
	);
	//
	// we assume this is the first UserOp of the call
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
				// cannot find second UserOp in this call
				CPPAD_ASSERT_UNKNOWN(false);
				break;
			}
			play->get_op_info(++i_op, op, op_arg, i_var);
		}
		CPPAD_ASSERT_UNKNOWN( variable.size() > 0 );
		return;
	}
	// is_varialbe is a reference to work with a better name
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

// ===========================================================================
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
	The input size and elements of this vector do not matter.
	If in_subgraph_[i_op] == n_dep_ (n_dep_ + 1),
	the result for this operator depends (does not depend)
	on the selected independent variables.
	Note that for user function call operators i_op,
	in_subgraph[i_op] is n_dep_ + 1 except for the first UserOp in the
	atomic function call sequence. For the first UserOp,
	it is n_dep_ (n_dep_ + 1) if any of the results for the call sequence
	depend (do not depend) on the selected independent variables.
	Except for UserOP, only operators with NumRes(op) > 0 are included
	in the dependency (have in_subgraph_ value n_dep_);
	e.g., comparision operators have in_subgraph_ value n_dep_ + 1.
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
// ===========================================================================
/*!
Include entire atomic function call when first UserOp is in a subgraph.

\tparam Base
this operation sequence was recording using AD<Base>.

\param play
is the operation sequence corresponding to the ADFun<Base> function.

\param subgraph
It a set of operator indices in this recording.
If the corresponding operator is a UserOp, it assumed to be the
first one in the corresponding atomic function call.
The other call operators are included in the subgraph.
*/
template <typename Base>
void subgraph_entire_call(
	const player<Base>*       play         ,
	pod_vector<addr_t>&       subgraph     )
{
	// add extra operators corresponding to rest of atomic function calls
	size_t n_sub = subgraph.size();
	for(size_t k = 0; k < n_sub; ++k)
	{	size_t i_op = subgraph[k];
		//
		if( play->GetOp(i_op) == UserOp )
		{	// This is the first UserOp of this atomic function call
			while( play->GetOp(++i_op) != UserOp )
			{	switch(play->GetOp(i_op))
				{
					case UsravOp:
					case UsrrvOp:
					case UsrrpOp:
					case UsrapOp:
					subgraph.push_back( addr_t(i_op) );
					break;

					default:
					// cannot find second UserOp in this call
					CPPAD_ASSERT_UNKNOWN(false);
					break;
				}
			}
			// THis is the second UserOp of this atomic function call
			subgraph.push_back( addr_t(i_op) );
		}
	}

}
// ===========================================================================
/*!
Compute dependency sparsity pattern for an ADFun<Base> function.

\tparam Base
the operation sequence was recorded using AD<Base>.

\tparam BoolVector
a simple vector class with elements of type bool.

\param play
is the operation sequence corresponding to the ADFun<Base> function.

\param sub_info
is the subgraph information for this ADFun object.

\param dep_taddr
mapping from user dependent variable index to variable index in play
(must have size sub_info.n_dep()).

\param select_domain
only the selected independent variables will be included in the sparsity
pattern (must have size sub_info.n_ind()).

\param select_range
only the selected dependent variables will be included in the sparsity pattern
(must have size sub_info.n_dep()).

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
	0 <= col_out[k] < sub_info.n_ind()
	select_domain[ col_out[k] ] == true
\endcode

\par UserOp
All of the inputs and outputs for an atomic function call are considered
to be connected.
2DO: It would be good to use the sparsity patters for atomic function calls
to to make the sparsity pattern more efficient.
*/

template <typename Base, typename BoolVector>
void subgraph_sparsity(
	const player<Base>*        play          ,
	subgraph_info&             sub_info      ,
	const vector<size_t>&      dep_taddr     ,
	const BoolVector&          select_domain ,
	const BoolVector&          select_range  ,
	pod_vector<size_t>&        row_out       ,
	pod_vector<size_t>&        col_out       )
{
	// check dimension assumptions
	CPPAD_ASSERT_UNKNOWN(
		size_t( dep_taddr.size() ) == sub_info.n_dep()
	);
	CPPAD_ASSERT_UNKNOWN(
		size_t(select_domain.size()) == sub_info.n_ind()
	);
	CPPAD_ASSERT_UNKNOWN(
		size_t(select_range.size()) == sub_info.n_dep()
	);

	// number of dependent variables
	size_t n_dep = dep_taddr.size();
	CPPAD_ASSERT_UNKNOWN( size_t(select_range.size()) == n_dep );

	// start with an empty sparsity pattern
	row_out.resize(0);
	col_out.resize(0);

	// map_user_op
	if( sub_info.map_user_op().size() == 0 )
		sub_info.set_map_user_op(play);
	else
	{	CPPAD_ASSERT_UNKNOWN( sub_info.check_map_user_op(play) );
	}
	const pod_vector<addr_t>& map_user_op( sub_info.map_user_op() );
	CPPAD_ASSERT_UNKNOWN( map_user_op.size() == play->num_op_rec() );

	// subgraph of operators that are are connected to one of the selected
	// dependent variables and depend on the selected independent variables
	pod_vector<addr_t> subgraph;

	// initialize in_subgraph
	sub_info.init_rev_in_subgraph(play, select_domain);
	pod_vector<addr_t>& in_subgraph( sub_info.in_subgraph() );
	CPPAD_ASSERT_UNKNOWN( in_subgraph.size() == play->num_op_rec() );
	//
	addr_t depend_no  = addr_t( n_dep + 1 );
	addr_t depend_yes = addr_t( n_dep );

	// for each of the selected dependent variables
	CPPAD_ASSERT_UNKNOWN( depend_yes < depend_no );
	CPPAD_ASSERT_UNKNOWN( NumRes(BeginOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( NumRes(InvOp) == 1 );
	for(size_t i_dep = 0; i_dep < n_dep; ++i_dep) if( select_range[i_dep] )
	{	CPPAD_ASSERT_UNKNOWN( i_dep < size_t( depend_yes ) );
		//
		get_rev_subgraph(
			play            ,
			dep_taddr       ,
			map_user_op     ,
			depend_yes      ,
			addr_t(i_dep)   ,
			in_subgraph     ,
			subgraph
		);
		//
		for(size_t k = 0; k < subgraph.size(); k++)
		{	size_t i_op = subgraph[k];
			//
			// operator corresponding to this index
			OpCode op = play->GetOp(i_op);
			//
			// This version of the subgraph only has first UserOp
			// for each atomic functionc all.
			CPPAD_ASSERT_UNKNOWN( NumRes(op) > 0 || op == UserOp );
			//
			// independent variable entries correspond to sparsity pattern
			if( op == InvOp )
			{	CPPAD_ASSERT_NARG_NRES(op, 0, 1);
				// i_var is equal i_op becasue BeginOp and InvOp have 1 result
				size_t i_var = i_op;       // tape index for this variable
				size_t i_ind = i_var - 1;  // user index for this variable
				CPPAD_ASSERT_UNKNOWN( play->var2op(i_var) == i_op );
				CPPAD_ASSERT_UNKNOWN( select_domain[i_ind] );
				//
				// put this pair in the sparsity pattern
				row_out.push_back(i_dep);
				col_out.push_back(i_ind);
			}
		}
	}
}

} } // END_CPPAD_LOCAL_NAMESPACE

# endif
