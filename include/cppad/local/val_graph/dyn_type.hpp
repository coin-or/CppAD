# ifndef  CPPAD_LOCAL_VAL_GRAPH_DYN_TYPE_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_DYN_TYPE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
-------------------------------------------------------------------------------
{xrst_begin_parent type_dyn_op dev}

Types of Dynamic Parameter Operators
####################################

{xrst_end type_dyn_op}
-------------------------------------------------------------------------------
{xrst_begin unary_dyn_op dev}

Is This a Binary Dynamic Parameter Operator
###########################################

Prototype
*********
{xrst_literal
   // BEGIN_UNARY_DYN_OP
   // END_UNARY_DYN_OP
}

dyn_op
******
is the dynamic parameter operator.

Return
******
The return is true if this operator has one operands and is recorded by
specifying the operator and parameter address of the operand on the tape.

{xrst_end unary_dyn_op}
*/
namespace CppAD { namespace local { namespace val_graph {

// BEGIN_UNARY_DYN_OP
inline bool unary_dyn_op(op_code_dyn dyn_op)
// END_UNARY_DYN_OP
{  bool result;
   switch(dyn_op)
   {  default:
      result = false;
      break;
      //
      case abs_dyn:
      case acos_dyn:
      case acosh_dyn:
      case asin_dyn:
      case asinh_dyn:
      case atan_dyn:
      case atanh_dyn:
      case cos_dyn:
      case cosh_dyn:
      case erf_dyn:
      case erfc_dyn:
      case exp_dyn:
      case expm1_dyn:
      case fabs_dyn:
      case log1p_dyn:
      case log_dyn:
      case neg_dyn:
      case sign_dyn:
      case sin_dyn:
      case sinh_dyn:
      case sqrt_dyn:
      case tan_dyn:
      case tanh_dyn:
      result = true;
      break;
   }
   return result;
}
/*
-------------------------------------------------------------------------------
{xrst_begin binary_dyn_op dev}

Is This a Binary Dynamic Parameter Operator
###########################################

Prototype
*********
{xrst_literal
   // BEGIN_BINARY_DYN_OP
   // END_BINARY_DYN_OP
}

dyn_op
******
is the dynamic parameter operator.

Return
******
The return is true if this operator has two operands and is recorded by
specifying the operator and parameter address for each of the operands.

{xrst_end binary_dyn_op}
*/
// BEGIN_BINARY_DYN_OP
inline bool binary_dyn_op(op_code_dyn dyn_op)
// END_BINARY_DYN_OP
{  bool result;
   switch(dyn_op)
   {  default:
      result = false;
      break;
      case add_dyn:
      case div_dyn:
      case mul_dyn:
      case pow_dyn:
      case sub_dyn:
      case zmul_dyn:
      result = true;
      break;
   }
   return result;
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
