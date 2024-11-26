# ifndef CPPAD_CORE_ATOMIC_THREE_ATOMIC_HPP
# define CPPAD_CORE_ATOMIC_THREE_ATOMIC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_three_define}

Defining Atomic Functions: Third Generation
###########################################

Syntax
******

Define Class
============

| ``class`` *atomic_user* : ``public CppAD::atomic_three<`` *Base* > {
| |tab| ...
| };

Construct Atomic Function
=========================
*atomic_user* *afun* ( *ctor_arg_list* )

Use Atomic Function
===================
*afun* ( *ax* , *ay* )

Class Member Callbacks
======================

| *ok* = *afun* . ``for_type`` (
| |tab| *parameter_x* , *type_x* , *type_y*
| )
| *ok* = *afun* . ``forward`` (
| |tab| *parameter_x* , *type_x* ,
| |tab| *need_y* , *order_low* , *order_up* , *taylor_x* , *taylor_y*
| )
| *ok* = *afun* . ``reverse`` (
| |tab| *parameter_x* , *type_x* ,
| |tab| *order_up* , *taylor_x* , *taylor_y* , *partial_x* , *partial_y*
| )
| *ok* = *afun* . ``jac_sparsity`` (
| |tab| *parameter_x* , *type_x* , *dependency* , *select_x* *select_y* , *pattern_out*
| )
| *ok* = *afun* . ``hes_sparsity`` (
| |tab| *parameter_x* , *type_x* , *select_x* *select_y* , *pattern_out*
| )
| *ok* = *afun* . ``rev_depend`` (
| |tab| *parameter_x* , *type_x* , *depend_x* , *depend_y*
| )

See Also
********
:ref:`chkpoint_two-name` , :ref:`atomic_two-name`

Purpose
*******

Speed
=====
In some cases, it is possible to compute derivatives of a function

.. math::

   y = g(x) \; {\rm where} \; g : \B{R}^n \rightarrow \B{R}^m

more efficiently than by coding it using ``AD`` < *Base* >
:ref:`glossary@Operation@Atomic` operations
and letting CppAD do the rest.
The class ``atomic_three`` < ``Base`` > is used to
create a new atomic operation corresponding to a function :math:`g(x)`
where the user specifies how to compute the derivatives
and sparsity patterns for :math:`g(x)`.

Reduce Memory
=============
If the function :math:`g(x)` is used many times during the recording
of an :ref:`ADFun-name` object,
using an atomic version of :math:`g(x)` removes the need for repeated
copies of the corresponding ``AD`` < *Base* > operations and variables
in the recording.

ad_type
*******
The type ``CppAD::ad_type_enum``
is used to specify if an AD object is a
:ref:`constant parameter<glossary@Parameter@Constant>`
:ref:`dynamic parameter<glossary@Parameter@Dynamic>`
or :ref:`glossary@Variable` .
It has the following possible values:

.. csv-table::
   :widths: auto

   *ad_type_enum*,Meaning
   ``constant_enum``,constant parameter
   ``dynamic_enum``,dynamic parameter
   ``variable_enum``,variable

In addition,
``constant_enum < dynamic_enum < variable_enum`` .

Virtual Functions
*****************
The
:ref:`callback functions<atomic_three_define@Syntax@Class Member Callbacks>`
are implemented by defining the virtual functions in the
*atomic_user* class.
These functions compute derivatives,
sparsity patterns, and dependency relations.
Each virtual function has a default implementation
that returns *ok* == ``false`` .
The :ref:`for_type<atomic_three_for_type-name>`
and :ref:`forward<atomic_three_forward-name>` function
(for the case *order_up*  == 0 ) must be implemented.
Otherwise, only those functions and orders
required by the your calculations need to be implemented.
For example,
*forward* for the case *order_up*  == 2 can just return
*ok* == ``false`` unless you require
forward mode calculation of second derivatives.

Base
****
This is the base type of the elements of
:ref:`atomic_three_afun@ax` and :ref:`atomic_three_afun@ay`
in the corresponding *afun* ( *ax* , *ay* ) call; i.e.,
the elements of *ax* and *ay* have type
``AD`` < *Base* > .

parameter_x
***********
All the virtual functions include this argument which has prototype

   ``const CppAD::vector<`` *Base* > *parameter_x*

Its size is equal to *n* = *ax* . ``size`` ()
in corresponding *afun* ( *ax* , *ay* ) call.

Constant
========
For *j* =0,..., *n* ``-1`` ,
if *ax* [ *j* ] is a :ref:`con_dyn_var@Constant` parameter,

   *parameter_x* [ *j* ] == *ax* [ *j* ]

Dynamic
=======
If *ax* [ *j* ] is a :ref:`con_dyn_var@Dynamic` parameter,
*parameter_x* [ *j* ] value of *ax* [ *j* ] corresponding to the
previous call to :ref:`new_dynamic-name` for the corresponding function object.

Variable
========
If *ax* [ *j* ] is a variable,
the value of *parameter_x* [ *j* ] is not specified.
See the
:ref:`atomic_three_mat_mul.hpp<atomic_three_mat_mul.hpp@Purpose@parameter_x>`
for an example using *parameter_x* .

type_x
******
All the virtual functions include this argument.
Its size is equal to *n* = *ax* . ``size`` ()
in corresponding *afun* ( *ax* , *ay* ) call.
For *j* =0,..., *n* ``-1`` ,
if *ax* [ *j* ] is a constant parameter,

   *type_x* [ *j* ] == ``CppAD::constant_enum``

if *ax* [ *j* ] is a dynamic parameter,

   *type_x* [ *j* ] == ``CppAD::dynamic_enum``

if *ax* [ *j* ] is a variable,

   *type_x* [ *j* ] == ``CppAD::variable_enum``

See the
:ref:`atomic_three_mat_mul.hpp<atomic_three_mat_mul.hpp@Purpose@type_x>`
for an example using *type_x* .

Contents
********
{xrst_toc_table
   include/cppad/core/atomic/three/ctor.hpp
   include/cppad/core/atomic/three/afun.hpp
   include/cppad/core/atomic/three/for_type.hpp
   include/cppad/core/atomic/three/forward.hpp
   include/cppad/core/atomic/three/reverse.hpp
   include/cppad/core/atomic/three/jac_sparsity.hpp
   include/cppad/core/atomic/three/hes_sparsity.hpp
   include/cppad/core/atomic/three/rev_depend.hpp
}

{xrst_end atomic_three_define}
-------------------------------------------------------------------------------
*/

# include <set>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/atomic_index.hpp>

// needed before one can use in_parallel
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic_three.hpp
Base class for atomic function operations.
*/

template <class Base>
class atomic_three {
// ===================================================================
private:
   // ------------------------------------------------------
   // constants
   //
   /// index of this object in local::atomic_index
   /// (set by constructor and not changed; i.e., effectively const)
   size_t index_;
   //
   // -----------------------------------------------------
   //
   /// temporary work space used by member functions, declared here to avoid
   // memory allocation/deallocation for each usage
   struct work_struct {
      vector<ad_type_enum>        type_x;
      vector<ad_type_enum>        type_y;
      //
      vector<Base>                taylor_x;
      vector<Base>                taylor_y;
      //
      vector< AD<Base> >          ataylor_x;
      vector< AD<Base> >          ataylor_y;
      //
      sparse_rc< vector<size_t> > pattern;
   };
   // Use pointers, to avoid false sharing between threads.
   // Not using: vector<work_struct*> work_;
   // so that deprecated atomic examples do not result in a memory leak.
   work_struct* work_[CPPAD_MAX_NUM_THREADS];
   // -----------------------------------------------------
public:
   //
   // atomic_index
   size_t atomic_index(void) const
   { return index_; }
   // =====================================================================
   // In User API
   // =====================================================================
   //
   // ---------------------------------------------------------------------
   // ctor: doxygen in atomic/three_ctor.hpp
   atomic_three(void);
   atomic_three(const std::string& name);

   // ------------------------------------------------------------------------
   // operator(): see doxygen in atomic_three/afun.hpp
   template <class ADVector>
   void operator()(
      const ADVector&  ax     ,
              ADVector&  ay
   );
   // ------------------------------------------------------------------------
   // type: doxygen in atomic/three_for_type.hpp
   virtual bool for_type(
      const vector<Base>&          parameter_x ,
      const vector<ad_type_enum>&  type_x      ,
      vector<ad_type_enum>&        type_y
   );
   // ------------------------------------------------------------------------
   // type: doxygen in atomic/three_rev_depend.hpp
   virtual bool rev_depend(
      const vector<Base>&          parameter_x ,
      const vector<ad_type_enum>&  type_x      ,
      vector<bool>&                depend_x    ,
      const vector<bool>&          depend_y
   );
   // ------------------------------------------------------------------------
   // forward: see docygen in atomic/three_forward.hpp
   virtual bool forward(
      const vector<Base>&          parameter_x ,
      const vector<ad_type_enum>&  type_x      ,
      size_t                       need_y      ,
      size_t                       order_low   ,
      size_t                       order_up    ,
      const vector<Base>&          taylor_x    ,
      vector<Base>&                taylor_y
   );
   virtual bool forward(
      const vector< AD<Base> >&    aparameter_x ,
      const vector<ad_type_enum>&  type_x       ,
      size_t                       need_y       ,
      size_t                       order_low    ,
      size_t                       order_up     ,
      const vector< AD<Base> >&    ataylor_x    ,
      vector< AD<Base> >&          ataylor_y
   );
   // ------------------------------------------------------------------------
   // reverse: see docygen in atomic/three_reverse.hpp
   virtual bool reverse(
      const vector<Base>&          parameter_x ,
      const vector<ad_type_enum>&  type_x      ,
      size_t                       order_up    ,
      const vector<Base>&          taylor_x    ,
      const vector<Base>&          taylor_y    ,
      vector<Base>&                partial_x   ,
      const vector<Base>&          partial_y
   );
   virtual bool reverse(
      const vector< AD<Base> >&    aparameter_x ,
      const vector<ad_type_enum>&  type_x       ,
      size_t                       order_up    ,
      const vector< AD<Base> >&    ataylor_x   ,
      const vector< AD<Base> >&    ataylor_y   ,
      vector< AD<Base> >&          apartial_x  ,
      const vector< AD<Base> >&    apartial_y
   );
   // ------------------------------------------------------------
   // jac_sparsity: see doxygen in atomic/three_jac_sparsity.hpp
   virtual bool jac_sparsity(
      const vector<Base>&          parameter_x ,
      const vector<ad_type_enum>&  type_x      ,
      bool                         dependency  ,
      const vector<bool>&          select_x    ,
      const vector<bool>&          select_y    ,
      sparse_rc< vector<size_t> >& pattern_out
   );
   template <class InternalSparsity>
   bool for_jac_sparsity(
      bool                             dependency   ,
      const vector<Base>&              parameter_x  ,
      const vector<ad_type_enum>&      type_x       ,
      const vector<size_t>&            x_index      ,
      const vector<size_t>&            y_index      ,
      InternalSparsity&                var_sparsity
   );
   template <class InternalSparsity>
   bool rev_jac_sparsity(
      bool                             dependency   ,
      const vector<Base>&              parameter_x  ,
      const vector<ad_type_enum>&      type_x       ,
      const vector<size_t>&            x_index      ,
      const vector<size_t>&            y_index      ,
      InternalSparsity&                var_sparsity
   );
   // ------------------------------------------------------------
   // hes_sparsity: see doxygen in atomic/three_jac_sparsity.hpp
   virtual bool hes_sparsity(
      const vector<Base>&                     parameter_x  ,
      const vector<ad_type_enum>&             type_x       ,
      const vector<bool>&                     select_x     ,
      const vector<bool>&                     select_y     ,
      sparse_rc< vector<size_t> >&            pattern_out
   );
   template <class InternalSparsity>
   bool for_hes_sparsity(
      const vector<Base>&              parameter_x      ,
      const vector<ad_type_enum>&      type_x           ,
      const vector<size_t>&            x_index          ,
      const vector<size_t>&            y_index          ,
      size_t                           np1              ,
      size_t                           numvar           ,
      const InternalSparsity&          rev_jac_sparsity ,
      InternalSparsity&                for_sparsity
   );
   template <class InternalSparsity>
   bool rev_hes_sparsity(
      const vector<Base>&              parameter_x      ,
      const vector<ad_type_enum>&      type_x           ,
      const vector<size_t>&            x_index          ,
      const vector<size_t>&            y_index          ,
      const InternalSparsity&          for_jac_pattern  ,
      bool*                            rev_jac_flag     ,
      InternalSparsity&                hes_sparsity
   );

   // =====================================================================
   // Not in User API
   // =====================================================================

   /// Name corresponding to a atomic_three object
   const std::string atomic_name(void) const
   {  bool        set_null = false;
      size_t      type  = 0;          // set to avoid warning
      std::string name;
      void*       v_ptr = nullptr; // set to avoid warning
      local::atomic_index<Base>(set_null, index_, type, &name, v_ptr);
      CPPAD_ASSERT_UNKNOWN( type == 3 );
      return name;
   }
   /// destructor informs CppAD that this atomic function with this index
   /// has dropped out of scope by setting its pointer to null
   virtual ~atomic_three(void)
   {  // change object pointer to null, but leave name for error reporting
      bool         set_null = true;
      size_t       type  = 0;          // set to avoid warning
      std::string* name  = nullptr;
      void*        v_ptr = nullptr; // set to avoid warning
      local::atomic_index<Base>(set_null, index_, type, name, v_ptr);
      CPPAD_ASSERT_UNKNOWN( type == 3 );
      //
      // free temporary work memory
      for(size_t thread = 0; thread < CPPAD_MAX_NUM_THREADS; thread++)
         free_work(thread);
   }
   /// allocates work_ for a specified thread
   void allocate_work(size_t thread)
   {  if( work_[thread] == nullptr )
      {  // allocate the raw memory
         size_t min_bytes = sizeof(work_struct);
         size_t num_bytes;
         void*  v_ptr     = thread_alloc::get_memory(min_bytes, num_bytes);
         // save in work_
         work_[thread]    = reinterpret_cast<work_struct*>( v_ptr );
         // call constructor
         new( work_[thread] ) work_struct;
      }
      return;
   }
   /// frees work_ for a specified thread
   void free_work(size_t thread)
   {  if( work_[thread] != nullptr )
      {  // call destructor
         work_[thread]->~work_struct();
         // return memory to avialable pool for this thread
         thread_alloc::return_memory(
            reinterpret_cast<void*>(work_[thread])
         );
         // mark this thread as not allocated
         work_[thread] = nullptr;
      }
      return;
   }
   /// atomic_three function object corresponding to a certain index
   static atomic_three* class_object(size_t index)
   {  bool         set_null = false;
      size_t       type  = 0;          // set to avoid warning
      std::string* name  = nullptr;
      void*        v_ptr = nullptr; // set to avoid warning
      local::atomic_index<Base>(set_null, index, type, name, v_ptr);
      CPPAD_ASSERT_UNKNOWN( type == 3 );
      return reinterpret_cast<atomic_three*>( v_ptr );
   }
   /// atomic_three function name corresponding to a certain index
   static const std::string class_name(size_t index)
   {  bool        set_null = false;
      size_t      type  = 0;          // set to avoid warning
      std::string name;
      void*       v_ptr = nullptr; // set to avoid warning
      local::atomic_index<Base>(set_null, index, type, &name, v_ptr);
      CPPAD_ASSERT_UNKNOWN( type == 3 );
      return name;
   }

   /*!
   Set value of id (used by deprecated atomic_one class)

   This function is called just before calling any of the virtual function
   and has the corresponding id of the corresponding virtual call.
   */
   virtual void set_old(size_t id)
   { }
// ---------------------------------------------------------------------------
};
} // END_CPPAD_NAMESPACE

// member functions
# include <cppad/core/atomic/three/ctor.hpp>
# include <cppad/core/atomic/three/afun.hpp>
# include <cppad/core/atomic/three/for_type.hpp>
# include <cppad/core/atomic/three/rev_depend.hpp>
# include <cppad/core/atomic/three/forward.hpp>
# include <cppad/core/atomic/three/reverse.hpp>
# include <cppad/core/atomic/three/jac_sparsity.hpp>
# include <cppad/core/atomic/three/hes_sparsity.hpp>

# endif
