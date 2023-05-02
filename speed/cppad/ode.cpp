// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin cppad_ode.cpp}

Cppad Speed: Gradient of Ode Solution
#####################################

Specifications
**************
See :ref:`link_ode-name` .

Implementation
**************

{xrst_spell_off}
{xrst_code cpp} */
# include <cppad/cppad.hpp>
# include <cppad/speed/ode_evaluate.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cassert>

// Note that CppAD uses global_option["memory"] at the main program level
# include <map>
extern std::map<std::string, bool> global_option;
// see comments in main program for this external
extern size_t global_cppad_thread_alloc_inuse;

bool link_ode(
   size_t                     size       ,
   size_t                     repeat     ,
   CppAD::vector<double>      &x         ,
   CppAD::vector<double>      &jacobian
)
{  global_cppad_thread_alloc_inuse = 0;

   // --------------------------------------------------------------------
   // check global options
   const char* valid[] = { "memory", "onetape", "optimize", "val_graph"};
   size_t n_valid = sizeof(valid) / sizeof(valid[0]);
   typedef std::map<std::string, bool>::iterator iterator;
   //
   for(iterator itr=global_option.begin(); itr!=global_option.end(); ++itr)
   {  if( itr->second )
      {  bool ok = false;
         for(size_t i = 0; i < n_valid; i++)
            ok |= itr->first == valid[i];
         if( ! ok )
            return false;
      }
   }
   // --------------------------------------------------------------------
   // optimization options: no conditional skips or compare operators
   std::string optimize_options =
      "no_conditional_skip no_compare_op no_print_for_op";
   if( global_option["val_graph"] )
      optimize_options += " val_graph";
   // --------------------------------------------------------------------
   // setup
   assert( x.size() == size );
   assert( jacobian.size() == size * size );

   typedef CppAD::AD<double>       ADScalar;
   typedef CppAD::vector<ADScalar> ADVector;

   size_t j;
   size_t p = 0;              // use ode to calculate function values
   size_t n = size;           // number of independent variables
   size_t m = n;              // number of dependent variables
   ADVector  X(n), Y(m);      // independent and dependent variables
   CppAD::ADFun<double>  f;   // AD function

   // do not even record comparison operators
   size_t abort_op_index = 0;
   bool record_compare   = false;

   // -------------------------------------------------------------
   if( ! global_option["onetape"] ) while(repeat--)
   {  // choose next x value
      uniform_01(n, x);
      for(j = 0; j < n; j++)
         X[j] = x[j];

      // declare independent variables
      Independent(X, abort_op_index, record_compare);

      // evaluate function
      CppAD::ode_evaluate(X, p, Y);

      // create function object f : X -> Y
      f.Dependent(X, Y);

      if( global_option["optimize"] )
         f.optimize(optimize_options);

      // skip comparison operators
      f.compare_change_count(0);

      jacobian = f.Jacobian(x);
   }
   else
   {  // an x value
      uniform_01(n, x);
      for(j = 0; j < n; j++)
         X[j] = x[j];

      // declare the independent variable vector
      Independent(X, abort_op_index, record_compare);

      // evaluate function
      CppAD::ode_evaluate(X, p, Y);

      // create function object f : X -> Y
      f.Dependent(X, Y);

      if( global_option["optimize"] )
         f.optimize(optimize_options);

      // skip comparison operators
      f.compare_change_count(0);

      while(repeat--)
      {  // get next argument value
         uniform_01(n, x);

         // evaluate jacobian
         jacobian = f.Jacobian(x);
      }
   }
   size_t thread                   = CppAD::thread_alloc::thread_num();
   global_cppad_thread_alloc_inuse = CppAD::thread_alloc::inuse(thread);
   return true;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end cppad_ode.cpp}
*/
