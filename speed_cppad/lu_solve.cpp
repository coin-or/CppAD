/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin LuSolveSpeed.cpp$$
$spell
	Geq
	Cpp
	Lu
$$

$section Speed Test Lu Factor and Solve: Example and Test$$

$index Lu, speed$$
$index speed, Lu$$

$code
$verbatim%speed_cppad/lu_solve.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/

// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include "../example/lu_vec_ad.hpp"

# include <sstream>
# include <string>

std::string LuSolve(size_t size, size_t repeat)
{
	using namespace std;
	using namespace CppAD;
	typedef AD<double> ADdouble;

	size_t n = size;
	size_t m = size - 1;
	if( m == 0 )
		m = 2;

	CppADvector<ADdouble>   A       (n * n);
	CppADvector<ADdouble>   CopyA   (n * n);
	CppADvector<ADdouble>   Rhs     (n * m);
	CppADvector<ADdouble>   Result  (n * m);
	ADdouble                logdet;
	ADdouble                signdet;

	// matrix defining the linear equations
	CppADvector<double> a     (n * n);
	CppADvector<double> rhs   (n * m);
	CppADvector<double> result(n * m);

	size_t  i;
	size_t  j;
	size_t  k;

	double  t;
	double  tj;

	// Original matrix
	for(i = 0; i < n; i++)
	{	t  = i / double(n);
		tj = 1.;
		for(j = 0; j < n; j++)
		{	A[i * n + j] = a[i * n + j] = tj;
			tj *= t;
		}
	}

	// right hand side
	for(j = 0; j < n; j++)
		for(k = 0; k < m; k++)
			rhs[ j * m + k ] = double(j + k);

	size_t memory = 0;
	size_t length = 0;
	while(repeat--)
	{
		Independent(A);

		// copy A
		for(i = 0; i < n * n; i++)
			CopyA[i] = A[i];

		// copy rhs
		for(i = 0; i < n * m; i++)
			Rhs[i] = rhs[i];

		// Solve the equation
		signdet = LuSolve(n, m, CopyA, Rhs, Result, logdet);

		// construct f: A -> Result
		ADFun<double> f(A, Result);

		// solve A * Rhs = Result
 		result = f.Forward(0, a);

		// save for later return
		length = f.Size();
		memory = f.Memory();
	}
	return "Solve Linear equations with retaping";
}

std::string LuVecAD(size_t size, size_t repeat)
{
	using namespace std;
	using namespace CppAD;
	typedef AD<double> ADdouble;

	size_t n = size;
	size_t m = size - 1;
	if( m == 0 )
		m = 2;

	CppADvector<ADdouble>     A         (n * n);
	CppADvector<ADdouble>     CopyResult(n * m);

	VecAD<double>       CopyA     (n * n);
	VecAD<double>       Rhs       (n * m);
	VecAD<double>       Result    (n * m);

	ADdouble            logdet;
	ADdouble            signdet;

	// matrix defining the linear equations
	CppADvector<double> a(n * n);

	// dependent variable is solution of linear equations
	CppADvector<double> result(n * m);

	size_t  i;
	size_t  j;
	size_t  k;

	double  t;
	double  tj;

	// Original matrix
	for(i = 0; i < n; i++)
	{	t  = i / double(n);
		tj = 1.;
		for(j = 0; j < n; j++)
		{	A[i * n + j] = a[i * n + j] = tj;
			tj *= t;
		}
	}

	// right hand side
	for(j = 0; j < n; j++)
		for(k = 0; k < m; k++)
			Rhs[ j * m + k ] = double(j + k);

	Independent(A);

	// Copy the matrix
	ADdouble index(0);
	for(i = 0; i < n*n; i++)
	{	CopyA[index] = A[i];
		index += 1.;
	}

	// Solve the equation
	signdet = LuVecAD(n, m, CopyA, Rhs, Result, logdet);

	// copy the result
	index = 0.;
	for(i = 0; i < n * m; i++)
	{	CopyResult[i] = Result[index];
		index += 1.;
	}

	// construct f: A -> Result
	ADFun<double> f(A, CopyResult);

	while(repeat--)
	{	// pretend to copy new matrix
		for(i = 0; i < n*n; i++)
			a[i] = a[i];

		// solve a * Rhs = result
 		result = f.Forward(0, a);
	}
	return "Solve Linear equations with out retaping";
}

// END PROGRAM
