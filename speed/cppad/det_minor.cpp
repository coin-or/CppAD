// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin cppad_det_minor.cpp}

Cppad Speed: Gradient of Determinant by Minor Expansion
#######################################################

Specifications
**************
See :ref:`link_det_minor-name` .

Implementation
**************
{xrst_spell_off}
{xrst_code cpp} */
# include <cppad/cppad.hpp>
# include <cppad/speed/det_by_minor.hpp>
# include <cppad/speed/uniform_01.hpp>

// Note that CppAD uses global_option["memory"] at the main program level
# include <map>
extern std::map<std::string, bool> global_option;
// see comments in main program for this external
extern size_t global_cppad_thread_alloc_inuse;

namespace {
   // typedefs
   typedef CppAD::AD<double>       a_double;
   typedef CppAD::vector<a_double> a_vector;
   //
   // setup
   void setup(
      // inputs
      size_t size             ,
      // outputs
      CppAD::ADFun<double>& f )
   {
      // object for computing determinant
      CppAD::det_by_minor<a_double>   a_det(size);
      //
      // number of independent variables
      size_t nx = size * size;
      //
      // choose a matrix
      CppAD::vector<double> matrix(nx);
      CppAD::uniform_01(nx, matrix);
      //
      // copy to independent variables
      a_vector   a_A(nx);
      for(size_t j = 0; j < nx; ++j)
         a_A[j] = matrix[j];
      //
      // declare independent variables for function computation
      bool record_compare   = false;
      size_t abort_op_index = 0;
      CppAD::Independent(a_A, abort_op_index, record_compare);
      //
      // AD computation of the determinant
      a_vector a_detA(1);
      a_detA[0] = a_det(a_A);
      //
      // f : A -> detA
      f.Dependent(a_A, a_detA);
      //
      // optimize
      std::string optimize_options =
      "no_conditional_skip no_compare_op no_print_for_op no_cumulative_sum_op";
      if( global_option["val_graph"] )
         optimize_options += " val_graph";
      if( global_option["optimize"] )
         f.optimize(optimize_options);
   }

}

bool link_det_minor(
   const std::string&         job      ,
   size_t                     size     ,
   size_t                     repeat   ,
   CppAD::vector<double>     &matrix   ,
   CppAD::vector<double>     &gradient )
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
   // ---------------------------------------------------------------------
   //
   // AD function mapping matrix to determinant
   static CppAD::ADFun<double> static_f;
   //
   // size corresponding to static_f
   static size_t static_size = 0;
   //
   // number of independent variables
   size_t nx = size * size;
   //
   // vectors of reverse mode weights
   CppAD::vector<double> w(1);
   w[0] = 1.;
   //
   // onetape
   bool onetape = global_option["onetape"];
   // -----------------------------------------------------------------------
   if( job == "setup" )
   {  if( onetape )
      {  setup(size, static_f);
         static_size = size;
      }
      else
      {  static_size = 0;
      }
      return true;
   }
   if( job ==  "teardown" )
   {  static_f = CppAD::ADFun<double>();
      return true;
   }
   // -----------------------------------------------------------------------
   CPPAD_ASSERT_UNKNOWN( job == "run" );
   while(repeat--)
   {  if( onetape )
      {  // use if before assert to avoid warning
         if( size != static_size )
         {  CPPAD_ASSERT_UNKNOWN( size == static_size );
         }
      }
      else
      {  setup(size, static_f);
      }
      // get next matrix
      CppAD::uniform_01(nx, matrix);

      // evaluate the gradient
      static_f.Forward(0, matrix);
      gradient = static_f.Reverse(1, w);
   }
   size_t thread                   = CppAD::thread_alloc::thread_num();
   global_cppad_thread_alloc_inuse = CppAD::thread_alloc::inuse(thread);
   return true;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end cppad_det_minor.cpp}
*/
