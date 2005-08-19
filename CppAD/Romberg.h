# ifndef CppADRombergIncluded
# define CppADRombergIncluded

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
/*
$begin Romberg$$
$spell
	const
	Cpp
	Romberg
$$

$section Romberg Integration$$

$index integrate, Romberg$$
$index Romberg, Integrate$$

$table
$bold Syntax$$
$cnext $code # include <CppAD/Romberg.h>$$ 
$rnext $cnext
$syntax%Romberg(%F%, %a%, %b%, %n%, %r%, %e%)%$$
$tend

$fend 20$$

$head Description$$
Returns the Romberg integration estimates for
$latex \[
	I = \int_a^b F(x) {\bf d} x
\] $$

$head Include$$
The file $code CppAD/Romberg.h$$ is included by $code CppAD/CppAD.h$$
but it can also be included separately with out the rest of 
the $code CppAD$$ routines.

$head F$$
The object $italic F$$ can be of any type, but it must support 
the operation
$syntax%
	%F%(%x%)
%$$
The argument $italic x$$ to $italic F$$ has prototype
$syntax%
	const %Scalar% &%x%
%$$
The return value of $italic F$$ is a $italic Scalar$$ object
(see description of $xref/Romberg/Scalar/Scalar/$$ below). 

$head a$$
The argument $italic a$$ has prototype
$syntax%
	const %Scalar% &%a%
%$$ 
It specifies the lower limit for the integration.

$head b$$
The argument $italic b$$ has prototype
$syntax%
	const %Scalar% &%b%
%$$ 
It specifies the upper limit for the integration.

$head n$$
The argument $italic n$$ has prototype
$syntax%
	size_t %n%
%$$ 
A total number of $latex 2^{n-1} + 1$$ evaluations of $syntax%%F%(%x%)%$$
are used to estimate the integral.

$head r$$
The argument $italic r$$ has prototype
$syntax%
	%Vector% &%r%
%$$ 
(see description of $xref/Romberg/Vector/Vector/$$ below). 
It is a vector of size $italic n$$.
The input value of its elements does not matter
and the output value of its elements are the estimates of the integral
with different order accuracy.
To be specific,
$latex \[
	r_i = \int_a^b F(x) {\bf d} x + O \left[ (b - a) / 2^{n-1} \right]^{2(i+1)}
\] $$

$head e$$
The argument $italic e$$ has prototype
$syntax%
	%Vector% &%e%
%$$ 
It is a vector of size $italic m$$.
The input value of its elements does not matter
and the output value of its elements are approximations
for the corresponding integral estimates; i.e.,
$latex \[
	e_i \approx \left| r_i - \int_a^b F(x) {\bf d} x \right|
\] $$

$head Scalar$$
The following operations must be defined for $italic Scalar$$ objects
$italic a$$ and $italic b$$:

$table
$bold Operation$$ $cnext $bold Description$$  $rnext
$tend

$head Vector$$
The type $italic Vector$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type Scalar/$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$children%
	Example/Romberg.cpp
%$$
$head Example$$
$comment%
	Example/Romberg.cpp
%$$
The file
$xref/Romberg.cpp/$$
contains an example and test a test of using this routine.
It returns true if it succeeds and false otherwise.

$head Source Code$$
The source code for this routine is in the file
$code CppAD/Romberg.h$$.

$end
*/

# include <CppAD/CppADError.h>

namespace CppAD { // BEGIN CppAD namespace

template <class Fun, class Scalar, class Vector>
void Romberg(
	Fun           &F , 
	const Scalar  &a , 
	const Scalar  &b , 
	size_t         n , 
	Vector        &r , 
	Vector        &e )
{
	size_t i, k;
	Scalar pow2, sum, x; 

	size_t ipow2 = 1;
	Scalar zero  = Scalar(0);
	Scalar two   = Scalar(2);

	CppADUsageError(
		n >= 2,
		"Romberg: n must be greater than or equal 2"
	);

	//  set r[i] = trapazoidal rule with 2^i intervals in [a, b]
	r[0]  = ( F(a) + F(b) ) * (b - a) / two; 
	for(i = 1; i < n; i++)
	{	ipow2 *= 2;
		pow2   = Scalar(ipow2);
		sum    = zero;
		for(k = 1; k < ipow2; k += 2)
		{	// start = a + (b-a)/pow2, increment = 2*(b-a)/pow2
			x    = ( (pow2 - k) * a + k * b ) / pow2;
			sum += F(x);
		}
		// combine function evaluations in sum with those in T[i-1]
		r[i] = r[i-1] / two + sum * (b - a) / pow2;
	}

	// now compute the higher order estimates
	size_t ipow4    = 1;   // order of accuract for previous estimate
	Scalar pow4, pow4minus, best;
	for(i = 0; i < n; i++)
	{	// smallest step size estimate for order step^(2*i) estimate
		best = r[n-1];

		// corresponding error estimate
		e[i] = r[n-1] - r[n-2];
		if( e[i] < zero )
			e[i] = - e[i];

		// previous estimate is accurate to O[ step^(2*i) ]
		ipow4    *= 4;
		pow4      = Scalar(ipow4);
		pow4minus = Scalar(ipow4-1);
		for(k = n-1; k > i; k--)
			r[k] = ( pow4 * r[k] - r[k-1] ) / pow4minus;

		r[i] = best;
	}
	return;
}

} // END CppAD namespace

# endif
