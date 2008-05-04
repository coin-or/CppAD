/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin double_ode.cpp$$
$spell
	CppAD
	cppad
	hpp
	bool
	cstring
	retape
	resize
	endif
$$

$section Double Speed: Ode Solution$$

$index double, speed ode$$
$index speed, double ode$$
$index ode, speed double$$

$head link_ode$$
$index link_ode$$
Routine that computes the gradient of determinant using CppAD:
$codep */
# include <cstring>
# include <cppad/vector.hpp>
# include <cppad/speed/ode_evaluate.hpp>
# include <cppad/speed/uniform_01.hpp>

// value can be true of false
# define DOUBLE_COMPUTE_GRADIENT 0

bool link_ode(
	size_t                     size       ,
	size_t                     repeat     ,
	bool                       retape     ,
	CppAD::vector<double>      &x         ,
	CppAD::vector<double>      &gradient
)
{	// -------------------------------------------------------------
	// setup

	size_t n = size;
	assert( x.size() == n );

	size_t m = 0;
	CppAD::vector<double> f(1);
# if DOUBLE_COMPUTE_GRADIENT
	m = 1;
	f.resize(n);
# endif

	while(repeat--)
	{ 	// choose next x value
		uniform_01(n, x);

		// evaluate function
		CppAD::ode_evaluate(x, m, f);

	}
	gradient[0] = f[0];
# if DOUBLE_COMPUTE_GRADIENT
	gradient    = f;
# endif
	return true;
}
/* $$
$end
*/
