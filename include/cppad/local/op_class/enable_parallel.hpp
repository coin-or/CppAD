# ifndef CPPAD_LOCAL_OP_CLASS_ENABLE_PARALLEL_HPP
# define CPPAD_LOCAL_OP_CLASS_ENABLE_PARALLEL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------

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
# include <cppad/local/op_class/pow_pv.hpp>
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
# include <cppad/local/op_class/zmul_vp.hpp>
// END_SORT_THIS_LINE_MINUS_1
//

namespace CppAD { namespace local { namespace op_class { // BEGIN namespace
template <class Base> void enable_parallel(void)
{  //
   // BEGIN_SORT_THIS_LINE_PLUS_1
   abs_v_t<Base>::get_instance();
   acos_v_t<Base>::get_instance();
   acosh_v_t<Base>::get_instance();
   add_pv_t<Base>::get_instance();
   add_vv_t<Base>::get_instance();
   asin_v_t<Base>::get_instance();
   asinh_v_t<Base>::get_instance();
   atan_v_t<Base>::get_instance();
   atanh_v_t<Base>::get_instance();
   cos_v_t<Base>::get_instance();
   cosh_v_t<Base>::get_instance();
   div_pv_t<Base>::get_instance();
   div_vp_t<Base>::get_instance();
   div_vv_t<Base>::get_instance();
   exp_v_t<Base>::get_instance();
   expm1_v_t<Base>::get_instance();
   log1p_v_t<Base>::get_instance();
   log_v_t<Base>::get_instance();
   mul_pv_t<Base>::get_instance();
   mul_vv_t<Base>::get_instance();
   neg_v_t<Base>::get_instance();
   pow_pv_t<Base>::get_instance();
   pow_vv_t<Base>::get_instance();
   sign_v_t<Base>::get_instance();
   sin_v_t<Base>::get_instance();
   sinh_v_t<Base>::get_instance();
   sqrt_v_t<Base>::get_instance();
   sub_pv_t<Base>::get_instance();
   sub_vp_t<Base>::get_instance();
   sub_vv_t<Base>::get_instance();
   tan_v_t<Base>::get_instance();
   tanh_v_t<Base>::get_instance();
   zmul_vp_t<Base>::get_instance();
   // END_SORT_THIS_LINE_MINUS_1
}
}}} // END namespace

# undef CPPAD_OP_CLASS_INSTANCE
# endif
