// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin fadbad_mat_mul.cpp}

Fadbad Speed: Matrix Multiplication
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
# include <FADBAD++/badiff.h>
# include <cppad/speed/mat_sum_sq.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/utility/vector.hpp>

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
   // The correctness check for this test is failing, so abort (for now).
   return false;

   // -----------------------------------------------------
   // setup

   // object for computing determinant
   typedef fadbad::B<double>       ADScalar;
   typedef CppAD::vector<ADScalar> ADVector;

   size_t j;                // temporary index
   size_t m = 1;            // number of dependent variables
   size_t n = size * size;  // number of independent variables
   ADVector   X(n);         // AD domain space vector
   ADVector   Y(n);         // Store product matrix
   ADVector   Z(m);         // AD range space vector

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
      Z[0].diff(0, m);  // index 0 of m dependent variables

      // evaluate and return gradient using reverse mode
      for(j = 0; j < n; j++)
         dz[j] = X[j].d(0); // partial Z[0] w.r.t X[j]
   }
   // return function value
   z[0] = Z[0].x();

   // ---------------------------------------------------------
   return true;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end fadbad_mat_mul.cpp}
*/
