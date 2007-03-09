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

/*
-------------------------------------------------------------------------------
$begin MulEq$$
$spell
	Vec
	const
	Add
	Eq
$$

$index computed, assignment AD multiply$$
$index AD, computed assignment multiply$$
$index assignment, computed AD multiply$$
$index multiply, AD computed assignment$$
$index *=, AD computed assignment$$

$section AD Computed Assignment Multiplication Operator$$

$head Syntax$$

$syntax%%y% *= %x%$$


$head Purpose$$
Computes the product of $italic x$$ times $italic y$$ 
and places the result in 
$italic y$$ where $italic y$$ is a $syntax%AD<%Base%>%$$ object.

$head x$$
The operand $italic x$$ has one of the following prototypes
$syntax%
	int       %%                    %x%
	const %Base%                   &%x%
	const AD<%Base%>               &%x%
	const VecAD<%Base%>::reference &%x%
%$$

$head y$$
This operation is a member function of $italic y$$
which has the following type
$syntax%
        AD<%Base%> %y%
%$$

$head Result$$
The result of this assignment
can be used as a reference to $italic y$$.
For example, if the operand $italic z$$ has the following prototype
$syntax%
	AD<%Base%> %z%
%$$
then the syntax
$syntax%
	%z% = %y% *= %x%
%$$
will assign the value of 
$syntax%%y% * %x%$$ to $italic z$$ 
(as well as to $italic y$$).

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
	\D{[ f(x) * g(x) ]}{x} = g(x) * \D{f(x)}{x} + f(x) * \D{g(x)}{x}
\] $$


$head Example$$
$children%
	example/mul_eq.cpp
%$$
The file
$xref/MulEq.cpp/$$
contains an example and test of these operations.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/
//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
AD<Base>& AD<Base>::operator *= (const AD<Base> &right)
{	Base left;

	left   = value_;
	value_ *= right.value_;

	if( Parameter(*this) )
	{	if( Variable(right) )
		{	if( ! IdenticalZero(left) )
			{	if( IdenticalOne(left) )
				{	// z = 1 * right
					make_variable(right.id_, right.taddr_);
				}
				else	right.tape_this()->RecordOp(
						MulpvOp, 
						*this, 
						left, 
						right.taddr_
				);
			}
		}
	}
	else if( Parameter(right) )
	{	if( ! IdenticalOne(right.value_) )
		{	if( IdenticalZero(right.value_) )
				make_parameter();
			else	tape_this()->RecordOp(MulvpOp, 
					*this, taddr_, right.value_
			);
		}
	}
	else
	{	CppADUsageError(
			id_ == right.id_,
			"Multiplying AD objects that are"
			" variables on different tapes."
		);
		tape_this()->RecordOp(MulvvOp, 
			*this, taddr_, right.taddr_
		);
	}
	return *this;
}

// int, Base, and VecAD_reference<Base> cases are folded in using
// CPPAD_ASSIGN_MEMBER(*=) in the file AD.h

} // END CppAD namespace

# endif 
