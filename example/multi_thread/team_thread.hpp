# ifndef CPPAD_EXAMPLE_MULTI_THREAD_TEAM_THREAD_HPP
# define CPPAD_EXAMPLE_MULTI_THREAD_TEAM_THREAD_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin team_thread.hpp}
{xrst_spell
   bthread
   initializes
   pthreads
   statically
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

Purpose
*******
These routines start, use, and stop a team of threads that can
be used with the CppAD type ``AD<double>`` .
For example,
these could be OpenMP threads, pthreads, or Boost threads to name a few.

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
The argument
*num_threads*  > 0 has type ``size_t``
and specifies the number of threads in this team.
This initializes both ``AD<double>`` and ``team_work``
to be used with *num_threads* .
If *num_threads*  > 1 ,
*num_threads* ``- 1`` new threads are created
and put in a waiting state until ``team_work`` is called.

team_work
*********
This routine may be called one or more times
between the call to ``team_create`` and ``team_destroy`` .
The argument *worker* has type
``bool`` *worker* ( ``void`` ) .
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
This routines returns a name that identifies this thread_team.
The return value has prototype

   ``const char`` * *name*

and is a statically allocated ``'\0'`` terminated C string.

ok
**
The return value *ok* has type ``bool`` .
It is ``false`` if an error is detected during the
corresponding call.
Otherwise it is ``true`` .
{xrst_toc_hidden
   example/multi_thread/openmp/team_openmp.cpp
   example/multi_thread/bthread/team_bthread.cpp
   example/multi_thread/pthread/team_pthread.cpp
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

Speed Test of Implementation
****************************
Speed tests of using CppAD with the team implementations above
can be found in:

.. csv-table::
   :widths: auto

   harmonic.cpp,:ref:`harmonic.cpp-title`
   multi_newton.cpp,:ref:`multi_newton.cpp-title`

Source
******
{xrst_spell_off}
{xrst_code cpp} */
# include <cstddef> // for size_t

extern bool team_create(size_t num_threads);
extern bool team_work(void worker(void));
extern bool team_destroy(void);
extern const char* team_name(void);
/* {xrst_code}
{xrst_spell_on}

{xrst_end team_thread.hpp}
*/

# endif
