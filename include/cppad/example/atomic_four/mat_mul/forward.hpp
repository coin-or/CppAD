# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_FORWARD_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_FORWARD_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_mat_mul_forward.hpp}

Atomic Matrix Multiply Forward Mode: Example Implementation
###########################################################

Purpose
*******
The ``forward`` routine overrides the virtual functions
used by the atomic_four base; see
:ref:`forward<atomic_four_forward-name>` .

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_mat_mul_forward.hpp}
*/
// BEGIN C++
# include <cppad/example/atomic_four/mat_mul/mat_mul.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// forward override for Base matrix multiply
template <class Base>
bool atomic_mat_mul<Base>::forward(
   size_t                                     call_id     ,
   const CppAD::vector<bool>&                 select_y    ,
   size_t                                     order_low   ,
   size_t                                     order_up    ,
   const CppAD::vector<Base>&                 taylor_x    ,
   CppAD::vector<Base>&                       taylor_y    )
{
   // q
   size_t q     = order_up + 1;
   //
   // n_left, n_middle, n_right
   size_t n_left, n_middle, n_right;
   get(call_id, n_left, n_middle, n_right);
# ifndef NDEBUG
   // n, m
   size_t n     = taylor_x.size();
   size_t m     = taylor_y.size();
   //
   // check sizes
   assert( n == n_middle * ( n_left +  n_right ) * q );
   assert( m == n_left * n_right * q );
# endif
   //
   // offset
   size_t offset = n_left * n_middle;
   //
   // for k = order_low, ..., order_up :
   //    C^k = sum_ell A^ell * B^{k-ell}
   CppAD::vector<Base> x( n_middle * ( n_left + n_right) );
   CppAD::vector<Base> y(n_left * n_right);
   CppAD::vector<Base> sum(n_left * n_right);
   for(size_t k = order_low; k < q; ++k)
   {  // sum = 0
      for(size_t i = 0; i < n_left * n_right; ++i)
         sum[i] = Base(0);
      for(size_t ell = 0; ell <= k; ++ell)
      {  // x = [ A^ell, B^{k-ell} ]
         for(size_t i = 0; i < n_left * n_middle; ++i)
            x[i] = taylor_x[ i * q + ell ];
         for(size_t i = 0; i < n_middle * n_right; ++i)
            x[offset + i] = taylor_x[ (offset + i) * q + (k - ell) ];
         //
         // y = A^ell * B^{k-ell}
         base_mat_mul(n_left, n_middle, n_right, x, y);
         //
         // sum += y
         for(size_t i = 0; i < n_left * n_right; ++i)
            sum[i] += y[i];
      }
      // C^k = sum
      for(size_t i = 0; i < n_left * n_right; ++i)
         taylor_y[i * q + k] = sum[i];
   }
   return true;
}
//
// forward override for AD<Base> matrix multiply
template <class Base>
bool atomic_mat_mul<Base>::forward(
   size_t                                     call_id     ,
   const CppAD::vector<bool>&                 select_y    ,
   size_t                                     order_low   ,
   size_t                                     order_up    ,
   const CppAD::vector< CppAD::AD<Base> >&    ataylor_x   ,
   CppAD::vector< CppAD::AD<Base> >&          ataylor_y   )
{  //
   // vector, AD
   using CppAD::vector;
   using CppAD::AD;
   // q
   size_t q     = order_up + 1;
   //
   // n_left, n_middle, n_right
   size_t n_left, n_middle, n_right;
   get(call_id, n_left, n_middle, n_right);
# ifndef NDEBUG
   // n, m
   size_t n     = ataylor_x.size();
   size_t m     = ataylor_y.size();
   //
   // check sizes
   assert( n == n_middle * ( n_left +  n_right ) * q );
   assert( m == n_left * n_right * q );
# endif
   //
   // offset
   size_t offset = n_left * n_middle;
   //
   // for k = order_low, ..., order_up :
   //    C^k = sum_ell A^ell * B^{k-ell}
   vector< AD<Base> > ax( n_middle *( n_left + n_right) );
   vector< AD<Base> > ay(n_left * n_right);
   vector< AD<Base> > asum(n_left * n_right);
   for(size_t k = order_low; k < q; ++k)
   {  // sum = 0
      for(size_t i = 0; i < n_left * n_right; ++i)
         asum[i] = AD<Base>(0);
      for(size_t ell = 0; ell <= k; ++ell)
      {  // ax = [ A^ell, B^{k-ell} ]
         for(size_t i = 0; i < n_left * n_middle; ++i)
            ax[i] = ataylor_x[ i * q + ell ];
         for(size_t i = 0; i < n_middle * n_right; ++i)
            ax[offset + i] = ataylor_x[ (offset + i) * q + (k - ell) ];
         //
         // ay = A^ell * B^{k-ell}
         (*this)(call_id, ax, ay);
         //
         // asum += ay
         for(size_t i = 0; i < n_left * n_right; ++i)
            asum[i] += ay[i];
      }
      // C^k = asum
      for(size_t i = 0; i < n_left * n_right; ++i)
         ataylor_y[i * q + k] = asum[i];
   }
   return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
