/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin HesMinorDet.cpp$$
$spell
	Cpp
$$

$section Gradient of Determinant Using Expansion by Minors: Example and Test$$
$mindex gradient minors expansion$$
$index example, gradient$$
$index test, gradient$$
$index example, minors expansion$$
$index test, minors expansion$$

$code
$verbatim%example/hes_minor_det.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <cppad/speed/det_by_minor.hpp>
# include <complex>

typedef std::complex<double>     Complex;
typedef CppAD::AD<Complex>       ADComplex;
typedef CPPAD_TEST_VECTOR<ADComplex>   ADVector;

// ----------------------------------------------------------------------------

bool HesMinorDet()
{	bool ok = true;

	using namespace CppAD;

	size_t n = 2;

	// object for computing determinants
	det_by_minor<ADComplex> Det(n);

	// independent and dependent variable vectors
	CPPAD_TEST_VECTOR<ADComplex>  X(n * n);
	CPPAD_TEST_VECTOR<ADComplex>  D(1);

	// value of the independent variable
	size_t i;
	for(i = 0; i < n * n; i++)
		X[i] = Complex(int(i), -int(i));

	// set the independent variables
	Independent(X);

	// comupute the determinant
	D[0] = Det(X); 

	// create the function object
	ADFun<Complex> f(X, D);

	// argument value
	CPPAD_TEST_VECTOR<Complex>     x( n * n );
	for(i = 0; i < n * n; i++)
		x[i] = Complex(2 * i, i);

	// first derivative of the determinant
	CPPAD_TEST_VECTOR<Complex> H( n * n * n * n);
	H = f.Hessian(x, 0);

	/*
	f(x)     = x[0] * x[3] - x[1] * x[2]
	f'(x)    = ( x[3], -x[2], -x[1], x[0] )
	*/
	Complex zero(0., 0.);
	Complex one(1., 0.);
	Complex Htrue[]  = { 
		zero, zero, zero,  one,
		zero, zero, -one, zero,
		zero, -one, zero, zero,
		 one, zero, zero, zero
	};
	for( i = 0; i < n*n*n*n; i++)
		ok &= Htrue[i] == H[i];

	return ok;

}

// END PROGRAM
