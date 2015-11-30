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
$begin double_det_lu.cpp$$
$spell
	onetape
	bool
	cppad
	hpp
	Lu
	det
	CppAD
$$

$section Double Speed: Determinant Using Lu Factorization$$
$mindex link_det_lu speed matrix factor$$


$head Specifications$$
See $cref link_det_lu$$.

$head Implementation$$
$codep */
# include <cppad/utility/vector.hpp>
# include <cppad/speed/det_by_lu.hpp>
# include <cppad/speed/uniform_01.hpp>

// Note that CppAD uses global_memory at the main program level
extern bool
	global_onetape, global_atomic, global_optimize;

bool link_det_lu(
	size_t                           size     ,
	size_t                           repeat   ,
	CppAD::vector<double>           &matrix   ,
	CppAD::vector<double>           &det      )
{
	if(global_onetape||global_atomic||global_optimize)
		return false;
	// -----------------------------------------------------
	// setup
	CppAD::det_by_lu<double>  Det(size);
	size_t n = size * size; // number of independent variables

	// ------------------------------------------------------

	while(repeat--)
	{	// get the next matrix
		CppAD::uniform_01(n, matrix);

		// computation of the determinant
		det[0] = Det(matrix);
	}
	return true;
}
/* $$
$end
*/
