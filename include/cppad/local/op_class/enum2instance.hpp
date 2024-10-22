# ifndef CPPAD_LOCAL_OP_CLASS_ENUM2INSTANCE_HPP
# define CPPAD_LOCAL_OP_CLASS_ENUM2INSTANCE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin var_enum2instance dev}

Map a Variable Op Codes to a Base Class Instance
################################################

Syntax
******
| *var_base_op* = ``enum2instance`` ( *op_code* )

Prototype
*********
{xrst_literal ,
   // BEGIN NAMESPACE , // END NAMESPACE
   // BEGIN ENUM2INSTANCE  , // END ENUM2INSTANCE
}


{xrst_end var_enum2instance}
*/

# include <cppad/local/op_class/var_binary_op.hpp>

// BEGIN_SORT_THIS_LINE_PLUS_1
# include <cppad/local/op_class/abs_v.hpp>
# include <cppad/local/op_class/acos_v.hpp>
# include <cppad/local/op_class/acosh_v.hpp>
# include <cppad/local/op_class/add_pv.hpp>
# include <cppad/local/op_class/add_vv.hpp>
# include <cppad/local/op_class/asin_v.hpp>
# include <cppad/local/op_class/asinh_v.hpp>
# include <cppad/local/op_class/atan_v.hpp>
# include <cppad/local/op_class/atanh_v.hpp>
# include <cppad/local/op_class/cos_v.hpp>
# include <cppad/local/op_class/cosh_v.hpp>
# include <cppad/local/op_class/div_pv.hpp>
# include <cppad/local/op_class/div_vp.hpp>
# include <cppad/local/op_class/div_vv.hpp>
# include <cppad/local/op_class/exp_v.hpp>
# include <cppad/local/op_class/expm1_v.hpp>
# include <cppad/local/op_class/log1p_v.hpp>
# include <cppad/local/op_class/log_v.hpp>
# include <cppad/local/op_class/mul_pv.hpp>
# include <cppad/local/op_class/mul_vv.hpp>
# include <cppad/local/op_class/neg_v.hpp>
# include <cppad/local/op_class/pow_vp.hpp>
# include <cppad/local/op_class/sign_v.hpp>
# include <cppad/local/op_class/sin_v.hpp>
# include <cppad/local/op_class/sinh_v.hpp>
# include <cppad/local/op_class/sqrt_v.hpp>
# include <cppad/local/op_class/sub_pv.hpp>
# include <cppad/local/op_class/sub_vp.hpp>
# include <cppad/local/op_class/sub_vv.hpp>
# include <cppad/local/op_class/tan_v.hpp>
# include <cppad/local/op_class/tanh_v.hpp>
# include <cppad/local/op_class/zmul_pv.hpp>
# include <cppad/local/op_class/zmul_vv.hpp>
// END_SORT_THIS_LINE_MINUS_1
//
# define CPPAD_OP_CLASS_INSTANCE(op_lower, OpCode)\
   case OpCode: \
   result = op_lower##_t<Base>::get_instance(); \
   break;

// BEGIN NAMESPACE
namespace CppAD { namespace local { namespace op_class {
// END NAMESPACE
// BEGIN ENUM2INSTANCE
template <class Base>
var_base_op_t<Base>* enum2instance(OpCode op_enum)
// END ENUM2INSTANCE
{
   //
   var_base_op_t<Base>* result;
   switch(op_enum)
   {
      default:
      assert( false );
      result = nullptr; // set in this case to avoid compiler warning
      break;

      // BEGIN_SORT_THIS_LINE_PLUS_1
      CPPAD_OP_CLASS_INSTANCE(abs_v, AbsOp)
      CPPAD_OP_CLASS_INSTANCE(acos_v, AcosOp)
      CPPAD_OP_CLASS_INSTANCE(acosh_v, AcoshOp)
      CPPAD_OP_CLASS_INSTANCE(add_pv, AddpvOp)
      CPPAD_OP_CLASS_INSTANCE(add_vv, AddvvOp)
      CPPAD_OP_CLASS_INSTANCE(asin_v, AsinOp)
      CPPAD_OP_CLASS_INSTANCE(asinh_v, AsinhOp)
      CPPAD_OP_CLASS_INSTANCE(atan_v, AtanOp)
      CPPAD_OP_CLASS_INSTANCE(atanh_v, AtanhOp)
      CPPAD_OP_CLASS_INSTANCE(cos_v, CosOp)
      CPPAD_OP_CLASS_INSTANCE(cosh_v, CoshOp)
      CPPAD_OP_CLASS_INSTANCE(div_pv, DivpvOp)
      CPPAD_OP_CLASS_INSTANCE(div_vp, DivvpOp)
      CPPAD_OP_CLASS_INSTANCE(div_vv, DivvvOp)
      CPPAD_OP_CLASS_INSTANCE(exp_v, ExpOp)
      CPPAD_OP_CLASS_INSTANCE(expm1_v, Expm1Op)
      CPPAD_OP_CLASS_INSTANCE(log1p_v, Log1pOp)
      CPPAD_OP_CLASS_INSTANCE(log_v, LogOp)
      CPPAD_OP_CLASS_INSTANCE(mul_pv, MulpvOp)
      CPPAD_OP_CLASS_INSTANCE(mul_vv, MulvvOp)
      CPPAD_OP_CLASS_INSTANCE(neg_v, NegOp)
      CPPAD_OP_CLASS_INSTANCE(pow_vp, PowvpOp)
      CPPAD_OP_CLASS_INSTANCE(sign_v, SignOp)
      CPPAD_OP_CLASS_INSTANCE(sin_v, SinOp)
      CPPAD_OP_CLASS_INSTANCE(sinh_v, SinhOp)
      CPPAD_OP_CLASS_INSTANCE(sqrt_v, SqrtOp)
      CPPAD_OP_CLASS_INSTANCE(sub_pv, SubpvOp)
      CPPAD_OP_CLASS_INSTANCE(sub_vp, SubvpOp)
      CPPAD_OP_CLASS_INSTANCE(sub_vv, SubvvOp)
      CPPAD_OP_CLASS_INSTANCE(tan_v, TanOp)
      CPPAD_OP_CLASS_INSTANCE(tanh_v, TanhOp)
      CPPAD_OP_CLASS_INSTANCE(zmul_pv, ZmulpvOp)
      CPPAD_OP_CLASS_INSTANCE(zmul_vv, ZmulvvOp)
      // END_SORT_THIS_LINE_MINUS_1
   }
   return result;
}
}}} // END namespace

# undef CPPAD_OP_CLASS_INSTANCE
# endif
