# ifndef CppADLuFactorIncluded
# define CppADLuFactorIncluded

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
$begin LuFactor$$
$escape #$$
$spell
	Cpp
	Geq
	Lu
	bool
	const
	ip
	jp
	namespace
	std
	typename
$$

$index LuFactor$$
$index linear, Lu factor equation$$
$index equation, Lu factor$$
$index determinant, Lu factor$$
$index solve, Lu factor$$

$section LU Factorization of A Square Matrix$$

$pre
$$

$table
$bold Syntax$$ $cnext $code# include <CppAD/LuFactor.h>$$
$rnext $cnext 
$syntax%%sign% = LuFactor(%ip%, %jp%, %LU%)%$$
$tend

$fend 25$$

$head Description$$
Computes an LU factorization of the matrix $italic A$$ 
where $italic A$$ is a square matrix.

$head Include$$
The file $code CppAD/LuFactor.h$$ is included by $code CppAD/CppAD.h$$
but it can also be included separately with out the rest of 
the $code CppAD$$ routines.

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

$head sign$$
The return value $italic sign$$ has prototype
$syntax%
	int %sign%
%$$
If $italic A$$ is invertible, $italic sign$$ is plus or minus one
and is the sign of the permutation corresponding to the row ordering
$italic ip$$ and column ordering $italic jp$$.
If $italic A$$ is not invertible, $italic sign$$ is zero.

$head ip$$
The argument $italic ip$$ has prototype
$syntax%
	%SizeVector% &%ip%
%$$
(see description of $xref/LuFactor/SizeVector/SizeVector/$$ below).
The size of $italic ip$$ is referred to as $italic n$$ in the
specifications below.
The input value of the elements of $italic ip$$ does not matter.
The output value of the elements of $italic ip$$ determine
the order of the rows in the permuted matrix.

$head jp$$
The argument $italic jp$$ has prototype
$syntax%
	%SizeVector% &%jp%
%$$
(see description of $xref/LuFactor/SizeVector/SizeVector/$$ below).
The size of $italic jp$$ must be equal to $italic n$$.
The input value of the elements of $italic jp$$ does not matter.
The output value of the elements of $italic jp$$ determine
the order of the columns in the permuted matrix.

$head LU$$
The argument $italic LU$$ has the prototype
$syntax%
	%FloatVector% &%LU%
%$$
and the size of $italic LU$$ must equal $latex n * n$$
(see description of $xref/LuFactor/FloatVector/FloatVector/$$ below).

$subhead A$$
We define $italic A$$ as the matrix corresponding to the input 
value of $italic LU$$.

$subhead P$$
We define the permuted matrix $italic P$$ in terms of $italic A$$ by
$syntax%
	%P%(%i%, %j%) = %A%[ %ip%[%i%] * %n% + %jp%[%j%] ]
%$$

$subhead L$$
We define the lower triangular matrix $italic L$$ in terms of the 
output value of $italic LU$$.
The matrix $italic L$$ is zero above the diagonal
and the rest of the elements are defined by
$syntax%
	%L%(%i%, %j%) = %LU%[ %ip%[%i%] * %n% + %jp%[%j%] ]
%$$
for $latex i = 0 , \ldots , n-1$$ and $latex j = 0 , \ldots , i$$.

$subhead U$$
We define the upper triangular matrix $italic U$$ in terms of the
output value of $italic LU$$.
The matrix $italic U$$ is zero below the diagonal,
one on the diagonal,
and the rest of the elements are defined by
$syntax%
	%U%(%i%, %j%) = %LU%[ %ip%[%i%] * %n% + %jp%[%j%] ]
%$$
for $latex i = 0 , \ldots , n-2$$ and $latex j = i+1 , \ldots , n-1$$.

$subhead Factor$$
If the return value $italic sign$$ is non-zero,
$syntax%
	%L% * %U% = %P%
%$$
If the return value of $italic sign$$ is zero,
the contents of $italic L$$ and $italic U$$ are not defined. 

$subhead Determinant$$
$index determinant$$
If the return value $italic sign$$ is zero,
the determinant of the input value of $italic A$$ is zero.
If $italic sign$$ is non-zero,
using the output value of $italic LU$$
the determinant of the matrix $italic A$$ is equal to
$syntax%
	%sign% * %LU%[%ip%[0], %jp%[0]] * %...% * %LU%[%ip%[%n%-1], %jp%[%n%-1]] 
%$$


$head SizeVector$$
The type $italic SizeVector$$ must be a $xref/SimpleVector/$$ class with
$xref/SimpleVector/Elements of Specified Type/elements of type size_t/$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head FloatVector$$
The type $italic FloatVector$$ must be a 
$xref/SimpleVector//simple vector class/$$.
The routine $xref/CheckSimpleVector/$$ will generate an error message
if this is not the case.

$head Float$$
This notation is used to denote the type corresponding
to the elements of a $italic FloatVector$$.
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

$head AbsGeq$$
Including the file $code LuFactor.h$$ defines the template function 
$syntax%
	template <typename %Float%>
	bool AbsGeq<%Float%>(const %Float% &%x%, const %Float% &%y%)
%$$
in the $code CppAD$$ namespace.
This function returns true if the absolute value of 
$italic x$$ is greater than or equal the absolute value of $italic y$$. 
It is used by $code LuFactor$$ to choose the pivot elements.
This template function definition uses the operator 
$code <=$$ to obtain the absolute value for $italic Float$$ objects. 
If this operator is not defined for your use of $italic Float$$,
you will need to specialize this template so that it works for your
use of $code LuFactor$$.
$pre

$$
Complex numbers do not have the operation $code <=$$ defined.
The specializations
$syntax%
bool AbsGeq< std::complex<float> > 
	(const std::complex<float> &%x%, const std::complex<float> &%y%)
bool AbsGeq< std::complex<double> > 
	(const std::complex<double> &%x%, const std::complex<double> &%y%)
%$$ 
are define by including $code LuFactor.h$$
These return true if the sum of the square of the real and imaginary parts
of $italic x$$ is greater than or equal the 
sum of the square of the real and imaginary parts of $italic y$$. 

$children%
	Example/LuFactor.cpp
%$$
$head Example$$
The file $xref/LuSolve.h/$$ is a good example usage of 
$code LuFactor$$ with $code LuInvert$$.
The file 
$xref/LuFactor.cpp/$$
contains an example and test of using $code LuInvert$$ by itself.
It returns true if it succeeds and false otherwise.

$end
--------------------------------------------------------------------------
$begin LuFactor.h$$
$spell
	Cpp
	Geq
	Lu
	bool
	const
	emax
	endif
	etmp
	ifdef
	imag
	imax
	indx
	inline
	ip
	jmax
	jp
	namespace
	std
	typedef
	typename
	xabs
	xsq
	yabs
	ysq
$$

$section LuFactor Source Code$$

$index LuFactor, source$$
$index source, LuFactor$$
$index matrix, LuFactor source$$

$codep */
# include <complex>
# include <vector>

# ifdef _MSC_VER
// Fix for problem with MSC standard math functions name space location
# include <CppAD/CppAD.h>  
# endif

# include <CppAD/CppADError.h>
# include <CppAD/CheckSimpleVector.h>
# include <CppAD/CheckNumericType.h>

namespace CppAD { // BEGIN CppAD namespace

// AbsGeq
template <typename Float>
inline bool AbsGeq(const Float &x, const Float &y)
{	Float xabs = x;
	if( xabs <= Float(0) )
		xabs = - xabs;
	Float yabs = y;
	if( yabs <= Float(0) )
		yabs = - yabs;
	return xabs >= yabs;
}
inline bool AbsGeq(
	const std::complex<double> &x, 
	const std::complex<double> &y)
{	double xsq = x.real() * x.real() + x.imag() * x.imag();
	double ysq = y.real() * y.real() + y.imag() * y.imag();

	return xsq >= ysq;
}
inline bool AbsGeq(
	const std::complex<float> &x, 
	const std::complex<float> &y)
{	float xsq = x.real() * x.real() + x.imag() * x.imag();
	float ysq = y.real() * y.real() + y.imag() * y.imag();

	return xsq >= ysq;
}

// LuFactor
template <typename SizeVector, typename FloatVector>
int LuFactor(SizeVector &ip, SizeVector &jp, FloatVector &LU)
{	
	// type of the elements of LU
	typedef typename FloatVector::value_type Float;

	// check numeric type specifications
	CheckNumericType<Float>();

	// check simple vector class specifications
	CheckSimpleVector<Float, FloatVector>();
	CheckSimpleVector<size_t, SizeVector>();

	size_t  i, j;          // some temporary indices
	const Float zero( 0 ); // the value zero as a Float object
	size_t  imax;          // row index of maximum element
	size_t  jmax;          // column indx of maximum element
	Float    emax;         // maximum absolute value
	size_t  p;             // count pivots
	int     sign;          // sign of the permutation
	Float   etmp;          // temporary element
	Float   pivot;         // pivot element

	// -------------------------------------------------------
	size_t n = ip.size();
	CppADUsageError(
		jp.size() == n,
		"Error in LuFactor: jp must have size equal to n"
	);
	CppADUsageError(
		LU.size() == n * n,
		"Error in LuFactor: LU must have size equal to n * m"
	);
	// -------------------------------------------------------

	// initialize row and column order in matrix not yet pivoted
	for(i = 0; i < n; i++)
	{	ip[i] = i;
		jp[i] = i;
	}
	// initialize the sign of the permutation
	sign = 1;
	// ---------------------------------------------------------
	// Reduce the matrix P to L * U using n pivots
	for(p = 0; p < n; p++)
	{	// determine row and column corresponding to element of 
		// maximum absolute value in remaining part of P
		imax = jmax = n;
		emax = zero;
		for(i = p; i < n; i++)
		{	for(j = p; j < n; j++)
			{	CppADUnknownError(
					(ip[i] < n) & (jp[j] < n)
				);
				etmp = LU[ ip[i] * n + jp[j] ];

				// check if maximum absolute value so far
				if( AbsGeq (etmp, emax) )
				{	imax = i;
					jmax = j;
					emax = etmp;
				}
			}
		}
		CppADUsageError( 
			(imax < n) & (jmax < n) ,
			"AbsGeq must return true when second argument is zero"
		);
		if( imax != p )
		{	// switch rows so max absolute element is in row p
			i        = ip[p];
			ip[p]    = ip[imax];
			ip[imax] = i;
			sign     = -sign;
		}
		if( jmax != p )
		{	// switch columns so max absolute element is in column p
			j        = jp[p];
			jp[p]    = jp[jmax];
			jp[jmax] = j;
			sign     = -sign;
		}
		// pivot using the max absolute element
		pivot   = LU[ ip[p] * n + jp[p] ];

		// check for determinant equal to zero
		if( pivot == zero )
		{	// abort the mission
			return   0;
		}

		// Reduce U by the elementary transformations that maps 
		// LU( ip[p], jp[p] ) to one.  Only need transform elements
		// above the diagonal in U and LU( ip[p] , jp[p] ) is
		// corresponding value below diagonal in L.
		for(j = p+1; j < n; j++)
			LU[ ip[p] * n + jp[j] ] /= pivot;

		// Reduce U by the elementary transformations that maps 
		// LU( ip[i], jp[p] ) to zero. Only need transform elements 
		// above the diagonal in U and LU( ip[i], jp[p] ) is 
		// corresponding value below diagonal in L.
		for(i = p+1; i < n; i++ )
		{	etmp = LU[ ip[i] * n + jp[p] ];
			for(j = p+1; j < n; j++)
			{	LU[ ip[i] * n + jp[j] ] -= 
					etmp * LU[ ip[p] * n + jp[j] ];
			} 
		}
	}
	return sign;
}
} // END CppAD namespace 
/* $$
$end
------------------------------------------------------------------------------
*/

# endif
