/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Default.cpp$$
$spell
	Cpp
$$

$section Default AD Constructor: Example and Test$$

$index AD, default construct$$
$index construct, , AD default$$
$index example, default AD construct$$
$index test, default AD construct$$


$code
$verbatim%example/default.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>

bool Default(void)
{	bool ok = true;
	using CppAD::AD;

	// default AD constructor
	AD<double> x, y;

	// check that they are parameters
	ok &= Parameter(x);
	ok &= Parameter(y);

	// assign them values
	x = 3.; 
	y = 4.;

	// just check a simple operation
	ok &= (x + y == 7.);

	return ok;
}

// END PROGRAM
