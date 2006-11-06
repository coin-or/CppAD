/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Test of directional derivative in AD< AD< double > > case.
*/

# include <CppAD/CppAD.h>


bool Abs(void)
{	// test if CppAD::abs uses if statement for value computations
	bool ok = true;

	using CppAD::Independent;
	using CppAD::ADFun;
	typedef CppAD::AD<double>      ADdouble;
	typedef CppAD::AD< ADdouble > ADDdouble;

	CppADvector< ADdouble > u(1);
	u[0] = double(0);
	Independent(u);

	CppADvector< ADDdouble > v(1);
	v[0] = ADDdouble(u[0]);
	CppAD::Independent(v);

	CppADvector< ADDdouble > w(1);
	w[0] = CppAD::abs(v[0]);

	// f(v) = |w|
	CppAD::ADFun< ADdouble > f(v, w);

	// extract the value of w
	CppADvector< ADdouble > x(1);
	x[0] = CppAD::Value(w[0]);
	
	// g(u) = |u|
	CppAD::ADFun<double> g(u, x);

	// compute direction derivative of f at zero in positive direction
	CppADvector< ADdouble > dv(1);
	CppADvector< ADdouble > dw(1);
	dv[0] = 1;
	dw    = f.Forward(1, dv);
	ok   &= (dw[0] == 1);

	// compute direction derivative of g at zero in positive direction
	CppADvector<double> du(1);
	CppADvector<double> dx(1);
	du[0] = 1;
	dx    = g.Forward(1, du);
	ok   &= (dx[0] == 1);

	return ok;
}
