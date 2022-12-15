// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/utility/vector.hpp>
# include <cppad/utility/poly.hpp>
# include <cppad/utility/near_equal.hpp>
# include <cppad/utility/time_test.hpp>
// BEGIN PROTOTYPE
extern bool link_poly(
   size_t                     size     ,
   size_t                     repeat   ,
   CppAD::vector<double>&     a        ,
   CppAD::vector<double>&     z        ,
   CppAD::vector<double>&     ddp
);
// END PROTOTYPE
/*
-------------------------------------------------------------------------------
{xrst_begin link_poly}
{xrst_spell
   ddp
}

Speed Testing Second Derivative of a Polynomial
###############################################

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
The same template routine :ref:`Poly-name` is used
by the different AD packages.

Return Value
************
If this speed test is not yet
supported by a particular *package* ,
the corresponding return value for ``link_poly``
should be ``false`` .

size
****
The argument *size* is the order of the polynomial
(the number of coefficients in the polynomial).

repeat
******
The argument *repeat* is the number of different argument values
that the second derivative (or just the polynomial) will be computed at.

a
*
The argument *a* is a vector with *size* elements.
The input value of its elements does not matter.
The output value of its elements is the coefficients of the
polynomial that is differentiated
(*i*-th element is coefficient of order *i* ).

z
*
The argument *z* is a vector with one element.
The input value of the element does not matter.
The output of its element is the polynomial argument value
were the last second derivative (or polynomial value) was computed.

ddp
***
The argument *ddp* is a vector with one element.
The input value of its element does not matter.
The output value of its element is the
second derivative of the polynomial with respect to it's argument value.

double
======
In the case where *package* is ``double`` ,
the output value of the element of *ddp*
is the polynomial value (the second derivative is not computed).

{xrst_end link_poly}
-----------------------------------------------------------------------------
*/
// ---------------------------------------------------------------------------
// The routines below are documented in dev_link.omh
// ---------------------------------------------------------------------------
namespace {
void time_poly_callback(size_t size, size_t repeat)
   {  // free statically allocated memory
      if( size == 0 && repeat == 0 )
         return;
      //
      CppAD::vector<double>  a(size), z(1), ddp(1);

      link_poly(size, repeat, a, z, ddp);
      return;
   }
}
// ---------------------------------------------------------------------------
bool available_poly(void)
{  size_t size   = 10;
   size_t repeat = 1;
   CppAD::vector<double>  a(size), z(1), ddp(1);

   return link_poly(size, repeat, a, z, ddp);
}
// ---------------------------------------------------------------------------
bool correct_poly(bool is_package_double)
{  size_t size   = 10;
   size_t repeat = 1;
   CppAD::vector<double>  a(size), z(1), ddp(1);
   double eps99 = 99.0 * std::numeric_limits<double>::epsilon();

   link_poly(size, repeat, a, z, ddp);

   size_t k;
   double check;
   if( is_package_double )
      k = 0;
   else
      k = 2;
   check = CppAD::Poly(k, a, z[0]);

   bool ok = CppAD::NearEqual(check, ddp[0], eps99, eps99);
   return ok;
}
// ---------------------------------------------------------------------------
double time_poly(double time_min, size_t size)
{  return CppAD::time_test(time_poly_callback, time_min, size);
}
// ---------------------------------------------------------------------------
