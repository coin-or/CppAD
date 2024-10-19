# ifndef CPPAD_LOCAL_OP_CLASS_OP_ENUM2INSTANCE_HPP
# define CPPAD_LOCAL_OP_CLASS_OP_ENUM2INSTANCE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/local/op_class/base_op.hpp>

// BEGIN_SORT_THIS_LINE_PLUS_1
# include <cppad/local/op_class/add_pv.hpp>
# include <cppad/local/op_class/add_vv.hpp>
# include <cppad/local/op_class/sub_pv.hpp>
# include <cppad/local/op_class/sub_vp.hpp>
# include <cppad/local/op_class/sub_vv.hpp>
// END_SORT_THIS_LINE_MINUS_1
//
# define CPPAD_OP_CLASS_INSTANCE(op_lower, OpCode)\
   case OpCode: \
   result = op_lower##_t<Base>::get_instance(); \
   break;

namespace CppAD { namespace local { // BEGIN namespace
template <class Base>
base_op_t<Base>* op_enum2instance(OpCode op_enum)
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
      CPPAD_OP_CLASS_INSTANCE(sub_pv, SubpvOp)
      CPPAD_OP_CLASS_INSTANCE(sub_vp, SubvpOp)
      CPPAD_OP_CLASS_INSTANCE(sub_vv, SubvvOp)
      // END_SORT_THIS_LINE_MINUS_1
   }
   return result;
}
}} // END namespace

# undef CPPAD_OP_CLASS_INSTANCE
# endif
