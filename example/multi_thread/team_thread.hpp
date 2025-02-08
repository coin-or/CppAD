# ifndef CPPAD_EXAMPLE_MULTI_THREAD_TEAM_THREAD_HPP
# define CPPAD_EXAMPLE_MULTI_THREAD_TEAM_THREAD_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin team_thread.hpp}
{xrst_spell
   bthread
   openmp
   posix
   pthread
   sthread
}
Specifications for A Team of AD Threads
#######################################

Syntax
******
| ``include`` ``"team_thread.hpp"``
| *ok* = ``team_create`` ( *num_threads* )
| *ok* = ``team_work`` ( *worker* )
| *ok* = ``team_destroy`` ()
| *name* = ``team_name`` ()

Prototype
*********
{xrst_literal
   // BEGIN PROTOTYPE
   // END PROTOTYPE
}

Purpose
*******
These routines start, use, and stop a team of threads that can
be used with the CppAD type ``AD<double>`` .
Examples are provided for
OpenMP threads, Posix threads, Boost threads and Standard threads.

Restrictions
************
Calls to the routines
``team_create`` ,
``team_work`` , and
``team_destroy`` , must all be done by the master thread; i.e.,
:ref:`thread_num<ta_thread_num-name>` must be zero.
In addition, they must all be done in sequential execution mode; i.e.,
when the master thread is the only thread that is running
(:ref:`in_parallel<ta_in_parallel-name>` must be false).

team_create
***********
The argument *num_threads*  > 0
specifies the number of threads in this team.
This initializes both ``AD<double>`` and ``team_work``
to be used with *num_threads* .
If *num_threads*  > 1 ,
*num_threads* ``- 1`` new threads are created
and put in a waiting state until ``team_work`` is called.

team_work
*********
This routine may be called one or more times
between the call to ``team_create`` and ``team_destroy`` .
Each call to ``team_work`` runs *num_threads* versions
of *worker* with the corresponding value of
:ref:`thread_num<ta_thread_num-name>`
between zero and *num_threads* ``- 1`` and
different for each thread,

team_destroy
************
This routine terminates all the other threads except for
thread number zero; i.e., it terminates the threads corresponding to

   *thread_num* = 1 , ... , *num_threads* ``-1``

team_name
*********
This routines returns a name that identifies this threading system.
The return value is a statically allocated ``'\0'`` terminated C string.

ok
**
The return value *ok*
is ``false`` if an error is detected during the
corresponding call.
Otherwise it is ``true`` .
{xrst_toc_hidden
   example/multi_thread/openmp/team_openmp.cpp
   example/multi_thread/bthread/team_bthread.cpp
   example/multi_thread/pthread/team_pthread.cpp
   example/multi_thread/sthread/team_sthread.cpp
}

Example Use
***********
Example use of these specifications can be found in the file
:ref:`team_example.cpp-name` .

Example Implementation
**********************
Example implementations of these specifications can be found in the files:

.. csv-table::
   :widths: auto

   team_openmp.cpp,:ref:`team_openmp.cpp-title`
   team_bthread.cpp,:ref:`team_bthread.cpp-title`
   team_pthread.cpp,:ref:`team_pthread.cpp-title`
   team_sthread.cpp,:ref:`team_sthread.cpp-title`

Speed Test of Implementation
****************************
Speed tests of using CppAD with the team implementations above
can be found in:

.. csv-table::
   :widths: auto

   harmonic.cpp,:ref:`harmonic.cpp-title`
   multi_newton.cpp,:ref:`multi_newton.cpp-title`

{xrst_end team_thread.hpp}
*/
# include <cstddef> // for size_t

// BEGIN PROTOTYPE
extern bool team_create(size_t num_threads);
extern bool team_work(void worker(void));
extern bool team_destroy(void);
extern const char* team_name(void);
// END PROTOTYPE

# endif
