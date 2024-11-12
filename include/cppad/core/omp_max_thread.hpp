# ifndef CPPAD_CORE_OMP_MAX_THREAD_HPP
# define CPPAD_CORE_OMP_MAX_THREAD_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin omp_max_thread app}
{xrst_spell
   mp
}

OpenMP Parallel Setup
#####################

Deprecated 2011-06-23
*********************
Use :ref:`thread_alloc::parallel_setup<ta_parallel_setup-name>`
to set the number of threads.

Syntax
******
| ``AD`` < *Base* >:: ``omp_max_thread`` ( *number* )

Purpose
*******
By default, for each ``AD`` < *Base* > class there is only one
tape that records :ref:`glossary@AD of Base` operations.
This tape is a global variable and hence it cannot be used
by multiple OpenMP threads at the same time.
The ``omp_max_thread`` function is used to set the
maximum number of OpenMP threads that can be active.
In this case, there is a different tape corresponding to each
``AD`` < *Base* > class and thread pair.

number
******
The argument *number* has prototype

   ``size_t`` *number*

It must be greater than zero and specifies the maximum number of
OpenMp threads that will be active at one time.

Independent
***********
Each call to :ref:`Independent(x)<Independent-name>`
creates a new :ref:`glossary@Tape@Active` tape.
All of the operations with the corresponding variables
must be preformed by the same OpenMP thread.
This includes the corresponding call to
:ref:`f.Dependent(x,y)<Dependent-name>` or the
:ref:`ADFun f(x, y)<fun_construct@Sequence Constructor>`
during which the tape stops recording and the variables
become parameters.

Restriction
***********
No tapes can be
:ref:`glossary@Tape@Active` when this function is called.

{xrst_end omp_max_thread}
-----------------------------------------------------------------------------
*/

// BEGIN CppAD namespace
namespace CppAD {

template <class Base>
void AD<Base>::omp_max_thread(size_t number)
{
# ifdef _OPENMP
   thread_alloc::parallel_setup(
      number, omp_alloc::in_parallel, omp_alloc::get_thread_num
   );
# else
   CPPAD_ASSERT_KNOWN(
      number == 1,
      "omp_max_thread: number > 1 and _OPENMP is not defined"
   );
# endif
   parallel_ad<Base>();
}

} // END CppAD namespace

# endif
