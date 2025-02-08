# ifndef CPPAD_CORE_COMPOUND_ASSIGN_HPP
# define CPPAD_CORE_COMPOUND_ASSIGN_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
-------------------------------------------------------------------------------
{xrst_begin compound_assign}
{xrst_spell
   div
}

AD Compound Assignment Operators
################################

Syntax
******
| *x* *Op* *y*

Purpose
*******
Performs compound assignment operations
where either *x* has type
``AD`` < *Base* > .

Op
**
The operator *Op* is one of the following

.. csv-table::
   :widths: auto

   **Op**,**Meaning**
   ``+=``,*x* is assigned *x* plus *y*
   ``-=``,*x* is assigned *x* minus *y*
   ``*=``,*x* is assigned *x* times *y*
   ``/=``,*x* is assigned *x* divided by *y*

Base
****
The type *Base* is determined by the operand *x* .

x
*
The operand *x* has the following prototype

   ``AD`` < *Base* > & *x*

y
*
The operand *y* has the following prototype

   ``const`` *Type* & *y*

where *Type* is
``VecAD`` < *Base* >:: ``reference`` ,
``AD`` < *Base* > ,
*Base* , or
``double`` .

Result
******
The result of this assignment
can be used as a reference to *x* .
For example, if *z* has the following type

   ``AD`` < *Base* > *z*

then the syntax

   *z* = *x* += *y*

will compute *x* plus *y*
and then assign this value to both *x* and *z* .

Operation Sequence
******************
This is an :ref:`atomic_base<glossary@Operation@Atomic>`
:ref:`glossary@AD of Base` operation
and hence it is part of the current
AD of *Base*
:ref:`operation sequence<glossary@Operation@Sequence>` .
{xrst_toc_hidden
   example/general/add_eq.cpp
   example/general/sub_eq.cpp
   example/general/mul_eq.cpp
   example/general/div_eq.cpp
}

Example
*******
The following files contain examples and tests of these functions.
Each test returns true if it succeeds and false otherwise.

.. csv-table::
   :widths: auto

   add_eq.cpp,:ref:`add_eq.cpp-title`
   sub_eq.cpp,:ref:`sub_eq.cpp-title`
   mul_eq.cpp,:ref:`mul_eq.cpp-title`
   div_eq.cpp,:ref:`div_eq.cpp-title`

Derivative
**********
If :math:`f` and :math:`g` are
:ref:`Base functions<glossary@Base Function>`

Addition
========

.. math::

   \D{[ f(x) + g(x) ]}{x} = \D{f(x)}{x} + \D{g(x)}{x}

Subtraction
===========

.. math::

   \D{[ f(x) - g(x) ]}{x} = \D{f(x)}{x} - \D{g(x)}{x}

Multiplication
==============

.. math::

   \D{[ f(x) * g(x) ]}{x} = g(x) * \D{f(x)}{x} + f(x) * \D{g(x)}{x}

Division
========

.. math::

   \D{[ f(x) / g(x) ]}{x} =
      [1/g(x)] * \D{f(x)}{x} - [f(x)/g(x)^2] * \D{g(x)}{x}

{xrst_end compound_assign}
-----------------------------------------------------------------------------
*/
# include <cppad/core/add_eq.hpp>
# include <cppad/core/sub_eq.hpp>
# include <cppad/core/mul_eq.hpp>
# include <cppad/core/div_eq.hpp>

# endif
