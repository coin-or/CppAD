# ifndef CPPAD_CORE_CHKPOINT_TWO_CHKPOINT_TWO_HPP
# define CPPAD_CORE_CHKPOINT_TWO_CHKPOINT_TWO_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file chkpoint_two.hpp
Second generation checkpoint functions.
*/

/*
{xrst_begin chkpoint_two}
{xrst_spell
  chk
}

Checkpoint Functions: Second Generation
#######################################

Syntax
******

Constructor
===========

| ``chkpoint_two`` < *Base* > *chk_fun* ( *fun* , *name* ,
| |tab| *internal_bool* , *use_hes_sparsity* , *use_base2ad* , *use_in_parallel*
| )

Use Checkpoint Function
=======================
*chk_fun* ( *ax* , *ay* )

new_dynamic
===========
*chk_fun* . ``new_dynamic`` ( *dynamic* )

Reduce Memory
*************
You can reduce the size of the tape and memory required for AD
using a checkpoint representation of a function
:math:`g : \B{R}^n \rightarrow \B{R}^m`.

Faster Recording
****************
It may also reduce the time to make a recording if the same :math:`g(x)`
is used many times (with different values) during the
recording of an ``ADFun`` < *Base* > object.

Repeating Forward
*****************
Normally, CppAD stores :ref:`forward-name` mode results,
until they freed using :ref:`capacity_order-name` ,
or the corresponding :ref:`ADFun-name` object is deleted.
This is not true for ``chkpoint_two`` functions
because the same checkpoint function may be used repeatedly
with different arguments during a single forward mode operation.
Thus, forward mode results are computed for each use of *chk_fun*
in a forward mode sweep.

Operation Sequence
******************
The :ref:`operation sequence<glossary@Operation@Sequence>`
representing :math:`g(x)` is fixed; i.e.,
it cannot depend on the value of :math:`x`.

atomic_three
************
The ``chkpoint_two`` class is derived from ``atomic_three`` ,
hence some of its error message will refer to atomic operations.
The ``chkpoint_two`` class implements all the
:ref:`atomic_three_define@Virtual Functions`
and hence its source code,

   ``include/cppad/core/chkpoint_two/chkpoint_two.hpp``

provides an example for :ref:`atomic_three-name` operations.
The difference is that ``chkpoint_two.hpp`` uses AD
instead of user provided derivatives.

Base
****
The type *Base* specifies the base type for AD operations;
i.e., *chk_fun* can be used during the recording of
``AD`` < *Base* > operations.

Contents
********
{xrst_toc_table
   include/cppad/core/chkpoint_two/ctor.hpp
   include/cppad/core/chkpoint_two/chk_fun.xrst
   include/cppad/core/chkpoint_two/dynamic.hpp
   example/chkpoint_two/get_started.cpp
   example/chkpoint_two/compare.cpp
   example/chkpoint_two/base2ad.cpp
   example/chkpoint_two/dynamic.cpp
   example/chkpoint_two/ode.cpp
}

{xrst_end chkpoint_two}
*/

template <class Base>
class chkpoint_two : public atomic_three<Base> {
// ---------------------------------------------------------------------------
private:
   /// are sparsity calculations using bools or sets of integers
   const bool internal_bool_;
   //
   /// can this checkpoint function calculate Hessian sparsity patterns
   const bool use_hes_sparsity_;
   //
   /// can this checkpoint function be used in base2ad recordings
   const bool use_base2ad_;
   //
   /// can this checkpoint function be used in parallel mode
   const bool use_in_parallel_;
   //
   /// Jacobian sparsity for g(x) with dependncy true.
   /// This is set by the constructor and constant after that.
   sparse_rc< vector<size_t> > jac_sparsity_;
   //
   /// Hessian sparsity for g(x). If use_hes_sparsity_ is true,
   /// This is set by the constructor and constant after that.
   sparse_rc< vector<size_t> > hes_sparsity_;
   //
   /// Function corresponding to this checkpoint object.
   /// If use_in_parallel_, this is constant after the constructor.
   ADFun<Base>    g_;
   //
   /// AD version of function corresponding to this checkpoint object
   /// If use_in_parallel_, this is constant after the constructor.
   ADFun< AD<Base>, Base>  ag_;
   // ------------------------------------------------------------------------
   // member_
   // ------------------------------------------------------------------------
   /// If use_in_parallel_ is true, must have a separate copy member data
   /// that is not constant.
   struct member_struct {
      //
      /// function corresponding to this checkpoint object
      ADFun<Base>                 g_;
      //
      /// AD version of this function object
      ADFun< AD<Base>, Base >     ag_;
      //
   };
   /// use pointers and allocate memory to avoid false sharing
   /// (initialized to null by constructor)
   member_struct* member_[CPPAD_MAX_NUM_THREADS];
   //
   // ------------------------------------------------------------------------
   /// allocate member_ for this thread
   void allocate_member(size_t thread)
   {  CPPAD_ASSERT_UNKNOWN( use_in_parallel_ );
      if( member_[thread] == nullptr )
      {  // allocaate raw memory
         size_t min_bytes = sizeof(member_struct);
         size_t num_bytes;
         void* v_ptr = thread_alloc::get_memory(min_bytes, num_bytes);
         // convert to member_struct*
         member_[thread] = reinterpret_cast<member_struct*>(v_ptr);
         // call member_struct constructor
         new( member_[thread] ) member_struct;
         //
         // The thread has a copy of corresponding information.
         member_[thread]->g_  = g_;
         member_[thread]->ag_ = ag_;
      }
      return;
   }
   //
   // ------------------------------------------------------------------------
   /// free member_ for this thread
   void free_member(size_t thread)
   {  if( member_[thread] != nullptr )
      {  // call destructor
         member_[thread]->~member_struct();
         // return raw m,emory to available pool for this thread
         void* v_ptr = reinterpret_cast<void*>(member_[thread]);
         thread_alloc::return_memory(v_ptr);
         // mark member for this thread as not allocated
         member_[thread] = nullptr;
      }
      return;
   }
   // -----------------------------------------------------------------------
   // atomic_three virtual functions
   // ------------------------------------------------------------------------
   // type
   virtual bool for_type(
      const vector<Base>&          parameter_x ,
      const vector<ad_type_enum>&  type_x      ,
      vector<ad_type_enum>&        type_y
   );
   // forward
   virtual bool forward(
      const vector<Base>&          parameter_x ,
      const vector<ad_type_enum>&  type_x      ,
      size_t                       need_y      ,
      size_t                       order_low   ,
      size_t                       order_up    ,
      const vector<Base>&          taylor_x    ,
      vector<Base>&                taylor_y
   );
   // AD forward
   virtual bool forward(
      const vector< AD<Base> >&    aparameter_x ,
      const vector<ad_type_enum>&  type_x       ,
      size_t                       need_y       ,
      size_t                       order_low    ,
      size_t                       order_up     ,
      const vector< AD<Base> >&    ataylor_x    ,
      vector< AD<Base> >&          ataylor_y
   );
   // reverse
   virtual bool reverse(
      const vector<Base>&          parameter_x ,
      const vector<ad_type_enum>&  type_x      ,
      size_t                       order_up    ,
      const vector<Base>&          taylor_x    ,
      const vector<Base>&          taylor_y    ,
      vector<Base>&                partial_x   ,
      const vector<Base>&          partial_y
   );
   // AD reverse
   virtual bool reverse(
      const vector< AD<Base> >&    aparameter_x ,
      const vector<ad_type_enum>&  type_x       ,
      size_t                       order_up     ,
      const vector< AD<Base> >&    ataylor_x    ,
      const vector< AD<Base> >&    ataylor_y    ,
      vector< AD<Base> >&          apartial_x   ,
      const vector< AD<Base> >&    apartial_y
   );
   // jac_sparsity
   virtual bool jac_sparsity(
      const vector<Base>&            parameter_x  ,
      const vector<ad_type_enum>&    type_x       ,
      bool                           dependency   ,
      const vector<bool>&            select_x     ,
      const vector<bool>&            select_y     ,
      sparse_rc< vector<size_t> >&   pattern_out
   );
   // hes_sparsity
   virtual bool hes_sparsity(
      const vector<Base>&            parameter_x  ,
      const vector<ad_type_enum>&    type_x       ,
      const vector<bool>&            select_x     ,
      const vector<bool>&            select_y     ,
      sparse_rc< vector<size_t> >&   pattern_out
   );
   // rev_depend
   virtual bool rev_depend(
      const vector<Base>&            parameter_x ,
      const vector<ad_type_enum>&    type_x      ,
      vector<bool>&                  depend_x    ,
      const vector<bool>&            depend_y
   );
public:
   // ctor
   chkpoint_two(
      const ADFun<Base>& fun    ,
      const std::string& name   ,
      bool  internal_bool       ,
      bool  use_hes_sparsity    ,
      bool  use_base2ad         ,
      bool  use_in_parallel
   );
   //
   // destructor
   ~chkpoint_two(void);
   //
   // assignment operator
   void operator=(const chkpoint_two& other)
   {  CPPAD_ASSERT_KNOWN(false,
         "cannot use chkpoint_two assignment operator"
      );
   }
   // copy constructor
   chkpoint_two(const chkpoint_two& other)
   :
   internal_bool_    ( other.internal_bool_ ) ,
   use_hes_sparsity_ ( other.use_hes_sparsity_ ) ,
   use_base2ad_      ( other.use_base2ad_ ) ,
   use_in_parallel_  ( other.use_in_parallel_ ) ,
   jac_sparsity_     ( other.jac_sparsity_ ) ,
   hes_sparsity_     ( other.hes_sparsity_ )
   {  g_  = other.g_;
      ag_ = other.ag_;
   }
   //
   // new_dynamic
   template <class BaseVector>
   void new_dynamic(const BaseVector& dynamic);
};

} // END_CPPAD_NAMESPACE

# include <cppad/core/chkpoint_two/ctor.hpp>
# include <cppad/core/chkpoint_two/dynamic.hpp>
# include <cppad/core/chkpoint_two/for_type.hpp>
# include <cppad/core/chkpoint_two/forward.hpp>
# include <cppad/core/chkpoint_two/reverse.hpp>
# include <cppad/core/chkpoint_two/jac_sparsity.hpp>
# include <cppad/core/chkpoint_two/hes_sparsity.hpp>
# include <cppad/core/chkpoint_two/rev_depend.hpp>

# endif
