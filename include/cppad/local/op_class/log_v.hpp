# ifndef CPPAD_LOCAL_OP_CLASS_LOG_V_HPP
# define CPPAD_LOCAL_OP_CLASS_LOG_V_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/op_class/var_unary_op.hpp>

namespace CppAD { namespace local { namespace op_class { // BEGIN namespace
template <class Base> class log_v_t : public var_unary_op_t<Base>
{
public:
   //
   // op2enum
   OpCode op2enum(void) const override
   {  return LogOp; }
   //
   // get_instance
   static log_v_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static log_v_t instance;
      CPPAD_ASSERT_NARG_NRES(
         instance.op2enum(), instance.n_arg(), instance.n_res()
      );
      return &instance;
   }
   //
   //
   // n_arg
   size_t n_arg(void) const override
   {  return 1; }

   size_t n_res(void) const override
   {  return 1; }
   //
   // forward
   static void forward(
      size_t p           ,
      size_t        q         ,
      size_t        i_z       ,
      const addr_t* arg       ,
      const Base*   parameter ,
      size_t        cap_order ,
      Base*         taylor    )
   {
      size_t k;

      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(LogOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( NumRes(LogOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );
      CPPAD_ASSERT_UNKNOWN( p <= q );

      // Taylor coefficients corresponding to argument and result
      Base* x = taylor + size_t(arg[0]) * cap_order;
      Base* z = taylor + i_z * cap_order;

      if( p == 0 )
      {  z[0] = log( x[0] );
         p++;
         if( q == 0 )
            return;
      }
      if ( p == 1 )
      {  z[1] = x[1] / x[0];
         p++;
      }
      for(size_t j = p; j <= q; j++)
      {
         z[j] = -z[1] * x[j-1];
         for(k = 2; k < j; k++)
            z[j] -= Base(double(k)) * z[k] * x[j-k];
         z[j] /= Base(double(j));
         z[j] += x[j];
         z[j] /= x[0];
      }
   }
   //
   // forward_dir
   static void forward_dir(
      size_t        q         ,
      size_t        r         ,
      size_t        i_z       ,
      const addr_t* arg       ,
      const Base*   parameter ,
      size_t        cap_order ,
      Base*         taylor    )
   {

      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(LogOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( NumRes(LogOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( 0 < q );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );

      // Taylor coefficients corresponding to argument and result
      size_t num_taylor_per_var = (cap_order-1) * r + 1;
      Base* x = taylor + size_t(arg[0]) * num_taylor_per_var;
      Base* z = taylor + i_z * num_taylor_per_var;

      size_t m = (q-1) * r + 1;
      for(size_t ell = 0; ell < r; ell++)
      {  z[m+ell] = Base(double(q)) * x[m+ell];
         for(size_t k = 1; k < q; k++)
            z[m+ell] -= Base(double(k)) * z[(k-1)*r+1+ell] * x[(q-k-1)*r+1+ell];
         z[m+ell] /= (Base(double(q)) * x[0]);
      }
   }
   //
   // forward_0
   static void forward_0(
      size_t        i_z       ,
      const addr_t* arg       ,
      const Base*   parameter ,
      size_t        cap_order ,
      Base*         taylor    )
   {

      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(LogOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( NumRes(LogOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( 0 < cap_order );

      // Taylor coefficients corresponding to argument and result
      Base* x = taylor + size_t(arg[0]) * cap_order;
      Base* z = taylor + i_z * cap_order;

      z[0] = log( x[0] );
   }
   //
   // reverse
   static void reverse(
      size_t        d         ,
      size_t        i_z       ,
      const addr_t* arg       ,
      const Base*   parameter ,
      size_t        cap_order ,
      const Base*   taylor    ,
      size_t        nc_partial,
      Base*         partial   )
   {  size_t j, k;

      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(LogOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( NumRes(LogOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( d < cap_order );
      CPPAD_ASSERT_UNKNOWN( d < nc_partial );

      // Taylor coefficients and partials corresponding to argument
      const Base* x  = taylor  + size_t(arg[0]) * cap_order;
      Base* px       = partial + size_t(arg[0]) * nc_partial;

      // Taylor coefficients and partials corresponding to result
      const Base* z  = taylor  + i_z * cap_order;
      Base* pz       = partial + i_z * nc_partial;

      Base inv_x0 = Base(1.0) / x[0];

      j = d;
      while(j)
      {  // scale partial w.r.t z[j]
         pz[j]   = azmul(pz[j]   , inv_x0);

         px[0]   -= azmul(pz[j], z[j]);
         px[j]   += pz[j];

         // further scale partial w.r.t. z[j]
         pz[j]   /= Base(double(j));

         for(k = 1; k < j; k++)
         {  pz[k]   -= Base(double(k)) * azmul(pz[j], x[j-k]);
            px[j-k] -= Base(double(k)) * azmul(pz[j], z[k]);
         }
         --j;
      }
      px[0] += azmul(pz[0], inv_x0);
   }
};
}}} // END namespace
# endif
