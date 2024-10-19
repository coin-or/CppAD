# ifndef CPPAD_LOCAL_OP_CLASS_ENABLE_PARALLEL_HPP
# define CPPAD_LOCAL_OP_CLASS_ENABLE_PARALLEL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------

// BEGIN_SORT_THIS_LINE_PLUS_1
# include <cppad/local/op_class/add_pv.hpp>
# include <cppad/local/op_class/add_vv.hpp>
# include <cppad/local/op_class/div_pv.hpp>
# include <cppad/local/op_class/div_vp.hpp>
# include <cppad/local/op_class/div_vv.hpp>
# include <cppad/local/op_class/mul_pv.hpp>
# include <cppad/local/op_class/mul_vv.hpp>
# include <cppad/local/op_class/sub_pv.hpp>
# include <cppad/local/op_class/sub_vp.hpp>
# include <cppad/local/op_class/sub_vv.hpp>
// END_SORT_THIS_LINE_MINUS_1
//

namespace CppAD { namespace local { namespace op_class { // BEGIN namespace
template <class Base> void enable_parallel(void)
{  //
   // BEGIN_SORT_THIS_LINE_PLUS_1
   add_pv_t<Base>::get_instance();
   add_vv_t<Base>::get_instance();
   div_pv_t<Base>::get_instance();
   div_vp_t<Base>::get_instance();
   div_vv_t<Base>::get_instance();
   mul_pv_t<Base>::get_instance();
   mul_vv_t<Base>::get_instance();
   sub_pv_t<Base>::get_instance();
   sub_vp_t<Base>::get_instance();
   sub_vv_t<Base>::get_instance();
   // END_SORT_THIS_LINE_MINUS_1
}
}}} // END namespace

# undef CPPAD_OP_CLASS_INSTANCE
# endif
