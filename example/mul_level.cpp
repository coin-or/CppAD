/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin mul_level.cpp$$
$spell
	Adolc
	adouble
	CppAD
	abs
$$

$section Multiple Tapes: Example and Test$$
$index multiple, AD level$$
$index level, multiple AD$$
$index AD, multiple level$$

$head Purpose$$
This is an example and test of using the $code AD<double>$$ type,
together with the $code AD< AD<double> >$$ type,
for multiple levels of taping.
The example computes
the value
$latex \[
	\frac{d}{dx} \left[ f^{(1)} (x) * v \right]
\] $$
where $latex f : \R^n \rightarrow \R$$ and
$latex v \in \R^n$$.
The example $xref/HesTimesDir.cpp/$$ computes the same value using only
one level of taping (more efficient) and the identity
$latex \[
	\frac{d}{dx} \left[ f^{(1)} (x) * v \right] = f^{(2)} (x) * v
\] $$
The example $cref/mul_level_adolc.cpp/$$ computes the same values using
Adolc's type $code adouble$$ and CppAD's type $code AD<adouble>$$.

$code
$verbatim%example/mul_level.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>

namespace { // put this function in the empty namespace
	// f(x) = |x|^2 = .5 * ( x[0]^2 + ... + x[n-1]^2 + .5 )
	template <class Type>
	Type f(CPPAD_TEST_VECTOR<Type> &x)
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
}

bool mul_level(void) 
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
	aa_f[0] = f(aa_x);                 // has only one component

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
// END PROGRAM
