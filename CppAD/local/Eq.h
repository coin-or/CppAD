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
	Vec
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

$head Syntax$$
$syntax%%y% = %x%$$



$head Purpose$$
Assigns a new value to the AD object $italic y$$.
The object $italic y$$ will have the same dependence on the 
$xref/glossary/Independent Variable/independent variables/$$ as $italic x$$
($italic y$$ is a 
$xref/glossary/Variable/variable/$$ if and only if $italic x$$ is a variable).

$head x$$
The operand $italic x$$ has one of the following prototypes
$syntax%
	const int %%                   &%x%
	const %Base%                   &%x%
	const AD<%Base%>               &%x%
	const VecAD<%Base%>::reference &%x%
%$$

$head y$$
This operation is a member function for $italic y$$ 
which has the one of the following prototypes
$syntax%
	AD<%Base%>               %y%
	VecAD<%Base%>::reference %y%
%$$

$head Result$$

$subhead AD<Base>$$
$index assignment, multiple$$
$index multiple, assignment$$
If $italic y$$ has type $syntax%AD<%Base%>%$$,
the result of this assignment 
can be used as a reference to $italic y$$.
For example, if $italic z$$ has the following type
$syntax%
	AD<%Base%> %z%
%$$
then the syntax
$syntax%
	%z% = %y% = %x%
%$$
will assign the value of $italic x$$ to $italic z$$ 
(as well as to $italic y$$).

$subhead VecAD<Base>::reference$$
If $italic y$$ has type $syntax%VecAD<%Base%>::reference%$$,
this assignment has a $code void$$ result.
For example, the following syntax is not valid:
$syntax%
	%z% = %y% = %x%
%$$

$head Operation Sequence$$
This is an AD of $italic Base$$
$xref/glossary/Operation/Atomic/atomic operation/1/$$
and hence is part of the current
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$.

$head Example$$
$children%
	Example/Eq.cpp
%$$
The file
$xref/Eq.cpp/$$
contains an example and test of these operations.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/

namespace CppAD { //  BEGIN CppAD namespace

template <class Base>
inline AD<Base>& AD<Base>::operator=(const AD<Base> &right)
{	value   = right.value;
	id      = right.id;
	taddr   = right.taddr;

	// check that all variables are parameters while tape is empty
	CppADUnknownError(
		Parameter(*this) | (Tape()->State() != Empty)
	);

	return *this;
}


} // END CppAD namespace

# endif
