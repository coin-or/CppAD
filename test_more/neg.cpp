/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
old example and test now only used for testing
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>

bool Neg(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector, indices, values, and declaration
	CPPAD_TEST_VECTOR< AD<double> > U(2);
	size_t s = 0;
	size_t t = 1;
	U[s]     = 3.;
	U[t]     = 4.;
	Independent(U);

	// dependent variable vector and indices
	CPPAD_TEST_VECTOR< AD<double> > Z(1);
	size_t x = 0;

	// dependent variable values
	Z[x] = - U[t];   //  - AD<double> 

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z);
	CPPAD_TEST_VECTOR<double> v( f.Domain() );
	CPPAD_TEST_VECTOR<double> w( f.Range() );

	// check values
	ok &= ( Z[x] == -4. );

	// forward computation of partials w.r.t. s
	v[s] = 1.;
	v[t] = 0.;
	w    = f.Forward(1, v);
	ok &= ( w[x] == 0. );   // dx/ds

	// forward computation of partials w.r.t. t
	v[s] = 0.;
	v[t] = 1.;
	w    = f.Forward(1, v);
	ok &= ( w[x] == -1. );   // dx/dt

	// reverse computation of second partials of z
	CPPAD_TEST_VECTOR<double> r( f.Domain() * 2 );
	w[x] = 1.;
	r    = f.Reverse(2, w);
	ok &= ( r[2 * s + 1] == 0. );  // d^2 x / (ds ds)
	ok &= ( r[2 * t + 1] == 0. );  // d^2 x / (ds dt)
	 
	return ok;
}
// END PROGRAM
