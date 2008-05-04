/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin cppad_ode.cpp$$
$spell
	CppAD
	cppad
	hpp
	bool
	cstring
	retape
	typedef
	gradient gradient
	cassert
$$

$section CppAD Speed: Gradient of Ode Solution$$

$index cppad, speed ode gradient$$
$index speed, cppad ode gradient$$
$index gradient, ode speed cppad$$
$index ode, gradient speed cppad$$

$head link_ode$$
$index link_ode$$
Routine that computes the gradient of determinant using CppAD:
$codep */
# include <cstring>
# include <cppad/cppad.hpp>
# include <cppad/speed/ode_evaluate.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cassert>

bool link_ode(
	size_t                     size       ,
	size_t                     repeat     ,
	bool                       retape     ,
	CppAD::vector<double>      &x         ,
	CppAD::vector<double>      &gradient
)
{	// -------------------------------------------------------------
	// setup
	typedef CppAD::AD<double>       ADScalar;
	typedef CppAD::vector<ADScalar> ADVector;
	typedef CppAD::vector<double>   DblVector;

	size_t j;
	size_t m = 0;
	size_t n = size;
	assert( x.size() == n );

	ADVector  X(n);
	ADVector  Y(1);
	DblVector w(1);
	w[0] = 1.;

	if( retape ) while(repeat--)
	{ 	// choose next x value
		uniform_01(n, x);
		for(j = 0; j < n; j++)
			X[j] = x[j];

		// declare the independent variable vector
		Independent(X);

		// evaluate function
		CppAD::ode_evaluate(X, m, Y);

		// create function object f : X -> Y
		CppAD::ADFun<double>   F(X, Y);

		// use reverse mode to compute gradient
		gradient = F.Reverse(1, w);
	}
	else
	{	// choose any x value
		for(j = 0; j < n; j++)
			X[j] = 0.;

		// declare the independent variable vector
		Independent(X);

		// evaluate function
		CppAD::ode_evaluate(X, m, Y);

		// create function object f : X -> Y
		CppAD::ADFun<double>   F(X, Y);

		while(repeat--)
		{ 	// choose next x value
			uniform_01(n, x);
			// zero order forward mode to evaluate function at x
			F.Forward(0, x);
			// first order reverse mode to compute gradient
			gradient = F.Reverse(1, w);
		}
	}

	return true;
}
/* $$
$end
*/
