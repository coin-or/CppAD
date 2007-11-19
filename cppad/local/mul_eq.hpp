# ifndef CPPAD_MUL_EQ_INCLUDED
# define CPPAD_MUL_EQ_INCLUDED

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
AD<Base>& AD<Base>::operator *= (const AD<Base> &right)
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
		"*= left operand is a variable for a different thread"
	);
	CPPAD_ASSERT_KNOWN(
		(! var_right) || right.id_ == tape->id_ ,
		"*= right operand is a variable for a different thread"
	);
# endif
	Base left;
	left    = value_;
	value_ *= right.value_;

	if( var_left )
	{	if( var_right )
		{	// this = variable * variable
			CPPAD_ASSERT_UNKNOWN( NumVar(MulvvOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( NumInd(MulvvOp) == 2 );

			// put operand addresses in tape
			tape->Rec_.PutInd(taddr_, right.taddr_);
			// put operator in the tape
			taddr_ = tape->Rec_.PutOp(MulvvOp);
			// make this a variable
			CPPAD_ASSERT_UNKNOWN( id_ = tape->id_ );
		}
		else if( IdenticalOne( right.value_ ) )
		{	// this = variable * 1
		}
		else if( IdenticalZero( right.value_ ) )
		{	// this = variable * 0
			make_parameter();
		}
		else
		{	// this = variable * parameter
			CPPAD_ASSERT_UNKNOWN( NumVar(MulvpOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( NumInd(MulvpOp) == 2 );

			// put operand addresses in tape
			size_t p = tape->Rec_.PutPar(right.value_);
			tape->Rec_.PutInd(taddr_, p);
			// put operator in the tape
			taddr_ = tape->Rec_.PutOp(MulvpOp);
			// make this a variable
			CPPAD_ASSERT_UNKNOWN( id_ == tape->id_);
		}
	}
	else if( var_right  )
	{	if( IdenticalZero(left) )
		{	// this = 0 * right
		}
		else if( IdenticalOne(left) )
		{	// this = 1 * right
			make_variable(right.id_, right.taddr_);
		}
		else
		{	// this = parameter * variable
			CPPAD_ASSERT_UNKNOWN( NumVar(MulpvOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( NumInd(MulpvOp) == 2 );

			// put operand addresses in tape
			size_t p = tape->Rec_.PutPar(left);
			tape->Rec_.PutInd(p, right.taddr_);
			// put operator in the tape
			taddr_ = tape->Rec_.PutOp(MulpvOp);
			// make this a variable
			id_ = tape->id_;
		}
	}
	return *this;
}

CPPAD_FOLD_ASSIGNMENT_OPERATOR(*=)

} // END CppAD namespace

# endif 
