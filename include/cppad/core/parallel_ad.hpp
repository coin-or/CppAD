# ifndef CPPAD_CORE_PARALLEL_AD_HPP
# define CPPAD_CORE_PARALLEL_AD_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin parallel_ad}
{xrst_spell
   teardown
}

Enable AD Calculations During Parallel Mode
###########################################

Syntax
******
``parallel_ad`` < *Base* >()

Purpose
*******
The function
``parallel_ad`` < *Base* >()
must be called before any ``AD`` < *Base>* objects are used
in :ref:`parallel<ta_in_parallel-name>` mode.
In addition, if this routine is called after one is done using
parallel mode, it will free extra memory used to keep track of
the multiple ``AD`` < *Base* > tapes required for parallel execution.

Discussion
**********
By default, for each ``AD`` < *Base* > class there is only one
tape that records :ref:`glossary@AD of Base` operations.
This tape is a global variable and hence it cannot be used
by multiple threads at the same time.
The :ref:`parallel_setup<ta_parallel_setup-name>` function informs CppAD of the
maximum number of threads that can be active in parallel mode.
This routine does extra setup
(and teardown) for the particular *Base* type.

CheckSimpleVector
*****************
This routine has the side effect of calling the routines

   ``CheckSimpleVector`` < *Type* , ``CppAD::vector<`` *Type* > >()

where *Type* is *Base* and ``AD`` < *Base* > .

Example
*******
The files
:ref:`team_openmp.cpp-name` ,
:ref:`team_bthread.cpp-name` , and
:ref:`team_pthread.cpp-name` ,
contain examples and tests that implement this function.

Restriction
***********
This routine cannot be called in parallel mode or while
there is a tape recording ``AD`` < *Base* > operations.

{xrst_end parallel_ad}
-----------------------------------------------------------------------------
*/

# include <cppad/local/std_set.hpp>

// BEGIN CppAD namespace
namespace CppAD {

/*!
Enable parallel execution mode with <code>AD<Base></code> by initializing
static variables that my be used.
*/

template <class Base>
void parallel_ad(void)
{  CPPAD_ASSERT_KNOWN(
      ! thread_alloc::in_parallel() ,
      "parallel_ad must be called before entering parallel execution mode."
   );
   CPPAD_ASSERT_KNOWN(
      AD<Base>::tape_ptr() == nullptr ,
      "parallel_ad cannot be called while a tape recording is in progress"
   );

   // ensure statics in following functions are initialized
   elapsed_seconds();
   ErrorHandler::Current();
   local::NumArg(local::BeginOp);
   local::NumRes(local::BeginOp);
   local::one_element_std_set<size_t>();
   local::two_element_std_set<size_t>();

   // the sparse_pack class has member functions with static data
   local::sparse::pack_setvec sp;
   sp.resize(1, 1);       // so can call add_element
   sp.add_element(0, 0);  // has static data
   sp.clear(0);           // has static data
   sp.is_element(0, 0);   // has static data
   local::sparse::pack_setvec::const_iterator itr(sp, 0); // has static data
   ++itr;                                  // has static data

   // statics that depend on the value of Base
   AD<Base>::tape_id_ptr(0);
   AD<Base>::tape_handle(0);
   discrete<Base>::List();
   CheckSimpleVector< Base, CppAD::vector<Base> >();
   CheckSimpleVector< AD<Base>, CppAD::vector< AD<Base> > >();

}

} // END CppAD namespace

# endif
