/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Abs.cpp$$
$spell
	abs
$$

$section AD Absolute Value Function: Example and Test$$

$index abs, example$$
$index example, abs$$
$index test, abs$$
$index derivative, directional example$$
$index directional, derivative example$$

$code
$verbatim%example/abs.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>

bool Abs(void)
{	bool ok = true;

	using CppAD::abs;
	using CppAD::AD;
	using CppAD::NearEqual;

	// domain space vector
	size_t n = 1;
	CPPAD_TEST_VECTOR< AD<double> > x(n);
	x[0]     = 0.;

	// declare independent variables and start tape recording
	CppAD::Independent(x);

	// range space vector
	size_t m = 3;
	CPPAD_TEST_VECTOR< AD<double> > y(m);
	y[0]     = abs(x[0] - 1.);
	y[1]     = abs(x[0]);
	y[2]     = abs(x[0] + 1.);

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y);

	// check values
	ok &= (y[0] == 1.);
	ok &= (y[1] == 0.);
	ok &= (y[2] == 1.);

	// forward computation of partials w.r.t. a positive x[0] direction
	size_t p = 1;
	CPPAD_TEST_VECTOR<double> dx(n);
	CPPAD_TEST_VECTOR<double> dy(m);
	dx[0] = 1.;
	dy    = f.Forward(p, dx);
	ok  &= (dy[0] == - dx[0]);
	ok  &= (dy[1] == + dx[0]);
	ok  &= (dy[2] == + dx[0]);

	// forward computation of partials w.r.t. a negative x[0] direction
	dx[0] = -1.;
	dy    = f.Forward(p, dx);
	ok  &= (dy[0] == - dx[0]);
	ok  &= (dy[1] == - dx[0]);
	ok  &= (dy[2] == + dx[0]);

	// reverse computation of derivative of y[0] 
	p    = 0;
	CPPAD_TEST_VECTOR<double>  w(m);
	CPPAD_TEST_VECTOR<double> dw(n);
	w[0] = 1.; w[1] = 0.; w[2] = 0.;
	dw   = f.Reverse(p+1, w);
	ok  &= (dw[0] == -1.);

	// reverse computation of derivative of y[1] 
	w[0] = 0.; w[1] = 1.; w[2] = 0.;
	dw   = f.Reverse(p+1, w);
	ok  &= (dw[0] == 0.);

	// reverse computation of derivative of y[2] 
	w[0] = 0.; w[1] = 0.; w[2] = 1.;
	dw   = f.Reverse(p+1, w);
	ok  &= (dw[0] == 1.);

	// use a VecAD<Base>::reference object with abs
	CppAD::VecAD<double> v(1);
	AD<double> zero(0);
	v[zero]           = -1;
	AD<double> result = abs(v[zero]);
	ok   &= NearEqual(result, 1., 1e-10, 1e-10);

	return ok;
}

// END PROGRAM
