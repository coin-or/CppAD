// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin cppad_det_lu.cpp}

Cppad Speed: Gradient of Determinant Using Lu Factorization
###########################################################

Specifications
**************
See :ref:`link_det_lu-name` .

Implementation
**************
{xrst_spell_off}
{xrst_code cpp} */
# include <cppad/speed/det_by_lu.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/cppad.hpp>

// Note that CppAD uses global_option["memory"] at the main program level
# include <map>
extern std::map<std::string, bool> global_option;
// see comments in main program for this external
extern size_t global_cppad_thread_alloc_inuse;

bool link_det_lu(
   size_t                           size     ,
   size_t                           repeat   ,
   CppAD::vector<double>           &matrix   ,
   CppAD::vector<double>           &gradient )
{  global_cppad_thread_alloc_inuse = 0;

   // --------------------------------------------------------------------
   // check global options
   const char* valid[] = { "memory", "optimize", "val_graph"};
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
   // optimization options:
   std::string optimize_options =
      "no_conditional_skip no_compare_op no_print_for_op";
   if( global_option["val_graph"] )
      optimize_options += " val_graph";
   // -----------------------------------------------------
   // setup
   typedef CppAD::AD<double>           ADScalar;
   typedef CppAD::vector<ADScalar>     ADVector;
   CppAD::det_by_lu<ADScalar>          Det(size);

   size_t i;               // temporary index
   size_t m = 1;           // number of dependent variables
   size_t n = size * size; // number of independent variables
   ADVector   A(n);        // AD domain space vector
   ADVector   detA(m);     // AD range space vector
   CppAD::ADFun<double> f; // AD function object

   // vectors of reverse mode weights
   CppAD::vector<double> w(1);
   w[0] = 1.;

   // do not even record comparison operators
   size_t abort_op_index = 0;
   bool record_compare   = false;

   // ------------------------------------------------------
   while(repeat--)
   {  // get the next matrix
      CppAD::uniform_01(n, matrix);
      for( i = 0; i < n; i++)
         A[i] = matrix[i];

      // declare independent variables
      Independent(A, abort_op_index, record_compare);

      // AD computation of the determinant
      detA[0] = Det(A);

      // create function object f : A -> detA
      f.Dependent(A, detA);
      if( global_option["optimize"] )
         f.optimize(optimize_options);

      // evaluate and return gradient using reverse mode
      f.Forward(0, matrix);
      gradient = f.Reverse(1, w);
   }
   size_t thread                   = CppAD::thread_alloc::thread_num();
   global_cppad_thread_alloc_inuse = CppAD::thread_alloc::inuse(thread);
   return true;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end cppad_det_lu.cpp}
*/
