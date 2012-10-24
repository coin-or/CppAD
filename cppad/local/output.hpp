/* $Id$ */
# ifndef CPPAD_OUTPUT_INCLUDED
# define CPPAD_OUTPUT_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Output$$
$spell
	VecAD
	std
	ostream
	const
$$

$index <<, AD output$$
$index AD, stream output$$
$index output, AD$$
$index stream, AD output$$
$index write, AD$$

$section AD Output Stream Operator$$ 

$head Syntax$$
$icode%os% << %x%$$


$head Purpose$$
Writes the $icode Base$$ value_, corresponding to $icode x$$,
to the output stream $icode os$$.

$head os$$
The operand $icode os$$ has prototype
$codei%
	std::ostream &%os%
%$$

$head x$$
The operand $icode x$$ has one of the following prototypes
$codei%
	const AD<%Base%>               &%x%
	const VecAD<%Base%>::reference &%x%
%$$

$head Result$$
The result of this operation can be used as a reference to $icode os$$.
For example, if the operand $icode y$$ has prototype
$codei%
	AD<%Base%> %y%
%$$
then the syntax
$codei%
	%os% << %x% << %y%
%$$
will output the value corresponding to $icode x$$
followed by the value corresponding to $icode y$$. 

$head Operation Sequence$$
The result of this operation is not an
$cref/AD of Base/glossary/AD of Base/$$ object.
Thus it will not be recorded as part of an
AD of $icode Base$$
$cref/operation sequence/glossary/Operation/Sequence/$$.

$head Example$$
$children%
	example/output.cpp
%$$
The file
$cref output.cpp$$
contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
------------------------------------------------------------------------------
*/

namespace CppAD { 

	template <class Base>
	CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
	std::ostream& operator << 
	(std::ostream &os, const AD<Base> &x)
	{ 	return (os << x.value_); }

	template <class Base>
	CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
	std::ostream& operator << 
	(std::ostream &os, const VecAD_reference<Base> &x)
	{ 	return (os << x.ADBase()); }

}

# endif
