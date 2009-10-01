/* $Id: sparse_jacobian.cpp */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cstring>
# include <cppad/vector.hpp>

/*
$begin sacado_sparse_jacobian.cpp$$
$spell
        bool
        CppAD
        sacado
	sparse_jacobian
$$

$section sacado Speed: sparse_jacobian$$

$codep */
// The sacado version of this test is not yet available
bool link_sparse_jacobian(
	size_t                     repeat   , 
	CppAD::vector<double>     &x        ,
	CppAD::vector<size_t>     &i        ,
	CppAD::vector<size_t>     &j        ,
	CppAD::vector<double>     &jacobian )
{
	return false;
}
/* $$
$end
*/
