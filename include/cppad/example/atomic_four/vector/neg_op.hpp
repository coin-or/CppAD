# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_NEG_OP_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_NEG_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_vector_neg_op.hpp}

Atomic Vector Negative Operator: Example Implementation
#######################################################

Example
*******
The file :ref:`atomic_four_vector_neg.cpp-name` contains an example
and test for this operator.

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_vector_neg_op.hpp}
*/
// BEGIN C++
# include <cppad/example/atomic_four/vector/vector.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
// ---------------------------------------------------------------------------
template <class Base>
void atomic_vector<Base>::forward_neg(
   size_t                                           m,
   size_t                                           p,
   size_t                                           q,
   const CppAD::vector<Base>&                       tx,
   CppAD::vector<Base>&                             ty)
{
   for(size_t k = p; k < q; ++k)
   {  for(size_t i = 0; i < m; ++i)
      {  size_t u_index  = i * q + k;
         size_t y_index  = i * q + k;
         // y_i^k = - u_i^k
         ty[y_index] = - tx[u_index];
      }
   }
}
template <class Base>
void atomic_vector<Base>::forward_neg(
   size_t                                           m,
   size_t                                           p,
   size_t                                           q,
   const CppAD::vector< CppAD::AD<Base> >&          atx,
   CppAD::vector< CppAD::AD<Base> >&                aty)
{  size_t n = m;
   assert( atx.size() == n * q );
   assert( aty.size() == m * q );
   //
   // atu
   ad_const_iterator atu = atx.begin();
   //
   // ax
   ad_vector ax(n);
   ad_iterator au = ax.begin();
   //
   // ay
   ad_vector ay(m);
   //
   for(size_t k = p; k < q; ++k)
   {  // au = u^k
      copy_mat_to_vec(m, q, k, atu, au);
      // ay = - au
      (*this)(neg_enum, ax, ay); // atomic vector neg
      // y^k = ay
      copy_vec_to_mat(m, q, k, ay.begin(), aty.begin() );
   }
}
// ---------------------------------------------------------------------------
// reverse_neg
template <class Base>
void atomic_vector<Base>::reverse_neg(
   size_t                                           m,
   size_t                                           q,
   const CppAD::vector<Base>&                       tx,
   const CppAD::vector<Base>&                       ty,
   CppAD::vector<Base>&                             px,
   const CppAD::vector<Base>&                       py)
{
   for(size_t k = 0; k < q; ++k)
   {  for(size_t i = 0; i < m; ++i)
      {  size_t u_index  = i * q + k;
         size_t y_index  = i * q + k;
         // y_i^k = - u_i^k
         px[u_index] = - py[y_index];
      }
   }
}
template <class Base>
void atomic_vector<Base>::reverse_neg(
   size_t                                           m,
   size_t                                           q,
   const CppAD::vector< CppAD::AD<Base> >&          atx,
   const CppAD::vector< CppAD::AD<Base> >&          aty,
   CppAD::vector< CppAD::AD<Base> >&                apx,
   const CppAD::vector< CppAD::AD<Base> >&          apy)
{  size_t n = m;
   assert( atx.size() == n * q );
   assert( aty.size() == m * q );
   //
   // apu
   ad_iterator apu = apx.begin();
   //
   // ax
   ad_vector ax(n);
   ad_iterator au = ax.begin();
   //
   // ay
   ad_vector ay(m);
   //
   for(size_t k = 0; k < q; ++k)
   {  // au = py^k
      copy_mat_to_vec(m, q, k, apy.begin(), au);
      // ay = - au
      (*this)(neg_enum, ax, ay); // atomic vector neg
      // pu^k = ay
      copy_vec_to_mat(m, q, k, ay.begin(), apu);
   }
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
