// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin fadbad_det_lu.cpp}

Fadbad Speed: Gradient of Determinant Using Lu Factorization
############################################################

Specifications
**************
See :ref:`link_det_lu-name` .

Implementation
**************
{xrst_spell_off}
{xrst_code cpp} */
// suppress conversion warnings before other includes
# include <cppad/wno_conversion.hpp>
//
# include <FADBAD++/badiff.h>
# include <cppad/speed/det_by_lu.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/utility/vector.hpp>

// list of possible options
# include <map>
extern std::map<std::string, bool> global_option;

bool link_det_lu(
   size_t                     size     ,
   size_t                     repeat   ,
   CppAD::vector<double>     &matrix   ,
   CppAD::vector<double>     &gradient )
{
   // speed test global option values
   if( global_option["onetape"] || global_option["atomic"] )
      return false;
   if( global_option["memory"] || global_option["optimize"] )
      return false;
   // -----------------------------------------------------
   // setup
   //
   // object for computing determinant
   typedef fadbad::B<double>       ADScalar;
   typedef CppAD::vector<ADScalar> ADVector;
   CppAD::det_by_lu<ADScalar>      Det(size);

   size_t i;                // temporary index
   size_t m = 1;            // number of dependent variables
   size_t n = size * size;  // number of independent variables
   ADScalar   detA;         // AD value of the determinant
   ADVector   A(n);         // AD version of matrix

   // ------------------------------------------------------
   while(repeat--)
   {  // get the next matrix
      CppAD::uniform_01(n, matrix);

      // set independent variable values
      for(i = 0; i < n; i++)
         A[i] = matrix[i];

      // compute the determinant
      detA = Det(A);

      // create function object f : A -> detA
      detA.diff(0, (unsigned int) m);  // index 0 of m dependent variables

      // evaluate and return gradient using reverse mode
      for(i =0; i < n; i++)
         gradient[i] = A[i].d(0); // partial detA w.r.t A[i]
   }
   // ---------------------------------------------------------
   return true;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end fadbad_det_lu.cpp}
*/
