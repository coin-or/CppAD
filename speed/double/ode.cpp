// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin double_ode.cpp}

Double Speed: Ode Solution
##########################

Specifications
**************
See :ref:`link_ode-name` .

Implementation
**************

{xrst_spell_off}
{xrst_code cpp} */
# include <cstring>
# include <cppad/utility/vector.hpp>
# include <cppad/speed/ode_evaluate.hpp>
# include <cppad/speed/uniform_01.hpp>

// Note that CppAD uses global_option["memory"] at the main program level
# include <map>
extern std::map<std::string, bool> global_option;

bool link_ode(
   size_t                     size       ,
   size_t                     repeat     ,
   CppAD::vector<double>      &x         ,
   CppAD::vector<double>      &jacobian
)
{
   if(global_option["onetape"]||global_option["atomic"]||global_option["optimize"])
      return false;
   // -------------------------------------------------------------
   // setup
   assert( x.size() == size );

   size_t n = size;

   size_t m = 0;
   CppAD::vector<double> f(n);

   while(repeat--)
   {  // choose next x value
      uniform_01(n, x);

      // evaluate function
      CppAD::ode_evaluate(x, m, f);

   }
   size_t i;
   for(i = 0; i < n; i++)
      jacobian[i] = f[i];
   return true;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end double_ode.cpp}
*/
