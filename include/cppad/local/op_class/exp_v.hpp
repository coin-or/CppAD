# ifndef CPPAD_LOCAL_OP_CLASS_EXP_V_HPP
# define CPPAD_LOCAL_OP_CLASS_EXP_V_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/op_class/var_unary_op.hpp>

namespace CppAD { namespace local { // BEGIN namespace
template <class Base> class exp_v_t : public op_class::var_unary_op_t<Base>
{
public:
   //
   // op2enum
   OpCode op2enum(void) const override
   {  return ExpOp; }
   //
   // get_instance
   static exp_v_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static exp_v_t instance;
      CPPAD_ASSERT_NARG_NRES(
         instance.op2enum(), instance.n_arg(), instance.n_res()
      );
      return &instance;
   }
   //
   // n_res
   size_t n_res(void) const override
   {  return 1; }
   //
   // forward
   void forward(
      size_t p           ,
      size_t        q         ,
      size_t        i_z       ,
      const addr_t* arg       ,
      const Base*   parameter ,
      size_t        cap_order ,
      Base*         taylor    ) const override
   {
      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(ExpOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( NumRes(ExpOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );
      CPPAD_ASSERT_UNKNOWN( p <= q );

      // Taylor coefficients corresponding to argument and result
      Base* x = taylor + size_t(arg[0]) * cap_order;
      Base* z = taylor + i_z * cap_order;

      size_t k;
      if( p == 0 )
      {  z[0] = exp( x[0] );
         p++;
      }
      for(size_t j = p; j <= q; j++)
      {
         z[j] = x[1] * z[j-1];
         for(k = 2; k <= j; k++)
            z[j] += Base(double(k)) * x[k] * z[j-k];
         z[j] /= Base(double(j));
      }
   }
   //
   // forward_dir
   void forward_dir(
      size_t        q         ,
      size_t        r         ,
      size_t        i_z       ,
      const addr_t* arg       ,
      const Base*   parameter ,
      size_t        cap_order ,
      Base*         taylor    ) const override
   {
      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(ExpOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( NumRes(ExpOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );
      CPPAD_ASSERT_UNKNOWN( 0 < q );

      // Taylor coefficients corresponding to argument and result
      size_t num_taylor_per_var = (cap_order-1) * r + 1;
      Base* x = taylor + size_t(arg[0]) * num_taylor_per_var;
      Base* z = taylor + i_z * num_taylor_per_var;

      size_t m = (q-1)*r + 1;
      for(size_t ell = 0; ell < r; ell++)
      {  z[m+ell] = Base(double(q)) * x[m+ell] * z[0];
         for(size_t k = 1; k < q; k++)
            z[m+ell] += Base(double(k)) * x[(k-1)*r+ell+1] * z[(q-k-1)*r+ell+1];
         z[m+ell] /= Base(double(q));
      }
   }
   //
   // forward_0
   void forward_0(
      size_t        i_z       ,
      const addr_t* arg       ,
      const Base*   parameter ,
      size_t        cap_order ,
      Base*         taylor    ) const override
   {
      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(ExpOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( NumRes(ExpOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( 0 < cap_order );

      // Taylor coefficients corresponding to argument and result
      Base* x = taylor + size_t(arg[0]) * cap_order;
      Base* z = taylor + i_z * cap_order;

      z[0] = exp( x[0] );
   }
   //
   // reverse
   void reverse(
      size_t        d         ,
      size_t        i_z       ,
      const addr_t* arg       ,
      const Base*   parameter ,
      size_t        cap_order ,
      const Base*   taylor    ,
      size_t        nc_partial,
      Base*         partial   ) const override
   {
      // check assumptions
      CPPAD_ASSERT_UNKNOWN( NumArg(ExpOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( NumRes(ExpOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( d < cap_order );
      CPPAD_ASSERT_UNKNOWN( d < nc_partial );

      // Taylor coefficients and partials corresponding to argument
      const Base* x  = taylor  + size_t(arg[0]) * cap_order;
      Base* px       = partial + size_t(arg[0]) * nc_partial;

      // Taylor coefficients and partials corresponding to result
      const Base* z  = taylor  + i_z * cap_order;
      Base* pz       = partial + i_z * nc_partial;

      // If pz is zero, make sure this operation has no effect
      // (zero times infinity or nan would be non-zero).
      bool skip(true);
      for(size_t i_d = 0; i_d <= d; i_d++)
         skip &= IdenticalZero(pz[i_d]);
      if( skip )
         return;

      // loop through orders in reverse
      size_t j, k;
      j = d;
      while(j)
      {  // scale partial w.r.t z[j]
         pz[j] /= Base(double(j));

         for(k = 1; k <= j; k++)
         {  px[k]   += Base(double(k)) * azmul(pz[j], z[j-k]);
            pz[j-k] += Base(double(k)) * azmul(pz[j], x[k]);
         }
         --j;
      }
      px[0] += azmul(pz[0], z[0]);
   }
};
}} // END namespace
# endif
