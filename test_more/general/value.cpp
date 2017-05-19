/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Old Value example now used just for valiadation testing
*/
// BEGIN C++

# include <cppad/cppad.hpp>

bool Value(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector, indices, values, and declaration
	CPPAD_TESTVECTOR(AD<double>) U(2);
	size_t s = 0;
	size_t t = 1;
	U[s] = 3.;
	U[t] = 4.;
	Independent(U);

	// cannot call Value after Independent (tape is recording)

	// dependent variable vector and indices
	CPPAD_TESTVECTOR(AD<double>) Z(1);
	size_t x = 0;

	// dependent variable values
	Z[x] = - U[t];

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z);
	CPPAD_TESTVECTOR(double) v( f.Domain() );
	CPPAD_TESTVECTOR(double) w( f.Range() );

	// can call Value after ADFun constructor (tape is no longer recording)

	// check value of s
	double sValue = Value(U[s]);
	ok &= ( sValue == 3. );

	// check value of x
	double xValue = Value(Z[x]);
	ok &= ( xValue == -4. );

	return ok;
}
// END C++
