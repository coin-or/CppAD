# ifndef CPPAD_CPPAD_IPOPT_SRC_VEC_FUN_PATTERN_HPP
# define CPPAD_CPPAD_IPOPT_SRC_VEC_FUN_PATTERN_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
# include "cppad_ipopt_nlp.hpp"
// ---------------------------------------------------------------------------
namespace cppad_ipopt {
// ---------------------------------------------------------------------------
/*!
\file vec_fun_pattern.hpp
\brief Determine sparsity pattern for a vector of AD fuction objects.

\ingroup vec_fun_pattern_cpp
*/

extern void vec_fun_pattern(
   size_t                                          K              ,
   const CppAD::vector<size_t>&                    p              ,
   const CppAD::vector<size_t>&                    q              ,
   const CppAD::vectorBool&                        retape         ,
   CppAD::vector< CppAD::ADFun<Ipopt::Number> >&   r_fun          ,
   CppAD::vector<CppAD::vectorBool>&               pattern_jac_r  ,
   CppAD::vector<CppAD::vectorBool>&               pattern_hes_r
);

// ---------------------------------------------------------------------------
} // end namespace cppad_ipopt
// ---------------------------------------------------------------------------
# endif
