/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ad_input.cpp$$
$spell
	Cpp
	cstddef
$$

$section AD Output Operator: Example and Test$$

$index <<, AD example$$
$index input, AD example$$
$index example, AD input$$
$index test, AD input$$

$code
$verbatim%example/ad_input.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++

# include <cppad/cppad.hpp>

# include <sstream>  // std::istringstream
# include <string>   // std::string

bool ad_input(void)
{	bool ok = true;

	std::string str ("123 456");
	std::istringstream is(str);

	CppAD::AD<double> x(1.), y(2.);

	is >> x >> y;

	ok &= (x == 123.);
	ok &= (y == 456.);

	return ok;
}
// END C++
