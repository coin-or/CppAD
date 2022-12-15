// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin double_sparse_jacobian.cpp}

Double Speed: Sparse Jacobian
#############################

Specifications
**************
See :ref:`link_sparse_jacobian-name` .

Implementation
**************

{xrst_spell_off}
{xrst_code cpp} */
# include <cppad/utility/vector.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/speed/sparse_jac_fun.hpp>

// Note that CppAD uses global_option["memory"] at the main program level
# include <map>
extern std::map<std::string, bool> global_option;

bool link_sparse_jacobian(
   const std::string&               job      ,
   size_t                           size     ,
   size_t                           repeat   ,
   size_t                           m        ,
   const CppAD::vector<size_t>&     row      ,
   const CppAD::vector<size_t>&     col      ,
          CppAD::vector<double>&     x        ,
          CppAD::vector<double>&     jacobian ,
          size_t&                    n_color  )
{
   if(global_option["onetape"]||global_option["atomic"]||global_option["optimize"]||global_option["boolsparsity"])
      return false;
   // -----------------------------------------------------
   // setup
   using CppAD::vector;
   size_t i;
   size_t order = 0;          // order for computing function value
   size_t n     = size;       // argument space dimension
   vector<double> yp(m);      // function value yp = f(x)

   // ------------------------------------------------------
   while(repeat--)
   {  // choose a value for x
      CppAD::uniform_01(n, x);

      // computation of the function
      CppAD::sparse_jac_fun<double>(m, n, x, row, col, order, yp);
   }
   for(i = 0; i < m; i++)
      jacobian[i] = yp[i];

   return true;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end double_sparse_jacobian.cpp}
*/
