# ifndef CPPAD_SUB_EQ_INCLUDED
# define CPPAD_SUB_EQ_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
-------------------------------------------------------------------------------
$begin SubEq$$
$spell
	Vec
	const
	Add
	Eq
$$

$index computed, assignment AD subtract$$
$index AD, computed assignment subtract$$
$index assignment, computed AD subtract$$
$index subtract, AD computed assignment$$
$index -=, AD computed assignment$$

$section AD Computed Assignment Subtraction Operator$$

$head Syntax$$

$syntax%%y% -= %x%$$


$head Purpose$$
Computes the difference of $italic x$$ minus $italic y$$ 
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
	%z% = %y% -= %x%
%$$
will assign the value of 
$syntax%%y% - %x%$$ to $italic z$$ 
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
	\D{[ f(x) - g(x) ]}{x} = \D{f(x)}{x} - \D{g(x)}{x}
\] $$


$head Example$$
$children%
	example/sub_eq.cpp
%$$
The file
$xref/SubEq.cpp/$$
contains an example and test of these operations.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/
//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
AD<Base>& AD<Base>::operator -= (const AD<Base> &right)
{	Base left;

	left   = value;
	value -= right.value;

	if( Parameter(*this) )
	{	if( Variable(right) )
		{	Tape()->RecordOp(SubpvOp, 
				*this, left, right.taddr
			);
		}
	}
	else if( Parameter(right) )
	{	if( ! IdenticalZero(right) )
		{	Tape()->RecordOp(SubvpOp, 
				*this, taddr, right.value
			);
		}
	}
	else	Tape()->RecordOp(SubvvOp, 
			*this, taddr, right.taddr
	);
	return *this;
}

// int, Base, and VecAD_reference<Base> cases are folded in using
// CPPAD_ASSIGN_MEMBER(-=) in the file AD.h

} // END CppAD namespace

# endif 
