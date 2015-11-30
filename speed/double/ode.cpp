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
$begin double_ode.cpp$$
$spell
	onetape
	Jacobian
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
$mindex link_ode speed$$


$head Specifications$$
See $cref link_ode$$.

$head Implementation$$

$codep */
# include <cstring>
# include <cppad/utility/vector.hpp>
# include <cppad/speed/ode_evaluate.hpp>
# include <cppad/speed/uniform_01.hpp>

// Note that CppAD uses global_memory at the main program level
extern bool
	global_onetape, global_atomic, global_optimize;

bool link_ode(
	size_t                     size       ,
	size_t                     repeat     ,
	CppAD::vector<double>      &x         ,
	CppAD::vector<double>      &jacobian
)
{
	if(global_onetape||global_atomic||global_optimize)
		return false;
	// -------------------------------------------------------------
	// setup
	assert( x.size() == size );

	size_t n = size;

	size_t m = 0;
	CppAD::vector<double> f(n);

	while(repeat--)
	{	// choose next x value
		uniform_01(n, x);

		// evaluate function
		CppAD::ode_evaluate(x, m, f);

	}
	size_t i;
	for(i = 0; i < n; i++)
		jacobian[i] = f[i];
	return true;
}
/* $$
$end
*/
