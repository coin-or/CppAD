/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin double_poly.cpp$$
$spell
	retape
	bool
	cppad
	CppAD
	hpp
$$

$section Double Speed: Evaluate a Polynomial$$

$index cppad, speed polynomial$$
$index speed, cppad polynomial$$
$index polynomial, speed cppad$$

$head link_poly$$
$index link_poly$$
Routine that computes the second derivative of a polynomial using CppAD:
$codep */
# include <cppad/cppad.hpp>
# include <cppad/speed/uniform_01.hpp>

bool link_poly(
	size_t                     size     , 
	size_t                     repeat   , 
	bool                       retape   ,
	CppAD::vector<double>     &a        ,  // coefficients of polynomial
	CppAD::vector<double>     &z        ,  // polynomial argument value
	CppAD::vector<double>     &p        )  // second derivative w.r.t z  
{
	// -----------------------------------------------------
	// setup

	// ------------------------------------------------------
	while(repeat--)
	{	// get the next argument value
		CppAD::uniform_01(1, z);

		// evaluate the polynomial at the new argument value
		p[0] = CppAD::Poly(0, a, z[0]);
	}
	return true;
}
/* $$
$end
*/
