/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin CopyAD.cpp$$
$spell
	Cpp
$$

$section AD Copy Constructor: Example and Test$$

$index copy, AD object$$
$index example, copy AD object$$
$index test, copy AD object$$

$code
$verbatim%example/copy_ad.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>

bool CopyAD(void)
{	bool ok = true;   // initialize test result flag
	using CppAD::AD;  // so can use AD in place of CppAD::AD

	// domain space vector
	size_t n = 1;
	CPPAD_TEST_VECTOR< AD<double> > x(n);
	x[0]     = 2.;

	// declare independent variables and start tape recording
	CppAD::Independent(x);

	// create an AD<double> that does not depend on x
	AD<double> b = 3.;   

	// use copy constructor 
	AD<double> u(x[0]);    
	AD<double> v = b;

	// check which are parameters
	ok &= Variable(u);
	ok &= Parameter(v);

	// range space vector
	size_t m = 2;
	CPPAD_TEST_VECTOR< AD<double> > y(m);
	y[0]  = u;
	y[1]  = v;

	// create f: x -> y and vectors used for derivative calculations
	CppAD::ADFun<double> f(x, y);
	CPPAD_TEST_VECTOR<double> dx(n);
	CPPAD_TEST_VECTOR<double> dy(m);
 
 	// check parameters flags
 	ok &= ! f.Parameter(0);
 	ok &=   f.Parameter(1);

	// check function values
	ok &= ( y[0] == 2. );
	ok &= ( y[1] == 3. );

	// forward computation of partials w.r.t. x[0]
	dx[0] = 1.;
	dy    = f.Forward(1, dx);
	ok   &= ( dy[0] == 1. );   // du / dx
	ok   &= ( dy[1] == 0. );   // dv / dx

	return ok;
}

// END PROGRAM
