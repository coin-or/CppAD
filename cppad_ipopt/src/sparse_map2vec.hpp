# ifndef CPPAD_CPPAD_IPOPT_SRC_SPARSE_MAP2VEC_HPP
# define CPPAD_CPPAD_IPOPT_SRC_SPARSE_MAP2VEC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
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
