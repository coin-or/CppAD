// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/utility/vector.hpp>

/*
$begin fadbad_sparse_hessian.cpp$$
$spell
   const
   Fadbad
   bool
   CppAD
$$

$section Fadbad Speed: Sparse Hessian$$

$srccode%cpp% */
// A fadbad version of this test is not yet available
bool link_sparse_hessian(
   size_t                            size      ,
   size_t                            repeat    ,
   const CppAD::vector<size_t>&      row       ,
   const CppAD::vector<size_t>&      col       ,
   CppAD::vector<double>&            x         ,
   CppAD::vector<double>&            hessian   ,
   size_t&                           n_color
)
{
   return false;
}
/* %$$
$end
*/
