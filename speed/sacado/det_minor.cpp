// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin sacado_det_minor.cpp}

Sacado Speed: Gradient of Determinant by Minor Expansion
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
# include <Sacado.hpp>
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
   // check none of the global options is true
   typedef std::map<std::string, bool>::iterator iterator;
   for(iterator itr=global_option.begin(); itr!=global_option.end(); ++itr)
   {  if( itr->second )
         return false;
   }
   // -----------------------------------------------------
   // not using job
   // -----------------------------------------------------

   // AD types
   typedef Sacado::Rad::ADvar<double>    r_double;
   typedef CppAD::vector<r_double>       r_vector;

   // object for computing deterinant
   CppAD::det_by_minor<r_double>         r_det(size);

   // number of independent variables
   size_t n = size * size;

   // independent variable vector
   r_vector   r_A(n);

   // AD value of the determinant
   r_double   r_detA;

   // ------------------------------------------------------
   while(repeat--)
   {  // get the next matrix
      CppAD::uniform_01(n, matrix);

      // set independent variable values
      for(size_t j = 0; j < n; ++j)
         r_A[j] = matrix[j];

      // compute the determinant
      r_detA = r_det(r_A);

      // reverse mode compute gradient of last computed value; i.e., detA
      r_double::Gradcomp();

      // return gradient
      for(size_t j =0; j < n; ++j)
         gradient[j] = r_A[j].adj(); // partial detA w.r.t A[j]
   }
   // ---------------------------------------------------------
   return true;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end sacado_det_minor.cpp}
*/
