# ifndef CppADLuSolveIncluded
# define CppADLuSolveIncluded

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
$begin LuSolve$$
$escape #$$
$spell
	det
	exp
	Leq
	typename
	bool
	const
	namespace
	std
	Geq
	Lu
	CppAD
	signdet
	logdet
$$

$index LuSolve$$
$index linear, equation$$
$index equation, linear$$
$index determinant, Lu$$
$index solve, linear equation$$

$section Compute Determinant and Solve Linear Equations$$

$pre
$$

$head Syntax$$ $code# include <CppAD/LuSolve.h>$$
$pre
$$
$syntax%%signdet% = LuSolve(%n%, %m%, %A%, %B%, %X%, %logdet%)%$$


$head Description$$
Use an LU factorization of the matrix $italic A$$ to
compute its determinant 
and solve for $italic X$$ in the linear of equation
$latex \[
	A * X = B
\] $$
where $italic A$$ is an 
$italic n$$ by $italic n$$ matrix,
$italic X$$ is an 
$italic n$$ by $italic m$$ matrix, and
$italic B$$ is an $latex n x m$$ matrix.

$head Include$$
The file $code CppAD/LuSolve.h$$ is included by $code CppAD/CppAD.h$$
but it can also be included separately with out the rest of 
the $code CppAD$$ routines.

$head Factor and Invert$$
This routine is an easy to user interface to
$xref/LuFactor/$$ and $xref/LuInvert/$$ for computing determinants and
solutions of linear equations.
These separate routines should be used if
one right hand side $italic B$$
depends on the solution corresponding to another
right hand side (with the same value of $italic A$$).
In this case only one call to $code LuFactor$$ is required
but there will be multiple calls to $code LuInvert$$.


$head Matrix Storage$$
All matrices are stored in row major order.
To be specific, if $latex Y$$ is a vector
that contains a $latex p$$ by $latex q$$ matrix,
the size of $latex Y$$ must be equal to $latex  p * q $$ and for
$latex i = 0 , \ldots , p-1$$,
$latex j = 0 , \ldots , q-1$$,
$latex \[
	Y_{i,j} = Y[ i * q + j ]
\] $$

$head signdet$$
The return value $italic signdet$$ is a $code int$$ value
that specifies the sign factor for the determinant of $italic A$$.
This determinant of $italic A$$ is zero if and only if $italic signdet$$
is zero. 

$head n$$
The argument $italic n$$ has type $code size_t$$ 
and specifies the number of rows in the matrices
$italic A$$,
$italic X$$,
and $italic B$$.
The number of columns in $italic A$$ is also equal to $italic n$$.

$head m$$
The argument $italic m$$ has type $code size_t$$ 
and specifies the number of columns in the matrices
$italic X$$
and $italic B$$.
If $italic m$$ is zero,
only the determinant of $italic A$$ is computed and
the matrices $italic X$$ and $italic B$$ are not used.

$head A$$
The argument $italic A$$ has the prototype
$syntax%
	const %FloatVector% &%A%
%$$
and the size of $italic A$$ must equal $latex n * n$$
(see description of $xref/LuSolve/FloatVector/FloatVector/$$ below).
This is the $latex n$$ by $italic n$$ matrix that 
we are computing the determinant of 
and that defines the linear equation.

$head B$$
The argument $italic B$$ has the prototype
$syntax%
	const %FloatVector% &%B%
%$$
and the size of $italic B$$ must equal $latex n * m$$
(see description of $xref/LuSolve/FloatVector/FloatVector/$$ below).
This is the $latex n$$ by $italic m$$ matrix that 
defines the right hand side of the linear equations.
If $italic m$$ is zero, $italic B$$ is not used.

$head X$$
The argument $italic X$$ has the prototype
$syntax%
	%FloatVector% &%X%
%$$
and the size of $italic X$$ must equal $latex n * m$$
(see description of $xref/LuSolve/FloatVector/FloatVector/$$ below).
The input value of $italic X$$ does not matter.
On output, the elements of $italic X$$ contain the solution
of the equation we wish to solve
(unless $italic signdet$$ is equal to zero).
If $italic m$$ is zero, $italic X$$ is not used.

$head logdet$$
The argument $italic logdet$$ has prototype
$syntax%
	%Float% &%logdet%
%$$
On input, the value of $italic logdet$$ does not matter.
On output, it has been set to the 
log of the determinant of $italic A$$ 
(but not quite).
To be more specific,
the determinant of $italic A$$ is given by the formula
$syntax%
	%det% = %signdet% * exp( %logdet% )
%$$
This enables $code LuSolve$$ to use logs of absolute values
in the case where $italic Float$$ corresponds to a real number.

$head Float$$
The type $italic Float$$ must satisfy the conditions
for a $xref/NumericType/$$ type.
The routine $xref/CheckNumericType/$$ will generate an error message
if this is not the case.
In addition, the following operations must be defined for any pair
of $italic Float$$ objects $italic x$$ and $italic y$$:

$table
$bold Operation$$ $cnext $bold Description$$  $rnext
$syntax%log(%x%)%$$ $cnext
	returns the logarithm of $italic x$$ as a $italic Float$$ object
$tend

$head FloatVector$$
The type $italic FloatVector$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type Float/$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head LeqZero$$
Including the file $code LuSolve.h$$ defines the template function 
$syntax%
	template <typename %Float%>
	bool LeqZero<%Float%>(const %Float% &%x%)
%$$
in the $code CppAD$$ namespace.
This function returns true if $italic x$$ is less than or equal to zero
and false otherwise.
It is used by $code LuSolve$$ to avoid taking the log of
zero (or a negative number if $italic Float$$ corresponds to real numbers).
This template function definition assumes that the operator 
$code <=$$ is defined for $italic Float$$ objects. 
If this operator is not defined for your use of $italic Float$$,
you will need to specialize this template so that it works for your
use of $code LuSolve$$.
$pre

$$
Complex numbers do not have the operation or $code <=$$ defined.
In addition, in the complex case, 
one can take the log of a negative number.
The specializations
$syntax%
	bool LeqZero< std::complex<float> > (const std::complex<float> &%x%)
	bool LeqZero< std::complex<double> >(const std::complex<double> &%x%)
%$$ 
are defined by including $code LuSolve.h$$.
These return true if $italic x$$ is zero and false otherwise.

$head AbsGeq$$
Including the file $code LuSolve.h$$ defines the template function 
$syntax%
	template <typename %Float%>
	bool AbsGeq<%Float%>(const %Float% &%x%, const %Float% &%y%)
%$$
If the type $italic Float$$ does not support the $code <=$$ operation
and it is not $code std::complex<float>$$ or $code std::complex<double>$$,
see the documentation for $code AbsGeq$$ in $xref/LuFactor/AbsGeq/LuFactor/$$. 

$children%
	Example/LuSolve.cpp
%$$
$head Example$$
The file 
$xref/LuSolve.cpp/$$
contains an example and test of using this routine.
It returns true if it succeeds and false otherwise.

$end
--------------------------------------------------------------------------
$begin LuSolve.h$$
$spell
	Cpp
	Leq
	Lu
	bool
	const
	endif
	ifdef
	inline
	ip
	jp
	logdet
	namespace
	signdet
	std
	typename
$$

$section LuSolve Source Code$$

$index source, LuSolve$$
$index LuSolve, source code$$
$index determinant, source code$$
$index linear, equation source code$$
$index matrix, factor source$$

$codep */
# include <complex>
# include <vector>

# ifdef _MSC_VER
// Fix for problem with MSC standard math functions name space location
# include <CppAD/CppAD.h>  
# endif

# include <CppAD/local/CppADError.h>
# include <CppAD/CheckSimpleVector.h>
# include <CppAD/CheckNumericType.h>
# include <CppAD/LuFactor.h>
# include <CppAD/LuInvert.h>

namespace CppAD { // BEGIN CppAD namespace

// LeqZero
template <typename Float>
inline bool LeqZero(const Float &x)
{	return x <= Float(0); }
inline bool LeqZero( const std::complex<double> &x )
{	return x == std::complex<double>(0); }
inline bool LeqZero( const std::complex<float> &x )
{	return x == std::complex<float>(0); }

// LuSolve
template <typename Float, typename FloatVector>
int LuSolve(
	size_t             n      ,
	size_t             m      , 
	const FloatVector &A      , 
	const FloatVector &B      , 
	FloatVector       &X      , 
	Float        &logdet      )
{	
	// check numeric type specifications
	CheckNumericType<Float>();

	// check simple vector class specifications
	CheckSimpleVector<Float, FloatVector>();

	size_t        p;       // index of pivot element (diagonal of L)
	int     signdet;       // sign of the determinant
	Float     pivot;       // pivot element

	// the value zero
	const Float zero(0);

	// pivot row and column order in the matrix
	std::vector<size_t> ip(n);
	std::vector<size_t> jp(n);

	// -------------------------------------------------------
	CppADUsageError(
		A.size() == n * n,
		"Error in LuSolve: A must have size equal to n * n"
	);
	CppADUsageError(
		B.size() == n * m,
		"Error in LuSolve: B must have size equal to n * m"
	);
	CppADUsageError(
		X.size() == n * m,
		"Error in LuSolve: X must have size equal to n * m"
	);
	// -------------------------------------------------------

	// copy A so that it does not change
	FloatVector Lu(A);

	// copy B so that it does not change
	X = B;

	// Lu factor the matrix A
	signdet = LuFactor(ip, jp, Lu);

	// compute the log of the determinant
	logdet  = Float(0);
	for(p = 0; p < n; p++)
	{	// pivot using the max absolute element
		pivot   = Lu[ ip[p] * n + jp[p] ];

		// check for determinant equal to zero
		if( pivot == zero )
		{	// abort the mission
			logdet = Float(0);
			return   0;
		}

		// update the determinant
		if( LeqZero ( pivot ) )
		{	logdet += log( - pivot );
			signdet = - signdet;
		}
		else	logdet += log( pivot );

	}

	// solve the linear equations
	LuInvert(ip, jp, Lu, X);

	// return the sign factor for the determinant
	return signdet;
}
} // END CppAD namespace 
/* $$
$end
------------------------------------------------------------------------------
*/

# endif
