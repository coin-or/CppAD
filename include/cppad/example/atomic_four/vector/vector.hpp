# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_VECTOR_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_VECTOR_HPP

// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_vector.hpp}

Atomic Vector Class: Example Implementation
###########################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_vector.hpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>

// declare forward_op and reverse_op for Base and AD<Base> cases
// where op is an operator name; e.g., forward_add and reverse_add.
# define CPPAD_ATOMIC_FOUR_FORWARD_AND_REVERSE(op)             \
   static void forward_ ## op(                                \
      size_t                                           m,    \
      size_t                                           p,    \
      size_t                                           q,    \
      const CppAD::vector<Base>&                       tx,   \
      CppAD::vector<Base>&                             ty    \
   );                                                         \
   void forward_ ## op(                                       \
      size_t                                           m,    \
      size_t                                           p,    \
      size_t                                           q,    \
      const CppAD::vector< CppAD::AD<Base> >&          atx,  \
      CppAD::vector< CppAD::AD<Base> >&                aty   \
   );                                                         \
   static void reverse_ ## op(                                \
      size_t                                           m,    \
      size_t                                           q,    \
      const CppAD::vector<Base>&                       tx,   \
      const CppAD::vector<Base>&                       ty,   \
      CppAD::vector<Base>&                             px,   \
      const CppAD::vector<Base>&                       py    \
   );                                                         \
   void reverse_ ## op(                                       \
      size_t                                           m,    \
      size_t                                           q,    \
      const CppAD::vector< CppAD::AD<Base> >&          atx,  \
      const CppAD::vector< CppAD::AD<Base> >&          aty,  \
      CppAD::vector< CppAD::AD<Base> >&                apx,  \
      const CppAD::vector< CppAD::AD<Base> >&          apy   \
   );

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
template <class Base>
class atomic_vector : public CppAD::atomic_four<Base> {
//
public:
   // BEGIN_SORT_THIS_LINE_PLUS_4
   // BEGIN op_enum_t
   // atomic_vector::op_enum_t
   typedef enum {
      add_enum,
      div_enum,
      mul_enum,
      neg_enum,
      sub_enum,
      number_op_enum
   } op_enum_t;
   // END op_enum_t
   // END_SORT_THIS_LINE_MINUS_4
   //
   // ctor
   atomic_vector(const std::string& name) :
   CppAD::atomic_four<Base>(name)
   { }
private:
   typedef CppAD::vector< CppAD::AD<Base> >      ad_vector;
   typedef typename ad_vector::iterator          ad_iterator;
   typedef typename ad_vector::const_iterator    ad_const_iterator;
   typedef typename ad_iterator::difference_type ad_difference_type;
   //
   static bool is_unary(op_enum_t op)
   {  bool result = true;
      switch(op)
      {  case add_enum:
         case sub_enum:
         case mul_enum:
         case div_enum:
         result = false;
         break;

         default:
         break;
      }
      return result;
   }
   // ------------------------------------------------------------------------
   // copy routines
   // ------------------------------------------------------------------------
   static void copy_vec_to_mat(
      size_t            m,
      size_t            q,
      size_t            k ,
      ad_const_iterator vec,
      ad_iterator       mat)
   {  for(size_t i = 0; i < m; ++i)
      {  size_t index  = i * q + k;
         *(mat + ad_difference_type(index) ) =
            *(vec + ad_difference_type(i) );
      }
   }
   // copy_mat_to_vec
   static void copy_mat_to_vec(
      size_t            m,
      size_t            q,
      size_t            k,
      ad_const_iterator mat,
      ad_iterator       vec)
   {  for(size_t i = 0; i < m; ++i)
      {  size_t index  = i * q + k;
         *(vec + ad_difference_type(i) ) =
            *(mat + ad_difference_type(index) );
      }
   }
   // -----------------------------------------------------------------------
   // overrides
   // -----------------------------------------------------------------------
   //
   // for_type
   bool for_type(
      size_t                                     call_id     ,
      const CppAD::vector<CppAD::ad_type_enum>&  type_x      ,
      CppAD::vector<CppAD::ad_type_enum>&        type_y
   ) override;
   //
   // rev_depend
   bool rev_depend(
      size_t                         call_id     ,
      const CppAD::vector<bool>&    ident_zero_x ,
      CppAD::vector<bool>&           depend_x    ,
      const CppAD::vector<bool>&     depend_y
   ) override;
   //
   // jac_sparsity
   bool jac_sparsity(
      size_t                                         call_id      ,
      bool                                           dependency   ,
      const CppAD::vector<bool>&                     ident_zero_x ,
      const CppAD::vector<bool>&                     select_x     ,
      const CppAD::vector<bool>&                     select_y     ,
      CppAD::sparse_rc< CppAD::vector<size_t> >&     pattern_out
   ) override;
   //
   // hes_sparsity
   bool hes_sparsity(
      size_t                                         call_id      ,
      const CppAD::vector<bool>&                     ident_zero_x ,
      const CppAD::vector<bool>&                     select_x     ,
      const CppAD::vector<bool>&                     select_y     ,
      CppAD::sparse_rc< CppAD::vector<size_t> >&     pattern_out
   ) override;
   //
   // Base forward
   bool forward(
      size_t                                           call_id,
      const CppAD::vector<bool>&                       select_y,
      size_t                                           order_low,
      size_t                                           order_up,
      const CppAD::vector<Base>&                       tx,
      CppAD::vector<Base>&                             ty
   ) override;
   //
   // AD<Base> forward
   bool forward(
      size_t                                           call_id,
      const CppAD::vector<bool>&                       select_y,
      size_t                                           order_low,
      size_t                                           order_up,
      const CppAD::vector< CppAD::AD<Base> >&          atx,
      CppAD::vector< CppAD::AD<Base> >&                aty
   ) override;
   //
   // Base reverse
   bool reverse(
      size_t                                           call_id,
      const CppAD::vector<bool>&                       select_x,
      size_t                                           order_up,
      const CppAD::vector<Base>&                       tx,
      const CppAD::vector<Base>&                       ty,
      CppAD::vector<Base>&                             px,
      const CppAD::vector<Base>&                       py
   ) override;
   //
   // AD<Base> reverse
   bool reverse(
      size_t                                           call_id,
      const CppAD::vector<bool>&                       select_x,
      size_t                                           order_up,
      const CppAD::vector< CppAD::AD<Base> >&          atx,
      const CppAD::vector< CppAD::AD<Base> >&          aty,
      CppAD::vector< CppAD::AD<Base> >&                apx,
      const CppAD::vector< CppAD::AD<Base> >&          apy
   ) override;
   // ----------------------------------------------------------------------
   // Forward and Reverse Implementation Routines
   // ----------------------------------------------------------------------
   CPPAD_ATOMIC_FOUR_FORWARD_AND_REVERSE(add)
   CPPAD_ATOMIC_FOUR_FORWARD_AND_REVERSE(sub)
   CPPAD_ATOMIC_FOUR_FORWARD_AND_REVERSE(mul)
   CPPAD_ATOMIC_FOUR_FORWARD_AND_REVERSE(div)
   CPPAD_ATOMIC_FOUR_FORWARD_AND_REVERSE(neg)
};
} // END_CPPAD_NAMESPACE

# undef CPPAD_ATOMIC_FOUR_FORWARD_AND_REVERSE

# include <cppad/example/atomic_four/vector/rev_depend.hpp>
# include <cppad/example/atomic_four/vector/for_type.hpp>
# include <cppad/example/atomic_four/vector/jac_sparsity.hpp>
# include <cppad/example/atomic_four/vector/hes_sparsity.hpp>
# include <cppad/example/atomic_four/vector/reverse_op.hpp>
# include <cppad/example/atomic_four/vector/forward_op.hpp>
# include <cppad/example/atomic_four/vector/neg_op.hpp>
# include <cppad/example/atomic_four/vector/div_op.hpp>
# include <cppad/example/atomic_four/vector/mul_op.hpp>
# include <cppad/example/atomic_four/vector/sub_op.hpp>
# include <cppad/example/atomic_four/vector/add_op.hpp>
// END C++

# endif
