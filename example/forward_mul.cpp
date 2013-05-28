/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin forward_mul.cpp$$
$spell
	Cpp
$$

$section Forward Mode: Example and Test of Multiple Orders$$
$index forward, multiple orders$$
$index multiple, forward orders$$
$index order, multiple forward$$

$code
$verbatim%example/forward_mul.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
bool forward_mul(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	double eps = 10. * std::numeric_limits<double>::epsilon();

	// domain space vector
	size_t n = 2;
	CPPAD_TESTVECTOR(AD<double>) X(n);
	X[0] = 0.; 
	X[1] = 1.;

	// declare independent variables and starting recording
	CppAD::Independent(X);

	// range space vector
	size_t m = 1;
	CPPAD_TESTVECTOR(AD<double>) Y(m);
	Y[0] = X[0] * X[0] * X[1];

	// create f: X -> Y and stop tape recording
	CppAD::ADFun<double> f(X, Y);

	// initially, the variable values during taping are stored in f
	ok &= f.size_taylor() == 1;

	// Compute three forward orders at one
	size_t p = 2, p1 = p+1;
	CPPAD_TESTVECTOR(double) x_p(n * (p+1)), y_p(m * (p+1));
	x_p[0 * p1 + 0] = 3.; x_p[1 * p1 + 0] = 4.; // order 0
	x_p[0 * p1 + 1] = 1.; x_p[1 * p1 + 1] = 0.; // order 1
	x_p[0 * p1 + 2] = 0.; x_p[1 * p1 + 2] = 0.; // order 2
	// X(t) =   x^0 + x^1 * t + x^2 * t^2
	//      = [ 3 + t, 4 ]   
	//
	y_p  = f.Forward(p, x_p);
	// Y(t) = F[X(t)] = y^0 + y^1 * t + y^2 * t^2 + o(t^3)
	//
	// check order zero
	CPPAD_TESTVECTOR(double) x(n);
	x[0] = x_p[0 * p1 + 0];
	x[1] = x_p[1 * p1 + 0];
	ok  &= NearEqual(y_p[0 * p1 + 0] , x[0]*x[0]*x[1], eps, eps);
	//
	// check order one
	ok  &= NearEqual(y_p[0 * p1 + 1] , 2.*x[0]*x[1], eps, eps);
	// check order two
	double F_00 = 2. * y_p[0 * p1 + 2]; // second partial F w.r.t. x[0], x[0]
	ok   &= NearEqual(F_00, 2.*x[1], eps, eps);

	// check number of orders per variable
	ok   &= f.size_taylor() == 3;

	return ok;
}
// END C++
