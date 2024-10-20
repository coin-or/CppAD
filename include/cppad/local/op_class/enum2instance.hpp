# ifndef CPPAD_LOCAL_OP_CLASS_ENUM2INSTANCE_HPP
# define CPPAD_LOCAL_OP_CLASS_ENUM2INSTANCE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/local/op_class/binary_op.hpp>

// BEGIN_SORT_THIS_LINE_PLUS_1
# include <cppad/local/op_class/add_pv.hpp>
# include <cppad/local/op_class/add_vv.hpp>
# include <cppad/local/op_class/div_pv.hpp>
# include <cppad/local/op_class/div_vp.hpp>
# include <cppad/local/op_class/div_vv.hpp>
# include <cppad/local/op_class/mul_pv.hpp>
# include <cppad/local/op_class/mul_vv.hpp>
# include <cppad/local/op_class/pow_vp.hpp>
# include <cppad/local/op_class/sub_pv.hpp>
# include <cppad/local/op_class/sub_vp.hpp>
# include <cppad/local/op_class/sub_vv.hpp>
# include <cppad/local/op_class/zmul_pv.hpp>
# include <cppad/local/op_class/zmul_vv.hpp>
// END_SORT_THIS_LINE_MINUS_1
//
# define CPPAD_OP_CLASS_INSTANCE(op_lower, OpCode)\
   case OpCode: \
   result = op_lower##_t<Base>::get_instance(); \
   break;

namespace CppAD { namespace local { namespace op_class { // BEGIN namespace
template <class Base>
base_op_t<Base>* enum2instance(OpCode op_enum)
{  //
   base_op_t<Base>* result;
   switch(op_enum)
   {
      default:
      assert( false );
      result = nullptr; // set in this case to avoid compiler warning
      break;

      // BEGIN_SORT_THIS_LINE_PLUS_1
      CPPAD_OP_CLASS_INSTANCE(add_pv, AddpvOp)
      CPPAD_OP_CLASS_INSTANCE(add_vv, AddvvOp)
      CPPAD_OP_CLASS_INSTANCE(div_pv, DivpvOp)
      CPPAD_OP_CLASS_INSTANCE(div_vp, DivvpOp)
      CPPAD_OP_CLASS_INSTANCE(div_vv, DivvvOp)
      CPPAD_OP_CLASS_INSTANCE(mul_pv, MulpvOp)
      CPPAD_OP_CLASS_INSTANCE(mul_vv, MulvvOp)
      CPPAD_OP_CLASS_INSTANCE(pow_vp, PowvpOp)
      CPPAD_OP_CLASS_INSTANCE(sub_pv, SubpvOp)
      CPPAD_OP_CLASS_INSTANCE(sub_vp, SubvpOp)
      CPPAD_OP_CLASS_INSTANCE(sub_vv, SubvvOp)
      CPPAD_OP_CLASS_INSTANCE(zmul_pv, ZmulpvOp)
      CPPAD_OP_CLASS_INSTANCE(zmul_vv, ZmulvvOp)
      // END_SORT_THIS_LINE_MINUS_1
   }
   return result;
}
}}} // END namespace

# undef CPPAD_OP_CLASS_INSTANCE
# endif
