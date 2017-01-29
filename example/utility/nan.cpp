/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin nan.cpp$$

$section nan: Example and Test$$


$code
$srcfile%example/utility/nan.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/

// BEGIN C++
# include <cppad/utility/nan.hpp>
# include <vector>
# include <limits>

bool nan(void)
{	bool ok = true;

	// get a nan
	double double_zero = 0.;
	double double_nan = std::numeric_limits<double>::quiet_NaN();

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

	// check that nan is not equal to itself
	ok &= (double_nan != double_nan);

	return ok;
}

// END C++
