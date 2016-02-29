// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin double_sparse_hessian.cpp$$
$spell
	onetape
	boolsparsity
	const
	hes
	bool
	cppad
	hpp
	CppAD
$$

$section Double Speed: Sparse Hessian$$
$mindex link_sparse_hessian speed$$


$head Specifications$$
See $cref link_sparse_hessian$$.

$head Implementation$$

$srccode%cpp% */
# include <cppad/utility/vector.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/speed/sparse_hes_fun.hpp>

// Note that CppAD uses global_option["memory"] at the main program level
# include <map>
extern std::map<std::string, bool> global_option;

bool link_sparse_hessian(
	size_t                           size     ,
	size_t                           repeat   ,
	const CppAD::vector<size_t>&     row      ,
	const CppAD::vector<size_t>&     col      ,
	CppAD::vector<double>&           x        ,
	CppAD::vector<double>&           hessian  ,
	size_t&                          n_sweep  )
{
	if(global_option["onetape"]||global_option["atomic"]||global_option["optimize"]||global_option["boolsparsity"])
		return false;
	// -----------------------------------------------------
	// setup
	using CppAD::vector;
	size_t order = 0;          // derivative order corresponding to function
	size_t n     = size;       // argument space dimension
	size_t m     = 1;          // range space dimension
	vector<double> y(m);       // function value

	// choose a value for x
	CppAD::uniform_01(n, x);

	// ------------------------------------------------------

	while(repeat--)
	{
		// computation of the function
		CppAD::sparse_hes_fun<double>(n, x, row, col, order, y);
	}
	hessian[0] = y[0];

	return true;
}
/* %$$
$end
*/
