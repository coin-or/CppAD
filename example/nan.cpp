/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin nan.cpp$$

$section nan: Example and Test$$

$index nan, example$$
$index example, nan$$
$index test, nan$$

$code
$verbatim%example/nan.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/

// these definitions will be erased when nan.hpp is included
# ifdef nan
# else
# define nan(z)   Error_if_this_symbol_gets_used
# endif
# ifdef isnan
# else
# define isnan(z) Error_if_this_symbol_gets_used
# endif



// BEGIN PROGRAM
# include <cppad/nan.hpp>
# include <vector>

bool nan(void)
{	bool ok = true;

	// get a nan
	double double_zero = 0.;
	double double_nan = CppAD::nan(double_zero);

	// create a simple vector with no nans
	std::vector<double> v(2);
	v[0] = double_zero;
	v[1] = double_zero;

	// check that zero is not nan
	ok &= ! CppAD::isnan(double_zero);
	ok &= ! CppAD::hasnan(v);

	// check that nan is a nan
	v[1] = double_nan;
	ok &= CppAD::isnan(double_nan);
	ok &= CppAD::hasnan(v);

	return ok;
}

// END PROGRAM
