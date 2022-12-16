# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_REVERSE_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_REVERSE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_mat_mul_reverse.hpp}

Atomic Matrix Multiply Reverse Mode: Example Implementation
###########################################################

Purpose
*******
The ``reverse`` routine overrides the virtual functions
used by the atomic_four base; see
:ref:`reverse<atomic_four_reverse-name>` .

Theory
******
See mat_mul :ref:`atomic_four_mat_mul@Theory@Reverse` theory.

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_mat_mul_reverse.hpp}
*/
// BEGIN C++
# include <cppad/example/atomic_four/mat_mul/mat_mul.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// reverse override for Base matrix multiply
template <class Base>
bool atomic_mat_mul<Base>::reverse(
   size_t                                     call_id     ,
   const CppAD::vector<bool>&                 select_y    ,
   size_t                                     order_up    ,
   const CppAD::vector<Base>&                 taylor_x    ,
   const CppAD::vector<Base>&                 taylor_y    ,
   CppAD::vector<Base>&                       partial_x   ,
   const CppAD::vector<Base>&                 partial_y   )
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
   assert( n == partial_x.size() );
   assert( m == partial_y.size() );
# endif
   //
   // offset
   size_t x_offset = n_left * n_middle;
   //
   // u, v, u_offset
   // note that resize only re-alocates when capacity is not large enough
   CppAD::vector<Base> u;
   CppAD::vector<Base> v;
   size_t u_offset;
   //
   // partial_x
   for(size_t i = 0; i < partial_x.size(); ++i)
      partial_x[i] = Base(0);
   //
   // k
   size_t k = q;
   while(k > 0)
   {  --k;
      //
      // for ell = 0, ..., k :
      //    bar{A}^ell      += bar{C}^k [ B^{k-ell} ]^T
      //    bar{B}^{k-ell}  += [ A^ell ]^T \bar{C}^k
      for(size_t ell = 0; ell < q; ++ell)
      {  //
         // u = [ \bar{C}^k, B^{k-ell}^T ]
         u.resize(0);
         u.resize( n_left * n_right + n_right * n_middle );
         u_offset = n_left * n_right;
         for(size_t i = 0; i < n_left * n_right; ++i)
            u[i] = partial_y[ i * q + k ];
         for(size_t i = 0; i < n_middle; ++i)
         {  for(size_t j = 0; j < n_right; ++j)
            {  size_t ij = i * n_right + j;
               size_t ji = j * n_middle + i;
               u[u_offset + ji] =
                  taylor_x[(x_offset + ij) * q + (k - ell) ];
            }
         }
         //
         // v = \bar{C} * [ B^{k-ell} ]^T
         v.resize(0);
         v.resize( n_left * n_middle );
         base_mat_mul(n_left, n_right, n_middle, u, v);
         //
         // \bar{A}^ell += v
         for(size_t i = 0; i < n_left * n_middle; ++i)
            partial_x[i * q + ell] += v[i];
         //
         // u = [ A^ell^T , \bar{C}^k ]
         u.resize(0);
         u.resize( n_middle * n_left + n_left * n_right );
         u_offset = n_middle * n_left;
         for(size_t i = 0; i < n_left; ++i)
         {  for(size_t j = 0; j < n_middle; ++j)
            {  size_t ij = i * n_middle + j;
               size_t ji = j * n_left + i;
               u[ji] = taylor_x[ij * q + ell];
            }
         }
         for(size_t i = 0; i < n_left * n_right; ++i)
            u[u_offset + i] = partial_y[ i * q + k ];
         //
         // v = [ A^ell ]^T * \bar{C}^k
         v.resize(0);
         v.resize( n_middle * n_right );
         base_mat_mul(n_middle, n_left, n_right, u, v);
         //
         // \bar{B}^{k-\ell} += v
         for(size_t i = 0; i < n_middle * n_right; ++i)
            partial_x[ (x_offset + i) * q + (k - ell) ] += v[i];
      }
   }
   return true;
}
//
// reverse override for AD<Base> matrix multiply
template <class Base>
bool atomic_mat_mul<Base>::reverse(
   size_t                                     call_id     ,
   const CppAD::vector<bool>&                 select_y    ,
   size_t                                     order_up    ,
   const CppAD::vector< AD<Base> >&           ataylor_x   ,
   const CppAD::vector< AD<Base> >&           ataylor_y   ,
   CppAD::vector< AD<Base> >&                 apartial_x  ,
   const CppAD::vector< AD<Base> >&           apartial_y  )
{
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
   assert( n == apartial_x.size() );
   assert( m == apartial_y.size() );
# endif
   //
   // offset
   size_t x_offset = n_left * n_middle;
   //
   // u, v, u_offset
   // note that resize only re-alocates when capacity is not large enough
   CppAD::vector< AD<Base> > u;
   CppAD::vector< AD<Base> > v;
   size_t u_offset;
   size_t i_call;
   //
   // apartial_x
   for(size_t i = 0; i < apartial_x.size(); ++i)
      apartial_x[i] =  AD<Base> (0);
   //
   // k
   size_t k = q;
   while(k > 0)
   {  --k;
      //
      // for ell = 0, ..., k :
      //    bar{A}^ell      += bar{C}^k [ B^{k-ell} ]^T
      //    bar{B}^{k-ell}  += [ A^ell ]^T \bar{C}^k
      for(size_t ell = 0; ell < q; ++ell)
      {  //
         // u = [ \bar{C}^k, B^{k-ell}^T ]
         u.resize(0);
         u.resize( n_left * n_right + n_right * n_middle );
         u_offset = n_left * n_right;
         for(size_t i = 0; i < n_left * n_right; ++i)
            u[i] = apartial_y[ i * q + k ];
         for(size_t i = 0; i < n_middle; ++i)
         {  for(size_t j = 0; j < n_right; ++j)
            {  size_t ij = i * n_right + j;
               size_t ji = j * n_middle + i;
               u[u_offset + ji] =
                  ataylor_x[(x_offset + ij) * q + (k - ell) ];
            }
         }
         //
         // v = \bar{C} * [ B^{k-ell} ]^T
         v.resize(0);
         v.resize( n_left * n_middle );
         i_call = set(n_left, n_right, n_middle);
         (*this)(i_call, u, v);
         //
         // \bar{A}^ell += v
         for(size_t i = 0; i < n_left * n_middle; ++i)
            apartial_x[i * q + ell] += v[i];
         //
         // u = [ A^ell^T , \bar{C}^k ]
         u.resize(0);
         u.resize( n_middle * n_left + n_left * n_right );
         u_offset = n_middle * n_left;
         for(size_t i = 0; i < n_left; ++i)
         {  for(size_t j = 0; j < n_middle; ++j)
            {  size_t ij = i * n_middle + j;
               size_t ji = j * n_left + i;
               u[ji] = ataylor_x[ij * q + ell];
            }
         }
         for(size_t i = 0; i < n_left * n_right; ++i)
            u[u_offset + i] = apartial_y[ i * q + k ];
         //
         // v = [ A^ell ]^T * \bar{C}^k
         v.resize(0);
         v.resize( n_middle * n_right );
         i_call = set(n_middle, n_left, n_right);
         (*this)(i_call, u, v);
         //
         // \bar{B}^{k-\ell} += v
         for(size_t i = 0; i < n_middle * n_right; ++i)
            apartial_x[ (x_offset + i) * q + (k - ell) ] += v[i];
      }
   }
   return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
