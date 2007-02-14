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

/*
-------------------------------------------------------------------------------
$begin Add$$
$spell
	Vec
	Var
	const
	Add
$$

$index binary, AD add operator$$
$index AD, binary add operator$$
$index operator, AD binary add$$
$index add, AD binary operator$$
$index +, AD binary operator$$

$section AD Binary Addition Operator$$

$head Syntax$$

$syntax%%z% = %x% + %y%$$


$head Purpose$$
Computes the sum of $italic x$$ plus $italic y$$ 
where one of the two operands has type
$syntax%AD<%Base%>%$$ or
$syntax%VecAD<%Base%>::reference%$$.

$head x$$
The operand $italic x$$ has one of the following prototypes
$syntax%
	int       %%                    %x%
	const %Base%                   &%x%
	const AD<%Base%>               &%x%
	const VecAD<%Base%>::reference &%x%
%$$

$head y$$
The operand $italic y$$ has one of the following prototypes
$syntax%
	int       %%                    %y%
	const %Base%                   &%y%
	const AD<%Base%>               &%y%
	const VecAD<%Base%>::reference &%y%
%$$

$head z$$
The result $italic z$$ has type
$syntax%
	const AD<%Base%> %z%
%$$

$head Operation Sequence$$
This is an AD of $italic Base$$
$xref/glossary/Operation/Atomic/atomic operation/1/$$
and hence is part of the current
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$.

$head Derivative$$
If $latex f$$ and $latex g$$ are 
$xref/glossary/Base Function/Base functions/$$
$latex \[
	\D{[ f(x) + g(x) ]}{x} = \D{f(x)}{x} + \D{g(x)}{x}
\] $$

$head Example$$
$children%
	example/add.cpp
%$$
The file
$xref/Add.cpp/$$
contains an example and test of these operations.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/
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
			Tape()->RecordOp(AddvvOp, 
				result, taddr_, right.taddr_
			);
		}
		else if( IdenticalZero(right.value_) )
		{	// result = variable + 0
			result.MakeVariable(taddr_);
		}
		else
		{	// result = variable + parameter
			Tape()->RecordOp(AddvpOp, 
				result, taddr_, right.value_
			);
		}
	}
	else if( Variable(right) )
	{	if( IdenticalZero(value_) )
		{	// result = 0 + variable
			result.MakeVariable(right.taddr_);
		}
		else
		{	// result = parameter + variable
			Tape()->RecordOp(AddpvOp, 
				result, value_, right.taddr_
			);
		}
	}
	return result;
}

// convert other cases into the case above
CPPAD_FOLD_BINARY_OPERATOR(AD<Base>, +)

} // END CppAD namespace

# endif 
