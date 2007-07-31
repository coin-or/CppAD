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
{	Base left;

	left   = value_;
	value_ += right.value_;

	if( Parameter(*this) )
	{	if( Variable(right) )
		{	if( IdenticalZero(left) )
			{	// z = 0 + right
				make_variable(right.id_, right.taddr_);
			}
			else	right.tape_this()->RecordOp(AddpvOp, 
					*this, left, right.taddr_
			);
		}
	}
	else if( Parameter(right) )
	{	if( ! IdenticalZero( right.value_ ) )
		{	tape_this()->RecordOp(AddvpOp, 
				*this, taddr_, right.value_
			);
		}
	}
	else
	{	CPPAD_ASSERT_KNOWN(
			id_ == right.id_,
			"Adding AD objects that are"
			" variables on different tapes."
		);
		tape_this()->RecordOp(AddvvOp, 
				*this, taddr_, right.taddr_
		);
	}
	return *this;
}

CPPAD_FOLD_ASSIGNMENT_OPERATOR(+=)

} // END CppAD namespace

# endif 
