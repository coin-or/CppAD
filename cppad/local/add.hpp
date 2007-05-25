# ifndef CPPAD_ADD_INCLUDED
# define CPPAD_ADD_INCLUDED

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
AD<Base> AD<Base>::operator +(const AD<Base> &right) const
{
	AD<Base> result;
	CppADUnknownError( Parameter(result) );

	result.value_  = value_ + right.value_;

	if( Variable(*this) )
	{	if( Variable(right) )
		{	// result = variable + variable
			CppADUsageError(
				id_ == right.id_,
				"Adding AD objects that are"
				" variables on different tapes."
			);
			tape_this()->RecordOp(AddvvOp, 
				result, taddr_, right.taddr_
			);
		}
		else if( IdenticalZero(right.value_) )
		{	// result = variable + 0
			result.make_variable(id_, taddr_);
		}
		else
		{	// result = variable + parameter
			tape_this()->RecordOp(AddvpOp, 
				result, taddr_, right.value_
			);
		}
	}
	else if( Variable(right) )
	{	if( IdenticalZero(value_) )
		{	// result = 0 + variable
			result.make_variable(right.id_, right.taddr_);
		}
		else
		{	// result = parameter + variable
			right.tape_this()->RecordOp(AddpvOp, 
				result, value_, right.taddr_
			);
		}
	}
	return result;
}

// convert other cases into the case above
CPPAD_FOLD_AD_VALUED_BINARY_OPERATOR(+)

} // END CppAD namespace

# endif 
