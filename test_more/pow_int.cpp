/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Old example now just used for validation testing.
*/

# include <cppad/cppad.hpp>

bool PowInt(void)
{	bool ok = true;

	using CppAD::pow;
	using CppAD::exp;
	using CppAD::log;
	using namespace CppAD;


	// independent variable vector, indices, values, and declaration
	CPPAD_TEST_VECTOR< AD<double> > U(1);
	U[0]     = 2.;
	Independent(U);

	// dependent variable vector and indices
	CPPAD_TEST_VECTOR< AD<double> > Z(2);

	// dependent variable values
	Z[0]         = pow(U[0], 5);     // x = u^5
	Z[1]         = pow(U[0], -5);    // y = u^{-5}

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z);
	CPPAD_TEST_VECTOR<double> v( f.Domain() );
	CPPAD_TEST_VECTOR<double> w( f.Range() );

	/*
	x_u = 5 * u^4
	y_u = - 5 * u^{-6}
	*/

	// check function values values
	double u = Value(U[0]);
	ok &= NearEqual(Z[0] , exp( log(u) * 5.),              1e-10 , 1e-10);
	ok &= NearEqual(Z[1] , exp( - log(u) * 5.),            1e-10 , 1e-10);

	// forward computation of partials 
	v[0] = 1.;
	w = f.Forward(1, v);
	ok &= NearEqual(w[0] , 5. * exp( log(u) * 4.),         1e-10 , 1e-10);
	ok &= NearEqual(w[1] , - 5. * exp( - log(u) * 6.),     1e-10 , 1e-10);

	return ok;
}
