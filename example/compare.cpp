/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Compare.cpp$$
$spell
	Cpp
$$

$section AD Binary Comparison Operators: Example and Test$$

$index compare, AD example$$
$index example, AD compare$$
$index test, AD compare$$

$index <, example$$
$index <=, example$$
$index >, example$$
$index >=, example$$
$index ==, example$$
$index !=, example$$

$code
$verbatim%example/compare.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>

bool Compare(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;

	// declare independent variables and start tape recording
	size_t n  = 2;
	double x0 = 0.5;
	double x1 = 1.5;
	CPPAD_TEST_VECTOR< AD<double> > x(n);
	x[0]      = x0; 
	x[1]      = x1; 
	CppAD::Independent(x);

	// some binary comparision operations
	AD<double> p;
	if( x[0] < x[1] )
		p = x[0];   // values in x choose this case
	else	p = x[1];
	if( x[0] <= x[1] )
		p *= x[0];  // values in x choose this case
	else	p *= x[1];
	if( x[0] >  x[1] )
		p *= x[0]; 
	else	p *= x[1];  // values in x choose this case
	if( x[0] >= x[1] )
		p *= x[0]; 
	else	p *= x[1];  // values in x choose this case
	if( x[0] == x[1] )
		p *= x[0]; 
	else	p *= x[1];  // values in x choose this case
	if( x[0] != x[1] )
		p *= x[0];  // values in x choose this case
	else	p *= x[1]; 

	// dependent variable vector 
	size_t m = 1;
	CPPAD_TEST_VECTOR< AD<double> > y(m);
	y[0] = p;

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y); 

	// check value 
	ok &= NearEqual(y[0] , x0*x0*x1*x1*x1*x0,  1e-10 , 1e-10);

	// forward computation of partials w.r.t. x[0]
	CPPAD_TEST_VECTOR<double> dx(n);
	CPPAD_TEST_VECTOR<double> dy(m);
	dx[0] = 1.;
	dx[1] = 0.;
	dy    = f.Forward(1, dx);
	ok   &= NearEqual(dy[0], 3.*x0*x0*x1*x1*x1, 1e-10, 1e-10);

	// forward computation of partials w.r.t. x[1]
	dx[0] = 0.;
	dx[1] = 1.;
	dy    = f.Forward(1, dx);
	ok   &= NearEqual(dy[0], 3.*x0*x0*x1*x1*x0, 1e-10, 1e-10);

	// reverse computation of derivative of y[0]
	CPPAD_TEST_VECTOR<double>  w(m);
	CPPAD_TEST_VECTOR<double> dw(n);
	w[0]  = 1.;
	dw    = f.Reverse(1, w);
	ok   &= NearEqual(dw[0], 3.*x0*x0*x1*x1*x1, 1e-10, 1e-10);
	ok   &= NearEqual(dw[1], 3.*x0*x0*x1*x1*x0, 1e-10, 1e-10);

	return ok;
}

// END PROGRAM
