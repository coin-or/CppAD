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
	//
	size_t n = 3; // size of x
	size_t m = 1; // size of y
	size_t s = 2; // size of u and z
	//
	// record the function f(x)
	CPPAD_TESTVECTOR( AD<double> ) ax(n), ay(m);
	for(size_t j = 0; j < n; j++)
		ax[j] = double(j + 1);
	Independent( ax );
	// for this example, we ensure first absolute value is | x_0 + x_1 |
	AD<double> a0 = abs( ax[0] + ax[1] );
	// and second absolute value is | x_1 + x_2 |
	AD<double> a1 = abs( ax[1] + ax[2] );
	ay[0]         = a0 + a1;
	ADFun<double> f(ax, ay);

	// create its abs_normal representation in g
	ADFun<double> g;
	f.abs_normal(g);

	// check the dimension of the domain space for g
	ok &= g.Domain() == n + s;

	// check the dimension of the range space for g
	ok &= g.Range() == m + s;

	// zero order forward mode
	CPPAD_TESTVECTOR(double) xu(n+s), yz(m+s);
	for(size_t j = 0; j < n + s; j++)
		xu(j) = double(j + 2);
	yz = g.Forward(0, xu);

	// extract (x,u) from xu
	double x0 = xu[0];
	double x1 = xu[1];
	double x2 = xu[2];
	double u0 = xu[3];
	double u1 = xu[4];

	// check the compents of (y,z)
	double y0 = u0 + u1;
	ok       &= y0 == yz[0];
	double z0 = x0 + x1;
	ok       &= z0 == yz[1];
	double z1 = x1 + x2;
	ok       &= z1 == yz[2];

	return ok;
}
// END C++
