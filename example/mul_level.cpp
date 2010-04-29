/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell

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
In this example, we use $code AD< AD<double> >$$ (level two taping),
the compute values of the function $latex f : \R^n \rightarrow \R$$ where 
$latex \[
	f(x) = \frac{1}{2} \left( x_0^2 + \cdots + x_{n-1}^2 \right)
\] $$
We then use $code AD<double>$$ (level one taping) to compute
the directional derivative
$latex \[
f^{(1)} (x) * v  = x_0 v_0 + \cdots + x_{n-1} v_{n-1}
\] $$.
where $latex v \in \R^n$$.
We then use $code double$$ (no taping) to compute
$latex \[
\frac{d}{dx} \left[ f^{(1)} (x) * v \right] = v
\] $$
This is only meant as an example of multiple levels of taping.
The example $xref/HesTimesDir.cpp/$$ computes the same value more
efficiently by using the identity:
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

namespace { 
	// f(x) = |x|^2 / 2 = .5 * ( x[0]^2 + ... + x[n-1]^2 )
	template <class Type>
	Type f(CPPAD_TEST_VECTOR<Type> &x)
	{	Type sum;

		sum  = 0.;
		size_t i = x.size();
		for(i = 0; i < x.size(); i++)
			sum += x[i] * x[i];

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

	// Values for the independent variables while taping the function f(x)
	for(j = 0; j < n; j++)
		aa_x[j] = double(j);
	// Declare the independent variable for taping f(x)
	CppAD::Independent(aa_x); 

	// Use AD< AD<double> > to tape the evaluation of f(x)
	CPPAD_TEST_VECTOR<ADDdouble> aa_f(1); 
	aa_f[0] = f(aa_x); 

	// Declare a_F as the corresponding ADFun< AD<double> > function f(x)
	// (make sure we do not run zero order forward during constructor)
	CppAD::ADFun<ADdouble> a_F;
	a_F.Dependent(aa_x, aa_f);

	// Values for the independent variables while taping f'(x) * v
	for(j = 0; j < n; j++)
		a_x[j] = double(j);
	// Declare the independent variable for taping f'(x) * v
	CppAD::Independent(a_x); 
	// set the argument value x for computing f'(x) * v
	a_F.Forward(0, a_x);
	// compute f'(x) * v
	CPPAD_TEST_VECTOR<ADdouble> a_v(n);
	CPPAD_TEST_VECTOR<ADdouble> a_df(1);
	for(j = 0; j < n; j++)
		a_v[j] = double(n - j);
	a_df = a_F.Forward(1, a_v); 

	// declare dF as ADFun<double> function corresponding to f'(x) * v
	CppAD::ADFun<double> df(a_x, a_df);

	// compute the d/dx of f'(x) * v = f''(x) * v
	CPPAD_TEST_VECTOR<double> w(1);
	CPPAD_TEST_VECTOR<double> dw(n);
	w[0] = 1.;
	dw   = df.Reverse(1, w);

	for(j = 0; j < n; j++)
		ok &= CppAD::NearEqual(dw[j], a_v[j], 1e-10, 1e-10);

	return ok;
}
// END PROGRAM
