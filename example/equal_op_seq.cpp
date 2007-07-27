/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin EqualOpSeq.cpp$$
$spell
	Op
$$

$section EqualOpSeq: Example and Test$$

$index EqualOpSeq, example$$
$index example, EqualOpSeq$$
$index test, EqualOpSeq$$

$code
$verbatim%example/equal_op_seq.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>

bool EqualOpSeq(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::EqualOpSeq;

	// domain space vector
	size_t n  = 1;
	double x0 = 1.;
	CPPAD_TEST_VECTOR< AD<double> > x(n);
	x[0]      = x0; 

	// declare independent variables and start tape recording
	CppAD::Independent(x);

	AD<double> a = 1. + x[0];  // this variable is 1 + x0
	AD<double> b = 2. * x[0];  // this variable is 2 * x0

	// both a and b are variables
	ok &= (a == b);            // 1 + 1     == 2 * 1
	ok &= ! EqualOpSeq(a, b);  // 1 + x[0]  != 2 * x[0] 

	// range space vector 
	size_t m = 1;
	CPPAD_TEST_VECTOR< AD<double> > y(m);
	y[0] = a;

	// both y[0] and a are variables
	EqualOpSeq(y[0], a);       // 2 * x[0] == 2 * x[0]

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y); 

	// both a and b are parameters (after the creation of f above)
	ok &= EqualOpSeq(a, b);    // 1 + 1 == 2 * 1

	return ok;
}

// END PROGRAM
