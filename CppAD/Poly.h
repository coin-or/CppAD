# ifndef CppADPolyIncluded
# define CppADPolyIncluded

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
$begin Poly$$
$spell
	CppAD
	namespace
	cstddef
	ifndef
	endif
	deg
	const
	std
	da
$$

$index Poly$$
$index polynomial$$
$index derivative, polynomial template$$
$index template, polynomial derivative$$

$section Evaluate a Polynomial or its Derivative$$

$table 
$bold Syntax$$
$cnext $code # include <CppAD/Poly.h>$$
$rnext $cnext 
$syntax%%p% = Poly(%k%, %a%, %z%)%$$
$tend

$fend 25$$

$head Description$$
Computes the $th k$$ derivative of the polynomial 
$latex P(z)$$ where
$latex \[
	P(z) = a_0 + a_1 z^1 + \cdots + a_d z^d
\] $$
and $italic a$$ is a vector of length $italic d+1$$.
If $italic k$$ is equal to zero, the return value is $latex P(z)$$.

$head Include$$
The file $code CppAD/Poly.h$$ is included by $code CppAD/CppAD.h$$
but it can also be included separately with out the rest of 
the $code CppAD$$ routines.
Including this file defines
$code Poly$$ within the $code CppAD$$ namespace.

$head p$$
The return value $italic p$$ is a $italic Type$$ object
and is set equal to $latex P^{(k)} (z)$$
(see the description of $xref/Poly/Type/Type/$$ below).

$head k$$
The argument $italic k$$ has type $code size_t$$
and specifies the order of the derivative.

$head a$$
The argument $italic a$$ has prototype
$syntax%
	const %Vector% &%a%
%$$
(see description of $xref/Poly/Vector/Vector/$$ below).
It specifies the vector corresponding to the polynomial $latex P(z)$$.

$head z$$
The argument $italic z$$ has prototype
$syntax%
	const %Type% &%z%
%$$
(see description of $italic Type$$ below).
It specifies the point at which to evaluate the polynomial


$head Type$$
It is assumed that
multiplication and addition of $italic Type$$ objects
are commutative.

$subhead Casting$$
The syntax $syntax%%Type%(%i%)%$$ 
must convert from a $code size_t$$ value $italic i$$
to a $italic Type$$ object. 

$subhead Operations$$
The following operations must be supported by $italic Type$$:
$table
$code  =$$    $cnext assignment     $rnext
$code  *=$$   $cnext multiplication computed assignment $rnext
$code  +=$$   $cnext addition computed assignment 

$tend


$head Vector$$
The type $italic Vector$$ must have all the operations
specified by 
$syntax%
	%SimpleVector%<%Type%>%
%$$
where $italic SimpleVector$$ is a 
$xref/SimpleVector//Simple Vector/$$ template class. 

$head Example$$
$children%
	Example/Poly.cpp
%$$
The file
$xref/Poly.cpp/$$
contains an example and test of this routine.
It returns true if it succeeds and false otherwise.


$end
------------------------------------------------------------------------------
$begin Poly.h$$

$section Poly Source Code$$
$spell
	cstddef
	namespace
	CppAD
	const
	da
$$

$index source, Poly$$
$index Poly, source$$

$codep */
# include <cstddef>  // used to defined size_t
namespace CppAD {    // BEGIN CppAD namespace

template <class Type, class Vector>
Type Poly(size_t k, const Vector &a, const Type &z)
{	size_t i;
	size_t d = a.size() - 1;

	// case where derivative order greater than degree of polynomial
	if( k > d )
		return Type(0);
	// case where we are evaluating a derivative
	if( k > 0 )
	{	// initialize factor as (k-1) !
		size_t factor = 1;
		for(i = 2; i < k; i++)
			factor *= i;

		// set b to coefficient vector corresponding to derivative
		Vector b(d - k + 1);
		for(i = k; i <= d; i++)
		{	factor   *= i;
			b[i - k]  = a[i] * Type(factor); 
			factor   /= (i - k + 1);
		}
		// value of derivative polynomial
		return Poly(0, b, z);
	}
	// case where we are evaluating the original polynomial
	Type sum = a[d];
	i        = d;
	while(i > 0)
	{	sum *= z;
		sum += a[--i];
	}
	return sum;
}
} // END CppAD namespace
/* $$
$end
---------------------------------------------------------------------------
*/

# endif
