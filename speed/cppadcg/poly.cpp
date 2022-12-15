// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin cppadcg_poly.cpp}

Cppadcg Speed: Second Derivative of a Polynomial
################################################

Specifications
**************
:ref:`link_poly-name`

Implementation
**************
// a cppadcg version of this test is not yet implemented
{xrst_spell_off}
{xrst_code cpp} */
# include <map>
# include <cppad/utility/vector.hpp>

// list of possible options
extern std::map<std::string, bool> global_option;

bool link_poly(
   size_t                     size     ,
   size_t                     repeat   ,
   CppAD::vector<double>     &a        ,  // coefficients of polynomial
   CppAD::vector<double>     &z        ,  // polynomial argument value
   CppAD::vector<double>     &ddp      )  // second derivative w.r.t z
{  return false; }
/* {xrst_code}
{xrst_spell_on}

{xrst_end cppadcg_poly.cpp}
*/
