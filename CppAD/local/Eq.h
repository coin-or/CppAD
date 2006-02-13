# ifndef CppADEqIncluded
# define CppADEqIncluded

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
-------------------------------------------------------------------------------
$begin Eq$$
$spell
	Var
	const
	inline
$$

$index =, AD$$
$index AD, =$$

$index AD, assignment operator$$
$index operator, AD assignment$$
$index assignment, AD operator$$

$section AD Assignment Operator$$

$table
$bold Syntax$$ $cnext 
$syntax% %y% = %x%$$
$tend

$fend 20$$


$head Purpose$$
Assigns a new value to the AD object $italic y$$.
The object $italic y$$ will have the same dependence on the 
$xref/glossary/Independent Variable/independent variables/$$ as $italic x$$
($italic y$$ is a 
$xref/glossary/Variable/variable/$$ if and only if $italic x$$ is a variable).

$head x$$
The operand $italic x$$ has prototype
$syntax%
	const %Type%     &%x%
%$$
where $italic Type$$ is $syntax%const AD<%Base%>%$$,
$italic Base$$ or any type that can be implicitly converted to $italic Base$$.

$head y$$
The operand $italic y$$ has the following prototype
$syntax%
	AD<%Base%> &%y%
%$$

$head Multiple Assignments$$
$index assignment, multiple$$
$index multiple, assignment$$
It is possible to use the result of this assignment operator 
in multiple assignment.
For example, if the operand $italic z$$ has the following prototype
$syntax%
	AD<%Base%> &%z%
%$$
then the syntax
$syntax%
	%z% = %y% = %x%
%$$
will assign the value of $italic x$$ to $italic z$$ 
(as well as to $italic y$$).

$head Example$$
$children%
	Example/Eq.cpp
%$$
The file
$xref/Eq.cpp/$$
contains an example and a test of these operations.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
inline AD<Base>& AD<Base>::operator=(const AD<Base> &right)
{
	// check that all variables are parameters while tape is empty
	CppADUnknownError(
		Parameter(*this) | (Tape()->State() != Empty)
	);

	value   = right.value;
	id      = right.id;
	taddr   = right.taddr;

	return *this;
}


} // END CppAD namespace


# endif
