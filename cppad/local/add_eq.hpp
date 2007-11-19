# ifndef CPPAD_ADD_EQ_INCLUDED
# define CPPAD_ADD_EQ_INCLUDED

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
AD<Base>& AD<Base>::operator += (const AD<Base> &right)
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
		"+= left operand is a variable for a different thread"
	);
	CPPAD_ASSERT_KNOWN(
		(! var_right) || right.id_ == tape->id_ ,
		"+= right operand is a variable for a different thread"
	);
# endif
	Base left;
	left    = value_;
	value_ += right.value_;

	if( var_left )
	{	if( var_right )
		{	// this = variable + variable
			CPPAD_ASSERT_UNKNOWN( NumVar(AddvvOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( NumInd(AddvvOp) == 2 );

			// put operand addresses in tape
			tape->Rec_.PutInd(taddr_, right.taddr_);
			// put operator in the tape
			taddr_ = tape->Rec_.PutOp(AddvvOp);
			// make this a variable
			CPPAD_ASSERT_UNKNOWN( id_ = tape->id_ );
		}
		else if( ! IdenticalZero( right.value_ ) )
		{	// this = variable + parameter
			CPPAD_ASSERT_UNKNOWN( NumVar(AddvpOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( NumInd(AddvpOp) == 2 );

			// put operand addresses in tape
			size_t p = tape->Rec_.PutPar(right.value_);
			tape->Rec_.PutInd(taddr_, p);
			// put operator in the tape
			taddr_ = tape->Rec_.PutOp(AddvpOp);
			// make this a variable
			CPPAD_ASSERT_UNKNOWN( id_ == tape->id_);
		}
	}
	else if( var_right  )
	{	if( IdenticalZero(left) )
		{	// this = 0 + right
			make_variable(right.id_, right.taddr_);
		}
		else
		{	// this = parameter + variable
			CPPAD_ASSERT_UNKNOWN( NumVar(AddpvOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( NumInd(AddpvOp) == 2 );

			// put operand addresses in tape
			size_t p = tape->Rec_.PutPar(left);
			tape->Rec_.PutInd(p, right.taddr_);
			// put operator in the tape
			taddr_ = tape->Rec_.PutOp(AddpvOp);
			// make this a variable
			id_ = tape->id_;
		}
	}
	return *this;
}

CPPAD_FOLD_ASSIGNMENT_OPERATOR(+=)

} // END CppAD namespace

# endif 
