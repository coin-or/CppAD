/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Old Copy example now used just for valiadation testing
*/

# include <cppad/cppad.hpp>

bool Copy(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector, indices, values, and declaration
	CPPAD_TEST_VECTOR< AD<double> > U(1);
	size_t is = 0;
	U[is]     = 2.;
	Independent(U);

	// create an AD<double> that does not depend on s
	AD<double> t = 3.;   

	// use copy constructor 
	AD<double> x(U[is]);    
	AD<double> y(t);

	// check which are parameters
	ok &= ! Parameter(x);
	ok &= Parameter(y);

	// dependent variable vector, indices, and values
	CPPAD_TEST_VECTOR< AD<double> > Z(2);
	size_t ix = 0;
	size_t iy = 1;
	Z[ix]     = x;
	Z[iy]     = y;

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z);
	CPPAD_TEST_VECTOR<double> v( f.Domain() );
	CPPAD_TEST_VECTOR<double> w( f.Range() );
 
 	// check parameters flags
 	ok &= ! f.Parameter(ix);
 	ok &=   f.Parameter(iy);

	// check function values
	ok &= ( Z[ix] == 2. );
	ok &= ( Z[iy] == 3. );

	// forward computation of partials w.r.t. s
	v[is] = 1.;
	w     = f.Forward(1, v);
	ok &= ( w[ix] == 1. );   // dx/ds
	ok &= ( w[iy] == 0. );   // dy/ds

	return ok;
}

// END PROGRAM
