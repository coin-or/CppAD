// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin to_string.cpp$$

$section to_string: Example and Test$$

$code
$verbatim%example/to_string.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>

namespace {
	template <class Base>
	bool test(void)
	{	bool  ok  = true;
		Base eps = std::numeric_limits<Base>::epsilon();
		Base pi  = 4.0 * std::atan(1.);

		std::string s = CppAD::to_string( CppAD::AD<Base>(pi) );
		Base check    = std::atof( s.c_str() );
		ok           &= std::fabs( check - pi ) <= 2.0 * eps;

		return ok;
	}
}

bool to_string(void)
{	bool ok = true;

	ok &= test<float>();
	ok &= test<double>();

	return ok;
}

// END C++
