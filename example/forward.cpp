/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Forward.cpp$$
$spell
	Cpp
$$

$section Forward Mode: Example and Test$$
$index Forward$$
$index example, Forward$$
$index test, Forward$$

$code
$verbatim%example/forward.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>
namespace { // --------------------------------------------------------
// define the template function ForwardCases<Vector> in empty namespace
template <class Vector> 
bool ForwardCases(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;

	// domain space vector
	size_t n = 2;
	CPPAD_TEST_VECTOR< AD<double> > X(n);
	X[0] = 0.; 
	X[1] = 1.;

	// declare independent variables and starting recording
	CppAD::Independent(X);

	// range space vector
	size_t m = 1;
	CPPAD_TEST_VECTOR< AD<double> > Y(m);
	Y[0] = X[0] * X[0] * X[1];

	// create f: X -> Y and stop tape recording
	CppAD::ADFun<double> f(X, Y);

	// The highest order Forward mode calculation below is is second order.
	// This corresponds to three Taylor coefficients per variable 
	// (zero, first, and second order).
	f.capacity_taylor(3);  // pre-allocate memory for speed of execution

	// initially, the variable values during taping are stored in f
	ok &= f.size_taylor() == 1;

	// zero order forward mode using notaiton in ForwardZero
	// use the template parameter Vector for the vector type
	Vector x(n);
	Vector y(m);
	x[0] = 3.;
	x[1] = 4.;
	y    = f.Forward(0, x);
	ok  &= NearEqual(y[0] , x[0]*x[0]*x[1], 1e-10, 1e-10);
	ok  &= f.size_taylor() == 1;

	// first order forward mode using notation in ForwardOne
	// X(t)           = x + dx * t
	// Y(t) = F[X(t)] = y + dy * t + o(t)
	Vector dx(n);
	Vector dy(m);
	dx[0] = 1.;
	dx[1] = 0.;
	dy    = f.Forward(1, dx); // partial F w.r.t. x[0]
	ok   &= NearEqual(dy[0] , 2.*x[0]*x[1], 1e-10, 1e-10);
	ok   &= f.size_taylor() == 2;

	// second order forward mode using notaiton in ForwardAny
	// X(t) =           x + dx * t + x_2 * t^2
	// Y(t) = F[X(t)] = y + dy * t + y_2 * t^2 + o(t^3)
	Vector x_2(n);
	Vector y_2(m);
	x_2[0]      = 0.;
	x_2[1]      = 0.;
	y_2         = f.Forward(2, x_2);
	double F_00 = 2. * y_2[0]; // second partial F w.r.t. x[0], x[0]
	ok         &= NearEqual(F_00, 2.*x[1], 1e-10, 1e-10);
	ok         &= f.size_taylor() == 3;

	// suppose we no longer need second order Taylor coefficients
	f.capacity_taylor(2);
	ok &= f.size_taylor() == 2;

	// actually we no longer need any Taylor coefficients
	f.capacity_taylor(0);
	ok &= f.size_taylor() == 0;

	return ok;
}
} // End empty namespace 
# include <vector>
# include <valarray>
bool Forward(void)
{	bool ok = true;
	// Run with Vector equal to three different cases
	// all of which are Simple Vectors with elements of type double.
	ok &= ForwardCases< CppAD::vector  <double> >();
	ok &= ForwardCases< std::vector    <double> >();
	ok &= ForwardCases< std::valarray  <double> >();
	return ok;
}
// END PROGRAM
