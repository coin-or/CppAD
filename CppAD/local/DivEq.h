# ifndef CppADDivEqIncluded
# define CppADDivEqIncluded

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
$begin DivEq$$
$spell
	Var
	const
	Add
	Eq
$$

$index computed, assignment AD divide$$
$index AD, computed assignment divide$$
$index assignment, computed AD divide$$
$index divide, AD computed assignment$$
$index /=, AD computed assignment$$

$section AD Computed Assignment Division Operator$$

$table
$bold Syntax$$ 
$cnext 
$syntax% %y% /= %x%$$
$tend

$fend 20$$

$head Purpose$$
Computes the quotient of $italic x$$ divided by $italic y$$ 
and places the result in 
$italic y$$ where $italic y$$ is a $syntax%AD<%Base%>%$$ object.

$head x$$
The operand $italic x$$ has prototype
$syntax%
        const %Type% &%x%
%$$
where $italic Type$$ is $syntax%AD<%Base%>%$$, $italic Base$$, or $code int$$.

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
	%z% = %y% /= %x%
%$$
will assign the value of 
$syntax%%y% / %x%$$ to $italic z$$ 
(as well as to $italic y$$).

$head Assumptions$$
If the $code /=$$ operator is used with an 
$syntax%AD<%Base%>%$$ object,
it must be defined
for the type $italic Base$$.
In addition,
if $latex f$$ and $latex g$$ are 
$xref/glossary/Base Function/Base functions/$$
$latex \[
	\D{[ f(x) * g(x) ]}{x} = 
		[1/g(x)] * \D{f(x)}{x} - [f(x)/g(x)^2] * \D{g(x)}{x}
\] $$



$head Example$$
$children%
	Example/DivEq.cpp
%$$
The file
$xref/DivEq.cpp/$$
contains an example and test of these operations.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/
//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
AD<Base>& AD<Base>::operator /= (const AD<Base> &right)
{	Base left;

	left   = value;
	value /= right.value;

	if( Tape()->State() == Recording )
	{	if( Parameter(*this) )
		{	if( Variable(right) )
			{	if( ! IdenticalZero(left) )
				{	Tape()->RecordOp(DivpvOp, 
						*this, left, right.taddr
					);
				}
			}
		}
		else if( Parameter(right) )
		{	if( ! IdenticalOne(right.value) )
			{	Tape()->RecordOp(DivvpOp, 
					*this, taddr, right.value
				);
			}
		}
		else	Tape()->RecordOp(DivvvOp, 
				*this, taddr, right.taddr
		);
	}
	return *this;
}

// int, Base, and VecAD_reference<Base> cases are folded in using
// CppADAssignMember(/=) in the file AD.h


} // END CppAD namespace

# endif 
