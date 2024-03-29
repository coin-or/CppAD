// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin fadbad_poly.cpp}

Fadbad Speed: Second Derivative of a Polynomial
###############################################

Specifications
**************
See :ref:`link_poly-name` .

Implementation
**************

{xrst_spell_off}
{xrst_code cpp} */
# include <cppad/utility/vector.hpp>
# include <cppad/utility/poly.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <FADBAD++/tadiff.h>

// list of possible options
# include <map>
extern std::map<std::string, bool> global_option;

bool link_poly(
   size_t                     size     ,
   size_t                     repeat   ,
   CppAD::vector<double>     &a        ,  // coefficients of polynomial
   CppAD::vector<double>     &z        ,  // polynomial argument value
   CppAD::vector<double>     &ddp      )  // second derivative w.r.t z
{
   if( global_option["atomic"] )
      return false;
   if( global_option["memory"] || global_option["onetape"] || global_option["optimize"] )
      return false;
   // -----------------------------------------------------
   // setup
   size_t i;             // temporary index
   fadbad::T<double>  Z; // domain space AD value
   fadbad::T<double>  P; // range space AD value

   // choose the polynomial coefficients
   CppAD::uniform_01(size, a);

   // AD copy of the polynomial coefficients
   CppAD::vector< fadbad::T<double> > A(size);
   for(i = 0; i < size; i++)
      A[i] = a[i];

   // ------------------------------------------------------
   while(repeat--)
   {  // get the next argument value
      CppAD::uniform_01(1, z);

      // independent variable value
      Z    = z[0]; // argument value
      Z[1] = 1;    // argument first order Taylor coefficient

      // AD computation of the dependent variable
      P = CppAD::Poly(0, A, Z);

      // Taylor-expand P to degree one
      P.eval(2);

      // second derivative is twice second order Taylor coefficient
      ddp[0] = 2. * P[2];

      // Free DAG corresponding to P does not seem to improve speed.
      // Probably because it gets freed the next time P is assigned.
      // P.reset();
   }
   // ------------------------------------------------------
   return true;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end fadbad_poly.cpp}
*/
