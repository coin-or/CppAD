/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin exp_2_for0.cpp$$
$spell
	std
	exp_2_seq
	cmath
	bool
	fabs
$$

$section exp_2: Verify Zero Order Forward Sweep$$

$index zero, order exp_2$$
$index order, zero exp_2$$
$index exp_2, zero order$$


$codep */
# include <cmath>          // for fabs function
bool exp_2_for0(double *v)  // double v[6]
{	bool  ok = true;
	double x = .5;

	v[1] = x;                                  // v1 = x
	ok  &= std::fabs( v[1] - 0.5) < 1e-10;

	v[2] = 1. + v[1];                          // v2 = 1 + v1
	ok  &= std::fabs( v[2] - 1.5) < 1e-10;

	v[3] = v[1] * v[1];                        // v3 = v1 * v1
	ok  &= std::fabs( v[3] - 0.25) < 1e-10;

	v[4] = v[3] / 2.;                          // v4 = v3 / 2
	ok  &= std::fabs( v[4] - 0.125) < 1e-10;

	v[5] = v[2] + v[4];                        // v5  = v2 + v4
	ok  &= std::fabs( v[5] - 1.625) < 1e-10;

	return ok;
}
bool exp_2_for0(void)
{	double v[6];
	return exp_2_for0(v);
}
/* $$
$end
*/
