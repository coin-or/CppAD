# ifndef CPPAD_LOCAL_VAR_OP_NEG_OP_HPP
# define CPPAD_LOCAL_VAR_OP_NEG_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

namespace CppAD { namespace local { namespace var_op {
template <class Base>

// See forward_unary1_op in developer documentation
inline void neg_forward_any(
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
   CPPAD_ASSERT_NARG_NRES( NegOp, 1, 1 );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );
   CPPAD_ASSERT_UNKNOWN( p <= q );

   // Taylor coefficients corresponding to argument and result
   Base* x = taylor + i_x * cap_order;
   Base* z = taylor + i_z * cap_order;

   for(size_t k = p; k <= q; k++)
      z[k] = - x[k];
}

// See forward_unary1_op_dir in  developer documentation
// See dev documentation: forward_unary_op
template <class Base>
inline void neg_forward_dir(
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
   // i_x
   size_t i_x = size_t(arg[0]);
   //

   // check assumptions
   CPPAD_ASSERT_NARG_NRES( NegOp, 1, 1 );
   CPPAD_ASSERT_UNKNOWN( 0 < q );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );

   // Taylor coefficients corresponding to argument and result
   size_t num_taylor_per_var = (cap_order-1) * r + 1;
   Base* x = taylor + i_x * num_taylor_per_var;
   Base* z = taylor + i_z * num_taylor_per_var;

   size_t m = (q-1) * r + 1;
   for(size_t ell = 0; ell < r; ell++)
      z[m+ell] = - x[m+ell];
}

// See forward_unary1_op_0 in developer documentation
// See dev documentation: forward_unary_op
template <class Base>
inline void neg_forward_0(
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
   CPPAD_ASSERT_NARG_NRES( NegOp, 1, 1 );
   CPPAD_ASSERT_UNKNOWN( 0 < cap_order );

   // Taylor coefficients corresponding to argument and result
   Base* x = taylor + i_x * cap_order;
   Base* z = taylor + i_z * cap_order;

   z[0] = - x[0];
}

// See reverse_unary1_op in developer documentation
// See dev documentation: reverse_unary_op
template <class Base>
inline void neg_reverse(
   size_t        i_z          ,
   const addr_t* arg          ,
   size_t        cap_order    ,
   const Base*   taylor       ,
   size_t        n_order      ,
   Base*         partial      )
{  // d  //
   //
   // i_x
   size_t i_x = size_t(arg[0]);
   //
   size_t d = n_order - 1;
   //
   // check assumptions
   CPPAD_ASSERT_NARG_NRES( NegOp, 1, 1 );
   CPPAD_ASSERT_UNKNOWN( n_order <= cap_order );

   // Taylor coefficients and partials corresponding to argument
   Base* px       = partial + i_x * n_order;

   // Taylor coefficients and partials corresponding to result
   Base* pz       = partial + i_z * n_order;

   Base neg_one   = Base(-1.0);
   for(size_t k = 0; k <= d; ++k)
      px[k]  += azmul(pz[k], neg_one);

}

} } } // END namespace
# endif
