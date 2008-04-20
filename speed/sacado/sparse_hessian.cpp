/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/vector.hpp>

/*
$begin sacado_sparse_hessian.cpp$$
$spell
	Sacado
	bool
	CppAD
$$

$section Sacado Speed: Sparse Hessian$$

Indicate that this test is not available:
$codep */

// The sacado version of this test is not yet available 
extern bool link_sparse_hessian(
        size_t                     repeat     ,
        CppAD::vector<double>      &x         ,
        CppAD::vector<size_t>      &i         ,
        CppAD::vector<size_t>      &j         ,
        CppAD::vector<double>      &hessian
)
{
	return false;
}
/* $$
$end
*/
