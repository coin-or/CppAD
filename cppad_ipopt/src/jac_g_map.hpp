# ifndef CPPAD_CPPAD_IPOPT_SRC_JAC_G_MAP_HPP
# define CPPAD_CPPAD_IPOPT_SRC_JAC_G_MAP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
# include "cppad_ipopt_nlp.hpp"
/*!
\file jac_g_map.hpp
\brief Create a mapping between two representations for Jacobian of g.

\ingroup jac_g_map_cpp
*/
// ---------------------------------------------------------------------------
namespace cppad_ipopt {
// ---------------------------------------------------------------------------


extern void jac_g_map(
   cppad_ipopt_fg_info*  fg_info                                  ,
   size_t                                          m              ,
   size_t                                          n              ,
   size_t                                          K              ,
   const CppAD::vector<size_t>&                    L              ,
   const CppAD::vector<size_t>&                    p              ,
   const CppAD::vector<size_t>&                    q              ,
   const CppAD::vector<CppAD::vectorBool>&         pattern_jac_r  ,
   CppAD::vector<size_t>&                          I              ,
   CppAD::vector<size_t>&                          J              ,
   CppAD::vector< std::map<size_t,size_t> >&       index_jac_g
);

// ---------------------------------------------------------------------------
} // end namespace cppad_ipopt
// ---------------------------------------------------------------------------

# endif
