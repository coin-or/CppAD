# ifndef CPPAD_LOCAL_OP_CLASS_SINH_V_HPP
# define CPPAD_LOCAL_OP_CLASS_SINH_V_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/op_class/var_unary_op.hpp>

namespace CppAD { namespace local { namespace op_class { // BEGIN namespace
template <class Base> class sinh_v_t : public var_unary_op_t<Base>
{
public:
   //
   // op2enum
   OpCode op2enum(void) const override
   {  return SinhOp; }
   //
   // get_instance
   static sinh_v_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static sinh_v_t instance;
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
      CPPAD_ASSERT_UNKNOWN( NumArg(SinhOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( NumRes(SinhOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );
      CPPAD_ASSERT_UNKNOWN( p <= q );

      // Taylor coefficients corresponding to argument and result
      Base* x = taylor + size_t(arg[0]) * cap_order;
      Base* s = taylor + i_z * cap_order;
      Base* c = s      -       cap_order;


      // rest of this routine is identical for the following cases:
      // sin_v.hpp, cos_v.hpp, sinh_v.hpp, cos_v.hpp_op
      // (except that there is a sign difference for hyperbolic case).
      size_t k;
      if( p == 0 )
      {  s[0] = sinh( x[0] );
         c[0] = cosh( x[0] );
         p++;
      }
      for(size_t j = p; j <= q; j++)
      {
         s[j] = Base(0.0);
         c[j] = Base(0.0);
         for(k = 1; k <= j; k++)
         {  s[j] += Base(double(k)) * x[k] * c[j-k];
            c[j] += Base(double(k)) * x[k] * s[j-k];
         }
         s[j] /= Base(double(j));
         c[j] /= Base(double(j));
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
      CPPAD_ASSERT_UNKNOWN( NumArg(SinhOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( NumRes(SinhOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( 0 < q );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );

      // Taylor coefficients corresponding to argument and result
      size_t num_taylor_per_var = (cap_order-1) * r + 1;
      Base* x = taylor + size_t(arg[0]) * num_taylor_per_var;
      Base* s = taylor + i_z * num_taylor_per_var;
      Base* c = s      -       num_taylor_per_var;


      // rest of this routine is identical for the following cases:
      // sin_v.hpp, cos_v.hpp, sinh_v.hpp, cos_v.hpp_op
      // (except that there is a sign difference for the hyperbolic case).
      size_t m = (q-1) * r + 1;
      for(size_t ell = 0; ell < r; ell++)
      {  s[m+ell] = Base(double(q)) * x[m + ell] * c[0];
         c[m+ell] = Base(double(q)) * x[m + ell] * s[0];
         for(size_t k = 1; k < q; k++)
         {  s[m+ell] += Base(double(k)) * x[(k-1)*r+1+ell] * c[(q-k-1)*r+1+ell];
            c[m+ell] += Base(double(k)) * x[(k-1)*r+1+ell] * s[(q-k-1)*r+1+ell];
         }
         s[m+ell] /= Base(double(q));
         c[m+ell] /= Base(double(q));
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
      CPPAD_ASSERT_UNKNOWN( NumArg(SinhOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( NumRes(SinhOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( 0 < cap_order );

      // Taylor coefficients corresponding to argument and result
      Base* x = taylor + size_t(arg[0]) * cap_order;
      Base* s = taylor + i_z * cap_order;  // called z in documentation
      Base* c = s      -       cap_order;  // called y in documentation

      s[0] = sinh( x[0] );
      c[0] = cosh( x[0] );
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
      CPPAD_ASSERT_UNKNOWN( NumArg(SinhOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( NumRes(SinhOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( d < cap_order );
      CPPAD_ASSERT_UNKNOWN( d < nc_partial );

      // Taylor coefficients and partials corresponding to argument
      const Base* x  = taylor  + size_t(arg[0]) * cap_order;
      Base* px       = partial + size_t(arg[0]) * nc_partial;

      // Taylor coefficients and partials corresponding to first result
      const Base* s  = taylor  + i_z * cap_order; // called z in doc
      Base* ps       = partial + i_z * nc_partial;

      // Taylor coefficients and partials corresponding to auxillary result
      const Base* c  = s  - cap_order; // called y in documentation
      Base* pc       = ps - nc_partial;


      // rest of this routine is identical for the following cases:
      // sin_v.hpp, cos_v.hpp, sinh_v.hpp, cos_v.hpp_op.
      size_t j = d;
      size_t k;
      while(j)
      {
         ps[j]   /= Base(double(j));
         pc[j]   /= Base(double(j));
         for(k = 1; k <= j; k++)
         {
            px[k]   += Base(double(k)) * azmul(ps[j], c[j-k]);
            px[k]   += Base(double(k)) * azmul(pc[j], s[j-k]);

            ps[j-k] += Base(double(k)) * azmul(pc[j], x[k]);
            pc[j-k] += Base(double(k)) * azmul(ps[j], x[k]);

         }
         --j;
      }
      px[0] += azmul(ps[0], c[0]);
      px[0] += azmul(pc[0], s[0]);
   }
};
}}} // END namespace
# endif
