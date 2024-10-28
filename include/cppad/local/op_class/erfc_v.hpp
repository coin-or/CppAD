# ifndef CPPAD_LOCAL_OP_CLASS_ERFC_V_HPP
# define CPPAD_LOCAL_OP_CLASS_ERFC_V_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/op_class/erf.hpp>

// BEGIN NAMESPACE
namespace CppAD { namespace local { namespace op_class {
// END NAMESPACE

// BEGIN ERFC_V_T
template <class Base> class erfc_v_t : public var_unary_op_t<Base>
// END ERFC_V_T
{
public:
   //
   // op2enum
   OpCode op2enum(void) const override
   {  return ErfcOp; }
   //
   // n_arg
   size_t n_arg(void) const override
   {  return 2; }
   //
   // n_res
   size_t n_res(void) const override
   {  return 5; }
   //
   // get_instance
   static erfc_v_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static erfc_v_t instance;
      CPPAD_ASSERT_NARG_NRES(
         instance.op2enum(), instance.n_arg(), instance.n_res()
      );
      return &instance;
   }
   //
   // forward
   static void forward(
      size_t        p           ,
      size_t        q           ,
      size_t        i_z         ,
      const addr_t* arg         ,
      const Base*   parameter   ,
      size_t        cap_order   ,
      Base*         taylor      )
   {  forward_erf_op(
         ErfcOp, p, q, i_z, arg, parameter, cap_order, taylor
      );
   }
   //
   // forward_dir
   static void forward_dir(
      size_t        q           ,
      size_t        r           ,
      size_t        i_z         ,
      const addr_t* arg         ,
      const Base*   parameter   ,
      size_t        cap_order   ,
      Base*         taylor      )
   {  forward_erf_op_dir(
         ErfcOp, q, r, i_z, arg, parameter, cap_order, taylor
      );
   }
   //
   // forward_0
   static void forward_0(
      size_t        i_z         ,
      const addr_t* arg         ,
      const Base*   parameter   ,
      size_t        cap_order   ,
      Base*         taylor      )
   {  forward_erf_op_0(
         ErfcOp, i_z, arg, parameter, cap_order, taylor
      );
   }
   //
   // reverse
   static void reverse(
      size_t        d           ,
      size_t        i_z         ,
      const addr_t* arg         ,
      const Base*   parameter   ,
      size_t        cap_order   ,
      const Base*   taylor      ,
      size_t        nc_partial  ,
      Base*         partial     )
   {  reverse_erf_op(
         ErfcOp, d, i_z, arg, parameter, cap_order, taylor, nc_partial, partial
      );
   }
};


}}} // END namespace
# endif
