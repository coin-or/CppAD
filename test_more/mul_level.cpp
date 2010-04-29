/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */


# ifdef CPPAD_ADOLC_TEST
# include <adolc/adouble.h>
# include <adolc/taping.h>
# include <adolc/interfaces.h>
// adouble definitions not in Adolc distribution and 
// required in order to use CppAD::AD<adouble>
# include "../example/base_adolc.hpp"
# endif

# include <cppad/cppad.hpp>
# include <limits>

namespace { // BEGIN empty namespace

bool One(void) 
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

// f(x) = |x|^2 = .5 * ( x[0]^2 + ... + x[n-1]^2 )
template <class Type>
Type f_Two(CPPAD_TEST_VECTOR<Type> &x)
{	Type sum;

	// check assignment of AD< AD<double> > = double
	sum  = .5;
	sum += .5;

	size_t i = x.size();
	while(i--)
		sum += x[i] * x[i];

	// check computed assignment AD< AD<double> > -= int
	sum -= 1; 

	// check double * AD< AD<double> > 
	return .5 * sum;
} 

bool Two(void) 
{	bool ok = true;                          // initialize test result

	typedef CppAD::AD<double>   ADdouble;    // for one level of taping
	typedef CppAD::AD<ADdouble> ADDdouble;   // for two levels of taping
	size_t n = 5;                            // dimension for example
	size_t j;                                // a temporary index variable

	CPPAD_TEST_VECTOR<double>       x(n);
	CPPAD_TEST_VECTOR<ADdouble>   a_x(n);
	CPPAD_TEST_VECTOR<ADDdouble> aa_x(n);

	// value of the independent variables
	for(j = 0; j < n; j++)
		a_x[j] = x[j] = double(j); // x[j] = j
	Independent(a_x);                  // a_x is indedendent for ADdouble
	for(j = 0; j < n; j++)
		aa_x[j] = a_x[j];          // track how aa_x depends on a_x
	CppAD::Independent(aa_x);          // aa_x is independent for ADDdouble

	// compute function
	CPPAD_TEST_VECTOR<ADDdouble> aa_f(1);    // scalar valued function
	aa_f[0] = f_Two(aa_x);                   // has only one component

	// declare inner function (corresponding to ADDdouble calculation)
	CppAD::ADFun<ADdouble> a_F(aa_x, aa_f);

	// compute f'(x) 
	size_t p = 1;                        // order of derivative of a_F
	CPPAD_TEST_VECTOR<ADdouble> a_w(1);  // weight vector for a_F
	CPPAD_TEST_VECTOR<ADdouble> a_df(n); // value of derivative
	a_w[0] = 1;                          // weighted function same as a_F
	a_df   = a_F.Reverse(p, a_w);        // gradient of f

	// declare outter function (corresponding to ADdouble calculation)
	CppAD::ADFun<double> df(a_x, a_df);

	// compute the d/dx of f'(x) * v = f''(x) * v
	CPPAD_TEST_VECTOR<double> v(n);
	CPPAD_TEST_VECTOR<double> ddf_v(n);
	for(j = 0; j < n; j++)
		v[j] = double(n - j);
	ddf_v = df.Reverse(p, v);

	// f(x)       = .5 * ( x[0]^2 + x[1]^2 + ... + x[n-1]^2 )
	// f'(x)      = (x[0], x[1], ... , x[n-1])
	// f''(x) * v = ( v[0], v[1],  ... , x[n-1] )
	for(j = 0; j < n; j++)
		ok &= CppAD::NearEqual(ddf_v[j], v[j], 1e-10, 1e-10);

	return ok;
}

# ifdef CPPAD_ADOLC_TEST

bool Three(void) 
{	bool ok = true;                   // initialize test result

	typedef adouble      ADdouble;         // for first level of taping
	typedef CppAD::AD<ADdouble> ADDdouble; // for second level of taping
	size_t n = 5;                          // number independent variables

	CPPAD_TEST_VECTOR<double>       x(n);
	CPPAD_TEST_VECTOR<ADdouble>   a_x(n);
	CPPAD_TEST_VECTOR<ADDdouble> aa_x(n);

	// value of the independent variables
	int tag = 0;                         // Adolc setup
	int keep = 1;
	trace_on(tag, keep);
	size_t j;
	for(j = 0; j < n; j++)
	{	x[j] = double(j);           // x[j] = j
		a_x[j] <<= x[j];            // a_x is independent for ADdouble
	}
	for(j = 0; j < n; j++)
		aa_x[j] = a_x[j];          // track how aa_x depends on a_x
	CppAD::Independent(aa_x);          // aa_x is independent for ADDdouble

	// compute function
	CPPAD_TEST_VECTOR<ADDdouble> aa_f(1);    // scalar valued function
	aa_f[0] = f_Two(aa_x);                   // has only one component

	// declare inner function (corresponding to ADDdouble calculation)
	CppAD::ADFun<ADdouble> a_F(aa_x, aa_f);

	// compute f'(x) 
	size_t p = 1;                        // order of derivative of a_F
	CPPAD_TEST_VECTOR<ADdouble> a_w(1);  // weight vector for a_F
	CPPAD_TEST_VECTOR<ADdouble> a_df(n); // value of derivative
	a_w[0] = 1;                          // weighted function same as a_F
	a_df   = a_F.Reverse(p, a_w);        // gradient of f

	// declare outter function 
	// (corresponding to the tape of adouble operations)
	double df_j;
	for(j = 0; j < n; j++)
		a_df[j] >>= df_j;
	trace_off();

	// compute the d/dx of f'(x) * v = f''(x) * v
	size_t m      = n;                     // # dependent in f'(x)
	double *v = 0, *ddf_v = 0;
	v     = CPPAD_TRACK_NEW_VEC(m, v);     // track v = new double[m]
	ddf_v = CPPAD_TRACK_NEW_VEC(n, ddf_v); // track ddf_v = new double[n]
	for(j = 0; j < n; j++)
		v[j] = double(n - j);
	fos_reverse(tag, int(m), int(n), v, ddf_v);

	// f(x)       = .5 * ( x[0]^2 + x[1]^2 + ... + x[n-1]^2 )
	// f'(x)      = (x[0], x[1], ... , x[n-1])
	// f''(x) * v = ( v[0], v[1],  ... , x[n-1] )
	for(j = 0; j < n; j++)
		ok &= CppAD::NearEqual(ddf_v[j], v[j], 1e-10, 1e-10);

	CPPAD_TRACK_DEL_VEC(v);                 // check usage of delete
	CPPAD_TRACK_DEL_VEC(ddf_v);
	return ok;
}

# endif // CPPAD_ADOLC_TEST

} // END empty namespace

bool mul_level(void)
{	bool ok = true;
	ok     &= One();
	ok     &= Two();
# ifdef CPPAD_ADOLC_TEST
	ok     &= Three();
# endif
	return ok;
}
