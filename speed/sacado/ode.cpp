// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin sacado_ode.cpp}

Sacado Speed: Gradient of Ode Solution
######################################

Specifications
**************
See :ref:`link_ode-name` .

Implementation
**************

{xrst_spell_off}
{xrst_code cpp} */
// suppress conversion warnings before other includes
# include <cppad/wno_conversion.hpp>
//
# include <Sacado.hpp>
# include <cassert>
# include <cppad/utility/vector.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/speed/ode_evaluate.hpp>

// list of possible options
# include <map>
extern std::map<std::string, bool> global_option;

bool link_ode(
   size_t                     size       ,
   size_t                     repeat     ,
   CppAD::vector<double>      &x         ,
   CppAD::vector<double>      &jacobian
)
{
   // speed test global option values
   if( global_option["atomic"] )
      return false;
   if( global_option["memory"] || global_option["onetape"] || global_option["optimize"] )
      return false;
   // -------------------------------------------------------------
   // setup
   assert( x.size() == size );
   assert( jacobian.size() == size * size );

   typedef Sacado::Fad::DFad<double>  ADScalar;
   typedef CppAD::vector<ADScalar>    ADVector;

   size_t i, j;
   size_t p = 0;          // use ode to calculate function values
   size_t n = size;       // number of independent variables
   size_t m = n;          // number of dependent variables
   ADVector X(n), Y(m);   // independent and dependent variables

   // -------------------------------------------------------------
   while(repeat--)
   {  // choose next x value
      CppAD::uniform_01(n, x);
      for(j = 0; j < n; j++)
      {  // set up for X as the independent variable vector
         X[j] = ADScalar(int(n), int(j), x[j]);
      }

      // evaluate function
      CppAD::ode_evaluate(X, p, Y);

      // return values with Y as the dependent variable vector
      for(i = 0; i < m; i++)
      {  for(j = 0; j < n; j++)
            jacobian[ i * n + j ] = Y[i].dx(j);
      }
   }
   return true;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end sacado_ode.cpp}
*/
