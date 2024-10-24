# ifndef CPPAD_LOCAL_OP_CLASS_TAN_V_HPP
# define CPPAD_LOCAL_OP_CLASS_TAN_V_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/op_class/var_unary_op.hpp>

namespace CppAD { namespace local { namespace op_class { // BEGIN namespace
template <class Base> class tan_v_t : public var_unary_op_t<Base>
{
public:
   //
   // op2enum
   OpCode op2enum(void) const override
   {  return TanOp; }
   //
   // get_instance
   static tan_v_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static tan_v_t instance;
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
   {  return 2; }
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
      CPPAD_ASSERT_UNKNOWN( NumArg(TanOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( NumRes(TanOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );
      CPPAD_ASSERT_UNKNOWN( p <= q );

      // Taylor coefficients corresponding to argument and result
      Base* x = taylor + size_t(arg[0]) * cap_order;
      Base* z = taylor + i_z * cap_order;
      Base* y = z      -       cap_order;

      size_t k;
      if( p == 0 )
      {  z[0] = tan( x[0] );
         y[0] = z[0] * z[0];
         p++;
      }
      for(size_t j = p; j <= q; j++)
      {  Base base_j = static_cast<Base>(double(j));

         z[j] = x[j];
         for(k = 1; k <= j; k++)
            z[j] += Base(double(k)) * x[k] * y[j-k] / base_j;

         y[j] = z[0] * z[j];
         for(k = 1; k <= j; k++)
            y[j] += z[k] * z[j-k];
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
      CPPAD_ASSERT_UNKNOWN( NumArg(TanOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( NumRes(TanOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( 0 < q );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );

      // Taylor coefficients corresponding to argument and result
      size_t num_taylor_per_var = (cap_order-1) * r + 1;
      Base* x = taylor + size_t(arg[0]) * num_taylor_per_var;
      Base* z = taylor + i_z * num_taylor_per_var;
      Base* y = z      -       num_taylor_per_var;

      size_t k;
      size_t m = (q-1) * r + 1;
      for(size_t ell = 0; ell < r; ell++)
      {  z[m+ell] = Base(double(q)) * ( x[m+ell] + x[m+ell] * y[0]);
         for(k = 1; k < q; k++)
            z[m+ell] +=  Base(double(k)) * x[(k-1)*r+1+ell] * y[(q-k-1)*r+1+ell];
         z[m+ell] /= Base(double(q));
         //
         y[m+ell] = Base(2.0) * z[m+ell] * z[0];
         for(k = 1; k < q; k++)
            y[m+ell] += z[(k-1)*r+1+ell] * z[(q-k-1)*r+1+ell];
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
      CPPAD_ASSERT_UNKNOWN( NumArg(TanOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( NumRes(TanOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( 0 < cap_order );

      // Taylor coefficients corresponding to argument and result
      Base* x = taylor + size_t(arg[0]) * cap_order;
      Base* z = taylor + i_z * cap_order;  // called z in documentation
      Base* y = z      -       cap_order;  // called y in documentation

      z[0] = tan( x[0] );
      y[0] = z[0] * z[0];
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
      CPPAD_ASSERT_UNKNOWN( NumArg(TanOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( NumRes(TanOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( d < cap_order );
      CPPAD_ASSERT_UNKNOWN( d < nc_partial );

      // Taylor coefficients and partials corresponding to argument
      const Base* x  = taylor  + size_t(arg[0]) * cap_order;
      Base* px       = partial + size_t(arg[0]) * nc_partial;

      // Taylor coefficients and partials corresponding to first result
      const Base* z  = taylor  + i_z * cap_order; // called z in doc
      Base* pz       = partial + i_z * nc_partial;

      // Taylor coefficients and partials corresponding to auxillary result
      const Base* y  = z  - cap_order; // called y in documentation
      Base* py       = pz - nc_partial;


      size_t j = d;
      size_t k;
      Base base_two(2);
      while(j)
      {
         px[j]   += pz[j];
         pz[j]   /= Base(double(j));
         for(k = 1; k <= j; k++)
         {  px[k]   += azmul(pz[j], y[j-k]) * Base(double(k));
            py[j-k] += azmul(pz[j], x[k]) * Base(double(k));
         }
         for(k = 0; k < j; k++)
            pz[k] += azmul(py[j-1], z[j-k-1]) * base_two;

         --j;
      }
      px[0] += azmul(pz[0], Base(1.0) + y[0]);
   }
};
}}} // END namespace
# endif
