/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Sinh.cpp$$
$spell
	sinh
$$

$section The AD sinh Function: Example and Test$$

$index sinh, AD example$$
$index example, AD sinh$$
$index test, AD sinh$$

$code
$verbatim%example/sinh.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <cmath>

bool Sinh(void)
{	bool ok = true;

	using CppAD::AD;
	using CppAD::NearEqual;

	// domain space vector
	size_t n  = 1;
	double x0 = 0.5;
	CPPAD_TEST_VECTOR< AD<double> > x(n);
	x[0]      = x0;

	// declare independent variables and start tape recording
	CppAD::Independent(x);

	// range space vector 
	size_t m = 1;
	CPPAD_TEST_VECTOR< AD<double> > y(m);
	y[0] = CppAD::sinh(x[0]);

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y); 

	// check value 
	double check = std::sinh(x0);
	ok &= NearEqual(y[0] , check,  1e-10 , 1e-10);

	// forward computation of first partial w.r.t. x[0]
	CPPAD_TEST_VECTOR<double> dx(n);
	CPPAD_TEST_VECTOR<double> dy(m);
	dx[0] = 1.;
	dy    = f.Forward(1, dx);
	check = std::cosh(x0);
	ok   &= NearEqual(dy[0], check, 1e-10, 1e-10);

	// reverse computation of derivative of y[0]
	CPPAD_TEST_VECTOR<double>  w(m);
	CPPAD_TEST_VECTOR<double> dw(n);
	w[0]  = 1.;
	dw    = f.Reverse(1, w);
	ok   &= NearEqual(dw[0], check, 1e-10, 1e-10);

	// use a VecAD<Base>::reference object with sinh
	CppAD::VecAD<double> v(1);
	AD<double> zero(0);
	v[zero]           = x0;
	AD<double> result = CppAD::sinh(v[zero]);
	check = std::sinh(x0);
	ok   &= NearEqual(result, check, 1e-10, 1e-10);

	return ok;
}

// END PROGRAM
