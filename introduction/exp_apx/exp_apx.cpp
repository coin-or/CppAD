/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin exp_apx.cpp$$
$spell
	exp_apx
$$

$section exp_apx: Example and Test$$

$index exp_apx, introduction$$
$index introduction, exp_apx$$

$code
$verbatim%introduction/exp_apx/exp_apx.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cmath>             // for fabs function
# include "exp_apx.hpp"       // definition of exp_apx algorithm
bool exp_apx(void)
{	double x     = .5;
	double e     = .2;
	double check = 1 + .5 + .125; // include 1 and only 1 term less than e
	bool   ok    = std::fabs( exp_apx(x, e) - check ) <= 1e-10; 
	return ok;
}
// END PROGRAM
