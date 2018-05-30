
# ifndef CPPAD_LOCAL_PLAY_RANDOM_ITERATOR_HPP
# define CPPAD_LOCAL_PLAY_RANDOM_ITERATOR_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

// BEGIN_CPPAD_LOCAL_PLAY_NAMESPACE
namespace CppAD { namespace local { namespace play {

/*!
\file random_iterator.hpp
*/

/*!
Constant random iterator for a player object.

Except for constructor, the public API for this class is a super-set of
(contains) the API for the sequential iterator class.
*/
template <class Base>
class const_random_iterator {
private:
	/// play_
	const player<Base>*       play_;

	/// subgraph_
	const pod_vector<addr_t>* subgraph_;

	/// index in subgraph of current operator
	size_t subgraph_index_;

public:
	/// assignment operator
	void operator=(const const_random_iterator& rhs)
	{	play_            = rhs.play_;
		subgraph_        = rhs.subgraph_;
		subgraph_index_  = rhs.subgraph_index_;
		return;
	}
	/// Create an iterator starting either at beginning or end of subgraph
	const_random_iterator(
		const player<Base>*       play             ,
		const pod_vector<addr_t>* subgraph         ,
		size_t                    subgraph_index   )
	:
	play_           ( play )            ,
	subgraph_       ( subgraph )        ,
	subgraph_index_ ( subgraph_index )
	{ }
	/*!
	Advance iterator to next operator
	*/
	const_random_iterator<Base>& operator++(void)
	{	++subgraph_index_;
		return *this;
	}
	/// No correction necessary when using random access to player
	void correct_before_increment(void)
	{	return; }
	/*!
	Backup iterator to previous operator
	*/
	const_random_iterator<Base>& operator--(void)
	{	--subgraph_index_;
		return *this;
	}
	/*!
	No correction necessary when using random access to player.

	\param op_arg
	not used or modified.
	*/
	void correct_after_decrement(const addr_t*& op_arg)
	{	return; }
	/*!
	\brief
	Get information corresponding to current operator.

	\param op [out]
	op code for this operator.

	\param op_arg [out]
	pointer to the first arguement to this operator.

	\param var_index [out]
	index of the last variable (primary variable) for this operator.
	If there is no primary variable for this operator, var_index
	is not sepcified and could have any value.
	*/
	void op_info(
		OpCode&        op         ,
		const addr_t*& op_arg     ,
		size_t&        var_index  ) const
	{	// op
		size_t i_op = (*subgraph_)[subgraph_index_];
		play_->random_access(i_op, op, op_arg, var_index);
	}
	/// current operator index
	size_t op_index(void)
	{	return (*subgraph_)[subgraph_index_]; }
};

} } } // BEGIN_CPPAD_LOCAL_PLAY_NAMESPACE

# endif

