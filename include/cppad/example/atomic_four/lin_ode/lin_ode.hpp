# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_LIN_ODE_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_LIN_ODE_HPP

// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_lin_ode.hpp}

Atomic Linear ODE Class: Example Implementation
###############################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_lin_ode.hpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
template <class Base>
class atomic_lin_ode : public CppAD::atomic_four<Base> {
//
public:
   // BEGIN sparse_rc_typedef
   typedef CppAD::sparse_rc< CppAD::vector<size_t> > sparse_rc;
   // END sparse_rc_typedef
   //
   // ctor
   atomic_lin_ode(const std::string& name) :
   CppAD::atomic_four<Base>(name)
   {  for(size_t thread = 0; thread < CPPAD_MAX_NUM_THREADS; ++thread)
         work_[thread] = nullptr;
   }
   // destructor
   ~atomic_lin_ode(void)
   {  for(size_t thread = 0; thread < CPPAD_MAX_NUM_THREADS; ++thread)
      {  if( work_[thread] != nullptr  )
         {  // allocated in set member function
            delete work_[thread];
         }
      }
   }
   // set
   size_t set(
      const Base& r,
      const Base& step,
      sparse_rc&  pattern,
      const bool& transpose
   );
   //
   // get
   void get(
      size_t     call_id,
      Base&      r,
      Base&      step,
      sparse_rc& pattern,
      bool&      transpose
   );
   //
   // base_solver
   static void base_solver(
      const Base&                r          ,
      const Base&                step       ,
      const sparse_rc&           pattern    ,
      const bool&                transpose  ,
      const CppAD::vector<Base>& x          ,
      CppAD::vector<Base>&       y
   );
   //
   // test_rev_depend
   // public version of this function that is used for example / testing
   bool test_rev_depend(
      size_t                                         call_id,
      CppAD::vector<bool>&                           ident_zero_x,
      CppAD::vector<bool>&                           depend_x,
      const CppAD::vector<bool>&                     depend_y)
   {  bool ok = rev_depend(call_id, ident_zero_x, depend_x, depend_y);
      return ok;
   }
private:
   //
   // information connected to one call of this atomic function
   // pattern points to pattern_vec for this thread
   struct call_struct {
      size_t thread; Base r; Base step; size_t pattern_index; bool transpose;
   };
   //
   // information connected to each thread
   // patterns are in a separate vector so do not need one for every call
   struct thread_struct {
      CppAD::vector<sparse_rc>   pattern_vec;
      CppAD::vector<call_struct> call_vec;
   };
   //
   // Use pointers, to avoid false sharing between threads.
   thread_struct* work_[CPPAD_MAX_NUM_THREADS];
   //
   // extend_ode
   template <class Float>
   static void extend_ode(
      const sparse_rc&            pattern         ,
      const bool&                 transpose       ,
      const CppAD::vector<Float>& taylor_x        ,
      sparse_rc&                  pattern_extend  ,
      CppAD::vector<Float>&       x_extend
   );
   //
   // reverse_one
   static void reverse_one(
      const Base&                r           ,
      const Base&                step        ,
      const sparse_rc&           pattern     ,
      const bool&                transpose   ,
      const CppAD::vector<bool>& select_x    ,
      const CppAD::vector<Base>& x           ,
      const CppAD::vector<Base>& w           ,
      CppAD::vector<Base>&       partial_x
   );
   void reverse_one(
      const size_t&                           call_id     ,
      const CppAD::vector<bool>&              select_x    ,
      const CppAD::vector< CppAD::AD<Base> >& x           ,
      const CppAD::vector< CppAD::AD<Base> >& w           ,
      CppAD::vector< CppAD::AD<Base> >&       partial_x
   );
   // -----------------------------------------------------------------------
   // overrides
   // -----------------------------------------------------------------------
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
   // Cases not yet implemented
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

# include <cppad/example/atomic_four/lin_ode/set.hpp>
# include <cppad/example/atomic_four/lin_ode/get.hpp>
# include <cppad/example/atomic_four/lin_ode/base_solver.hpp>
# include <cppad/example/atomic_four/lin_ode/for_type.hpp>
# include <cppad/example/atomic_four/lin_ode/forward.hpp>
# include <cppad/example/atomic_four/lin_ode/reverse.hpp>
# include <cppad/example/atomic_four/lin_ode/jac_sparsity.hpp>
# include <cppad/example/atomic_four/lin_ode/hes_sparsity.hpp>
# include <cppad/example/atomic_four/lin_ode/rev_depend.hpp>
// END C++
# endif
