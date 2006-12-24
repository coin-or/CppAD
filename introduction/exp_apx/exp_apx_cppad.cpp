/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin exp_apx_cppad.cpp$$
$spell
	cppad.hpp
	cmath
	fabs
	bool
	exp_apx_cppad
	du
	dv
	dw
	endl
	hpp
	http
	org
	std
	www
	CppAD
$$

$section exp_apx: CppAD First Order Forward and Reverse$$.

$head Purpose$$
Use CppAD forward and reverse modes to compute the
partial derivative with respect to $latex x$$,
at the point $latex x = .5$$ and $latex e = .2$$,
of the function 
$syntax%
	exp_apx(%x%, %e%)
%$$
as defined by the $cref/exp_apx.hpp/$$ include file.

$head Exercises$$
$list number$$
Create and tests a modified version of the routine below that computes
partial derivative with respect to $latex x$$,
at the point $latex x = .1$$ and $latex e = .2$$,
of the function 
$syntax%
	exp_apx(%x%, %e%)
%$$
$lnext
Create and test a modified version of the routine below that computes
partial derivative with respect to $latex x$$,
at the point $latex x = .1$$ and $latex e = .2$$,
of the function corresponding to the operation sequence 
for $latex x = .5$$ and $latex e = .2$$.
Hint: you could define a vector u with two components and use
$syntax%
	%f%.Forward(0, %u%)
%$$
to run zero order forward mode at a point different
form the point where the operation sequence corresponding to
$italic f$$ was recorded.
$lend
$codep */

# include <cppad/cppad.hpp>  // http://www.coin-or.org/CppAD/ 
# include "exp_apx.hpp"      // our example exponential function approximation
bool exp_apx_cppad(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::vector;    // can use any simple vector template class
	using CppAD::NearEqual; // checks if values are nearly equal

	// domain space vector
	size_t n = 2; // dimension of the domain space
	vector< AD<double> > U(n);
	U[0] = .5;    // value of x for this operation sequence
	U[1] = .2;    // value of e for this operation sequence

	// declare independent variables and start recording operation sequence
	CppAD::Independent(U);

	// evaluate our exponential approximation
	AD<double> x   = U[0];
	AD<double> e   = U[1];
	AD<double> apx = exp_apx(x, e);  

	// range space vector
	size_t m = 1;  // dimension of the range space
	vector< AD<double> > V(m);
	V[0] = apx;    // variable that represents only range space component

	// Create f: U -> V corresponding to this operation sequence
	// and stop recording. This also executes a zero order forward 
	// mode sweep using values in U for x and e.
	CppAD::ADFun<double> f(U, V);

	// first order forward mode sweep that computes partial w.r.t x
	vector<double> du(n);  // differential in domain space
	vector<double> dv(m);  // differential in range space
	du[0] = 1.;  // x direction in domain space
	du[1] = 0.;
	// partial of exp_apx(x, e) with respect to x
	dv    = f.Forward(1, du);
	double check = 1.5;
	ok   &= NearEqual(dv[0], check, 1e-10, 1e-10);

	// first order reverse mode sweep that computes the derivative
	vector<double>  w(m);   // weights for components of the range
	vector<double> dw(n);   // derivative of the weighted function
	w[0] = 1.;   // only one weight and it is one
	// derivative of w[0] * exp_apx(x, e)
	dw   = f.Reverse(1, w);
	check = 1.5;  // partial of exp_apx(x, e) with respect to x
	ok   &= NearEqual(dw[0], check, 1e-10, 1e-10);
	check = 0.;   // partial of exp_apx(x, e) with respect to e
	ok   &= NearEqual(dw[1], check, 1e-10, 1e-10);

	return ok;
}
/* $$
$end
*/
