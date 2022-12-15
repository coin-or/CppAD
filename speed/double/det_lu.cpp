// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin double_det_lu.cpp}

Double Speed: Determinant Using Lu Factorization
################################################

Specifications
**************
See :ref:`link_det_lu-name` .

Implementation
**************
{xrst_spell_off}
{xrst_code cpp} */
# include <cppad/utility/vector.hpp>
# include <cppad/speed/det_by_lu.hpp>
# include <cppad/speed/uniform_01.hpp>

// Note that CppAD uses global_option["memory"] at the main program level
# include <map>
extern std::map<std::string, bool> global_option;

bool link_det_lu(
   size_t                           size     ,
   size_t                           repeat   ,
   CppAD::vector<double>           &matrix   ,
   CppAD::vector<double>           &det      )
{
   if(global_option["onetape"]||global_option["atomic"]||global_option["optimize"])
      return false;
   // -----------------------------------------------------
   // setup
   CppAD::det_by_lu<double>  Det(size);
   size_t n = size * size; // number of independent variables

   // ------------------------------------------------------

   while(repeat--)
   {  // get the next matrix
      CppAD::uniform_01(n, matrix);

      // computation of the determinant
      det[0] = Det(matrix);
   }
   return true;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end double_det_lu.cpp}
*/
