/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin exp_eps_for.cpp$$
$spell
	std
	vars
	exp_eps_for
	cmath
	fabs
	bool
$$

$section exp_eps: Verify First Order Forward Sweep$$

$index first, order exp_2$$
$index order, first exp_2$$
$index exp_2, first order$$

$codep */
# include <cmath>                   // for fabs function
extern bool exp_eps_seq(double *v); // computes zero order forward sweep
bool exp_eps_for(void)
{	bool ok = true;
	double v[8], v_x[8];

	// set the value of v[j] for j = 1 , ... , 7
	ok &= exp_eps_seq(v);

	v_x[1] = 1.;                                       // v_1 = x
	ok    &= std::fabs( v_x[1] - 1. ) <= 1e-10;

	v_x[2] = 1. * v_x[1];                              // v_2 = 1 * v_1
	ok    &= std::fabs( v_x[2] - 1. ) <= 1e-10;

	v_x[3] = v_x[2] / 1.;                              // v_3 = v_2 / 1
	ok    &= std::fabs( v_x[3] - 1. ) <= 1e-10;

	v_x[4] = v_x[3];                                   // v_4 = 1 + v_3
	ok    &= std::fabs( v_x[4] - 1. ) <= 1e-10;

	v_x[5] = v_x[3] * v[1] + v[3] * v_x[1];            // v_5 = v_3 * v_1
	ok    &= std::fabs( v_x[5] - 1. ) <= 1e-10;

	v_x[6] = v_x[5] / 2.;                              // v_6 = v_5 / 2
	ok    &= std::fabs( v_x[6] - 0.5 ) <= 1e-10;

	v_x[7] = v_x[4] + v_x[6];                          // v_7 = v_4 + v_6
	ok    &= std::fabs( v_x[7] - 1.5 ) <= 1e-10;

	return ok;
}
/* $$
$end
*/
