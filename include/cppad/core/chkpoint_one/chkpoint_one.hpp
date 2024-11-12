# ifndef CPPAD_CORE_CHKPOINT_ONE_CHKPOINT_ONE_HPP
# define CPPAD_CORE_CHKPOINT_ONE_CHKPOINT_ONE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/sparse/list_setvec.hpp>
# include <cppad/local/sparse/pack_setvec.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file chkpoint_one.hpp
First generation checkpoint functions.
*/

/*
{xrst_begin chkpoint_one app}
{xrst_spell
   algo
   sv
}

Checkpoint Functions: First Generation
######################################

Deprecated 2019-01-14
*********************
Using the ``checkpoint`` class has been deprecated.
Use :ref:`chkpoint_two-name` instead.

Syntax
******
| ``checkpoint`` < *Base* > *atom_fun* (
| |tab| *name* , *algo* , *ax* , *ay* , *sparsity* , *optimize*
| )
| *sv* = *atom_fun* . ``size_var`` ()
| *atom_fun* . ``option`` ( *option_value* )
| *algo* ( *ax* , *ay* )
| *atom_fun* ( *ax* , *ay* )
| *checkpoint* < ``Base`` >:: *clear* ()

See Also
********
:ref:`atomic_two-name` , :ref:`rev_checkpoint.cpp-name`

Purpose
*******

Reduce Memory
=============
You can reduce the size of the tape and memory required for AD by
checkpointing functions of the form :math:`y = f(x)` where
:math:`f : \B{R}^n \rightarrow \B{R}^m`.

Faster Recording
================
It may also reduce the time to make a recording the same function
for different values of the independent variable.
Note that the operation sequence for a recording that uses :math:`f(x)`
may depend on its independent variables.

Repeating Forward
=================
Normally, CppAD store :ref:`forward-name` mode results until they freed
using :ref:`capacity_order-name` or the corresponding :ref:`ADFun-name` object is
deleted. This is not true for checkpoint functions because a checkpoint
function may be used repeatedly with different arguments in the same tape.
Thus, forward mode results are recomputed each time a checkpoint function
is used during a forward or reverse mode sweep.

Restriction
===========
The :ref:`operation sequence<glossary@Operation@Sequence>`
representing :math:`f(x)` cannot depend on the value of :math:`x`.
The approach in the :ref:`rev_checkpoint.cpp-name` example case be applied
when the operation sequence depends on :math:`x`.

Multiple Level AD
=================
If *Base* is an AD type, it is possible to record *Base*
operations.
Note that *atom_fun* will treat *algo* as an atomic
operation while recording ``AD`` < ``Base`` > operations, but not while
recording *Base* operations.
See the ``chkpoint_one_mul_level.cpp`` example.

Method
******
The ``checkpoint`` class is derived from ``atomic_base``
and makes this easy.
It implements all the ``atomic_base``
:ref:`atomic_two@Virtual Functions`
and hence its source code ``cppad/core/chkpoint_one/chkpoint_one.hpp``
provides an example implementation of :ref:`atomic_two-name` .
The difference is that ``chkpoint_one.hpp`` uses AD
instead of user provided derivatives.

constructor
***********
The syntax for the checkpoint constructor is

   ``checkpoint`` < *Base* > *atom_fun* ( *name* , *algo* , *ax* , *ay* )

#. This constructor cannot be called in :ref:`parallel<ta_in_parallel-name>` mode.
#. You cannot currently be recording
   ``AD`` < *Base* > operations when the constructor is called.
#. This object *atom_fun* must not be destructed for as long
   as any ``ADFun`` < *Base* > object uses its atomic operation.
#. This class is implemented as a derived class of
   :ref:`atomic<atomic_two_ctor@atomic_base>` and hence
   some of its error message will refer to ``atomic_base`` .

Base
****
The type *Base* specifies the base type for AD operations.

ADVector
********
The type *ADVector* must be a
:ref:`simple vector class<SimpleVector-name>` with elements of type
``AD`` < *Base* > .

name
****
This *checkpoint* constructor argument has prototype

   ``const char`` * *name*

It is the name used for error reporting.
The suggested value for *name* is *atom_fun* ; i.e.,
the same name as used for the object being constructed.

ax
**
This argument has prototype

   ``const`` *ADVector* & *ax*

and size must be equal to *n* .
It specifies vector :math:`x \in \B{R}^n`
at which an ``AD`` < *Base* > version of
:math:`y = f(x)` is to be evaluated.

ay
**
This argument has prototype

   *ADVector* & *ay*

Its input size must be equal to *m* and does not change.
The input values of its elements do not matter.
Upon return, it is an ``AD`` < *Base* > version of
:math:`y = f(x)`.

sparsity
********
This argument has prototype

   ``atomic_base`` < *Base* >:: ``option_enum`` *sparsity*

It specifies :ref:`atomic_two_ctor@atomic_base@sparsity`
in the ``atomic_base`` constructor and must be either
``atomic_base`` < *Base* >:: ``pack_sparsity_enum`` ,
``atomic_base`` < *Base* >:: ``bool_sparsity_enum`` , or
``atomic_base`` < *Base* >:: ``set_sparsity_enum`` .
This argument is optional and its default value is unspecified.

optimize
********
This argument has prototype

   ``bool`` *optimize*

It specifies if the recording corresponding to the atomic function
should be :ref:`optimized<optimize-name>` .
One expects to use a checkpoint function many times, so it should
be worth the time to optimize its operation sequence.
For debugging purposes, it may be useful to use the
original operation sequence (before optimization)
because it corresponds more closely to *algo* .
This argument is optional and its default value is true.

size_var
********
This ``size_var`` member function return value has prototype

   ``size_t`` *sv*

It is the :ref:`fun_property@size_var` for the
``ADFun`` < *Base* > object is used to store the operation sequence
corresponding to *algo* .

option
******
The ``option`` syntax can be used to set the type of sparsity
pattern used by *atom_fun* .
This is an ``atomic_base`` < *Base* > function and its documentation
can be found at :ref:`atomic_two_option-name` .

algo
****
The type of *algo* is arbitrary, except for the fact that
the syntax

   *algo* ( *ax* , *ay* )

must evaluate the function :math:`y = f(x)` using
``AD`` < *Base* > operations.
In addition, we assume that the
:ref:`operation sequence<glossary@Operation@Sequence>`
does not depend on the value of *ax* .

atom_fun
********
Given *ax* it computes the corresponding value of *ay*
using the operation sequence corresponding to *algo* .
If ``AD`` < *Base* > operations are being recorded,
it enters the computation as single operation in the recording
see :ref:`Independent@Start Recording` .
(Currently each use of *atom_fun* actually corresponds to
*m* + *n* +2 operations and creates *m* new variables,
but this is not part of the CppAD specifications and my change.)

Memory
******

Restriction
===========
The ``clear`` routine cannot be called
while in :ref:`parallel<ta_in_parallel-name>` execution mode.

Parallel Mode
*************
The CppAD checkpoint function delays the caching of certain calculations
until they are needed.
In :ref:`parallel model<ta_parallel_setup-name>` ,
this may result in :ref:`thread_alloc::inuse(thread)<ta_inuse-name>`
being non-zero even though the specified thread is no longer active.
This memory will be freed when the checkpoint object is deleted.

clear
=====
The ``atomic_base`` class holds onto static work space
that is not connected to a particular object
(in order to increase speed by avoiding system memory allocation calls).
This call makes to work space :ref:`available<ta_available-name>` to
for other uses by the same thread.
This should be called when you are done using the
atomic functions for a specific value of *Base* .

{xrst_end chkpoint_one}
*/

template <class Base>
class checkpoint : public atomic_base<Base> {
// ---------------------------------------------------------------------------
private:
   /// same as option_enum in base class
   typedef typename atomic_base<Base>::option_enum option_enum;
   //
   // ------------------------------------------------------------------------
   // member_
   // ------------------------------------------------------------------------
   // same checkpoint object can be used by multiple threads
   struct member_struct {
      //
      /// AD function corresponding to this checkpoint object
      ADFun<Base>             f_;
      ADFun< AD<Base>, Base > af_;
      //
      /// sparsity for entire Jacobian f(x)^{(1)}
      /// does not change so can cache it
      local::sparse::list_setvec jac_sparse_set_;
      vectorBool                 jac_sparse_bool_;
      //
      /// sparsity for sum_i f_i(x)^{(2)} does not change so can cache it
      local::sparse::list_setvec hes_sparse_set_;
      vectorBool                 hes_sparse_bool_;
   };
   /// This version of work is const except during constructor
   member_struct const_member_;

   /// use pointers and allocate memory to avoid false sharing
   member_struct* member_[CPPAD_MAX_NUM_THREADS];
   //
   /// allocate member_ for this thread
   void allocate_member(size_t thread)
   {  if( member_[thread] == nullptr )
      {  member_[thread] = new member_struct;
         // The function is recorded in sequential mode and placed in
         // const_member_.f_, other threads have copy.
         member_[thread]->f_ = const_member_.f_;
      }
      return;
   }
   //
   /// free member_ for this thread
   void free_member(size_t thread)
   {  if( member_[thread] != nullptr )
      {  delete member_[thread];
         member_[thread] = nullptr;
      }
      return;
   }
   // ------------------------------------------------------------------------
   option_enum sparsity(void)
   {  return static_cast< atomic_base<Base>* >(this)->sparsity(); }
   // ------------------------------------------------------------------------
   /// set jac_sparse_set_
   void set_jac_sparse_set(void);
   /// set jac_sparse_bool_
   void set_jac_sparse_bool(void);
   // ------------------------------------------------------------------------
   /// set hes_sparse_set_
   void set_hes_sparse_set(void);
   /// set hes_sparse_bool_
   void set_hes_sparse_bool(void);
   // ------------------------------------------------------------------------
   /*!
   Link from user_atomic to forward sparse Jacobian pack and bool

   \copydetails atomic_base::for_sparse_jac
   */
   template <class sparsity_type>
   bool for_sparse_jac(
      size_t                                  q  ,
      const sparsity_type&                    r  ,
              sparsity_type&                    s  ,
      const vector<Base>&                     x
   );
   // ------------------------------------------------------------------------
   /*!
   Link from user_atomic to reverse sparse Jacobian pack and bool

   \copydetails atomic_base::rev_sparse_jac
   */
   template <class sparsity_type>
   bool rev_sparse_jac(
      size_t                                  q  ,
      const sparsity_type&                    rt ,
              sparsity_type&                    st ,
      const vector<Base>&                     x
   );
   /*!
   Link from user_atomic to reverse sparse Hessian  bools

   \copydetails atomic_base::rev_sparse_hes
   */
   template <class sparsity_type>
   bool rev_sparse_hes(
      const vector<bool>&                     vx ,
      const vector<bool>&                     s  ,
              vector<bool>&                     t  ,
      size_t                                  q  ,
      const sparsity_type&                    r  ,
      const sparsity_type&                    u  ,
              sparsity_type&                    v  ,
      const vector<Base>&                     x
   );
public:
   // ------------------------------------------------------------------------
   /*!
   Constructor of a checkpoint object

   \param name [in]
   is the user's name for the AD version of this atomic operation.

   \param algo [in/out]
   user routine that compute AD function values
   (not const because state may change during evaluation).

   \param ax [in]
   argument value where algo operation sequence is taped.

   \param ay [out]
   function value at specified argument value.

   \param sparsity [in]
   what type of sparsity patterns are computed by this function,
   pack_sparsity_enum bool_sparsity_enum, or set_sparsity_enum.
   The default value is unspecified.

   \param optimize [in]
   should the operation sequence corresponding to the algo be optimized.
   The default value is true, but it is
   sometimes useful to use false for debugging purposes.
   */
   template <class Algo, class ADVector>
   checkpoint(
      const char*                    name            ,
      Algo&                          algo            ,
      const ADVector&                ax              ,
      ADVector&                      ay              ,
      option_enum                    sparsity =
            atomic_base<Base>::pack_sparsity_enum  ,
      bool                           optimize = true
   );
   /// destructor
   ~checkpoint(void)
   {
# ifndef NDEBUG
      if( thread_alloc::in_parallel() )
      {  std::string msg = atomic_base<Base>::atomic_name();
         msg += ": checkpoint destructor called in parallel mode.";
         CPPAD_ASSERT_KNOWN(false, msg.c_str() );
      }
# endif
      for(size_t thread = 0; thread < CPPAD_MAX_NUM_THREADS; ++thread)
         free_member(thread);
   }
   // ------------------------------------------------------------------------
   /*!
   Implement the user call to atom_fun.size_var().
   */
   size_t size_var(void)
   {  // make sure member_ is allocated for this thread
      size_t thread = thread_alloc::thread_num();
      allocate_member(thread);
      //
      return member_[thread]->f_.size_var();
   }
   // ------------------------------------------------------------------------
   /*!
   Implement the user call to atom_fun(ax, ay).

   \tparam ADVector
   A simple vector class with elements of type AD<Base>.

   \param id
   optional parameter which must be zero if present.

   \param ax
   is the argument vector for this call,
   ax.size() determines the number of arguments.

   \param ay
   is the result vector for this call,
   ay.size() determines the number of results.
   */
   template <class ADVector>
   void operator()(const ADVector& ax, ADVector& ay, size_t id = 0)
   {  CPPAD_ASSERT_KNOWN(
         id == 0,
         "checkpoint: id is non-zero in atom_fun(ax, ay, id)"
      );
      this->atomic_base<Base>::operator()(ax, ay, id);
   }
   // ------------------------------------------------------------------------
   /*!
   Link from user_atomic to forward mode

   \copydetails atomic_base::forward
   */
   virtual bool forward(
      size_t                      p  ,
      size_t                      q  ,
      const vector<bool>&         vx ,
              vector<bool>&         vy ,
      const vector<Base>&         tx ,
              vector<Base>&         ty
   );
   virtual bool forward(
      size_t                      p   ,
      size_t                      q   ,
      const vector<bool>&         vx  ,
              vector<bool>&         vy  ,
      const vector< AD<Base> >&   atx ,
              vector< AD<Base> >&   aty
   );
   // ------------------------------------------------------------------------
   /*!
   Link from user_atomic to reverse mode

   \copydetails atomic_base::reverse
   */
   virtual bool reverse(
      size_t                          q  ,
      const vector<Base>&             tx ,
      const vector<Base>&             ty ,
              vector<Base>&             px ,
      const vector<Base>&             py
   );
   virtual bool reverse(
      size_t                          q  ,
      const vector< AD<Base> >&       atx ,
      const vector< AD<Base> >&       aty ,
              vector< AD<Base> >&       apx ,
      const vector< AD<Base> >&       apy
   );
   // ------------------------------------------------------------------------
   /*!
   Link from user_atomic to forward sparse Jacobian pack

   \copydetails atomic_base::for_sparse_jac
   */
   virtual bool for_sparse_jac(
      size_t                                  q  ,
      const vectorBool&                       r  ,
              vectorBool&                       s  ,
      const vector<Base>&                     x
   );
   /*!
   Link from user_atomic to forward sparse Jacobian bool

   \copydetails atomic_base::for_sparse_jac
   */
   virtual bool for_sparse_jac(
      size_t                                  q  ,
      const vector<bool>&                     r  ,
              vector<bool>&                     s  ,
      const vector<Base>&                     x
   );
   /*!
   Link from user_atomic to forward sparse Jacobian sets

   \copydetails atomic_base::for_sparse_jac
   */
   virtual bool for_sparse_jac(
      size_t                                  q  ,
      const vector< std::set<size_t> >&       r  ,
              vector< std::set<size_t> >&       s  ,
      const vector<Base>&                     x
   );
   // ------------------------------------------------------------------------
   /*!
   Link from user_atomic to reverse sparse Jacobian pack

   \copydetails atomic_base::rev_sparse_jac
   */
   virtual bool rev_sparse_jac(
      size_t                                  q  ,
      const vectorBool&                       rt ,
              vectorBool&                       st ,
      const vector<Base>&                     x
   );
   /*!
   Link from user_atomic to reverse sparse Jacobian bool

   \copydetails atomic_base::rev_sparse_jac
   */
   virtual bool rev_sparse_jac(
      size_t                                  q  ,
      const vector<bool>&                     rt ,
              vector<bool>&                     st ,
      const vector<Base>&                     x
   );
   /*!
   Link from user_atomic to reverse Jacobian sets

   \copydetails atomic_base::rev_sparse_jac
   */
   virtual bool rev_sparse_jac(
      size_t                                  q  ,
      const vector< std::set<size_t> >&       rt ,
              vector< std::set<size_t> >&       st ,
      const vector<Base>&                     x
   );
   // ------------------------------------------------------------------------
   /*!
   Link from user_atomic to reverse sparse Hessian pack

   \copydetails atomic_base::rev_sparse_hes
   */
   virtual bool rev_sparse_hes(
      const vector<bool>&                     vx ,
      const vector<bool>&                     s  ,
              vector<bool>&                     t  ,
      size_t                                  q  ,
      const vectorBool&                       r  ,
      const vectorBool&                       u  ,
              vectorBool&                       v  ,
      const vector<Base>&                     x
   );
   /*!
   Link from user_atomic to reverse sparse Hessian bool

   \copydetails atomic_base::rev_sparse_hes
   */
   virtual bool rev_sparse_hes(
      const vector<bool>&                     vx ,
      const vector<bool>&                     s  ,
              vector<bool>&                     t  ,
      size_t                                  q  ,
      const vector<bool>&                     r  ,
      const vector<bool>&                     u  ,
              vector<bool>&                     v  ,
      const vector<Base>&                     x
   );
   /*!
   Link from user_atomic to reverse sparse Hessian sets

   \copydetails atomic_base::rev_sparse_hes
   */
   virtual bool rev_sparse_hes(
      const vector<bool>&                     vx ,
      const vector<bool>&                     s  ,
              vector<bool>&                     t  ,
      size_t                                  q  ,
      const vector< std::set<size_t> >&       r  ,
      const vector< std::set<size_t> >&       u  ,
              vector< std::set<size_t> >&       v  ,
      const vector<Base>&                     x
   );
};

} // END_CPPAD_NAMESPACE

// functions implemented in cppad/core/checkpoint files
# include <cppad/core/chkpoint_one/ctor.hpp>
# include <cppad/core/chkpoint_one/reverse.hpp>
# include <cppad/core/chkpoint_one/forward.hpp>
# include <cppad/core/chkpoint_one/rev_sparse_hes.hpp>
# include <cppad/core/chkpoint_one/rev_sparse_jac.hpp>
# include <cppad/core/chkpoint_one/for_sparse_jac.hpp>
# include <cppad/core/chkpoint_one/set_hes_sparse_bool.hpp>
# include <cppad/core/chkpoint_one/set_hes_sparse_set.hpp>
# include <cppad/core/chkpoint_one/set_jac_sparse_bool.hpp>
# include <cppad/core/chkpoint_one/set_jac_sparse_set.hpp>

# endif
