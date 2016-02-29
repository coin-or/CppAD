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
$begin double_sparse_jacobian.cpp$$
$spell
	const
	onetape
	boolsparsity
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
	std
$$

$section Double Speed: Sparse Jacobian$$
$mindex link_sparse_jacobian speed$$


$head Specifications$$
See $cref link_sparse_jacobian$$.

$head Implementation$$

$srccode%cpp% */
# include <cppad/utility/vector.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/speed/sparse_jac_fun.hpp>

// Note that CppAD uses global_option["memory"] at the main program level
# include <map>
extern std::map<std::string, bool> global_option;

bool link_sparse_jacobian(
	size_t                           size     ,
	size_t                           repeat   ,
	size_t                           m        ,
	const CppAD::vector<size_t>&     row      ,
	const CppAD::vector<size_t>&     col      ,
	      CppAD::vector<double>&     x        ,
	      CppAD::vector<double>&     jacobian ,
	      size_t&                    n_sweep  )
{
	if(global_option["onetape"]||global_option["atomic"]||global_option["optimize"]||global_option["boolsparsity"])
		return false;
	// -----------------------------------------------------
	// setup
	using CppAD::vector;
	size_t i;
	size_t order = 0;          // order for computing function value
	size_t n     = size;       // argument space dimension
	vector<double> yp(m);      // function value yp = f(x)

	// ------------------------------------------------------
	while(repeat--)
	{	// choose a value for x
		CppAD::uniform_01(n, x);

		// computation of the function
		CppAD::sparse_jac_fun<double>(m, n, x, row, col, order, yp);
	}
	for(i = 0; i < m; i++)
		jacobian[i] = yp[i];

	return true;
}
/* %$$
$end
*/
