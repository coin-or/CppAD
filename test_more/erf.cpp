/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Old example now just used for validation testing
*/

# include <cppad/cppad.hpp>

bool Erf(void)
{	bool ok = true;
	using namespace CppAD;
	using CppAD::atan;
	using CppAD::exp;
	using CppAD::sqrt;

	// Construct function object corresponding to erf
	CPPAD_TEST_VECTOR< AD<double> > X(1);
	CPPAD_TEST_VECTOR< AD<double> > Y(1);
	X[0] = 0.;
	Independent(X);
	Y[0] = erf(X[0]);
	ADFun<double> Erf(X, Y);

	// vectors to use with function object
	CPPAD_TEST_VECTOR<double> x(1);
	CPPAD_TEST_VECTOR<double> y(1);
	CPPAD_TEST_VECTOR<double> dx(1);
	CPPAD_TEST_VECTOR<double> dy(1);

	// check value at zero
	x[0]  = 0.;
	y = Erf.Forward(0, x);	
	ok &= NearEqual(0., y[0], 4e-4, 0.);

	// check the derivative of error function
	dx[0]         = 1.;
	double pi     = 4. * atan(1.);
	double factor = 2. / sqrt( pi );
	int i;
	for(i = -10; i <= 10; i++)
	{	x[0] = i / 4.;
		y    = Erf.Forward(0, x);	

		// check derivative
		double derf = factor * exp( - x[0] * x[0] );
		dy          = Erf.Forward(1, dx);
		ok         &= NearEqual(derf, dy[0], 0., 2e-3);

		// test using erf with AD< AD<double> >
		AD< AD<double> > X0 = x[0];
		AD< AD<double> > Y0 = erf(X0);

		ok &= ( y[0] == Value( Value(Y0) ) );
	}
	return ok;
}
