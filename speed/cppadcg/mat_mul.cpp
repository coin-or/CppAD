// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
$begin cppadcg_mat_mul.cpp$$
$spell
   cppadcg
$$

$section Cppadcg Speed: Matrix Multiplication$$


$head Specifications$$
$cref link_mat_mul$$

$head Implementation$$
// a cppadcg version of this test is not yet implemented
$srccode%cpp% */
# include <map>
# include <cppad/utility/vector.hpp>

// list of possible options
extern std::map<std::string, bool> global_option;

bool link_mat_mul(
   size_t                           size     ,
   size_t                           repeat   ,
   CppAD::vector<double>&           x        ,
   CppAD::vector<double>&           z        ,
   CppAD::vector<double>&           dz       )
{  return false; }
/* %$$
$end
*/
