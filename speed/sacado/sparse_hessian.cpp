/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/vector.hpp>

/*
$begin sacado_sparse_hessian.cpp$$
$spell
	const
	Sacado
	bool
	CppAD
$$

$section Sacado Speed: Sparse Hessian$$

$codep */
// The sacado version of this test is not yet available 
extern bool link_sparse_hessian(
        size_t                           size       ,
        size_t                           repeat     ,
        CppAD::vector<double>            &x         ,
        const CppAD::vector<size_t>      &row       ,
        const CppAD::vector<size_t>      &col       ,
        CppAD::vector<double>            &hessian
)
{
	return false;
}
/* $$
$end
*/
