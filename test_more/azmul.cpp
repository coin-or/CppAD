/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/cppad.hpp>
# include <cmath>

bool azmul(void)
{	bool ok = true;

	using CppAD::AD;
	using CppAD::NearEqual;
	double inf = std::numeric_limits<double>::infinity();
	double eps = 10. * std::numeric_limits<double>::epsilon();

	typedef AD<double>    a1double;
	typedef AD<a1double>  a2double;

	// domain space vector
	size_t n = 2;
	size_t m = 3;

	// double level
	CPPAD_TESTVECTOR(double) x(n), y(n);
	x[0] = 3.0;
	x[1] = 4.0;

	// a1 level
	CPPAD_TESTVECTOR(a1double) a1x(n), a1y(m);
	for(size_t j = 0; j < n; j++)
		a1x[j] = x[j];
	CppAD::Independent(a1x);

	// a2 level
	CPPAD_TESTVECTOR(a2double) a2x(n), a2y(m);
	for(size_t j = 0; j < n; j++)
		a2x[j] = a1x[j];
	CppAD::Independent(a2x);

	// a2 recording
	a2y[0] = CppAD::azmul(a2x[0], a2x[1]); // azmul(variable, variable)
	a2y[1] = CppAD::azmul(a1x[0], a2x[1]); // azmul(parameter, variable)
	a2y[2] = CppAD::azmul(a2x[0], a1x[1]); // azmul(variable, parameter)

	// create a1f: x -> y and stop a2 recording
	CppAD::ADFun<a1double> a1f;
	a1f.Dependent(a2x, a2y);

	// a1 recording
	a1y = a1f.Forward(0, a1x);            // azmul(variable, variable) only

	// create f: x -> y and stop a1 recording
	CppAD::ADFun<double> f;
	f.Dependent(a1x, a1y);

	// check value when x[0] is not zero
	y = f.Forward(0, x);
	for(size_t i = 0; i < m; i++)
		ok &= NearEqual(y[i] , x[0] * x[1],  eps, eps);

	// check value x[0] is zero and x[1] is infinity
	x[0] = 0.0;
	x[1] = inf;
	y    = f.Forward(0, x);
	for(size_t i = 0; i < m; i++)
		ok &= y[i] ==  0.0;

	return ok;
}
