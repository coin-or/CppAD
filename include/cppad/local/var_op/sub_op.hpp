# ifndef CPPAD_LOCAL_VAR_OP_SUB_OP_HPP
# define CPPAD_LOCAL_VAR_OP_SUB_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

namespace CppAD { namespace local { namespace var_op {

// --------------------------- Subvv -----------------------------------------

// See dev documentation: forward_binary_op
template <class Base>
inline void subvv_forward_any(
   size_t        order_low   ,
   size_t        order_up    ,
   size_t        i_z         ,
   const addr_t* arg         ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
{   // p, q
   size_t p = order_low;
   size_t q = order_up;
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(SubvvOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(SubvvOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );
   CPPAD_ASSERT_UNKNOWN( p <= q );

   // Taylor coefficients corresponding to arguments and result
   Base* x = taylor + size_t(arg[0]) * cap_order;
   Base* y = taylor + size_t(arg[1]) * cap_order;
   Base* z = taylor + i_z    * cap_order;

   for(size_t d = p; d <= q; d++)
      z[d] = x[d] - y[d];
}

// See dev documentation: forward_binary_op
template <class Base>
inline void subvv_forward_dir(
   size_t        order_up    ,
   size_t        n_dir       ,
   size_t        i_z         ,
   const addr_t* arg         ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
{   // q, r
   size_t q = order_up;
   size_t r = n_dir;
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(SubvvOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(SubvvOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( 0 < q );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );

   // Taylor coefficients corresponding to arguments and result
   size_t num_taylor_per_var = (cap_order-1) * r + 1;
   size_t m                  = (q-1) * r + 1;
   Base* x = taylor + size_t(arg[0]) * num_taylor_per_var + m;
   Base* y = taylor + size_t(arg[1]) * num_taylor_per_var + m;
   Base* z = taylor + i_z    * num_taylor_per_var + m;

   for(size_t ell = 0; ell < r; ell++)
      z[ell] = x[ell] - y[ell];
}


// See dev documentation: forward_binary_op
template <class Base>
inline void subvv_forward_0(
   size_t        i_z         ,
   const addr_t* arg         ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
{  //
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(SubvvOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(SubvvOp) == 1 );

   // Taylor coefficients corresponding to arguments and result
   Base* x = taylor + size_t(arg[0]) * cap_order;
   Base* y = taylor + size_t(arg[1]) * cap_order;
   Base* z = taylor + i_z    * cap_order;

   z[0] = x[0] - y[0];
}


// See dev documentation: reverse_binary_op
template <class Base>
inline void subvv_reverse(
   size_t        i_z         ,
   const addr_t* arg         ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   const Base*   taylor      ,
   size_t        n_order     ,
   Base*         partial     )
{  // d
   //
   size_t d = n_order - 1;
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(SubvvOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(SubvvOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( n_order <= cap_order );

   // Partial derivatives corresponding to arguments and result
   Base* px = partial + size_t(arg[0]) * n_order;
   Base* py = partial + size_t(arg[1]) * n_order;
   Base* pz = partial + i_z    * n_order;

   // number of indices to access
   size_t i = d + 1;
   while(i)
   {  --i;
      px[i] += pz[i];
      py[i] -= pz[i];
   }
}

// --------------------------- Subpv -----------------------------------------

// See dev documentation: forward_binary_op
template <class Base>
inline void subpv_forward_any(
   size_t        order_low   ,
   size_t        order_up    ,
   size_t        i_z         ,
   const addr_t* arg         ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
{   // p, q
   size_t p = order_low;
   size_t q = order_up;
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(SubpvOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(SubpvOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );
   CPPAD_ASSERT_UNKNOWN( p <= q );

   // Taylor coefficients corresponding to arguments and result
   Base* y = taylor + size_t(arg[1]) * cap_order;
   Base* z = taylor + i_z    * cap_order;

   // Paraemter value
   Base x = parameter[ arg[0] ];
   if( p == 0 )
   {  z[0] = x - y[0];
      p++;
   }
   for(size_t d = p; d <= q; d++)
      z[d] = - y[d];
}

// See dev documentation: forward_binary_op
template <class Base>
inline void subpv_forward_dir(
   size_t        order_up    ,
   size_t        n_dir       ,
   size_t        i_z         ,
   const addr_t* arg         ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
{   // q, r
   size_t q = order_up;
   size_t r = n_dir;
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(SubpvOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(SubpvOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( 0 < q );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );

   // Taylor coefficients corresponding to arguments and result
   size_t num_taylor_per_var = (cap_order-1) * r + 1;
   size_t m                  = (q-1) * r + 1;
   Base* y = taylor + size_t(arg[1]) * num_taylor_per_var + m;
   Base* z = taylor + i_z    * num_taylor_per_var + m;

   // Paraemter value
   for(size_t ell = 0; ell < r; ell++)
      z[ell] = - y[ell];
}

// See dev documentation: forward_binary_op
template <class Base>
inline void subpv_forward_0(
   size_t        i_z         ,
   const addr_t* arg         ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
{  //
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(SubpvOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(SubpvOp) == 1 );

   // Paraemter value
   Base x = parameter[ arg[0] ];

   // Taylor coefficients corresponding to arguments and result
   Base* y = taylor + size_t(arg[1]) * cap_order;
   Base* z = taylor + i_z    * cap_order;

   z[0] = x - y[0];
}


// See dev documentation: reverse_binary_op
template <class Base>
inline void subpv_reverse(
   size_t        i_z         ,
   const addr_t* arg         ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   const Base*   taylor      ,
   size_t        n_order     ,
   Base*         partial     )
{  // d
   //
   size_t d = n_order - 1;
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(SubvvOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(SubvvOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( n_order <= cap_order );

   // Partial derivatives corresponding to arguments and result
   Base* py = partial + size_t(arg[1]) * n_order;
   Base* pz = partial + i_z    * n_order;

   // number of indices to access
   size_t i = d + 1;
   while(i)
   {  --i;
      py[i] -= pz[i];
   }
}

// --------------------------- Subvp -----------------------------------------

// See dev documentation: forward_binary_op
template <class Base>
inline void subvp_forward_any(
   size_t        order_low   ,
   size_t        order_up    ,
   size_t        i_z         ,
   const addr_t* arg         ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
{   // p, q
   size_t p = order_low;
   size_t q = order_up;
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(SubvpOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(SubvpOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );
   CPPAD_ASSERT_UNKNOWN( p <= q );

   // Taylor coefficients corresponding to arguments and result
   Base* x = taylor + size_t(arg[0]) * cap_order;
   Base* z = taylor + i_z    * cap_order;

   // Parameter value
   Base y = parameter[ arg[1] ];
   if( p == 0 )
   {  z[0] = x[0] - y;
      p++;
   }
   for(size_t d = p; d <= q; d++)
      z[d] = x[d];
}

// See dev documentation: forward_binary_op
template <class Base>
inline void subvp_forward_dir(
   size_t        order_up    ,
   size_t        n_dir       ,
   size_t        i_z         ,
   const addr_t* arg         ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
{   // q, r
   size_t q = order_up;
   size_t r = n_dir;
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(SubvpOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(SubvpOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( 0 < q );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );

   // Taylor coefficients corresponding to arguments and result
   size_t num_taylor_per_var = (cap_order-1) * r + 1;
   Base* x = taylor + size_t(arg[0]) * num_taylor_per_var;
   Base* z = taylor + i_z    * num_taylor_per_var;

   // Parameter value
   size_t m = (q-1) * r + 1;
   for(size_t ell = 0; ell < r; ell++)
      z[m+ell] = x[m+ell];
}


// See dev documentation: forward_binary_op
template <class Base>
inline void subvp_forward_0(
   size_t        i_z         ,
   const addr_t* arg         ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
{  //
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(SubvpOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(SubvpOp) == 1 );

   // Parameter value
   Base y = parameter[ arg[1] ];

   // Taylor coefficients corresponding to arguments and result
   Base* x = taylor + size_t(arg[0]) * cap_order;
   Base* z = taylor + i_z    * cap_order;

   z[0] = x[0] - y;
}


// See dev documentation: reverse_binary_op
template <class Base>
inline void subvp_reverse(
   size_t        i_z         ,
   const addr_t* arg         ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   const Base*   taylor      ,
   size_t        n_order     ,
   Base*         partial     )
{  // d
   //
   size_t d = n_order - 1;
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(SubvpOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(SubvpOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( n_order <= cap_order );

   // Partial derivatives corresponding to arguments and result
   Base* px = partial + size_t(arg[0]) * n_order;
   Base* pz = partial + i_z    * n_order;

   // number of indices to access
   size_t i = d + 1;
   while(i)
   {  --i;
      px[i] += pz[i];
   }
}

} } } // END namespace
# endif
