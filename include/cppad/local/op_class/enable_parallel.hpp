# ifndef CPPAD_LOCAL_OP_CLASS_ENABLE_PARALLEL_HPP
# define CPPAD_LOCAL_OP_CLASS_ENABLE_PARALLEL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/local/op_class/enum2op.hpp>

namespace CppAD { namespace local { namespace op_class { // BEGIN namespace
template <class Base> void enable_parallel(void)
{  //
   // calls get_instance() for all var_base_op<Base> operators
   enum2op<Base>( AbsOp );
}
}}} // END namespace

# undef CPPAD_OP_CLASS_INSTANCE
# endif
