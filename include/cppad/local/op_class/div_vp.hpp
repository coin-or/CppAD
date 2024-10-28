# ifndef CPPAD_LOCAL_OP_CLASS_DIV_VP_HPP
# define CPPAD_LOCAL_OP_CLASS_DIV_VP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/op_class/var_binary_op.hpp>

namespace CppAD { namespace local { namespace op_class { // BEGIN namespace
template <class Base> class div_vp_t : public var_binary_op_t<Base>
{
public:
   //
   // op2enum
   OpCode op2enum(void) const override
   {  return DivvpOp; }
   //
   // n_res
   size_t n_res(void) const override
   {  return 1; }
   //
   // get_instance
   static div_vp_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static div_vp_t instance;
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
      CPPAD_ASSERT_UNKNOWN( NumArg(DivvpOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(DivvpOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );
      CPPAD_ASSERT_UNKNOWN( p <= q );

      // Taylor coefficients corresponding to arguments and result
      Base* x = taylor + size_t(arg[0]) * cap_order;
      Base* z = taylor + i_z    * cap_order;

      // Parameter value
      Base y = parameter[ arg[1] ];

      // Using CondExp and multiple levels of AD, it can make sense
      // to divide by zero so do not make it an error.
      for(size_t d = p; d <= q; d++)
         z[d] = x[d] / y;
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
      CPPAD_ASSERT_UNKNOWN( NumArg(DivvpOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(DivvpOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );
      CPPAD_ASSERT_UNKNOWN( 0 < q  );

      // Taylor coefficients corresponding to arguments and result
      size_t num_taylor_per_var = (cap_order-1) * r + 1;
      Base* x = taylor + size_t(arg[0]) * num_taylor_per_var;
      Base* z = taylor +    i_z * num_taylor_per_var;

      // Parameter value
      Base y = parameter[ arg[1] ];

      // Using CondExp and multiple levels of AD, it can make sense
      // to divide by zero so do not make it an error.
      size_t m = (q-1)*r + 1;
      for(size_t ell = 0; ell < r; ell++)
         z[m + ell] = x[m + ell] / y;
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
      CPPAD_ASSERT_UNKNOWN( NumArg(DivvpOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(DivvpOp) == 1 );

      // Parameter value
      Base y = parameter[ arg[1] ];

      // Taylor coefficients corresponding to arguments and result
      Base* x = taylor + size_t(arg[0]) * cap_order;
      Base* z = taylor + i_z    * cap_order;

      z[0] = x[0] / y;
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
      CPPAD_ASSERT_UNKNOWN( NumArg(DivvpOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(DivvpOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( d < cap_order );
      CPPAD_ASSERT_UNKNOWN( d < nc_partial );

      // Argument values
      Base  y = parameter[ arg[1] ];

      // Partial derivatives corresponding to arguments and result
      Base* px = partial + size_t(arg[0]) * nc_partial;
      Base* pz = partial + i_z    * nc_partial;

      // Using CondExp, it can make sense to divide by zero
      // so do not make it an error.
      Base inv_y = Base(1.0) / y;

      // number of indices to access
      size_t j = d + 1;
      while(j)
      {  --j;
         px[j] += azmul(pz[j], inv_y);
      }
   }
};
}}} // END namespace
# endif
