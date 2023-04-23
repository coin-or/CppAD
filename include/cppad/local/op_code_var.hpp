# ifndef CPPAD_LOCAL_OP_CODE_VAR_HPP
# define CPPAD_LOCAL_OP_CODE_VAR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <string>
# include <sstream>
# include <iomanip>

# include <cppad/local/atomic_index.hpp>
# include <cppad/local/define.hpp>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/pod_vector.hpp>

// needed before one can use CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*!
{xrst_begin op_code_var dev}
{xrst_spell
   addpv
   funap
   funav
   funrp
   funrv
   initializes
   ldp
   ldv
   opcode
   powpv
   powvv
   pv
   stpp
   stpv
   stvp
   stvv
   vp
   vv
}

Variable Op Codes
#################

Namespace
*********
All of these definitions are in the ``CppAD::local`` namespace.

opcode_t
********
This type is used to save space when storing operator enum type in vectors.
{xrst_spell_off}
{xrst_code hpp} */
typedef CPPAD_VEC_ENUM_TYPE opcode_t;
/* {xrst_code}
{xrst_spell_on}

OpCode
******
This enum type is used to distinguish different ``AD`` < *Base* >
atomic operations.
Each value in the enum type ends with the characters ``Op`` .
Ignoring the ``Op`` at the end,
the operators appear in alphabetical order.

arg[i]
******
We use the notation *arg* [ ``i`` ] below
for the *i*-th operator argument which is a position integer
represented using the type ``addr_t`` .

Unary
*****
An operator commented as unary below
has one argument (arg[0]) and it is a variable index.
All of these operators have one result variable.

Binary And Compare
******************
An operator commented as binary or compare below
has two arguments.
If it is a compare operator it has no result variables
(the result is true or false but not a variable).
Otherwise, it has one result variable.
These operators use the following convention for the operator ending
and the left argument (arg[0]) and right argument (arg[1]):

.. csv-table::
   :widths: auto

   *Ending*,*Left*,*Right*
   ``pvOp``,parameter index,variable index
   ``vpOp``,variable index,parameter index
   ``vvOp``,variable index,variable index

For example, ``AddpvOp`` represents the addition operator where the left
operand is a parameter and the right operand is a variable.

Pow
===
The binary ``pow`` ( *x* , *y* ) operators PowpvOp, PowvvOp are
special because they have three variable results instead of one.
To be specific, they compute
``log`` ( *x* ) ,
``log`` ( *x* ) * *y* ,
``exp`` ( ``log`` ( *x* ) * *y* )

{xrst_comment ------------------------------------------------------------- }
AFunOp
******
This operator appears at the start and end of every atomic function call.
This operator has no results variables.

arg[0]
======
This is the :ref:`atomic_index-name` for this function.

arg[1]
======
This is the :ref:`atomic_four_call@call_id` information.
It is also he :ref:`atomic_one@id`
for atomic one functions which have been deprecated.

arg[2]
======
is the number of arguments to this atomic function.
We use the notation *n* = *arg* [2] below.

arg[3]
======
is the number of results for this atomic function.
We use the notation *m* = *arg* [3] below.

Arguments
=========
There are *n* operators after the first ``AFunOp`` ,
one for each argument.
If the *j*-th argument is a parameter (variable)
the corresponding operator is ``FunapOp`` ( ``FunavOp`` ), and
the corresponding operator argument is a parameter index (variable index).
These operators have no result variables.

Results
=======
There are *m* operators after the last argument operator
one for each result.
If the *i*-th result is a parameter (variable)
the corresponding operator is ``FunrpOp`` ( ``FunrvOp`` ).
In the parameter case, there is one argument and it is the parameter index,
and not result variables.
In the variable case, there are no arguments and one result variable.
The index for the new variable with the next possible index.

{xrst_comment ------------------------------------------------------------- }
BeginOp
*******
This operator marks the start of the tape.
It has one parameter index argument that is nan and corresponds
to parameter index zero.
It also has one variable result that has index zero which is used to
indicate that a value is not a variable.
for indicate an parameter.

{xrst_comment ------------------------------------------------------------- }
CExpOp
******
This is a :ref:`conditional expression<condexp-name>` ; i.e., the corresponding
source code is

   *result* = ``CondExp`` *Rel* ( *left* , *right* , *if_true* , *if_false*

This operator has one variable result.

arg[0]
======
This is a :ref:`base_cond_exp@CompareOp` value corresponding
to :ref:`condexp@Rel` above.  ( *Rel* = ``Ne`` is not possible).

arg[1]
======
The first four bits of this integer are used as flags; see below.

arg[2]
======
If arg[1] & 1 is true (false),
this is the variable index (parameter index) corresponding to *left* .

arg[3]
======
If arg[1] & 2 is true (false),
this is the variable index (parameter index) corresponding to *right* .

arg[4]
======
If arg[1] & 4 is true (false),
this is the variable index (parameter index) corresponding to *if_true* .

arg[5]
======
If arg[1] & 8 is true (false),
this is the variable index (parameter index) corresponding to *if_false* .

{xrst_comment ------------------------------------------------------------- }
CSkipOp
*******
The conditional skip operator (used to skip operations that depend on false
branches to conditional expressions).
This operator has not result variables.

arg[0]
======
This is a :ref:`base_cond_exp@CompareOp` value corresponding
to this conditional skip.

arg[1]
======
The first two bits of this integer are used as flags; see below.

arg[2]
======
If arg[1] & 1 is true (false),
this is the variable index (parameter index) corresponding to *left* .

arg[3]
======
If arg[1] & 2 is true (false),
this is the variable index (parameter index) corresponding to *right* .

arg[4]
======
is the number of operations to skip if the comparison is true.
We use the notation *n* = *arg* [4] below.

arg[5]
======
is the number of operations to skip if the comparison is false.
We use the notation *m* = *arg* [5] below.

arg[6+i]
========
For *i* = 0, ..., *n* ``-1`` , this is the index
of an operator that can be skipped if the comparison is true.

arg[6+n+i]
==========
For *i* = 0, ..., *m* ``-1`` , this is the index
of an operator that can be skipped if the comparison is false.

arg[6+n+m]
==========
The is the total number operators that might be skipped; i.e., *n* + *m* .

{xrst_comment ------------------------------------------------------------- }
CSumOp
******
Is a cumulative summation operator
which has one result variable.

arg[0]
======
is the index of the parameter that initializes the summation.

arg[1]
======
argument index that flags the end of the addition variables,
we use the notation *k* = *arg* [1] below.

arg[2]
======
argument index that flags the end of the subtraction variables,
we use the notation *ell* = *arg* [2] below.

arg[3]
======
argument index that flags the end of the addition dynamic parameters,
we use the notation *m* = *arg* [3] below.

arg[4]
======
argument index that flags the end of the subtraction dynamic parameters,
we use the notation *n* = *arg* [4] below.

arg[5+i]
========
for *i* = 0, ..., *k* ``-6`` ,
this is the index of the *i*-th variable to be added in the summation.

arg[k+i]
========
for *i* = 0, ..., *ell* ``-`` *k* ``-1`` ,
this is the index of the *i*-th variable to be subtracted in the summation.

arg[ell+i]
==========
for *i* = 0, ..., *m* ``-`` *ell* ``-1`` , this is the index of the
*i*-th dynamic parameter to be added in the summation.

arg[m+i]
========
for *i* = 0, ..., *n* ``-`` *m* ``-1`` , this is the index of the
*i*-th dynamic parameter to be subtracted in the summation.

arg[n]
======
This is equal to *n* .
Note that there are *n* +1 arguments to this operator
and having this value at the end enable reverse model to know how far
to back up to get to the start of this operation.

{xrst_comment ------------------------------------------------------------- }
DisOp
*****
Call to a user defined :ref:`discrete-name` function.
This operator has one result variable.

arg[0]
======
is the index, in the order of the functions defined by the user,
for this discrete function.

arg[1]
======
variable index corresponding to the argument for this function call.

{xrst_comment ------------------------------------------------------------- }
Load
****
The load operators create a new variable corresponding to
*vec* [ *ind* ] where *vec* is a :ref:`VecAD-name` vector
and *ind* is an ``AD`` < *Base* > .
For these operators either *vec* or *ind* is a variable
and there is one variable result.

LdpOp
=====
This load is used for an index *ind* that is a parameter.

LdvOp
=====
This load is used for an index *ind* that is a variable.

arg[0]
======
is the offset of this VecAD vector
relative to the beginning of the single array
that contains all VecAD elements for all the VecAD vectors.
This corresponds to the first element of this vector and not its size
(which comes just before the first element).

arg[1]
======
is the index in this VecAD vector for this load operation.
For the ``LdpOp`` (``LdvOp`` ) operator this is the
parameter index (variable index) corresponding to *ind* .

arg[2]
======
is the index of this VecAD load operation in the set of all
the load operations in this recording.
This includes both dynamic parameter and variable loads.
It is used to map load operations to corresponding
dynamic parameters and variables.

{xrst_comment ------------------------------------------------------------- }
Store
*****
The store operators store information corresponding to
*vec* [ *ind* ] = ``right`` where *vec* is a :ref:`VecAD-name` vector
and *ind* is an ``AD`` < *Base* > .
For these operators either *vec* , *ind* , or *right*
is a variable and there is no result.

StppOp
======
This store is used when *ind* and *right* are parameters.

StpvOp
======
This store is used when *ind* is a parameter
and *right* is a variable.

StvpOp
======
This store is used when *ind* is a variable
and *right* is a parameter.

StvvOp
======
This store is used when *index* and *right* are variables.

arg[0]
======
is the offset of this VecAD vector
relative to the beginning of the single array
that contains all VecAD elements for all the VecAD vectors.
This corresponds to the first element of this vector and not its size
(which comes just before the first element).

arg[1]
======
is the index in this VecAD vector for this store operation.
For the ``StppOp`` and ``StpvOp`` cases
this is the parameter index corresponding to *ind* .
For the ``StvpOp`` and ``StvvOp`` cases,
this is the variable index corresponding to *ind* .

arg[2]
======
For the ``StppOp`` and ``StvpOp`` cases,
this is the parameter index corresponding to *right* .
For the ``StpvOp`` and ``StvvOp`` cases,
this is the variable index corresponding to *right* .

{xrst_comment ------------------------------------------------------------- }
ParOp
*****
This operator has one result that is equal to a parameter
(not that all the derivatives for this result will be zero).

arg[0]
======
Is the index of the parameter that determines the value of the variable.

{xrst_comment ------------------------------------------------------------- }
PriOp
*****
This operator implements the :ref:`PrintFor-name` command

   ``PrintFor`` ( *pos* , *before* , *value* , *after* )

arg[0]
======
The first two bits of this integer are used as flags; see below.

arg[1]
======
If arg[1] & 1 is true (false),
this is the variable index (parameter index) corresponding to *pos* .

arg[2]
======
is the text index corresponding to *before* .

arg[3]
======
If arg[1] & 2 is true (false),
this is the variable index (parameter index) corresponding to *value* .

arg[4]
======
is the text index corresponding to *after* .

{xrst_comment ------------------------------------------------------------- }

Source
******
{xrst_spell_off}
{xrst_code hpp} */
// BEGIN_SORT_THIS_LINE_PLUS_2
enum OpCode {
   AFunOp,   // see its heading above
   AbsOp,    // unary fabs
   AcosOp,   // unary acos
   AcoshOp,  // unary acosh
   AddpvOp,  // binary +
   AddvvOp,  // ...
   AsinOp,   // unary asin
   AsinhOp,  // unary asinh
   AtanOp,   // unary atan
   AtanhOp,  // unary atanh
   BeginOp,  // see its heading above
   CExpOp,   // ...
   CSkipOp,  // see its heading above
   CSumOp,   // ...
   CosOp,    // unary cos
   CoshOp,   // unary cosh
   DisOp,    // ...
   DivpvOp,  // binary /
   DivvpOp,  // ...
   DivvvOp,  // ...
   EndOp,    // used to mark the end of the tape
   EqppOp,   // compare equal
   EqpvOp,   // ...
   EqvvOp,   // ...
   ErfOp,    // unary erf
   ErfcOp,   // unary erfc
   ExpOp,    // unary exp
   Expm1Op,  // unary expm1
   FunapOp,  // see AFun heading above
   FunavOp,  // ...
   FunrpOp,  // ...
   FunrvOp,  // ...
   InvOp,    // independent variable, no argumements, one result variable
   LdpOp,    // see its heading above
   LdvOp,    // ...
   LeppOp,   // compare <=
   LepvOp,   // ...
   LevpOp,   // ...
   LevvOp,   // ...
   Log1pOp,  // unary log1p
   LogOp,    // unary log
   LtppOp,   // compare <
   LtpvOp,   // ...
   LtvpOp,   // ...
   LtvvOp,   // ...
   MulpvOp,  // binary *
   MulvvOp,  // ...
   NegOp,    // unary negative
   NeppOp,   // compare !=
   NepvOp,   // ...
   NevvOp,   // ...
   ParOp,    // see its heading above
   PowpvOp,  // see its heading above
   PowvpOp,  // binary
   PowvvOp,  // see its heading above
   PriOp,    // see its heading above
   SignOp,   // unary sign
   SinOp,    // unary sin
   SinhOp,   // unary sinh
   SqrtOp,   // unary sqrt
   StppOp,   // see its heading above
   StpvOp,   // ...
   StvpOp,   // ...
   StvvOp,   // ...
   SubpvOp,  // binary -
   SubvpOp,  // ...
   SubvvOp,  // ...
   TanOp,    // unary tan
   TanhOp,   // unary tanh
   ZmulpvOp, // binary azmul
   ZmulvpOp, // ...
   ZmulvvOp, // ...
   NumberOp  // number of operator codes (not an operator)
};
// END_SORT_THIS_LINE_MINUS_3
/* {xrst_code}
{xrst_spell_on}

{xrst_end op_code_var}
*/
// Note that bin/check_op_code.sh assumes the pattern NumberOp occurs
// at the end of this list and only at the end of this list.

/*!
Number of arguments for a specified operator.

\return
Number of arguments corresponding to the specified operator.

\param op
Operator for which we are fetching the number of arugments.

\par NumArgTable
this table specifes the number of arguments stored for each
occurance of the operator that is the i-th value in the OpCode enum type.
For example, for the first three OpCode enum values we have
\verbatim
OpCode   j   NumArgTable[j]  Meaning
AbsOp    0                1  index of variable we are taking absolute value of
AcosOp   1                1  index of variable we are taking acos of
AcoshOp  2                1  index of variable we are taking acosh of
\endverbatim
Note that the meaning of the arguments depends on the operator.
*/
inline size_t NumArg( OpCode op)
{  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;

   // agreement with OpCode is checked by bin/check_op_code.sh
   // BEGIN_SORT_THIS_LINE_PLUS_2
   static const size_t NumArgTable[] = {
      /* AFunOp   */ 4,
      /* AbsOp    */ 1,
      /* AcosOp   */ 1,
      /* AcoshOp  */ 1,
      /* AddpvOp  */ 2,
      /* AddvvOp  */ 2,
      /* AsinOp   */ 1,
      /* AsinhOp  */ 1,
      /* AtanOp   */ 1,
      /* AtanhOp  */ 1,
      /* BeginOp  */ 1,  // offset first real argument to have index 1
      /* CExpOp   */ 6,
      /* CSkipOp  */ 0,  // (has a variable number of arguments, not zero)
      /* CSumOp   */ 0,  // (has a variable number of arguments, not zero)
      /* CosOp    */ 1,
      /* CoshOp   */ 1,
      /* DisOp    */ 2,
      /* DivpvOp  */ 2,
      /* DivvpOp  */ 2,
      /* DivvvOp  */ 2,
      /* EndOp    */ 0,
      /* EqppOp   */ 2,
      /* EqpvOp   */ 2,
      /* EqvvOp   */ 2,
      /* ErfOp    */ 3,
      /* ErfcOp   */ 3,
      /* ExpOp    */ 1,
      /* Expm1Op  */ 1,
      /* FunapOp  */ 1,
      /* FunavOp  */ 1,
      /* FunrpOp  */ 1,
      /* FunrvOp  */ 0,
      /* InvOp    */ 0,
      /* LdpOp    */ 3,
      /* LdvOp    */ 3,
      /* LeppOp   */ 2,
      /* LepvOp   */ 2,
      /* LevpOp   */ 2,
      /* LevvOp   */ 2,
      /* Log1pOp  */ 1,
      /* LogOp    */ 1,
      /* LtppOp   */ 2,
      /* LtpvOp   */ 2,
      /* LtvpOp   */ 2,
      /* LtvvOp   */ 2,
      /* MulpvOp  */ 2,
      /* MulvvOp  */ 2,
      /* NegOp    */ 1,
      /* NeppOp   */ 2,
      /* NepvOp   */ 2,
      /* NevvOp   */ 2,
      /* ParOp    */ 1,
      /* PowpvOp  */ 2,
      /* PowvpOp  */ 2,
      /* PowvvOp  */ 2,
      /* PriOp    */ 5,
      /* SignOp   */ 1,
      /* SinOp    */ 1,
      /* SinhOp   */ 1,
      /* SqrtOp   */ 1,
      /* StppOp   */ 3,
      /* StpvOp   */ 3,
      /* StvpOp   */ 3,
      /* StvvOp   */ 3,
      /* SubpvOp  */ 2,
      /* SubvpOp  */ 2,
      /* SubvvOp  */ 2,
      /* TanOp    */ 1,
      /* TanhOp   */ 1,
      /* ZmulpvOp */ 2,
      /* ZmulvpOp */ 2,
      /* ZmulvvOp */ 2,
      0  // NumberOp not used
   };
   // END_SORT_THIS_LINE_MINUS_3
# ifndef NDEBUG
   // only do these checks once to save time
   static bool first = true;
   if( first )
   {  first = false;
      // check that NumberOp is last value in op code table
      CPPAD_ASSERT_UNKNOWN(
         size_t(NumberOp) + 1 == sizeof(NumArgTable)/sizeof(NumArgTable[0])
      );
      //Check that the type CPPAD_VEC_ENUM_TYPE as required by define.hpp
      CPPAD_ASSERT_UNKNOWN( is_pod<opcode_t>() );
      size_t number_op_size_t = size_t( NumberOp );
      CPPAD_ASSERT_UNKNOWN(
         number_op_size_t < std::numeric_limits<opcode_t>::max()
      );
   }
   // do this check every time
   CPPAD_ASSERT_UNKNOWN( size_t(op) < size_t(NumberOp) );
# endif

   return NumArgTable[op];
}

/*!
Number of variables resulting from the specified operation.

\param op
Operator for which we are fecching the number of results.

\par NumResTable
table specifes the number of varibles that result for each
occurance of the operator that is the i-th value in the OpCode enum type.
For example, for the first three OpCode enum values we have
\verbatim
OpCode   j   NumResTable[j]  Meaning
AbsOp    0                1  variable that is the result of the absolute value
AcosOp   1                2  acos(x) and sqrt(1-x*x) are required for this op
AcoshOp  2                2  acosh(x) and sqrt(x*x-1) are required for this op
\endverbatim
*/
inline size_t NumRes(OpCode op)
{  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;

   // agreement with OpCode is checked by bin/check_op_code.sh
   // BEGIN_SORT_THIS_LINE_PLUS_2
   static const size_t NumResTable[] = {
      /* AFunOp   */ 0,
      /* AbsOp    */ 1,
      /* AcosOp   */ 2,
      /* AcoshOp  */ 2,
      /* AddpvOp  */ 1,
      /* AddvvOp  */ 1,
      /* AsinOp   */ 2,
      /* AsinhOp  */ 2,
      /* AtanOp   */ 2,
      /* AtanhOp  */ 2,
      /* BeginOp  */ 1,  // offsets first variable to have index one (not zero)
      /* CExpOp   */ 1,
      /* CSkipOp  */ 0,
      /* CSumOp   */ 1,
      /* CosOp    */ 2,
      /* CoshOp   */ 2,
      /* DisOp    */ 1,
      /* DivpvOp  */ 1,
      /* DivvpOp  */ 1,
      /* DivvvOp  */ 1,
      /* EndOp    */ 0,
      /* EqppOp   */ 0,
      /* EqpvOp   */ 0,
      /* EqvvOp   */ 0,
      /* ErfOp    */ 5,
      /* ErfcOp   */ 5,
      /* ExpOp    */ 1,
      /* Expm1Op  */ 1,
      /* FunapOp  */ 0,
      /* FunavOp  */ 0,
      /* FunrpOp  */ 0,
      /* FunrvOp  */ 1,
      /* InvOp    */ 1,
      /* LdpOp    */ 1,
      /* LdvOp    */ 1,
      /* LeppOp   */ 0,
      /* LepvOp   */ 0,
      /* LevpOp   */ 0,
      /* LevvOp   */ 0,
      /* Log1pOp  */ 1,
      /* LogOp    */ 1,
      /* LtppOp   */ 0,
      /* LtpvOp   */ 0,
      /* LtvpOp   */ 0,
      /* LtvvOp   */ 0,
      /* MulpvOp  */ 1,
      /* MulvvOp  */ 1,
      /* NegOp    */ 1,
      /* NeppOp   */ 0,
      /* NepvOp   */ 0,
      /* NevvOp   */ 0,
      /* ParOp    */ 1,
      /* PowpvOp  */ 3,
      /* PowvpOp  */ 1,
      /* PowvvOp  */ 3,
      /* PriOp    */ 0,
      /* SignOp   */ 1,
      /* SinOp    */ 2,
      /* SinhOp   */ 2,
      /* SqrtOp   */ 1,
      /* StppOp   */ 0,
      /* StpvOp   */ 0,
      /* StvpOp   */ 0,
      /* StvvOp   */ 0,
      /* SubpvOp  */ 1,
      /* SubvpOp  */ 1,
      /* SubvvOp  */ 1,
      /* TanOp    */ 2,
      /* TanhOp   */ 2,
      /* ZmulpvOp */ 1,
      /* ZmulvpOp */ 1,
      /* ZmulvvOp */ 1,
      0  // NumberOp not used and avoids g++ 4.3.2 warn when pycppad builds
   };
   // END_SORT_THIS_LINE_MINUS_3
   // check ensuring conversion to size_t is as expected
   CPPAD_ASSERT_UNKNOWN( size_t(NumberOp) + 1 ==
      sizeof(NumResTable) / sizeof(NumResTable[0])
   );
   // this test ensures that all indices are within the table
   CPPAD_ASSERT_UNKNOWN( size_t(op) < size_t(NumberOp) );

   return NumResTable[op];
}


/*!
Fetch the name for a specified operation.

\return
name of the specified operation.

\param op
Operator for which we are fetching the name
*/
inline std::string OpName(OpCode op)
{  // agreement with OpCode is checked by bin/check_op_code.sh
   // BEGIN_SORT_THIS_LINE_PLUS_2
   static const char *OpNameTable[] = {
      "AFunOp"  ,
      "AbsOp"   ,
      "AcosOp"  ,
      "AcoshOp" ,
      "AddpvOp" ,
      "AddvvOp" ,
      "AsinOp"  ,
      "AsinhOp" ,
      "AtanOp"  ,
      "AtanhOp" ,
      "BeginOp" ,
      "CExpOp"  ,
      "CSkipOp" ,
      "CSumOp"  ,
      "CosOp"   ,
      "CoshOp"  ,
      "DisOp"   ,
      "DivpvOp" ,
      "DivvpOp" ,
      "DivvvOp" ,
      "EndOp"   ,
      "EqppOp"  ,
      "EqpvOp"  ,
      "EqvvOp"  ,
      "ErfOp"   ,
      "ErfcOp"  ,
      "ExpOp"   ,
      "Expm1Op" ,
      "FunapOp" ,
      "FunavOp" ,
      "FunrpOp" ,
      "FunrvOp" ,
      "InvOp"   ,
      "LdpOp"   ,
      "LdvOp"   ,
      "LeppOp"  ,
      "LepvOp"  ,
      "LevpOp"  ,
      "LevvOp"  ,
      "Log1pOp" ,
      "LogOp"   ,
      "LtppOp"  ,
      "LtpvOp"  ,
      "LtvpOp"  ,
      "LtvvOp"  ,
      "MulpvOp" ,
      "MulvvOp" ,
      "NegOp"   ,
      "NeppOp"  ,
      "NepvOp"  ,
      "NevvOp"  ,
      "ParOp"   ,
      "PowpvOp" ,
      "PowvpOp" ,
      "PowvvOp" ,
      "PriOp"   ,
      "SignOp"  ,
      "SinOp"   ,
      "SinhOp"  ,
      "SqrtOp"  ,
      "StppOp"  ,
      "StpvOp"  ,
      "StvpOp"  ,
      "StvvOp"  ,
      "SubpvOp" ,
      "SubvpOp" ,
      "SubvvOp" ,
      "TanOp"   ,
      "TanhOp"  ,
      "ZmulpvOp",
      "ZmulvpOp",
      "ZmulvvOp",
      "Number"  // not used
   };
   // END_SORT_THIS_LINE_MINUS_3

   // check ensuring conversion to size_t is as expected
   CPPAD_ASSERT_UNKNOWN(
      size_t(NumberOp) + 1 == sizeof(OpNameTable)/sizeof(OpNameTable[0])
   );
   // this test ensures that all indices are within the table
   CPPAD_ASSERT_UNKNOWN( size_t(op) < size_t(NumberOp) );

   // result
   std::string result = OpNameTable[op];
   result             = result.substr(0, result.size() - 2);

   return result;
}

/*!
Prints a single field corresponding to an operator.

A specified leader is printed in front of the value
and then the value is left justified in the following width character.

\tparam Type
is the type of the value we are printing.

\param os
is the stream that we are printing to.

\param leader
are characters printed before the value.

\param value
is the value being printed.

\param width
is the number of character to print the value in.
If the value does not fit in the width, the value is replace
by width '*' characters.
*/
template <class Type>
void printOpField(
   std::ostream      &os ,
   const char *   leader ,
   const Type     &value ,
   size_t          width )
{
   std::ostringstream buffer;
   std::string        str;

   // first print the leader
   os << leader;

   // print the value into an internal buffer
   buffer << std::setw( int(width) ) << value;
   str = buffer.str();

   // length of the string
   size_t len = str.size();
   if( len > width )
   {
      for(size_t i = 0; i < width-1; i++)
         os << str[i];
      os << "*";
      return;
   }

   // count number of spaces at begining
   size_t nspace = 0;
   while(str[nspace] == ' ' && nspace < len)
      nspace++;

   // left justify the string
   size_t i = nspace;
   while( i < len )
      os << str[i++];

   i = width - len + nspace;
   while(i--)
      os << " ";
}

/*!
Prints a single operator and its operands

\tparam Base
Is the base type for these AD< Base > operations.

\param os
is the output stream that the information is printed on.

\param play
Is the entire recording for the tape that this operator is in.

\param i_op
is the index for the operator corresponding to this operation.

\param i_var
is the index for the variable corresponding to the result of this operation
(if NumRes(op) > 0).

\param op
The operator code (OpCode) for this operation.

\param arg
is the vector of argument indices for this operation
(must have NumArg(op) elements).
*/
template <class Base, class RecBase>
void printOp(
   std::ostream&          os     ,
   const local::player<Base>* play,
   size_t                 i_op   ,
   size_t                 i_var  ,
   OpCode                 op     ,
   const addr_t*          arg    )
{
   CPPAD_ASSERT_KNOWN(
      ! thread_alloc::in_parallel() ,
      "cannot print trace of AD operations in parallel mode"
   );
   static const char *CompareOpName[] =
      { "Lt", "Le", "Eq", "Ge", "Gt", "Ne" };

   // print operator
   printOpField(os,  "o=",      i_op,  5);
   if( NumRes(op) > 0 && op != BeginOp )
      printOpField(os,  "v=",      i_var, 5);
   else
      printOpField(os,  "v=",      "",    5);
   if( op == CExpOp || op == CSkipOp )
   {  printOpField(os, "", OpName(op).c_str(), 5);
      printOpField(os, "", CompareOpName[ arg[0] ], 3);
   }
   else
      printOpField(os, "", OpName(op).c_str(), 8);

   // print other fields
   size_t ncol = 5;
   switch( op )
   {
      case CSkipOp:
      /*
      arg[0]     = the Rel operator: Lt, Le, Eq, Ge, Gt, or Ne
      arg[1] & 1 = is left a variable
      arg[1] & 2 = is right a variable
      arg[2]     = index correspoding to left
      arg[3]     = index correspoding to right
      arg[4] = number of operations to skip if CExpOp comparison is true
      arg[5] = number of operations to skip if CExpOp comparison is false
      arg[6] -> arg[5+arg[4]]               = skip operations if true
      arg[6+arg[4]] -> arg[5+arg[4]+arg[5]] = skip operations if false
      arg[6+arg[4]+arg[5]] = arg[4] + arg[5]
      */
      CPPAD_ASSERT_UNKNOWN( arg[6+arg[4]+arg[5]] == arg[4]+arg[5] );
      CPPAD_ASSERT_UNKNOWN(arg[1] != 0);
      if( arg[1] & 1 )
         printOpField(os, " vl=", arg[2], ncol);
      else
         printOpField(os, " pl=", play->GetPar( size_t(arg[2]) ), ncol);
      if( arg[1] & 2 )
         printOpField(os, " vr=", arg[3], ncol);
      else
         printOpField(os, " pr=", play->GetPar( size_t(arg[3]) ), ncol);
      if( size_t(arg[4]) < 3 )
      {  for(addr_t i = 0; i < arg[4]; i++)
            printOpField(os, " ot=", arg[6+i], ncol);
      }
      else
      {  printOpField(os, "\n\tot=", arg[6+0], ncol);
         for(addr_t i = 1; i < arg[4]; i++)
            printOpField(os, " ot=", arg[6+i], ncol);
      }
      if( size_t(arg[5]) < 3 )
      {  for(addr_t i = 0; i < arg[5]; i++)
            printOpField(os, " of=", arg[6+arg[4]+i], ncol);
      }
      else
      {  printOpField(os, "\n\tof=", arg[6+arg[4]+0], ncol);
         {  for(addr_t i = 1; i < arg[5]; i++)
               printOpField(os, " of=", arg[6+arg[4]+i], ncol);
         }
      }
      break;

      case CSumOp:
      /*
      arg[0] = index of parameter that initializes summation
      arg[1] = end in arg of addition variables in summation
      arg[2] = end in arg of subtraction variables in summation
      arg[3] = end in arg of addition dynamic parameters in summation
      arg[4] = end in arg of subtraction dynamic parameters in summation
      arg[5],      ... , arg[arg[1]-1]: indices for addition variables
      arg[arg[1]], ... , arg[arg[2]-1]: indices for subtraction variables
      arg[arg[2]], ... , arg[arg[3]-1]: indices for additon dynamics
      arg[arg[3]], ... , arg[arg[4]-1]: indices for subtraction dynamics
      arg[arg[4]] = arg[4]
      */
      CPPAD_ASSERT_UNKNOWN( arg[arg[4]] == arg[4] );
      printOpField(os, " pr=", play->GetPar( size_t(arg[0]) ), ncol);
      for(addr_t i = 5; i < arg[1]; i++)
             printOpField(os, " +v=", arg[i], ncol);
      for(addr_t i = arg[1]; i < arg[2]; i++)
             printOpField(os, " -v=", arg[i], ncol);
      for(addr_t i = arg[2]; i < arg[3]; i++)
             printOpField(os, " +d=", play->GetPar( size_t(arg[i]) ), ncol);
      for(addr_t i = arg[3]; i < arg[4]; i++)
             printOpField(os, " -d=", play->GetPar( size_t(arg[i]) ), ncol);
      break;

      case LdpOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
      printOpField(os, "off=", arg[0], ncol);
      printOpField(os, "  p=", play->GetPar( size_t(arg[1]) ), ncol);
      break;

      case LdvOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
      printOpField(os, "off=", arg[0], ncol);
      printOpField(os, "  v=", arg[1], ncol);
      break;

      case StppOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
      printOpField(os, "off=", arg[0], ncol);
      printOpField(os, " pl=", play->GetPar( size_t(arg[1]) ), ncol);
      printOpField(os, " pr=", play->GetPar( size_t(arg[2]) ), ncol);
      break;

      case StpvOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
      printOpField(os, "off=", arg[0], ncol);
      printOpField(os, "  p=", play->GetPar( size_t(arg[1]) ), ncol);
      printOpField(os, "  v=", arg[2], ncol);
      break;

      case StvpOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
      printOpField(os, "off=", arg[0], ncol);
      printOpField(os, "  v=", arg[1], ncol);
      printOpField(os, "  p=", play->GetPar( size_t(arg[2]) ), ncol);
      break;

      case StvvOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
      printOpField(os, "off=", arg[0], ncol);
      printOpField(os, " vl=", arg[1], ncol);
      printOpField(os, " vr=", arg[2], ncol);
      break;

      case AddvvOp:
      case DivvvOp:
      case EqvvOp:
      case LevvOp:
      case LtvvOp:
      case NevvOp:
      case MulvvOp:
      case PowvvOp:
      case SubvvOp:
      case ZmulvvOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
      printOpField(os, " vl=", arg[0], ncol);
      printOpField(os, " vr=", arg[1], ncol);
      break;

      case AddpvOp:
      case EqpvOp:
      case DivpvOp:
      case LepvOp:
      case LtpvOp:
      case NepvOp:
      case SubpvOp:
      case MulpvOp:
      case PowpvOp:
      case ZmulpvOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
      printOpField(os, " pl=", play->GetPar( size_t(arg[0]) ), ncol);
      printOpField(os, " vr=", arg[1], ncol);
      break;

      case DivvpOp:
      case LevpOp:
      case LtvpOp:
      case PowvpOp:
      case SubvpOp:
      case ZmulvpOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
      printOpField(os, " vl=", arg[0], ncol);
      printOpField(os, " pr=", play->GetPar( size_t(arg[1]) ), ncol);
      break;

      case AbsOp:
      case AcosOp:
      case AcoshOp:
      case AsinOp:
      case AsinhOp:
      case AtanOp:
      case AtanhOp:
      case CosOp:
      case CoshOp:
      case ExpOp:
      case Expm1Op:
      case LogOp:
      case Log1pOp:
      case NegOp:
      case SignOp:
      case SinOp:
      case SinhOp:
      case SqrtOp:
      case FunavOp:
      case TanOp:
      case TanhOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 1 );
      printOpField(os, "  v=", arg[0], ncol);
      break;

      case ErfOp:
      case ErfcOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
      // arg[1] points to the parameter 0
      // arg[2] points to the parameter 2 / sqrt(pi)
      printOpField(os, "  v=", arg[0], ncol);
      break;

      case ParOp:
      case FunapOp:
      case FunrpOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 1 );
      printOpField(os, "  p=", play->GetPar( size_t(arg[0]) ), ncol);
      break;

      case AFunOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 4 );
      {
         // get the name of this atomic function
         bool         set_null   = false;
         size_t       atom_index = size_t( arg[0] );
         size_t       type       = 0;          // set to avoid warning
         std::string name;
         void*        v_ptr    = nullptr; // set to avoid warning
         atomic_index<RecBase>(set_null, atom_index, type, &name, v_ptr);
         printOpField(os, " f=",   name.c_str(), ncol);
         printOpField(os, " i=", arg[1], ncol);
         printOpField(os, " n=", arg[2], ncol);
         printOpField(os, " m=", arg[3], ncol);
      }
      break;

      case PriOp:
      CPPAD_ASSERT_NARG_NRES(op, 5, 0);
      if( arg[0] & 1 )
         printOpField(os, " v=", arg[1], ncol);
      else
         printOpField(os, " p=", play->GetPar( size_t(arg[1]) ), ncol);
      os << "before=\"" << play->GetTxt( size_t(arg[2]) ) << "\"";
      if( arg[0] & 2 )
         printOpField(os, " v=", arg[3], ncol);
      else
         printOpField(os, " p=", play->GetPar( size_t(arg[3]) ), ncol);
      os << "after=\"" << play->GetTxt( size_t(arg[4]) ) << "\"";
      break;

      case BeginOp:
      // argument not used (created by independent)
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 1 );
      break;

      case EndOp:
      case InvOp:
      case FunrvOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 0 );
      break;

      case DisOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
      {  const std::string name = discrete<Base>::name( size_t(arg[0]) );
         printOpField(os, " f=", name.c_str(), ncol);
         printOpField(os, " x=", arg[1], ncol);
      }
      break;


      case CExpOp:
      CPPAD_ASSERT_UNKNOWN(arg[1] != 0);
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 6 );
      if( arg[1] & 1 )
         printOpField(os, " vl=", arg[2], ncol);
      else
         printOpField(os, " pl=", play->GetPar( size_t(arg[2]) ), ncol);
      if( arg[1] & 2 )
         printOpField(os, " vr=", arg[3], ncol);
      else
         printOpField(os, " pr=", play->GetPar( size_t(arg[3]) ), ncol);
      if( arg[1] & 4 )
         printOpField(os, " vt=", arg[4], ncol);
      else
         printOpField(os, " pt=", play->GetPar( size_t(arg[4]) ), ncol);
      if( arg[1] & 8 )
         printOpField(os, " vf=", arg[5], ncol);
      else
         printOpField(os, " pf=", play->GetPar( size_t(arg[5]) ), ncol);
      break;

      case EqppOp:
      case LeppOp:
      case LtppOp:
      case NeppOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
      printOpField(os, " pl=", play->GetPar( size_t(arg[0]) ), ncol);
      printOpField(os, " pr=", play->GetPar( size_t(arg[1]) ), ncol);
      break;

      default:
      CPPAD_ASSERT_UNKNOWN(0);
   }
}

/*!
Prints the result values correspnding to an operator.

\tparam Base
Is the base type for these AD< Base > operations.

\tparam Value
Determines the type of the values that we are printing.

\param os
is the output stream that the information is printed on.

\param nfz
is the number of forward sweep calculated values of type Value
that correspond to this operation
(ignored if NumRes(op) == 0).

\param fz
points to the first forward calculated value
that correspond to this operation
(ignored if NumRes(op) == 0).

\param nrz
is the number of reverse sweep calculated values of type Value
that correspond to this operation
(ignored if NumRes(op) == 0).

\param rz
points to the first reverse calculated value
that correspond to this operation
(ignored if NumRes(op) == 0).
*/
template <class Value>
void printOpResult(
   std::ostream          &os     ,
   size_t                 nfz    ,
   const  Value          *fz     ,
   size_t                 nrz    ,
   const  Value          *rz     )
{
   size_t k;
   for(k = 0; k < nfz; k++)
      os << "| fz[" << k << "]=" << fz[k];
   for(k = 0; k < nrz; k++)
      os << "| rz[" << k << "]=" << rz[k];
}

/*!
Determines which arguments are variaibles for an operator.

\param op
is the operator. Note that CSkipOp and CSumOp are special cases
because the true number of arguments is not equal to NumArg(op)
and the true number of arguments num_arg can be large.
It may be more efficient to handle these cases separately
(see below).

\param arg
is the argument vector for this operator.

\param is_variable
If the input value of the elements in this vector do not matter.
Upon return, resize has been used to set its size to the true number
of arguments to this operator.
If op != CSkipOp and op != CSumOp, is_variable.size() = NumArg(op).
The j-th argument for this operator is a
variable index if and only if is_variable[j] is true. Note that the variable
index 0, for the BeginOp, does not correspond to a real variable and false
is returned for this case.

\par CSkipOp
In the case of CSkipOp,
\code
      is_variable.size()  = 7 + arg[4] + arg[5];
      is_variable[2]      = (arg[1] & 1) != 0;
      is_variable[3]      = (arg[1] & 2) != 0;
\endcode
and all the other is_variable[j] values are false.

\par CSumOp
In the case of CSumOp,
\code
      is_variable.size() = arg[4]
      for(size_t j = 5; j < arg[2]; ++j)
         is_variable[j] = true;
\endcode
and all the other is_variable values are false.
*/
template <class Addr>
void arg_is_variable(
   OpCode            op          ,
   const Addr*       arg         ,
   pod_vector<bool>& is_variable )
{  size_t num_arg = NumArg(op);
   is_variable.resize( num_arg );
   //
   switch(op)
   {
      // -------------------------------------------------------------------
      // cases where true number of arugments = NumArg(op) == 0

      case EndOp:
      case InvOp:
      case FunrvOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 0 );
      break;

      // -------------------------------------------------------------------
      // cases where NumArg(op) == 1
      case AbsOp:
      case AcoshOp:
      case AcosOp:
      case AsinhOp:
      case AsinOp:
      case AtanhOp:
      case AtanOp:
      case CoshOp:
      case CosOp:
      case Expm1Op:
      case ExpOp:
      case Log1pOp:
      case LogOp:
      case NegOp:
      case SignOp:
      case SinhOp:
      case SinOp:
      case SqrtOp:
      case TanhOp:
      case TanOp:
      case FunavOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 1 );
      is_variable[0] = true;
      break;

      case BeginOp:
      case ParOp:
      case FunapOp:
      case FunrpOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 1 );
      is_variable[0] = false;
      break;


      // -------------------------------------------------------------------
      // cases where NumArg(op) == 2

      case AddpvOp:
      case DisOp:
      case DivpvOp:
      case EqpvOp:
      case LepvOp:
      case LtpvOp:
      case MulpvOp:
      case NepvOp:
      case PowpvOp:
      case SubpvOp:
      case ZmulpvOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
      is_variable[0] = false;
      is_variable[1] = true;
      break;

      case DivvpOp:
      case LevpOp:
      case LtvpOp:
      case PowvpOp:
      case SubvpOp:
      case ZmulvpOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
      is_variable[0] = true;
      is_variable[1] = false;
      break;

      case AddvvOp:
      case DivvvOp:
      case EqvvOp:
      case LevvOp:
      case LtvvOp:
      case MulvvOp:
      case NevvOp:
      case PowvvOp:
      case SubvvOp:
      case ZmulvvOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
      is_variable[0] = true;
      is_variable[1] = true;
      break;

      case ErfOp:
      case ErfcOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
      is_variable[0] = true;
      is_variable[1] = false; // parameter index corresponding to zero
      is_variable[2] = false; // parameter index corresponding to one
      break;

      // --------------------------------------------------------------------
      // cases where NumArg(op) == 3

      case LdpOp:
      case StppOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
      is_variable[0] = false;
      is_variable[1] = false;
      is_variable[2] = false;
      break;

      case LdvOp:
      case StvpOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
      is_variable[0] = false;
      is_variable[1] = true;
      is_variable[2] = false;
      break;

      case StpvOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
      is_variable[0] = false;
      is_variable[1] = false;
      is_variable[2] = true;
      break;

      case StvvOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 3 );
      is_variable[0] = false;
      is_variable[1] = true;
      is_variable[2] = true;
      break;

      // --------------------------------------------------------------------
      // case where NumArg(op) == 4
      case AFunOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 4 );
      for(size_t i = 0; i < 4; i++)
         is_variable[i] = false;
      break;

      // --------------------------------------------------------------------
      // case where NumArg(op) == 5
      case PriOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 5 );
      is_variable[0] = false;
      is_variable[1] = (arg[0] & 1) != 0;
      is_variable[2] = false;
      is_variable[3] = (arg[0] & 2) != 0;
      is_variable[4] = false;
      break;

      // --------------------------------------------------------------------
      // case where NumArg(op) == 6
      case CExpOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 6 );
      is_variable[0] = false;
      is_variable[1] = false;
      is_variable[2] = (arg[0] & 1) != 0;
      is_variable[3] = (arg[0] & 2) != 0;
      is_variable[4] = (arg[0] & 4) != 0;
      is_variable[5] = (arg[0] & 8) != 0;
      break;

      // -------------------------------------------------------------------
      // CSkipOp:
      case CSkipOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 0 )
      //
      // true number of arguments
      num_arg = size_t(7 + arg[4] + arg[5]);
      is_variable.resize(num_arg);
      is_variable[0] = false;
      is_variable[1] = false;
      is_variable[2] = (arg[1] & 1) != 0;
      is_variable[3] = (arg[1] & 2) != 0;
      for(size_t i = 4; i < num_arg; ++i)
         is_variable[i] = false;
      break;

      // -------------------------------------------------------------------
      // CSumOp:
      case CSumOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 0 )
      //
      // true number of arguments
      num_arg = size_t(arg[4]);
      //
      is_variable.resize( num_arg );
      for(size_t i = 0; i < num_arg; ++i)
         is_variable[i] = (5 <= i) & (i < size_t(arg[2]));
      break;

      case EqppOp:
      case LeppOp:
      case LtppOp:
      case NeppOp:
      CPPAD_ASSERT_UNKNOWN( NumArg(op) == 2 );
      is_variable[0] = false;
      is_variable[1] = false;
      break;

      // --------------------------------------------------------------------
      default:
      CPPAD_ASSERT_UNKNOWN(false);
      break;
   }
   return;
}

} } // END_CPPAD_LOCAL_NAMESPACE
# endif
