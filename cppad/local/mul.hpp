# ifndef CPPAD_MUL_INCLUDED
# define CPPAD_MUL_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
AD<Base> operator * (const AD<Base> &left , const AD<Base> &right)
{	ADTape<Base> *tape = AD<Base>::tape_ptr();
	bool var_left, var_right;
# ifdef NDEBUG
	if( tape == CPPAD_NULL )
	{	var_left =  false;
		var_right = false;
	}
	else
	{
		var_left  = left.id_  == tape->id_;
		var_right = right.id_ == tape->id_;
	}
# else
	var_left  = Variable(left);
	var_right = Variable(right);
	CPPAD_ASSERT_KNOWN(
		(! var_left) || left.id_ == tape->id_ ,
		"* left operand is a variable for a different thread"
	);
	CPPAD_ASSERT_KNOWN(
		(! var_right) || right.id_ == tape->id_ ,
		"* right operand is a variable for a different thread"
	);
# endif

	AD<Base> result;
	result.value_  = left.value_ * right.value_;
	CPPAD_ASSERT_UNKNOWN( Parameter(result) );

	if( var_left )
	{	if( var_right )
		{	// result = variable * variable
			CPPAD_ASSERT_UNKNOWN( NumVar(MulvvOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( NumInd(MulvvOp) == 2 );

			// put operand addresses in tape
			tape->Rec_.PutInd(left.taddr_, right.taddr_);
			// put operator in the tape
			result.taddr_ = tape->Rec_.PutOp(MulvvOp);
			// make result a variable
			result.id_ = tape->id_;
		}
		else if( IdenticalZero(right.value_) )
		{	// result = variable * 0
		}
		else if( IdenticalOne(right.value_) )
		{	// result = variable * 1
			result.make_variable(left.id_, left.taddr_);
		}
		else
		{	// result = variable * parameter
			CPPAD_ASSERT_UNKNOWN( NumVar(MulvpOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( NumInd(MulvpOp) == 2 );

			// put operand addresses in tape
			size_t p = tape->Rec_.PutPar(right.value_);
			tape->Rec_.PutInd(left.taddr_, p);
			// put operator in the tape
			result.taddr_ = tape->Rec_.PutOp(MulvpOp);
			// make result a variable
			result.id_ = tape->id_;
		}
	}
	else if( var_right )
	{	if( IdenticalZero(left.value_) )
		{	// result = 0 * variable
		}
		else if( IdenticalOne(left.value_) )
		{	// result = 1 * variable
			result.make_variable(right.id_, right.taddr_);
		}
		else
		{	// result = parameter * variable
			CPPAD_ASSERT_UNKNOWN( NumVar(MulpvOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( NumInd(MulpvOp) == 2 );

			// put operand addresses in tape
			size_t p = tape->Rec_.PutPar(left.value_);
			tape->Rec_.PutInd(p, right.taddr_);
			// put operator in the tape
			result.taddr_ = tape->Rec_.PutOp(MulpvOp);
			// make result a variable
			result.id_ = tape->id_;
		}
	}
	return result;
}

// convert other cases into the case above
CPPAD_FOLD_AD_VALUED_BINARY_OPERATOR(*)

} // END CppAD namespace

# endif 
