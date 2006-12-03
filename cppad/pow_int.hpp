# ifndef CPPAD_POW_INT_INCLUDED
# define CPPAD_POW_INT_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
-------------------------------------------------------------------------------
$begin PowInt$$
$spell
	cppad.hpp
	CppAD
	namespace
	const
$$

$index pow, integer$$
$index exponent, integer$$
$index integer, pow$$

$section The Integer Power Function$$

$head Syntax$$
$code # include <cppad/pow_int.hpp>$$
$pre
$$
$syntax%%y% = pow(%x%, %n%)%$$


$head Purpose$$
Determines the value of the power function 
$latex \[
	{\rm pow} (x, n) = x^n
\] $$
for integer exponents $italic n$$.

$head Include$$
The file $code cppad/pow_int.hpp$$ is included by $code cppad/cppad.hpp$$
but it can also be included separately with out the rest of 
the $code CppAD$$ routines.
Including this file defines
$code pow$$ within the $code CppAD$$ namespace.

$head x$$
The argument $italic x$$ has prototype
$syntax%
	const %Type% &%x%
%$$ 

$head n$$
The argument $italic n$$ has prototype
$syntax%
	int %n%
%$$ 
(see $xref/pow/$$ for the case where $italic n$$ is an 
$syntax%AD<%Base%>%$$ object).

$head y$$
The result $italic y$$ has prototype
$syntax%
	%Type% %y%
%$$

$head Type$$
The type $italic Type$$ must support the following operations
where $italic a$$ and $italic b$$ are $italic Type$$ objects
and $italic i$$ is an $code int$$:
$table
$bold Operation$$  $pre  $$
	$cnext $bold Description$$ 
	$cnext $bold Result Type$$ 
$rnext
$syntax%%Type% %a%(%i%)%$$ 
	$cnext construction of a $italic Type$$ object from an $code int$$
	$cnext $italic Type$$
$rnext
$syntax%%a% * %b%$$ 
	$cnext binary multiplication of $italic Type$$ objects
	$cnext $italic Type$$
$rnext
$syntax%%a% / %b%$$ 
	$cnext binary division of $italic Type$$ objects
	$cnext $italic Type$$
$tend

$head Operation Sequence$$
The $italic Type$$ operation sequence used to calculate $italic y$$ is 
$xref/glossary/Operation/Independent/independent/1/$$
of $italic x$$.


$head Example$$
$children%
	example/pow_int.cpp
%$$
The file
$xref/PowInt.cpp/$$
contains an example and test of this function.   
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/

namespace CppAD { 

	template <class Type>
	inline Type pow (const Type &x, const int &n)
	{
		Type p(1);
		int n2 = n / 2;

		if( n == 0 )
			return p;
		if( n < 0 )
			return p / pow(x, -n);
		if( n == 1 )
			return x;

		// p = (x^2)^(n/2)
		p = pow( x * x , n2 );

		// n is even case
		if( n % 2 == 0 )
			return p;

		// n is odd case
		return p * x;
	}

}

# endif 
