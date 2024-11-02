# ifndef CPPAD_LOCAL_VAR_OP_COMPARE_HPP
# define CPPAD_LOCAL_VAR_OP_COMPARE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ---------------------------------------------------------------------------
/*
{xrst_begin var_compare dev}

Implement Comparison Operators
##############################

Prototype
*********
{xrst_literal
   // BEGIN PROTOTYPE
   // END PROTOTYPE
}

Base
****
is the base type for the operator; i.e., this operation was recorded
using ``AD`` < *Base* > and computations by these operators are done using
type *Base* .

op_enum
*******
is the enum value for the comparison operator that we are implementing.

arg
***
is the vector of argument indices for this operator.
All the comparison operators have two arguments and the corresponding
indices are *arg*\ [0] and *arg*\ [1] .

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

compare_change_count
********************
is the compare_change_number at which *this_op_index* is returned
as *compare_change_op_index*.
If it is zero, the comparison changes are not counted.

compare_change_number
*********************
If *compare_change_count* is zero, this value is not modified.
Otherwise, if this operator comparison has changed (is no longer true),
*compare_change_number* is incremented by one.

compare_change_op_index
***********************
If *compare_change_count* is zero, this value is not modified.
Otherwise, if this operator comparison has changed (is no longer true), and
the new value of *compare_change_number* is equal to *compare_change_count* ,
*compare_change_op_index* is set equal to *this_op_index* .

{xrst_end var_compare}
*/

# include <cppad/local/op_code_var.hpp>
# include <cppad/core/cppad_assert.hpp>

namespace CppAD { namespace local { namespace var_op { // BEGIN namespace

// BEGIN PROTOTYPE
template <class Base> void compare(
   OpCode        op_enum                 ,
   const addr_t* arg                     ,
   const Base*   parameter               ,
   size_t        cap_order               ,
   const Base*   taylor                  ,
   size_t        this_op_index           ,
   size_t        compare_change_count    ,
   size_t&       compare_change_number   ,
   size_t&       compare_change_op_index )
// END PROTOTYPE
{  //
   // n_arg, n_res
   CPPAD_ASSERT_NARG_NRES(op_enum, 2, 0);
   //
   // special case
   if( compare_change_count == 0 )
      return;
   //
   // x, y
   Base x, y;
   switch(op_enum)
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
   switch(op_enum)
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
   {  ++compare_change_number;
      if( compare_change_number == compare_change_count )
         compare_change_op_index = this_op_index;
   }
}
}}} // END namespace
# endif
