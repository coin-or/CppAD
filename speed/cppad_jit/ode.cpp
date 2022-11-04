// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
$begin cppad_jit_ode.cpp$$
$spell
    cppad_jit
$$

$section cppad_jit Speed: Ode$$

$head Specifications$$
$cref link_ode$$

$head Implementation$$
// a cppad_jit version of this test is not yet implemented
$srccode%cpp% */
# include <map>
# include <cppad/utility/vector.hpp>

// list of possible options
extern std::map<std::string, bool> global_option;

bool link_ode(
    size_t                     size       ,
    size_t                     repeat     ,
    CppAD::vector<double>      &x         ,
    CppAD::vector<double>      &jacobian  )
{   return false; }
/* %$$
$end
*/
