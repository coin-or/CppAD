// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin double_sparse_hessian.cpp}

Double Speed: Sparse Hessian
############################

Specifications
**************
See :ref:`link_sparse_hessian-name` .

Implementation
**************

{xrst_spell_off}
{xrst_code cpp} */
# include <cppad/utility/vector.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/speed/sparse_hes_fun.hpp>

// Note that CppAD uses global_option["memory"] at the main program level
# include <map>
extern std::map<std::string, bool> global_option;

bool link_sparse_hessian(
   size_t                           size     ,
   size_t                           repeat   ,
   const CppAD::vector<size_t>&     row      ,
   const CppAD::vector<size_t>&     col      ,
   CppAD::vector<double>&           x        ,
   CppAD::vector<double>&           hessian  ,
   size_t&                          n_color  )
{
   if(global_option["onetape"]||global_option["atomic"]||global_option["optimize"]||global_option["boolsparsity"])
      return false;
   // -----------------------------------------------------
   // setup
   using CppAD::vector;
   size_t order = 0;          // derivative order corresponding to function
   size_t n     = size;       // argument space dimension
   size_t m     = 1;          // range space dimension
   vector<double> y(m);       // function value

   // choose a value for x
   CppAD::uniform_01(n, x);

   // ------------------------------------------------------

   while(repeat--)
   {
      // computation of the function
      CppAD::sparse_hes_fun<double>(n, x, row, col, order, y);
   }
   hessian[0] = y[0];

   return true;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end double_sparse_hessian.cpp}
*/
