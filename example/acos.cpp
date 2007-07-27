/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Acos.cpp$$
$spell
	cos
	acos
$$

$section The AD acos Function: Example and Test$$

$index acos, AD example$$
$index example, AD acos$$
$index test, AD acos$$

$code
$verbatim%example/acos.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>

bool Acos(void)
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

	// a temporary value
	AD<double> cos_of_x0 = CppAD::cos(x[0]);

	// range space vector 
	size_t m = 1;
	CPPAD_TEST_VECTOR< AD<double> > y(m);
	y[0] = CppAD::acos(cos_of_x0);

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y); 

	// check value 
	ok &= NearEqual(y[0] , x0,  1e-10 , 1e-10);

	// forward computation of first partial w.r.t. x[0]
	CPPAD_TEST_VECTOR<double> dx(n);
	CPPAD_TEST_VECTOR<double> dy(m);
	dx[0] = 1.;
	dy    = f.Forward(1, dx);
	ok   &= NearEqual(dy[0], 1., 1e-10, 1e-10);

	// reverse computation of derivative of y[0]
	CPPAD_TEST_VECTOR<double>  w(m);
	CPPAD_TEST_VECTOR<double> dw(n);
	w[0]  = 1.;
	dw    = f.Reverse(1, w);
	ok   &= NearEqual(dw[0], 1., 1e-10, 1e-10);

	// use a VecAD<Base>::reference object with acos
	CppAD::VecAD<double> v(1);
	AD<double> zero(0);
	v[zero] = cos_of_x0;
	AD<double> result = CppAD::acos(v[zero]);
	ok     &= NearEqual(result, x0, 1e-10, 1e-10);

	return ok;
}

// END PROGRAM
