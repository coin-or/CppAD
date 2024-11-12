# ifndef CPPAD_CORE_COND_EXP_HPP
# define CPPAD_CORE_COND_EXP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
-------------------------------------------------------------------------------
{xrst_begin CondExp}
{xrst_spell
   ge
}

AD Conditional Expressions
##########################

Syntax
******
| *result* = ``CondExp`` *Rel* ( *left* , *right* , *if_true* , *if_false* )

Purpose
*******
Record,
as part of an AD of *Base*
:ref:`operation sequence<glossary@Operation@Sequence>` ,
the conditional result

| |tab| ``if`` ( *left* *Cop* *right*  )
| |tab| |tab| *result* = *if_true*
| |tab| ``else``
| |tab| |tab| *result* = *if_false*

The relational *Rel* and comparison operator *Cop*
above have the following correspondence:

.. csv-table::

   *Rel* , ``Lt`` , ``Le`` , ``Eq`` , ``Ge`` , ``Gt``
   *Cop* ,  <     , <=     , ==     , >=     , >

If *f* is the :ref:`ADFun-name` object corresponding to the
AD operation sequence,
the assignment choice for *result*
in an AD conditional expression is made each time
:ref:`f.Forward<Forward-name>` is used to evaluate the zero order Taylor
coefficients with new values for the
:ref:`independent variables<glossary@Tape@Independent Variable>` .
This is in contrast to the :ref:`AD comparison operators<Compare-name>`
which are boolean valued and not included in the AD operation sequence.

Rel
***
In the syntax above, the relation *Rel* represents one of the following
two characters: ``Lt`` , ``Le`` , ``Eq`` , ``Ge`` , ``Gt`` .
As in the table above,
*Rel* determines which comparison operator *Cop* is used
when comparing *left* and *right* .

Type
****
These functions are defined in the CppAD namespace for arguments of
*Type* is *Base* or ``AD`` < *Base* > .
Note that all four arguments,
*left* , *right*, *if_true* , *if_false* ,  must have the same type.

left
****
The argument *left* has prototype

   ``const`` *Type* & *left*

It specifies the value for the left side of the comparison operator.

right
*****
The argument *right* has prototype

   ``const`` *Type* & *right*

It specifies the value for the right side of the comparison operator.

if_true
*******
The argument *if_true* has prototype

   ``const`` *Type* & *if_true*

It specifies the return value if the result of the comparison is true.

if_false
********
The argument *if_false* has prototype

   ``const`` *Type* & *if_false*

It specifies the return value if the result of the comparison is false.

result
******
The *result* has prototype

   *Type* & *if_false*

Optimize
********
The :ref:`optimize-name` method will optimize conditional expressions
in the following way:
During :ref:`zero order forward mode<forward_zero-name>` ,
once the value of the *left* and *right* have been determined,
it is known if the true or false case is required.
From this point on, values corresponding to the case that is not required
are not computed.
This optimization is done for the rest of zero order forward mode
as well as forward and reverse derivatives calculations.

Deprecate 2005-08-07
********************
Previous versions of CppAD used

   ``CondExp`` ( *flag* , *if_true* , *if_false* )

for the same meaning as

   ``CondExpGt`` ( *flag* , *Type* (0), *if_true* , *if_false* )

Use of ``CondExp`` is deprecated, but continues to be supported.

Operation Sequence
******************
This is an AD of *Base*
:ref:`atomic operation<glossary@Operation@Atomic>`
and hence is part of the current
AD of *Base*
:ref:`operation sequence<glossary@Operation@Sequence>` .

Example
*******

Test
****
{xrst_toc_hidden
   example/general/cond_exp.cpp
}
The file
:ref:`cond_exp.cpp-name`
contains an example and test of this function.

Atan2
*****
The following implementation of the
AD :ref:`atan2-name` function is a more complex
example of using conditional expressions:
{xrst_literal
   include/cppad/core/atan2.hpp
   BEGIN CondExp
   // END CondExp
}

{xrst_end CondExp}
-------------------------------------------------------------------------------
*/
//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
AD<Base> CondExpOp(
   enum  CompareOp cop       ,
   const AD<Base> &left      ,
   const AD<Base> &right     ,
   const AD<Base> &if_true   ,
   const AD<Base> &if_false  )
{
   AD<Base> result;
   CPPAD_ASSERT_UNKNOWN( Parameter(result) );

   // check first case where do not need to tape
   if( IdenticalCon(left) && IdenticalCon(right) )
   {  result = CondExpOp(
         cop, left.value_, right.value_, if_true.value_, if_false.value_
      );
      return result;
   }

   // must use CondExp in case Base is an AD type and recording
   result.value_ = CondExpOp(cop,
      left.value_, right.value_, if_true.value_, if_false.value_);

   local::ADTape<Base> *tape = AD<Base>::tape_ptr();

   // add this operation to the tape
   if( tape != nullptr ) tape->Rec_.cond_exp(
         tape->id_, cop, result, left, right, if_true, if_false
   );

   return result;
}

// ------------ CondExpOp(left, right, if_true, if_false) ----------------

# define CPPAD_COND_EXP(Name)                                        \
   template <class Base>                                           \
   CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION                           \
   AD<Base> CondExp##Name(                                         \
      const AD<Base> &left      ,                                \
      const AD<Base> &right     ,                                \
      const AD<Base> &if_true   ,                                \
      const AD<Base> &if_false  )                                \
   {                                                               \
      return CondExpOp(Compare##Name,                            \
         left, right, if_true, if_false);                      \
   }

// AD<Base>
CPPAD_COND_EXP(Lt)
CPPAD_COND_EXP(Le)
CPPAD_COND_EXP(Eq)
CPPAD_COND_EXP(Ge)
CPPAD_COND_EXP(Gt)
template <class Base>
CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
AD<Base> CondExp(
   const AD<Base> &flag      ,
   const AD<Base> &if_true   ,
   const AD<Base> &if_false  )
{
   return CondExpOp(CompareGt, flag, AD<Base>(0), if_true, if_false);
}

# undef CPPAD_COND_EXP
} // END CppAD namespace

# endif
