
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
template <class Base, class Addr>
class const_random_iterator {
private:
	/// sorted subset of operator indices that we will include
	const pod_vector<addr_t>* subgraph_;

	/// vector of operators on the tape
	const pod_vector<CPPAD_OP_CODE_TYPE>* op_vec_;

	/// vector of arguments for all the operators
	const pod_vector<addr_t>* arg_vec_;

	/// mapping from operator index to index of first argument in arg_vec_
	const pod_vector<Addr>* op2arg_vec_;

	/// mapping from operator index to index of primary (last) result
	const pod_vector<Addr>* op2var_vec_;

	/// mapping from primary results to correspinding operator indices
	const pod_vector<Addr>* var2op_vec_;

	/// index in subgraph of current operator
	size_t subgraph_index_;

public:
	/// default assignment operator
	void operator=(const const_random_iterator& rhs)
	{
		subgraph_        = rhs.subgraph_;
		op_vec_          = rhs.op_vec_;
		op2arg_vec_      = rhs.op2arg_vec_;
		op2var_vec_      = rhs.op2var_vec_;
		var2op_vec_      = rhs.var2op_vec_;
		subgraph_index_  = rhs.subgraph_index_;
		return;
	}
	/*!
	Create a random iterator starting either at beginning or end of subgraph
	*/
	const_random_iterator(
		const pod_vector<addr_t>*             subgraph   , ///< subgraph_
		const pod_vector<CPPAD_OP_CODE_TYPE>* op_vec     , ///< op_vec_
		const pod_vector<addr_t>*             arg_vec    , ///< arg_vec_
		const pod_vector<addr_t>*             op2arg_vec , ///< op2ar_vec_
		const pod_vector<addr_t>*             op2var_vec , ///< op2var_vec_
		const pod_vector<addr_t>*             var2op_vec , ///< var2op_vec_
		size_t subgraph_index                            ) ///< subgraph_index_
	:
	subgraph_        ( subgraph )                                  ,
	op_vec_          ( op_vec )                                    ,
	arg_vec_         ( arg_vec )                                   ,
	op2arg_vec_      ( op2arg_vec->pod_vector_ptr<Addr>() )         ,
	op2var_vec_      ( op2var_vec->pod_vector_ptr<Addr>() )         ,
	var2op_vec_      ( var2op_vec->pod_vector_ptr<Addr>() )         ,
	subgraph_index_  ( subgraph_index )
	{ }
	/*!
	Advance iterator to next operator
	*/
	const_random_iterator<Base, Addr>& operator++(void)
	{	++subgraph_index_;
		return *this;
	}
	/// No correction necessary when using random access to player
	void correct_before_increment(void)
	{	return; }
	/*!
	Backup iterator to previous operator
	*/
	const_random_iterator<Base, Addr>& operator--(void)
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
		op              = OpCode( (*op_vec_)[op_index] );
		op_arg          = (*op2arg_vec_)[op_index] + arg_vec_->data();
		var_index       = (*op2var_vec_)[op_index];
	}
	/// current operator index
	size_t op_index(void)
	{	return (*subgraph_)[subgraph_index_]; }
	/*!
	\brief
	Unpack extra information when current op is a UserOp

	\param op [in]
	must be a UserOp

	\param op_arg [in]
	is the arguments for this operator

	\param user_old [out]
	is the extra information passed to the old style user atomic functions.

	\param user_m [out]
	is the number of results for this user atmoic function.

	\param user_n [out]
	is the number of arguments for this user atmoic function.

	\return
	is a pointer to this user atomic function.
	*/
	atomic_base<Base>* user_info(
		const OpCode     op         ,
		const addr_t*    op_arg     ,
		size_t&          user_old   ,
		size_t&          user_m     ,
		size_t&          user_n     ) const
	{	return player<Base>::get_user_info(
			op, op_arg, user_old, user_m, user_n
		);
	}
};
} } } // BEGIN_CPPAD_LOCAL_PLAY_NAMESPACE

# endif

