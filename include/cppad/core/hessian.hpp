# ifndef CPPAD_CORE_HESSIAN_HPP
# define CPPAD_CORE_HESSIAN_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin Hessian}

Hessian: Easy Driver
####################

Syntax
******
| *hes* = *f* . ``Hessian`` ( *x* , *w* )
| *hes* = *f* . ``Hessian`` ( *x* , *l* )

Purpose
*******
We use :math:`F : \B{R}^n \rightarrow \B{R}^m` to denote the
:ref:`glossary@AD Function` corresponding to *f* .
The syntax above sets *hes* to the Hessian
The syntax above sets *h* to the Hessian

.. math::

   hes = \dpow{2}{x} \sum_{i=1}^m w_i F_i (x)

The routine :ref:`sparse_hessian-name` may be faster in the case
where the Hessian is sparse.

f
*
The object *f* has prototype

   ``ADFun`` < *Base* > *f*

Note that the :ref:`ADFun-name` object *f* is not ``const``
(see :ref:`Hessian@Hessian Uses Forward` below).

x
*
The argument *x* has prototype

   ``const`` *Vector* & *x*

(see :ref:`Hessian@Vector` below)
and its size
must be equal to *n* , the dimension of the
:ref:`fun_property@Domain` space for *f* .
It specifies
that point at which to evaluate the Hessian.

l
*
If the argument *l* is present, it has prototype

   ``size_t`` *l*

and is less than *m* , the dimension of the
:ref:`fun_property@Range` space for *f* .
It specifies the component of *F*
for which we are evaluating the Hessian.
To be specific, in the case where the argument *l* is present,

.. math::

   w_i = \left\{ \begin{array}{ll}
      1 & i = l \\
      0 & {\rm otherwise}
   \end{array} \right.

w
*
If the argument *w* is present, it has prototype

   ``const`` *Vector* & *w*

and size :math:`m`.
It specifies the value of :math:`w_i` in the expression
for *h* .

hes
***
The result *hes* has prototype

   *Vector* *hes*

(see :ref:`Hessian@Vector` below)
and its size is :math:`n * n`.
For :math:`j = 0 , \ldots , n - 1`
and :math:`\ell = 0 , \ldots , n - 1`

.. math::

   hes [ j * n + \ell ] = \DD{ w^{\rm T} F }{ x_j }{ x_\ell } ( x )

Vector
******
The type *Vector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
*Base* .
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.

Hessian Uses Forward
********************
After each call to :ref:`Forward-name` ,
the object *f* contains the corresponding
:ref:`Taylor coefficients<glossary@Taylor Coefficient>` .
After a call to ``Hessian`` ,
the zero order Taylor coefficients correspond to
*f* . ``Forward`` (0, *x* )
and the other coefficients are unspecified.

Example
*******
{xrst_toc_hidden
   example/general/hessian.cpp
   example/general/hes_lagrangian.cpp
}
The routines
:ref:`hessian.cpp-name` and
:ref:`hes_lagrangian.cpp-name`
are examples and tests of ``Hessian`` .
They return ``true`` , if they succeed and ``false`` otherwise.

{xrst_end Hessian}
-----------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base, class RecBase>
template <class Vector>
Vector ADFun<Base,RecBase>::Hessian(const Vector &x, size_t l)
{  size_t i, m = Range();
   CPPAD_ASSERT_KNOWN(
      l < m,
      "Hessian: index i is not less than range dimension for f"
   );

   Vector w(m);
   for(i = 0; i < m; i++)
      w[i] = Base(0.0);
   w[l] = Base(1.0);

   return Hessian(x, w);
}


template <class Base, class RecBase>
template <class Vector>
Vector ADFun<Base,RecBase>::Hessian(const Vector &x, const Vector &w)
{  size_t j;
   size_t k;

   size_t n = Domain();

   // check Vector is Simple Vector class with Base type elements
   CheckSimpleVector<Base, Vector>();

   CPPAD_ASSERT_KNOWN(
      size_t(x.size()) == n,
      "Hessian: length of x not equal domain dimension for f"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(w.size()) == Range(),
      "Hessian: length of w not equal range dimension for f"
   );

   // point at which we are evaluating the Hessian
   Forward(0, x);

   // define the return value
   Vector hes(n * n);

   // direction vector for calls to forward
   Vector u(n);
   for(j = 0; j < n; j++)
      u[j] = Base(0.0);


   // location for return values from Reverse
   Vector ddw(n * 2);

   // loop over forward directions
   for(j = 0; j < n; j++)
   {  // evaluate partials of entire function w.r.t. j-th coordinate
      u[j] = Base(1.0);
      Forward(1, u);
      u[j] = Base(0.0);

      // evaluate derivative of partial corresponding to F_i
      ddw = Reverse(2, w);

      // return desired components
      for(k = 0; k < n; k++)
         hes[k * n + j] = ddw[k * 2 + 1];
   }

   return hes;
}

} // END CppAD namespace

# endif
