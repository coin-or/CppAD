# ifndef CPPAD_LOCAL_OP_CLASS_POW_VV_HPP
# define CPPAD_LOCAL_OP_CLASS_POW_VV_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/local/op_class/var_binary_op.hpp>

// BEGIN _NAMESPACE
namespace CppAD { namespace local { namespace op_class {
// END _NAMESPACE

// BEGIN POW_VV_T
template <class Base> class pow_vv_t : public var_binary_op_t<Base>
// END POW_VV_T
{
public:
   //
   // op2enum
   OpCode op2enum(void) const override
   {  return PowvvOp; }
   //
   // BEGIN _N_RES
   size_t n_res(void) const override
   {  return 3; }
   // END _N_RES
   //
   // get_instance
   static pow_vv_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static pow_vv_t instance;
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
   {  // arg_tmp
      addr_t arg_tmp[1];
      //
      // convert from final result to first result
      i_z -= 2; // 2 = NumRes(PowvvOp) - 1;

      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(PowvvOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(PowvvOp) == 3 );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );
      CPPAD_ASSERT_UNKNOWN( p <= q );
      CPPAD_ASSERT_UNKNOWN(
         size_t( std::numeric_limits<addr_t>::max() ) >= i_z
      );

      // z_0 = log(x)
      log_v_t<Base>::get_instance()->forward(
         p, q, i_z, arg, parameter, cap_order, taylor
      );
      //
      // z_1 = z_0 * y
      addr_t adr[2];
      adr[0] = addr_t( i_z );
      adr[1] = arg[1];
      mul_vv_t<Base>::get_instance()->forward(
         p, q, i_z+1, adr, parameter, cap_order, taylor
      );
      // z_2 = exp(z_1)
      // final result for zero order case is exactly the same as for Base
      if( p == 0 )
      {  // Taylor coefficients corresponding to arguments and result
         Base* x   = taylor + size_t(arg[0]) * cap_order;
         Base* y   = taylor + size_t(arg[1]) * cap_order;
         Base* z_2 = taylor + (i_z+2) * cap_order;

         z_2[0] = pow(x[0], y[0]);
         p++;
      }
      if( p <= q )
      {  arg_tmp[0]  = addr_t( i_z + 1 );
         exp_v_t<Base>::get_instance()->forward(
            p, q, i_z+2, arg_tmp, parameter, cap_order, taylor
         );
      }
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
   {
      // convert from final result to first result
      i_z -= 2; // 2 = NumRes(PowvvOp) - 1

      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(PowvvOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(PowvvOp) == 3 );
      CPPAD_ASSERT_UNKNOWN( 0 < q );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );
      CPPAD_ASSERT_UNKNOWN(
         size_t( std::numeric_limits<addr_t>::max() ) >= i_z
      );

      // z_0 = log(x)
      log_v_t<Base>::get_instance()->forward_dir(
         q, r, i_z, arg, parameter, cap_order, taylor
      );

      // z_1 = y * z_0
      addr_t adr[2];
      adr[0] = addr_t( i_z );
      adr[1] = arg[1];
      mul_vv_t<Base>::get_instance()->forward_dir(
         q, r, i_z+1, adr, parameter, cap_order, taylor
      );

      // z_2 = exp(z_1)
      adr[0] = addr_t( i_z + 1 );
      exp_v_t<Base>::get_instance()->forward_dir(
         q, r, i_z+2, adr, parameter, cap_order, taylor
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
   {
      // convert from final result to first result
      i_z -= 2; // NumRes(PowvvOp) - 1;

      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(PowvvOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(PowvvOp) == 3 );

      // Taylor coefficients corresponding to arguments and result
      Base* x   = taylor + size_t(arg[0]) * cap_order;
      Base* y   = taylor + size_t(arg[1]) * cap_order;
      Base* z_0 = taylor + i_z    * cap_order;
      Base* z_1 = z_0    +          cap_order;
      Base* z_2 = z_1    +          cap_order;

      z_0[0] = log( x[0] );
      z_1[0] = z_0[0] * y[0];
      z_2[0] = pow(x[0], y[0]);

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
   {  addr_t arg_tmp[2];

      // convert from final result to first result
      i_z -= 2; // NumRes(PowvvOp) - 1;

      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(PowvvOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(PowvvOp) == 3 );
      CPPAD_ASSERT_UNKNOWN( d < cap_order );
      CPPAD_ASSERT_UNKNOWN( d < nc_partial );
      CPPAD_ASSERT_UNKNOWN(
         size_t( std::numeric_limits<addr_t>::max() ) >= i_z
      );

      // z_2 = exp(z_1)
      arg_tmp[0]  = addr_t( i_z + 1 );
      exp_v_t<Base>::get_instance()->reverse(
         d, i_z+2, arg_tmp, parameter, cap_order, taylor, nc_partial, partial
      );

      // z_1 = z_0 * y
      arg_tmp[0] = addr_t( i_z );
      arg_tmp[1] = arg[1];
      mul_vv_t<Base>::get_instance()->reverse(
         d, i_z+1, arg_tmp, parameter, cap_order, taylor, nc_partial, partial
      );

      // z_0 = log(x)
      log_v_t<Base>::get_instance()->reverse(
         d, i_z, arg, parameter, cap_order, taylor, nc_partial, partial
      );
   }
};
}}} // END namespace
# endif
