# ifndef CppADDivEqIncluded
# define CppADDivEqIncluded

// BEGIN SHORT COPYRIGHT
/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-05 Bradley M. Bell

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
// END SHORT COPYRIGHT

/*
-------------------------------------------------------------------------------
$begin DivEq$$
$spell
	Var
	const
	Div
	Eq
$$

$index /=$$
$index computed, /=$$
$index assign, /=$$
$index divide, /=$$

$section The AD Division Computed Assignment Operator$$

$table
$bold Syntax$$ 
$cnext 
$syntax%%left% /= %right%$$
$tend

$fend 20$$

$head Description$$
Suppose that $italic left$$ is an
$syntax%AD<%Base%>%$$ object, 
and $italic right$$ is an $code int$$, $italic Base$$
or $syntax%AD<%Base%>%$$ object.
In this case
$syntax%
	%left% /= %right%
%$$
returns an
$syntax%AD<%Base%>%$$ object
where $code /=$$ has the same interpretation as
for the $italic Base$$ type.
In addition, this must be the same as
$syntax%
	%left% = %left% / %right%
%$$


$head Operands$$
Note the value of the left operand is changed
and the value of the right remains unchanged.


If the left operand has value zero and is a
$xref/glossary/Parameter/parameter/$$ or a $italic Base$$ operand,
the result is a parameter.
Otherwise,
if the left or right operand is a variable,
the result is a 
$xref/glossary/Variable/variable/$$.
$pre

$$
If the right operand is 
exactly equal to one and it is a
$xref/glossary/Parameter/parameter/$$ or a $italic Base$$ operand,
the result has the same derivative information as the 
left operand and no extra space on the tape is required for this operation.

$head Assumptions$$

If the $code /=$$ operator is used with an 
$syntax%AD<%Base%>%$$ object,
it must be a
$xref/CompEq//computed assignment operator/$$
for the base type $italic Base$$.
In addition,
if $latex f$$ and $latex g$$ are 
$xref/glossary/Base Function/Base functions/$$
with the same range dimension,
$latex \[
	\D{[ f(x) / g(x) ]}{x} = 
		[1/g(x)] * \D{f(x)}{x} - [f(x)/g(x)^2] * \D{g(x)}{x} 
\] $$


$head Example$$
$children%
	Example/DivEq.cpp
%$$
The file
$xref/DivEq.cpp/$$
contains an example and a test of these operations.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/
//  BEGIN CppAD namespace
namespace CppAD {

// Computed Assignment Division operators: 
// assume CppAD.h has already been included

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


} // END CppAD namespace

# endif 
