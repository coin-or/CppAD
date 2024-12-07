# ifndef CPPAD_LOCAL_VAR_OP_LOG_OP_HPP
# define CPPAD_LOCAL_VAR_OP_LOG_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

namespace CppAD { namespace local { namespace var_op {

// See dev documentation: forward_unary_op
template <class Base>
inline void log_forward_any(
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
   size_t k;

   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(LogOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(LogOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );
   CPPAD_ASSERT_UNKNOWN( p <= q );

   // Taylor coefficients corresponding to argument and result
   Base* x = taylor + i_x * cap_order;
   Base* z = taylor + i_z * cap_order;

   if( p == 0 )
   {  z[0] = log( x[0] );
      p++;
      if( q == 0 )
         return;
   }
   if ( p == 1 )
   {  z[1] = x[1] / x[0];
      p++;
   }
   for(size_t j = p; j <= q; j++)
   {
      z[j] = -z[1] * x[j-1];
      for(k = 2; k < j; k++)
         z[j] -= Base(double(k)) * z[k] * x[j-k];
      z[j] /= Base(double(j));
      z[j] += x[j];
      z[j] /= x[0];
   }
}

// See dev documentation: forward_unary_op
template <class Base>
inline void log_forward_dir(
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
   CPPAD_ASSERT_UNKNOWN( NumArg(LogOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(LogOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( 0 < q );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );

   // Taylor coefficients corresponding to argument and result
   size_t num_taylor_per_var = (cap_order-1) * r + 1;
   Base* x = taylor + i_x * num_taylor_per_var;
   Base* z = taylor + i_z * num_taylor_per_var;

   size_t m = (q-1) * r + 1;
   for(size_t ell = 0; ell < r; ell++)
   {  z[m+ell] = Base(double(q)) * x[m+ell];
      for(size_t k = 1; k < q; k++)
         z[m+ell] -= Base(double(k)) * z[(k-1)*r+1+ell] * x[(q-k-1)*r+1+ell];
      z[m+ell] /= (Base(double(q)) * x[0]);
   }
}

// See dev documentation: forward_unary_op
template <class Base>
inline void log_forward_0(
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
   CPPAD_ASSERT_UNKNOWN( NumArg(LogOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(LogOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( 0 < cap_order );

   // Taylor coefficients corresponding to argument and result
   Base* x = taylor + i_x * cap_order;
   Base* z = taylor + i_z * cap_order;

   z[0] = log( x[0] );
}


// See dev documentation: reverse_unary_op
template <class Base>
inline void log_reverse(
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
   size_t j, k;

   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(LogOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(LogOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( n_order <= cap_order );

   // Taylor coefficients and partials corresponding to argument
   const Base* x  = taylor  + i_x * cap_order;
   Base* px       = partial + i_x * n_order;

   // Taylor coefficients and partials corresponding to result
   const Base* z  = taylor  + i_z * cap_order;
   Base* pz       = partial + i_z * n_order;

   Base inv_x0 = Base(1.0) / x[0];

   j = d;
   while(j)
   {  // scale partial w.r.t z[j]
      pz[j]   = azmul(pz[j]   , inv_x0);

      px[0]   -= azmul(pz[j], z[j]);
      px[j]   += pz[j];

      // further scale partial w.r.t. z[j]
      pz[j]   /= Base(double(j));

      for(k = 1; k < j; k++)
      {  pz[k]   -= Base(double(k)) * azmul(pz[j], x[j-k]);
         px[j-k] -= Base(double(k)) * azmul(pz[j], z[k]);
      }
      --j;
   }
   px[0] += azmul(pz[0], inv_x0);
}

} } } // END namespace
# endif
