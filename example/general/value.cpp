/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin value.cpp$$
$spell
	Cpp
	cstddef
$$

$section Convert From AD to its Base Type: Example and Test$$
$mindex Value record$$


$code
$srcfile%example/general/value.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++

# include <cppad/cppad.hpp>

bool Value(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::Value;

	// domain space vector
	size_t n = 2;
	CPPAD_TESTVECTOR(AD<double>) x(n);
	x[0] = 3.;
	x[1] = 4.;

	// check value before recording
	ok &= (Value(x[0]) == 3.);
	ok &= (Value(x[1]) == 4.);

	// declare independent variables and start tape recording
	CppAD::Independent(x);

	// range space vector
	size_t m = 1;
	CPPAD_TESTVECTOR(AD<double>) y(m);
	y[0] = - x[1];

	// cannot call Value(x[j]) or Value(y[0]) here (currently variables)
	AD<double> p = 5.;        // p is a parameter (does not depend on x)
	ok &= (Value(p) == 5.);

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y);

	// can call Value(x[j]) or Value(y[0]) here (currently parameters)
	ok &= (Value(x[0]) ==  3.);
	ok &= (Value(x[1]) ==  4.);
	ok &= (Value(y[0]) == -4.);

	return ok;
}
// END C++
