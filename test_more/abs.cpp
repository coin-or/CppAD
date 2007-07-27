/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Test of directional derivative in AD< AD< double > > case.
*/

# include <cppad/cppad.hpp>


bool Abs(void)
{	// test if CppAD::abs uses if statement for value computations
	bool ok = true;

	using CppAD::Independent;
	using CppAD::ADFun;
	typedef CppAD::AD<double>      ADdouble;
	typedef CppAD::AD< ADdouble > ADDdouble;

	CPPAD_TEST_VECTOR< ADdouble > u(1);
	u[0] = double(0);
	Independent(u);

	CPPAD_TEST_VECTOR< ADDdouble > v(1);
	v[0] = ADDdouble(u[0]);
	CppAD::Independent(v);

	CPPAD_TEST_VECTOR< ADDdouble > w(1);
	w[0] = CppAD::abs(v[0]);

	// f(v) = |w|
	CppAD::ADFun< ADdouble > f(v, w);

	// extract the value of w
	CPPAD_TEST_VECTOR< ADdouble > x(1);
	x[0] = CppAD::Value(w[0]);
	
	// g(u) = |u|
	CppAD::ADFun<double> g(u, x);

	// compute direction derivative of f at zero in positive direction
	CPPAD_TEST_VECTOR< ADdouble > dv(1);
	CPPAD_TEST_VECTOR< ADdouble > dw(1);
	dv[0] = 1;
	dw    = f.Forward(1, dv);
	ok   &= (dw[0] == 1);

	// compute direction derivative of g at zero in positive direction
	CPPAD_TEST_VECTOR<double> du(1);
	CPPAD_TEST_VECTOR<double> dx(1);
	du[0] = 1;
	dx    = g.Forward(1, du);
	ok   &= (dx[0] == 1);

	return ok;
}
