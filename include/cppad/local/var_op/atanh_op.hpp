# ifndef CPPAD_LOCAL_VAR_OP_ATANH_OP_HPP
# define CPPAD_LOCAL_VAR_OP_ATANH_OP_HPP

// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------


namespace CppAD { namespace local { namespace var_op {


// See dev documentation: forward_unary_op
template <class Base>
inline void atanh_forward_any(
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
   CPPAD_ASSERT_UNKNOWN( NumArg(AtanhOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(AtanhOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );
   CPPAD_ASSERT_UNKNOWN( p <= q );

   // Taylor coefficients corresponding to argument and result
   Base* x = taylor + i_x * cap_order;
   Base* z = taylor + i_z * cap_order;
   Base* b = z      -       cap_order;  // called y in documentation

   size_t k;
   if( p == 0 )
   {  z[0] = atanh( x[0] );
      b[0] = Base(1.0) - x[0] * x[0];
      p++;
   }
   for(size_t j = p; j <= q; j++)
   {
      b[j] = - Base(2.0) * x[0] * x[j];
      z[j] = Base(0.0);
      for(k = 1; k < j; k++)
      {  b[j] -= x[k] * x[j-k];
         z[j] -= Base(double(k)) * z[k] * b[j-k];
      }
      z[j] /= Base(double(j));
      z[j] += x[j];
      z[j] /= b[0];
   }
}

// See dev documentation: forward_unary_op
template <class Base>
inline void atanh_forward_dir(
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
   CPPAD_ASSERT_UNKNOWN( NumArg(AtanhOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(AtanhOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( 0 < q );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );

   // Taylor coefficients corresponding to argument and result
   size_t num_taylor_per_var = (cap_order-1) * r + 1;
   Base* x = taylor + i_x * num_taylor_per_var;
   Base* z = taylor + i_z * num_taylor_per_var;
   Base* b = z      -       num_taylor_per_var; // called y in documentation

   size_t m = (q-1) * r + 1;
   for(size_t ell = 0; ell < r; ell++)
   {  b[m+ell] = - Base(2.0) * x[m+ell] * x[0];
      z[m+ell] = Base(double(q)) * x[m+ell];
      for(size_t k = 1; k < q; k++)
      {  b[m+ell] -= x[(k-1)*r+1+ell] * x[(q-k-1)*r+1+ell];
         z[m+ell] -= Base(double(k)) * z[(k-1)*r+1+ell] * b[(q-k-1)*r+1+ell];
      }
      z[m+ell] /= ( Base(double(q)) * b[0] );
   }
}

// See dev documentation: forward_unary_op
template <class Base>
inline void atanh_forward_0(
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
   CPPAD_ASSERT_UNKNOWN( NumArg(AtanhOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(AtanhOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( 0 < cap_order );

   // Taylor coefficients corresponding to argument and result
   Base* x = taylor + i_x * cap_order;
   Base* z = taylor + i_z * cap_order;
   Base* b = z      -       cap_order; // called y in documentation

   z[0] = atanh( x[0] );
   b[0] = Base(1.0) - x[0] * x[0];
}

// See dev documentation: reverse_unary_op
template <class Base>
inline void atanh_reverse(
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
   CPPAD_ASSERT_UNKNOWN( NumArg(AtanhOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(AtanhOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( n_order <= cap_order );

   // Taylor coefficients and partials corresponding to argument
   const Base* x  = taylor  + i_x * cap_order;
   Base* px       = partial + i_x * n_order;

   // Taylor coefficients and partials corresponding to first result
   const Base* z  = taylor  + i_z * cap_order;
   Base* pz       = partial + i_z * n_order;

   // Taylor coefficients and partials corresponding to auxiliary result
   const Base* b  = z  - cap_order; // called y in documentation
   Base* pb       = pz - n_order;

   Base inv_b0 = Base(1.0) / b[0];

   // number of indices to access
   size_t j = d;
   size_t k;
   while(j)
   {  // scale partials w.r.t z[j] and b[j]
      pz[j]  = azmul(pz[j], inv_b0);
      pb[j] *= Base(2.0);

      pb[0] -= azmul(pz[j], z[j]);
      px[j] += pz[j] - azmul(pb[j], x[0]);
      px[0] -= azmul(pb[j], x[j]);

      // more scaling of partials w.r.t z[j]
      pz[j] /= Base(double(j));

      for(k = 1; k < j; k++)
      {  pb[j-k] -= Base(double(k)) * azmul(pz[j], z[k]);
         pz[k]   -= Base(double(k)) * azmul(pz[j], b[j-k]);
         px[k]   -= azmul(pb[j], x[j-k]);
      }
      --j;
   }
   px[0] += azmul(pz[0], inv_b0) - Base(2.0) * azmul(pb[0], x[0]);
}

} } } // END namespace
# endif
