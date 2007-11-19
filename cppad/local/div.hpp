# ifndef CPPAD_DIV_INCLUDED
# define CPPAD_DIV_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
AD<Base> AD<Base>::operator /(const AD<Base> &right) const
{	ADTape<Base> *tape = tape_ptr();
	bool var_left, var_right;
# ifdef NDEBUG
	if( tape == CPPAD_NULL )
	{	var_left =  false;
		var_right = false;
	}
	else
	{
		var_left  = id_       == tape->id_;
		var_right = right.id_ == tape->id_;
	}
# else
	var_left  = Variable(*this);
	var_right = Variable(right);
	CPPAD_ASSERT_KNOWN(
		(! var_left) || id_ == tape->id_ ,
		"/ left operand is a variable for a different thread"
	);
	CPPAD_ASSERT_KNOWN(
		(! var_right) || right.id_ == tape->id_ ,
		"/ right operand is a variable for a different thread"
	);
# endif

	AD<Base> result;
	result.value_  = value_ / right.value_;
	CPPAD_ASSERT_UNKNOWN( Parameter(result) );

	if( var_left )
	{	if( var_right )
		{	// result = variable / variable
			CPPAD_ASSERT_KNOWN(
				id_ == right.id_,
				"Dividing AD objects that are"
				" variables on different tapes."
			);
			CPPAD_ASSERT_UNKNOWN( NumVar(DivvvOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( NumInd(DivvvOp) == 2 );

			// put operand addresses in tape
			tape->Rec_.PutInd(taddr_, right.taddr_);
			// put operator in the tape
			result.taddr_ = tape->Rec_.PutOp(DivvvOp);
			// make result a variable
			result.id_ = tape->id_;
		}
		else if( IdenticalOne(right.value_) )
		{	// result = variable / 1
			result.make_variable(id_, taddr_);
		}
		else
		{	// result = variable / parameter
			CPPAD_ASSERT_UNKNOWN( NumVar(DivvpOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( NumInd(DivvpOp) == 2 );

			// put operand addresses in tape
			size_t p = tape->Rec_.PutPar(right.value_);
			tape->Rec_.PutInd(taddr_, p);
			// put operator in the tape
			result.taddr_ = tape->Rec_.PutOp(DivvpOp);
			// make result a variable
			result.id_ = tape->id_;
		}
	}
	else if( Variable(right) )
	{	if( IdenticalZero(value_) )
		{	// result = 0 / variable
		}
		else
		{	// result = parameter / variable
			CPPAD_ASSERT_UNKNOWN( NumVar(DivpvOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( NumInd(DivpvOp) == 2 );

			// put operand addresses in tape
			size_t p = tape->Rec_.PutPar(value_);
			tape->Rec_.PutInd(p, right.taddr_);
			// put operator in the tape
			result.taddr_ = tape->Rec_.PutOp(DivpvOp);
			// make result a variable
			result.id_ = tape->id_;
		}
	}
	return result;
}

// convert other cases into the case above
CPPAD_FOLD_AD_VALUED_BINARY_OPERATOR(/)


} // END CppAD namespace

# endif 
