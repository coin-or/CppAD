/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin reverse_one.cpp$$
$spell
	Cpp
$$

$section First Order Reverse Mode: Example and Test$$

$index reverse, first order$$
$index example, first order reverse$$
$index test, first order reverse$$

$code
$verbatim%example/reverse_one.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>
namespace { // ----------------------------------------------------------
// define the template function reverse_one_cases<Vector> in empty namespace
template <typename Vector> 
bool reverse_one_cases(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;

	// domain space vector
	size_t n = 2;
	CPPAD_TEST_VECTOR< AD<double> > X(n);
	X[0] = 0.; 
	X[1] = 1.;

	// declare independent variables and start recording
	CppAD::Independent(X);

	// range space vector
	size_t m = 1;
	CPPAD_TEST_VECTOR< AD<double> > Y(m);
	Y[0] = X[0] * X[0] * X[1];

	// create f : X -> Y and stop recording
	CppAD::ADFun<double> f(X, Y);

	// use first order reverse mode to evaluate derivative of y[0]
	// and use the values in X for the independent variables.
	CPPAD_TEST_VECTOR<double> w(m), dw(n);
	w[0] = 1.;
	dw   = f.Reverse(1, w);
	ok  &= NearEqual(dw[0] , 2.*X[0]*X[1], 1e-10, 1e-10);
	ok  &= NearEqual(dw[1] ,    X[0]*X[0], 1e-10, 1e-10);

	// use zero order forward mode to evaluate y at x = (3, 4)
	// and use the template parameter Vector for the vector type
	Vector x(n), y(m);
	x[0]    = 3.;
	x[1]    = 4.;
	y       = f.Forward(0, x);
	ok     &= NearEqual(y[0] , x[0]*x[0]*x[1], 1e-10, 1e-10);

	// use first order reverse mode to evaluate derivative of y[0]
	// and using the values in x for the independent variables.
	w[0] = 1.;
	dw   = f.Reverse(1, w);
	ok  &= NearEqual(dw[0] , 2.*x[0]*x[1], 1e-10, 1e-10);
	ok  &= NearEqual(dw[1] ,    x[0]*x[0], 1e-10, 1e-10);

	return ok;
}
} // End empty namespace 
# include <vector>
# include <valarray>
bool reverse_one(void)
{	bool ok = true;
	// Run with Vector equal to three different cases
	// all of which are Simple Vectors with elements of type double.
	ok &= reverse_one_cases< CppAD::vector  <double> >();
	ok &= reverse_one_cases< std::vector    <double> >();
	ok &= reverse_one_cases< std::valarray  <double> >();
	return ok;
}
// END PROGRAM
