/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin exp_apx_for.cpp$$
$spell
	std
	vars
	exp_apx_for
	cmath
	fabs
	bool
$$

$section exp_apx Forward Mode Verification$$
$codep */

# include <cmath>                           // for fabs function
extern bool exp_apx_seq(void);              // prototype for exp_apx_seq
extern double a[1], q[3], r[3], s[3], k[3]; // global vars set by exp_apx_seq
bool exp_apx_for(void)
{	bool ok = true;
	double a_x[1], q_x[3], r_x[3], s_x[3];

	// make sure global variables have been computed by exp_apx_seq
	ok &= exp_apx_seq();

	// initial r and s values are parameters
	r_x[0] = s_x[0] = 0.;

	a_x[0] = 1.;                                         // a = x
	ok    &= std::fabs( a_x[0] - 1. ) <= 1e-10;

	q_x[1] = r_x[0] * a[0] + r[0] * a_x[0];              // q = r * a
	ok    &= std::fabs( q_x[1] - 1. ) <= 1e-10;

	r_x[1] = q_x[1] / k[0];                              // r = q / k
	ok    &= std::fabs( r_x[1] - 1. ) <= 1e-10;

	s_x[1] = s_x[0] + r_x[1];                           // s = s + r
	ok    &= std::fabs( r_x[1] - 1. ) <= 1e-10;

	q_x[2] = r_x[1] * a[0] + r[1] * a_x[0];              // q = r * a
	ok    &= std::fabs( q_x[2] - 1. ) <= 1e-10;

	r_x[2] = q_x[2] / k[1];                              // r = q / k
	ok    &= std::fabs( r_x[2] - 0.5 ) <= 1e-10;

	s_x[2] = s_x[1] + r_x[2];                           // s = s + r
	ok    &= std::fabs( s_x[2] - 1.5 ) <= 1e-10;

	return ok;
}
/* $$
$end
*/
