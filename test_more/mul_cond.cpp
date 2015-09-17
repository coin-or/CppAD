// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
Test of multi-level conditional expressions reverse mode
*/

# include <cppad/cppad.hpp>

bool mul_cond(void)
{
	bool ok = true;
	using CppAD::vector;
	double eps = 10. * std::numeric_limits<double>::epsilon();
	//
	typedef CppAD::AD<double>   a1double;
	typedef CppAD::AD<a1double> a2double;
	//
	a2double a2zero = a2double(0.0);
	a2double a2one  = a2double(1.0);
	//
	// --------------------------------------------------------------------
	// create a1f = f(x)
	size_t n = 1;
	size_t m = 2;
	vector<a2double> a2x(n), a2y(m);
	a2x[0] = a2double( 5.0 );
	Independent(a2x);

	// div
	a2y[0]  = CondExpGt(a2x[0], a2zero, a2one / a2x[0], a2zero);

	// abs
	a2y[1]  = CondExpGt(a2x[0], a2zero, abs( a2one / a2x[0] ), a2zero);

	CppAD::ADFun<a1double> a1f;
	a1f.Dependent(a2x, a2y);
	// --------------------------------------------------------------------
	// create g = sum_i f_i '(x)
	vector<a1double> a1x(n), a1dy(n), a1w(m);
	a1x[0] = 2.0;
	for(size_t i = 0; i < m; i++)
		a1w[i] = 1.0;
	//
	Independent(a1x);
	a1f.Forward(0, a1x);
	a1dy   = a1f.Reverse(1, a1w);
	CppAD::ADFun<double> g;
	g.Dependent(a1x, a1dy);
	// --------------------------------------------------------------------
	// check g where x[0] > 0
	vector<double> x(1), y(1);
	x[0] = 2.0;
	y    = g.Forward(0, x);
	double check = 0.0;
	check += -1.0 / (x[0] * x[0]); //  1 / x[0]
	check += -1.0 / (x[0] * x[0]); // abs( 1 / x[0] )
	ok  &= CppAD::NearEqual(y[0], check, eps, eps);
	// --------------------------------------------------------------------
	// check g where x[0] = 0
	x[0] = 0.0;
	y    = g.Forward(0, x);
	ok  &= CppAD::NearEqual(y[0], 0.0, eps, eps);
	// --------------------------------------------------------------------
	return ok;
}
