# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_SUB_OP_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_SUB_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_vector_sub_op.hpp}

Atomic Vector Subtract Operator: Example Implementation
#######################################################

Forward Mode
************
see theory for forward mode
:ref:`forward_theory@Binary Operators@Subtraction` .

Reverse Mode
************
see theory for reverse mode
:ref:`reverse_theory@Binary Operators@Subtraction` .

Example
*******
The file :ref:`atomic_four_vector_sub.cpp-name` contains an example
and test for this operator.

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_vector_sub_op.hpp}
*/
// BEGIN C++
# include <cppad/example/atomic_four/vector/vector.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
// --------------------------------------------------------------------------
// forward_sub
template <class Base>
void atomic_vector<Base>::forward_sub(
   size_t                                           m,
   size_t                                           p,
   size_t                                           q,
   const CppAD::vector<Base>&                       tx,
   CppAD::vector<Base>&                             ty)
{
   for(size_t k = p; k < q; ++k)
   {  for(size_t i = 0; i < m; ++i)
      {  size_t u_index  =       i * q + k;
         size_t v_index  = (m + i) * q + k;
         size_t y_index  =       i * q + k;
         // y_i^k = u_i^k - v_i^k
         ty[y_index]     = tx[u_index] - tx[v_index];
      }
   }
}
template <class Base>
void atomic_vector<Base>::forward_sub(
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
   // ax
   ad_vector ax(n);
   ad_iterator au = ax.begin();
   ad_iterator av = au + ad_difference_type(m);
   //
   // ay
   ad_vector ay(m);
   //
   for(size_t k = p; k < q; ++k)
   {  // au = u^k
      copy_mat_to_vec(m, q, k, atu, au);
      // av = v^k
      copy_mat_to_vec(m, q, k, atv, av);
      // ay = au - av
      (*this)(sub_enum, ax, ay); // atomic vector sub
      // y^k = ay
      copy_vec_to_mat(m, q, k, ay.begin(), aty.begin());
   }
}
// --------------------------------------------------------------------------
// reverse_sub
template <class Base>
void atomic_vector<Base>::reverse_sub(
   size_t                                           m,
   size_t                                           q,
   const CppAD::vector<Base>&                       tx,
   const CppAD::vector<Base>&                       ty,
   CppAD::vector<Base>&                             px,
   const CppAD::vector<Base>&                       py)
{
   for(size_t k = 0; k < q; ++k)
   {  for(size_t i = 0; i < m; ++i)
      {  size_t u_index  =       i * q + k;
         size_t v_index  = (m + i) * q + k;
         size_t y_index  =       i * q + k;
         // y_i^k = u_i^k + v_i^k
         px[u_index] =   py[y_index];
         px[v_index] = - py[y_index];
      }
   }
}
template <class Base>
void atomic_vector<Base>::reverse_sub(
   size_t                                           m,
   size_t                                           q,
   const CppAD::vector< CppAD::AD<Base> >&          atx,
   const CppAD::vector< CppAD::AD<Base> >&          aty,
   CppAD::vector< CppAD::AD<Base> >&                apx,
   const CppAD::vector< CppAD::AD<Base> >&          apy)
{
# ifndef NDEBUG
   size_t n = 2 * m;
   assert( atx.size() == n * q );
   assert( aty.size() == m * q );
   assert( apx.size() == n * q );
   assert( apy.size() == m * q );
# endif
   //
   // apu, apv
   ad_iterator apu = apx.begin();
   ad_iterator apv = apu + ad_difference_type(m * q);
   //
   // ax
   ad_vector ax(m);
   ad_iterator au = ax.begin();
   //
   // ay
   ad_vector ay(m);
   //
   for(size_t k = 0; k < q; ++k)
   {  // au = apy^k
      copy_mat_to_vec(m, q, k, apy.begin(), au);
      // apu^k = au
      copy_vec_to_mat(m, q, k, au, apu);
      // ay = - au
      (*this)(neg_enum, ax, ay); // atomic vector neg
      // apv^k = ay
      copy_vec_to_mat(m, q, k, ay.begin(), apv);
   }
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
