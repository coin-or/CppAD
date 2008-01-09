/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin sparse_hessian.cpp$$
$spell
	Cpp
	Hessian
$$

$section Sparse Hessian: Example and Test$$

$index Hessian, sparse$$
$index example, sparse Hessian$$
$index test, sparse Hessian$$
$index spare, Hessian example$$

$code
$verbatim%example/sparse_hessian.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
namespace { // ---------------------------------------------------------
// define the template function Case<Vector> in empty namespace
template <typename Vector> 
bool Case()
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	size_t i;

	// domain space vector
	size_t n = 3;
	CPPAD_TEST_VECTOR< AD<double> >  X(n);
	for(i = 0; i < n; i++)
		X[i] = AD<double> (0);

	// declare independent variables and starting recording
	CppAD::Independent(X);

	size_t m = 1;
	CPPAD_TEST_VECTOR< AD<double> >  Y(m);
	Y[0] = X[0] * X[0] + X[0] * X[1] + X[1] * X[1] + X[2] * X[2];

	// create f: X -> Y and stop tape recording
	CppAD::ADFun<double> f(X, Y);

	// new value for the independent variable vector
	Vector x(n);
	for(i = 0; i < n; i++)
		x[i] = double(i);

	// second derivative of y[1] 
	Vector w(m);
	w[0] = 1.;
	Vector h( n * n );
	h = f.SparseHessian(x, w);
	/*
	    [ 2 1 0 ]
	h = [ 1 2 0 ]
            [ 0 0 2 ]
	*/
	ok &=  NearEqual(2., h[0*n+0], 1e-10, 1e-10 );
	ok &=  NearEqual(1., h[0*n+1], 1e-10, 1e-10 );
	ok &=  NearEqual(0., h[0*n+2], 1e-10, 1e-10 );

	ok &=  NearEqual(1., h[1*n+0], 1e-10, 1e-10 );
	ok &=  NearEqual(2., h[1*n+1], 1e-10, 1e-10 );
	ok &=  NearEqual(0., h[1*n+2], 1e-10, 1e-10 );

	ok &=  NearEqual(0., h[2*n+0], 1e-10, 1e-10 );
	ok &=  NearEqual(0., h[2*n+1], 1e-10, 1e-10 );
	ok &=  NearEqual(2., h[2*n+2], 1e-10, 1e-10 );

	return ok;
}
} // End empty namespace 
# include <vector>
# include <valarray>
bool sparse_hessian(void)
{	bool ok = true;
	// Run with Vector equal to three different cases
	// all of which are Simple Vectors with elements of type double.
	ok &= Case< CppAD::vector  <double> >();
# if 0
	ok &= Case< std::vector    <double> >();
	ok &= Case< std::valarray  <double> >();
# endif
	return ok;
}
// END PROGRAM
