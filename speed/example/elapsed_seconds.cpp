// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin elapsed_seconds.cpp$$
$spell
	Cpp
	Lu
$$

$section Elapsed Seconds: Example and Test$$
$mindex seconds timer$$


$code
$srcfile%speed/example/elapsed_seconds.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/utility/elapsed_seconds.hpp>

# include <iostream>
# include <algorithm>
# include <cmath>

# define CPPAD_DEBUG_ELAPSED_SECONDS 0

bool elapsed_seconds(void)
{	bool ok = true;

	double max_diff = 0.;
	double s0 = CppAD::elapsed_seconds();
	double s1 = CppAD::elapsed_seconds();
	double s2 = CppAD::elapsed_seconds();
	while(s2 - s0 < 1.)
	{	max_diff = std::max(s2 - s1, max_diff);
		s1 = s2;
		s2 = CppAD::elapsed_seconds();

	}
# if CPPAD_DEBUG_ELAPSED_SECONDS
	std::cout << "max_diff = " << max_diff << std::endl;
# endif
	ok &= 0. < max_diff && max_diff < .04;
	return ok;
}

// END C++
