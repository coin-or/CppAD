/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

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
the compute values of the function $latex f : \B{R}^n \rightarrow \B{R}$$ where 
$latex \[
	f(x) = \frac{1}{2} \left( x_0^2 + \cdots + x_{n-1}^2 \right)
\] $$
We then use $code AD<double>$$ (level one taping) to compute
the directional derivative
$latex \[
f^{(1)} (x) * v  = x_0 v_0 + \cdots + x_{n-1} v_{n-1}
\] $$.
where $latex v \in \B{R}^n$$.
We then use $code double$$ (no taping) to compute
$latex \[
\frac{d}{dx} \left[ f^{(1)} (x) * v \right] = v
\] $$
This is only meant as an example of multiple levels of taping.
The example $cref hes_times_dir.cpp$$ computes the same value more
efficiently by using the identity:
$latex \[
	\frac{d}{dx} \left[ f^{(1)} (x) * v \right] = f^{(2)} (x) * v
\] $$
The example $cref mul_level_adolc.cpp$$ computes the same values using
Adolc's type $code adouble$$ and CppAD's type $code AD<adouble>$$.

$code
$verbatim%example/mul_level.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++

# include <cppad/cppad.hpp>

namespace { 
	// f(x) = |x|^2 / 2 = .5 * ( x[0]^2 + ... + x[n-1]^2 )
	template <class Type>
	Type f(CPPAD_TESTVECTOR(Type) &x)
	{	Type sum;

		sum  = 0.;
		size_t i = size_t(x.size());
		for(i = 0; i < size_t(x.size()); i++)
			sum += x[i] * x[i];

		return .5 * sum;
	} 
}

bool mul_level(void) 
{	bool ok = true;                          // initialize test result

	typedef CppAD::AD<double>    A1_double;  // for one level of taping
	typedef CppAD::AD<A1_double> A2_double;  // for two levels of taping
	size_t n = 5;                            // dimension for example
	size_t j;                                // a temporary index variable

	CPPAD_TESTVECTOR(double)       x(n);
	CPPAD_TESTVECTOR(A1_double)  a1_x(n), a1_v(n), a1_dy(1) ;
	CPPAD_TESTVECTOR(A2_double)  a2_x(n), a2_y(1);

	// Values for the independent variables while taping the function f(x)
	for(j = 0; j < n; j++)
		a2_x[j] = a1_x[j] = x[j] = double(j);
	// Declare the independent variable for taping f(x)
	CppAD::Independent(a2_x); 

	// Use AD< AD<double> > to tape the evaluation of f(x)
	a2_y[0] = f(a2_x); 

	// Declare a1_F as the corresponding ADFun< AD<double> > value a2_y
	// (make sure we do not run zero order forward during constructor)
	CppAD::ADFun<A1_double> a1_F;
	a1_F.Dependent(a2_x, a2_y);

	// Values for the independent variables while taping f'(x) * v
	// Declare the independent variable for taping f'(x) * v
	CppAD::Independent(a1_x); 
	// set the argument value x for computing f'(x) * v
	a1_F.Forward(0, a1_x);
	// compute f'(x) * v
	for(j = 0; j < n; j++)
		a1_v[j] = double(n - j);
	a1_dy = a1_F.Forward(1, a1_v); 

	// declare dF as ADFun<double> function corresponding to f'(x) * v
	CppAD::ADFun<double> dF;
	dF.Dependent(a1_x, a1_dy);

	// optimize out operations not necessary for function f'(x) * v
	dF.optimize();

	// Evaluate f'(x) * v
	dF.Forward(0, x);

	// compute the d/dx of f'(x) * v = f''(x) * v = v
	CPPAD_TESTVECTOR(double) w(1);
	CPPAD_TESTVECTOR(double) dw(n);
	w[0] = 1.;
	dw   = dF.Reverse(1, w);

	for(j = 0; j < n; j++)
		ok &= CppAD::NearEqual(dw[j], a1_v[j], 1e-10, 1e-10);

	return ok;
}
// END C++
