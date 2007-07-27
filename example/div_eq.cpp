/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin DivEq.cpp$$

$section AD Computed Assignment Division: Example and Test$$

$index /=, AD example$$
$index computed, AD assignment divide example$$
$index assignment, AD computed divide example$$
$index example, AD computed assignment divide$$
$index test, AD computed assignment divide$$

$index computed, /= example$$
$index assign, /= example$$
$index plus, /= example$$
$index add, /= example$$

$code
$verbatim%example/div_eq.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>

bool DivEq(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;

	// domain space vector
	size_t  n = 1;
	double x0 = .5;
	CPPAD_TEST_VECTOR< AD<double> > x(n);
	x[0]      = x0; 

	// declare independent variables and start tape recording
	CppAD::Independent(x);

	// range space vector 
	size_t m = 2;
	CPPAD_TEST_VECTOR< AD<double> > y(m);
	y[0] = x[0] * x[0];  // initial value
	y[0] /= 2;           // AD<double> /= int
	y[0] /= 4.;          // AD<double> /= double
	y[1] = y[0] /= x[0]; // use the result of a computed assignment

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y); 

	// check value 
	ok &= NearEqual(y[0] , x0*x0/(2.*4.*x0),  1e-10 , 1e-10);
	ok &= NearEqual(y[1] ,             y[0],  1e-10 , 1e-10);

	// forward computation of partials w.r.t. x[0]
	CPPAD_TEST_VECTOR<double> dx(n);
	CPPAD_TEST_VECTOR<double> dy(m);
	dx[0] = 1.;
	dy    = f.Forward(1, dx);
	ok   &= NearEqual(dy[0], 1./8., 1e-10, 1e-10);
	ok   &= NearEqual(dy[1], 1./8., 1e-10, 1e-10);

	// reverse computation of derivative of y[0]
	CPPAD_TEST_VECTOR<double>  w(m);
	CPPAD_TEST_VECTOR<double> dw(n);
	w[0]  = 1.;
	w[1]  = 0.;
	dw    = f.Reverse(1, w);
	ok   &= NearEqual(dw[0], 1./8., 1e-10, 1e-10);

	// use a VecAD<Base>::reference object with computed division
	CppAD::VecAD<double> v(1);
	AD<double> zero(0);
	AD<double> result = 2;
	v[zero] = 1;
	result /= v[zero];
	ok     &= (result == 2);

	return ok;
}

// END PROGRAM
