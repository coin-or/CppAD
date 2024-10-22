# ifndef CPPAD_LOCAL_OP_CLASS_POW_PV_HPP
# define CPPAD_LOCAL_OP_CLASS_POW_PV_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/local/op_class/var_binary_op.hpp>

namespace CppAD { namespace local { namespace op_class { // BEGIN namespace
template <class Base> class pow_pv_t : public var_binary_op_t<Base>
{
public:
   //
   // op2enum
   OpCode op2enum(void) const override
   {  return PowpvOp; }
   //
   // n_res
   size_t n_res(void) const override
   {  return 3; }
   //
   // get_instance
   static pow_pv_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static pow_pv_t instance;
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
   {
      // convert from final result to first result
      i_z -= 2; // 2 = NumRes(PowpvOp) - 1;

      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(PowpvOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(PowpvOp) == 3 );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );
      CPPAD_ASSERT_UNKNOWN( p <= q );

      // Taylor coefficients corresponding to arguments and result
      Base* z_0 = taylor + i_z    * cap_order;

      // z_0 = log(x)
      Base x    = parameter[ arg[0] ];
      size_t d;
      for(d = p; d <= q; d++)
      {  if( d == 0 )
            z_0[d] = log(x);
         else
            z_0[d] = Base(0.0);
      }

      // 2DO: remove requirement that i_z * cap_order <= max addr_t value
      CPPAD_ASSERT_KNOWN(
         size_t( std::numeric_limits<addr_t>::max() ) >= i_z * cap_order,
         "cppad_tape_addr_type maximum value has been exceeded\n"
         "This is due to a kludge in the pow operation and should be fixed."
      );

      // z_1 = z_0 * y
      addr_t adr[2];
      // offset of z_i in taylor (as if it were a parameter); i.e., log(x)
      adr[0] = addr_t( i_z * cap_order );
      // offset of y in taylor (as a variable)
      adr[1] = arg[1];

      // Trick: use taylor both for the parameter vector and variable values
      forward_mulpv_op(p, q, i_z+1, adr, taylor, cap_order, taylor);

      // z_2 = exp(z_1)
      // zero order case exactly same as Base type operation
      if( p == 0 )
      {  Base* y   = taylor + size_t(arg[1]) * cap_order;
         Base* z_2 = taylor + (i_z+2) * cap_order;
         z_2[0] = pow(x, y[0]);
         p++;
      }
      if( p <= q )
         forward_exp_op(p, q, i_z+2, i_z+1, cap_order, taylor);
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
      i_z -= 2; // 2 = NumRes(PowpvOp) - 1;

      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(PowpvOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(PowpvOp) == 3 );
      CPPAD_ASSERT_UNKNOWN( 0 < q );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );

      // Taylor coefficients corresponding to arguments and result
      size_t num_taylor_per_var = (cap_order-1) * r + 1;
      Base* z_0 = taylor + i_z * num_taylor_per_var;

      // z_0 = log(x)
      size_t m  = (q-1) * r + 1;
      for(size_t ell = 0; ell < r; ell++)
         z_0[m+ell] = Base(0.0);

      // 2DO: remove requirement i_z * num_taylor_per_var <= max addr_t value
      CPPAD_ASSERT_KNOWN(
         size_t( std::numeric_limits<addr_t>::max() ) >= i_z * num_taylor_per_var,
         "cppad_tape_addr_type maximum value has been exceeded\n"
         "This is due to a kludge in the pow operation and should be fixed."
      );

      // z_1 = z_0 * y
      addr_t adr[2];
      // offset of z_0 in taylor (as if it were a parameter); i.e., log(x)
      adr[0] = addr_t( i_z * num_taylor_per_var );
      // ofset of y in taylor (as a variable)
      adr[1] = arg[1];

      // Trick: use taylor both for the parameter vector and variable values
      forward_mulpv_op_dir(q, r, i_z+1, adr, taylor, cap_order, taylor);

      // z_2 = exp(z_1)
      forward_exp_op_dir(q, r, i_z+2, i_z+1, cap_order, taylor);
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
      i_z -= 2; // NumRes(PowpvOp) - 1;

      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(PowpvOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(PowpvOp) == 3 );

      // Paraemter value
      Base x = parameter[ arg[0] ];

      // Taylor coefficients corresponding to arguments and result
      Base* y   = taylor + size_t(arg[1]) * cap_order;
      Base* z_0 = taylor + i_z    * cap_order;
      Base* z_1 = z_0    +          cap_order;
      Base* z_2 = z_1    +          cap_order;

      // z_0 = log(x)
      z_0[0] = log(x);

      // z_1 = z_0 * y
      z_1[0] = z_0[0] * y[0];

      // z_2 = exp(z_1)
      // zero order case exactly same as Base type operation
      z_2[0] = pow(x, y[0]);
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
   {
      // convert from final result to first result
      i_z -= 2; // NumRes(PowpvOp) - 1;

      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(PowvvOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(PowvvOp) == 3 );
      CPPAD_ASSERT_UNKNOWN( d < cap_order );
      CPPAD_ASSERT_UNKNOWN( d < nc_partial );

      // z_2 = exp(z_1)
      reverse_exp_op(
         d, i_z+2, i_z+1, cap_order, taylor, nc_partial, partial
      );

      // 2DO: remove requirement that i_z * cap_order <= max addr_t value
      CPPAD_ASSERT_KNOWN(
         size_t( std::numeric_limits<addr_t>::max() ) >= i_z * cap_order,
         "cppad_tape_addr_type maximum value has been exceeded\n"
         "This is due to a kludge in the pow operation and should be fixed."
      );

      // z_1 = z_0 * y
      addr_t adr[2];
      adr[0] = addr_t( i_z * cap_order ); // offset of z_0[0] in taylor
      adr[1] = arg[1];                    // index of y in taylor and partial
      // use taylor both for parameter and variable values
      reverse_mulpv_op(
         d, i_z+1, adr, taylor, cap_order, taylor, nc_partial, partial
      );

      // z_0 = log(x)
      // x is a parameter
   }
};
}}} // END namespace
# endif
