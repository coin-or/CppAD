# ifndef CPPAD_CORE_INTEGER_HPP
# define CPPAD_CORE_INTEGER_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
------------------------------------------------------------------------------
{xrst_begin Integer}

Convert From AD to Integer
##########################

Syntax
******
| *i* = ``Integer`` ( *x* )

Purpose
*******
Converts from an AD type to the corresponding integer value.

i
*
The result *i* has prototype

   ``int`` *i*

x
*

Real Types
==========
If the argument *x* has either of the following prototypes:

| |tab| ``const float`` & *x*
| |tab| ``const double`` & *x*

the fractional part is dropped to form the integer value.
For example, if *x* is 1.5, *i* is 1.
In general, if :math:`x \geq 0`, *i* is the
greatest integer less than or equal *x* .
If :math:`x \leq 0`, *i* is the
smallest integer greater than or equal *x* .

Complex Types
=============
If the argument *x* has either of the following prototypes:

| |tab| ``const std::complex<float>`` & *x*
| |tab| ``const std::complex<double>`` & *x*

The result *i* is given by

   *i* = ``Integer`` ( *x* . ``real`` ())

AD Types
========
If the argument *x* has either of the following prototypes:

| |tab| ``const AD`` < *Base* >               & *x*
| |tab| ``const VecAD`` < *Base* >:: ``reference &`` *x*

*Base* must support the ``Integer`` function and
the conversion has the same meaning as for *Base* .

Operation Sequence
******************
The result of this operation is not an
:ref:`glossary@AD of Base` object.
Thus it will not be recorded as part of an
AD of *Base*
:ref:`operation sequence<glossary@Operation@Sequence>` .

Example
*******
{xrst_toc_hidden
   example/general/integer.cpp
}
The file
:ref:`integer.cpp-name`
contains an example and test of this operation.

{xrst_end Integer}
------------------------------------------------------------------------------
*/


namespace CppAD {

   template <class Base>
   CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
   int Integer(const AD<Base> &x)
   {  return Integer(x.value_); }

   template <class Base>
   CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
   int Integer(const VecAD_reference<Base> &x)
   {  return Integer( x.ADBase() ); }
}
# endif
