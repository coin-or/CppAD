# ifndef CPPAD_LOCAL_OP_CLASS_ZMUL_VV_HPP
# define CPPAD_LOCAL_OP_CLASS_ZMUL_VV_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/local/op_class/var_binary_op.hpp>

namespace CppAD { namespace local { namespace op_class { // BEGIN namespace
template <class Base> class zmul_vv_t : public var_binary_op_t<Base>
{
public:
   //
   // get_instance
   static zmul_vv_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static zmul_vv_t instance;
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
      CPPAD_ASSERT_UNKNOWN( NumArg(ZmulvvOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(ZmulvvOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );
      CPPAD_ASSERT_UNKNOWN( p <= q );

      // Taylor coefficients corresponding to arguments and result
      Base* x = taylor + size_t(arg[0]) * cap_order;
      Base* y = taylor + size_t(arg[1]) * cap_order;
      Base* z = taylor + i_z    * cap_order;

      size_t k;
      for(size_t d = p; d <= q; d++)
      {  z[d] = Base(0.0);
         for(k = 0; k <= d; k++)
            z[d] += azmul(x[d-k], y[k]);
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
      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(ZmulvvOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(ZmulvvOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( 0 < q );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );

      // Taylor coefficients corresponding to arguments and result
      size_t num_taylor_per_var = (cap_order-1) * r + 1;
      Base* x = taylor + size_t(arg[0]) * num_taylor_per_var;
      Base* y = taylor + size_t(arg[1]) * num_taylor_per_var;
      Base* z = taylor +    i_z * num_taylor_per_var;

      size_t k, ell, m;
      for(ell = 0; ell < r; ell++)
      {  m = (q-1)*r + ell + 1;
         z[m] = azmul(x[0], y[m]) + azmul(x[m],  y[0]);
         for(k = 1; k < q; k++)
            z[m] += azmul(x[(q-k-1)*r + ell + 1], y[(k-1)*r + ell + 1]);
      }
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
      CPPAD_ASSERT_UNKNOWN( NumArg(ZmulvvOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(ZmulvvOp) == 1 );

      // Taylor coefficients corresponding to arguments and result
      Base* x = taylor + size_t(arg[0]) * cap_order;
      Base* y = taylor + size_t(arg[1]) * cap_order;
      Base* z = taylor + i_z    * cap_order;

      z[0] = azmul(x[0], y[0]);
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
      CPPAD_ASSERT_UNKNOWN( NumArg(ZmulvvOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(ZmulvvOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( d < cap_order );
      CPPAD_ASSERT_UNKNOWN( d < nc_partial );

      // Arguments
      const Base* x  = taylor + size_t(arg[0]) * cap_order;
      const Base* y  = taylor + size_t(arg[1]) * cap_order;

      // Partial derivatives corresponding to arguments and result
      Base* px = partial + size_t(arg[0]) * nc_partial;
      Base* py = partial + size_t(arg[1]) * nc_partial;
      Base* pz = partial + i_z    * nc_partial;

      // number of indices to access
      size_t j = d + 1;
      size_t k;
      while(j)
      {  --j;
         for(k = 0; k <= j; k++)
         {
            px[j-k] += azmul(pz[j], y[k]);
            py[k]   += azmul(pz[j], x[j-k]);
         }
      }
   }
};
}}} // END namespace
# endif