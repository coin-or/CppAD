/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <adolc/adouble.h>
# include <adolc/taping.h>
# include <adolc/interfaces.h>

// adouble definitions not in Adolc distribution and 
// required in order to use CppAD::AD<adouble>
# include "../example/base_adolc.hpp"

# include <cppad/cppad.hpp>

bool base_adolc(void) 
{	bool ok = true;                   // initialize test result

	typedef adouble             ADdouble;  // for first level of taping
	typedef CppAD::AD<ADdouble> ADDdouble; // for second level of taping
	size_t n = 4;                          // number independent variables

	CPPAD_TEST_VECTOR<ADdouble>   a_x(n);
	CPPAD_TEST_VECTOR<ADDdouble> aa_x(n);

	// value of the independent variables
	int tag = 0;                         // Adolc setup
	int keep = 1;
	trace_on(tag, keep);
	size_t j;
	for(j = 0; j < n; j++)
		a_x[j] <<= double(n - j);  // a_x is independent for ADdouble
	for(j = 0; j < n; j++)
		aa_x[j] = a_x[j];          // track how aa_x depends on a_x
	CppAD::Independent(aa_x);          // aa_x is independent for ADDdouble
	 

	// compute function
	size_t m = 5;
	CPPAD_TEST_VECTOR<ADDdouble> aa_f(m);    // dependent variables

	// different cases of conditional expressions
	aa_f[0] = CondExpLt(aa_x[0], aa_x[1], aa_x[2], aa_x[3]);
	aa_f[1] = CondExpLe(aa_x[0], aa_x[1], aa_x[2], aa_x[3]);
	aa_f[2] = CondExpEq(aa_x[0], aa_x[1], aa_x[2], aa_x[3]);
	aa_f[3] = CondExpGe(aa_x[0], aa_x[1], aa_x[2], aa_x[3]);
	aa_f[4] = CondExpGt(aa_x[0], aa_x[1], aa_x[2], aa_x[3]);

	// declare inner function (just to stop inner taping)
	CppAD::ADFun<ADdouble> a_F(aa_x, aa_f);

	// set values for outer function same as inner function
	// (corresponding to the tape of adobule operations)
	double f_j;
	for(j = 0; j < m; j++)
		Value(aa_f[j]) >>= f_j;
	trace_off();

	// arrays for Adolc zos_forward
	double *x, *y;
	x = CPPAD_TRACK_NEW_VEC(n, x);
	y = CPPAD_TRACK_NEW_VEC(m, y);

	// switch order of arguments from when taping was done
	for(j = 0; j < n; j++)
		x[j] = double(j);
	zos_forward(tag, int(m), int(n), keep, x, y);
	// CondExpLt(0, 1, 2, 3)
	ok &= (y[0] == double(2));
	// CondExpLe(0, 1, 2, 3)
	ok &= (y[1] == double(2));
	// CondExpEq(0, 1, 2, 3)
	ok &= (y[2] == double(3));
	// CondExpGe(0, 1, 2, 3)
	ok &= (y[3] == double(3));
	// CondExpGt(0, 1, 2, 3)
	ok &= (y[4] == double(3));

	// set left = right and true < false
	x[0] = x[1] = 1.;
	x[2] = 2.;
	x[3] = 3.;
	zos_forward(tag, int(m), int(n), keep, x, y);
	// CondExpLt(1, 1, 2, 3)
	ok &= (y[0] == double(3));
	// CondExpLe(1, 1, 2, 3)
	ok &= (y[1] == double(2));
	// CondExpEq(1, 1, 2, 3)
	ok &= (y[2] == double(2));
	// CondExpGe(1, 1, 2, 3)
	ok &= (y[3] == double(2));
	// CondExpGt(1, 1, 2, 3)
	ok &= (y[4] == double(3));

	CPPAD_TRACK_DEL_VEC(x);
	CPPAD_TRACK_DEL_VEC(y);
	return ok;
}
// END PROGRAM
