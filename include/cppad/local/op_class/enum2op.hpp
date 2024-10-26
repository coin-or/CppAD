# ifndef CPPAD_LOCAL_OP_CLASS_ENUM2OP_HPP
# define CPPAD_LOCAL_OP_CLASS_ENUM2OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin var_enum2op dev}

Map a Variable Op Codes to a Base Class Instance
################################################

Syntax
******
| *var_base_op* = ``enum2op`` ( *op_code* )

Prototype
*********
{xrst_literal ,
   // BEGIN NAMESPACE , // END NAMESPACE
   // BEGIN ENUM2OP  , // END ENUM2OP
}

Parallel Execution
******************
The first call to this routine initializes static data in this
routine and in all of the ``get_instance`` functions corresponding
to classes derived from :ref:`var_base_op_t-name`. 
Hence the first call cannot be in parallel mode.

{xrst_end var_enum2op}
*/

# include <vector>
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
# include <cppad/local/op_class/erf_v.hpp>
# include <cppad/local/op_class/erfc_v.hpp>
# include <cppad/local/op_class/exp_v.hpp>
# include <cppad/local/op_class/expm1_v.hpp>
# include <cppad/local/op_class/log1p_v.hpp>
# include <cppad/local/op_class/log_v.hpp>
# include <cppad/local/op_class/mul_pv.hpp>
# include <cppad/local/op_class/mul_vv.hpp>
# include <cppad/local/op_class/neg_v.hpp>
# include <cppad/local/op_class/pow_pv.hpp>
# include <cppad/local/op_class/pow_vp.hpp>
# include <cppad/local/op_class/pow_vv.hpp>
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
# include <cppad/local/op_class/zmul_vp.hpp>
# include <cppad/local/op_class/zmul_vv.hpp>
// END_SORT_THIS_LINE_MINUS_1
//
// BEGIN NAMESPACE
namespace CppAD { namespace local { namespace op_class {
// END NAMESPACE
// BEGIN ENUM2OP
template <class Base>
inline var_base_op_t<Base>* enum2op(OpCode op_enum)
// END ENUM2OP
{  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
   //
   // op_ptr
   static var_base_op_t<Base>* op_ptr[NumberOp];
   static bool first = true;
   if( first )
   {  first = false;
      for(size_t i = 0; i < size_t(NumberOp); ++i)
         op_ptr[i] = nullptr;

      // op_ptr
      // BEGIN_SORT_THIS_LINE_PLUS_1
      op_ptr[ AbsOp ]     = abs_v_t<Base>::get_instance();
      op_ptr[ AcosOp ]    = acos_v_t<Base>::get_instance();
      op_ptr[ AcoshOp ]   = acosh_v_t<Base>::get_instance();
      op_ptr[ AddpvOp ]   = add_pv_t<Base>::get_instance();
      op_ptr[ AddvvOp ]   = add_vv_t<Base>::get_instance();
      op_ptr[ AsinOp ]    = asin_v_t<Base>::get_instance();
      op_ptr[ AsinhOp ]   = asinh_v_t<Base>::get_instance();
      op_ptr[ AtanOp ]    = atan_v_t<Base>::get_instance();
      op_ptr[ AtanhOp ]   = atanh_v_t<Base>::get_instance();
      op_ptr[ CosOp ]     = cos_v_t<Base>::get_instance();
      op_ptr[ CoshOp ]    = cosh_v_t<Base>::get_instance();
      op_ptr[ DivpvOp ]   = div_pv_t<Base>::get_instance();
      op_ptr[ DivvpOp ]   = div_vp_t<Base>::get_instance();
      op_ptr[ DivvvOp ]   = div_vv_t<Base>::get_instance();
      op_ptr[ ErfOp ]     = erf_v_t<Base>::get_instance();
      op_ptr[ ErfcOp ]    = erfc_v_t<Base>::get_instance();
      op_ptr[ ExpOp ]     = exp_v_t<Base>::get_instance();
      op_ptr[ Expm1Op ]   = expm1_v_t<Base>::get_instance();
      op_ptr[ Log1pOp ]   = log1p_v_t<Base>::get_instance();
      op_ptr[ LogOp ]     = log_v_t<Base>::get_instance();
      op_ptr[ MulpvOp ]   = mul_pv_t<Base>::get_instance();
      op_ptr[ MulvvOp ]   = mul_vv_t<Base>::get_instance();
      op_ptr[ NegOp ]     = neg_v_t<Base>::get_instance();
      op_ptr[ PowpvOp ]   = pow_pv_t<Base>::get_instance();
      op_ptr[ PowvpOp ]   = pow_vp_t<Base>::get_instance();
      op_ptr[ PowvvOp ]   = pow_vv_t<Base>::get_instance();
      op_ptr[ SignOp ]    = sign_v_t<Base>::get_instance();
      op_ptr[ SinOp ]     = sin_v_t<Base>::get_instance();
      op_ptr[ SinhOp ]    = sinh_v_t<Base>::get_instance();
      op_ptr[ SqrtOp ]    = sqrt_v_t<Base>::get_instance();
      op_ptr[ SubpvOp ]   = sub_pv_t<Base>::get_instance();
      op_ptr[ SubvpOp ]   = sub_vp_t<Base>::get_instance();
      op_ptr[ SubvvOp ]   = sub_vv_t<Base>::get_instance();
      op_ptr[ TanOp ]     = tan_v_t<Base>::get_instance();
      op_ptr[ TanhOp ]    = tanh_v_t<Base>::get_instance();
      op_ptr[ ZmulpvOp ]  = zmul_pv_t<Base>::get_instance();
      op_ptr[ ZmulvpOp ]  = zmul_vp_t<Base>::get_instance();
      op_ptr[ ZmulvvOp ]  = zmul_vv_t<Base>::get_instance();
      // END_SORT_THIS_LINE_MINUS_1
   }
   return op_ptr[ op_enum ];
}
}}} // END namespace

# endif
