# ifndef CPPAD_LOCAL_VAR_OP_SIGN_OP_HPP
# define CPPAD_LOCAL_VAR_OP_SIGN_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------


namespace CppAD { namespace local { namespace var_op {

// See dev documentation: forward_unary_op
template <class Base>
inline void sign_forward_any(
   size_t        order_low   ,
   size_t        order_up    ,
   size_t        i_z         ,
   const addr_t* arg         ,
   size_t        cap_order   ,
   Base*         taylor      )
{   // p, q
   size_t p = order_low;
   size_t q = order_up;
   //
   // i_x
   size_t i_x = size_t(arg[0]);
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(SignOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(SignOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );
   CPPAD_ASSERT_UNKNOWN( p <= q );

   // Taylor coefficients corresponding to argument and result
   Base* x = taylor + i_x * cap_order;
   Base* z = taylor + i_z * cap_order;

   if( p == 0 )
   {  z[0] = sign(x[0]);
      p++;
   }
   for(size_t j = p; j <= q; j++)
      z[j] = Base(0.);
}
// See dev documentation: forward_unary_op
template <class Base>
inline void sign_forward_dir(
   size_t        order_up    ,
   size_t        n_dir       ,
   size_t        i_z         ,
   const addr_t* arg         ,
   size_t        cap_order   ,
   Base*         taylor      )
{   // q, r
   size_t q = order_up;
   size_t r = n_dir;
   //
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

// See dev documentation: forward_unary_op
template <class Base>
inline void sign_forward_0(
   size_t        i_z         ,
   const addr_t* arg         ,
   size_t        cap_order   ,
   Base*         taylor      )
{  //
   //
   // i_x
   size_t i_x = size_t(arg[0]);
   //

   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(SignOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(SignOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( 0 < cap_order );

   // Taylor coefficients corresponding to argument and result
   Base x0 = *(taylor + i_x * cap_order);
   Base* z = taylor + i_z * cap_order;

   z[0] = sign(x0);
}

// See dev documentation: reverse_unary_op
template <class Base>
inline void sign_reverse(
   size_t        i_z          ,
   const addr_t* arg          ,
   size_t        cap_order    ,
   const Base*   taylor       ,
   size_t        n_order      ,
   Base*         partial      )
{  //
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(SignOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(SignOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( n_order <= cap_order );

   // nothing to do because partials of sign are zero
   return;
}

} } } // END namespace
# endif
