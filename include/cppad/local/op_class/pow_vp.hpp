# ifndef CPPAD_LOCAL_OP_CLASS_POW_VP_HPP
# define CPPAD_LOCAL_OP_CLASS_POW_VP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/op_class/var_binary_op.hpp>

namespace CppAD { namespace local { namespace op_class { // BEGIN namespace
template <class Base> class pow_vp_t : public var_binary_op_t<Base>
{
public:
   //
   // op2enum
   OpCode op2enum(void) const override
   {  return PowvpOp; }
   //
   // get_instance
   static pow_vp_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static pow_vp_t instance;
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
      CPPAD_ASSERT_UNKNOWN( NumArg(PowvpOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(PowvpOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );
      CPPAD_ASSERT_UNKNOWN( p <= q );
      CPPAD_ASSERT_UNKNOWN(
         size_t( std::numeric_limits<addr_t>::max() ) >= i_z
      );

      // Taylor coefficients corresponding to arguments and result
      Base* x = taylor + size_t(arg[0]) * cap_order;
      Base* z = taylor + i_z    * cap_order;

      // Paraemter value
      Base y = parameter[ arg[1] ];

      // Special solution when x[0] is zero
      Base b0 = Base( 0.0 );

      // special case zero order
      if( p == 0 )
      {  z[0] = pow(x[0], y);
         p++;
      }
      for(size_t j = p; j <= q; ++j)
      {  Base sum = Base(0);
         for(size_t k = 1; k < j; ++k)
         {  Base bk = Base( double(k) );
            sum += bk * (y * x[k] * z[j-k] - z[k] * x[j-k]);
         }
         Base bj = Base( double(j) );
         Base zj = ( y * z[0] * x[j] + sum / bj ) / x[0];
         z[j] = CondExpEq(x[0], b0, b0, zj);
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
      CPPAD_ASSERT_UNKNOWN( NumArg(PowvpOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(PowvpOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( 0 < q );
      CPPAD_ASSERT_UNKNOWN( q < cap_order );
      CPPAD_ASSERT_UNKNOWN(
         size_t( std::numeric_limits<addr_t>::max() ) >= i_z
      );

      // Taylor coefficients corresponding to arguments and result
      size_t num_taylor_per_var = (cap_order-1) * r + 1;
      Base* x = taylor + size_t(arg[0]) * num_taylor_per_var;
      Base* z = taylor +    i_z * num_taylor_per_var;

      // Parameter value
      Base y = parameter[ arg[1] ];

      // special solution when x[0] is zero
      Base b0 = Base( 0.0 );

      // index in Taylor coefficients where multiple directions start
      size_t m = (q-1)*r + 1;
      //
      // loop over directions
      for(size_t ell = 0; ell < r; ell++)
      {  Base sum = Base(0);
         for(size_t k = 1; k < q; ++k)
         {  Base xk   = x[(k-1)*r   + ell + 1];
            Base zk   = z[(k-1)*r   + ell + 1];
            Base xqk  = x[(q-k-1)*r + ell + 1];
            Base zqk  = z[(q-k-1)*r + ell + 1];
            Base bk   = Base( double(k) );
            sum += bk * (y * xk * zqk - zk * xqk);
         }
         Base xq  = x[(q-1)*r + ell + 1];
         Base bq   = Base( double(q) );
         Base zell = ( y * z[0] * xq + sum / bq ) / x[0];
         z[m+ell]  = CondExpEq(x[0], b0, b0, zell);
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
      CPPAD_ASSERT_UNKNOWN( NumArg(PowvpOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(PowvpOp) == 1 );

      // Paraemter value
      Base y = parameter[ arg[1] ];

      // Taylor coefficients corresponding to arguments and result
      Base* x = taylor + size_t(arg[0]) * cap_order;
      Base* z = taylor + i_z * cap_order;

      z[0] = pow(x[0], y);
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
      CPPAD_ASSERT_UNKNOWN( NumArg(PowvpOp) == 2 );
      CPPAD_ASSERT_UNKNOWN( NumRes(PowvpOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( d < cap_order );
      CPPAD_ASSERT_UNKNOWN( d < nc_partial );
      CPPAD_ASSERT_UNKNOWN(
         size_t( std::numeric_limits<addr_t>::max() ) >= i_z
      );

      // Taylor coefficients
      const Base* x = taylor + size_t( arg[0] ) * cap_order;
      const Base* z = taylor + i_z * cap_order;

      // parameter value
      const Base  y = parameter[ arg[1] ];

      // Partial derivatives corresponding to arguments and result
      Base* px = partial + size_t(arg[0]) * nc_partial;
      Base* pz = partial + i_z * nc_partial;

      // Special solution when x[0] is zero
      Base b0 = Base( 0.0 );

      // Place to hold px for this operator until conditional assigment at end
      // Avoid allocaing memory when nc_partial is small
   # ifndef NDEBUG
      CppAD::vector<Base> work(nc_partial);
   # else
      Base*               work;
      Base                small_work[2];
      CppAD::vector<Base> large_work(0);
      if( nc_partial < 3 )
         work = small_work;
      else
      {  large_work.resize(nc_partial);
         work = large_work.data();
      }
   # endif
      for(size_t j = 0; j <= d; ++j)
         work[j] = px[j];

      // reverse z^j for j = d, ..., 1
      size_t j = d;
      while(j)
      {  // j
         Base bj = Base( double(j) );
         //
         // x^j term
         work[j] += azmul(pz[j], y * z[0] / x[0]);
         //
         // x^k terms
         for(size_t k = 1; k < j; ++k)
         {  Base bk   = Base( double(k) );
            Base term = (bk * y - Base( double(j-k) ) ) * z[j-k] / (bj * x[0]);
            work[k] += azmul(pz[j], term);
         }
         //
         // z^k terms
         for(size_t k = 1; k < j; ++k)
         {  Base bk   = Base( double(k) );
            Base term = (Base( double(j-k) ) * y - bk) * x[j-k] / (bj * x[0]);
            pz[k] += azmul(pz[j], term);
         }
         //
         // x^0 term
         work[0] -= azmul(pz[j], z[j] / x[0]);
         //
         // z^0 term
         pz[0] += azmul(pz[j], y * x[j] / x[0] );
         //
         // next j
         --j;
      }
      // reverse z^0
      work[0] += azmul(pz[0], y * z[0] / x[0]);
      //
      for(j = 0; j <=d; ++j)
         px[j] = CondExpEq(x[0], b0, b0, work[j]);
   }
};
}}} // END namespace
# endif
