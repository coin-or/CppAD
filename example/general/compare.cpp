/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin compare.cpp$$
$spell
	Cpp
$$

$section AD Binary Comparison Operators: Example and Test$$
$mindex compare < <= > >= == !=$$



$code
$srcfile%example/general/compare.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>

bool Compare(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	double eps99 = 99.0 * std::numeric_limits<double>::epsilon();

	// declare independent variables and start tape recording
	size_t n  = 2;
	double x0 = 0.5;
	double x1 = 1.5;
	CPPAD_TESTVECTOR(AD<double>) x(n);
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
	CPPAD_TESTVECTOR(AD<double>) y(m);
	y[0] = p;

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y);

	// check value
	ok &= NearEqual(y[0] , x0*x0*x1*x1*x1*x0, eps99, eps99);

	// forward computation of partials w.r.t. x[0]
	CPPAD_TESTVECTOR(double) dx(n);
	CPPAD_TESTVECTOR(double) dy(m);
	dx[0] = 1.;
	dx[1] = 0.;
	dy    = f.Forward(1, dx);
	ok   &= NearEqual(dy[0], 3.*x0*x0*x1*x1*x1, eps99, eps99);

	// forward computation of partials w.r.t. x[1]
	dx[0] = 0.;
	dx[1] = 1.;
	dy    = f.Forward(1, dx);
	ok   &= NearEqual(dy[0], 3.*x0*x0*x1*x1*x0, eps99, eps99);

	// reverse computation of derivative of y[0]
	CPPAD_TESTVECTOR(double)  w(m);
	CPPAD_TESTVECTOR(double) dw(n);
	w[0]  = 1.;
	dw    = f.Reverse(1, w);
	ok   &= NearEqual(dw[0], 3.*x0*x0*x1*x1*x1, eps99, eps99);
	ok   &= NearEqual(dw[1], 3.*x0*x0*x1*x1*x0, eps99, eps99);

	return ok;
}

// END C++
