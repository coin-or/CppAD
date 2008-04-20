/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin double_sparse_hessian.cpp$$
$spell
	bool
	cppad
	hpp
	CppAD
	cmath
	exp
	tmp
	std
$$

$section Double Speed: Sparse Hessian$$

$index double, speed sparse hessian$$
$index speed, double sparse hessian$$
$index sparse, hessian speed double$$

$head link_sparse_hessian$$
$index link_sparse_hessian$$
Routine that computes function values for $cref/sparse_evaluate/$$
$codep */
# include <cppad/vector.hpp>
# include <cppad/speed/uniform_01.hpp>

// must include cmath before sparse_evaluate so that exp is defined for double
# include <cmath>
# include <cppad/speed/sparse_evaluate.hpp>

bool link_sparse_hessian(
	size_t                     repeat   , 
	CppAD::vector<double>     &x        ,
	CppAD::vector<size_t>     &i        ,
	CppAD::vector<size_t>     &j        ,
	CppAD::vector<double>     &hessian  )
{
	// -----------------------------------------------------
	// setup
	using CppAD::vector;
	size_t order = 0;        // derivative order corresponding to function
	size_t n     = x.size(); // argument space dimension
	size_t ell   = i.size(); // size of index vectors
	vector<double> y(1);     // function value

	// temporaries
	size_t k;
	vector<double> tmp(2 * ell);

	// choose a value for x
	CppAD::uniform_01(n, x);
	
	// ------------------------------------------------------

	while(repeat--)
	{
		// get the next set of indices
		CppAD::uniform_01(2 * ell, tmp);
		for(k = 0; k < ell; k++)
		{	i[k] = size_t( n * tmp[k] );
			i[k] = std::min(n-1, i[k]);
			//
			j[k] = size_t( n * tmp[k + ell] );
			j[k] = std::min(n-1, j[k]);
		}

		// computation of the function
		CppAD::sparse_evaluate(x, i, j, order, y);
	}
	hessian[0] = y[0];

	return true;
}
/* $$
$end
*/
