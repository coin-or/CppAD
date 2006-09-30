/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin ExpApxRev.cpp$$
$spell
	cstddef
	cmath
	vars
	ir
	iq
	ir
	ia
	df
	ExpApxRev
	bool
	std
	fabs
$$

$section ExpApx Reverse Mode Verification$$
$codep */
# include <cstddef>                         // for size_t
# include <cmath>                           // for fabs function
extern bool ExpApxSeq(void);                // prototype for ExpApxSeq
extern double a[1], q[3], r[3], s[3], k[3]; // global vars set by ExpApxSeq
bool ExpApxRev(void)
{	bool ok = true;

	// ordering of arguments is: 
	// a0, q0, q1, q2, r0, r1, r2, s0, s1, s2 
	// corresponding index offsets for each of the parameters
	size_t ia = 0, iq = ia+1, ir = iq+3, is = ir+3;

	// make sure global variables have been computed by ExpApxSeq
	ok &= ExpApxSeq();

	// initial all partial derivatives as zero
	double df[10];
	size_t j;
	for(j = 0; j < 10; j++)
		df[j] = 0.;

	// f0
	df[is+2] = 1.;
	ok &= std::fabs( df[is+2] - 1. ) <= 1e-10;    // f0_s2

	// remove s2 = s1 + r2 from f0 to get f1
	df[is+1] += df[is+2];
	df[ir+2] += df[is+2];
	ok &= std::fabs( df[is+1] - 1. ) <= 1e-10;    // f1_s1
	ok &= std::fabs( df[ir+2] - 1. ) <= 1e-10;    // f1_r2

	// remove r2 = q2 / k1 from f1 to get f2
	df[iq+2] += df[ir+2] / k[1];
	ok &= std::fabs( df[iq+2] - 0.5 ) <= 1e-10;   // f2_q2

	// remove q2 = r1 * a0 from f2 to get f3
	df[ir+1] += df[iq+2] * a[0];
	df[ia+0] += r[1] * df[iq+2];
	ok &= std::fabs( df[ir+1] - 0.25 ) <= 1e-10;  // f3_r1
	ok &= std::fabs( df[ia+0] - 0.25 ) <= 1e-10;  // f3_a0

	// remove s1 = s0 + r1 from f3 to get f4
	df[ir+1] += df[is+1];
	ok &= std::fabs( df[ir+1] - 1.25 ) <= 1e-10;  // f4_r1

	// remove r1 = q1 / k0 from f4 to get f5
	df[iq+1] += df[ir+1] / k[0];
	ok &= std::fabs( df[iq+1] - 1.25 ) <= 1e-10;  // f5_q1

	// remove q1 = r0 * a0 from f5 to get f6
	df[ia+0] += r[0] * df[iq+1];
	ok &= std::fabs( df[ia+0] - 1.5 ) <= 1e-10;   // f6_a0

	return ok;
}
/* $$
$end
*/
