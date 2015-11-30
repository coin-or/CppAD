// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

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
	onetape
	CppAD
	typedef
	endif
	cassert
$$

$section Sacado Speed: Gradient of Ode Solution$$
$mindex link_ode speed$$


$head Specifications$$
See $cref link_ode$$.

$head Implementation$$

$codep */
# include <Sacado.hpp>
// # include <algorithm>
# include <cassert>
# include <cppad/utility/vector.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/speed/ode_evaluate.hpp>

// list of possible options
extern bool global_memory, global_onetape, global_atomic, global_optimize;

bool link_ode(
	size_t                     size       ,
	size_t                     repeat     ,
	CppAD::vector<double>      &x         ,
	CppAD::vector<double>      &jacobian
)
{
	// speed test global option values
	if( global_atomic )
		return false;
	if( global_memory || global_onetape || global_optimize )
		return false;
	// -------------------------------------------------------------
	// setup
	assert( x.size() == size );
	assert( jacobian.size() == size * size );

	typedef Sacado::Fad::DFad<double>  ADScalar;
	typedef CppAD::vector<ADScalar>    ADVector;

	size_t i, j;
	size_t p = 0;          // use ode to calculate function values
	size_t n = size;       // number of independent variables
	size_t m = n;          // number of dependent variables
	ADVector X(n), Y(m);   // independent and dependent variables

	// -------------------------------------------------------------
	while(repeat--)
	{	// choose next x value
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
