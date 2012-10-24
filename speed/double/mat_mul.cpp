/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin double_mat_mul.cpp$$
$spell
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

$index cppad, speed matrix multiply$$
$index speed, cppad matrix multiply$$
$index matrix, multiply speed cppad$$
$index multiply, matrix speed cppad$$

$head Specifications$$
See $cref link_mat_mul$$.

$head Implementation$$
$index cppad, link_mat_mul$$
$index link_mat_mul, cppad$$
$codep */
# include <cppad/cppad.hpp>
# include <cppad/speed/mat_sum_sq.hpp>
# include <cppad/speed/uniform_01.hpp>

bool link_mat_mul(
	size_t                           size     , 
	size_t                           repeat   , 
	CppAD::vector<double>&           x        ,
	CppAD::vector<double>&           z        ,
	CppAD::vector<double>&           dz
)
{
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
/* $$
$end
*/
