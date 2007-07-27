/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Add.cpp$$

$section AD Binary Addition: Example and Test$$

$index +, AD example$$
$index add, AD example$$
$index plus, AD example$$
$index example, AD add$$
$index test, AD add$$

$code
$verbatim%example/add.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>

bool Add(void)
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

	// some binary addition operations
	AD<double> a = x[0] + 1.; // AD<double> + double
	AD<double> b = a    + 2;  // AD<double> + int
	AD<double> c = 3.   + b;  // double     + AD<double> 
	AD<double> d = 4    + c;  // int        + AD<double> 

	// range space vector 
	size_t m = 1;
	CPPAD_TEST_VECTOR< AD<double> > y(m);
	y[0] = d + x[0];          // AD<double> + AD<double> 

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y); 

	// check value 
	ok &= NearEqual(y[0] , 2. * x0 + 10,  1e-10 , 1e-10);

	// forward computation of partials w.r.t. x[0]
	CPPAD_TEST_VECTOR<double> dx(n);
	CPPAD_TEST_VECTOR<double> dy(m);
	dx[0] = 1.;
	dy    = f.Forward(1, dx);
	ok   &= NearEqual(dy[0], 2., 1e-10, 1e-10);

	// reverse computation of derivative of y[0]
	CPPAD_TEST_VECTOR<double>  w(m);
	CPPAD_TEST_VECTOR<double> dw(n);
	w[0]  = 1.;
	dw    = f.Reverse(1, w);
	ok   &= NearEqual(dw[0], 2., 1e-10, 1e-10);

	// use a VecAD<Base>::reference object with addition
	CppAD::VecAD<double> v(1);
	AD<double> zero(0);
	v[zero] = a;
	AD<double> result = v[zero] + 2;
	ok     &= (result == b);

	return ok;
}

// END PROGRAM
