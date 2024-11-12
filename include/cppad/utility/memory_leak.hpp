# ifndef CPPAD_UTILITY_MEMORY_LEAK_HPP
# define CPPAD_UTILITY_MEMORY_LEAK_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin memory_leak app}

Memory Leak Detection
#####################

Deprecated 2012-04-06
*********************
This routine has been deprecated.
You should instead use the routine :ref:`ta_free_all-name` .

Syntax
******
| # ``include <cppad/utility/memory_leak.hpp>``
| ``flag`` = ``memory_leak`` ()
| *flag* = *memory_leak* ( *add_static* )

Purpose
*******
This routine checks that the are no memory leaks
caused by improper use of :ref:`thread_alloc-name` memory allocator.
The deprecated memory allocator :ref:`track_new_del-name` is also checked.
Memory errors in the deprecated :ref:`omp_alloc-name` allocator are
reported as being in ``thread_alloc`` .

thread
******
It is assumed that :ref:`in_parallel()<ta_in_parallel-name>` is false
and :ref:`thread_num<ta_thread_num-name>` is zero when
``memory_leak`` is called.

add_static
**********
This argument has prototype

   ``size_t`` *add_static*

and its default value is zero.
Static variables hold onto memory forever.
If the argument *add_static* is present (and non-zero),
``memory_leak`` adds this amount of memory to the
:ref:`inuse<ta_inuse-name>` sum that corresponds to
static variables in the program.
A call with *add_static* should be make after
a routine that has static variables which
use :ref:`get_memory<ta_get_memory-name>` to allocate memory.
The value of *add_static* should be the difference of

   ``thread_alloc::inuse`` (0)

before and after the call.
Since multiple statics may be allocated in different places in the program,
it is expected that there will be multiple calls
that use this option.

flag
****
The return value *flag* has prototype

   ``bool`` *flag*

If *add_static* is non-zero,
the return value for ``memory_leak`` is false.
Otherwise, the return value for ``memory_leak`` should be false
(indicating that the only allocated memory corresponds to static variables).

inuse
*****
It is assumed that, when ``memory_leak`` is called,
there should not be any memory
:ref:`inuse<ta_inuse-name>` or :ref:`omp_inuse-name` for any thread
(except for inuse memory corresponding to static variables).
If there is, a message is printed and ``memory_leak`` returns false.

available
*********
It is assumed that, when ``memory_leak`` is called,
there should not be any memory
:ref:`available<ta_available-name>` or :ref:`omp_available-name` for any thread;
i.e., it all has been returned to the system.
If there is memory still available for any thread,
``memory_leak`` returns false.

TRACK_COUNT
***********
It is assumed that, when ``memory_leak`` is called,
:ref:`track_new_del@TrackCount` will return a zero value.
If it returns a non-zero value,
``memory_leak`` returns false.

Error Message
*************
If this is the first call to ``memory_leak`` , no message is printed.
Otherwise, if it returns true, an error message is printed
to standard output describing the memory leak that was detected.

{xrst_end memory_leak}
*/
# include <iostream>
# include <cppad/local/define.hpp>
# include <cppad/utility/omp_alloc.hpp>
# include <cppad/utility/thread_alloc.hpp>
# include <cppad/utility/track_new_del.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file memory_leak.hpp
File that implements a memory check at end of a CppAD program
*/

/*!
Function that checks
allocator thread_alloc for misuse that results in memory leaks.
Deprecated routines in track_new_del.hpp and omp_alloc.hpp are also checked.

\param add_static [in]
The amount specified by add_static is added to the amount
of memory that is expected to be used by thread zero for static variables.

\return
If add_static is non-zero, the return value is false.
Otherwise, if one of the following errors is detected,
the return value is true:

\li
Thread zero does not have the expected amount of inuse memory
(for static variables).
\li
A thread, other than thread zero, has any inuse memory.
\li
Any thread has available memory.

\par
If an error is detected, diagnostic information is printed to standard
output.
*/
inline bool memory_leak(size_t add_static = 0)
{  // CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL not necessary given asserts below
   static size_t thread_zero_static_inuse     = 0;
   using std::cout;
   using std::endl;
   using CppAD::thread_alloc;
   using CppAD::omp_alloc;
   // --------------------------------------------------------------------
   CPPAD_ASSERT_KNOWN(
      ! thread_alloc::in_parallel(),
      "memory_leak: in_parallel() is true."
   );
   CPPAD_ASSERT_KNOWN(
      thread_alloc::thread_num() == 0,
      "memory_leak: thread_num() is not zero."
   );
   if( add_static != 0 )
   {  thread_zero_static_inuse += add_static;
      return false;
   }
   bool leak                 = false;
   size_t thread             = 0;

   // check that memory in use for thread zero corresponds to statics
   size_t num_bytes = thread_alloc::inuse(thread);
   if( num_bytes != thread_zero_static_inuse )
   {  leak = true;
      cout << "thread zero: static inuse = " << thread_zero_static_inuse;
      cout << ", current inuse(0)= " << num_bytes << endl;
   }
   // check that no memory is currently available for this thread
   num_bytes = thread_alloc::available(thread);
   if( num_bytes != 0 )
   {  leak = true;
      cout << "thread zero: available    = ";
      cout << num_bytes << endl;
   }
   for(thread = 1; thread < CPPAD_MAX_NUM_THREADS; thread++)
   {
      // check that no memory is currently in use for this thread
      num_bytes = thread_alloc::inuse(thread);
      if( num_bytes != 0 )
      {  leak = true;
         cout << "thread " << thread << ": inuse(thread) = ";
         cout << num_bytes << endl;
      }
      // check that no memory is currently available for this thread
      num_bytes = thread_alloc::available(thread);
      if( num_bytes != 0 )
      {  leak = true;
         cout << "thread " << thread << ": available(thread) = ";
         cout << num_bytes << endl;
      }
   }
   // ----------------------------------------------------------------------
   // check track_new_del
   if( CPPAD_TRACK_COUNT() != 0 )
   {  leak = true;
      CppAD::TrackElement::Print();
   }
   return leak;
}

} // END_CPPAD_NAMESPACE
# endif
