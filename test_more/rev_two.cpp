/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cppad/cppad.hpp>

bool RevTwo()
{	bool ok = true;
	using CppAD::AD;
	using CppAD::vector;
	using CppAD::NearEqual;


	size_t n = 2;
   	vector< AD<double> > X(n);
	X[0] = 1.;
	X[1] = 1.;
	Independent(X);

	size_t m = 1;
   	vector< AD<double> > Y(m);
	Y[0] = X[0] * X[0] + X[0] * X[1] + 2. * X[1] * X[1];
	CppAD::ADFun<double> F(X,Y);

	vector<double> x(n);
	x[0] = .5;
	x[1] = 1.5;

	size_t L = 1;
	vector<size_t> I(L);
	vector<size_t> J(L);
	vector<double> H(n);
	I[0] = 0;
	J[0] = 0;
	H    = F.RevTwo(x, I, J);
	ok  &= NearEqual(H[0], 2., 1e-10, 1e-10);
	ok  &= NearEqual(H[1], 1., 1e-10, 1e-10);
	J[0] = 1;
	H    = F.RevTwo(x, I, J);
	ok  &= NearEqual(H[0], 1., 1e-10, 1e-10);
	ok  &= NearEqual(H[1], 4., 1e-10, 1e-10);

	return ok;
}
