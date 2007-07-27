/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin pow_int.cpp$$
$spell
	tan
	atan
$$

$section The Pow Integer Exponent: Example and Test$$

$index pow, int$$
$index example, pow int$$
$index test, pow int$$

$code
$verbatim%example/pow_int.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <cmath>

bool pow_int(void)
{	bool ok = true;

	using CppAD::AD;
	using CppAD::NearEqual;

	// declare independent variables and start tape recording
	size_t n  = 1;
	double x0 = -0.5;
	CPPAD_TEST_VECTOR< AD<double> > x(n);
	x[0]      = x0;
	CppAD::Independent(x);

	// dependent variable vector 
	size_t m = 7;
	CPPAD_TEST_VECTOR< AD<double> > y(m);
	int i;
	for(i = 0; i < int(m); i++) 
		y[i] = CppAD::pow(x[0], i - 3);

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y); 

	// check value 
	double check;
	for(i = 0; i < int(m); i++) 
	{	check = std::pow(x0, double(i - 3));
		ok &= NearEqual(y[i] , check,  1e-10 , 1e-10);
	}

	// forward computation of first partial w.r.t. x[0]
	CPPAD_TEST_VECTOR<double> dx(n);
	CPPAD_TEST_VECTOR<double> dy(m);
	dx[0] = 1.;
	dy    = f.Forward(1, dx);
	for(i = 0; i < int(m); i++) 
	{	check = double(i-3) * std::pow(x0, double(i - 4));
		ok &= NearEqual(dy[i] , check,  1e-10 , 1e-10);
	}

	// reverse computation of derivative of y[i]
	CPPAD_TEST_VECTOR<double>  w(m);
	CPPAD_TEST_VECTOR<double> dw(n);
	for(i = 0; i < int(m); i++) 
		w[i] = 0.;
	for(i = 0; i < int(m); i++) 
	{	w[i] = 1.;	
		dw    = f.Reverse(1, w);
		check = double(i-3) * std::pow(x0, double(i - 4));
		ok &= NearEqual(dw[0] , check,  1e-10 , 1e-10);
		w[i] = 0.;	
	}

	return ok;
}

// END PROGRAM
