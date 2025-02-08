# ifndef CPPAD_CORE_REV_ONE_HPP
# define CPPAD_CORE_REV_ONE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin RevOne}
{xrst_spell
   dw
}

First Order Derivative: Driver Routine
######################################

Syntax
******
| *dw* = *f* . ``RevOne`` ( *x* , *i* )

Purpose
*******
We use :math:`F : \B{R}^n \rightarrow \B{R}^m` to denote the
:ref:`glossary@AD Function` corresponding to *f* .
The syntax above sets *dw* to the
derivative of :math:`F_i` with respect to :math:`x`; i.e.,

.. math::

   dw =
   F_i^{(1)} (x)
   = \left[
      \D{ F_i }{ x_0 } (x) , \cdots , \D{ F_i }{ x_{n-1} } (x)
   \right]

f
*
The object *f* has prototype

   ``ADFun`` < *Base* > *f*

Note that the :ref:`ADFun-name` object *f* is not ``const``
(see :ref:`RevOne@RevOne Uses Forward` below).

x
*
The argument *x* has prototype

   ``const`` *Vector* & *x*

(see :ref:`RevOne@Vector` below)
and its size
must be equal to *n* , the dimension of the
:ref:`fun_property@Domain` space for *f* .
It specifies
that point at which to evaluate the derivative.

i
*
The index *i* has prototype

   ``size_t`` *i*

and is less than :math:`m`, the dimension of the
:ref:`fun_property@Range` space for *f* .
It specifies the
component of :math:`F` that we are computing the derivative of.

dw
**
The result *dw* has prototype

   *Vector* *dw*

(see :ref:`RevOne@Vector` below)
and its size is *n* , the dimension of the
:ref:`fun_property@Domain` space for *f* .
The value of *dw* is the derivative of :math:`F_i`
evaluated at *x* ; i.e.,
for :math:`j = 0 , \ldots , n - 1`

.. math::

   dw[ j ] = \D{ F_i }{ x_j } ( x )

Vector
******
The type *Vector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
*Base* .
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.

RevOne Uses Forward
*******************
After each call to :ref:`Forward-name` ,
the object *f* contains the corresponding
:ref:`Taylor coefficients<glossary@Taylor Coefficient>` .
After a call to ``RevOne`` ,
the zero order Taylor coefficients correspond to
*f* . ``Forward`` (0, *x* )
and the other coefficients are unspecified.

Example
*******
{xrst_toc_hidden
   example/general/rev_one.cpp
}
The routine
:ref:`RevOne<rev_one.cpp-name>` is both an example and test.
It returns ``true`` , if it succeeds and ``false`` otherwise.

{xrst_end RevOne}
-----------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base, class RecBase>
template <class Vector>
Vector ADFun<Base,RecBase>::RevOne(const Vector  &x, size_t i)
{  size_t i1;

   size_t n = Domain();
   size_t m = Range();

   // check Vector is Simple Vector class with Base type elements
   CheckSimpleVector<Base, Vector>();

   CPPAD_ASSERT_KNOWN(
      x.size() == n,
      "RevOne: Length of x not equal domain dimension for f"
   );
   CPPAD_ASSERT_KNOWN(
      i < m,
      "RevOne: the index i is not less than range dimension for f"
   );

   // point at which we are evaluating the derivative
   Forward(0, x);

   // component which are are taking the derivative of
   Vector w(m);
   for(i1 = 0; i1 < m; i1++)
      w[i1] = 0.;
   w[i] = Base(1.0);

   // dimension the return value
   Vector dw(n);

   // compute the return value
   dw = Reverse(1, w);

   return dw;
}

} // END CppAD namespace

# endif
