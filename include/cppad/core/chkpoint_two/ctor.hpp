# ifndef CPPAD_CORE_CHKPOINT_TWO_CTOR_HPP
# define CPPAD_CORE_CHKPOINT_TWO_CTOR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin chkpoint_two_ctor}
{xrst_spell
   chk
}

Checkpoint Function Constructor
###############################

Syntax
******
| ``chkpoint_two`` < *Base* > *chk_fun* ( *fun* , *name* ,
| |tab| *internal_bool* , *use_hes_sparsity* , *use_base2ad* , *use_in_parallel*
| )

Prototype
*********
{xrst_literal
   // BEGIN_PROTOTYPE
   // END_PROTOTYPE
}

Parallel
********
This constructor, and its corresponding destructor, must not be called in
:ref:`parallel<ta_in_parallel-name>` mode.
The object *chk_fun* should not be destructed for as long as there is
an ``ADFun`` < *Base* > object the has *chk_fun* in its recording.

Base
****
The type *Base* specifies the base type for AD operations.

fun
***
This specifies the function :math:`g(x)`.
Note that *fun* may or may not have been
:ref:`optimized<optimize-name>` before calling the constructor.
This will determine if the internal representation for *g* ( *x* )
is optimized.

name
****
is the name used for reporting errors using this checkpoint function.

internal_bool
*************
If true, sparsity calculations are done with sets represented
by vectors of boolean values.
Otherwise, vectors of sets are used for sparsity patterns.

use_hes_sparsity
****************
If true, Hessian sparsity patterns can be calculated for
``ADFun`` < *Base* > objects that have uses of *chk_fun*
in their recording.
This requires some extra memory and extra computation during the constructor.

use_base2ad
***********
If this is true, *chk_fun* can be used during the recording
of ``ADFun`` < *Base* > objects that get converted to
``ADFun< AD<`` *Base* > > objects using :ref:`base2ad-name` .
This requires some extra memory and extra computation during the constructor.

use_in_parallel
***************
If this is true, *chk_fun* can be used
:ref:`ta_parallel_setup@in_parallel` .
This requires some extra memory for a constant copy of the *fun*
information and a separate copy (that changes) for each thread.

chk_fun
*******
This is a checkpoint function representation of :math:`g(x)`
that can be used during the recording of ``AD`` < *Base* > operations.

{xrst_end chkpoint_two_ctor}
*/
namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file chkpoint_two/ctor.hpp
Constructor for chkpoint_two class.
*/

/*!
Constructor

\tparam Base
base class for recording AD<Base> operations using this checkpoint object.

\param fun
is the function g(x) corresponding to this checkpoint object.

\param name
is the name used for error reporting.

\param internal_bool
should sparisity calculations be done using bools (or sets).

\param use_hes_sparsity
will this checkpoint function be used with Hessian sparsity calculations.

\param use_base2ad
will this checkpoint function be used with base2ad.

\param use_in_parallel
will this checkpoint function be used in parallel mode.
*/

// BEGIN_PROTOTYPE
template <class Base>
chkpoint_two<Base>::chkpoint_two(
      const ADFun<Base>& fun    ,
      const std::string& name   ,
      bool  internal_bool       ,
      bool  use_hes_sparsity    ,
      bool  use_base2ad         ,
      bool  use_in_parallel     )
// END_PROTOTYPE
:
atomic_three<Base>(name)              ,
internal_bool_( internal_bool )       ,
use_hes_sparsity_( use_hes_sparsity ) ,
use_base2ad_ ( use_base2ad )          ,
use_in_parallel_ ( use_in_parallel )
{  CPPAD_ASSERT_KNOWN(
      ! thread_alloc::in_parallel() ,
      "chkpoint_two: constructor cannot be called in parallel mode."
   );
   // initialize member pointers as null;
   for(size_t thread = 0; thread < CPPAD_MAX_NUM_THREADS; thread++)
      member_[thread] = nullptr;
   //
   // g_
   g_ = fun;
   //
   // suppress check for nan because chkpoint_two object can be used in a
   // function that gets optimized and some checkpoint results may not matter.
   g_.check_for_nan(false);
   //
   // ag_
   if( use_base2ad )
      ag_ = g_.base2ad();
   //
   // jac_sparsity__
   size_t n = g_.Domain();
   size_t m = g_.Range();
   sparse_rc< vector<size_t> > pattern_in;
   bool transpose     = false;
   bool dependency    = true;
   if( n <= m || use_hes_sparsity )
   {  // use forward mode
      pattern_in.resize(n, n, n);
      for(size_t k = 0; k < n; ++k)
         pattern_in.set(k, k, k);
      g_.for_jac_sparsity(
         pattern_in,
         transpose,
         dependency,
         internal_bool,
         jac_sparsity_
      );
   }
   else
   {  // use reverse mode
      pattern_in.resize(m, m, m);
      for(size_t k = 0; k < m; ++k)
         pattern_in.set(k, k, k);
      g_.rev_jac_sparsity(
         pattern_in,
         transpose,
         dependency,
         internal_bool,
         jac_sparsity_
      );
   }
   //
   // hes_sparsity_
   if( use_hes_sparsity )
   {  vector<bool> select_y(m), select_x(n);
      for(size_t i = 0; i < m; ++i)
         select_y[i] = true;
      if( n <= m )
      {  for(size_t j = 0; j < n; ++j)
            select_x[j] = true;
         g_.for_hes_sparsity(
            select_x, select_y, internal_bool, hes_sparsity_
         );
      }
      else
      {  // forward jacobian sparsity is stored in g_
         g_.rev_hes_sparsity(
            select_y, transpose, internal_bool, hes_sparsity_
         );
      }
   }
   // free memory holding forward Jacobian sparsity
   if( internal_bool )
      g_.size_forward_bool(0);
   else
      g_.size_forward_set(0);
}
/// destructor
template <class Base>
chkpoint_two<Base>::~chkpoint_two(void)
{
# ifndef NDEBUG
   if( thread_alloc::in_parallel() )
   {  std::string msg = atomic_three<Base>::atomic_name();
      msg += ": chkpoint_two destructor called in parallel mode.";
      CPPAD_ASSERT_KNOWN(false, msg.c_str() );
   }
# endif
   for(size_t thread = 0; thread < CPPAD_MAX_NUM_THREADS; ++thread)
      free_member(thread);
   }
} // END_CPPAD_NAMESPACE
# endif
