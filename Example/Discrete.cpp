/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Discrete.cpp$$

$section Discrete AD Functions: Example and Test$$

$index discrete$$
$index example, discrete$$
$index test, discrete$$

$code
$verbatim%Example/Discrete.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <CppAD/CppAD.h>

namespace {
	double TableLookup(const double &x)
	{	static double Table[] = {
			5.,
			4.,
			3.,
			2.,
			1.
		};
		static size_t number = sizeof(Table) / sizeof(Table[0]);
		size_t i;
		if( x < 0. )
			return Table[0];

		i = static_cast<size_t>(x);
		if( i >= number )
			return Table[number-1];

		return Table[i];
	}
	// in empty namespace and outside any other routine
	CppADCreateDiscrete(double, TableLookup)
}


bool Discrete(void)
{	bool ok = true;
	using CppAD::AD;

	// domain space vector
	size_t n = 2;
	CppADvector< AD<double> > X(n);
	X[0] = 2.;
	X[1] = 3.;

	// declare independent variables and start tape recording
	CppAD::Independent(X);

	// range space vector
	size_t m = 1;
	CppADvector< AD<double> > Y(m);
	Y[0] = X[1] * TableLookup( X[0] );

	// create f: X -> Y and stop tape recording
	CppAD::ADFun<double> f(X, Y);

	// vectors for arguments to the fucntion object f
	CppADvector<double> x(n);   // argument values
	CppADvector<double> y(m);   // function values 
	CppADvector<double> w(m);   // function weights 
	CppADvector<double> dw(n);  // derivative of weighted function

	// check function value
	x[0] = Value(X[0]);
	x[1] = Value(X[1]);
	y[0] = Value(Y[0]);
	ok  &= y[0] == x[1] * TableLookup(x[0]);

	// evaluate f where x has different values
	x[0] = x[0] + 1.;
	x[1] = x[1] + 1.;
	y    = f.Forward(0, x);
	ok  &= y[0] == x[1] * TableLookup(x[0]);

	// evaluate derivaitve of y[0] 
	w[0] = 1.;
	dw   = f.Reverse(1, w);
	ok   &= dw[0] == 0.;
	ok   &= dw[1] == TableLookup(x[0]);

	return ok;
}

// END PROGRAM
