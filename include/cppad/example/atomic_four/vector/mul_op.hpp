# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_MUL_OP_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_MUL_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_vector_mul_op.hpp}

Atomic Vector Multiply Operator: Example Implementation
#######################################################

Forward Mode
************
see theory for forward mode
:ref:`forward_theory@Binary Operators@Multiplication` .

Reverse Mode
************
see theory for reverse mode
:ref:`reverse_theory@Binary Operators@Multiplication` .

Example
*******
The file :ref:`atomic_four_vector_mul.cpp-name` contains an example
and test for this operator.

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_vector_mul_op.hpp}
*/
// BEGIN C++
# include <cppad/example/atomic_four/vector/vector.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
// --------------------------------------------------------------------------
// forward_mul
template <class Base>
void atomic_vector<Base>::forward_mul(
   size_t                                           m,
   size_t                                           p,
   size_t                                           q,
   const CppAD::vector<Base>&                       tx,
   CppAD::vector<Base>&                             ty)
{
   for(size_t i = 0; i < m; ++i)
   {  for(size_t k = p; k < q; ++k)
      {  size_t y_index = i * q + k;
         // y^k = 0
         ty[y_index]    = 0.0;
         for(size_t d = 0; d <= k; d++)
         {  size_t u_index  =       i * q + (k-d);
            size_t v_index  = (m + i) * q + d;
            // y^k += u^{k-d} * v^d
            ty[y_index]    += tx[u_index] * tx[v_index];
         }
      }
   }
}
template <class Base>
void atomic_vector<Base>::forward_mul(
   size_t                                           m,
   size_t                                           p,
   size_t                                           q,
   const CppAD::vector< CppAD::AD<Base> >&          atx,
   CppAD::vector< CppAD::AD<Base> >&                aty)
{
   size_t n = 2 * m;
   assert( atx.size() == n * q );
   assert( aty.size() == m * q );
   //
   // atu, atv
   ad_const_iterator atu = atx.begin();
   ad_const_iterator atv = atu + ad_difference_type(m * q);
   //
   // ax_mul
   ad_vector ax_mul(n);
   ad_iterator au_mul = ax_mul.begin();
   ad_iterator av_mul = ax_mul.begin() + ad_difference_type(m);
   //
   // ax_add
   ad_vector ax_add(n);
   ad_iterator au_add = ax_add.begin();
   ad_iterator av_add = ax_add.begin() + ad_difference_type(m);
   //
   // ay
   ad_vector ay(m);
   //
   for(size_t k = p; k < q; ++k)
   {  // ay = 0
      for(size_t i = 0; i < m; ++i)
         ay[i] = 0.0;
      for(size_t d = 0; d <= k; d++)
      {  // u_add = ay
         for(size_t i = 0; i < m; ++i)
            au_add[i] = ay[i];
         //
         // au_mul = u^{k-d}
         copy_mat_to_vec(m, q, k-d, atu, au_mul);
         //
         // av_mul =  v^d
         copy_mat_to_vec(m, q, d, atv, av_mul);
         //
         // ay = au_mul * av_mul
         (*this)(mul_enum, ax_mul, ay); // atomic vector multiply
         //
         // v_add = ay
         for(size_t i = 0; i < m; ++i)
            av_add[i] = ay[i];
         //
         // ay = u_add + v_add
         (*this)(add_enum, ax_add, ay); // atomic vector add
      }
      // y^k = ay
      copy_vec_to_mat(m, q, k, ay.begin(), aty.begin());
   }
}
// --------------------------------------------------------------------------
// reverse_mul
template <class Base>
void atomic_vector<Base>::reverse_mul(
   size_t                                           m,
   size_t                                           q,
   const CppAD::vector<Base>&                       tx,
   const CppAD::vector<Base>&                       ty,
   CppAD::vector<Base>&                             px,
   const CppAD::vector<Base>&                       py)
{  size_t n = 2 * m;
   assert( tx.size() == n * q );
   assert( ty.size() == m * q );
   assert( px.size() == n * q );
   assert( py.size() == m * q );
   //
   // px
   for(size_t j = 0; j < n; ++j)
   {  for(size_t k = 0; k < q; ++k)
         px[j * q + k] = 0.0;
   }
   //
   // px
   for(size_t i = 0; i < m; ++i)
   {  // k
      size_t k = q;
      while(k)
      {  --k;
         //
         // y_index
         size_t y_index = i * q + k;
         //
         // px
         for(size_t d = 0; d <= k; ++d)
         {  size_t u_index  =       i * q + (k-d);
            size_t v_index  = (m + i) * q + d;
            //
            // must use azmul because py[y_index] = 0 may mean that this
            // component of the function was not selected.
            px[u_index]    += CppAD::azmul( py[y_index] , tx[v_index] );
            px[v_index]    += CppAD::azmul( py[y_index] , tx[u_index] );
         }
      }
   }
}
template <class Base>
void atomic_vector<Base>::reverse_mul(
   size_t                                           m,
   size_t                                           q,
   const CppAD::vector< CppAD::AD<Base> >&          atx,
   const CppAD::vector< CppAD::AD<Base> >&          aty,
   CppAD::vector< CppAD::AD<Base> >&                apx,
   const CppAD::vector< CppAD::AD<Base> >&          apy)
{  size_t n = 2 * m;
   assert( atx.size() == n * q );
   assert( aty.size() == m * q );
   assert( apx.size() == n * q );
   assert( apy.size() == m * q );
   //
   // atu, atv, apu, apv
   ad_const_iterator atu = atx.begin();
   ad_const_iterator atv = atu + ad_difference_type(m * q);
   ad_iterator       apu = apx.begin();
   ad_iterator       apv = apu + ad_difference_type(m * q);
   //
   // ax_mul
   // need azmul_op but it is not yet available
   ad_vector ax_mul(n);
   ad_iterator au_mul = ax_mul.begin();
   ad_iterator av_mul = ax_mul.begin() + ad_difference_type(m);
   //
   // ax_add
   ad_vector ax_add(n);
   ad_iterator au_add = ax_add.begin();
   ad_iterator av_add = ax_add.begin() + ad_difference_type(m);
   //
   // ay
   ad_vector ay(m);
   //
   // px
   // assigning to the value zero does not create operators on the tape
   for(size_t j = 0; j < n; ++j)
   {  for(size_t k = 0; k < q; ++k)
         apx[j * q + k] = 0.0;
   }
   //
   // k
   size_t k = q;
   while(k)
   {  --k;
      //
      // au_mul = apy^k
      copy_mat_to_vec(m, q, k, apy.begin(), au_mul);
      //
      // d
      for(size_t d = 0; d <=k; ++d)
      {  // -------------------------------------------------------------
         // reverse:
         //  px[v_index] += CppAD::azmul( py[y_index] , tx[u_index] );
         // -------------------------------------------------------------

         // av_mul = atu^{k-d}
         copy_mat_to_vec(m, q, k-d, atu, av_mul);
         //
         // ay = au_mul * av_mul
         (*this)(mul_enum, ax_mul, ay);
         //
         // au_add = ay
         for(size_t i = 0; i < m; ++i)
            au_add[i] = ay[i];
         //
         // av_add = apv^d
         copy_mat_to_vec(m, q, d, apv, av_add);
         //
         // ay = au_add + av_add
         (*this)(add_enum, ax_add, ay);
         //
         // apv^d =  ay
         copy_vec_to_mat(m, q, d, ay.begin(), apv);
         // -------------------------------------------------------------
         // reverse:
         //  px[u_index] += CppAD::azmul( py[y_index] , tx[v_index] );
         // -------------------------------------------------------------
         // av_mul = atv^{k-d}
         copy_mat_to_vec(m, q, k-d, atv, av_mul);
         //
         // ay = au_mul * av_mul
         (*this)(mul_enum, ax_mul, ay);
         //
         // au_add = ay
         for(size_t i = 0; i < m; ++i)
            au_add[i] = ay[i];
         //
         // av_add = apu^d
         copy_mat_to_vec(m, q, d, apu, av_add);
         //
         // ay = au_add + av_add
         (*this)(add_enum, ax_add, ay);
         //
         // apu^d =  ay
         copy_vec_to_mat(m, q, d, ay.begin(), apu);
      }
   }
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
