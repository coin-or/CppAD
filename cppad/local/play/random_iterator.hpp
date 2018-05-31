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

\tparam Base
This operation sequence was recorded using type AD<Base>.

\tparam Addr
An integer type capable of representing the largest value in the vectors
arg_vec, op2arg_vec, op2var_vec, var2op_vec.
*/
template <class Base, class Addr>
class const_random_iterator {
private:
	/// vector of operators on the tape
	const pod_vector<CPPAD_OP_CODE_TYPE>* op_vec_;

	/// vector of arguments for all the operators
	const pod_vector<addr_t>* arg_vec_;

	/// mapping from operator index to index of first argument in arg_vec_
	const pod_vector<Addr>* op2arg_vec_;

	/// mapping from operator index to index of primary (last) result
	const pod_vector<Addr>* op2var_vec_;

public:
	/// default constructor
	const_random_iterator(void) :
	op_vec_(CPPAD_NULL)     ,
	arg_vec_(CPPAD_NULL)    ,
	op2arg_vec_(CPPAD_NULL) ,
	op2var_vec_(CPPAD_NULL)
	{ }
	/// default assignment operator
	void operator=(const const_random_iterator& rhs)
	{
		op_vec_          = rhs.op_vec_;
		op2arg_vec_      = rhs.op2arg_vec_;
		op2var_vec_      = rhs.op2var_vec_;
		return;
	}
	/*!
	Create a random iterator starting either at beginning or end of subgraph
	*/
	const_random_iterator(
		const pod_vector<CPPAD_OP_CODE_TYPE>* op_vec     , ///< op_vec_
		const pod_vector<addr_t>*             arg_vec    , ///< arg_vec_
		const pod_vector<addr_t>*             op2arg_vec , ///< op2ar_vec_
		const pod_vector<addr_t>*             op2var_vec ) ///< op2var_vec_
	:
	op_vec_          ( op_vec )                                    ,
	arg_vec_         ( arg_vec )                                   ,
	op2arg_vec_      ( op2arg_vec->pod_vector_ptr<Addr>() )         ,
	op2var_vec_      ( op2var_vec->pod_vector_ptr<Addr>() )
	{ }
	/*!
	\brief
	fetch the information corresponding to an operator

	\param op_index
	index for this operator [in]

	\param op [out]
	op code for this operator.

	\param op_arg [out]
	pointer to the first arguement to this operator.

	\param var_index [out]
	index of the last variable (primary variable) for this operator.
	If there is no primary variable for this operator, i_var not sepcified
	and could have any value.
	*/
	void op_info(
		size_t         op_index   ,
		OpCode&        op         ,
		const addr_t*& op_arg     ,
		size_t&        var_index  ) const
	{	op        = OpCode( (*op_vec_)[op_index] );
		op_arg    = (*op2arg_vec_)[op_index] + arg_vec_->data();
		var_index = (*op2var_vec_)[op_index];
		return;
	}
};

} } } // BEGIN_CPPAD_LOCAL_PLAY_NAMESPACE

# endif
