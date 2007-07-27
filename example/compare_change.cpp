/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin CompareChange.cpp$$
$spell
	Cpp
$$

$section CompareChange and Re-Tape: Example and Test$$

$index compare, change$$
$index example, CompareChange$$
$index test, CompareChange$$
$index re-tape, example$$
$index example, re-tape$$
$index test, re-tape$$

$code
$verbatim%example/compare_change.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>

namespace { // put this function in the empty namespace
	template <typename Type>
	Type Minimum(const Type &x, const Type &y)
	{	// Use a comparision to compute the min(x, y)
		// (note that CondExp would never require retaping). 
		if( x < y )  
			return x;
		return y;
	}
}

bool CompareChange(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::ADFun;
	using CppAD::Independent;

	// domain space vector
	size_t n = 2;
	CPPAD_TEST_VECTOR< AD<double> > X(n);
	X[0] = 3.;
	X[1] = 4.;

	// declare independent variables and start tape recording
	CppAD::Independent(X);

	// range space vector
	size_t m = 1;
	CPPAD_TEST_VECTOR< AD<double> > Y(m);
	Y[0] = Minimum(X[0], X[1]);

	// create f: x -> y and stop tape recording
	ADFun<double> f(X, Y);

	// evaluate zero mode Forward where conditional has the same result
	// note that f.CompareChange is not defined when NDEBUG is true
	CPPAD_TEST_VECTOR<double> x(n);
	CPPAD_TEST_VECTOR<double> y(m);
	x[0] = 3.5;
	x[1] = 4.;  
	y    = f.Forward(0, x);
	ok  &= (y[0] == x[0]);
	ok  &= (y[0] == Minimum(x[0], x[1]));
	ok  &= (f.CompareChange() == 0);

	// evaluate zero mode Forward where conditional has different result
	x[0] = 4.;
	x[1] = 3.;
	y    = f.Forward(0, x);
	ok  &= (y[0] == x[0]);
	ok  &= (y[0] != Minimum(x[0], x[1]));
	ok  &= (f.CompareChange() == 1); 

	// re-tape to obtain the new AD operation sequence
	X[0] = 4.;
	X[1] = 3.;
	Independent(X);
	Y[0] = Minimum(X[0], X[1]);

	// stop tape and store result in f
	f.Dependent(Y);

	// evaluate the function at new argument values
	y    = f.Forward(0, x);
	ok  &= (y[0] == x[1]);
	ok  &= (y[0] == Minimum(x[0], x[1]));
	ok  &= (f.CompareChange() == 0); 

	return ok;
}


// END PROGRAM
