// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/utility/vector.hpp>
# include <cppad/speed/det_grad_33.hpp>
# include <cppad/speed/det_33.hpp>
# include <cppad/utility/time_test.hpp>

// BEGIN PROTOTYPE
extern bool link_det_lu(
   size_t                     size      ,
   size_t                     repeat    ,
   CppAD::vector<double>&     matrix    ,
   CppAD::vector<double>&     gradient
);
// END PROTOTYPE
/*
-------------------------------------------------------------------------------
{xrst_begin link_det_lu}

Speed Testing Gradient of Determinant Using Lu Factorization
############################################################

Prototype
*********
{xrst_literal
   // BEGIN PROTOTYPE
   // END PROTOTYPE
}

Purpose
*******
Each :ref:`speed_main@package`
must define a version of this routine as specified below.
This is used by the :ref:`speed_main-name` program
to run the corresponding speed and correctness tests.

Method
******
The same template routine :ref:`det_by_lu-name` is used
by the different AD packages.

Return Value
************
If this speed test is not yet
supported by a particular *package* ,
the corresponding return value for ``link_det_lu``
should be ``false`` .

size
****
The argument *size*
is the number of rows and columns in the matrix.

repeat
******
The argument *repeat* is the number of different matrices
that the gradient (or determinant) is computed for.

matrix
******
The argument *matrix* is a vector with *size* * *size* elements.
The input value of its elements does not matter.
The output value of its elements is the last matrix that the
gradient (or determinant) is computed for.

gradient
********
The argument *gradient* is a vector with *size* * *size* elements.
The input value of its elements does not matter.
The output value of its elements is the gradient of the
determinant of *matrix* with respect to its elements.

double
======
In the case where *package* is ``double`` ,
only the first element of *gradient* is used and it is actually
the determinant value (the gradient value is not computed).

{xrst_end link_det_lu}
-----------------------------------------------------------------------------
*/
// ---------------------------------------------------------------------------
// The routines below are documented in dev_link.omh
// ---------------------------------------------------------------------------
namespace {
   void time_det_lu_callback(size_t size, size_t repeat)
   {  // free statically allocated memory
      if( size == 0 && repeat == 0 )
         return;
      //
      CppAD::vector<double> matrix(size * size);
      CppAD::vector<double> gradient(size * size);

      link_det_lu(size, repeat, matrix, gradient);
      return;
   }
}
// ---------------------------------------------------------------------------
bool available_det_lu(void)
{  size_t size   = 3;
   size_t repeat = 1;
   CppAD::vector<double> matrix(size * size);
   CppAD::vector<double> gradient(size * size);

   return link_det_lu(size, repeat, matrix, gradient);
}
// ---------------------------------------------------------------------------
bool correct_det_lu(bool is_package_double)
{  size_t size   = 3;
   size_t repeat = 1;
   CppAD::vector<double> matrix(size * size);
   CppAD::vector<double> gradient(size * size);

   link_det_lu(size, repeat, matrix, gradient);
   bool ok;
   if( is_package_double )
      ok = CppAD::det_33(matrix, gradient);
   else
      ok = CppAD::det_grad_33(matrix, gradient);
   return ok;
}
// ---------------------------------------------------------------------------
double time_det_lu(double time_min, size_t size)
{  return CppAD::time_test(time_det_lu_callback, time_min, size);
}
