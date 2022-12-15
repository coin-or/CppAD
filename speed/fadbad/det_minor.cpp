// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin fadbad_det_minor.cpp}

Fadbad Speed: Gradient of Determinant by Minor Expansion
########################################################

Specifications
**************
See :ref:`link_det_minor-name` .

Implementation
**************

{xrst_spell_off}
{xrst_code cpp} */
// suppress conversion warnings before other includes
# include <cppad/wno_conversion.hpp>
//
# include <FADBAD++/badiff.h>
# include <cppad/speed/det_by_minor.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/utility/vector.hpp>

// list of possible options
# include <map>
extern std::map<std::string, bool> global_option;

bool link_det_minor(
   const std::string&         job      ,
   size_t                     size     ,
   size_t                     repeat   ,
   CppAD::vector<double>     &matrix   ,
   CppAD::vector<double>     &gradient )
{
   // --------------------------------------------------------------------
   // check not global options
   typedef std::map<std::string, bool>::iterator iterator;
   for(iterator itr=global_option.begin(); itr!=global_option.end(); ++itr)
   {  if( itr->second )
         return false;
   }
   // -----------------------------------------------------

   // AD types
   typedef fadbad::B<double>       b_double;
   typedef CppAD::vector<b_double> b_vector;

   // object that computes the determinant
   CppAD::det_by_minor<b_double>   b_det(size);

   // number of dependent variables
   unsigned int m = 1;

   // number of independent variables
   size_t n = size * size;

   // independent variable vector
   b_vector   b_A(n);

   // AD value of the determinant
   b_double  b_detA;

   // ------------------------------------------------------
   while(repeat--)
   {  // get the next matrix
      CppAD::uniform_01(n, matrix);

      // set independent variable values
      for(size_t j = 0; j < n; j++)
         b_A[j] = matrix[j];

      // compute the determinant
      b_detA = b_det(b_A);

      // create function object f : A -> detA
      b_detA.diff(0, m);  // index 0 of m dependent variables

      // evaluate and return gradient using reverse mode
      for(size_t j = 0; j < n; j++)
         gradient[j] = b_A[j].d(0); // partial detA w.r.t A[i]
   }
   // ---------------------------------------------------------
   return true;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end fadbad_det_minor.cpp}
*/
