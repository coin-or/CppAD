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
$begin fadbad_sparse_jacobian.cpp$$
$spell
        bool
        CppAD
        fadbad
	sparse_jacobian
$$

$section fadbad Speed: sparse_jacobian$$

$codep */
// The fadbad version of this test is not yet available
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
