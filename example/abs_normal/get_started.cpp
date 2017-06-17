/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin abs_normal_get_started.cpp$$

$section Getting Started with Atomic Operations: Example and Test$$

$head Under Construction$$

$head Purpose$$
Creates an $cref abs_normal$$ representation $latex g$$ for the function
$latex f : \B{R}^3 \rightarrow \B{R}$$ defined by
$latex \[
	f( x_0, x_1, x_2  ) = | x_0 + x_1 | + | x_1 + x_2 |
\] $$
The corresponding
$cref/g/abs_normal/g/$$ $latex : \B{R}^5 \rightarrow \B{R}^3$$ is
given by
$latex \[
\begin{array}{rcl}
	g_0 ( x_0, x_1, u_0, u_1 ) & = & u_0 + u_1
	\\
	g_1 ( x_0, x_1, u_0, u_1 ) & = & x_0 + x_1
	\\
	g_1 ( x_0, x_1, u_0, u_1 ) & = & x_1 + x_2
\end{array}
\] $$
$srcfile%example/abs_normal/get_started.cpp%0%// BEGIN C++%// END C++%1%$$

$end
-------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/cppad.hpp>

bool get_started(void)
{	bool ok = true;
	//
	using CppAD::AD;
	using CppAD::ADFun;

	// define the function f(x)
	size_t n = 3, m = 1;
	CPPAD_TESTVECTOR( AD<double> ) ax(n), ay(m);
	for(size_t j = 0; j < n; j++)
		ax[j] = double(j);
	Independent( ax );
	ay[0] = abs( ax[0] + ax[1] ) + abs( ax[1] + ax[2] );
	ADFun<double> f(ax, ay);

	// create its abs_normal representation in g
	ADFun<double> g;
	g.abs_normal(f);

	// nuber of absolute values terms in f
	size_t s = 2;

	// check the dimension of the domain space for g
	ok &= g.Domain() == n + s;

	// check the dimension of the range space for g
	ok &= g.Range() == m + s;


	return ok;
}
// END C++
