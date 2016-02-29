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
$begin double_mat_mul.cpp$$
$spell
	onetape
	CppAD
	cppad
	mul_mat
	hpp
	sq
	bool
	dz
	typedef
$$

$section CppAD Speed: Matrix Multiplication (Double Version)$$
$mindex speed multiply link_mat_mul$$


$head Specifications$$
See $cref link_mat_mul$$.

$head Implementation$$
$srccode%cpp% */
# include <cppad/cppad.hpp>
# include <cppad/speed/mat_sum_sq.hpp>
# include <cppad/speed/uniform_01.hpp>

// Note that CppAD uses global_option["memory"] at the main program level
# include <map>
extern std::map<std::string, bool> global_option;

bool link_mat_mul(
	size_t                           size     ,
	size_t                           repeat   ,
	CppAD::vector<double>&           x        ,
	CppAD::vector<double>&           z        ,
	CppAD::vector<double>&           dz
)
{
	if(global_option["onetape"]||global_option["atomic"]||global_option["optimize"])
		return false;
	// -----------------------------------------------------
	size_t n = size * size; // number of independent variables
	CppAD::vector<double> y(n);

	while(repeat--)
	{	// get the next matrix
		CppAD::uniform_01(n, x);

		// do computation
		mat_sum_sq(size, x, y, z);

	}
	return true;
}
/* %$$
$end
*/
