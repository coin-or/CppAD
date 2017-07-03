/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin min_non_smo.cpp$$
$spell
	smo
$$

$section abs_normal min_non_smo: Example and Test$$

$head Purpose$$
We minimize the function
$latex f : \B{R}^2 \rightarrow \B{R}$$ defined by
$latex \[
\begin{array}{rcl}
f( x_0, x_1  ) & = & x_0^2 + | x_1 |
\end{array}
\] $$


$head Source$$
$srcfile%example/abs_normal/min_non_smo.cpp%
	0%// BEGIN C++%// END C++%
1%$$

$end
-------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include "min_non_smo.hpp"

bool min_non_smo(void)
{	bool ok = true;
	//
	using CppAD::AD;
	using CppAD::ADFun;
	//
	typedef CPPAD_TESTVECTOR(size_t)       s_vector;
	typedef CPPAD_TESTVECTOR(double)       d_vector;
	typedef CPPAD_TESTVECTOR( AD<double> ) ad_vector;
	//
	size_t level = 0;    // level of tracing
	size_t n     = 2;    // size of x
	size_t m     = 1;    // size of y
	size_t s     = 1;    // number of data points and absolute values
	//
	// start recording the function f(x)
	ad_vector ad_x(n), ad_y(m);
	for(size_t j = 0; j < n; j++)
		ad_x[j] = double(j + 1);
	Independent( ad_x );
	//
	ad_y[0]         =  ad_x[0] * ad_x[0] + abs( ad_x[1] );
	ADFun<double> f(ad_x, ad_y);
	//
	// create its abs_normal representation in g, a
	ADFun<double> g, a;
	f.abs_normal_fun(g, a);

	// check dimension of domain and range space for g
	ok &= g.Domain() == n + s;
	ok &= g.Range()  == m + s;

	// check dimension of domain and range space for a
	ok &= a.Domain() == n;
	ok &= a.Range()  == s;

	// epsilon
	d_vector epsilon(2);
	double eps6 = 1e6 * std::numeric_limits<double>::epsilon();
	epsilon[0] = eps6;
	epsilon[1] = eps6;

	// maxitr
	s_vector maxitr(3);
	maxitr[0] = 20;
	maxitr[1] = 20;
	maxitr[2] = 20;

	// call min_non_smo
	d_vector bound_in(n), x_in(n), x_out(n);
	x_in[0] = 1.0;
	x_in[1] = 1.0;
	bound_in = x_in;
	//
	ok &= CppAD::min_non_smo(
		level, g, a, epsilon, maxitr, bound_in, x_in, x_out
	);

	return ok;
}
// END C++
