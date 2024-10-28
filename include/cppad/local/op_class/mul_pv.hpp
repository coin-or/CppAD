# ifndef CPPAD_LOCAL_OP_CLASS_MUL_PV_HPP
# define CPPAD_LOCAL_OP_CLASS_MUL_PV_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/op_class/var_binary_op.hpp>

namespace CppAD { namespace local { namespace op_class { // BEGIN namespace
template <class Base> class mul_pv_t : public var_binary_op_t<Base>
{
public:
   //
   // op2enum
   OpCode op2enum(void) const override
   {  return MulpvOp; }
   //
   // n_res
   size_t n_res(void) const override
   {  return 1; }
   //
   // get_instance
   static mul_pv_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static mul_pv_t instance;
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
      CPPAD_ASSERT_UNKNOWN( NumArg(MulpvOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(MulpvOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );
      CPPAD_ASSERT_UNKNOWN( p <= q );

      // Taylor coefficients corresponding to arguments and result
      Base* y = taylor + size_t(arg[1]) * cap_order;
      Base* z = taylor + i_z    * cap_order;

      // Paraemter value
      Base x = parameter[ arg[0] ];

      for(size_t d = p; d <= q; d++)
         z[d] = x * y[d];
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
      CPPAD_ASSERT_UNKNOWN( NumArg(MulpvOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(MulpvOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( 0 < q );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );

      // Taylor coefficients corresponding to arguments and result
      size_t num_taylor_per_var = (cap_order-1) * r + 1;
      size_t m                  = (q-1) * r + 1;
      Base* y = taylor + size_t(arg[1]) * num_taylor_per_var + m;
      Base* z = taylor + i_z    * num_taylor_per_var + m;

      // Paraemter value
      Base x = parameter[ arg[0] ];

      for(size_t ell = 0; ell < r; ell++)
         z[ell] = x * y[ell];
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
      CPPAD_ASSERT_UNKNOWN( NumArg(MulpvOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(MulpvOp) == 1 );

      // Paraemter value
      Base x = parameter[ arg[0] ];

      // Taylor coefficients corresponding to arguments and result
      Base* y = taylor + size_t(arg[1]) * cap_order;
      Base* z = taylor + i_z    * cap_order;

      z[0] = x * y[0];
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
      CPPAD_ASSERT_UNKNOWN( NumArg(MulpvOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(MulpvOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( d < cap_order );
      CPPAD_ASSERT_UNKNOWN( d < nc_partial );

      // Arguments
      Base x  = parameter[ arg[0] ];

      // Partial derivatives corresponding to arguments and result
      Base* py = partial + size_t(arg[1]) * nc_partial;
      Base* pz = partial + i_z    * nc_partial;

      // number of indices to access
      size_t j = d + 1;
      while(j)
      {  --j;
         // must use azmul because pz[j] = 0 may mean that this
         // component of the function was not selected.
         py[j] += azmul(pz[j], x);
      }
   }
};
}}} // END namespace
# endif
