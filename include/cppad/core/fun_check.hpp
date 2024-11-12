# ifndef CPPAD_CORE_FUN_CHECK_HPP
# define CPPAD_CORE_FUN_CHECK_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin FunCheck}

Check an ADFun Sequence of Operations
#####################################

Syntax
******
| *ok* = ``FunCheck`` ( *f* , *g* , *x* , *r* , *a* )

See Also
********
:ref:`FunCheck-name`

Purpose
*******
We use :math:`F : \B{R}^n \rightarrow \B{R}^m` to denote the
:ref:`glossary@AD Function` corresponding to *f* .
We use :math:`G : \B{R}^n \rightarrow \B{R}^m` to denote the
function corresponding to the C++ function object *g* .
This routine check if

.. math::

   F(x) = G(x)

If :math:`F(x) \neq G(x)`, the
:ref:`operation sequence<glossary@Operation@Sequence>`
corresponding to *f* does not represents the algorithm used
by *g* to calculate values for :math:`G`
(see :ref:`FunCheck@Discussion` below).

f
*
The ``FunCheck`` argument *f* has prototype

   ``ADFun`` < *Base* > *f*

Note that the :ref:`ADFun-name` object *f* is not ``const``
(see :ref:`Forward<FunCheck@FunCheck Uses Forward>` below).

g
*
The ``FunCheck`` argument *g* has prototype

   *Fun* & *g*

( *Fun* is defined the properties of *g* ).
The C++ function object *g* supports the syntax

   *y* = *g* ( *x* )

which computes :math:`y = G(x)`.

x
=
The *g* argument *x* has prototype

   ``const`` *Vector* & *x*

(see :ref:`FunCheck@Vector` below)
and its size
must be equal to *n* , the dimension of the
:ref:`fun_property@Domain` space for *f* .

y
*
The *g* result *y* has prototype

   *Vector* *y*

and its value is :math:`G(x)`.
The size of *y*
is equal to *m* , the dimension of the
:ref:`fun_property@Range` space for *f* .

x
*
The ``FunCheck`` argument *x* has prototype

   ``const`` *Vector* & *x*

and its size
must be equal to *n* , the dimension of the
:ref:`fun_property@Domain` space for *f* .
This specifies that point at which to compare the values
calculated by *f* and *G* .

r
*
The ``FunCheck`` argument *r* has prototype

   ``const`` *Base* & *r*

It specifies the relative error the element by element
comparison of the value of :math:`F(x)` and :math:`G(x)`.

a
*
The ``FunCheck`` argument *a* has prototype

   ``const`` *Base* & *a*

It specifies the absolute error the element by element
comparison of the value of :math:`F(x)` and :math:`G(x)`.

ok
**
The ``FunCheck`` result *ok* has prototype

   ``bool`` *ok*

It is true, if for :math:`i = 0 , \ldots , m-1`
either the relative error bound is satisfied

.. math::

   | F_i (x) - G_i (x) |
   \leq
   r ( | F_i (x) | + | G_i (x) | )

or the absolute error bound is satisfied

.. math::

   | F_i (x) - G_i (x) | \leq a

It is false if for some :math:`(i, j)` neither
of these bounds is satisfied.

Vector
******
The type *Vector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
*Base* .
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.

FunCheck Uses Forward
*********************
After each call to :ref:`Forward-name` ,
the object *f* contains the corresponding
:ref:`Taylor coefficients<glossary@Taylor Coefficient>` .
After ``FunCheck`` ,
the previous calls to :ref:`Forward-name` are undefined.

Discussion
**********
Suppose that the algorithm corresponding to *g* contains

| |tab| ``if`` ( *x*  >= 0 )
| |tab| |tab| *y* = ``exp`` ( *x* )
| |tab| ``else``
| |tab| |tab| *y* = ``exp`` ( ``-`` *x* )

where *x* and *y* are ``AD<double>`` objects.
It follows that the
AD of ``double`` :ref:`operation sequence<glossary@Operation@Sequence>`
depends on the value of *x* .
If the sequence of operations stored in *f* corresponds to
*g* with :math:`x \geq 0`,
the function values computed using *f* when :math:`x < 0`
will not agree with the function values computed by :math:`g`.
This is because the operation sequence corresponding to *g* changed
(and hence the object *f* does not represent the function
:math:`G` for this value of *x* ).
In this case, you probably want to re-tape the calculations
performed by *g* with the
:ref:`independent variables<glossary@Tape@Independent Variable>`
equal to the values in *x*
(so AD operation sequence properly represents the algorithm
for this value of independent variables).

Example
*******
{xrst_toc_hidden
   example/general/fun_check.cpp
}
The file
:ref:`fun_check.cpp-name`
contains an example and test of this function.

{xrst_end FunCheck}
---------------------------------------------------------------------------
*/

namespace CppAD {
   template <class Base, class RecBase, class Fun, class Vector>
   bool FunCheck(
      ADFun<Base, RecBase>  &f ,
      Fun                   &g ,
      const Vector          &x ,
      const Base            &r ,
      const Base            &a )
   {  bool ok = true;

      size_t m   = f.Range();
      Vector yf  = f.Forward(0, x);
      Vector yg  = g(x);

      size_t i;
      for(i = 0; i < m; i++)
         ok  &= NearEqual(yf[i], yg[i], r, a);
      return ok;
   }
}

# endif
