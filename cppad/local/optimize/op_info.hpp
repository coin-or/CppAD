// $Id$
# ifndef CPPAD_LOCAL_OPTIMIZE_OP_INFO_HPP
# define CPPAD_LOCAL_OPTIMIZE_OP_INFO_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*!
\file op_info.hpp
Create operator information tables
*/

// BEGIN_CPPAD_LOCAL__OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize {

/// information for one operator
struct op_info {
	/// op code
	OpCode        op;

	/// arguments
	const addr_t* arg;

	/// Primary (not auxillary) variable index for this operator. If the
	// operator has not results, this is num_var (an invalid variable index).
	size_t        i_var;
};

template <class Base>
void forward_pass(
	player<Base>*          play                ,
	vector<op_info>&       op2info             ,
	vector<size_t>&        var2op              )
{
	// number of operators in the tape
	const size_t num_op = play->num_op_rec();
	//
	// number of variables in the tape
	const size_t num_var = play->num_var_rec();
	//
	// initialize mapping from variable index to operator index
	var2op.resize(num_var);
	for(size_t i = 0; i < num_var; i++)
		var2op[i] = num_op; // invalid (used for auxillary variables)
	//
	// information set by forward_user
	size_t user_index=0, user_old=0, user_m=0, user_n=0, user_i=0, user_j=0;
	enum_user_state user_state = end_user;
	//
	// information set by forward_next
	OpCode        op;     // operator
	const addr_t* arg;    // arguments
	size_t        i_op;   // operator index
	size_t        i_var;  // variable index of first result
	//
	play->forward_start(op, arg, i_op, i_var);
	CPPAD_ASSERT_UNKNOWN( op              == BeginOp );
	CPPAD_ASSERT_UNKNOWN( NumRes(BeginOp) == 1 );
	CPPAD_ASSERT_UNKNOWN( i_op            == 0 );
	CPPAD_ASSERT_UNKNOWN( i_var           == 0 );
	op2info[i_op].op    = op;
	op2info[i_op].arg   = arg;
	op2info[i_op].i_var = i_var;
	//
	// This variaible index, 0, is automatically created, but not used because
	// it represents a paraemeter during the recording process. So we set
	var2op[i_var] = num_op;
	//
	while(op != EndOp)
	{	// next operator
		play->reverse_next(op, arg, i_op, i_var);
		//
		// information for this operator
		op2info[i_op].op    = op;
		op2info[i_op].arg   = arg;
		op2info[i_op].i_var = i_var;
		//
		// mapping from variable index to operator index
		if( NumRes(op) > 0 )
			var2op[i_var] = i_op;
		//
		switch( op )
		{	case CSumOp:
			// must correct arg before next operator
			play->forward_csum(op, arg, i_op, i_var);
			break;

			case CSkipOp:
			// must correct arg before next operator
			play->forward_csum(op, arg, i_op, i_var);
			break;

			case UserOp:
			case UsrapOp:
			case UsravOp:
			case UsrrpOp:
			case UsrrvOp:
			play->forward_user(op, user_state,
				user_index, user_old, user_m, user_n, user_i, user_j
			);
			break;

			default:
			break;
		}
	}
	return;
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
