# ifndef CPPAD_UTILITY_RUNGE_45_HPP
# define CPPAD_UTILITY_RUNGE_45_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin Runge45}
{xrst_spell
   karp
   kutta
   tf
   xf
}

An Embedded 4th and 5th Order Runge-Kutta ODE Solver
####################################################

Syntax
******
| # ``include <cppad/utility/runge_45.hpp>``
| *xf* = ``Runge45`` ( *F* , *M* , *ti* , *tf* , *xi* )
| *xf* = ``Runge45`` ( *F* , *M* , *ti* , *tf* , *xi* , *e* )

Purpose
*******
This is an implementation of the
Cash-Karp embedded 4th and 5th order Runge-Kutta ODE solver
described in Section 16.2 of :ref:`Bib@Numerical Recipes` .
We use :math:`n` for the size of the vector *xi* .
Let :math:`\B{R}` denote the real numbers
and let :math:`F : \B{R} \times \B{R}^n \rightarrow \B{R}^n`
be a smooth function.
The return value *xf* contains a 5th order
approximation for the value :math:`X(tf)` where
:math:`X : [ti , tf] \rightarrow \B{R}^n` is defined by
the following initial value problem:

.. math::
   :nowrap:

   \begin{eqnarray}
      X(ti)  & = & xi    \\
      X'(t)  & = & F[t , X(t)]
   \end{eqnarray}

If your set of ordinary differential equations
are stiff, an implicit method may be better
(perhaps :ref:`Rosen34-name` .)

Operation Sequence
******************
The :ref:`operation sequence<glossary@Operation@Sequence>` for *Runge*
does not depend on any of its *Scalar* input values provided that
the operation sequence for

   *F* . ``Ode`` ( *t* , *x* , *f* )

does not on any of its *Scalar* inputs (see below).

Include
*******
The file ``cppad/utility/runge_45.hpp``
is included by ``cppad/cppad.hpp``
but it can also be included separately with out the rest of
the ``CppAD`` routines.

xf
**
The return value *xf* has the prototype

   *Vector* *xf*

and the size of *xf* is equal to *n*
(see description of :ref:`Runge45@Vector` below).

.. math::

   X(tf) = xf + O( h^6 )

where :math:`h = (tf - ti) / M` is the step size.
If *xf* contains not a number :ref:`nan-name` ,
see the discussion for :ref:`Runge45@Fun@f` .

Fun
***
The class *Fun*
and the object *F* satisfy the prototype

   *Fun* & *F*

The object *F* (and the class *Fun* )
must have a member function named ``Ode``
that supports the syntax

   *F* . ``Ode`` ( *t* , *x* , *f* )

t
=
The argument *t* to *F* . ``Ode`` has prototype

   ``const`` *Scalar* & *t*

(see description of :ref:`Runge45@Scalar` below).

x
=
The argument *x* to *F* . ``Ode`` has prototype

   ``const`` *Vector* & *x*

and has size *n*
(see description of :ref:`Runge45@Vector` below).

f
=
The argument *f* to *F* . ``Ode`` has prototype

   *Vector* & *f*

On input and output, *f* is a vector of size *n*
and the input values of the elements of *f* do not matter.
On output,
*f* is set equal to :math:`F(t, x)` in the differential equation.
If any of the elements of *f* have the value not a number ``nan``
the routine ``Runge45`` returns with all the
elements of *xf* and *e* equal to ``nan`` .

Warning
=======
The argument *f* to *F* . ``Ode``
must have a call by reference in its prototype; i.e.,
do not forget the ``&`` in the prototype for *f* .

M
*
The argument *M* has prototype

   ``size_t`` *M*

It specifies the number of steps
to use when solving the differential equation.
This must be greater than or equal one.
The step size is given by :math:`h = (tf - ti) / M`, thus
the larger *M* , the more accurate the
return value *xf* is as an approximation
for :math:`X(tf)`.

ti
**
The argument *ti* has prototype

   ``const`` *Scalar* & *ti*

(see description of :ref:`Runge45@Scalar` below).
It specifies the initial time for *t* in the
differential equation; i.e.,
the time corresponding to the value *xi* .

tf
**
The argument *tf* has prototype

   ``const`` *Scalar* & *tf*

It specifies the final time for *t* in the
differential equation; i.e.,
the time corresponding to the value *xf* .

xi
**
The argument *xi* has the prototype

   ``const`` *Vector* & *xi*

and the size of *xi* is equal to *n* .
It specifies the value of :math:`X(ti)`

e
*
The argument *e* is optional and has the prototype

   *Vector* & *e*

If *e* is present,
the size of *e* must be equal to *n* .
The input value of the elements of *e* does not matter.
On output
it contains an element by element
estimated bound for the absolute value of the error in *xf*

.. math::

   e = O( h^5 )

where :math:`h = (tf - ti) / M` is the step size.
If on output, *e* contains not a number ``nan`` ,
see the discussion for :ref:`Runge45@Fun@f` .

Scalar
******
The type *Scalar* must satisfy the conditions
for a :ref:`NumericType-name` .
The routine :ref:`CheckNumericType-name` will generate an error message
if this is not the case.

fabs
====
In addition, the following function must be defined for
*Scalar* objects *a* and *b*

   *a* = ``fabs`` ( *b* )

Note that this operation is only used for computing *e* ; hence
the operation sequence for *xf* can still be independent of
the arguments to ``Runge45`` even if

   ``fabs`` ( *b* ) = ``std::max`` ( ``-`` *b* , *b* )

.

Vector
******
The type *Vector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type Scalar<SimpleVector@Elements of Specified Type>` .
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.

Parallel Mode
*************
For each set of types
:ref:`Runge45@Scalar` ,
:ref:`Runge45@Vector` , and
:ref:`Runge45@Fun` ,
the first call to ``Runge45``
must not be :ref:`parallel<ta_in_parallel-name>` execution mode.

Example
*******
{xrst_toc_hidden
   example/utility/runge45_1.cpp
   example/utility/runge_45.cpp
}
The file
:ref:`runge45_1.cpp-name`
contains a simple example and test of ``Runge45`` .

The file
:ref:`runge_45.cpp-name` contains an example using ``Runge45``
in the context of algorithmic differentiation.
It also returns true if it succeeds and false otherwise.

Source Code
***********
The source code for this routine is in the file
``cppad/runge_45.hpp`` .

{xrst_end Runge45}
--------------------------------------------------------------------------
*/
# include <cstddef>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/utility/check_simple_vector.hpp>
# include <cppad/utility/check_numeric_type.hpp>
# include <cppad/utility/nan.hpp>

// needed before one can use CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { // BEGIN CppAD namespace

template <class Scalar, class Vector, class Fun>
Vector Runge45(
   Fun           &F ,
   size_t         M ,
   const Scalar &ti ,
   const Scalar &tf ,
   const Vector &xi )
{  Vector e( xi.size() );
   return Runge45(F, M, ti, tf, xi, e);
}

template <class Scalar, class Vector, class Fun>
Vector Runge45(
   Fun           &F ,
   size_t         M ,
   const Scalar &ti ,
   const Scalar &tf ,
   const Vector &xi ,
   Vector       &e )
{
   CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;

   // check numeric type specifications
   CheckNumericType<Scalar>();

   // check simple vector class specifications
   CheckSimpleVector<Scalar, Vector>();

   // Cash-Karp parameters for embedded Runge-Kutta method
   // are static to avoid recalculation on each call and
   // do not use Vector to avoid possible memory leak
   static Scalar a[6] = {
      Scalar(0),
      Scalar(1) / Scalar(5),
      Scalar(3) / Scalar(10),
      Scalar(3) / Scalar(5),
      Scalar(1),
      Scalar(7) / Scalar(8)
   };
   static Scalar b[5 * 5] = {
      Scalar(1) / Scalar(5),
      Scalar(0),
      Scalar(0),
      Scalar(0),
      Scalar(0),

      Scalar(3) / Scalar(40),
      Scalar(9) / Scalar(40),
      Scalar(0),
      Scalar(0),
      Scalar(0),

      Scalar(3) / Scalar(10),
      -Scalar(9) / Scalar(10),
      Scalar(6) / Scalar(5),
      Scalar(0),
      Scalar(0),

      -Scalar(11) / Scalar(54),
      Scalar(5) / Scalar(2),
      -Scalar(70) / Scalar(27),
      Scalar(35) / Scalar(27),
      Scalar(0),

      Scalar(1631) / Scalar(55296),
      Scalar(175) / Scalar(512),
      Scalar(575) / Scalar(13824),
      Scalar(44275) / Scalar(110592),
      Scalar(253) / Scalar(4096)
   };
   static Scalar c4[6] = {
      Scalar(2825) / Scalar(27648),
      Scalar(0),
      Scalar(18575) / Scalar(48384),
      Scalar(13525) / Scalar(55296),
      Scalar(277) / Scalar(14336),
      Scalar(1) / Scalar(4),
   };
   static Scalar c5[6] = {
      Scalar(37) / Scalar(378),
      Scalar(0),
      Scalar(250) / Scalar(621),
      Scalar(125) / Scalar(594),
      Scalar(0),
      Scalar(512) / Scalar(1771)
   };

   CPPAD_ASSERT_KNOWN(
      M >= 1,
      "Error in Runge45: the number of steps is less than one"
   );
   CPPAD_ASSERT_KNOWN(
      e.size() == xi.size(),
      "Error in Runge45: size of e not equal to size of xi"
   );
   size_t i, j, k, m;              // indices

   size_t  n = xi.size();           // number of components in X(t)
   Scalar  ns = Scalar(int(M));     // number of steps as Scalar object
   Scalar  h = (tf - ti) / ns;      // step size
   Scalar  zero_or_nan = Scalar(0); // zero (nan if Ode returns has a nan)
   for(i = 0; i < n; i++)           // initialize e = 0
      e[i] = zero_or_nan;

   // vectors used to store values returned by F
   Vector fh(6 * n), xtmp(n), ftmp(n), x4(n), x5(n), xf(n);

   xf = xi;           // initialize solution
   for(m = 0; m < M; m++)
   {  // time at beginning of this interval
      // (convert to int to avoid MS compiler warning)
      Scalar t = ti * (Scalar(int(M - m)) / ns)
                 + tf * (Scalar(int(m)) / ns);

      // loop over integration steps
      x4 = x5 = xf;   // start x4 and x5 at same point for each step
      for(j = 0; j < 6; j++)
      {  // loop over function evaluations for this step
         xtmp = xf;  // location for next function evaluation
         for(k = 0; k < j; k++)
         {  // loop over previous function evaluations
            Scalar bjk = b[ (j-1) * 5 + k ];
            for(i = 0; i < n; i++)
            {  // loop over elements of x
               xtmp[i] += bjk * fh[i * 6 + k];
            }
         }
         // ftmp = F(t + a[j] * h, xtmp)
         F.Ode(t + a[j] * h, xtmp, ftmp);

         // if ftmp has a nan, set zero_or_nan to nan
         for(i = 0; i < n; i++)
            zero_or_nan *= ftmp[i];

         for(i = 0; i < n; i++)
         {  // loop over elements of x
            Scalar fhi    = ftmp[i] * h;
            fh[i * 6 + j] = fhi;
            x4[i]        += c4[j] * fhi;
            x5[i]        += c5[j] * fhi;
            x5[i]        += zero_or_nan;
         }
      }
      // accumulate error bound
      for(i = 0; i < n; i++)
      {  // cant use abs because cppad.hpp may not be included
         Scalar diff = x5[i] - x4[i];
         e[i] += fabs(diff);
         e[i] += zero_or_nan;
      }

      // advance xf for this step using x5
      xf = x5;
   }
   return xf;
}

} // End CppAD namespace

# endif
