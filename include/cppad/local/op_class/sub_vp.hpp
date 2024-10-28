# ifndef CPPAD_LOCAL_OP_CLASS_SUB_VP_HPP
# define CPPAD_LOCAL_OP_CLASS_SUB_VP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/op_class/var_binary_op.hpp>

namespace CppAD { namespace local { namespace op_class { // BEGIN namespace
template <class Base> class sub_vp_t : public var_binary_op_t<Base>
{
public:
   //
   // op2enum
   OpCode op2enum(void) const override
   {  return SubvpOp; }
   //
   // n_res
   size_t n_res(void) const override
   {  return 1; }
   //
   // get_instance
   static sub_vp_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static sub_vp_t instance;
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
   {
      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(SubvpOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(SubvpOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );
      CPPAD_ASSERT_UNKNOWN( p <= q );

      // Taylor coefficients corresponding to arguments and result
      Base* x = taylor + size_t(arg[0]) * cap_order;
      Base* z = taylor + i_z    * cap_order;

      // Parameter value
      Base y = parameter[ arg[1] ];
      if( p == 0 )
      {  z[0] = x[0] - y;
         p++;
      }
      for(size_t d = p; d <= q; d++)
         z[d] = x[d];
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
   {
      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(SubvpOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(SubvpOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( 0 < q );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );

      // Taylor coefficients corresponding to arguments and result
      size_t num_taylor_per_var = (cap_order-1) * r + 1;
      Base* x = taylor + size_t(arg[0]) * num_taylor_per_var;
      Base* z = taylor + i_z    * num_taylor_per_var;

      // Parameter value
      size_t m = (q-1) * r + 1;
      for(size_t ell = 0; ell < r; ell++)
         z[m+ell] = x[m+ell];
   }
   //
   // forward_0
   static void forward_0(
      size_t        i_z         ,
      const addr_t* arg         ,
      const Base*   parameter   ,
      size_t        cap_order   ,
      Base*         taylor      )
   {
      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(SubvpOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(SubvpOp) == 1 );

      // Parameter value
      Base y = parameter[ arg[1] ];

      // Taylor coefficients corresponding to arguments and result
      Base* x = taylor + size_t(arg[0]) * cap_order;
      Base* z = taylor + i_z    * cap_order;

      z[0] = x[0] - y;
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
   {
      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(SubvpOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(SubvpOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( d < cap_order );
      CPPAD_ASSERT_UNKNOWN( d < nc_partial );

      // Partial derivatives corresponding to arguments and result
      Base* px = partial + size_t(arg[0]) * nc_partial;
      Base* pz = partial + i_z    * nc_partial;

      // number of indices to access
      size_t i = d + 1;
      while(i)
      {  --i;
         px[i] += pz[i];
      }
   }
};
}}} // END namespace
# endif
