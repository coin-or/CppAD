# ifndef CPPAD_LOCAL_PLAY_SUBGRAPH_ITERATOR_HPP
# define CPPAD_LOCAL_PLAY_SUBGRAPH_ITERATOR_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/local/play/random_iterator.hpp>

// BEGIN_CPPAD_LOCAL_PLAY_NAMESPACE
namespace CppAD { namespace local { namespace play {

/*!
\file random_iterator.hpp
*/

/*!
Constant subgraph iterator for a player object.

\tparam Addr
An integer type capable of representing the largest value in the vectors
arg_vec, op2arg_vec, op2var_vec, var2op_vec.

Except for constructor, the public API for this class is the same as
for the sequential iterator class.
*/
template <class Addr>
class const_subgraph_iterator {
private:
	/// a random iterator used to access player information
	const const_random_iterator<Addr>* random_itr_;

	/// sorted subset of operator indices that we will include
	const pod_vector<addr_t>* subgraph_;

	/// index in subgraph of current operator
	/// The initial value for this index must be zero or subgraph.size()-1.
	size_t subgraph_index_;

public:
	/// default constructor
	const_subgraph_iterator(void) :
	random_itr_(CPPAD_NULL) ,
	subgraph_(CPPAD_NULL)   ,
	subgraph_index_(0)
	{ }
	/// default assignment operator
	void operator=(const const_subgraph_iterator& rhs)
	{
		random_itr_      = rhs.random_itr_;
		subgraph_        = rhs.subgraph_;
		subgraph_index_  = rhs.subgraph_index_;
		return;
	}
	/*!
	Create a subgraph iterator starting either at beginning or end of subgraph
	*/
	const_subgraph_iterator(
		const const_random_iterator<Addr>&    random_itr , ///< random_itr_
		const pod_vector<addr_t>*             subgraph   , ///< subgraph_
		size_t subgraph_index                            ) ///< subgraph_index_
	:
	random_itr_      ( &random_itr )       ,
	subgraph_        ( subgraph )          ,
	subgraph_index_  ( subgraph_index )
	{	CPPAD_ASSERT_UNKNOWN(
			subgraph_index == 0 || subgraph_index == subgraph->size() - 1
		);
	}
	/*!
	Advance iterator to next operator
	*/
	const_subgraph_iterator<Addr>& operator++(void)
	{	++subgraph_index_;
		return *this;
	}
	/// No correction necessary when using random access to player
	void correct_before_increment(void)
	{	return; }
	/*!
	Backup iterator to previous operator
	*/
	const_subgraph_iterator<Addr>& operator--(void)
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
		size_t op_index = (*subgraph_)[subgraph_index_];
		random_itr_->op_info(op_index, op, op_arg, var_index);
	}
	/// current operator index
	size_t op_index(void)
	{	return (*subgraph_)[subgraph_index_]; }
};

} } } // BEGIN_CPPAD_LOCAL_PLAY_NAMESPACE

# endif
