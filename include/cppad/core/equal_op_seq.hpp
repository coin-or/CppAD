# ifndef CPPAD_CORE_EQUAL_OP_SEQ_HPP
# define CPPAD_CORE_EQUAL_OP_SEQ_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
------------------------------------------------------------------------------
{xrst_begin EqualOpSeq}

Check if Two Value are Identically Equal
########################################

Syntax
******
| *b* = ``EqualOpSeq`` ( *x* , *y* )

Purpose
*******
Determine if two *x* and *y* are identically equal; i.e.,
not only is *x* == *y* true, but
if they are :ref:`variables<glossary@Variable>` ,
they correspond have the same
:ref:`operation sequence<glossary@Operation@Sequence>` .

Motivation
**********
Sometimes it is useful to cache information
and only recalculate when a function's arguments change.
In the case of AD variables,
it may be important not only when the argument values are equal,
but when they are related to the
:ref:`independent variables<glossary@Tape@Independent Variable>`
by the same operation sequence.
After the assignment

   *y* = *x*

these two AD objects would not only have equal values,
but would also correspond to the same operation sequence.

x
*
The argument *x* has prototype

   ``const AD`` < *Base* > & *x*

y
*
The argument *y* has prototype

   ``const AD`` < *Base* > & *y*

b
*
The result *b* has prototype

   ``bool`` *b*

The result is true if and only if one of the following cases holds:

#. Both *x* and *y* are variables
   and correspond to the same operation sequence.
#. Both *x* and *y* are parameters,
   *Base* is an AD type,
   and ``EqualOpSeq`` ( ``Value`` ( *x* ) , ``Value`` ( *y* ) ) is true.
#. Both *x* and *y* are parameters,
   *Base* is not an AD type,
   and *x* == *y* is true.

Example
*******
{xrst_toc_hidden
   example/general/equal_op_seq.cpp
}
The file
:ref:`equal_op_seq.cpp-name`
contains an example and test of ``EqualOpSeq`` .

{xrst_end EqualOpSeq}
------------------------------------------------------------------------------
*/


namespace CppAD {
   template <class Base>
   CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
   bool EqualOpSeq(const AD<Base> &x, const AD<Base> &y)
   {
      if( Parameter(x) )
      {  if( Parameter(y) )
            return EqualOpSeq(x.value_, y.value_);
         else
            return false;
      }
      else if( Parameter(y) )
         return false;

      return (x.taddr_ == y.taddr_);
   }

}

# endif
