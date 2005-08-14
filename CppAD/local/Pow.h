# ifndef CppADPowIncluded
# define CppADPowIncluded

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
$begin pow$$
$spell
	Cpp
	namespace
	exp
	inline
	const
	pow
$$

$index pow$$
$index exponent$$
$section The Power Function$$

$table
$bold Syntax$$ 
$cnext 
$syntax%inline AD<%Base%> pow (const AD<%Base%> &%x%, const AD<%Type%> &%y%)%$$
$rnext $cnext
$syntax%inline AD<%Base%> pow (const AD<%Base%> &%x%, int %y%)%$$
$tend

$fend 20$$

$head Description$$
Returns an
$syntax%AD<%Base%>%$$ object specified by
$latex \[
	{\rm pow} (x, y) = x^y
\] $$
If $italic y$$ is an $code int$$, the calculation is performed using
repeated multiplications.
Otherwise the $xref/log/$$ of $italic y$$ must be defined.
(for example, if $italic y$$ is zero, its log is not defined).


$head Assumptions$$
If $italic y$$ is not an $code int$$,
the derivative assumptions for the 
$xref/Log//log/$$ and $xref/Exp//exp/$$ functions must hold.

$head Standard Base Types$$
A definition for the $code pow$$ function
is automatically included (in the $code CppAD$$ namespace)
for the following 
$xref/glossary/Base Type/base types/$$:
$code float$$, $code double$$, $code complex<float>$$, 
and $code complex<double>$$.


$head Example$$
$children%
	Example/PowInt.cpp%
	Example/Pow.cpp
%$$
The file
$xref/PowInt.cpp/$$
contains an example and test of this function for the case
where $italic y$$ is an $code int$$.   
The file
$xref/Pow.cpp/$$
contains an example and test of this function for the case
where $italic y$$ is not an $code int$$.   
Both tests return true if they succeed and false otherwise.

$end
-------------------------------------------------------------------------------
*/

namespace CppAD {

CppADStandardMathBinaryFun(pow)

	template <class Base>
	inline AD<Base> pow (const AD<Base> &x, const AD<Base> &y)
	{	using CppAD::exp;
		using CppAD::log;

		return exp( y * log(x) );
	}

	template <class Base>
	inline AD<Base> pow (const AD<Base> &x, int n)
	{	AD<Base> p(1);
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
