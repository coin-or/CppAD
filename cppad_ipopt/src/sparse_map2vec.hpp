// $Id$
# ifndef CPPAD_CPPAD_IPOPT_SRC_SPARSE_MAP2VEC_HPP
# define CPPAD_CPPAD_IPOPT_SRC_SPARSE_MAP2VEC_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
// ---------------------------------------------------------------------------
namespace cppad_ipopt {
// ---------------------------------------------------------------------------
/*!
\file sparse_map2vec.hpp
\brief Create a two vector sparsity representation from a vector of maps.

\ingroup sparese_map2vec_cpp
*/

extern void sparse_map2vec(
	const CppAD::vector< std::map<size_t, size_t> > sparse,
	size_t&                                         n_nz  ,
	CppAD::vector<size_t>&                          i_row ,
	CppAD::vector<size_t>&                          j_col
);

// ---------------------------------------------------------------------------
} // end namespace cppad_ipopt
// ---------------------------------------------------------------------------
# endif
