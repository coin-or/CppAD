# ifndef CPPAD_CPPAD_IPOPT_SRC_HES_FG_MAP_HPP
# define CPPAD_CPPAD_IPOPT_SRC_HES_FG_MAP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
# include "cppad_ipopt_nlp.hpp"
/*!
\file hes_fg_map.hpp
\brief Create a mapping between two representations for Hessian of fg.

\ingroup hes_fg_map_cpp
*/
// ---------------------------------------------------------------------------
namespace cppad_ipopt {
// ---------------------------------------------------------------------------


extern void hes_fg_map(
   cppad_ipopt_fg_info*  fg_info                                  ,
   size_t                                          m              ,
   size_t                                          n              ,
   size_t                                          K              ,
   const CppAD::vector<size_t>&                    L              ,
   const CppAD::vector<size_t>&                    p              ,
   const CppAD::vector<size_t>&                    q              ,
   const CppAD::vector<CppAD::vectorBool>&         pattern_hes_r  ,
   CppAD::vector<size_t>&                          I              ,
   CppAD::vector<size_t>&                          J              ,
   CppAD::vector< std::map<size_t,size_t> >&       index_hes_fg
);

// ---------------------------------------------------------------------------
} // end namespace cppad_ipopt
// ---------------------------------------------------------------------------

# endif
