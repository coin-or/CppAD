/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin JacLuDet.cpp$$
$spell
	Lu
	Cpp
$$

$section Gradient of Determinant Using Lu Factorization: Example and Test$$
$mindex gradient Lu$$
$index example, gradient$$
$index test, gradient$$
$index example, Lu$$
$index test, Lu$$

$code
$verbatim%Example/JacLuDet.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include "DetByLu.h"

bool JacLuDet()
{	bool ok = true;

	using namespace CppAD;

	size_t n = 2;

	// object for computing determinants
	DetByLu<ADComplex> Det(n);

	// independent and dependent variable vectors
	CppADvector<ADComplex>  X(n * n);
	CppADvector<ADComplex>  D(1);

	// value of the independent variable
	size_t i;
	for(i = 0; i < n * n; i++)
		X[i] = Complex(int(i), -int(i));

	// set the independent variables
	Independent(X);

	// compute the determinant
	D[0]  = Det(X);

	// create the function object
	ADFun<Complex> f(X, D);

	// argument value
	CppADvector<Complex>     x( n * n );
	for(i = 0; i < n * n; i++)
		x[i] = Complex(2 * i, i);

	// first derivative of the determinant
	CppADvector<Complex> J( n * n );
	J = f.Jacobian(x);

	/*
	f(x)     = x[0] * x[3] - x[1] * x[2]
	*/
	Complex Jtrue[]  = { x[3], -x[2], -x[1], x[0] };
	for( i = 0; i < n*n; i++)
		ok &= NearEqual( Jtrue[i], J[i], 1e-10 , 1e-10 );

	return ok;
}

// END PROGRAM
