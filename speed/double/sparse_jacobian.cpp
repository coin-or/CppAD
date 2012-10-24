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
$begin double_sparse_jacobian.cpp$$
$spell
	yp
	jac
	Jacobian
	fp
	bool
	cppad
	hpp
	CppAD
	cmath
	exp
	tmp
	std
$$

$section Double Speed: Sparse Jacobian$$

$index link_sparse_jacobian, double$$
$index double, link_sparse_jacobian$$
$index speed, double$$
$index double, speed$$
$index sparse, speed double$$
$index jacobian, speed double$$

$head Specifications$$
See $cref link_sparse_jacobian$$.

$head Implementation$$

$codep */
# include <cppad/vector.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/speed/sparse_jac_fun.hpp>

bool link_sparse_jacobian(
	size_t                     size     , 
	size_t                     repeat   , 
	size_t                     m        ,
	CppAD::vector<double>     &x        ,
	CppAD::vector<size_t>     &row      ,
	CppAD::vector<size_t>     &col      ,
	CppAD::vector<double>     &jacobian )
{
	// -----------------------------------------------------
	// setup
	using CppAD::vector;
	size_t i, k;
	size_t order = 0;          // order for computing function value
	size_t n     = size;       // argument space dimension
	size_t K     = row.size(); // size of index vectors
	vector<double> yp(m);      // function value yp = f(x)

	// temporaries
	vector<double> tmp(2 * K);

	// choose a value for x
	CppAD::uniform_01(n, x);
	
	// ------------------------------------------------------

	while(repeat--)
	{
		// get the next set of indices
		CppAD::uniform_01(2 * K, tmp);
		for(k = 0; k < K; k++)
		{	row[k] = size_t( n * tmp[k] );
			row[k] = std::min(n-1, row[k]);
			//
			col[k] = size_t( n * tmp[k + K] );
			col[k] = std::min(n-1, col[k]);
		}

		// computation of the function
		CppAD::sparse_jac_fun<double>(m, n, x, row, col, order, yp);
	}
	for(i = 0; i < m; i++)
		jacobian[i] = yp[i];

	return true;
}
/* $$
$end
*/
