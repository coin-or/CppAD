# ifndef CPPAD_SPEED_ODE_EVALUATE_HPP
# define CPPAD_SPEED_ODE_EVALUATE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin ode_evaluate}
{xrst_spell
   fp
   retaped
}

Evaluate a Function Defined in Terms of an ODE
##############################################

Syntax
******
| # ``include <cppad/speed/ode_evaluate.hpp>``
| ``ode_evaluate`` ( *x* , *p* , *fp* )

Purpose
*******
This routine evaluates a function :math:`f : \B{R}^n \rightarrow \B{R}^n`
defined by

.. math::

   f(x) = y(x, 1)

where :math:`y(x, t)` solves the ordinary differential equation

.. math::
   :nowrap:

   \begin{eqnarray}
      y(x, 0)              & = & x
      \\
      \partial_t y (x, t ) & = & g[ y(x,t) , t ]
   \end{eqnarray}

where :math:`g : \B{R}^n \times \B{R} \rightarrow \B{R}^n`
is an unspecified function.

Inclusion
*********
The template function ``ode_evaluate``
is defined in the ``CppAD`` namespace by including
the file ``cppad/speed/ode_evaluate.hpp``
(relative to the CppAD distribution directory).

Float
*****

Operation Sequence
==================
The type *Float* must be a :ref:`NumericType-name` .
The *Float*
:ref:`operation sequence<glossary@Operation@Sequence>`
for this routine does not depend on the value of the argument *x* ,
hence it does not need to be retaped for each value of :math:`x`.

fabs
====
If *y* and *z* are *Float* objects, the syntax

   *y* = ``fabs`` ( *z* )

must be supported. Note that it does not matter if the operation
sequence for ``fabs`` depends on *z* because the
corresponding results are not actually used by ``ode_evaluate`` ;
see ``fabs`` in :ref:`Runge45<Runge45@Scalar@fabs>` .

x
*
The argument *x* has prototype

   ``const CppAD::vector<`` *Float* >& *x*

It contains he argument value for which the function,
or its derivative, is being evaluated.
The value :math:`n` is determined by the size of the vector *x* .

p
*
The argument *p* has prototype

   ``size_t`` *p*

p == 0
======
In this case a numerical method is used to solve the ode
and obtain an accurate approximation for :math:`y(x, 1)`.
This numerical method has a fixed
that does not depend on *x* .

p = 1
=====
In this case an analytic solution for the partial derivative
:math:`\partial_x y(x, 1)` is returned.

fp
**
The argument *fp* has prototype

   ``CppAD::vector<`` *Float* >& *fp*

The input value of the elements of *fp* does not matter.

Function
========
If *p* is zero, *fp* has size equal to :math:`n`
and contains the value of :math:`y(x, 1)`.

Gradient
========
If *p* is one, *fp* has size equal to *n^2*
and for :math:`i = 0 , \ldots 1`, :math:`j = 0 , \ldots , n-1`

.. math::

   \D{y[i]}{x[j]} (x, 1) = fp [ i \cdot n + j ]

{xrst_toc_hidden
   speed/example/ode_evaluate.cpp
   xrst/ode_evaluate.xrst
}

Example
*******
The file
:ref:`ode_evaluate.cpp-name`
contains an example and test  of ``ode_evaluate.hpp`` .

Source Code
***********
The file
:ref:`ode_evaluate.hpp-name`
contains the source code for this template function.

{xrst_end ode_evaluate}
*/
// BEGIN C++
# include <cppad/utility/vector.hpp>
# include <cppad/utility/ode_err_control.hpp>
# include <cppad/utility/runge_45.hpp>

namespace CppAD {

   template <class Float>
   class ode_evaluate_fun {
   public:
      // Given that y_i (0) = x_i,
      // the following y_i (t) satisfy the ODE below:
      // y_0 (t) = x[0]
      // y_1 (t) = x[1] + x[0] * t
      // y_2 (t) = x[2] + x[1] * t + x[0] * t^2/2
      // y_3 (t) = x[3] + x[2] * t + x[1] * t^2/2 + x[0] * t^3 / 3!
      // ...
      void Ode(
         const Float&                    t,
         const CppAD::vector<Float>&     y,
         CppAD::vector<Float>&           f)
      {  size_t n  = y.size();
         f[0]      = 0.;
         for(size_t k = 1; k < n; k++)
            f[k] = y[k-1];
      }
   };
   //
   template <class Float>
   void ode_evaluate(
      const CppAD::vector<Float>& x  ,
      size_t                      p  ,
      CppAD::vector<Float>&       fp )
   {  using CppAD::vector;
      typedef vector<Float> FloatVector;

      size_t n = x.size();
      CPPAD_ASSERT_KNOWN( p == 0 || p == 1,
         "ode_evaluate: p is not zero or one"
      );
      CPPAD_ASSERT_KNOWN(
         ((p==0) && (fp.size()==n)) || ((p==1) && (fp.size()==n*n)),
         "ode_evaluate: the size of fp is not correct"
      );
      if( p == 0 )
      {  // function that defines the ode
         ode_evaluate_fun<Float> F;

         // number of Runge45 steps to use
         size_t M = 10;

         // initial and final time
         Float ti = 0.0;
         Float tf = 1.0;

         // initial value for y(x, t); i.e. y(x, 0)
         // (is a reference to x)
         const FloatVector& yi = x;

         // final value for y(x, t); i.e., y(x, 1)
         // (is a reference to fp)
         FloatVector& yf = fp;

         // Use fourth order Runge-Kutta to solve ODE
         yf = CppAD::Runge45(F, M, ti, tf, yi);

         return;
      }
      /* Compute derivaitve of y(x, 1) w.r.t x
      y_0 (x, t) = x[0]
      y_1 (x, t) = x[1] + x[0] * t
      y_2 (x, t) = x[2] + x[1] * t + x[0] * t^2/2
      y_3 (x, t) = x[3] + x[2] * t + x[1] * t^2/2 + x[0] * t^3 / 3!
      ...
      */
      size_t i, j, k;
      for(i = 0; i < n; i++)
      {  for(j = 0; j < n; j++)
            fp[ i * n + j ] = 0.0;
      }
      size_t factorial = 1;
      for(k = 0; k < n; k++)
      {  if( k > 1 )
            factorial *= k;
         for(i = k; i < n; i++)
         {  // partial w.r.t x[i-k] of x[i-k] * t^k / k!
            j = i - k;
            fp[ i * n + j ] += 1.0 / Float(factorial);
         }
      }
   }
}
// END C++

# endif
