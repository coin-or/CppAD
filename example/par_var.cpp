/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ParVar.cpp$$

$section AD Parameter and Variable Functions: Example and Test$$

$index Parameter, example$$
$index Variable, example$$

$index example, Parameter$$
$index example, Variable$$

$index test, Parameter$$
$index test, Variable$$

$code
$verbatim%example/par_var.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>

bool ParVar(void)
{	bool ok = true;

	using CppAD::AD;
	using CppAD::VecAD;
	using CppAD::Parameter;
	using CppAD::Variable;

	// declare independent variables and start tape recording
	size_t n = 1;
	CPPAD_TEST_VECTOR< AD<double> > x(n);
	x[0]     = 0.;
	ok &= Parameter(x[0]);     // x[0] is a paraemter here
	CppAD::Independent(x);
	ok &= Variable(x[0]);      // now x[0] is a variable 

	// dependent variable vector
	size_t m = 2;
	CPPAD_TEST_VECTOR< AD<double> > y(m);
	y[0] = 2.;
	ok  &= Parameter(y[0]);    // y[0] does not depend on x[0]
	y[1] = abs(x[0]);
	ok  &= Variable(y[1]);     // y[1] does depends on x[0] 

	// VecAD objects
	VecAD<double> z(2);
	z[0] = 0.;
	z[1] = 1.;
	ok  &= Parameter(z);      // z does not depend on x[0]
	z[x[0]] = 2.;
	ok  &= Variable(z);       // z depends on x[0]
	

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y);

	// check that now all AD<double> objects are parameters
	ok &= Parameter(x[0]); ok &= ! Variable(x[0]);
	ok &= Parameter(y[0]); ok &= ! Variable(y[0]);
	ok &= Parameter(y[1]); ok &= ! Variable(y[1]);

	// check that the VecAD<double> object is a parameter
	ok &= Parameter(z);

	return ok;
}

// END PROGRAM
