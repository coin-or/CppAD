// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin double_poly.cpp}

Double Speed: Evaluate a Polynomial
###################################

Specifications
**************
See :ref:`link_poly-name` .

Implementation
**************

{xrst_spell_off}
{xrst_code cpp} */
# include <cppad/cppad.hpp>
# include <cppad/speed/uniform_01.hpp>

// Note that CppAD uses global_option["memory"] at the main program level
# include <map>
extern std::map<std::string, bool> global_option;

bool link_poly(
   size_t                     size     ,
   size_t                     repeat   ,
   CppAD::vector<double>     &a        ,  // coefficients of polynomial
   CppAD::vector<double>     &z        ,  // polynomial argument value
   CppAD::vector<double>     &p        )  // second derivative w.r.t z
{
   if(global_option["onetape"]||global_option["atomic"]||global_option["optimize"])
      return false;
   // -----------------------------------------------------
   // setup

   // ------------------------------------------------------
   while(repeat--)
   {  // get the next argument value
      CppAD::uniform_01(1, z);

      // evaluate the polynomial at the new argument value
      p[0] = CppAD::Poly(0, a, z[0]);
   }
   return true;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end double_poly.cpp}
*/
