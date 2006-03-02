# ifndef CppADAtanIncluded
# define CppADAtanIncluded

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
$begin atan$$
$spell
	Cpp
	namespace
	Arctangent
	const
	atan
$$

$index tan, inverse$$
$index inverse, tan$$
$index atan$$

$section The Inverse Tangent Function$$

$table
$bold Syntax$$ 
$cnext 
$syntax%AD<%Base%> atan (const AD<%Base%> &%x%)%$$
$tend

$fend 20$$

$head Description$$
Returns an
$syntax%AD<%Base%>%$$ object that is equal to
$syntax%
	atan(%x%)
%$$
where $code atan$$ has the same interpretation as
for the corresponding $xref/glossary/Base Type/base type/$$.

$head Base Type Requirement$$
A definition for the $code atan$$ function
is automatically included (in the $code CppAD$$ namespace)
for the following 
$code float$$ and $code double$$
$xref/glossary/Base Type/base types/$$.
If the $code atan$$ function is used with any other base type, 
it must be a defined for that base type.

$head Derivative Assumption$$
It is assumed that
$latex \[
\begin{array}{lcr}
	\D{[ {\rm atan} (x) ]}{x} & = & \frac{1}{1 + x^2}  
\end{array}
\] $$



$head Example$$
$children%
	Example/Atan.cpp
%$$
The file
$xref/Atan.cpp/$$
contains an example and test of this function.   
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/

namespace CppAD {

	// define for float, double
	CppADStandardMathUnaryBaseHalf(atan)

	// define for AD types
	CppADStandardMathUnaryTemplate(atan, AtanOp)
}

# endif 
