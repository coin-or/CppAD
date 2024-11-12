# ifndef CPPAD_UTILITY_ODE_ERR_CONTROL_HPP
# define CPPAD_UTILITY_ODE_ERR_CONTROL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin OdeErrControl}
{xrst_spell
   eabs
   eb
   ef
   erel
   nstep
   scur
   smax
   smin
   tb
   tf
   xa
   xb
   xf
}

An Error Controller for ODE Solvers
###################################

Syntax
******
| # ``include <cppad/utility/ode_err_control.hpp>``
| *xf* = ``OdeErrControl`` ( *method* , *ti* , *tf* , *xi* ,
| |tab| ``smin`` , ``smax`` , ``scur`` , ``eabs`` , ``erel`` , ``ef`` , ``maxabs`` , ``nstep``  )

Description
***********
Let :math:`\B{R}` denote the real numbers
and let :math:`F : \B{R} \times \B{R}^n \rightarrow \B{R}^n` be a smooth function.
We define :math:`X : [ti , tf] \rightarrow \B{R}^n` by
the following initial value problem:

.. math::
   :nowrap:

   \begin{eqnarray}
      X(ti)  & = & xi    \\
      X'(t)  & = & F[t , X(t)]
   \end{eqnarray}

The routine ``OdeErrControl`` can be used to adjust the step size
used an arbitrary integration methods in order to be as fast as possible
and still with in a requested error bound.

Include
*******
The file ``cppad/utility/ode_err_control.hpp`` is included by
``cppad/cppad.hpp``
but it can also be included separately with out the rest of
the ``CppAD`` routines.

Notation
********
The template parameter types :ref:`OdeErrControl@Scalar` and
:ref:`OdeErrControl@Vector` are documented below.

xf
**
The return value *xf* has the prototype

   *Vector* *xf*

(see description of :ref:`OdeErrControl@Vector` below).
and the size of *xf* is equal to *n* .
If *xf* contains not a number :ref:`nan-name` ,
see the discussion of :ref:`step<OdeErrControl@Method@Nan>` .

Method
******
The class *Method*
and the object *method* satisfy the following syntax

   *Method* & *method*

The object *method* must support ``step`` and
``order`` member functions defined below:

step
====
The syntax

   *method* . ``step`` ( *ta* , *tb* , *xa* , *xb* , *eb* )

executes one step of the integration method.

   *ta*

The argument *ta* has prototype

   ``const`` *Scalar* & *ta*

It specifies the initial time for this step in the
ODE integration.
(see description of :ref:`OdeErrControl@Scalar` below).

   *tb*

The argument *tb* has prototype

   ``const`` *Scalar* & *tb*

It specifies the final time for this step in the
ODE integration.

   *xa*

The argument *xa* has prototype

   ``const`` *Vector* & *xa*

and size *n* .
It specifies the value of :math:`X(ta)`.
(see description of :ref:`OdeErrControl@Vector` below).

   *xb*

The argument value *xb* has prototype

   *Vector* & *xb*

and size *n* .
The input value of its elements does not matter.
On output,
it contains the approximation for :math:`X(tb)` that the method obtains.

   *eb*

The argument value *eb* has prototype

   *Vector* & *eb*

and size *n* .
The input value of its elements does not matter.
On output,
it contains an estimate for the error in the approximation *xb* .
It is assumed (locally) that the error bound in this approximation
nearly equal to :math:`K (tb - ta)^m`
where *K* is a fixed constant and *m*
is the corresponding argument to ``CodeControl`` .

Nan
===
If any element of the vector *eb* or *xb* are
not a number ``nan`` ,
the current step is considered to large.
If this happens with the current step size equal to *smin* ,
``OdeErrControl`` returns with *xf* and *ef* as vectors
of ``nan`` .

order
=====
If *m* is ``size_t`` ,
the object *method* must also support the following syntax

   *m* = *method* . ``order`` ()

The return value *m* is the order of the error estimate;
i.e., there is a constant K such that if :math:`ti \leq ta \leq tb \leq tf`,

.. math::

   | eb(tb) | \leq K | tb - ta |^m

where *ta* , *tb* , and *eb* are as in
*method* . ``step`` ( *ta* , *tb* , *xa* , *xb* , *eb* )

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

The step size during a call to *method* is defined as
the corresponding value of :math:`tb - ta`.
If :math:`tf - ti \leq smin`,
the integration will be done in one step of size *tf - ti* .
Otherwise,
the minimum value of *tb - ta* will be :math:`smin`
except for the last two calls to *method* where it may be
as small as :math:`smin / 2`.

smax
****
The argument *smax* has prototype

   ``const`` *Scalar* & *smax*

It specifies the maximum step size to use during the integration;
i.e., the maximum value for :math:`tb - ta` in a call to *method* .
The value of *smax* must be greater than or equal *smin* .

scur
****
The argument *scur* has prototype

   *Scalar* & *scur*

The value of *scur* is the suggested next step size,
based on error criteria, to try in the next call to *method* .
On input it corresponds to the first call to *method* ,
in this call to ``OdeErrControl`` (where :math:`ta = ti`).
On output it corresponds to the next call to *method* ,
in a subsequent call to ``OdeErrControl`` (where *ta* = *tf* ).

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
:ref:`OdeErrControl@Error Criteria Discussion`
below).

erel
****
The argument *erel* has prototype

   ``const`` *Scalar* & *erel*

and is greater than or equal zero.
It specifies a bound for the relative
error in the return value *xf* as an approximation for :math:`X(tf)`
(see the
:ref:`OdeErrControl@Error Criteria Discussion`
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

If on output *ef* contains not a number ``nan`` ,
see the discussion of :ref:`step<OdeErrControl@Method@Nan>` .

maxabs
******
The argument *maxabs* is optional in the call to ``OdeErrControl`` .
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
The argument *nstep* is optional in the call to ``OdeErrControl`` .
If it is present, it has the prototype

   *size_t* & *nstep*

Its input value does not matter and its output value
is the number of calls to *method* . ``step``
used by ``OdeErrControl`` .

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
the error criteria above will force ``OdeErrControl``
to use step sizes equal to
:ref:`OdeErrControl@smin`
for steps ending near :math:`tb`.
In this case, the error relative to *maxabs* can be judged after
``OdeErrControl`` returns.
If *ef* is to large relative to *maxabs* ,
``OdeErrControl`` can be called again
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
   example/utility/ode_err_control.cpp
   example/utility/ode_err_maxabs.cpp
}
The files
:ref:`ode_err_control.cpp-name`
and
:ref:`ode_err_maxabs.cpp-name`
contain examples and tests of using this routine.
They return true if they succeed and false otherwise.

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
``cppad/ode_err_control.hpp`` .

{xrst_end OdeErrControl}
--------------------------------------------------------------------------
*/

// link exp and log for float and double
# include <cppad/base_require.hpp>

# include <cppad/core/cppad_assert.hpp>
# include <cppad/utility/check_simple_vector.hpp>
# include <cppad/utility/nan.hpp>

namespace CppAD { // Begin CppAD namespace

template <class Scalar, class Vector, class Method>
Vector OdeErrControl(
   Method          &method,
   const Scalar    &ti    ,
   const Scalar    &tf    ,
   const Vector    &xi    ,
   const Scalar    &smin  ,
   const Scalar    &smax  ,
   Scalar          &scur  ,
   const Vector    &eabs  ,
   const Scalar    &erel  ,
   Vector          &ef    ,
   Vector          &maxabs,
   size_t          &nstep )
{
   // check simple vector class specifications
   CheckSimpleVector<Scalar, Vector>();

   size_t n = size_t(xi.size());

   CPPAD_ASSERT_KNOWN(
      smin <= smax,
      "Error in OdeErrControl: smin > smax"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(eabs.size()) == n,
      "Error in OdeErrControl: size of eabs is not equal to n"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(maxabs.size()) == n,
      "Error in OdeErrControl: size of maxabs is not equal to n"
   );
   size_t m = method.order();
   CPPAD_ASSERT_KNOWN(
      m > 1,
      "Error in OdeErrControl: m is less than or equal one"
   );

   bool    ok;
   bool    minimum_step;
   size_t  i;
   Vector xa(n), xb(n), eb(n), nan_vec(n);

   // initialization
   Scalar zero(0.0);
   Scalar one(1.0);
   Scalar two(2.0);
   Scalar three(3.0);
   Scalar m1(double(m-1));
   Scalar ta = ti;
   for(i = 0; i < n; i++)
   {  nan_vec[i] = nan(zero);
      ef[i]      = zero;
      xa[i]      = xi[i];
      if( zero <= xi[i] )
         maxabs[i] = xi[i];
      else
         maxabs[i] = - xi[i];

   }
   nstep = 0;

   Scalar tb, step, lambda, axbi, a, r, root;
   while( ! (ta == tf) )
   {  // start with value suggested by error criteria
      step = scur;

      // check maximum
      if( smax <= step )
         step = smax;

      // check minimum
      minimum_step = step <= smin;
      if( minimum_step )
         step = smin;

      // check if near the end
      if( tf <= ta + step * three / two )
         tb = tf;
      else
         tb = ta + step;

      // try using this step size
      nstep++;
      method.step(ta, tb, xa, xb, eb);
      step = tb - ta;

      // check if this steps error estimate is ok
      ok = ! (hasnan(xb) || hasnan(eb));
      if( (! ok) && minimum_step )
      {  ef = nan_vec;
         return nan_vec;
      }

      // compute value of lambda for this step
      lambda = Scalar(10) * scur / step;
      for(i = 0; i < n; i++)
      {  if( zero <= xb[i] )
            axbi = xb[i];
         else
            axbi = - xb[i];
         a    = eabs[i] + erel * axbi;
         if( ! (eb[i] == zero) )
         {  r = ( a / eb[i] ) * step / (tf - ti);
            root = exp( log(r) / m1 );
            if( root <= lambda )
               lambda = root;
         }
      }
      if( ok && ( one <= lambda || step <= smin * three / two) )
      {  // this step is within error limits or
         // close to the minimum size
         ta = tb;
         for(i = 0; i < n; i++)
         {  xa[i] = xb[i];
            ef[i] = ef[i] + eb[i];
            if( zero <= xb[i] )
               axbi = xb[i];
            else
               axbi = - xb[i];
            if( axbi > maxabs[i] )
               maxabs[i] = axbi;
         }
      }
      if( ! ok )
      {  // decrease step an see if method will work this time
         scur = step / two;
      }
      else if( ! (ta == tf) )
      {  // step suggested by the error criteria is not used
         // on the last step because it may be very small.
         scur = lambda * step / two;
      }
   }
   return xa;
}

template <class Scalar, class Vector, class Method>
Vector OdeErrControl(
   Method          &method,
   const Scalar    &ti    ,
   const Scalar    &tf    ,
   const Vector    &xi    ,
   const Scalar    &smin  ,
   const Scalar    &smax  ,
   Scalar          &scur  ,
   const Vector    &eabs  ,
   const Scalar    &erel  ,
   Vector          &ef    )
{  Vector maxabs(xi.size());
   size_t nstep;
   return OdeErrControl(
   method, ti, tf, xi, smin, smax, scur, eabs, erel, ef, maxabs, nstep
   );
}

template <class Scalar, class Vector, class Method>
Vector OdeErrControl(
   Method          &method,
   const Scalar    &ti    ,
   const Scalar    &tf    ,
   const Vector    &xi    ,
   const Scalar    &smin  ,
   const Scalar    &smax  ,
   Scalar          &scur  ,
   const Vector    &eabs  ,
   const Scalar    &erel  ,
   Vector          &ef    ,
   Vector          &maxabs)
{  size_t nstep;
   return OdeErrControl(
   method, ti, tf, xi, smin, smax, scur, eabs, erel, ef, maxabs, nstep
   );
}

} // End CppAD namespace

# endif
