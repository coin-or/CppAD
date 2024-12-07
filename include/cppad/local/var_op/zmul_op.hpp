# ifndef CPPAD_LOCAL_VAR_OP_ZMUL_OP_HPP
# define CPPAD_LOCAL_VAR_OP_ZMUL_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

namespace CppAD { namespace local { namespace var_op {

// --------------------------- Zmulvv -----------------------------------------

// See dev documentation: forward_binary_op
template <class Base>
inline void zmulvv_forward_any(
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
   CPPAD_ASSERT_UNKNOWN( NumArg(ZmulvvOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(ZmulvvOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );
   CPPAD_ASSERT_UNKNOWN( p <= q );

   // Taylor coefficients corresponding to arguments and result
   Base* x = taylor + size_t(arg[0]) * cap_order;
   Base* y = taylor + size_t(arg[1]) * cap_order;
   Base* z = taylor + i_z    * cap_order;

   size_t k;
   for(size_t d = p; d <= q; d++)
   {  z[d] = Base(0.0);
      for(k = 0; k <= d; k++)
         z[d] += azmul(x[d-k], y[k]);
   }
}

// See dev documentation: forward_binary_op
template <class Base>
inline void zmulvv_forward_dir(
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
   CPPAD_ASSERT_UNKNOWN( NumArg(ZmulvvOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(ZmulvvOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( 0 < q );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );

   // Taylor coefficients corresponding to arguments and result
   size_t num_taylor_per_var = (cap_order-1) * r + 1;
   Base* x = taylor + size_t(arg[0]) * num_taylor_per_var;
   Base* y = taylor + size_t(arg[1]) * num_taylor_per_var;
   Base* z = taylor +    i_z * num_taylor_per_var;

   size_t k, ell, m;
   for(ell = 0; ell < r; ell++)
   {  m = (q-1)*r + ell + 1;
      z[m] = azmul(x[0], y[m]) + azmul(x[m],  y[0]);
      for(k = 1; k < q; k++)
         z[m] += azmul(x[(q-k-1)*r + ell + 1], y[(k-1)*r + ell + 1]);
   }
}


// See dev documentation: forward_binary_op
template <class Base>
inline void zmulvv_forward_0(
   size_t        i_z         ,
   const addr_t* arg         ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
{  //
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(ZmulvvOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(ZmulvvOp) == 1 );

   // Taylor coefficients corresponding to arguments and result
   Base* x = taylor + size_t(arg[0]) * cap_order;
   Base* y = taylor + size_t(arg[1]) * cap_order;
   Base* z = taylor + i_z    * cap_order;

   z[0] = azmul(x[0], y[0]);
}


// See dev documentation: reverse_binary_op
template <class Base>
inline void zmulvv_reverse(
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
   CPPAD_ASSERT_UNKNOWN( NumArg(ZmulvvOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(ZmulvvOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( n_order <= cap_order );

   // Arguments
   const Base* x  = taylor + size_t(arg[0]) * cap_order;
   const Base* y  = taylor + size_t(arg[1]) * cap_order;

   // Partial derivatives corresponding to arguments and result
   Base* px = partial + size_t(arg[0]) * n_order;
   Base* py = partial + size_t(arg[1]) * n_order;
   Base* pz = partial + i_z    * n_order;

   // number of indices to access
   size_t j = d + 1;
   size_t k;
   while(j)
   {  --j;
      for(k = 0; k <= j; k++)
      {
         px[j-k] += azmul(pz[j], y[k]);
         py[k]   += azmul(pz[j], x[j-k]);
      }
   }
}
// --------------------------- Zmulpv -----------------------------------------

// See dev documentation: forward_binary_op
template <class Base>
inline void zmulpv_forward_any(
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
   CPPAD_ASSERT_UNKNOWN( NumArg(ZmulpvOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(ZmulpvOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );
   CPPAD_ASSERT_UNKNOWN( p <= q );

   // Taylor coefficients corresponding to arguments and result
   Base* y = taylor + size_t(arg[1]) * cap_order;
   Base* z = taylor + i_z    * cap_order;

   // Paraemter value
   Base x = parameter[ arg[0] ];

   for(size_t d = p; d <= q; d++)
      z[d] = azmul(x, y[d]);
}

// See dev documentation: forward_binary_op
template <class Base>
inline void zmulpv_forward_dir(
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
   CPPAD_ASSERT_UNKNOWN( NumArg(ZmulpvOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(ZmulpvOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( 0 < q );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );

   // Taylor coefficients corresponding to arguments and result
   size_t num_taylor_per_var = (cap_order-1) * r + 1;
   size_t m                  = (q-1) * r + 1;
   Base* y = taylor + size_t(arg[1]) * num_taylor_per_var + m;
   Base* z = taylor + i_z    * num_taylor_per_var + m;

   // Paraemter value
   Base x = parameter[ arg[0] ];

   for(size_t ell = 0; ell < r; ell++)
      z[ell] = azmul(x, y[ell]);
}

// See dev documentation: forward_binary_op
template <class Base>
inline void zmulpv_forward_0(
   size_t        i_z         ,
   const addr_t* arg         ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
{  //
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(ZmulpvOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(ZmulpvOp) == 1 );

   // Paraemter value
   Base x = parameter[ arg[0] ];

   // Taylor coefficients corresponding to arguments and result
   Base* y = taylor + size_t(arg[1]) * cap_order;
   Base* z = taylor + i_z    * cap_order;

   z[0] = azmul(x, y[0]);
}


// See dev documentation: reverse_binary_op
template <class Base>
inline void zmulpv_reverse(
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
   CPPAD_ASSERT_UNKNOWN( NumArg(ZmulpvOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(ZmulpvOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( n_order <= cap_order );

   // Arguments
   Base x  = parameter[ arg[0] ];

   // Partial derivatives corresponding to arguments and result
   Base* py = partial + size_t(arg[1]) * n_order;
   Base* pz = partial + i_z    * n_order;

   // number of indices to access
   size_t j = d + 1;
   while(j)
   {  --j;
      py[j] += azmul(pz[j], x);
   }
}
// --------------------------- Zmulvp -----------------------------------------

// See dev documentation: forward_binary_op
template <class Base>
inline void zmulvp_forward_any(
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
   CPPAD_ASSERT_UNKNOWN( NumArg(ZmulvpOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(ZmulvpOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );
   CPPAD_ASSERT_UNKNOWN( p <= q );

   // Taylor coefficients corresponding to arguments and result
   Base* x = taylor + size_t(arg[0]) * cap_order;
   Base* z = taylor + i_z    * cap_order;

   // Paraemter value
   Base y = parameter[ arg[1] ];

   for(size_t d = p; d <= q; d++)
      z[d] = azmul(x[d], y);
}

// See dev documentation: forward_binary_op
template <class Base>
inline void zmulvp_forward_dir(
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
   CPPAD_ASSERT_UNKNOWN( NumArg(ZmulvpOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(ZmulvpOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( 0 < q );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );

   // Taylor coefficients corresponding to arguments and result
   size_t num_taylor_per_var = (cap_order-1) * r + 1;
   size_t m                  = (q-1) * r + 1;
   Base* x = taylor + size_t(arg[0]) * num_taylor_per_var + m;
   Base* z = taylor + i_z    * num_taylor_per_var + m;

   // Paraemter value
   Base y = parameter[ arg[1] ];

   for(size_t ell = 0; ell < r; ell++)
      z[ell] = azmul(x[ell], y);
}

// See dev documentation: forward_binary_op
template <class Base>
inline void zmulvp_forward_0(
   size_t        i_z         ,
   const addr_t* arg         ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
{  //
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(ZmulvpOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(ZmulvpOp) == 1 );

   // Paraemter value
   Base y = parameter[ arg[1] ];

   // Taylor coefficients corresponding to arguments and result
   Base* x = taylor + size_t(arg[0]) * cap_order;
   Base* z = taylor + i_z    * cap_order;

   z[0] = azmul(x[0], y);
}


// See dev documentation: reverse_binary_op
template <class Base>
inline void zmulvp_reverse(
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
   CPPAD_ASSERT_UNKNOWN( NumArg(ZmulvpOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( NumRes(ZmulvpOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( n_order <= cap_order );

   // Arguments
   Base y  = parameter[ arg[1] ];

   // Partial derivatives corresponding to arguments and result
   Base* px = partial + size_t(arg[0]) * n_order;
   Base* pz = partial + i_z    * n_order;

   // number of indices to access
   size_t j = d + 1;
   while(j)
   {  --j;
      px[j] += azmul(pz[j], y);
   }
}

} } } // END namespace
# endif
