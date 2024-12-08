# ifndef CPPAD_LOCAL_VAR_OP_COMPARE_OP_HPP
# define CPPAD_LOCAL_VAR_OP_COMPARE_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ---------------------------------------------------------------------------
/*
{xrst_begin_parent var_compare_op dev}
{xrst_spell
   eqpp
   lepp
   ltpp
   nepp
   eqpv
   lepv
   ltpv
   nepv
   levp
   ltvp
   eqvv
   levv
   ltvv
   nevv
}

Comparison Operators
####################

User Syntax
***********
*z* = *x* *op* *y*

x
*
is the left operand for this comparison.

y
*
is the right operand for this comparison.

z
*
is he result for this operator which is a boolean
(not a variable or parameter).

op
**
is one of the following: ==, <=, <, != .
The >= and > comparisons are folded into <= and < by switching *x* and *y* .

op_code
*******

EqppOp, LeppOp, LtppOp, NeppOp
==============================
These operators implement ==. <=, <, and != for the case where
both *x* and *y* are parameters.

EqpvOp, LepvOp, LtpvOp, NepvOp
==============================
These operators implement ==. <=, <, and != for the case where
*x* is a parameter and *y* is a variable.

LevpOp, LtvpOp
==============
These operators implement <= and < for the case where
*x* is a variable and *y* is a parameter.
The == and != operators are folded into EqpvOp and NepvOp by
switching *x* and *y* .

EqvvOp, LevvOp, LtvvOp, NevvOp
==============================
These operators implement ==. <=, <, and != for the case where
both *x* and *y* are variables.

arg
***

arg[0]
======
If *x* is a variable (parameter)
*arg* [0] is the variable index (parameter index) corresponding to *x* .

arg[1]
======
If *y* is a variable (parameter)
*arg* [1] is the variable index (parameter index) corresponding to *y* .

{xrst_end var_compare_op}

-------------------------------------------------------------------------------
{xrst_begin var_compare_forward_any dev}

Forward Comparison Operators
############################

Prototype
*********
{xrst_literal
   // BEGIN_COMPARE_FORWARD_ANY
   // END_COMPARE_FORWARD_ANY
}

Base
****
is the base type for the operator; i.e., this operation was recorded
using ``AD`` < *Base* > and computations by these operators are done using
type *Base* .

op_code, arg
************
see :ref:`var_compare_op@op_code`

parameter
*********
maps parameter indices to parameter values.

cap_order
*********
is the maximum number of orders that will fit in *taylor* .

taylor
******
The Taylor coefficient corresponding to
variable index *i* and order zero is

   *taylor* [ *i* * *cap_order* ]

this_op_index
*************
Is the operator index for this compare operation.

change_count
************
is the change_number at which *this_op_index* is returned
as *change_op_index*.
If it is zero, the comparison changes are not counted.

change_number
*************
If *change_count* is zero, this value is not modified.
Otherwise, if this operator comparison has changed (is no longer true),
*change_number* is incremented by one.

change_op_index
***************
If *change_count* is zero, this value is not modified.
Otherwise, if this operator comparison has changed (is no longer true), and
the new value of *change_number* is equal to *change_count* ,
*change_op_index* is set equal to *this_op_index* .

{xrst_end var_compare_forward_any}
*/

# include <cppad/local/op_code_var.hpp>
# include <cppad/core/cppad_assert.hpp>

namespace CppAD { namespace local { namespace var_op { // BEGIN namespace

// BEGIN_COMPARE_FORWARD_ANY
template <class Base> void compare_forward_any(
   op_code_var   op_code                 ,
   const addr_t* arg                     ,
   const Base*   parameter               ,
   size_t        cap_order               ,
   const Base*   taylor                  ,
   size_t        this_op_index           ,
   size_t        change_count            ,
   size_t&       change_number           ,
   size_t&       change_op_index         )
// END_COMPARE_FORWARD_ANY
{  //
   // n_arg, n_res
   CPPAD_ASSERT_NARG_NRES(op_code, 2, 0);
   //
   // special case
   if( change_count == 0 )
      return;
   //
   // x, y
   Base x, y;
   switch(op_code)
   {  //
      // pp
      case EqppOp:
      case LeppOp:
      case LtppOp:
      case NeppOp:
      x = parameter[ arg[0] ];
      y = parameter[ arg[1] ];
      break;
      //
      // pv
      case EqpvOp:
      case LepvOp:
      case LtpvOp:
      case NepvOp:
      x = parameter[ arg[0] ];
      y = taylor[ size_t(arg[1]) * cap_order ];
      break;

      // vp
      case LevpOp:
      case LtvpOp:
      x = taylor[ size_t(arg[0]) * cap_order ];
      y = parameter[ arg[1] ];
      break;
      //
      // vv
      case EqvvOp:
      case LevvOp:
      case LtvvOp:
      case NevvOp:
      x = taylor[ size_t(arg[0]) * cap_order ];
      y = taylor[ size_t(arg[1]) * cap_order ];
      break;
      //
      default:
      // assign x and y to avoid compiler warnings
      CPPAD_ASSERT_UNKNOWN( false );
      x = CppAD::numeric_limits<Base>::quiet_NaN();
      y = CppAD::numeric_limits<Base>::quiet_NaN();
   }
   bool change;
   switch(op_code)
   {  //
      case EqppOp:
      case EqpvOp:
      case EqvvOp:
      change = x != y;
      break;

      case LeppOp:
      case LepvOp:
      case LevpOp:
      case LevvOp:
      change = GreaterThanZero(x - y);
      break;

      case LtppOp:
      case LtpvOp:
      case LtvpOp:
      case LtvvOp:
      change = GreaterThanOrZero(x - y);
      break;

      case NeppOp:
      case NepvOp:
      case NevvOp:
      change = x == y;
      break;

      default:
      // assign change to avoid compiler warning
      CPPAD_ASSERT_UNKNOWN( false );
      change = true;
      break;
   }
   if( change )
   {  ++change_number;
      if( change_number == change_count         )
         change_op_index = this_op_index;
   }
}
}}} // END namespace
# endif
