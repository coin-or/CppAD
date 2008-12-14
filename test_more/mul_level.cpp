/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/cppad.hpp>

# include <limits>

bool mul_level(void) 
{	bool ok = true;                          // initialize test result
	using CppAD::NearEqual;
	double eps = 10. * std::numeric_limits<double>::epsilon();


	typedef CppAD::AD<double>   ADdouble;    // for one level of taping
	typedef CppAD::AD<ADdouble> ADDdouble;   // for two levels of taping
	size_t n = 2;                            // dimension for example

	CPPAD_TEST_VECTOR<ADdouble>   a_x(n);
	CPPAD_TEST_VECTOR<ADDdouble> aa_x(n);

	// value of the independent variables
	a_x[0] = 2.; a_x[1] = 3.;
	Independent(a_x);
	aa_x[0] = a_x[0]; aa_x[1] = a_x[1];
	CppAD::Independent(aa_x);

	// compute the function f(x) = 2 * x[0] * x[1]
	CPPAD_TEST_VECTOR<ADDdouble> aa_f(1);
	aa_f[0] = 2. * aa_x[0] * aa_x[1];
	CppAD::ADFun<ADdouble> F(aa_x, aa_f);

	// re-evaluate f(2, 3) (must get proper deepedence on a_x).
	size_t p = 0;
	CPPAD_TEST_VECTOR<ADdouble> a_fp(1);
	a_fp    = F.Forward(p, a_x);
	ok     &= NearEqual(a_fp[0], 2. * a_x[0] * a_x[1], eps, eps);

	// compute the function g(x) = 2 * partial_x[0] f(x) = 4 * x[1]
	p = 1;
	CPPAD_TEST_VECTOR<ADdouble> a_dx(n), a_g(1);
	a_dx[0] = 1.; a_dx[1] = 0.;
	a_fp    = F.Forward(p, a_dx);
	a_g[0]  = 2. * a_fp[0];
	CppAD::ADFun<double> G(a_x, a_g);

	// compute partial_x[1] g(x)
	CPPAD_TEST_VECTOR<double>  xp(n), gp(1);
	p = 0;
	xp[0] = 4.; xp[1] = 5.;
	gp    = G.Forward(p, xp);
	ok   &= NearEqual(gp[0], 4. * xp[1], eps, eps);

	p = 1;
	xp[0] = 0.; xp[1] = 1.;
	gp    = G.Forward(p, xp);
	ok   &= NearEqual(gp[0], 4., eps, eps);

	return ok;
}
// END PROGRAM
