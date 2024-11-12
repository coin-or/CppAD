# ifndef CPPAD_UTILITY_ROSEN_34_HPP
# define CPPAD_UTILITY_ROSEN_34_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin Rosen34}
{xrst_spell
   dep
   rd
   rosenbrock
   test test
   tf
   xf
}

A 3rd and 4th Order Rosenbrock ODE Solver
#########################################

Syntax
******
| # ``include <cppad/utility/rosen_34.hpp>``
| *xf* = ``Rosen34`` ( *F* , *M* , *ti* , *tf* , *xi* )
| *xf* = ``Rosen34`` ( *F* , *M* , *ti* , *tf* , *xi* , *e* )

Description
***********
This is an embedded 3rd and 4th order Rosenbrock ODE solver
(see Section 16.6 of :ref:`Bib@Numerical Recipes`
for a description of Rosenbrock ODE solvers).
In particular, we use the formulas taken from page 100 of
:ref:`Bib@Shampine, L.F.`
(except that the fraction 98/108 has been correction to be 97/108).

We use :math:`n` for the size of the vector *xi* .
Let :math:`\B{R}` denote the real numbers
and let :math:`F : \B{R} \times \B{R}^n \rightarrow \B{R}^n` be a smooth function.
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

If your set of  ordinary differential equations are not stiff
an explicit method may be better (perhaps :ref:`Runge45-name` .)

Include
*******
The file ``cppad/utility/rosen_34.hpp``
is included by ``cppad/cppad.hpp``
but it can also be included separately with out the rest of
the ``CppAD`` routines.

xf
**
The return value *xf* has the prototype

   *Vector* *xf*

and the size of *xf* is equal to *n*
(see description of :ref:`Rosen34@Vector` below).

.. math::

   X(tf) = xf + O( h^5 )

where :math:`h = (tf - ti) / M` is the step size.
If *xf* contains not a number :ref:`nan-name` ,
see the discussion of :ref:`f<Rosen34@Fun@Nan>` .

Fun
***
The class *Fun*
and the object *F* satisfy the prototype

   *Fun* & *F*

This must support the following set of calls

| |tab| *F* . ``Ode`` ( *t* , *x* , *f* )
| |tab| *F* . ``Ode_ind`` ( *t* , *x* , *f_t* )
| |tab| *F* . ``Ode_dep`` ( *t* , *x* , *f_x* )

t
=
In all three cases,
the argument *t* has prototype

   ``const`` *Scalar* & *t*

(see description of :ref:`Rosen34@Scalar` below).

x
=
In all three cases,
the argument *x* has prototype

   ``const`` *Vector* & *x*

and has size *n*
(see description of :ref:`Rosen34@Vector` below).

f
=
The argument *f* to *F* . ``Ode`` has prototype

   *Vector* & *f*

On input and output, *f* is a vector of size *n*
and the input values of the elements of *f* do not matter.
On output,
*f* is set equal to :math:`F(t, x)`
(see *F* ( *t* , *x* ) in :ref:`Rosen34@Description` ).

f_t
===
The argument *f_t* to *F* . ``Ode_ind`` has prototype

   *Vector* & *f_t*

On input and output, *f_t* is a vector of size *n*
and the input values of the elements of *f_t* do not matter.
On output, the *i*-th element of
*f_t* is set equal to :math:`\partial_t F_i (t, x)`
(see *F* ( *t* , *x* ) in :ref:`Rosen34@Description` ).

f_x
===
The argument *f_x* to *F* . ``Ode_dep`` has prototype

   *Vector* & *f_x*

On input and output, *f_x* is a vector of size *n* * *n*
and the input values of the elements of *f_x* do not matter.
On output, the [ *i* * *n* + *j* ] element of
*f_x* is set equal to :math:`\partial_{x(j)} F_i (t, x)`
(see *F* ( *t* , *x* ) in :ref:`Rosen34@Description` ).

Nan
===
If any of the elements of *f* , *f_t* , or *f_x*
have the value not a number ``nan`` ,
the routine ``Rosen34`` returns with all the
elements of *xf* and *e* equal to ``nan`` .

Warning
=======
The arguments *f* , *f_t* , and *f_x*
must have a call by reference in their prototypes; i.e.,
do not forget the ``&`` in the prototype for
*f* , *f_t* and *f_x* .

Optimization
============
Every call of the form

   *F* . ``Ode_ind`` ( *t* , *x* , *f_t* )

is directly followed by a call of the form

   *F* . ``Ode_dep`` ( *t* , *x* , *f_x* )

where the arguments *t* and *x* have not changed between calls.
In many cases it is faster to compute the values of *f_t*
and *f_x* together and then pass them back one at a time.

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

(see description of :ref:`Rosen34@Scalar` below).
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

   e = O( h^4 )

where :math:`h = (tf - ti) / M` is the step size.

Scalar
******
The type *Scalar* must satisfy the conditions
for a :ref:`NumericType-name` .
The routine :ref:`CheckNumericType-name` will generate an error message
if this is not the case.
In addition, the following operations must be defined for
*Scalar* objects *a* and *b* :

.. list-table::
   :widths: auto

   * - **Operation**
     - **Description**
   * - *a* < *b*
     - less than operator (returns a ``bool`` object)

Vector
******
The type *Vector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type Scalar<SimpleVector@Elements of Specified Type>` .
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.

Parallel Mode
*************
For each set of types
:ref:`Rosen34@Scalar` ,
:ref:`Rosen34@Vector` , and
:ref:`Rosen34@Fun` ,
the first call to ``Rosen34``
must not be :ref:`parallel<ta_in_parallel-name>` execution mode.

Example
*******
{xrst_toc_hidden
   example/utility/rosen_34.cpp
}
The file
:ref:`rosen_34.cpp-name`
contains an example and test a test of using this routine.

Source Code
***********
The source code for this routine is in the file
``cppad/rosen_34.hpp`` .

{xrst_end Rosen34}
--------------------------------------------------------------------------
*/

# include <cstddef>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/utility/check_simple_vector.hpp>
# include <cppad/utility/check_numeric_type.hpp>
# include <cppad/utility/vector.hpp>
# include <cppad/utility/lu_factor.hpp>
# include <cppad/utility/lu_invert.hpp>

// needed before one can use CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { // BEGIN CppAD namespace

template <class Scalar, class Vector, class Fun>
Vector Rosen34(
   Fun           &F ,
   size_t         M ,
   const Scalar &ti ,
   const Scalar &tf ,
   const Vector &xi )
{  Vector e( xi.size() );
   return Rosen34(F, M, ti, tf, xi, e);
}

template <class Scalar, class Vector, class Fun>
Vector Rosen34(
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

   // Parameters for Shampine's Rosenbrock method
   // are static to avoid recalculation on each call and
   // do not use Vector to avoid possible memory leak
   static Scalar a[3] = {
      Scalar(0),
      Scalar(1),
      Scalar(3)   / Scalar(5)
   };
   static Scalar b[2 * 2] = {
      Scalar(1),
      Scalar(0),
      Scalar(24)  / Scalar(25),
      Scalar(3)   / Scalar(25)
   };
   static Scalar ct[4] = {
      Scalar(1)   / Scalar(2),
      - Scalar(3) / Scalar(2),
      Scalar(121) / Scalar(50),
      Scalar(29)  / Scalar(250)
   };
   static Scalar cg[3 * 3] = {
      - Scalar(4),
      Scalar(0),
      Scalar(0),
      Scalar(186) / Scalar(25),
      Scalar(6)   / Scalar(5),
      Scalar(0),
      - Scalar(56) / Scalar(125),
      - Scalar(27) / Scalar(125),
      - Scalar(1)  / Scalar(5)
   };
   static Scalar d3[3] = {
      Scalar(97) / Scalar(108),
      Scalar(11) / Scalar(72),
      Scalar(25) / Scalar(216)
   };
   static Scalar d4[4] = {
      Scalar(19)  / Scalar(18),
      Scalar(1)   / Scalar(4),
      Scalar(25)  / Scalar(216),
      Scalar(125) / Scalar(216)
   };
   CPPAD_ASSERT_KNOWN(
      M >= 1,
      "Error in Rosen34: the number of steps is less than one"
   );
   CPPAD_ASSERT_KNOWN(
      e.size() == xi.size(),
      "Error in Rosen34: size of e not equal to size of xi"
   );
   size_t i, j, k, l, m;             // indices

   size_t  n    = xi.size();         // number of components in X(t)
   Scalar  ns   = Scalar(double(M)); // number of steps as Scalar object
   Scalar  h    = (tf - ti) / ns;    // step size
   Scalar  zero = Scalar(0);         // some constants
   Scalar  one  = Scalar(1);
   Scalar  two  = Scalar(2);

   // permutation vectors needed for LU factorization routine
   CppAD::vector<size_t> ip(n), jp(n);

   // vectors used to store values returned by F
   Vector E(n * n), Eg(n), f_t(n);
   Vector g(n * 3), x3(n), x4(n), xf(n), ftmp(n), xtmp(n), nan_vec(n);

   // initialize e = 0, nan_vec = nan
   for(i = 0; i < n; i++)
   {  e[i]       = zero;
      nan_vec[i] = nan(zero);
   }

   xf = xi;           // initialize solution
   for(m = 0; m < M; m++)
   {  // time at beginning of this interval
      Scalar t = ti * (Scalar(int(M - m)) / ns)
                 + tf * (Scalar(int(m)) / ns);

      // value of x at beginning of this interval
      x3 = x4 = xf;

      // evaluate partial derivatives at beginning of this interval
      F.Ode_ind(t, xf, f_t);
      F.Ode_dep(t, xf, E);    // E = f_x
      if( hasnan(f_t) || hasnan(E) )
      {  e = nan_vec;
         return nan_vec;
      }

      // E = I - f_x * h / 2
      for(i = 0; i < n; i++)
      {  for(j = 0; j < n; j++)
            E[i * n + j] = - E[i * n + j] * h / two;
         E[i * n + i] += one;
      }

      // LU factor the matrix E
# ifndef NDEBUG
      int sign = LuFactor(ip, jp, E);
# else
      LuFactor(ip, jp, E);
# endif
      CPPAD_ASSERT_KNOWN(
         sign != 0,
         "Error in Rosen34: I - f_x * h / 2 not invertible"
      );

      // loop over integration steps
      for(k = 0; k < 3; k++)
      {  // set location for next function evaluation
         xtmp = xf;
         for(l = 0; l < k; l++)
         {  // loop over previous function evaluations
            Scalar bkl = b[(k-1)*2 + l];
            for(i = 0; i < n; i++)
            {  // loop over elements of x
               xtmp[i] += bkl * g[i*3 + l] * h;
            }
         }
         // ftmp = F(t + a[k] * h, xtmp)
         F.Ode(t + a[k] * h, xtmp, ftmp);
         if( hasnan(ftmp) )
         {  e = nan_vec;
            return nan_vec;
         }

         // Form Eg for this integration step
         for(i = 0; i < n; i++)
            Eg[i] = ftmp[i] + ct[k] * f_t[i] * h;
         for(l = 0; l < k; l++)
         {  for(i = 0; i < n; i++)
               Eg[i] += cg[(k-1)*3 + l] * g[i*3 + l];
         }

         // Solve the equation E * g = Eg
         LuInvert(ip, jp, E, Eg);

         // save solution and advance x3, x4
         for(i = 0; i < n; i++)
         {  g[i*3 + k]  = Eg[i];
            x3[i]      += h * d3[k] * Eg[i];
            x4[i]      += h * d4[k] * Eg[i];
         }
      }
      // Form Eg for last update to x4 only
      for(i = 0; i < n; i++)
         Eg[i] = ftmp[i] + ct[3] * f_t[i] * h;
      for(l = 0; l < 3; l++)
      {  for(i = 0; i < n; i++)
            Eg[i] += cg[2*3 + l] * g[i*3 + l];
      }

      // Solve the equation E * g = Eg
      LuInvert(ip, jp, E, Eg);

      // advance x4 and accumulate error bound
      for(i = 0; i < n; i++)
      {  x4[i] += h * d4[3] * Eg[i];

         // cant use abs because cppad.hpp may not be included
         Scalar diff = x4[i] - x3[i];
         if( diff < zero )
            e[i] -= diff;
         else
            e[i] += diff;
      }

      // advance xf for this step using x4
      xf = x4;
   }
   return xf;
}

} // End CppAD namespace

# endif
