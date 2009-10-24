/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin fun_assign.cpp$$

$section ADFun Assignment: Example and Test$$

$index ADFun, assignment example$$
$index example, ADFun assignment$$
$index assignment, ADFun example$$

$code
$verbatim%example/fun_assign.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>
# include <limits>

bool fun_assign(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;

	// ten times machine percision
	double eps = 10. * std::numeric_limits<double>::epsilon();

	// two ADFun<double> objects
	CppAD::ADFun<double> f, g;

	// domain space vector
	size_t n  = 1;
	CPPAD_TEST_VECTOR< AD<double> > x(n);
	x[0]      = .5; 

	// declare independent variables and start tape recording
	CppAD::Independent(x);

	// range space vector 
	size_t m = 1;
	CPPAD_TEST_VECTOR< AD<double> > y(m);
	y[0] = 1. + 2. * x[0] + 3. * x[0] * x[0];

	// store operation sequence in f
	f.Dependent(x, y);

	// make a copy in g
	g = f;

	// zero order forward mode at two different points
	// (this could be useful if f and g were executed in parallel)
	CPPAD_TEST_VECTOR<double> x_f(n), x_g(n), y_f(m), y_g(m);
	x_f[0] = 1.;
	x_g[0] = 2.;
	y_f    = f.Forward(0, x_f);
	y_g    = g.Forward(0, x_g);
	ok   &= NearEqual(y_f[0], 1.+2.*x_f[0]+3.*x_f[0]*x_f[0], eps, eps);
	ok   &= NearEqual(y_g[0], 1.+2.*x_g[0]+3.*x_g[0]*x_g[0], eps, eps);

	// first order reverse mode 
	// (this could be useful if f and g were executed in parallel)
	CPPAD_TEST_VECTOR<double> wf(m), wg(m), dy_f(n), dy_g(n);
	wf[0] = 1.;
	wg[0] = 1.;
	dy_f  = f.Reverse(1, wf);
	dy_g  = g.Reverse(1, wg);
	ok   &= NearEqual(dy_f[0], 2.+6.*x_f[0], eps, eps);
	ok   &= NearEqual(dy_g[0], 2.+6.*x_g[0], eps, eps);

	return ok;
}

// END PROGRAM
