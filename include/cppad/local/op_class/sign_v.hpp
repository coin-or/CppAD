# ifndef CPPAD_LOCAL_OP_CLASS_SIGN_V_HPP
# define CPPAD_LOCAL_OP_CLASS_SIGN_V_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/op_class/var_unary_op.hpp>

namespace CppAD { namespace local { namespace op_class { // BEGIN namespace
template <class Base> class sign_v_t : public var_unary_op_t<Base>
{
public:
   //
   // op2enum
   OpCode op2enum(void) const override
   {  return SignOp; }
   //
   // get_instance
   static sign_v_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static sign_v_t instance;
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
      CPPAD_ASSERT_UNKNOWN( NumArg(SignOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( NumRes(SignOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );
      CPPAD_ASSERT_UNKNOWN( p <= q );

      // Taylor coefficients corresponding to argument and result
      Base* x = taylor + size_t(arg[0]) * cap_order;
      Base* z = taylor + i_z * cap_order;

      if( p == 0 )
      {  z[0] = sign(x[0]);
         p++;
      }
      for(size_t j = p; j <= q; j++)
         z[j] = Base(0.);
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
      CPPAD_ASSERT_UNKNOWN( NumArg(SignOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( NumRes(SignOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( 0 < q );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );

      // Taylor coefficients corresponding to argument and result
      size_t num_taylor_per_var = (cap_order-1) * r + 1;
      size_t m = (q - 1) * r + 1;
      Base* z = taylor + i_z * num_taylor_per_var;

      for(size_t ell = 0; ell < r; ell++)
         z[m+ell] = Base(0.);
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
      CPPAD_ASSERT_UNKNOWN( NumArg(SignOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( NumRes(SignOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( 0 < cap_order );

      // Taylor coefficients corresponding to argument and result
      Base x0 = *(taylor + size_t(arg[0]) * cap_order);
      Base* z = taylor + i_z * cap_order;

      z[0] = sign(x0);
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
      CPPAD_ASSERT_UNKNOWN( NumArg(SignOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( NumRes(SignOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( d < cap_order );
      CPPAD_ASSERT_UNKNOWN( d < nc_partial );

      // nothing to do because partials of sign are zero
      return;
   }
};
}}} // END namespace
# endif
