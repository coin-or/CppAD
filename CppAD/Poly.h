# ifndef CppADPolyIncluded
# define CppADPolyIncluded

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
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

$head Syntax$$
$code # include <CppAD/Poly.h>$$
$pre
$$
$syntax%%p% = Poly(%k%, %a%, %z%)%$$


$head Description$$
Computes the $th k$$ derivative of the polynomial 
$latex \[
	P(z) = a_0 + a_1 z^1 + \cdots + a_d z^d
\] $$
If $italic k$$ is equal to zero, the return value is $latex P(z)$$.

$head Include$$
The file $code CppAD/Poly.h$$ is included by $code CppAD/CppAD.h$$
but it can also be included separately with out the rest of 
the $code CppAD$$ routines.
Including this file defines
$code Poly$$ within the $code CppAD$$ namespace.

$head k$$
The argument $italic k$$ has prototype
$syntax%
	size_t %k%
%$$
It specifies the order of the derivative to calculate.

$head a$$
The argument $italic a$$ has prototype
$syntax%
	const %Vector% &%a%
%$$
(see $xref/Poly/Vector/Vector/$$ below).
It specifies the vector corresponding to the polynomial $latex P(z)$$.

$head z$$
The argument $italic z$$ has prototype
$syntax%
	const %Type% &%z%
%$$
(see $italic Type$$ below).
It specifies the point at which to evaluate the polynomial

$head p$$
The result $italic p$$  has prototype
$syntax%
	%Type% %p%
%$$
(see $xref/Poly/Type/Type/$$ below)
and it is equal to the $th k$$ derivative of $latex P(z)$$; i.e., 
$latex \[
p = \frac{k !}{0 !} a_k 
  + \frac{(k+1) !}{1 !} a_{k+1} z^1 
  + \ldots
  + \frac{d !}{(d - k) !} a_d z^{d - k}
\]
$$
If $latex k > d$$, $syntax%%p% = %Type%(0)%$$.

$head Type$$
The type $italic Type$$ is determined by the argument $italic z$$.
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
The type $italic Vector$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type/$$
$italic Type$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head Operation Sequence$$
The $italic Type$$ operation sequence used to calculate $italic p$$ is 
$xref/glossary/Operation/Independent/independent/1/$$
of $italic z$$ and the elements of $italic a$$
(it does depend on the size of the vector $italic a$$).


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
*/
# include <cstddef>  // used to defined size_t
namespace CppAD {    // BEGIN CppAD namespace

template <class Type, class Vector>
Type Poly(size_t k, const Vector &a, const Type &z)
{	size_t i;
	size_t d = a.size() - 1;

	// check Vector is Simple Vector class with Type elements
	CheckSimpleVector<Type, Vector>();

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

# endif
