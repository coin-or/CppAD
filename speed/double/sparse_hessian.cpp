/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin double_sparse_hessian.cpp$$
$spell
	const
	hes
	bool
	cppad
	hpp
	CppAD
$$

$section Double Speed: Sparse Hessian$$

$index double, speed sparse hessian$$
$index speed, double sparse hessian$$
$index sparse, hessian speed double$$

$head link_sparse_hessian$$
$index link_sparse_hessian$$
$codep */
# include <cppad/vector.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/speed/sparse_hes_fun.hpp>

bool link_sparse_hessian(
	size_t                           repeat   , 
	CppAD::vector<double>           &x        ,
	const CppAD::vector<size_t>     &row      ,
	const CppAD::vector<size_t>     &col      ,
	CppAD::vector<double>           &hessian  )
{
	// -----------------------------------------------------
	// setup
	using CppAD::vector;
	size_t order = 0;          // derivative order corresponding to function
	size_t n     = x.size();   // argument space dimension
	size_t m     = 1;          // range space dimension 
	vector<double> y(m);       // function value

	// choose a value for x
	CppAD::uniform_01(n, x);
	
	// ------------------------------------------------------

	while(repeat--)
	{
		// computation of the function
		CppAD::sparse_hes_fun(x, row, col, order, y);
	}
	hessian[0] = y[0];

	return true;
}
/* $$
$end
*/
