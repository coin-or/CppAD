# ifndef CPPAD_CORE_BASE_COND_EXP_HPP
# define CPPAD_CORE_BASE_COND_EXP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin base_cond_exp}
{xrst_spell
   ge
}

Base Type Requirements for Conditional Expressions
##################################################

Purpose
*******
These definitions are required by the user's code to support the
``AD`` < *Base* > type for :ref:`CondExp-name` operations:

CompareOp
*********
The following ``enum`` type is used in the specifications below:
::

   namespace CppAD {
   // The conditional expression operator enum type
   enum CompareOp
   {  CompareLt, // less than
   CompareLe, // less than or equal
   CompareEq, // equal
   CompareGe, // greater than or equal
   CompareGt, // greater than
   CompareNe  // not equal
   };
   }

CondExpTemplate
***************
The type *Base* must support the syntax

| |tab| *result* = ``CppAD::CondExpOp`` (
| |tab| |tab| *cop* , *left* , *right* , *exp_if_true* , *exp_if_false*
| |tab| )

which computes implements the corresponding :ref:`CondExp-name`
function when the result has prototype

   *Base* *result*

The argument *cop* has prototype

   ``enum CppAD::CompareOp`` *cop*

The other arguments have the prototype

| |tab| ``const`` *Base* & *left*
| |tab| ``const`` *Base* & *right*
| |tab| ``const`` *Base* & *exp_if_true*
| |tab| ``const`` *Base* & *exp_if_false*

Ordered Type
============
If *Base* is a relatively simple type
that supports
``<`` , ``<=`` , ``==`` , ``>=`` , and ``>`` operators
its ``CondExpOp`` function can be defined by

| ``namespace CppAD`` {
| |tab| ``inline`` *Base* ``CondExpOp`` (
| |tab| ``enum CppAD::CompareOp`` *cop*             ,
| |tab| ``const`` *Base* & *left*           ,
| |tab| ``const`` *Base* & *right*          ,
| |tab| ``const`` *Base* & *exp_if_true*    ,
| |tab| ``const`` *Base* & *exp_if_false*   )
| |tab| { ``return CondExpTemplate`` (
| |tab| |tab| |tab| ``cop`` , ``left`` , ``right`` , ``trueCase`` , ``falseCase`` );
| |tab| }
| }

For example, see
:ref:`double CondExpOp<base_alloc.hpp@CondExpOp>` .
For an example of and implementation of ``CondExpOp`` with
a more involved *Base* type see
:ref:`adolc CondExpOp<base_adolc.hpp@CondExpOp>` .

Not Ordered
===========
If the type *Base* does not support ordering; i.e., does not support
the ``<`` , ``<=`` , ``>=`` , or ``>`` operator they should be defined
as resulting in error when used; e.g.

| ``namespace CppAD`` {
| |tab| ``inline bool operator<`` (
| |tab| ``const`` *Base* & *left*          ,
| |tab| ``const`` *Base* & *right*         )
| |tab| {  // ``attempt to use < operator with`` *Base* ``arguments``
| |tab| |tab| ``assert`` (0);
| |tab| |tab| ``return`` false;
| |tab| }
| }

Using the ``CondExpOp`` function does should also be defined a an error; e.g.,

| ``namespace CppAD`` {
| |tab| ``inline`` *Base* ``CondExpOp`` (
| |tab| ``enum CompareOp`` *cop*            ,
| |tab| ``const`` *Base* & *left*          ,
| |tab| ``const`` *Base* & *right*         ,
| |tab| ``const`` *Base* & *exp_if_true*   ,
| |tab| ``const`` *Base* & *exp_if_false*  )
| |tab| {  // ``attempt to use CondExp with a`` *Base* ``argument``
| |tab| |tab| ``assert`` (0);
| |tab| |tab| ``return`` *Base* (0);
| |tab| }
| }

For example, see
:ref:`complex CondExpOp<base_complex.hpp@CondExpOp>` .

CondExpRel
**********
The macro invocation

   ``CPPAD_COND_EXP_REL`` ( *Base* )

uses ``CondExpOp`` above to define the following functions

| |tab| ``CondExpLt`` ( *left* , *right* , *exp_if_true* , *exp_if_false* )
| |tab| ``CondExpLe`` ( *left* , *right* , *exp_if_true* , *exp_if_false* )
| |tab| ``CondExpEq`` ( *left* , *right* , *exp_if_true* , *exp_if_false* )
| |tab| ``CondExpGe`` ( *left* , *right* , *exp_if_true* , *exp_if_false* )
| |tab| ``CondExpGt`` ( *left* , *right* , *exp_if_true* , *exp_if_false* )

where the arguments have type *Base* .
This should be done inside of the CppAD namespace.
For example, see
:ref:`base_alloc<base_alloc.hpp@CondExpRel>` .

{xrst_end base_cond_exp}
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

/*!
\file base_cond_exp.hpp
CondExp operations that aid in meeting Base type requirements.
*/

/*!
\def CPPAD_COND_EXP_BASE_REL(Type, Rel, Op)
This macro defines the operation
\verbatim
   CondExpRel(left, right, exp_if_true, exp_if_false)
\endverbatim
The argument Type is the Base type for this base require operation.
The argument Rel is one of Lt, Le, Eq, Ge, Gt.
The argument Op is the corresponding CompareOp value.
*/
# define CPPAD_COND_EXP_BASE_REL(Type, Rel, Op)       \
   inline Type CondExp##Rel(                        \
      const Type& left      ,                     \
      const Type& right     ,                     \
      const Type& exp_if_true  ,                  \
      const Type& exp_if_false )                  \
   {  return CondExpOp(Op, left, right, exp_if_true, exp_if_false); \
   }

/*!
\def CPPAD_COND_EXP_REL(Type)
The macro defines the operations
\verbatim
   CondExpLt(left, right, exp_if_true, exp_if_false)
   CondExpLe(left, right, exp_if_true, exp_if_false)
   CondExpEq(left, right, exp_if_true, exp_if_false)
   CondExpGe(left, right, exp_if_true, exp_if_false)
   CondExpGt(left, right, exp_if_true, exp_if_false)
\endverbatim
The argument Type is the Base type for this base require operation.
*/
# define CPPAD_COND_EXP_REL(Type)                     \
   CPPAD_COND_EXP_BASE_REL(Type, Lt, CompareLt)     \
   CPPAD_COND_EXP_BASE_REL(Type, Le, CompareLe)     \
   CPPAD_COND_EXP_BASE_REL(Type, Eq, CompareEq)     \
   CPPAD_COND_EXP_BASE_REL(Type, Ge, CompareGe)     \
   CPPAD_COND_EXP_BASE_REL(Type, Gt, CompareGt)

/*!
Template function to implement Conditional Expressions for simple types
that have comparison operators.

\tparam CompareType
is the type of the left and right operands to the comparison operator.

\tparam ResultType
is the type of the result, which is the same as CompareType except
during forward and reverse mode sparese calculations.

\param cop
specifices which comparison to use; i.e.,
$code <$$,
$code <=$$,
$code ==$$,
$code >=$$,
$code >$$, or
$code !=$$.

\param left
is the left operand to the comparison operator.

\param right
is the right operand to the comparison operator.

\param exp_if_true
is the return value is the comparison results in true.

\param exp_if_false
is the return value is the comparison results in false.

\return
see exp_if_true and exp_if_false above.
*/
template <class CompareType, class ResultType>
ResultType CondExpTemplate(
   enum  CompareOp            cop          ,
   const CompareType&         left         ,
   const CompareType&         right        ,
   const ResultType&          exp_if_true  ,
   const ResultType&          exp_if_false )
{  ResultType returnValue;
   switch( cop )
   {
      case CompareLt:
      if( left < right )
         returnValue = exp_if_true;
      else
         returnValue = exp_if_false;
      break;

      case CompareLe:
      if( left <= right )
         returnValue = exp_if_true;
      else
         returnValue = exp_if_false;
      break;

      case CompareEq:
      if( left == right )
         returnValue = exp_if_true;
      else
         returnValue = exp_if_false;
      break;

      case CompareGe:
      if( left >= right )
         returnValue = exp_if_true;
      else
         returnValue = exp_if_false;
      break;

      case CompareGt:
      if( left > right )
         returnValue = exp_if_true;
      else
         returnValue = exp_if_false;
      break;

      default:
      CPPAD_ASSERT_UNKNOWN(0);
      returnValue = exp_if_true;
   }
   return returnValue;
}

} // END_CPPAD_NAMESPACE
# endif
