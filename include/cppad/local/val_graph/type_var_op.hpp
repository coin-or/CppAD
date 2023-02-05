# ifndef  CPPAD_LOCAL_VAL_GRAPH_TYPE_VAR_OP_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_TYPE_VAR_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
-------------------------------------------------------------------------------
{xrst_begin_parent type_var_op dev}

Types of Variable Operators
###########################

{xrst_end type_var_op}
-------------------------------------------------------------------------------
{xrst_begin unary_var_op dev}
{xrst_spell
   operands
}

Is This a Unary Variable Operator
#################################

Prototype
*********
{xrst_literal
   // BEGIN_UNARY_VAR_OP
   // END_UNARY_VAR_OP
}

var_op
******
is the variable operator.

Return
******
The return is true if this operator has one operands and is recorded by
specifying the operator and variable address of the operand on the tape.

{xrst_end unary_var_op}
*/
namespace CppAD { namespace local { namespace val_graph {

// BEGIN_UNARY_VAR_OP
inline bool unary_var_op(OpCode var_op)
// END_UNARY_VAR_OP
{  bool result;
   switch(var_op)
   {  default:
      result = false;
      break;
      //
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
      result = true;
      break;
   }
   return result;
}
/*
-------------------------------------------------------------------------------
{xrst_begin binary_var_op dev}
{xrst_spell
   operands
}

Is This a Binary Variable Operator
#################################

Prototype
*********
{xrst_literal
   // BEGIN_BINARY_VAR_OP
   // END_BINARY_VAR_OP
}

var_op
******
is the variable operator.

Return
******
The return is true if this operator has two operands and is recorded by
specifying the operator and variable or parameter address for each
of the operands.

{xrst_end binary_var_op}
*/
// BEGIN_BINARY_VAR_OP
inline bool binary_var_op(OpCode var_op)
// END_BINARY_VAR_OP
{  bool result;
   switch(var_op)
   {  default:
      result = false;
      break;
      //
      case AddpvOp:
      case AddvvOp:
      case DivpvOp:
      case DivvpOp:
      case DivvvOp:
      case MulpvOp:
      case MulvvOp:
      case PowvpOp:
      case PowvvOp:
      case SubpvOp:
      case SubvpOp:
      case SubvvOp:
      case ZmulpvOp:
      case ZmulvpOp:
      case ZmulvvOp:
      result = true;
      break;
   }
   return result;
}
/*
-------------------------------------------------------------------------------
{xrst_begin compare_var_op dev}
{xrst_spell
   operands
}

Is This a Comparison Variable Operator
######################################

Prototype
*********
{xrst_literal
   // BEGIN_COMPARE_VAR_OP
   // END_COMPARE_VAR_OP
}

var_op
******
is the variable operator.

Return
******
The return is true if this operator is a comparison operator.

{xrst_end compare_var_op}
*/
// BEGIN_COMPARE_VAR_OP
inline bool compare_var_op(OpCode var_op)
// END_COMPARE_VAR_OP
{  bool result;
   switch(var_op)
   {  default:
      result = false;
      break;
      //
      case local::EqppOp:
      case local::EqpvOp:
      case local::EqvvOp:
      case local::NeppOp:
      case local::NepvOp:
      case local::NevvOp:
      case local::LtppOp:
      case local::LtpvOp:
      case local::LtvpOp:
      case local::LtvvOp:
      case local::LeppOp:
      case local::LepvOp:
      case local::LevpOp:
      case local::LevvOp:
      result = true;
      break;
   }
   return result;
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
