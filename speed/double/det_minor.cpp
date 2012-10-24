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
$begin double_det_minor.cpp$$
$spell
	retape
	bool
	cppad
	det
	CppAD
	hpp
$$

$section Double Speed: Determinant by Minor Expansion$$

$index link_det_minor, double$$
$index double, link_det_minor$$
$index speed, double$$
$index double, speed$$
$index minor, speed double$$
$index determinant, speed double$$

$head Specifications$$
See $cref link_det_minor$$.

$head Implementation$$

$codep */
# include <cppad/vector.hpp>
# include <cppad/speed/det_by_minor.hpp>
# include <cppad/speed/uniform_01.hpp>

bool link_det_minor(
	size_t                     size     , 
	size_t                     repeat   , 
	CppAD::vector<double>     &matrix   ,
	CppAD::vector<double>     &det      )
{
	// -----------------------------------------------------
	// setup
	CppAD::det_by_minor<double>   Det(size);
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
