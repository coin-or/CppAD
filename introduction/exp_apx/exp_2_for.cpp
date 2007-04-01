/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin exp_2_for.cpp$$
$spell
	std
	exp_2_for
	cmath
	fabs
	bool
$$

$section exp_2: Verify First Order Forward Sweep$$

$index first, order exp_2$$
$index order, first exp_2$$
$index exp_2, first order$$

$codep */
# include <cmath>                 // prototype for fabs
extern bool exp_2_seq(double *v); // computes zero order forward sweep
bool exp_2_for(void)
{	bool ok = true;
	double v[6], v_x[6];

	// set the value of v[j] for j = 1 , ... , 5
	ok &= exp_2_seq(v);

	v_x[1] = 1.;                                      // v_1 = x
	ok    &= std::fabs( v_x[1] - 1. ) <= 1e-10;

	v_x[2] = v_x[1];                                  // v_2 = 1 + v_1
	ok    &= std::fabs( v_x[2] - 1. ) <= 1e-10;

	v_x[3] = v_x[1] * v[1] + v[1] * v_x[1];           // v_3 = v_1 * v_1
	ok    &= std::fabs( v_x[3] - 1. ) <= 1e-10;

	v_x[4] = v_x[3] / 2.;                             // v_4 = v_3 / 2
	ok    &= std::fabs( v_x[4] - 0.5) <= 1e-10;

	v_x[5] = v_x[2] + v_x[4];                         // v_5 = v_2 + v_4
	ok    &= std::fabs( v_x[5] - 1.5) <= 1e-10;

	return ok;
}
/* $$
$end
*/
