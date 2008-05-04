/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin sacado_ode.cpp$$
$spell
	Sacado
	cstring
	cppad
	hpp
	bool
	retape
	CppAD
	typedef
	ADvar
	Gradcomp
	endif
	cassert
$$

$section Sacado Speed: Gradient of Ode Solution$$

$index sacado, speed ode gradient$$
$index speed, sacado ode gradient$$
$index gradient, ode speed sacado$$
$index ode, gradient speed sacado$$

$head link_ode$$
$index link_ode$$
$codep */

# include <cstring>
# include <cppad/vector.hpp>

# define ODE_TEST_AVAILABLE 0

# if ! ODE_TEST_AVAILABLE
bool link_ode(
	size_t                     size       ,
	size_t                     repeat     ,
	bool                       retape     ,
	CppAD::vector<double>      &x         ,
	CppAD::vector<double>      &gradient
)
{
	return false;
}
# else

// There appears to be a problem with the way Sacado is used below
// because the following generates a segmentation fault.  

# include <Sacado.hpp>
# include <cppad/speed/ode_evaluate.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/vector.hpp>
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

	// object for computing determinant
	typedef Sacado::Rad::ADvar<double>   ADScalar; 
	typedef CppAD::vector<ADScalar>      ADVector; 

	size_t j;
	size_t m = 0;
	size_t n = size;
	assert( x.size() == n );
	ADVector  X(n);
	ADVector  Y(1);
	ADScalar  last;
	
	// ------------------------------------------------------
	while(repeat--)
	{	// choose next x value
		CppAD::uniform_01(n, x);

		// set independent variable values
		for(j = 0; j < n; j++)
			X[j] = x[j];

		// evaluate function
		CppAD::ode_evaluate(X, m, Y);

		// make sure function value is last assignment
		last = Y[0];

		// compute the gradient using reverse mode
		ADScalar::Gradcomp();

		// evaluate return gradient 
		for(j =0; j < n; j++)
			gradient[j] = X[j].adj();
	}
	// ---------------------------------------------------------
	return true;
}
# endif

/* $$
$end
*/
