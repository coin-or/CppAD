# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_MAT_MUL_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_MAT_MUL_MAT_MUL_HPP

// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_mat_mul.hpp}

Atomic Matrix Multiply Class: Example Implementation
####################################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_mat_mul.hpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
template <class Base>
class atomic_mat_mul : public CppAD::atomic_four<Base> {
//
public:
   // ctor
   atomic_mat_mul(const std::string& name) :
   CppAD::atomic_four<Base>(name)
   {  for(size_t thread = 0; thread < CPPAD_MAX_NUM_THREADS; ++thread)
         work_[thread] = nullptr;
   }
   // destructor
   ~atomic_mat_mul(void)
   {  for(size_t thread = 0; thread < CPPAD_MAX_NUM_THREADS; ++thread)
      {  if( work_[thread] != nullptr  )
         {  // allocated in set member function
            delete work_[thread];
         }
      }
   }
   // set
   size_t set(
      size_t n_left, size_t n_right, size_t n_middle
   );
   // get
   void get(
      size_t call_id, size_t& n_left, size_t& n_right, size_t& n_middle
   );
private:
   //
   // matrix dimensions corresponding to a call_id
   struct call_struct {
      size_t n_left; size_t n_middle; size_t n_right; size_t thread;
   };
   // map from call_id to matrix dimensions
   typedef CppAD::vector<call_struct> call_vector;
   //
   // Use pointers, to avoid false sharing between threads.
   call_vector* work_[CPPAD_MAX_NUM_THREADS];
   //
   // base_mat_mul
   static void base_mat_mul(
      size_t                     n_left     ,
      size_t                     n_middle   ,
      size_t                     n_right    ,
      const CppAD::vector<Base>& x          ,
      CppAD::vector<Base>&       y
   );
   //
   // -----------------------------------------------------------------------
   // overrides
   // -----------------------------------------------------------------------
   //
   // for_type
   bool for_type(
      size_t                                        call_id,
      const CppAD::vector<CppAD::ad_type_enum>&     type_x,
      CppAD::vector<CppAD::ad_type_enum>&           type_y
   ) override;
   //
   // Base forward
   bool forward(
      size_t                                           call_id,
      const CppAD::vector<bool>&                       select_y,
      size_t                                           order_low,
      size_t                                           order_up,
      const CppAD::vector<Base>&                       taylor_x,
      CppAD::vector<Base>&                             taylor_y
   ) override;
   //
   // AD<Base> forward
   bool forward(
      size_t                                           call_id,
      const CppAD::vector<bool>&                       select_y,
      size_t                                           order_low,
      size_t                                           order_up,
      const CppAD::vector< CppAD::AD<Base> >&          ataylor_x,
      CppAD::vector< CppAD::AD<Base> >&                ataylor_y
   ) override;
   //
   // Base reverse
   bool reverse(
      size_t                                           call_id,
      const CppAD::vector<bool>&                       select_x,
      size_t                                           order_up,
      const CppAD::vector<Base>&                       taylor_x,
      const CppAD::vector<Base>&                       taylor_y,
      CppAD::vector<Base>&                             partial_x,
      const CppAD::vector<Base>&                       partial_y
   ) override;
   //
   // AD<Base> reverse
   bool reverse(
      size_t                                           call_id,
      const CppAD::vector<bool>&                       select_x,
      size_t                                           order_up,
      const CppAD::vector< CppAD::AD<Base> >&          ataylor_x,
      const CppAD::vector< CppAD::AD<Base> >&          ataylor_y,
      CppAD::vector< CppAD::AD<Base> >&                apartial_x,
      const CppAD::vector< CppAD::AD<Base> >&          apartial_y
   ) override;
   //
   // jac_sparsity
   bool jac_sparsity(
      size_t                                         call_id,
      bool                                           dependency,
      const CppAD::vector<bool>&                     ident_zero_x,
      const CppAD::vector<bool>&                     select_x,
      const CppAD::vector<bool>&                     select_y,
      CppAD::sparse_rc< CppAD::vector<size_t> >&     pattern_out
   ) override;
   //
   // hes_sparsity
   bool hes_sparsity(
      size_t                                         call_id,
      const CppAD::vector<bool>&                     ident_zero_x,
      const CppAD::vector<bool>&                     select_x,
      const CppAD::vector<bool>&                     select_y,
      CppAD::sparse_rc< CppAD::vector<size_t> >&     pattern_out
   ) override;
   //
   // rev_depend
   bool rev_depend(
      size_t                                         call_id,
      const CppAD::vector<bool>&                     ident_zero_x,
      CppAD::vector<bool>&                           depend_x,
      const CppAD::vector<bool>&                     depend_y
   ) override;
};
} // END_CPPAD_NAMESPACE

# include <cppad/example/atomic_four/mat_mul/set.hpp>
# include <cppad/example/atomic_four/mat_mul/get.hpp>
# include <cppad/example/atomic_four/mat_mul/base_mat_mul.hpp>
# include <cppad/example/atomic_four/mat_mul/for_type.hpp>
# include <cppad/example/atomic_four/mat_mul/forward.hpp>
# include <cppad/example/atomic_four/mat_mul/reverse.hpp>
# include <cppad/example/atomic_four/mat_mul/jac_sparsity.hpp>
# include <cppad/example/atomic_four/mat_mul/hes_sparsity.hpp>
# include <cppad/example/atomic_four/mat_mul/rev_depend.hpp>
// END C++

# endif
