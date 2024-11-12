# ifndef CPPAD_CORE_NEAR_EQUAL_EXT_HPP
# define CPPAD_CORE_NEAR_EQUAL_EXT_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin near_equal_ext}

Compare AD and Base Objects for Nearly Equal
############################################

Syntax
******
| *b* = ``NearEqual`` ( *x* , *y* , *r* , *a* )

Purpose
*******
The routine :ref:`NearEqual-name` determines if two objects of
the same type are nearly.
This routine is extended to the case where one object can have type
*Type* while the other can have type
``AD`` < *Type* > or
``AD< std::complex<`` *Type* > > .

x
*
The arguments *x*
has one of the following possible prototypes:

| |tab| ``const`` *Type* & *x*
| |tab| ``const AD`` < *Type* >                 & *x*
| |tab| ``const AD< std::complex<`` *Type* > > & *x*

y
*
The arguments *y*
has one of the following possible prototypes:

| |tab| ``const`` *Type* & *y*
| |tab| ``const AD`` < *Type* >                 & *y*
| |tab| ``const AD< std::complex<`` *Type* > > & *x*

r
*
The relative error criteria *r* has prototype

   ``const`` *Type* & *r*

It must be greater than or equal to zero.
The relative error condition is defined as:

.. math::

   \frac{ | x - y | } { |x| + |y| } \leq r

a
*
The absolute error criteria *a* has prototype

   ``const`` *Type* & *a*

It must be greater than or equal to zero.
The absolute error condition is defined as:

.. math::

   | x - y | \leq a

b
*
The return value *b* has prototype

   ``bool`` *b*

If either *x* or *y* is infinite or not a number,
the return value is false.
Otherwise, if either the relative or absolute error
condition (defined above) is satisfied, the return value is true.
Otherwise, the return value is false.

Type
****
The type *Type* must be a
:ref:`NumericType-name` .
The routine :ref:`CheckNumericType-name` will generate
an error message if this is not the case.
If *a* and *b* have type *Type* ,
the following operation must be defined

.. list-table::
   :widths: auto

   * - **Operation**
     - **Description**
   * - *a* <= *b*
     - less that or equal operator (returns a ``bool`` object)

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
   example/general/near_equal_ext.cpp
}
The file :ref:`near_equal_ext.cpp-name` contains an example
and test of this extension of :ref:`NearEqual-name` .
It return true if it succeeds and false otherwise.

{xrst_end near_equal_ext}

*/
// BEGIN CppAD namespace
namespace CppAD {
// ------------------------------------------------------------------------

// fold into base type and then use <cppad/near_equal.hpp>
template <class Base>
CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
bool NearEqual(
const AD<Base> &x, const AD<Base> &y, const Base &r, const Base &a)
{  return NearEqual(x.value_, y.value_, r, a);
}

template <class Base>
CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
bool NearEqual(
const Base &x, const AD<Base> &y, const Base &r, const Base &a)
{  return NearEqual(x, y.value_, r, a);
}

template <class Base>
CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
bool NearEqual(
const AD<Base> &x, const Base &y, const Base &r, const Base &a)
{  return NearEqual(x.value_, y, r, a);
}

// fold into AD type and then use cases above
template <class Base>
CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
bool NearEqual(
   const VecAD_reference<Base> &x, const VecAD_reference<Base> &y,
   const Base &r, const Base &a)
{  return NearEqual(x.ADBase(), y.ADBase(), r, a);
}
template <class Base>
CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
bool NearEqual(const VecAD_reference<Base> &x, const AD<Base> &y,
   const Base &r, const Base &a)
{  return NearEqual(x.ADBase(), y, r, a);
}
template <class Base>
CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
bool NearEqual(const VecAD_reference<Base> &x, const Base &y,
   const Base &r, const Base &a)
{  return NearEqual(x.ADBase(), y, r, a);
}
template <class Base>
CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
bool NearEqual(const AD<Base> &x, const VecAD_reference<Base> &y,
   const Base &r, const Base &a)
{  return NearEqual(x, y.ADBase(), r, a);
}
template <class Base>
CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
bool NearEqual(const Base &x, const VecAD_reference<Base> &y,
   const Base &r, const Base &a)
{  return NearEqual(x, y.ADBase(), r, a);
}

} // END CppAD namespace

# endif
