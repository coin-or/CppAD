// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin jac_lu_det.cpp$$
$spell
	Lu
	Cpp
$$

$section Gradient of Determinant Using Lu Factorization: Example and Test$$

$code
$verbatim%example/jac_lu_det.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++

# include <cppad/cppad.hpp>
# include <cppad/speed/det_by_lu.hpp>

bool JacLuDet(void)
{	bool ok = true;

	using namespace CppAD;

	size_t n = 2;

	// object for computing determinants
	det_by_lu<ADComplex> Det(n);

	// independent and dependent variable vectors
	CPPAD_TESTVECTOR(ADComplex)  X(n * n);
	CPPAD_TESTVECTOR(ADComplex)  D(1);

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
	CPPAD_TESTVECTOR(Complex)     x( n * n );
	for(i = 0; i < n * n; i++)
		x[i] = Complex(2 * i, i);

	// first derivative of the determinant
	CPPAD_TESTVECTOR(Complex) J( n * n );
	J = f.Jacobian(x);

	/*
	f(x)     = x[0] * x[3] - x[1] * x[2]
	*/
	Complex Jtrue[]  = { x[3], -x[2], -x[1], x[0] };
	for( i = 0; i < n*n; i++)
		ok &= NearEqual( Jtrue[i], J[i], 1e-10 , 1e-10 );

	return ok;
}

// END C++
