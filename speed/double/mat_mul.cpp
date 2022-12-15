// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin double_mat_mul.cpp}

Double Speed: Matrix Multiplication
###################################

Specifications
**************
See :ref:`link_mat_mul-name` .

Implementation
**************
{xrst_spell_off}
{xrst_code cpp} */
# include <cppad/cppad.hpp>
# include <cppad/speed/mat_sum_sq.hpp>
# include <cppad/speed/uniform_01.hpp>

// Note that CppAD uses global_option["memory"] at the main program level
# include <map>
extern std::map<std::string, bool> global_option;

bool link_mat_mul(
   size_t                           size     ,
   size_t                           repeat   ,
   CppAD::vector<double>&           x        ,
   CppAD::vector<double>&           z        ,
   CppAD::vector<double>&           dz
)
{
   if(global_option["onetape"]||global_option["atomic"]||global_option["optimize"])
      return false;
   // -----------------------------------------------------
   size_t n = size * size; // number of independent variables
   CppAD::vector<double> y(n);

   while(repeat--)
   {  // get the next matrix
      CppAD::uniform_01(n, x);

      // do computation
      mat_sum_sq(size, x, y, z);

   }
   return true;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end double_mat_mul.cpp}
*/
