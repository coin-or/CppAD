# ifndef CPPAD_LOCAL_OP_CLASS_OP_ENUM2INSTANCE_HPP
# define CPPAD_LOCAL_OP_CLASS_OP_ENUM2INSTANCE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/local/op_class/base_op.hpp>
# include <cppad/local/op_class/addvv_op.hpp>
//
# define CPPAD_OP_CLASS_INSTANCE(lower, Mixed)\
   case Mixed: \
   result = lower##_op_t<Base>::get_instance(); \
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

      CPPAD_OP_CLASS_INSTANCE(addvv, AddvvOp)
   }
   return result;
}
}} // END namespace

# undef CPPAD_OP_CLASS_INSTANCE
# endif
