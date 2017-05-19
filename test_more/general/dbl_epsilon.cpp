/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Check the value of machine epsilon is accurate enough for the correctness tests
*/

# include <cfloat>
# include <limits>

bool dbl_epsilon(void)
{	bool ok = true;

	// CppAD correctness tests assume machine epsilon is less than 1e-13
	ok &= DBL_EPSILON < 1e-13;
	ok &= std::numeric_limits<double>::digits10 >= 13;

	return ok;
}
