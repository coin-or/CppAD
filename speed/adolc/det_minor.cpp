// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin adolc_det_minor.cpp}

Adolc Speed: Gradient of Determinant by Minor Expansion
#######################################################

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
# include <adolc/adolc.h>
# include <cppad/utility/vector.hpp>
# include <cppad/speed/det_by_minor.hpp>
# include <cppad/speed/uniform_01.hpp>

// list of possible options
# include <map>
extern std::map<std::string, bool> global_option;

namespace {
   void setup(int tag, size_t size, const CppAD::vector<double>& matrix)
   {  // number of independent variables
      int n = size * size;

      // object for computing determinant
      CppAD::det_by_minor<adouble> a_det(size);

      // declare independent variables
      int keep = 1; // keep forward mode results
      trace_on(tag, keep);
      CppAD::vector<adouble> a_A(n);
      for(int j = 0; j < n; ++j)
         a_A[j] <<= matrix[j];

      // AD computation of the determinant
      adouble a_detA = a_det(a_A);

      // create function object f : A -> detA
      double f;
      a_detA >>= f;
      trace_off();
   }
}

bool link_det_minor(
   const std::string&         job      ,
   size_t                     size     ,
   size_t                     repeat   ,
   CppAD::vector<double>     &matrix   ,
   CppAD::vector<double>     &gradient )
{
   // --------------------------------------------------------------------
   // check global options
   // Allow colpack true even though it is not used below because it is
   // true durng the adolc correctness tests.
   const char* valid[] = { "onetape", "optimize", "colpack"};
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
   // -----------------------------------------------------
   // size corresponding to current tape
   static size_t static_size = 0;
   //
   // number of independent variables
   int n = size * size;
   //
   // tape identifier
   int tag  = 0;
   //
   bool onetape = global_option["onetape"];
   // ----------------------------------------------------------------------
   if( job == "setup" )
   {  if( onetape )
      {  // get a matrix
         CppAD::uniform_01(size_t(n), matrix);
         //
         // recrod the tape
         setup(tag, size, matrix);
         static_size = size;
      }
      else
      {  static_size = 0;
      }
      return true;
   }
   if( job == "teardown" )
   {  // 2DO: How does one free an adolc tape ?
      return true;
   }
   // ----------------------------------------------------------------------
   CPPAD_ASSERT_UNKNOWN( job == "run" );
   //
   // number of dependent variables
   int m    = 1;
   //
   // vectors of reverse mode weights
   CppAD::vector<double> u(m);
   u[0] = 1.;
   //
   if( onetape ) while(repeat--)
   {  if( size != static_size )
      {  CPPAD_ASSERT_UNKNOWN( size == static_size );
      }

      // choose a matrix
      CppAD::uniform_01(n, matrix);

      // evaluate the determinant at the new matrix value
      int keep = 1; // keep this forward mode result
      double f;     // function result
      zos_forward(tag, m, n, keep, matrix.data(), &f);

      // evaluate and return gradient using reverse mode
      fos_reverse(tag, m, n, u.data(), gradient.data());
   }
   else while(repeat--)
   {
      // choose a matrix
      CppAD::uniform_01(n, matrix);

      // record the tape
      setup(tag, size, matrix);

      // evaluate and return gradient using reverse mode
      fos_reverse(tag, m, n, u.data(), gradient.data());
   }
   // --------------------------------------------------------------------
   return true;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end adolc_det_minor.cpp}
*/
