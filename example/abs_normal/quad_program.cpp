/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin quad_program.cpp$$

$section Example and Test of quad_program$$

$head Under Construction$$

$head Problem$$
Our original problem is
$latex \[
	\R{minimize} \; | u - 1| \; \R{w.r.t} \; u \in \B{R}
\] $$
with respect to $latex u \in \B{R}$$.
We reformulate this as the following problem
$latex \[
\begin{array}{rlr}
	\R{minimize}      & v             & \R{w.r.t} \; (u,v) \in \B{R}^2 \\
	\R{subject \; to} &  u - 1 \leq v \\
	                  &  1 - u \leq v
\end{array}
\] $$
This is equivalent to
$latex \[
\begin{array}{rlr}
	\R{minimize}
	& (0, 1) \cdot (u, v)^\R{T}  & \R{w.r.t} \; (u,v) \in \B{R}^2 \\
	\R{subject \; to}
	&
	\left( \begin{array}{cc} 1 & -1 \\ -1 & -1 \end{array} \right)
	\left( \begin{array}{c} u \\ v \end{array} \right)
	\leq
	\left( \begin{array}{c} 1 \\ -1 \end{array} \right)
\end{array}
\] $$
which is in the form expected by $cref quad_program$$.


$head Source$$
$srcfile%example/abs_normal/quad_program.cpp%
	0%// BEGIN C++%// END C++%
1%$$

$end
*/
// BEGIN C++
# include <limits>
# include <cppad/utility/vector.hpp>
# include "quad_program.hpp"

bool quad_program(void)
{	bool ok = true;
	typedef CppAD::vector<double> vector;
	//
	size_t n = 2;
	size_t m = 2;
	vector A(m*n), b(m), H(n*n), g(n), xout(n), yout(m), sout(m);
	A[ 0 * n + 0 ] =  1.0; // A(0,0)
	A[ 0 * n + 1 ] = -1.0; // A(0,1)
	A[ 1 * n + 0 ] = -1.0; // A(1,0)
	A[ 1 * n + 1 ] = -1.0; // A(1,1)
	//
	b[0]           =  1.0;
	b[1]           = -1.0;
	//
	g[0]           =  0.0;
	g[1]           =  1.0;
	//
	// H = 0
	for(size_t i = 0; i < n * n; i++)
		H[i] = 0.0;
	//
	double epsilon = 99.0 * std::numeric_limits<double>::epsilon();
	size_t maxitr  = 10;
	//
	ok &= CppAD::quad_program(A, b, H, g, epsilon, maxitr, xout, yout, sout);
	//
	// check optimal value for u
	ok &= std::fabs( xout[0] - 1.0 ) < epsilon;
	//
	std::cout << "xout = " << xout << "\n";
	std::cout << "yout = " << yout << "\n";
	std::cout << "sout = " << sout << "\n";
	//
	return ok;
}
// END C++
