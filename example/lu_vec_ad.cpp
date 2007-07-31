/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin LuVecAD$$
$escape #$$
$spell
	signdet
	Lu
	Vec
	Rhs
	logdet
	Cpp
	CPPAD_TEST_VECTOR
$$

$index LuVecAD$$
$index Lu, linear equation$$
$index determinant, Lu$$
$index solve, linear equation$$
$index equation, solve linear$$
$index linear, solve equation$$

$section Lu Factor and Solve with Recorded Pivoting$$

$pre
$$

$head Syntax$$
$syntax%int LuVecAD(
	size_t %n%,
	size_t %m%,
	VecAD<%double%> &%Matrix%,
	VecAD<%double%> &%Rhs%,
	VecAD<%double%> &%Result%,
	AD<%double%> &%logdet%)%$$


$head Purpose$$
Solves the linear equation
$latex \[
	Matrix * Result = Rhs
\] $$
where $italic Matrix$$ is an $latex n \times n$$ matrix,
$italic Rhs$$ is an $latex n x m$$ matrix, and
$italic Result$$ is an $latex n x m$$ matrix.
$pre

$$
The routine $xref/LuSolve/$$ uses an arbitrary vector type,
instead of $xref/VecAD/$$,
to hold its elements.
The pivoting operations for a $code ADFun$$ object
corresponding to an $code LuVecAD$$ solution
will change to be optimal for the matrix being factored.
$pre

$$
It is often the case that
$code LuSolve$$ is faster than $code LuVecAD$$ when $code LuSolve$$
uses a simple vector class with 
$xref/SimpleVector/Elements of Specified Type/elements of type double/$$,
but the corresponding $xref/ADFun/$$ objects have a fixed
set of pivoting operations.

$head Storage Convention$$
The matrices stored in row major order.
To be specific, if $latex A$$ contains the vector storage for an
$latex n x m$$ matrix,
$latex i$$ is between zero and $latex  n-1$$,
and $latex j$$ is between zero and $latex m-1$$,
$latex \[

	A_{i,j} = A[ i * m + j ]
\] $$
(The length of $latex A$$ must be equal to $latex  n * m $$.)

$head n$$
is the number of rows in 
$italic Matrix$$,
$italic Rhs$$,
and $italic Result$$.

$head m$$
is the number of columns in 
$italic Rhs$$
and $italic Result$$.
It is ok for $italic m$$ to be zero which is reasonable when
you are only interested in the determinant of $italic Matrix$$.


$head Matrix$$
On input, this is an
$latex n \times n$$ matrix containing the variable coefficients for 
the equation we wish to solve.
On output, the elements of $italic Matrix$$ have been overwritten
and are not specified.

$head Rhs$$
On input, this is an
$latex n \times m$$ matrix containing the right hand side
for the equation we wish to solve.
On output, the elements of $italic Rhs$$ have been overwritten
and are not specified.
If $italic m$$ is zero, $italic Rhs$$ is not used.

$head Result$$
On input, this is an
$latex n \times m$$ matrix and the value of its elements do not matter.
On output, the elements of $italic Rhs$$ contain the solution
of the equation we wish to solve
(unless the value returned by $code LuVecAD$$ is equal to zero).
If $italic m$$ is zero, $italic Result$$ is not used.

$head logdet$$
On input, the value of $italic logdet$$ does not matter.
On output, it has been set to the 
log of the determinant of $italic Matrix$$ (but not quite).
To be more specific,
if $italic signdet$$ is the value returned by $code LuVecAD$$,
the determinant of $italic Matrix$$ is given by the formula
$latex \[
	det = signdet \exp( logdet )
\] $$
This enables $code LuVecAD$$ to use logs of absolute values.


$head Example$$
$children%
	example/lu_vec_ad_ok.cpp
%$$
The file
$xref/LuVecADOk.cpp/$$
contains an example and test of $code LuVecAD$$.
It returns true if it succeeds and false otherwise.


$end
------------------------------------------------------------------------------
*/

# include "lu_vec_ad.hpp"
# include <cppad/cppad.hpp>

// BEGIN CppAD namespace
namespace CppAD {

AD<double> LuVecAD(
	size_t                           n,
	size_t                           m,
	CppAD::VecAD<double>             &Matrix,
	CppAD::VecAD<double>             &Rhs,
	CppAD::VecAD<double>             &Result,
	CppAD::AD<double>                &logdet)
{
	using namespace CppAD;
	typedef AD<double> Type;

	// temporary index
	Type index;
	Type jndex;

	// index and maximum element value
	Type   imax;
	Type   jmax;
	Type   itmp;
	Type   jtmp;
	Type   emax;

	// some temporary indices
	Type i;
	Type j;
	Type k;

	// count pivots
	Type p;

	// sign of the determinant
	Type  signdet;

	// temporary values
	Type  etmp;
	Type  diff;

	// pivot element
	Type  pivot;

	// singular matrix
	Type  singular = 0.;

	// some constants 
	Type M(m); 
	Type N(n);
	Type One(1);
	Type Zero(0);

	// pivot row and column order in the matrix
	VecAD<double> ip(n);
	VecAD<double> jp(n);

	// -------------------------------------------------------

	// initialize row and column order in matrix not yet pivoted
	for(i = 0; i < N; i += 1.)
	{	ip[i] = i;
		jp[i] = i;
	}

	// initialize the log determinant
	logdet  = 0.;
	signdet = 1;

	for(p = 0; p < N; p += 1.)
	{
		// determine row and column corresponding to element of 
		// maximum absolute value in remaining part of Matrix
		imax = N;
		jmax = N;
		emax = 0.;
		for(i = p; i < N; i += 1.)
		{	itmp = ip[i] * N;
			for(j = p; j < N; j += 1.)
			{	CPPAD_ASSERT_UNKNOWN(
					(ip[i] < N) & (jp[j] < N)
				);
				index = itmp + jp[j];
				etmp  = Matrix[ index ];

				// compute absolute value of element
				etmp = abs(etmp);

				// update maximum absolute value so far
				emax = CondExpGe(etmp, emax, etmp, emax);
				imax = CondExpGe(etmp, emax,    i, imax);
				jmax = CondExpGe(etmp, emax,    j, jmax);
			}
		}
		assert( (imax < N) & (jmax < N) );

	
		// switch rows so max absolute element is in row p
		index    = ip[p];
		ip[p]    = ip[imax];
		ip[imax] = index;

		// if imax != p, switch sign of determinant
		signdet  = CondExpEq(imax, p, signdet,  -signdet);

		// switch columns so max absolute element is in column p
		jndex    = jp[p];
		jp[p]    = jp[jmax];
		jp[jmax] = jndex;

		// if imax != p, switch sign of determinant
		signdet  = CondExpEq(jmax, p, signdet,  -signdet);
	
		// pivot using the max absolute element
		itmp    = ip[p] * N;
		index   = itmp + jp[p];
		pivot   = Matrix[ index ];

		// update the singular matrix flag
		singular = CondExpEq(pivot, Zero, One, singular);

		// update the log of absolute determinant and its sign
		etmp     = abs(pivot);
		logdet   = logdet + log( etmp );
		signdet  = CondExpGe(pivot, Zero, signdet, - signdet);

		/*
		Reduce by the elementary transformations that maps 
		Matrix( ip[p], jp[p] ) to one and Matrix( ip[i], jp[p] )
		to zero for i = p + 1., ... , n-1
		*/

		// divide row number ip[p] by pivot element
		for(j = p + 1.; j < N; j += 1.)
		{
			index           = itmp + jp[j];
			Matrix[ index ] = Matrix[ index ] / pivot;
		}

		// not used anymore so no need to set to 1
		// Matrix[ ip[p] * N + jp[p] ] = Type(1);

		// divide corresponding row of right hand side by pivot element
		itmp = ip[p] * M;
		for(k = 0; k < M; k += 1.)
		{
			index = itmp + k;
			Rhs[ index ] = Rhs[ index ] / pivot;
		}

		for(i = p + 1.; i < N; i += 1. )
		{	itmp  = ip[i] * N;
			jtmp  = ip[p] * N;

			index = itmp + jp[p];
			etmp  = Matrix[ index ];

			for(j = p + 1.; j < N; j += 1.)
			{	index = itmp + jp[j];
				jndex = jtmp + jp[j];
				Matrix[ index ] = Matrix[ index ] 
					        - etmp * Matrix[ jndex ];
			}

			itmp = ip[i] * M;
			jtmp = ip[p] * M; 
			for(k = 0; k < M; k += 1.)
			{
				index = itmp + k;
				jndex = jtmp + k;
				Rhs[ index ] = Rhs[ index ]
				             - etmp * Rhs[ jndex ];
			}

			// not used any more so no need to set to zero
			// Matrix[ ip[i] * N + jp[p] ] = 0.;
		}

	}

	// loop over equations
	for(k = 0; k < M; k += 1.)
	{	// loop over variables
		p = N;
		while( p > 0. )
		{	p -= 1.;
			index = ip[p] * M + k;
			jndex = jp[p] * M + k;
			etmp            = Rhs[ index ];
			Result[ jndex ] = etmp;
			for(i = 0; i < p; i += 1. )
			{
				index = ip[i] * M + k;
				jndex = ip[i] * N + jp[p];
				Rhs[ index ] = Rhs[ index ] 
				             - etmp * Matrix[ jndex ];
			}
		}
	}

	// make sure return zero in the singular case
	return (1. - singular) * signdet;
}

} // END CppAD namespace
