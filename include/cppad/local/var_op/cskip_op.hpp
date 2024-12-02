# ifndef CPPAD_LOCAL_VAR_OP_CSKIP_OP_HPP
# define CPPAD_LOCAL_VAR_OP_CSKIP_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

namespace CppAD { namespace local { namespace var_op {
/*!
{xrst_begin_parent var_cskip_op dev}

The Conditional Skip Operator
#############################
{xrst_spell
   ge
}

CSkipOp
*******
is the op code for this operator.

User Syntax
***********
| *z* = ``CondExp`` *Rel* ( *left* , *right* , *if_true* , *if_false* )

Rel
***
is Lt, Le, Eq, Ge, or Gt .

arg
***

arg[0]
======
Is the comparison operator for this conditional skip
as a static cast from CompareOp to addr_t :
{xrst_literal
   include/cppad/local/declare_ad.hpp
   // BEGIN_COMPARE_OP
   // END_COMPARE_OP
}
Note that arg[0] cannot be equal to CompareNe; i.e.
the last enum value CompareNe will not appear.

arg[1]
======
The first two bits of this value are used as flags; see below.

arg[2]
======
If arg[1] & 1 is zero (is one) ,
arg[2] is the parameter (variable) index corresponding to *left* .

arg[3]
======
If arg[1] & 2 is zero (is one) ,
arg[3] is the parameter (variable) index corresponding to *right* .

arg[4]
======
is the number of operations to skip if the comparison result is true
and the left and right operands are
:ref:`base_identical@Identical@IdenticalCon` .

arg[5]
======
is the number of operations to skip if the comparison result is false
and the left and right operands are identically constant.

arg[6+i]
========
For i = 0 , ... , arg[4] - 1,
arg[6 + i] is the index of an operation to skip if the comparison
is identically true.

arg[6+arg[4]+i]
===============
For i = 0 , ... , arg[5] - 1,
arg[6 + i] is the index of an operation to skip if the comparison
is identically false.

arg[6+arg[4]+arg[5]]
====================
is the total number operators that might be skipped;
i.e., arg[4] + arg[5] .

{xrst_end var_cskip_op}
------------------------------------------------------------------------------
{xrst_begin var_cskip_forward_0 dev}

Zero Order Forward Conditional Skip Operator
############################################

Prototype
*********
{xrst_literal
   // BEGIN_CSKIP_FORWARD_0
   // END_CSKIP_FORWARD_0
}

Base
****
Base type for the operator; i.e., this operation was recorded
using AD<Base> and computations by this routine are done using type Base.

i_z
***
variable index corresponding to the result of the previous operation.
This is only used for error checking. To be specific,
if the left and right operand for the conditional expression is a variable,
its index must be less than or equal this value.

num_par
*******
is the total number of values in the *parameter* vector.

parameter
*********
maps parameter indices to parameter values.

cap_order
*********
is the maximum number of orders that can fit in *taylor* .

taylor
******
For *j* <= *i_z* ,
the Taylor coefficient corresponding to variable *j* and order zero is

   *taylor* [ *j* * *cap_order* + 0  ]

cskip_op
********
is vector specifying which operations are at this point are know to be
unnecessary and can be skipped. This is both an input and an output; i.e.,
the call may add more true values to *cskip_op* .

{xrst_end var_cskip_forward_0}
*/
// BEGIN_CSKIP_FORWARD_0
template <class Base>
inline void cskip_forward_0(
   size_t               i_z            ,
   const addr_t*        arg            ,
   size_t               num_par        ,
   const Base*          parameter      ,
   size_t               cap_order      ,
   Base*                taylor         ,
   bool*                cskip_op       )
// END_CSKIP_FORWARD_0
{  //
   //
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < size_t(CompareNe) );
   CPPAD_ASSERT_UNKNOWN( arg[1] != 0 );
   //
   Base left, right;
   if( arg[1] & 1 )
   {  // If variable arg[2] <= i_z, it has already been computed,
      // but it will be skipped for higher orders.
      CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) <= i_z );
      left = taylor[ size_t(arg[2]) * cap_order + 0 ];
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[2]) < num_par );
      left = parameter[ arg[2] ];
   }
   if( arg[1] & 2 )
   {  // If variable arg[3] <= i_z, it has already been computed,
      // but it will be skipped for higher orders.
      CPPAD_ASSERT_UNKNOWN( size_t(arg[3]) <= i_z );
      right = taylor[ size_t(arg[3]) * cap_order + 0 ];
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( size_t(arg[3]) < num_par );
      right = parameter[ arg[3] ];
   }
   bool ok_to_skip = IdenticalCon(left) && IdenticalCon(right);
   if( ! ok_to_skip )
      return;

   // initialize to avoid compiler warning
   bool true_case = false;
   Base diff      = left - right;
   switch( CompareOp( arg[0] ) )
   {
      case CompareLt:
      true_case = LessThanZero(diff);
      break;

      case CompareLe:
      true_case = LessThanOrZero(diff);
      break;

      case CompareEq:
      true_case = IdenticalZero(diff);
      break;

      case CompareGe:
      true_case = GreaterThanOrZero(diff);
      break;

      case CompareGt:
      true_case = GreaterThanZero(diff);
      break;

      case CompareNe:
      true_case = ! IdenticalZero(diff);
      break;

      default:
      CPPAD_ASSERT_UNKNOWN(false);
   }
   if( true_case )
   {  for(addr_t i = 0; i < arg[4]; i++)
         cskip_op[ arg[6+i] ] = true;
   }
   else
   {  for(addr_t i = 0; i < arg[5]; i++)
         cskip_op[ arg[6+arg[4]+i] ] = true;
   }
   return;
}
} } } // END namespace
# endif
