/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin sub_eq.cpp$$

$section AD Compound Assignment Subtraction: Example and Test$$
$mindex -= subtract assign plus add$$



$code
$srcfile%example/general/sub_eq.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>

bool SubEq(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	double eps99 = 99.0 * std::numeric_limits<double>::epsilon();

	// domain space vector
	size_t  n = 1;
	double x0 = .5;
	CPPAD_TESTVECTOR(AD<double>) x(n);
	x[0]      = x0;

	// declare independent variables and start tape recording
	CppAD::Independent(x);

	// range space vector
	size_t m = 2;
	CPPAD_TESTVECTOR(AD<double>) y(m);
	y[0] = 3. * x[0];    // initial value
	y[0] -= 2;           // AD<double> -= int
	y[0] -= 4.;          // AD<double> -= double
	y[1] = y[0] -= x[0]; // use the result of a compound assignment

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y);

	// check value
	ok &= NearEqual(y[0] , 3.*x0-(2.+4.+x0), eps99, eps99);
	ok &= NearEqual(y[1] ,             y[0], eps99, eps99);

	// forward computation of partials w.r.t. x[0]
	CPPAD_TESTVECTOR(double) dx(n);
	CPPAD_TESTVECTOR(double) dy(m);
	dx[0] = 1.;
	dy    = f.Forward(1, dx);
	ok   &= NearEqual(dy[0], 2., eps99, eps99);
	ok   &= NearEqual(dy[1], 2., eps99, eps99);

	// reverse computation of derivative of y[0]
	CPPAD_TESTVECTOR(double)  w(m);
	CPPAD_TESTVECTOR(double) dw(n);
	w[0]  = 1.;
	w[1]  = 0.;
	dw    = f.Reverse(1, w);
	ok   &= NearEqual(dw[0], 2., eps99, eps99);

	// use a VecAD<Base>::reference object with computed subtraction
	CppAD::VecAD<double> v(1);
	AD<double> zero(0);
	AD<double> result = 1;
	v[zero] = 2;
	result -= v[zero];
	ok     &= (result == -1);

	return ok;
}

// END C++
