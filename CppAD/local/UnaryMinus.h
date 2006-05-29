# ifndef CppADUnaryMinusIncluded
# define CppADUnaryMinusIncluded

/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */

/*
$begin UnaryMinus$$
$spell
	Vec
	const
	inline
$$

$index unary, AD minus operator$$
$index AD, unary minus operator$$
$index minus, AD unary operator$$
$index -, AD unary operator$$

$section AD Unary Minus Operator$$

$head Syntax$$

$syntax%%y% = - %x%$$


$head Purpose$$
Computes the negative of $italic x$$.

$head Base$$
The operation in the syntax above must be supported for the case where
the operand is a $code const$$ $italic Base$$ object.

$head x$$
The operand $italic x$$ has one of the following prototypes
$syntax%
	const AD<%Base%>               &%x%
	const VecAD<%Base%>::reference &%x%
%$$

$head y$$
The result $italic y$$ has type
$syntax%
	AD<%Base%> %y%
%$$
It is equal to the negative of the operand $italic x$$.

$head Operation Sequence$$
This is an AD of $italic Base$$
$xref/glossary/Operation/Atomic/atomic operation/1/$$
and hence is part of the current
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$.

$head Derivative$$
If $latex f$$ is a 
$xref/glossary/Base Function/Base function/$$,
$latex \[
	\D{[ - f(x) ]}{x} = - \D{f(x)}{x}
\] $$

$head Example$$
$children%
	Example/UnaryMinus.cpp
%$$
The file
$xref/UnaryMinus.cpp/$$
contains an example and test of this operation.

$end
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

// Broken g++ compiler inhibits declaring unary minus a member or friend
template <class Base>
inline AD<Base> AD<Base>::operator - (void) const 
{	// should make a more efficient version by adding unary minus to
	// Operator.h (some day)
	AD<Base> result(0);

	result  -= *this;

	return result;
}


template <class Base>
inline AD<Base> operator - (const VecAD_reference<Base> &right) 
{	return - right.ADBase(); }

}
//  END CppAD namespace


# endif
