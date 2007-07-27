/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Pow.cpp$$
$spell
$$

$section The AD Power Function: Example and Test$$

$index pow, AD example$$
$index example, AD pow$$
$index test, AD pow$$

$code
$verbatim%example/pow.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <cmath>

bool Pow(void)
{	bool ok = true;

	using CppAD::AD;
	using CppAD::NearEqual;

	// domain space vector
	size_t n  = 2;
	double x = 0.5;
	double y = 2.;
	CPPAD_TEST_VECTOR< AD<double> > XY(n);
	XY[0]      = x;
	XY[1]      = y;

	// declare independent variables and start tape recording
	CppAD::Independent(XY);

	// range space vector 
	size_t m = 3;
	CPPAD_TEST_VECTOR< AD<double> > Z(m);
	Z[0] = CppAD::pow(XY[0], XY[1]);  // pow(variable, variable)
	Z[1] = CppAD::pow(XY[0], y);      // pow(variable, parameter)
	Z[2] = CppAD::pow(x,     XY[1]);  // pow(parameter, variable)

	// create f: XY -> Z and stop tape recording
	CppAD::ADFun<double> f(XY, Z); 

	// check value 
	double check = std::pow(x, y);
	size_t i;
	for(i = 0; i < m; i++)
		ok &= NearEqual(Z[i] , check,  1e-10 , 1e-10);

	// forward computation of first partial w.r.t. x
	CPPAD_TEST_VECTOR<double> dxy(n);
	CPPAD_TEST_VECTOR<double> dz(m);
	dxy[0] = 1.;
	dxy[1] = 0.;
	dz    = f.Forward(1, dxy);
	check = y * std::pow(x, y-1.);
	ok   &= NearEqual(dz[0], check, 1e-10, 1e-10);
	ok   &= NearEqual(dz[1], check, 1e-10, 1e-10);
	ok   &= NearEqual(dz[2],    0., 1e-10, 1e-10);

	// forward computation of first partial w.r.t. y
	dxy[0] = 0.;
	dxy[1] = 1.;
	dz    = f.Forward(1, dxy);
	check = std::log(x) * std::pow(x, y);
	ok   &= NearEqual(dz[0], check, 1e-10, 1e-10);
	ok   &= NearEqual(dz[1],    0., 1e-10, 1e-10);
	ok   &= NearEqual(dz[2], check, 1e-10, 1e-10);

	// reverse computation of derivative of z[0] + z[1] + z[2]
	CPPAD_TEST_VECTOR<double>  w(m);
	CPPAD_TEST_VECTOR<double> dw(n);
	w[0]  = 1.;
	w[1]  = 1.;
	w[2]  = 1.;
	dw    = f.Reverse(1, w);
	check = y * std::pow(x, y-1.);
	ok   &= NearEqual(dw[0], 2. * check, 1e-10, 1e-10);
	check = std::log(x) * std::pow(x, y);
	ok   &= NearEqual(dw[1], 2. * check, 1e-10, 1e-10);

	// use a VecAD<Base>::reference object with pow
	CppAD::VecAD<double> v(2);
	AD<double> zero(0);
	AD<double> one(1);
	v[zero]           = XY[0];
	v[one]            = XY[1];
	AD<double> result = CppAD::pow(v[zero], v[one]);
	ok               &= NearEqual(result, Z[0], 1e-10, 1e-10);

	return ok;
}

// END PROGRAM
