# ifndef CPPAD_LOCAL_VAR_OP_TANH_OP_HPP
# define CPPAD_LOCAL_VAR_OP_TANH_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------


namespace CppAD { namespace local { namespace var_op {


// See dev documentation: forward_unary_op
template <class Base>
inline void tanh_forward_any(
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
   CPPAD_ASSERT_UNKNOWN( NumArg(TanOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(TanOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );
   CPPAD_ASSERT_UNKNOWN( p <= q );

   // Taylor coefficients corresponding to argument and result
   Base* x = taylor + i_x * cap_order;
   Base* z = taylor + i_z * cap_order;
   Base* y = z      -       cap_order;

   size_t k;
   if( p == 0 )
   {  z[0] = tanh( x[0] );
      y[0] = z[0] * z[0];
      p++;
   }
   for(size_t j = p; j <= q; j++)
   {  Base base_j = static_cast<Base>(double(j));

      z[j] = x[j];
      for(k = 1; k <= j; k++)
         z[j] -= Base(double(k)) * x[k] * y[j-k] / base_j;

      y[j] = z[0] * z[j];
      for(k = 1; k <= j; k++)
         y[j] += z[k] * z[j-k];
   }
}

// See dev documentation: forward_unary_op
template <class Base>
inline void tanh_forward_dir(
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
   CPPAD_ASSERT_UNKNOWN( NumArg(TanOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(TanOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( 0 < q );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );

   // Taylor coefficients corresponding to argument and result
   size_t num_taylor_per_var = (cap_order-1) * r + 1;
   Base* x = taylor + i_x * num_taylor_per_var;
   Base* z = taylor + i_z * num_taylor_per_var;
   Base* y = z      -       num_taylor_per_var;

   size_t k;
   size_t m = (q-1) * r + 1;
   for(size_t ell = 0; ell < r; ell++)
   {  z[m+ell] = Base(double(q)) * ( x[m+ell] - x[m+ell] * y[0] );
      for(k = 1; k < q; k++)
         z[m+ell] -= Base(double(k)) * x[(k-1)*r+1+ell] * y[(q-k-1)*r+1+ell];
      z[m+ell] /= Base(double(q));
      //
      y[m+ell] = Base(2.0) * z[m+ell] * z[0];
      for(k = 1; k < q; k++)
         y[m+ell] += z[(k-1)*r+1+ell] * z[(q-k-1)*r+1+ell];
   }
}

// See dev documentation: forward_unary_op
template <class Base>
inline void tanh_forward_0(
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
   CPPAD_ASSERT_UNKNOWN( NumArg(TanOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(TanOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( 0 < cap_order );

   // Taylor coefficients corresponding to argument and result
   Base* x = taylor + i_x * cap_order;
   Base* z = taylor + i_z * cap_order;  // called z in documentation
   Base* y = z      -       cap_order;  // called y in documentation

   z[0] = tanh( x[0] );
   y[0] = z[0] * z[0];
}


// See dev documentation: reverse_unary_op
template <class Base>
inline void tanh_reverse(
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
   CPPAD_ASSERT_UNKNOWN( NumArg(TanOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(TanOp) == 2 );
   CPPAD_ASSERT_UNKNOWN( n_order <= cap_order );

   // Taylor coefficients and partials corresponding to argument
   const Base* x  = taylor  + i_x * cap_order;
   Base* px       = partial + i_x * n_order;

   // Taylor coefficients and partials corresponding to first result
   const Base* z  = taylor  + i_z * cap_order; // called z in doc
   Base* pz       = partial + i_z * n_order;

   // Taylor coefficients and partials corresponding to auxiliary result
   const Base* y  = z  - cap_order; // called y in documentation
   Base* py       = pz - n_order;


   size_t j = d;
   size_t k;
   Base base_two(2);
   while(j)
   {
      px[j]   += pz[j];
      pz[j]   /= Base(double(j));
      for(k = 1; k <= j; k++)
      {  px[k]   -= azmul(pz[j], y[j-k]) * Base(double(k));
         py[j-k] -= azmul(pz[j], x[k]) * Base(double(k));
      }
      for(k = 0; k < j; k++)
         pz[k] += azmul(py[j-1], z[j-k-1]) * base_two;

      --j;
   }
   px[0] += azmul(pz[0], Base(1.0) - y[0]);
}

} } } // END namespace
# endif
