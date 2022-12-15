// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/utility/vector.hpp>
# include <cppad/speed/det_grad_33.hpp>
# include <cppad/speed/det_33.hpp>
# include <cppad/utility/time_test.hpp>
// BEGIN PROTOTYPE
extern bool link_det_minor(
   const std::string&         job       ,
   size_t                     size      ,
   size_t                     repeat    ,
   CppAD::vector<double>&     matrix    ,
   CppAD::vector<double>&     gradient
);
// END PROTOTYPE
/*
-------------------------------------------------------------------------------
{xrst_begin link_det_minor}

Speed Testing Gradient of Determinant by Minor Expansion
########################################################

Prototype
*********
{xrst_literal
   // BEGIN PROTOTYPE
   // END PROTOTYPE
}

Method
******
The same template class :ref:`det_by_minor-name`
is used by the different AD packages.

job
***
See the standard link specifications for :ref:`link_routines@job` .

size
****
See the standard link specifications for :ref:`link_routines@size`
In addition, *size* is the number of rows and columns in
*matrix* .

repeat
******
See the standard link specifications for :ref:`link_routines@repeat` .

matrix
******
The argument *matrix* is a vector with
*size* * *size* elements.
The input value of its elements does not matter.
The output value of its elements is the last matrix that the
gradient (or determinant) is computed for.

gradient
********
The argument *gradient* is a vector with
*size* * *size* elements.
The input value of its elements does not matter.
The output value of its elements is the gradient of the
determinant of *matrix* with respect to its elements.

double
======
In the case where *package* is ``double`` ,
only the first element of *gradient* is used and it is actually
the determinant value (the gradient value is not computed).

{xrst_end link_det_minor}
-----------------------------------------------------------------------------
*/
// ---------------------------------------------------------------------------
// The routines below are documented in dev_link.omh
// ---------------------------------------------------------------------------
namespace {
   void time_det_minor_callback(size_t size, size_t repeat)
   {  std::string           job("run");
      CppAD::vector<double> matrix(size * size);
      CppAD::vector<double> gradient(size * size);
      //
      link_det_minor(job, size, repeat, matrix, gradient);
      return;
   }
}
// ---------------------------------------------------------------------------
bool available_det_minor(void)
{
   size_t size   = 3;
   size_t repeat = 0;
   CppAD::vector<double> matrix(size * size);
   CppAD::vector<double> gradient(size * size);
   //
   std::string job = "setup";
   bool result = link_det_minor(job, size, repeat, matrix, gradient);
   //
   job = "teardown";
   link_det_minor(job, size, repeat, matrix, gradient);
   //
   return result;
}
// ---------------------------------------------------------------------------
bool correct_det_minor(bool is_package_double)
{
   size_t  size   = 3;
   size_t  repeat = 1;
   CppAD::vector<double> matrix(size * size);
   CppAD::vector<double> gradient(size * size);
   //
   std::string job = "setup";
   link_det_minor(job, size, repeat, matrix, gradient);
   //
   job = "run";
   link_det_minor(job, size, repeat, matrix, gradient);
   //
   bool ok = CppAD::det_grad_33(matrix, gradient);
   if( is_package_double )
      ok = CppAD::det_33(matrix, gradient);
   else
      ok = CppAD::det_grad_33(matrix, gradient);
   //
   job = "teardown";
   link_det_minor(job, size, repeat, matrix, gradient);
   //
   return ok;
}
double time_det_minor(double time_min, size_t size)
{  CPPAD_ASSERT_UNKNOWN( size != 0 );
   //
   CppAD::vector<double> matrix(size * size);
   CppAD::vector<double> gradient(size * size);
   //
   std::string job    = "setup";
   size_t      repeat = 0;
   link_det_minor(job, size, repeat, matrix, gradient);
   //
   // job is run in time_det_minor_callback
   double time = CppAD::time_test(time_det_minor_callback, time_min, size);
   //
   job = "teardown";
   link_det_minor(job, size, repeat, matrix, gradient);
   //
   return time;
}
// ---------------------------------------------------------------------------
