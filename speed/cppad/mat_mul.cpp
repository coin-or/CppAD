// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin cppad_mat_mul.cpp}

CppAD Speed, Matrix Multiplication
##################################

Specifications
**************
See :ref:`link_mat_mul-name` .

Implementation
**************

{xrst_spell_off}
{xrst_code cpp} */
# include <cppad/cppad.hpp>
# include <cppad/speed/mat_sum_sq.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/example/atomic_four/mat_mul/mat_mul.hpp>

// Note that CppAD uses global_option["memory"] at the main program level
# include <map>
extern std::map<std::string, bool> global_option;
// see comments in main program for this external
extern size_t global_cppad_thread_alloc_inuse;

bool link_mat_mul(
   size_t                           size     ,
   size_t                           repeat   ,
   CppAD::vector<double>&           x        ,
   CppAD::vector<double>&           z        ,
   CppAD::vector<double>&           dz
)
{  global_cppad_thread_alloc_inuse = 0;

   // --------------------------------------------------------------------
   // check global options
   const char* valid[] = {
      "memory", "onetape", "optimize", "atomic", "val_graph"
   };
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
   // -----------------------------------------------------
   // setup
   typedef CppAD::AD<double>           ADScalar;
   typedef CppAD::vector<ADScalar>     ADVector;

   size_t j;               // temporary index
   size_t m = 1;           // number of dependent variables
   size_t n = size * size; // number of independent variables
   ADVector   X(n);        // AD domain space vector
   ADVector   Y(n);        // Store product matrix
   ADVector   Z(m);        // AD range space vector
   CppAD::ADFun<double> f; // AD function object

   // vectors of reverse mode weights
   CppAD::vector<double> w(1);
   w[0] = 1.;

   // atomic function information
   CppAD::vector<ADScalar> ax(2 * n), ay(n);
   CppAD::atomic_mat_mul<double> atom_mul("atom_mul");
   //
   // do not even record comparison operators
   size_t abort_op_index = 0;
   bool record_compare   = false;

   // ------------------------------------------------------
   if( ! global_option["onetape"] ) while(repeat--)
   {  // get the next matrix
      CppAD::uniform_01(n, x);
      for( j = 0; j < n; j++)
         X[j] = x[j];

      // declare independent variables
      Independent(X, abort_op_index, record_compare);

      // do computations
      if( ! global_option["atomic"] )
         mat_sum_sq(size, X, Y, Z);
      else
      {  for(j = 0; j < n; j++)
         {  ax[j]     = X[j];
            ax[n + j] = X[j];
         }
         // Y = X * X
         size_t call_id = atom_mul.set(size, size, size);
         atom_mul(call_id, ax, ay);
         Z[0] = 0.;
         for(j = 0; j < n; j++)
            Z[0] += ay[j];
      }
      // create function object f : X -> Z
      f.Dependent(X, Z);

      if( global_option["optimize"] )
         f.optimize(optimize_options);

      // skip comparison operators
      f.compare_change_count(0);

      // evaluate and return gradient using reverse mode
      z  = f.Forward(0, x);
      dz = f.Reverse(1, w);
   }
   else
   {  // get a next matrix
      CppAD::uniform_01(n, x);
      for(j = 0; j < n; j++)
         X[j] = x[j];

      // declare independent variables
      Independent(X, abort_op_index, record_compare);

      // do computations
      if( ! global_option["atomic"] )
         mat_sum_sq(size, X, Y, Z);
      else
      {  for(j = 0; j < n; j++)
         {  ax[j]   = X[j];
            ax[j+n] = X[j];
         }
         // Y = X * X
         atom_mul(ax, ay);
         Z[0] = 0.;
         for(j = 0; j < n; j++)
            Z[0] += ay[j];
      }

      // create function object f : X -> Z
      f.Dependent(X, Z);

      if( global_option["optimize"] )
         f.optimize(optimize_options);

      // skip comparison operators
      f.compare_change_count(0);

      while(repeat--)
      {  // get a next matrix
         CppAD::uniform_01(n, x);

         // evaluate and return gradient using reverse mode
         z  = f.Forward(0, x);
         dz = f.Reverse(1, w);
      }
   }
   size_t thread                   = CppAD::thread_alloc::thread_num();
   global_cppad_thread_alloc_inuse = CppAD::thread_alloc::inuse(thread);
   // --------------------------------------------------------------------
   // Free temporary work space (any future atomic_mat_mul constructors
   // would create new temporary work space.)
   CppAD::user_atomic<double>::clear();
   // --------------------------------------------------------------------

   return true;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end cppad_mat_mul.cpp}
*/
