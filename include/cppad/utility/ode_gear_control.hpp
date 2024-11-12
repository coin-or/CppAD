# ifndef CPPAD_UTILITY_ODE_GEAR_CONTROL_HPP
# define CPPAD_UTILITY_ODE_GEAR_CONTROL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin OdeGearControl}
{xrst_spell
   dep
   eabs
   ef
   erel
   maxabs
   nstep
   sini
   smax
   smin
   tb
   test test
   tf
   xf
}

An Error Controller for Gear's Ode Solvers
##########################################

Syntax
******
| # ``include <cppad/utility/ode_gear_control.hpp>``
| *xf* = ``OdeGearControl`` ( *F* , *M* , *ti* , *tf* , *xi* ,
| |tab| ``smin`` , ``smax`` , ``sini`` , ``eabs`` , ``erel`` , ``ef`` , ``maxabs`` , ``nstep``  )

Purpose
*******
Let :math:`\B{R}` denote the real numbers
and let :math:`f : \B{R} \times \B{R}^n \rightarrow \B{R}^n` be a smooth function.
We define :math:`X : [ti , tf] \rightarrow \B{R}^n` by
the following initial value problem:

.. math::
   :nowrap:

   \begin{eqnarray}
      X(ti)  & = & xi    \\
      X'(t)  & = & f[t , X(t)]
   \end{eqnarray}

The routine :ref:`OdeGear-name` is a stiff multi-step method that
can be used to approximate the solution to this equation.
The routine ``OdeGearControl`` sets up this multi-step method
and controls the error during such an approximation.

Include
*******
The file ``cppad/utility/ode_gear_control.hpp``
is included by ``cppad/cppad.hpp``
but it can also be included separately with out the rest of
the ``CppAD`` routines.

Notation
********
The template parameter types :ref:`OdeGearControl@Scalar` and
:ref:`OdeGearControl@Vector` are documented below.

xf
**
The return value *xf* has the prototype

   *Vector* *xf*

and the size of *xf* is equal to *n*
(see description of :ref:`OdeGear@Vector` below).
It is the approximation for :math:`X(tf)`.

Fun
***
The class *Fun*
and the object *F* satisfy the prototype

   *Fun* & *F*

This must support the following set of calls

| |tab| *F* . ``Ode`` ( *t* , *x* , *f* )
| |tab| *F* . ``Ode_dep`` ( *t* , *x* , *f_x* )

t
=
The argument *t* has prototype

   ``const`` *Scalar* & *t*

(see description of :ref:`OdeGear@Scalar` below).

x
=
The argument *x* has prototype

   ``const`` *Vector* & *x*

and has size *N*
(see description of :ref:`OdeGear@Vector` below).

f
=
The argument *f* to *F* . ``Ode`` has prototype

   *Vector* & *f*

On input and output, *f* is a vector of size *N*
and the input values of the elements of *f* do not matter.
On output,
*f* is set equal to :math:`f(t, x)`
(see *f* ( *t* , *x* ) in :ref:`OdeGear@Purpose` ).

f_x
===
The argument *f_x* has prototype

   *Vector* & *f_x*

On input and output, *f_x* is a vector of size :math:`N * N`
and the input values of the elements of *f_x* do not matter.
On output,

.. math::

   f\_x [i * n + j] = \partial_{x(j)} f_i ( t , x )

Warning
=======
The arguments *f* , and *f_x*
must have a call by reference in their prototypes; i.e.,
do not forget the ``&`` in the prototype for
*f* and *f_x* .

M
*
The argument *M* has prototype

   ``size_t`` *M*

It specifies the order of the multi-step method; i.e.,
the order of the approximating polynomial
(after the initialization process).
The argument *M* must greater than or equal one.

ti
**
The argument *ti* has prototype

   ``const`` *Scalar* & *ti*

It specifies the initial time for the integration of
the differential equation.

tf
**
The argument *tf* has prototype

   ``const`` *Scalar* & *tf*

It specifies the final time for the integration of
the differential equation.

xi
**
The argument *xi* has prototype

   ``const`` *Vector* & *xi*

and size *n* .
It specifies value of :math:`X(ti)`.

smin
****
The argument *smin* has prototype

   ``const`` *Scalar* & *smin*

The minimum value of :math:`T[M] -  T[M-1]` in a call to ``OdeGear``
will be :math:`smin` except for the last two calls where it may be
as small as :math:`smin / 2`.
The value of *smin* must be less than or equal *smax* .

smax
****
The argument *smax* has prototype

   ``const`` *Scalar* & *smax*

It specifies the maximum step size to use during the integration;
i.e., the maximum value for :math:`T[M] - T[M-1]`
in a call to ``OdeGear`` .

sini
****
The argument *sini* has prototype

   *Scalar* & *sini*

The value of *sini* is the minimum
step size to use during initialization of the multi-step method; i.e.,
for calls to ``OdeGear`` where :math:`m < M`.
The value of *sini* must be less than or equal *smax*
(and can also be less than *smin* ).

eabs
****
The argument *eabs* has prototype

   ``const`` *Vector* & *eabs*

and size *n* .
Each of the elements of *eabs* must be
greater than or equal zero.
It specifies a bound for the absolute
error in the return value *xf* as an approximation for :math:`X(tf)`.
(see the
:ref:`OdeGearControl@Error Criteria Discussion`
below).

erel
****
The argument *erel* has prototype

   ``const`` *Scalar* & *erel*

and is greater than or equal zero.
It specifies a bound for the relative
error in the return value *xf* as an approximation for :math:`X(tf)`
(see the
:ref:`OdeGearControl@Error Criteria Discussion`
below).

ef
**
The argument value *ef* has prototype

   *Vector* & *ef*

and size *n* .
The input value of its elements does not matter.
On output,
it contains an estimated bound for the
absolute error in the approximation *xf* ; i.e.,

.. math::

   ef_i > | X( tf )_i - xf_i |

maxabs
******
The argument *maxabs* is optional in the call to ``OdeGearControl`` .
If it is present, it has the prototype

   *Vector* & *maxabs*

and size *n* .
The input value of its elements does not matter.
On output,
it contains an estimate for the
maximum absolute value of :math:`X(t)`; i.e.,

.. math::

   maxabs[i] \approx \max \left\{
      | X( t )_i | \; : \;  t \in [ti, tf]
   \right\}

nstep
*****
The argument *nstep* has the prototype

   *size_t* & *nstep*

Its input value does not matter and its output value
is the number of calls to :ref:`OdeGear-name`
used by ``OdeGearControl`` .

Error Criteria Discussion
*************************
The relative error criteria *erel* and
absolute error criteria *eabs* are enforced during each step of the
integration of the ordinary differential equations.
In addition, they are inversely scaled by the step size so that
the total error bound is less than the sum of the error bounds.
To be specific, if :math:`\tilde{X} (t)` is the approximate solution
at time :math:`t`,
*ta* is the initial step time,
and *tb* is the final step time,

.. math::

   \left| \tilde{X} (tb)_j  - X (tb)_j \right|
   \leq
   \frac{tf - ti}{tb - ta}
   \left[ eabs[j] + erel \;  | \tilde{X} (tb)_j | \right]

If :math:`X(tb)_j` is near zero for some :math:`tb \in [ti , tf]`,
and one uses an absolute error criteria :math:`eabs[j]` of zero,
the error criteria above will force ``OdeGearControl``
to use step sizes equal to
:ref:`OdeGearControl@smin`
for steps ending near :math:`tb`.
In this case, the error relative to *maxabs* can be judged after
``OdeGearControl`` returns.
If *ef* is to large relative to *maxabs* ,
``OdeGearControl`` can be called again
with a smaller value of *smin* .

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
   * - *a* <= *b*
     - returns true (false) if *a* is less than or equal
       (greater than) *b* .
   * - *a* == *b*
     - returns true (false) if *a* is equal to *b* .
   * - ``log`` ( *a* )
     - returns a *Scalar* equal to the logarithm of *a*
   * - ``exp`` ( *a* )
     - returns a *Scalar* equal to the exponential of *a*

Vector
******
The type *Vector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type Scalar<SimpleVector@Elements of Specified Type>` .
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.

Example
*******
{xrst_toc_hidden
   example/utility/ode_gear_control.cpp
}
The file
:ref:`ode_gear_control.cpp-name`
contains an example and test a test of using this routine.

Theory
******
Let :math:`e(s)` be the error as a function of the
step size :math:`s` and suppose that there is a constant
:math:`K` such that :math:`e(s) = K s^m`.
Let :math:`a` be our error bound.
Given the value of :math:`e(s)`, a step of size :math:`\lambda s`
would be ok provided that

.. math::
   :nowrap:

   \begin{eqnarray}
      a  & \geq & e( \lambda s ) (tf - ti) / ( \lambda s ) \\
      a  & \geq & K \lambda^m s^m (tf - ti) / ( \lambda s ) \\
      a  & \geq & \lambda^{m-1} s^{m-1} (tf - ti) e(s) / s^m \\
      a  & \geq & \lambda^{m-1} (tf - ti) e(s) / s           \\
      \lambda^{m-1} & \leq & \frac{a}{e(s)} \frac{s}{tf - ti}
   \end{eqnarray}

Thus if the right hand side of the last inequality is greater
than or equal to one, the step of size :math:`s` is ok.

Source Code
***********
The source code for this routine is in the file
``cppad/ode_gear_control.hpp`` .

{xrst_end OdeGearControl}
--------------------------------------------------------------------------
*/

// link exp and log for float and double
# include <cppad/base_require.hpp>

# include <cppad/utility/ode_gear.hpp>

namespace CppAD { // Begin CppAD namespace

template <class Scalar, class Vector, class Fun>
Vector OdeGearControl(
   Fun             &F     ,
   size_t           M     ,
   const Scalar    &ti    ,
   const Scalar    &tf    ,
   const Vector    &xi    ,
   const Scalar    &smin  ,
   const Scalar    &smax  ,
   Scalar          &sini  ,
   const Vector    &eabs  ,
   const Scalar    &erel  ,
   Vector          &ef    ,
   Vector          &maxabs,
   size_t          &nstep )
{
   // check simple vector class specifications
   CheckSimpleVector<Scalar, Vector>();

   // dimension of the state space
   size_t n = size_t(xi.size());

   CPPAD_ASSERT_KNOWN(
      M >= 1,
      "Error in OdeGearControl: M is less than one"
   );
   CPPAD_ASSERT_KNOWN(
      smin <= smax,
      "Error in OdeGearControl: smin is greater than smax"
   );
   CPPAD_ASSERT_KNOWN(
      sini <= smax,
      "Error in OdeGearControl: sini is greater than smax"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(eabs.size()) == n,
      "Error in OdeGearControl: size of eabs is not equal to n"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(maxabs.size()) == n,
      "Error in OdeGearControl: size of maxabs is not equal to n"
   );

   // some constants
   const Scalar zero(0);
   const Scalar one(1);
   const Scalar one_plus( Scalar(3) / Scalar(2) );
   const Scalar two(2);
   const Scalar ten(10);

   // temporary indices
   size_t i, k;

   // temporary Scalars
   Scalar step, sprevious, lambda, axi, a, root, r;

   // vectors of Scalars
   Vector T  (M + 1);
   Vector X( (M + 1) * n );
   Vector e(n);
   Vector xf(n);

   // initial integer values
   size_t m = 1;
   nstep    = 0;

   // initialize T
   T[0] = ti;

   // initialize X, ef, maxabs
   for(i = 0; i < n; i++)
   for(i = 0; i < n; i++)
   {  X[i] = xi[i];
      ef[i] = zero;
      X[i]  = xi[i];
      if( zero <= xi[i] )
         maxabs[i] = xi[i];
      else
         maxabs[i] = - xi[i];

   }

   // initial step size
   step = smin;

   while( T[m-1] < tf )
   {  sprevious = step;

      // check maximum
      if( smax <= step )
         step = smax;

      // check minimum
      if( m < M )
      {  if( step <= sini )
            step = sini;
      }
      else
         if( step <= smin )
            step = smin;

      // check if near the end
      if( tf <= T[m-1] + one_plus * step )
         T[m] = tf;
      else
         T[m] = T[m-1] + step;

      // try using this step size
      nstep++;
      OdeGear(F, m, n, T, X, e);
      step = T[m] - T[m-1];

      // compute value of lambda for this step
      lambda = Scalar(10) *  sprevious / step;
      for(i = 0; i < n; i++)
      {  axi = X[m * n + i];
         if( axi <= zero )
            axi = - axi;
         a  = eabs[i] + erel * axi;
         if( e[i] > zero )
         {  if( m == 1 )
               root = (a / e[i]) / ten;
            else
            {  r = ( a / e[i] ) * step / (tf - ti);
               root = exp( log(r) / Scalar(m-1) );
            }
            if( root <= lambda )
               lambda = root;
         }
      }

      bool advance;
      if( m == M )
         advance = one <= lambda || step <= one_plus * smin;
      else
         advance = one <= lambda || step <= one_plus * sini;


      if( advance )
      {  // accept the results of this time step
         CPPAD_ASSERT_UNKNOWN( m <= M );
         if( m == M )
         {  // shift for next step
            for(k = 0; k < m; k++)
            {  T[k] = T[k+1];
               for(i = 0; i < n; i++)
                  X[k*n + i] = X[(k+1)*n + i];
            }
         }
         // update ef and maxabs
         for(i = 0; i < n; i++)
         {  ef[i] = ef[i] + e[i];
            axi = X[m * n + i];
            if( axi <= zero )
               axi = - axi;
            if( axi > maxabs[i] )
               maxabs[i] = axi;
         }
         if( m != M )
            m++;  // all we need do in this case
      }

      // new step suggested by error criteria
      step = std::min(lambda , ten) * step / two;
   }
   for(i = 0; i < n; i++)
      xf[i] = X[(m-1) * n + i];

   return xf;
}

} // End CppAD namespace

# endif
