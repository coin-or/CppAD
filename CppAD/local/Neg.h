# ifndef CppADNegIncluded
# define CppADNegIncluded

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
$begin Neg$$
$spell
	Var
	const
	inline
	Add
$$

$index unary, minus$$
$index negative, operator$$
$index -, unary$$
$index operator, unary minus$$

$section Negation: The Unary Minus Operator$$

$table
$bold Syntax$$ 
$cnext 
$syntax%inline AD<%Base%> operator - ( const AD<%Base%> &%right% ) const%$$
$tend

$fend 20$$

$head Description$$
The operation
$syntax%
	- %right%
%$$
returns the additive inverse of $italic right$$.
The operand $italic right$$ is not changed by this operation.
If the operand is a variable,
the result is a 
$xref/glossary/Variable/variable/$$.

$head Assumptions$$

If the negation (unary $code -$$) operator is used with an 
$syntax%AD<%Base%>%$$ object,
it must also be defined for 
the base type $italic Base$$.

In addition,
if $latex f$$ is a 
$xref/glossary/Base Function/Base function/$$,
$latex \[
	\D{[ - f(x) ]}{x} = - \D{f(x)}{x}
\] $$



$head Example$$
$children%
	Example/Neg.cpp
%$$
The file
$xref/Neg.cpp/$$
contains an example and a test of this operation.

$end
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

// Broken g++ compiler inhibits declaring unary minus a member or friend
template <class Base>
inline AD<Base> operator - (AD<Base> const &right) 
{	AD<Base> result(0);

	result  -= right;

	return result;
}


template <class Base>
inline AD<Base> operator - (VecADelem<Base> const &right) 
{	return - right.ADBase(); }

}
//  END CppAD namespace


# endif
