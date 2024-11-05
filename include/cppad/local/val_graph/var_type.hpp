# ifndef  CPPAD_LOCAL_VAL_GRAPH_VAR_TYPE_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_VAR_TYPE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
-------------------------------------------------------------------------------
{xrst_begin type_var_op dev}

Types of Variable Operators
###########################

Prototype
*********
{xrst_literal
   // BEGIN_TYPE_VAR_OP
   // END_TYPE_VAR_OP
}

var_op
******
is the variable operator.

is_unary
********
If the return value for is_unary is true,
this operator has one operand, one result, and is recorded by
specifying the operator and variable address of the operand on the tape.

is_binary
*********
If the return value for is_binary is true,
this operator has two operands, one result, and is recorded by
specifying the operator and variable or parameter address for each operand.

is_compare
**********
If the return value for is_compare is true, this is a compare operator.
It has two operands, no result, and is recorded by
specifying the operator and variable or parameter address for each operand.

{xrst_end type_var_op}
*/
namespace CppAD { namespace local { namespace val_graph {

// BEGIN_TYPE_VAR_OP
inline void type_var_op(
   op_code_var var_op     ,
   bool& is_unary    ,
   bool& is_binary   ,
   bool& is_compare  )
// END_TYPE_VAR_OP
{  //
   switch(var_op)
   {  //
      default:
      CPPAD_ASSERT_UNKNOWN(false);
      break;
      // BEGIN_SORT_THIS_LINE_PLUS_1
      case AFunOp:
      case BeginOp:
      case CExpOp:
      case CSumOp:
      case DisOp:
      case EndOp:
      case FunapOp:
      case FunavOp:
      case FunrpOp:
      case FunrvOp:
      case InvOp:
      case LdpOp:
      case LdvOp:
      case ParOp:
      case PriOp:
      case StppOp:
      case StpvOp:
      case StvpOp:
      case StvvOp:
      // END_SORT_THIS_LINE_MINUS_1
      is_unary   = false;
      is_binary  = false;
      is_compare = false;
      break;
      //
      // unary operators
      // BEGIN_SORT_THIS_LINE_PLUS_1
      case AbsOp:
      case AcosOp:
      case AcoshOp:
      case AsinOp:
      case AsinhOp:
      case AtanOp:
      case AtanhOp:
      case CosOp:
      case CoshOp:
      case ErfOp:
      case ErfcOp:
      case ExpOp:
      case Expm1Op:
      case Log1pOp:
      case LogOp:
      case NegOp:
      case SignOp:
      case SinOp:
      case SinhOp:
      case SqrtOp:
      case TanOp:
      case TanhOp:
      // END_SORT_THIS_LINE_MINUS_1
      is_unary   = true;
      is_binary  = false;
      is_compare = false;
      break;
      //
      // binary operators
      // BEGIN_SORT_THIS_LINE_PLUS_1
      case AddpvOp:
      case AddvvOp:
      case DivpvOp:
      case DivvpOp:
      case DivvvOp:
      case MulpvOp:
      case MulvvOp:
      case PowpvOp:
      case PowvpOp:
      case PowvvOp:
      case SubpvOp:
      case SubvpOp:
      case SubvvOp:
      case ZmulpvOp:
      case ZmulvpOp:
      case ZmulvvOp:
      // END_SORT_THIS_LINE_MINUS_1
      is_unary   = false;
      is_binary  = true;
      is_compare = false;
      break;
      //
      // compare operators
      // BEGIN_SORT_THIS_LINE_PLUS_1
      case local::EqppOp:
      case local::EqpvOp:
      case local::EqvvOp:
      case local::LeppOp:
      case local::LepvOp:
      case local::LevpOp:
      case local::LevvOp:
      case local::LtppOp:
      case local::LtpvOp:
      case local::LtvpOp:
      case local::LtvvOp:
      case local::NeppOp:
      case local::NepvOp:
      case local::NevvOp:
      // END_SORT_THIS_LINE_MINUS_1
      is_unary   = false;
      is_binary  = false;
      is_compare = true;
      break;
   }
   return;
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
