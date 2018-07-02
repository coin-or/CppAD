# ifndef CPPAD_CORE_MUL_HPP
# define CPPAD_CORE_MUL_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
AD<Base> operator * (const AD<Base> &left , const AD<Base> &right)
{
	// compute the Base part
	AD<Base> result;
	result.value_  = left.value_ * right.value_;
	CPPAD_ASSERT_UNKNOWN( Parameter(result) );

	// check if there is a recording in progress
	local::ADTape<Base>* tape = AD<Base>::tape_ptr();
	if( tape == CPPAD_NULL )
		return result;
	tape_id_t tape_id = tape->id_;
	// tape_id cannot match the default value for tape_id_; i.e., 0
	CPPAD_ASSERT_UNKNOWN( tape_id > 0 );

	// check if left and right tapes match
	bool match_left  = left.tape_id_  == tape_id;
	bool match_right = right.tape_id_ == tape_id;

	// check if left and right are dynamic parameters
	bool dyn_left  = match_left  & left.dynamic_;
	bool dyn_right = match_right & right.dynamic_;

	// check if left and right are  variables
	bool var_left  = match_left  & (! left.dynamic_);
	bool var_right = match_right & (! right.dynamic_);

	if( var_left )
	{	if( var_right )
		{	// result = variable * variable
			CPPAD_ASSERT_UNKNOWN( local::NumRes(local::MulvvOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( local::NumArg(local::MulvvOp) == 2 );

			// put operand addresses in tape
			tape->Rec_.PutArg(left.taddr_, right.taddr_);
			// put operator in the tape
			result.taddr_ = tape->Rec_.PutOp(local::MulvvOp);
			// make result a variable
			result.tape_id_ = tape_id;
		}
		else if( IdenticalZero(right.value_) )
		{	// result = variable * 0
		}
		else if( IdenticalOne(right.value_) )
		{	// result = variable * 1
			result.make_variable(left.tape_id_, left.taddr_);
		}
		else
		{	// result = variable * parameter
			CPPAD_ASSERT_UNKNOWN( local::NumRes(local::MulpvOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( local::NumArg(local::MulpvOp) == 2 );

			// put operand addresses in tape
			addr_t p = right.taddr_;
			if( ! dyn_right )
				p = tape->Rec_.PutPar(right.value_);
			tape->Rec_.PutArg(p, left.taddr_);
			// put operator in the tape
			result.taddr_ = tape->Rec_.PutOp(local::MulpvOp);
			// make result a variable
			result.tape_id_ = tape_id;
		}
	}
	else if( var_right )
	{	if( IdenticalZero(left.value_) )
		{	// result = 0 * variable
		}
		else if( IdenticalOne(left.value_) )
		{	// result = 1 * variable
			result.make_variable(right.tape_id_, right.taddr_);
		}
		else
		{	// result = parameter * variable
			CPPAD_ASSERT_UNKNOWN( local::NumRes(local::MulpvOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( local::NumArg(local::MulpvOp) == 2 );

			// put operand addresses in tape
			addr_t p = left.taddr_;
			if( ! dyn_left )
				p = tape->Rec_.PutPar(left.value_);
			tape->Rec_.PutArg(p, right.taddr_);
			// put operator in the tape
			result.taddr_ = tape->Rec_.PutOp(local::MulpvOp);
			// make result a variable
			result.tape_id_ = tape_id;
		}
	}
	else
	{	CPPAD_ASSERT_KNOWN( ! (dyn_left | dyn_right) ,
		"binary *: one operand is a dynamic parameter and other not a variable"
		);
	}
	return result;
}

// convert other cases into the case above
CPPAD_FOLD_AD_VALUED_BINARY_OPERATOR(*)

} // END CppAD namespace

# endif
