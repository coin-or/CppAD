/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin LuVecADOk.cpp$$
$spell
	Geq
	Cpp
	Lu
$$

$section Lu Factor and Solve With Recorded Pivoting: Example and Test$$

$index Lu, record pivot$$
$index example, Lu record pivot$$
$index test, Lu record pivot$$

$code
$verbatim%example/lu_vec_ad_ok.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/

// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include "lu_vec_ad.hpp"
# include <cppad/speed/det_by_minor.hpp>

bool LuVecADOk(void)
{	bool  ok = true;

	using namespace CppAD;
	typedef AD<double> ADdouble;
	typedef CPPAD_TEST_VECTOR<ADdouble> ADVector;

	size_t              n = 3;
	size_t              m = 2;
	double a1[] = {
		3., 0., 0., // (1,1) is first  pivot
		1., 2., 1., // (2,2) is second pivot
		1., 0., .5  // (3,3) is third  pivot
	};
	double a2[] = {
		1., 2., 1., // (1,2) is second pivot
		3., 0., 0., // (2,1) is first  pivot
		1., 0., .5  // (3,3) is third  pivot
	};
	double rhs[] = {
		1., 3.,
		2., 2.,
		3., 1.
	};

	VecAD<double>       Copy    (n * n);
	VecAD<double>       Rhs     (n * m);
	VecAD<double>       Result  (n * m);
	ADdouble            logdet;
	ADdouble            signdet;

	// routine for checking determinants using expansion by minors
	det_by_minor<ADdouble> Det(n);

	// matrix we are computing the determinant of
	CPPAD_TEST_VECTOR<ADdouble> A(n * n);

	// dependent variable values
	CPPAD_TEST_VECTOR<ADdouble> Y(1 + n * m);

	size_t  i;
	size_t  j;
	size_t  k;

	// Original matrix
	for(i = 0; i < n * n; i++)
		A[i] = a1[i];

	// right hand side
	for(j = 0; j < n; j++)
		for(k = 0; k < m; k++)
			Rhs[ j * m + k ] = rhs[ j * m + k ];
		
	// Declare independent variables
	Independent(A);

	// Copy the matrix
	ADdouble index(0);
	for(i = 0; i < n*n; i++)
	{	Copy[index] = A[i];
		index += 1.;
	}

	// Solve the equation
	signdet = LuVecAD(n, m, Copy, Rhs, Result, logdet);

	// Result is the first n * m dependent variables
	index = 0.;
	for(i = 0; i < n * m; i++)
	{	Y[i] = Result[index];
		index += 1.;
	}

	// Determinant is last component of the solution
	Y[ n * m ] = signdet * exp( logdet );

	// construct f: A -> Y
	ADFun<double> f(A, Y);

	// check determinant using minors routine
	ADdouble determinant = Det( A );
	ok &= NearEqual(Y[n * m], determinant, 1e-10, 1e-10);


	// Check solution of Rhs = A * Result
	double sum;
	for(k = 0; k < m; k++)
	{	for(i = 0; i < n; i++)
		{	sum = 0.;
			for(j = 0; j < n; j++)
				sum += a1[i * n + j] * Value( Y[j * m + k] );
			ok &= NearEqual( rhs[i * m + k], sum, 1e-10, 1e-10 );
		}
	}
 
 	CPPAD_TEST_VECTOR<double> y2(1 + n * m);
 	CPPAD_TEST_VECTOR<double> A2(n * n);
 	for(i = 0; i < n * n; i++)
 		A[i] = A2[i] = a2[i];

 
 	y2          = f.Forward(0, A2);
 	determinant = Det(A);
 	ok &= NearEqual(y2[ n * m], Value(determinant), 1e-10, 1e-10);

	// Check solution of Rhs = A2 * Result
	for(k = 0; k < m; k++)
	{	for(i = 0; i < n; i++)
		{	sum = 0.;
			for(j = 0; j < n; j++)
				sum += a2[i * n + j] * y2[j * m + k];
			ok &= NearEqual( rhs[i * m + k], sum, 1e-10, 1e-10 );
		}
	}

	return ok;
}

// END PROGRAM
