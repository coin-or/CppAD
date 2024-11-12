# ifndef CPPAD_CORE_UNARY_PLUS_HPP
# define CPPAD_CORE_UNARY_PLUS_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin unary_plus}

AD Unary Plus Operator
######################

Syntax
******
| *y* = + *x*

Purpose
*******
Performs the unary plus operation
(the result *y* is equal to the operand *x* ).

x
*
The operand *x* has one of the following prototypes

| |tab| ``const AD`` < *Base* >               & *x*
| |tab| ``const VecAD`` < *Base* >:: ``reference &`` *x*

y
*
The result *y* has type

   ``AD`` < *Base* > *y*

It is equal to the operand *x* .

Operation Sequence
******************
This is an AD of *Base*
:ref:`atomic operation<glossary@Operation@Atomic>`
and hence is part of the current
AD of *Base*
:ref:`operation sequence<glossary@Operation@Sequence>` .

Derivative
**********
If :math:`f` is a
:ref:`glossary@Base Function` ,

.. math::

   \D{[ + f(x) ]}{x} = \D{f(x)}{x}

Example
*******
{xrst_toc_hidden
   example/general/unary_plus.cpp
}
The file
:ref:`unary_plus.cpp-name`
contains an example and test of this operation.

{xrst_end unary_plus}
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
AD<Base> AD<Base>::operator + (void) const
{  AD<Base> result(*this);

   return result;
}


template <class Base>
AD<Base> operator + (const VecAD_reference<Base> &right)
{  return right.ADBase(); }

}
//  END CppAD namespace


# endif
