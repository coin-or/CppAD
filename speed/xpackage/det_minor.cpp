// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin xpackage_det_minor.cpp}

Xpackage Speed: Gradient of Determinant by Minor Expansion
##########################################################

Specifications
**************
:ref:`link_det_minor-name`

Implementation
**************
// a xpackage version of this test is not yet implemented
{xrst_spell_off}
{xrst_code cpp} */
# include <map>
# include <cppad/utility/vector.hpp>

// list of possible options
extern std::map<std::string, bool> global_option;

bool link_det_minor(
   const std::string&         job      ,
   size_t                     size     ,
   size_t                     repeat   ,
   CppAD::vector<double>     &matrix   ,
   CppAD::vector<double>     &gradient )
{  return false; }
/* {xrst_code}
{xrst_spell_on}

{xrst_end xpackage_det_minor.cpp}
*/
