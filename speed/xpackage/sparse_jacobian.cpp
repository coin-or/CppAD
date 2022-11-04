// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cstring>
# include <cppad/utility/vector.hpp>

/*
$begin xpackage_sparse_jacobian.cpp$$
$spell
   xpackage
   jacobian
$$

$section Xpackage Speed: sparse_jacobian$$

$head Specifications$$
$cref link_sparse_jacobian$$

$srccode%cpp% */
// A xpackage version of this test is not yet available
bool link_sparse_jacobian(
   const std::string&               job      ,
   size_t                           size     ,
   size_t                           repeat   ,
   size_t                           m        ,
   const CppAD::vector<size_t>&     row      ,
   const CppAD::vector<size_t>&     col      ,
   CppAD::vector<double>&           x        ,
   CppAD::vector<double>&           jacobian ,
   size_t&                          n_color  )
{
   return false;
}
/* %$$
$end
*/
