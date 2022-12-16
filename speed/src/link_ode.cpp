// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/utility/vector.hpp>
# include <cppad/speed/ode_evaluate.hpp>
# include <cppad/utility/near_equal.hpp>
# include <cppad/utility/near_equal.hpp>
# include <cppad/utility/time_test.hpp>
// BEGIN PROTOTYPE
extern bool link_ode(
   size_t                     size       ,
   size_t                     repeat     ,
   CppAD::vector<double>&     x          ,
   CppAD::vector<double>&     jacobian
);
// END PROTOTYPE
/*
-------------------------------------------------------------------------------
{xrst_begin link_ode}
{xrst_spell
   fp
}

Speed Testing the Jacobian of Ode Solution
##########################################

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
The same template routine :ref:`ode_evaluate-name` is used
by th different AD packages.

f
*
The function
:math:`f : \B{R}^n \rightarrow \B{R}^n` that is defined and computed by
evaluating :ref:`ode_evaluate-name` with a call of the form

   ``ode_evaluate`` ( *x* , *p* , *fp* )

with *p* equal to zero.
Calls with the value *p* equal to one are used to check
the derivative values.

Return Value
************
If this speed test is not yet
supported by a particular *package* ,
the corresponding return value for ``link_ode``
should be ``false`` .

size
****
The argument *size*
is the number of variables in the ordinary differential equations
which is also equal to :math:`n`.

repeat
******
The argument *repeat* is the number of times the
Jacobian is computed.

x
*
The argument *x* is a vector with :math:`n` elements.
The input value of the elements of *x* does not matter.
On output, it has been set to the
argument value for which the function,
or its derivative, is being evaluated.
The value of this vector must change with each repetition.

jacobian
********
The argument *jacobian* is a vector with :math:`n^2` elements.
The input value of its elements does not matter.
The output value of its elements is the Jacobian of the function :math:`f(x)`
that corresponds to output value of *x* .
To be more specific, for
:math:`i = 0 , \ldots , n-1` and :math:`j = 0 , \ldots , n-1`,

.. math::

   \D{f[i]}{x[j]} (x) = jacobian [ i \cdot n + j ]

double
======
In the case where *package* is ``double`` ,
only the first :math:`n` element of *jacobian*
are modified and they are to the function value
:math:`f(x)` corresponding to the output value of *x* .

{xrst_end link_ode}
-----------------------------------------------------------------------------
*/
// ---------------------------------------------------------------------------
// The routines below are documented in dev_link.omh
// ---------------------------------------------------------------------------
namespace {
   void time_ode_callback(size_t n, size_t repeat)
   {  // free statically allocated memory
      if( n == 0 && repeat == 0 )
         return;
      //
      CppAD::vector<double> x(n);
      CppAD::vector<double> jacobian(n * n);
      link_ode(n, repeat, x, jacobian);
      return;
   }
}
// ---------------------------------------------------------------------------
bool available_ode(void)
{  size_t n      = 1;
   size_t repeat = 1;
   CppAD::vector<double> x(n);
   CppAD::vector<double> jacobian(n * n);

   return link_ode(n, repeat, x, jacobian);
}
// ----------------------------------------------------------------------------
bool correct_ode(bool is_package_double)
{  bool ok       = true;

   size_t n      = 5;
   size_t repeat = 1;
   CppAD::vector<double> x(n);
   CppAD::vector<double> jacobian(n * n);

   link_ode(n, repeat, x, jacobian);

   size_t size = n * n;
   size_t p = 1;
   if( is_package_double )
   {  p    = 0;  // check function value
      size = n;
   }
   CppAD::vector<double> check(size);
   CppAD::ode_evaluate(x, p, check);
   size_t k;
   for(k = 0; k < size; k++)
      ok &= CppAD::NearEqual(check[k], jacobian[k], 1e-6, 1e-6);

   return ok;
}
double time_ode(double time_min, size_t size)
{  return CppAD::time_test(time_ode_callback, time_min, size);
}
// ----------------------------------------------------------------------------
