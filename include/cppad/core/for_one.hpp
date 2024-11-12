# ifndef CPPAD_CORE_FOR_ONE_HPP
# define CPPAD_CORE_FOR_ONE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin ForOne}
{xrst_spell
   dy
}

First Order Partial Derivative: Driver Routine
##############################################

Syntax
******
| *dy* = *f* . ``ForOne`` ( *x* , *j* )

Purpose
*******
We use :math:`F : \B{R}^n \rightarrow \B{R}^m` to denote the
:ref:`glossary@AD Function` corresponding to *f* .
The syntax above sets *dy* to the
partial of :math:`F` with respect to :math:`x_j`; i.e.,

.. math::

   dy
   = \D{F}{ x_j } (x)
   = \left[
      \D{ F_0 }{ x_j } (x) , \cdots , \D{ F_{m-1} }{ x_j } (x)
   \right]

f
*
The object *f* has prototype

   ``ADFun`` < *Base* > *f*

Note that the :ref:`ADFun-name` object *f* is not ``const``
(see :ref:`ForOne@ForOne Uses Forward` below).

x
*
The argument *x* has prototype

   ``const`` *Vector* & *x*

(see :ref:`ForOne@Vector` below)
and its size
must be equal to *n* , the dimension of the
:ref:`fun_property@Domain` space for *f* .
It specifies
that point at which to evaluate the partial derivative.

j
*
The argument *j* has prototype

   ``size_t`` *j*

an is less than *n* ,
:ref:`fun_property@Domain` space for *f* .
It specifies the component of *F*
for which we are computing the partial derivative.

dy
**
The result *dy* has prototype

   *Vector* *dy*

(see :ref:`ForOne@Vector` below)
and its size is :math:`m`, the dimension of the
:ref:`fun_property@Range` space for *f* .
The value of *dy* is the partial of :math:`F` with respect to
:math:`x_j` evaluated at *x* ; i.e.,
for :math:`i = 0 , \ldots , m - 1`

.. math::

   dy[i] = \D{ F_i }{ x_j } ( x )

Vector
******
The type *Vector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
*Base* .
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.

ForOne Uses Forward
*******************
After each call to :ref:`Forward-name` ,
the object *f* contains the corresponding
:ref:`Taylor coefficients<glossary@Taylor Coefficient>` .
After a call to ``ForOne`` ,
the zero order Taylor coefficients correspond to
*f* . ``Forward`` (0, *x* )
and the other coefficients are unspecified.

Example
*******
{xrst_toc_hidden
   example/general/for_one.cpp
}
The routine
:ref:`ForOne<for_one.cpp-name>` is both an example and test.
It returns ``true`` , if it succeeds and ``false`` otherwise.

{xrst_end ForOne}
-----------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base, class RecBase>
template <class Vector>
Vector ADFun<Base,RecBase>::ForOne(const Vector &x, size_t j)
{  size_t j1;

   size_t n = Domain();
   size_t m = Range();

   // check Vector is Simple Vector class with Base type elements
   CheckSimpleVector<Base, Vector>();

   CPPAD_ASSERT_KNOWN(
      x.size() == n,
      "ForOne: Length of x not equal domain dimension for f"
   );
   CPPAD_ASSERT_KNOWN(
      j < n,
      "ForOne: the index j is not less than domain dimension for f"
   );

   // point at which we are evaluating the second partials
   Forward(0, x);

   // direction in which are are taking the derivative
   Vector dx(n);
   for(j1 = 0; j1 < n; j1++)
      dx[j1] = Base(0.0);
   dx[j] = Base(1.0);

   // dimension the return value
   Vector dy(m);

   // compute the return value
   dy = Forward(1, dx);

   return dy;
}

} // END CppAD namespace

# endif
