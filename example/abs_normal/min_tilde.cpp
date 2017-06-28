/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin min_tilde.cpp$$

$section abs_normal Approximation: Example and Test$$

$head Under Construction$$

$head Purpose$$
The function
$latex f : \B{R}^3 \rightarrow \B{R}$$ defined by
$latex \[
\begin{array}{rcl}
f( x_0, x_1  )
& = &
| d_0 - x_0 | + | d_1 - x_0 | + | d_2 - x_0 |
\\
& + &
| d_3 - x_1 | + | d_4 - x_1 | + | d_5 - x_1 |
\\
\end{array}
\] $$
is affine, except for its absolute value terms.
For this case, the abs_normal approximation should be equal
to the function itself.
The minimizer of this function is
$latex x_0 = \R{median}( d_0, d_1, d_2 )$$
and
$latex x_1 = \R{median}( d_3, d_4, d_5 )$$

$head Source$$
$srcfile%example/abs_normal/min_tilde.cpp%
	0%// BEGIN C++%// END C++%
1%$$

$end
-------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include "min_tilde.hpp"

namespace {
	CPPAD_TESTVECTOR(double) join(
		const CPPAD_TESTVECTOR(double)& x ,
		const CPPAD_TESTVECTOR(double)& u )
	{	size_t n = x.size();
		size_t s = u.size();
		CPPAD_TESTVECTOR(double) xu(n + s);
		for(size_t j = 0; j < n; j++)
			xu[j] = x[j];
		for(size_t j = 0; j < s; j++)
			xu[n + j] = u[j];
		return xu;
	}
}
bool min_tilde(void)
{	bool ok = true;
	//
	using CppAD::AD;
	using CppAD::ADFun;
	//
	typedef CPPAD_TESTVECTOR(size_t)       s_vector;
	typedef CPPAD_TESTVECTOR(double)       d_vector;
	typedef CPPAD_TESTVECTOR( AD<double> ) ad_vector;
	//
	size_t dpx   = 3;          // number of data points per x variable
	size_t level = 0;          // level of tracing
	size_t n     = 1;          // size of x
	size_t m     = 1;          // size of y
	size_t s     = dpx * n;    // number of data points and absolute values
	// data points
	d_vector  data(s);
	for(size_t i = 0; i < s; i++)
		data[i] = double(s - i) + 5.0 - (i % 2) / 2.0;
	//
	// record the function f(x)
	ad_vector ad_x(n), ad_y(m);
	for(size_t j = 0; j < n; j++)
		ad_x[j] = double(j + 1);
	Independent( ad_x );
	AD<double> sum = 0.0;
	for(size_t j = 0; j < n; j++)
		for(size_t k = 0; k < dpx; k++)
			sum += abs( data[j * dpx + k] - ad_x[j] );
	ad_y[0] = sum;
	ADFun<double> f(ad_x, ad_y);

	// create its abs_normal representation in g, a
	ADFun<double> g, a;
	f.abs_normal_fun(g, a);

	// check dimension of domain and range space for g
	ok &= g.Domain() == n + s;
	ok &= g.Range()  == m + s;

	// check dimension of domain and range space for a
	ok &= a.Domain() == n;
	ok &= a.Range()  == s;

	// --------------------------------------------------------------------
	// Choose a point x_hat
	d_vector x_hat(n);
	for(size_t j = 0; j < n; j++)
		x_hat[j] = double(0.0);

	// value of a_hat = a(x_hat)
	d_vector a_hat = a.Forward(0, x_hat);

	// (x_hat, a_hat)
	d_vector xu_hat = join(x_hat, a_hat);

	// value of g[ x_hat, a_hat ]
	d_vector g_hat = g.Forward(0, xu_hat);

	// Jacobian of g[ x_hat, a_hat ]
	d_vector g_jac = g.Jacobian(xu_hat);

	// trust region bound (make large enough to include solutuion)
	d_vector bound(n);
	for(size_t j = 0; j < n; j++)
		bound[j] = 10.0;

	// convergence criteria
	d_vector epsilon(2);
	double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
	epsilon[0]   = eps99;
	epsilon[1]   = eps99;

	// maximum number of iterations
	s_vector maxitr(2);
	maxitr[0] = 10; // maximum number of min_tilde iterations
	maxitr[1] = 25; // maximum number of qp_interior iterations

	// minimize the approxiamtion for f, which is equal to f because
	// f is affine, except for absolute value terms
	d_vector delta_x(n);
	ok &= CppAD::min_tilde(
		level, n, m, s, g_hat, g_jac, bound, epsilon, maxitr, delta_x
	);

	// number of data points per variable is odd
	ok &= dpx % 2 == 1;

	// check solution is median data value
	ok &= CppAD::NearEqual( delta_x[0], data[dpx / 2], eps99, eps99 );

	return ok;
}
// END C++
