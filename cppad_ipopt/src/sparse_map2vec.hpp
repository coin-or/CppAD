/* $Id$ */
# ifndef  CPPAD_SPARSE_MAP2VEC_INCLUDED
# define  CPPAD_SPARSE_MAP2VEC_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
CPPAD_BEGIN_NAMESPACE
/*!
\file sparse_map2vec.hpp
\brief Create a two vector sparsity representation from a vector of maps.
*/

extern void sparse_map2vec(
	const CppAD::vector< std::map<size_t, size_t> > sparse,
	size_t&                                         n_nz  ,
	CppAD::vector<size_t>&                          i_row ,
	CppAD::vector<size_t>&                          j_col
);

CPPAD_END_NAMESPACE
# endif
