# ifndef CPPAD_CORE_PARALLEL_AD_HPP
# define CPPAD_CORE_PARALLEL_AD_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin parallel_ad}
{xrst_spell
   rosen
   teardown
}

Enable AD Calculations During Parallel Mode
###########################################

Syntax
******
| ``parallel_ad`` < *Base* >()

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
This routine has the side effect of calling ``CheckSimpleVector`` for
some of the possible
:ref:`CheckSimpleVector@Scalar` and :ref:`CheckSimpleVector@Vector` cases.
The set of these cases may increase in the future and currently includes
the following:

.. csv-table::
   :header: Scalar, Vector

   ``bool``          , ``CppAD::vectorBool``
   ``size_t``        , ``CppAD::vector<size_t>``
   *Base*            , *vector* < *Base* >
   ``AD`` < *Base* > , *vector* ``AD`` < *Base* >

Where *vector* above is
``CppAD::vector`` ,
``std::vector`` , and
the :ref:`cppad_testvector-name` .


Example
*******
The files
:ref:`openmp_get_started.cpp-name` ,
:ref:`bthread_get_started.cpp-name` , and
:ref:`pthread_get_started.cpp-name` ,
contain examples and tests that implement this function.

Restriction
***********
This routine cannot be called in parallel mode or while
there is a tape recording ``AD`` < *Base* > operations.

Other Initialization
********************
If the following routines have static memory and must be called once
before being used in parallel mode:

#. :ref:`CheckSimpleVector <CheckSimpleVector@Parallel Mode>`
#. :ref:`thread_alloc, memory_leak <ta_parallel_setup-name>`
#. :ref:`Rosen34 <Rosen34@Parallel Mode>`
#. :ref:`Runge45 <Runge45@Parallel Mode>`
#. :ref:`discrete <Discrete@Parallel Mode>`
#. :ref:`atomic_one <atomic_one@afun@Parallel Mode>`


{xrst_end parallel_ad}
-----------------------------------------------------------------------------
*/

# include <vector>
# include <cppad/utility/vector.hpp>
# include <cppad/local/std_set.hpp>
# include <cppad/local/val_graph/enable_parallel.hpp>

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
   ErrorHandler::Current();                // error_handler.hpp
   elapsed_seconds();                      // elapsed_seconds.hpp
   local::num_arg_dyn(local::abs_dyn);     // op_code_dyn.hpp
   local::op_name_dyn(local::abs_dyn);     // op_code_dyn.hpp
   local::NumArg(local::BeginOp);          // op_code_var.hpp
   local::NumRes(local::BeginOp);          // op_code_var.hpp
   local::one_element_std_set<size_t>();   // std_set.hpp
   local::two_element_std_set<size_t>();   // std_set.hpp

   // the sparse_pack class has member functions with static data
   local::sparse::pack_setvec sp;
   sp.resize(1, 1);       // so can call add_element
   sp.add_element(0, 0);  // has static data
   sp.clear(0);           // has static data
   sp.is_element(0, 0);   // has static data
   local::sparse::pack_setvec::const_iterator itr(sp, 0); // has static data
   ++itr;                                  // has static data

   // statics that depend on the value of Base
   AD<Base>::tape_id_ptr(0);                  // tape_link.hpp
   AD<Base>::tape_handle(0);                  // tape_link.hpp
   local::val_graph::enable_parallel<Base>(); // val_graph/*_op.hpp
   discrete<Base>::List();                    // discrete.hpp

   // Some check_simple_vector.hpp cases
   //
   CheckSimpleVector< bool, CppAD::vectorBool >();
   CheckSimpleVector< size_t, CppAD::vector<size_t> >();
   CheckSimpleVector< Base, CppAD::vector<Base> >();
   CheckSimpleVector< AD<Base>, CppAD::vector< AD<Base> > >();
   //
   CheckSimpleVector< Base, std::vector<Base> >();
   CheckSimpleVector< AD<Base>, std::vector< AD<Base> > >();
   //
# if CPPAD_BOOSTVECTOR
   CheckSimpleVector< Base, boost::numeric::ublas::vector<Base> >();
   CheckSimpleVector< AD<Base>, boost::numeric::ublas::vector< AD<Base> > >();
# endif
   //
# if CPPAD_EIGENVECTOR
   CheckSimpleVector< Base, CppAD::eigen_vector<Base> >();
   CheckSimpleVector< AD<Base>, CppAD::eigen_vector< AD<Base> > > ();
# endif

}

} // END CppAD namespace

# endif
