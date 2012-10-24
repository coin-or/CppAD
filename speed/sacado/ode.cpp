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
$begin sacado_ode.cpp$$
$spell
	jacobian
	Sacado
	cppad
	hpp
	bool
	retape
	CppAD
	typedef
	endif
	cassert
$$

$section Sacado Speed: Gradient of Ode Solution$$

$index link_ode, sacado$$
$index sacado, link_ode$$
$index speed, sacado$$
$index sacado, speed$$
$index ode, speed sacado$$

$head Specifications$$
See $cref link_ode$$.

$head Implementation$$

$codep */
# include <Sacado.hpp>
// # include <algorithm>
# include <cassert>
# include <cppad/vector.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/speed/ode_evaluate.hpp>

bool link_ode(
	size_t                     size       ,
	size_t                     repeat     ,
	CppAD::vector<double>      &x         ,
	CppAD::vector<double>      &jacobian
)
{
	assert( x.size() == size );
	assert( jacobian.size() == size * size );

	// speed test global option values
	extern bool global_retape, global_atomic, global_optimize;
	if( ! global_retape || global_atomic || global_optimize )
		return false;

	// -------------------------------------------------------------
	// setup
	typedef Sacado::Fad::DFad<double>  ADScalar;
	typedef CppAD::vector<ADScalar>    ADVector;
	typedef CppAD::vector<double>     DblVector;

	size_t i, j;
	size_t p = 0;          // use ode to calculate function values
	size_t n = size;       // number of independent variables
	size_t m = n;          // number of dependent variables
	ADVector X(n), Y(m);   // independent and dependent variables

	// -------------------------------------------------------------
	while(repeat--)
	{ 	// choose next x value
		CppAD::uniform_01(n, x);
		for(j = 0; j < n; j++)
		{	// set up for X as the independent variable vector
			X[j] = ADScalar(int(n), int(j), x[j]);
		}

		// evaluate function
		CppAD::ode_evaluate(X, p, Y);

		// return values with Y as the dependent variable vector
		for(i = 0; i < m; i++)
		{	for(j = 0; j < n; j++)
				jacobian[ i * n + j ] = Y[i].dx(j);
		}
	}
	return true;
}
/* $$
$end
*/
