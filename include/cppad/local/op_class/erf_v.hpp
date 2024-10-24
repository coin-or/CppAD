# ifndef CPPAD_LOCAL_OP_CLASS_ERF_V_HPP
# define CPPAD_LOCAL_OP_CLASS_ERF_V_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/op/erf_op.hpp>

// BEGIN NAMESPACE
namespace CppAD { namespace local { namespace op_class {
// END NAMESPACE

// BEGIN ERF_V
template <class Base> class erf_v_t : public var_base_op_t<Base>
// END ERF_V
{
public:
   //
   // op2enum
   OpCode op2enum(void) const override
   {  return ErfOp; }
   //
   // n_arg
   size_t n_arg(void) const override
   {  return 2; }
   //
   // r_res
   size_t n_res(void) const override
   {  return 5; }
   //
   // get_instance
   static erf_v_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static erf_v_t instance;
      CPPAD_ASSERT_NARG_NRES(
         instance.op2enum(), instance.n_arg(), instance.n_res()
      );
      return &instance;
   }
   //
   // forward
   void forward(
      size_t        p           ,
      size_t        q           ,
      size_t        i_z         ,
      const addr_t* arg         ,
      const Base*   parameter   ,
      size_t        cap_order   ,
      Base*         taylor      ) const override
   {  forward_erf_op(
         ErfOp, p, q, i_z, arg, parameter, cap_order, taylor
      );
   }
   //
   // forward_dir
   void forward_dir(
      size_t        q           ,
      size_t        r           ,
      size_t        i_z         ,
      const addr_t* arg         ,
      const Base*   parameter   ,
      size_t        cap_order   ,
      Base*         taylor      ) const override
   {  forward_erf_op_dir(
         ErfOp, q, r, i_z, arg, parameter, cap_order, taylor
      );
   }
   //
   // forward_0
   void forward_0(
      size_t        i_z         ,
      const addr_t* arg         ,
      const Base*   parameter   ,
      size_t        cap_order   ,
      Base*         taylor      ) const override
   {  forward_erf_op_0(
         ErfOp, i_z, arg, parameter, cap_order, taylor
      );
   }
   //
   // reverse
   void reverse(
      size_t        d           ,
      size_t        i_z         ,
      const addr_t* arg         ,
      const Base*   parameter   ,
      size_t        cap_order   ,
      const Base*   taylor      ,
      size_t        nc_partial  ,
      Base*         partial     ) const override
   {  reverse_erf_op(
         ErfOp, d, i_z, arg, parameter, cap_order, taylor, nc_partial, partial
      );
   }
};


}}} // END namespace
# endif
