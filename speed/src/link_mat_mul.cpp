// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/utility/vector.hpp>
# include <cppad/utility/near_equal.hpp>
# include <cppad/utility/time_test.hpp>
// BEGIN PROTOTYPE
extern bool link_mat_mul(
   size_t                     size     ,
   size_t                     repeat   ,
   CppAD::vector<double>&     x        ,
   CppAD::vector<double>&     z        ,
   CppAD::vector<double>&     dz
);
// END PROTOTYPE
/*
-------------------------------------------------------------------------------
{xrst_begin link_mat_mul}
{xrst_spell
   dz
}

Speed Testing Derivative of Matrix Multiply
###########################################

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

Return Value
************
If this speed test is not yet
supported by a particular *package* ,
the corresponding return value for ``link_mat_mul``
should be ``false`` .

n
*
The argument *n* is the number of rows and columns
in the square matrix *x* .

repeat
******
The argument *repeat* is the number of different argument values
that the derivative of *z* (or just the value of *z* )
will be computed.

x
*
The argument *x* is a vector with
*x* . ``size`` () = *size* * *size* elements.
The input value of its elements does not matter.
The output value of its elements is the last random matrix
that is multiplied and then summed to form *z* ;

.. math::

   x_{i,j} = x[ i * s + j ]

where *s* = *size* .

z
*
The argument *z* is a vector with one element.
The input value of the element does not matter.
The output of its element the sum of the elements of
*y* = *x* * *x* ; i.e.,

.. math::
   :nowrap:

   \begin{eqnarray}
      y_{i,j} & = & \sum_{k=0}^{s-1} x_{i,k} x_{k, j}
      \\
      z       & = & \sum_{i=0}^{s-1} \sum_{j=0}^{s-1} y_{i,j}
   \end{eqnarray}

dz
**
The argument *dz* is a vector with
*dz* . ``size`` () = *size* * *size* .
The input values of its elements do not matter.
The output value of its elements form the
derivative of *z* with respect to *x* .

{xrst_end link_mat_mul}
-----------------------------------------------------------------------------
*/
// ---------------------------------------------------------------------------
// The routines below are documented in dev_link.omh
// ---------------------------------------------------------------------------
namespace {
   void time_mat_mul_callback(size_t size, size_t repeat)
   {  // free statically allocated memory
      if( size == 0 && repeat == 0 )
         return;
      //
      CppAD::vector<double>  x(size * size), z(1), dz(size * size);

      link_mat_mul(size, repeat, x, z, dz);
      return;
   }
}
// ---------------------------------------------------------------------------
bool available_mat_mul(void)
{  size_t size   = 3;
   size_t repeat = 1;
   CppAD::vector<double>  x(size * size), z(1), dz(size * size);

   return link_mat_mul(size, repeat, x, z, dz);
}
// --------------------------------------------------------------------------
bool correct_mat_mul(bool is_package_double)
{  size_t size   = 2;
   size_t repeat = 1;
   CppAD::vector<double>  x(size * size), z(1), dz(size * size);
   double eps99 = 99.0 * std::numeric_limits<double>::epsilon();

   link_mat_mul(size, repeat, x, z, dz);

   double x00 = x[0 * size + 0];
   double x01 = x[0 * size + 1];
   double x10 = x[1 * size + 0];
   double x11 = x[1 * size + 1];
   bool ok = true;
   double check;
   if( is_package_double )
   {  check  = 0;
      check += x00 * x00 + x01 * x10; // y00
      check += x00 * x01 + x01 * x11; // y01
      check += x10 * x00 + x11 * x10; // y10
      check += x10 * x01 + x11 * x11; // y11
      ok &= CppAD::NearEqual(check, z[0], eps99, eps99);
      return ok;
   }
   // partial w.r.t. x00
   check = x00 + x00 + x01 + x10;
   ok   &= CppAD::NearEqual(check, dz[0 * size + 0], eps99, eps99);
   // partial w.r.t. x01
   check = x10 + x00 + x11 + x10;
   ok   &= CppAD::NearEqual(check, dz[0 * size + 1], eps99, eps99);
   // partial w.r.t. x10
   check = x01 + x00 + x11 + x01;
   ok   &= CppAD::NearEqual(check, dz[1 * size + 0], eps99, eps99);
   // partial w.r.t. x11
   check = x01 + x10 + x11 + x11;
   ok   &= CppAD::NearEqual(check, dz[1 * size + 1], eps99, eps99);

   return ok;
}
double time_mat_mul(double time_min, size_t size)
{  return CppAD::time_test(time_mat_mul_callback, time_min, size);
}
// --------------------------------------------------------------------------
