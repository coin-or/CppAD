# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_DIV_OP_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_DIV_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_vector_div_op.hpp}

Atomic Vector Divide Operator: Example Implementation
#####################################################

Forward Mode
************
see theory for forward mode
:ref:`forward_theory@Binary Operators@Division` .

Reverse Mode
************
see theory for reverse mode
:ref:`reverse_theory@Binary Operators@Division` .

Example
*******
The file :ref:`atomic_four_vector_div.cpp-name` contains an example
and test for this operator.

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_vector_div_op.hpp}
*/
// BEGIN C++
# include <cppad/example/atomic_four/vector/vector.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
// ---------------------------------------------------------------------------
// forward_div
template <class Base>
void atomic_vector<Base>::forward_div(
   size_t                                           m,
   size_t                                           p,
   size_t                                           q,
   const CppAD::vector<Base>&                       tx,
   CppAD::vector<Base>&                             ty)
{
   for(size_t i = 0; i < m; ++i)
   {  for(size_t k = p; k < q; ++k)
      {  size_t y_index = i * q + k;
         size_t u_index = i * q + k;
         // y^k = u^k
         ty[y_index]     = tx[u_index];
         for(size_t d = 1; d <= k; d++)
         {  size_t y_other = i * q + (k-d);
            size_t v_index = (i + m) * q + d;
            // y^k -= y^{k-d} * v^d
            ty[y_index] -= ty[y_other] * tx[v_index];
         }
         size_t v0_index = (i + m) * q + 0;
         // y^k /= v^0
         ty[y_index] /= tx[v0_index];
      }
   }
}
template <class Base>
void atomic_vector<Base>::forward_div(
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
   // ax_div
   ad_vector ax_div(n);
   ad_iterator au_div = ax_div.begin();
   ad_iterator av_div = ax_div.begin() + ad_difference_type(m);
   //
   // ax_mul
   ad_vector ax_mul(n);
   ad_iterator au_mul = ax_mul.begin();
   ad_iterator av_mul = ax_mul.begin() + ad_difference_type(m);
   //
   // ax_sub
   ad_vector ax_sub(n);
   ad_iterator au_sub = ax_sub.begin();
   ad_iterator av_sub = ax_sub.begin() + ad_difference_type(m);
   //
   // ay
   ad_vector ay(m);
   //
   for(size_t k = p; k < q; ++k)
   {  // u_sub = u^k
      copy_mat_to_vec(m, q, k, atu, au_sub);
      for(size_t d = 1; d <= k; d++)
      {  // u_mul = y^{k-d}
         copy_mat_to_vec(m, q, k-d, aty.begin(), au_mul);
         // v_mul = v^d
         copy_mat_to_vec(m, q, d, atv, av_mul);
         // ay = u_mul * v_mul
         (*this)(mul_enum, ax_mul, ay); // atomic vector multiply
         // v_sub = ay
         for(size_t i = 0; i < m; ++i)
            av_sub[i] = ay[i];
         // ay = u_sub - v_sub
         (*this)(sub_enum, ax_sub, ay); // atomic vector subtract
         // u_sub = ay
         for(size_t i = 0; i < m; ++i)
            au_sub[i] = ay[i];
      }
      // u_div = u_sub
      for(size_t i = 0; i < m; ++i)
         au_div[i] = *(au_sub + ad_difference_type(i));
      // v_div = v^0
      copy_mat_to_vec(m, q, 0, atv, av_div);
      // ay = u_div / v_div
      (*this)(div_enum, ax_div, ay); // atomic vector divide
      // y^k = ay
      copy_vec_to_mat(m, q, k, ay.begin(), aty.begin());
   }
}
// ---------------------------------------------------------------------------
// reverse_div
template <class Base>
void atomic_vector<Base>::reverse_div(
   size_t                                           m,
   size_t                                           q,
   const CppAD::vector<Base>&                       tx,
   const CppAD::vector<Base>&                       ty,
   CppAD::vector<Base>&                             px,
   const CppAD::vector<Base>&                       py)
{
# ifndef NDEBUG
   size_t n = 2 * m;
   assert( tx.size() == n * q );
   assert( ty.size() == m * q );
   assert( px.size() == n * q );
   assert( py.size() == m * q );
# endif
   //
   // py_copy
   CppAD::vector<Base> py_copy( py );
   //
   // pv
   for(size_t i = 0; i < m; ++i)
   {  for(size_t k = 0; k < q; ++k)
      {  size_t v_index = (i + m) * q + k;
         px[v_index] = 0.0;
      }
   }
   // px
   for(size_t i = 0; i < m; ++i)
   {  size_t v0_index = (i + m) * q + 0;
      //
      // k
      size_t k = q;
      while(k)
      {  --k;
         //
         // y_index
         size_t y_index = i * q + k;
         //
         // py_scaled
         double py_scaled = py_copy[y_index] / tx[v0_index];
         //
         for(size_t d = 1; d <= k; d++)
         {  size_t y_other = i * q + (k-d);
            size_t v_index = (i + m) * q + d;
            //
            py_copy[y_other] -= py_scaled * tx[v_index];
            px[v_index]      -= py_scaled * ty[y_other];
         }
         size_t u_index = i * q + k;
         px[u_index]   = py_scaled;
         px[v0_index] -= py_scaled * ty[y_index];
      }
   }
}
template <class Base>
void atomic_vector<Base>::reverse_div(
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
   // ax_sub
   ad_vector ax_sub(n);
   ad_iterator au_sub = ax_sub.begin();
   ad_iterator av_sub = ax_sub.begin() + ad_difference_type(m);
   //
   // ax_mul
   ad_vector ax_mul(n);
   ad_iterator au_mul = ax_mul.begin();
   ad_iterator av_mul = ax_mul.begin() + ad_difference_type(m);
   //
   // ax_div
   ad_vector ax_div(n);
   ad_iterator au_div = ax_div.begin();
   ad_iterator av_div = ax_div.begin() + ad_difference_type(m);
   //
   // ay, apy_scaled
   ad_vector ay(m), apy_scaled(m);
   //
   // apy_copy
   ad_vector apy_copy( apy );
   //
   // apv
   for(size_t i = 0; i < m; ++i)
   {  for(size_t k = 0; k < q; ++k)
      {  size_t v_index = (i + m) * q + k;
         apx[v_index] = 0.0;
      }
   }
   //
   // av_div = atv^0
   copy_mat_to_vec(m, q, 0, atv, av_div);
   //
   // k
   size_t k = q;
   while(k)
   {  --k;
      //
      // au_div = apy^k
      copy_mat_to_vec(m, q, k, apy_copy.begin(), au_div);
      //
      // apy_scaled = au_div / av_dir
      (*this)(div_enum, ax_div, apy_scaled);
      //
      // au_mul = apy_scaled
      for(size_t i = 0; i < m; ++i)
         au_mul[i] = apy_scaled[i];
      //
      for(size_t d = 1; d <= k; ++d)
      {  //
         // av_mul = atv^d
         copy_mat_to_vec(m, q, d, atv, av_mul);
         //
         // ay = au_mul * av_mul
         (*this)(mul_enum, ax_mul, ay);
         //
         // au_sub = apy^{k-d}
         copy_mat_to_vec(m, q, k-d, apy_copy.begin(), au_sub);
         //
         // av_sub = ay
         for(size_t i = 0; i < m; ++i)
            av_sub[i] = ay[i];
         //
         // ay = au_sub - av_sub
         (*this)(sub_enum, ax_sub, ay);
         //
         // apy^{k-d} = ay
         copy_vec_to_mat(m, q, k-d, ay.begin(), apy_copy.begin());
         //
         // av_mul = aty^{k-d}
         copy_mat_to_vec(m, q, k-d, aty.begin(), av_mul);
         //
         // ay = au_mul * av_mul
         (*this)(mul_enum, ax_mul, ay);
         //
         // au_sub = apv^d
         copy_mat_to_vec(m, q, d, apv, au_sub);
         //
         // av_sub = ay
         for(size_t i = 0; i < m; ++i)
            av_sub[i] = ay[i];
         //
         // ay = au_sub - av_sub
         (*this)(sub_enum, ax_sub, ay);
         //
         // apv^d = ay
         copy_vec_to_mat(m, q, d, ay.begin(), apv);
      }
      //
      // apu^k = apy_scaled
      copy_vec_to_mat(m, q, k, apy_scaled.begin(), apu);
      //
      // av_mul = aty^k
      copy_mat_to_vec(m, q, k, aty.begin(), av_mul);
      //
      // ay = au_mul * av_mul
      (*this)(mul_enum, ax_mul, ay);
      //
      // au_sub = apv^0
      copy_mat_to_vec(m, q, 0, apv, au_sub);
      //
      // av_sub = ay
      for(size_t i = 0; i < m; ++i)
         av_sub[i] = ay[i];
      //
      // ay = au_sub - av_sub
      (*this)(sub_enum, ax_sub, ay);
      //
      // apv^0 = ay
      copy_vec_to_mat(m, q, 0, ay.begin(), apv);
   }
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
