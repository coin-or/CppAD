/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin sacado_mat_mul.cpp$$
$spell
	Sacado
	cppad.hpp
	bool
	mul
	dz
	CppAD
$$

$section Sacado Speed: Matrix Multiplication$$

$codep */
// The Sacado version of this test is not yet available
# include <cppad/vector.hpp>
bool link_mat_mul(
	size_t                           size     , 
	size_t                           repeat   , 
	CppAD::vector<double>&           x        ,
	CppAD::vector<double>&           z        ,
	CppAD::vector<double>&           dz
)
{	return false; }
/* $$
$end
*/
