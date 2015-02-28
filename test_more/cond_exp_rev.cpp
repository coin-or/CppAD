// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

// Test that reverse mode handles conditional expressions properly
// in that infinity and nans do not propagate thouh un-used case.

# include <cppad/cppad.hpp>

bool cond_exp_rev(void)
{	bool ok = true;
	using CppAD::vector;
	using CppAD::AD;

	AD<double> anan  = std::numeric_limits<double>::quiet_NaN();
	AD<double> azero = 0.0;


	size_t n = 2;
	size_t m = 1;
	vector< AD<double> > ax(n), ay(m);
	ax[0] = 1.0;
	ax[1] = anan;
	Independent(ax);

	// count operators
	size_t i_op = 0;

	// AbsOp
	ay[i_op++] = CondExpGt(ax[0], azero, ax[0], abs(ax[1]) );

	// create f : x -> y
	CppAD::ADFun<double> f(ax, ay);

	// weighting vector and reverse mode derivative
	vector<double> w(m), dw(n);
	for(size_t i = 0; i < m; i++)
		w[i] = 0.0;

	// check DivvOp
	ok &= i_op == m;
	for(i_op = 0; i_op < m; i_op++)
	{	w[i_op] = 1.0;
		dw = f.Reverse(1, w);
		ok &= dw[0] == 1.0;
		ok &= dw[1] == 0.0;
		w[i_op] = 0.0;
	}

	return ok;
}
