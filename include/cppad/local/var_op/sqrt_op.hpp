# ifndef CPPAD_LOCAL_VAR_OP_SQRT_OP_HPP
# define CPPAD_LOCAL_VAR_OP_SQRT_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------


namespace CppAD { namespace local { namespace var_op {


// See dev documentation: forward_unary_op
template <class Base>
inline void sqrt_forward_op(
   size_t p           ,
   size_t q           ,
   size_t        i_z         ,
   const addr_t* arg         ,
   size_t        cap_order   ,
   Base*         taylor      )
{  //
   // i_x
   size_t i_x = size_t(arg[0]);
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(SqrtOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(SqrtOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );
   CPPAD_ASSERT_UNKNOWN( p <= q );

   // Taylor coefficients corresponding to argument and result
   Base* x = taylor + i_x * cap_order;
   Base* z = taylor + i_z * cap_order;

   size_t k;
   if( p == 0 )
   {  z[0] = sqrt( x[0] );
      p++;
   }
   for(size_t j = p; j <= q; j++)
   {
      z[j] = Base(0.0);
      for(k = 1; k < j; k++)
         z[j] -= Base(double(k)) * z[k] * z[j-k];
      z[j] /= Base(double(j));
      z[j] += x[j] / Base(2.0);
      z[j] /= z[0];
   }
}

// See dev documentation: forward_unary_op
template <class Base>
inline void forward_sqrt_op_dir(
   size_t q           ,
   size_t r           ,
   size_t i_z         ,
   size_t i_x         ,
   size_t cap_order   ,
   Base*  taylor      )
{
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(SqrtOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(SqrtOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( 0 < q );
   CPPAD_ASSERT_UNKNOWN( q < cap_order );

   // Taylor coefficients corresponding to argument and result
   size_t num_taylor_per_var = (cap_order-1) * r + 1;
   Base* z = taylor + i_z * num_taylor_per_var;
   Base* x = taylor + i_x * num_taylor_per_var;

   size_t m = (q-1) * r + 1;
   for(size_t ell = 0; ell < r; ell++)
   {  z[m+ell] = Base(0.0);
      for(size_t k = 1; k < q; k++)
         z[m+ell] -= Base(double(k)) * z[(k-1)*r+1+ell] * z[(q-k-1)*r+1+ell];
      z[m+ell] /= Base(double(q));
      z[m+ell] += x[m+ell] / Base(2.0);
      z[m+ell] /= z[0];
   }
}

// See dev documentation: forward_unary_op
template <class Base>
inline void sqrt_forward_0(
   size_t        i_z         ,
   const addr_t* arg         ,
   size_t        cap_order   ,
   Base*         taylor      )
{  //
   // i_x
   size_t i_x = size_t(arg[0]);
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(SqrtOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(SqrtOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( 0 < cap_order );

   // Taylor coefficients corresponding to argument and result
   Base* x = taylor + i_x * cap_order;
   Base* z = taylor + i_z * cap_order;

   z[0] = sqrt( x[0] );
}

// See dev documentation: reverse_unary_op
template <class Base>
inline void reverse_sqrt_op(
   size_t      i_z          ,
   size_t      i_x          ,
   size_t      cap_order    ,
   const Base* taylor       ,
   size_t      n_order      ,
   Base*       partial      )
{  // d
   size_t d = n_order - 1;
   //
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(SqrtOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(SqrtOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( n_order <= cap_order );

   // Taylor coefficients and partials corresponding to argument
   Base* px       = partial + i_x * n_order;

   // Taylor coefficients and partials corresponding to result
   const Base* z  = taylor  + i_z * cap_order;
   Base* pz       = partial + i_z * n_order;


   Base inv_z0 = Base(1.0) / z[0];

   // number of indices to access
   size_t j = d;
   size_t k;
   while(j)
   {

      // scale partial w.r.t. z[j]
      pz[j]    = azmul(pz[j], inv_z0);

      pz[0]   -= azmul(pz[j], z[j]);
      px[j]   += pz[j] / Base(2.0);
      for(k = 1; k < j; k++)
         pz[k]   -= azmul(pz[j], z[j-k]);
      --j;
   }
   px[0] += azmul(pz[0], inv_z0) / Base(2.0);
}

} } } // END namespace
# endif
