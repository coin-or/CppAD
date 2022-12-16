// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin sacado_mat_mul.cpp}

Sacado Speed: Matrix Multiplication
###################################

Specifications
**************
See :ref:`link_mat_mul-name` .

Implementation
**************

{xrst_spell_off}
{xrst_code cpp} */
// suppress conversion warnings before other includes
# include <cppad/wno_conversion.hpp>
//
# include <Sacado.hpp>
# include <cppad/utility/vector.hpp>
# include <cppad/speed/mat_sum_sq.hpp>
# include <cppad/speed/uniform_01.hpp>

// list of possible options
# include <map>
extern std::map<std::string, bool> global_option;

bool link_mat_mul(
   size_t                           size     ,
   size_t                           repeat   ,
   CppAD::vector<double>&           x        ,
   CppAD::vector<double>&           z        ,
   CppAD::vector<double>&           dz       )
{
   // speed test global option values
   if( global_option["memory"] || global_option["onetape"] || global_option["atomic"] || global_option["optimize"] )
      return false;
   // -----------------------------------------------------
   // setup

   // object for computing determinant
   typedef Sacado::Rad::ADvar<double>    ADScalar;
   typedef CppAD::vector<ADScalar>       ADVector;

   size_t j;                // temporary index
   size_t m = 1;            // number of dependent variables
   size_t n = size * size;  // number of independent variables
   ADVector   X(n);         // AD domain space vector
   ADVector   Y(n);         // Store product matrix
   ADVector   Z(m);         // AD range space vector
   ADScalar   f;

   // ------------------------------------------------------
   while(repeat--)
   {  // get the next matrix
      CppAD::uniform_01(n, x);

      // set independent variable values
      for(j = 0; j < n; j++)
         X[j] = x[j];

      // do the computation
      mat_sum_sq(size, X, Y, Z);

      // create function object f : X -> Z
      f = Z[0];

      // reverse mode gradient of last ADvar computed value; i.e., f
      ADScalar::Gradcomp();

      // return gradient
      for(j = 0; j < n; j++)
         dz[j] = X[j].adj(); // partial f w.r.t X[j]
   }
   // return function value
   z[0] = f.val();

   // ---------------------------------------------------------
   return true;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end sacado_mat_mul.cpp}
*/
