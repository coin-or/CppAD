# ifndef CPPAD_LOCAL_OP_CLASS_SUB_VV_HPP
# define CPPAD_LOCAL_OP_CLASS_SUB_VV_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/op_class/var_binary_op.hpp>

namespace CppAD { namespace local { namespace op_class { // BEGIN namespace
template <class Base> class sub_vv_t : public var_binary_op_t<Base>
{
public:
   //
   // op2enum
   OpCode op2enum(void) const override
   {  return SubvvOp; }
   //
   // get_instance
   static sub_vv_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static sub_vv_t instance;
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
      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(SubvvOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(SubvvOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );
      CPPAD_ASSERT_UNKNOWN( p <= q );

      // Taylor coefficients corresponding to arguments and result
      Base* x = taylor + size_t(arg[0]) * cap_order;
      Base* y = taylor + size_t(arg[1]) * cap_order;
      Base* z = taylor + i_z    * cap_order;

      for(size_t d = p; d <= q; d++)
         z[d] = x[d] - y[d];
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
      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(SubvvOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(SubvvOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( 0 < q );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );

      // Taylor coefficients corresponding to arguments and result
      size_t num_taylor_per_var = (cap_order-1) * r + 1;
      size_t m                  = (q-1) * r + 1;
      Base* x = taylor + size_t(arg[0]) * num_taylor_per_var + m;
      Base* y = taylor + size_t(arg[1]) * num_taylor_per_var + m;
      Base* z = taylor + i_z    * num_taylor_per_var + m;

      for(size_t ell = 0; ell < r; ell++)
         z[ell] = x[ell] - y[ell];
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
      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(SubvvOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(SubvvOp) == 1 );

      // Taylor coefficients corresponding to arguments and result
      Base* x = taylor + size_t(arg[0]) * cap_order;
      Base* y = taylor + size_t(arg[1]) * cap_order;
      Base* z = taylor + i_z    * cap_order;

      z[0] = x[0] - y[0];
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
      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(SubvvOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(SubvvOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( d < cap_order );
      CPPAD_ASSERT_UNKNOWN( d < nc_partial );

      // Partial derivatives corresponding to arguments and result
      Base* px = partial + size_t(arg[0]) * nc_partial;
      Base* py = partial + size_t(arg[1]) * nc_partial;
      Base* pz = partial + i_z    * nc_partial;

      // number of indices to access
      size_t i = d + 1;
      while(i)
      {  --i;
         px[i] += pz[i];
         py[i] -= pz[i];
      }
   }
};
}}} // END namespace
# endif
