# ifndef CPPAD_UTILITY_OMP_ALLOC_HPP
# define CPPAD_UTILITY_OMP_ALLOC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/utility/thread_alloc.hpp>
# ifdef _OPENMP
# include <omp.h>
# endif

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
class omp_alloc{
// ============================================================================
public:
/*
{xrst_begin omp_max_num_threads app}
Set and Get Maximum Number of Threads for omp_alloc Allocator
#############################################################

Deprecated 2011-08-31
*********************
Use the functions :ref:`thread_alloc::parallel_setup<ta_parallel_setup-name>`
and :ref:`thread_alloc:num_threads<ta_num_threads-name>` instead.

Syntax
******
| # ``include <cppad/utility/omp_alloc.hpp>``
| ``omp_alloc::set_max_num_threads`` ( *number* )
| *number* = ``omp_alloc::get_max_num_threads`` ()

Purpose
*******
By default there is only one thread and all execution is in sequential mode
(not :ref:`parallel<omp_in_parallel-name>` ).

number
******
The argument and return value *number* has prototype

   ``size_t`` *number*

and must be greater than zero.

set_max_num_threads
*******************
Informs :ref:`omp_alloc-name` of the maximum number of OpenMP threads.

get_max_num_threads
*******************
Returns the valued used in the previous call to ``set_max_num_threads`` .
If there was no such previous call, the value one is returned
(and only thread number zero can use :ref:`omp_alloc-name` ).

Restrictions
************
The function ``set_max_num_threads`` must be called before
the program enters :ref:`parallel<omp_in_parallel-name>` execution mode.
In addition, this function cannot be called while in parallel mode.

{xrst_end omp_max_num_threads}
*/
   /*!
   Inform omp_alloc of the maximum number of OpenMP threads and enable
   parallel execution mode by initializing all statics in this file.

   \param number [in]
   maximum number of OpenMP threads.
   */
   static void set_max_num_threads(size_t number)
   {  thread_alloc::parallel_setup(
         number, omp_alloc::in_parallel, omp_alloc::get_thread_num
      );
      thread_alloc::hold_memory(number > 1);
   }
   /*!
   Get the current maximum number of OpenMP threads that omp_alloc can use.

   \return
   maximum number of OpenMP threads.
   */
   static size_t get_max_num_threads(void)
   {  return thread_alloc::num_threads(); }

/* -----------------------------------------------------------------------
{xrst_begin omp_in_parallel app}

Is The Current Execution in OpenMP Parallel Mode
################################################

Deprecated 2011-08-31
*********************
Use the function :ref:`thread_alloc::in_parallel<ta_in_parallel-name>` instead.

Syntax
******

   # ``include <cppad/utility/omp_alloc.hpp>``

*flag* = ``omp_alloc::in_parallel`` ()

Purpose
*******
Some of the :ref:`omp_alloc-name` allocation routines have different
specifications for parallel (not sequential) execution mode.
This routine enables you to determine if the current execution mode
is sequential or parallel.

flag
****
The return value has prototype

   ``bool`` *flag*

It is true if the current execution is in parallel mode
(possibly multi-threaded) and false otherwise (sequential mode).

{xrst_end omp_in_parallel}
*/
   /// Are we in a parallel execution state; i.e., is it possible that
   /// other threads are currently executing.
   static bool in_parallel(void)
   {
# ifdef _OPENMP
      return omp_in_parallel() != 0;
# else
      return false;
# endif
   }

/* -----------------------------------------------------------------------
{xrst_begin omp_get_thread_num app}

Get the Current OpenMP Thread Number
####################################

Deprecated 2011-08-31
*********************
Use the function :ref:`thread_alloc::thread_num<ta_thread_num-name>` instead.

Syntax
******

   # ``include <cppad/utility/omp_alloc.hpp>``

*thread* = ``omp_alloc::get_thread_num`` ()

Purpose
*******
Some of the :ref:`omp_alloc-name` allocation routines have a thread number.
This routine enables you to determine the current thread.

thread
******
The return value *thread* has prototype

   ``size_t`` *thread*

and is the currently executing thread number.
If ``_OPENMP`` is not defined, *thread* is zero.

{xrst_end omp_get_thread_num}
*/
   /// Get current OpenMP thread number (zero if _OpenMP not defined).
   static size_t get_thread_num(void)
   {
# ifdef _OPENMP
      size_t thread = static_cast<size_t>( omp_get_thread_num() );
      return thread;
# else
      return 0;
# endif
   }
/* -----------------------------------------------------------------------
{xrst_begin omp_get_memory app}

Get At Least A Specified Amount of Memory
#########################################

Deprecated 2011-08-31
*********************
Use the function :ref:`thread_alloc::get_memory<ta_get_memory-name>` instead.

Syntax
******

   # ``include <cppad/utility/omp_alloc.hpp>``

*v_ptr* = ``omp_alloc::get_memory`` ( *min_bytes* , *cap_bytes* )

Purpose
*******
Use :ref:`omp_alloc-name` to obtain a minimum number of bytes of memory
(for use by the :ref:`current thread<omp_get_thread_num-name>` ).

min_bytes
*********
This argument has prototype

   ``size_t`` *min_bytes*

It specifies the minimum number of bytes to allocate.

cap_bytes
*********
This argument has prototype

   ``size_t&`` *cap_bytes*

It's input value does not matter.
Upon return, it is the actual number of bytes (capacity)
that have been allocated for use,

   *min_bytes* <= *cap_bytes*

v_ptr
*****
The return value *v_ptr* has prototype

   ``void`` * *v_ptr*

It is the location where the *cap_bytes* of memory
that have been allocated for use begins.

Allocation Speed
****************
This allocation should be faster if the following conditions hold:

#. The memory allocated by a previous call to ``get_memory``
   is currently available for use.
#. The current *min_bytes* is between
   the previous *min_bytes* and previous *cap_bytes* .

{xrst_end omp_get_memory}
*/
   /*!
   Use omp_alloc to get a specified amount of memory.

   If the memory allocated by a previous call to get_memory is now
   avaialable, and min_bytes is between its previous value
   and the previous cap_bytes, this memory allocation will have
   optimal speed. Otherwise, the memory allocation is more complicated and
   may have to wait for other threads to complete an allocation.

   \param min_bytes [in]
   The minimum number of bytes of memory to be obtained for use.

   \param cap_bytes [out]
   The actual number of bytes of memory obtained for use.

   \return
   pointer to the beginning of the memory allocted for use.
   */
   static void* get_memory(size_t min_bytes, size_t& cap_bytes)
   {  return thread_alloc::get_memory(min_bytes, cap_bytes); }

/* -----------------------------------------------------------------------
{xrst_begin omp_return_memory app}

Return Memory to omp_alloc
##########################

Deprecated 2011-08-31
*********************
Use the function :ref:`thread_alloc::return_memory<ta_return_memory-name>` instead.

Syntax
******

   # ``include <cppad/utility/omp_alloc.hpp>``

``omp_alloc::return_memory`` ( *v_ptr* )

Purpose
*******
If :ref:`omp_max_num_threads-name` is one,
the memory is returned to the system.
Otherwise, the memory is retained by :ref:`omp_alloc-name` for quick future use
by the thread that allocated to memory.

v_ptr
*****
This argument has prototype

   ``void`` * *v_ptr*

.
It must be a pointer to memory that is currently in use; i.e.
obtained by a previous call to :ref:`omp_get_memory-name` and not yet returned.

Thread
******
Either the :ref:`current thread<omp_get_thread_num-name>` must be the same as during
the corresponding call to :ref:`omp_get_memory-name` ,
or the current execution mode must be sequential
(not :ref:`parallel<omp_in_parallel-name>` ).

NDEBUG
******
If ``NDEBUG`` is defined, *v_ptr* is not checked (this is faster).
Otherwise, a list of in use pointers is searched to make sure
that *v_ptr* is in the list.

{xrst_end omp_return_memory}
*/
   /*!
   Return memory that was obtained by get_memory.
   If  <code>max_num_threads(0) == 1</code>,
   the memory is returned to the system.
   Otherwise, it is retained by omp_alloc and available for use by
   get_memory for this thread.

   \param v_ptr [in]
   Value of the pointer returned by get_memory and still in use.
   After this call, this pointer will available (and not in use).

   \par
   We must either be in sequential (not parallel) execution mode,
   or the current thread must be the same as for the corresponding call
   to get_memory.
   */
   static void return_memory(void* v_ptr)
   {  thread_alloc::return_memory(v_ptr); }
/* -----------------------------------------------------------------------
{xrst_begin omp_free_available app}

Free Memory Currently Available for Quick Use by a Thread
#########################################################

Deprecated 2011-08-31
*********************
Use the function :ref:`thread_alloc::free_available<ta_free_available-name>`
instead.

Syntax
******

   # ``include <cppad/utility/omp_alloc.hpp>``

``omp_alloc::free_available`` ( *thread* )

Purpose
*******
Free memory, currently available for quick use by a specific thread,
for general future use.

thread
******
This argument has prototype

   ``size_t`` *thread*

Either :ref:`omp_get_thread_num-name` must be the same as *thread* ,
or the current execution mode must be sequential
(not :ref:`parallel<omp_in_parallel-name>` ).

{xrst_end omp_free_available}
*/
   /*!
   Return all the memory being held as available for a thread to the system.

   \param thread [in]
   this thread that will no longer have any available memory after this call.
   This must either be the thread currently executing, or we must be
   in sequential (not parallel) execution mode.
   */
   static void free_available(size_t thread)
   {  thread_alloc::free_available(thread); }
/* -----------------------------------------------------------------------
{xrst_begin omp_inuse app}

Amount of Memory a Thread is Currently Using
############################################

Deprecated 2011-08-31
*********************

Syntax
******

   # ``include <cppad/utility/omp_alloc.hpp>``

*num_bytes* = ``omp_alloc::inuse`` ( *thread* )
Use the function :ref:`thread_alloc::inuse<ta_inuse-name>` instead.

Purpose
*******
Memory being managed by :ref:`omp_alloc-name` has two states,
currently in use by the specified thread,
and quickly available for future use by the specified thread.
This function informs the program how much memory is in use.

thread
******
This argument has prototype

   ``size_t`` *thread*

Either :ref:`omp_get_thread_num-name` must be the same as *thread* ,
or the current execution mode must be sequential
(not :ref:`parallel<omp_in_parallel-name>` ).

num_bytes
*********
The return value has prototype

   ``size_t`` *num_bytes*

It is the number of bytes currently in use by the specified thread.

{xrst_end omp_inuse}
*/
   /*!
   Determine the amount of memory that is currently inuse.

   \param thread [in]
   Thread for which we are determining the amount of memory
   (must be < CPPAD_MAX_NUM_THREADS).
   Durring parallel execution, this must be the thread
   that is currently executing.

   \return
   The amount of memory in bytes.
   */
   static size_t inuse(size_t thread)
   {  return thread_alloc::inuse(thread); }
/* -----------------------------------------------------------------------
{xrst_begin omp_available app}

Amount of Memory Available for Quick Use by a Thread
####################################################

Deprecated 2011-08-31
*********************
Use the function :ref:`thread_alloc::available<ta_available-name>` instead.

Syntax
******

   # ``include <cppad/utility/omp_alloc.hpp>``

*num_bytes* = ``omp_alloc::available`` ( *thread* )

Purpose
*******
Memory being managed by :ref:`omp_alloc-name` has two states,
currently in use by the specified thread,
and quickly available for future use by the specified thread.
This function informs the program how much memory is available.

thread
******
This argument has prototype

   ``size_t`` *thread*

Either :ref:`omp_get_thread_num-name` must be the same as *thread* ,
or the current execution mode must be sequential
(not :ref:`parallel<omp_in_parallel-name>` ).

num_bytes
*********
The return value has prototype

   ``size_t`` *num_bytes*

It is the number of bytes currently available for use by the specified thread.

{xrst_end omp_available}
*/
   /*!
   Determine the amount of memory that is currently available for use.

   \copydetails inuse
   */
   static size_t available(size_t thread)
   {  return thread_alloc::available(thread); }
/* -----------------------------------------------------------------------
{xrst_begin omp_create_array app}

Allocate Memory and Create A Raw Array
######################################

Deprecated 2011-08-31
*********************
Use the function :ref:`thread_alloc::create_array<ta_create_array-name>` instead.

Syntax
******

   # ``include <cppad/utility/omp_alloc.hpp>``

*array* = ``omp_alloc::create_array<`` *Type* >( *size_min* , *size_out* ) .

Purpose
*******
Create a new raw array using :ref:`omp_alloc-name` a fast memory allocator
that works well in a multi-threading OpenMP environment.

Type
****
The type of the elements of the array.

size_min
********
This argument has prototype

   ``size_t`` *size_min*

This is the minimum number of elements that there can be
in the resulting *array* .

size_out
********
This argument has prototype

   ``size_t&`` *size_out*

The input value of this argument does not matter.
Upon return, it is the actual number of elements
in *array*
( *size_min* <= *size_out* ).

array
*****
The return value *array* has prototype

   *Type* * *array*

It is array with *size_out* elements.
The default constructor for *Type* is used to initialize the
elements of *array* .
Note that :ref:`omp_delete_array-name`
should be used to destroy the array when it is no longer needed.

Delta
*****
The amount of memory :ref:`omp_inuse-name` by the current thread,
will increase *delta* where

   ``sizeof`` ( *Type* ) * ( *size_out* + 1) > *delta* >= ``sizeof`` ( *Type* ) * *size_out*

The :ref:`omp_available-name` memory will decrease by *delta* ,
(and the allocation will be faster)
if a previous allocation with *size_min* between its current value
and *size_out* is available.

{xrst_end omp_create_array}
*/
   /*!
   Use omp_alloc to Create a Raw Array.

   \tparam Type
   The type of the elements of the array.

   \param size_min [in]
   The minimum number of elements in the array.

   \param size_out [out]
   The actual number of elements in the array.

   \return
   pointer to the first element of the array.
   The default constructor is used to initialize
   all the elements of the array.

   \par
   The extra_ field, in the omp_alloc node before the return value,
   is set to size_out.
   */
   template <class Type>
   static Type* create_array(size_t size_min, size_t& size_out)
   {  return thread_alloc::create_array<Type>(size_min, size_out); }
/* -----------------------------------------------------------------------
{xrst_begin omp_delete_array app}

Return A Raw Array to The Available Memory for a Thread
#######################################################

Deprecated 2011-08-31
*********************
Use the function :ref:`thread_alloc::delete_array<ta_delete_array-name>` instead.

Syntax
******

   # ``include <cppad/utility/omp_alloc.hpp>``

``omp_alloc::delete_array`` ( *array* ) .

Purpose
*******
Returns memory corresponding to a raw array
(create by :ref:`omp_create_array-name` ) to the
:ref:`omp_available-name` memory pool for the current thread.

Type
****
The type of the elements of the array.

array
*****
The argument *array* has prototype

   *Type* * *array*

It is a value returned by :ref:`omp_create_array-name` and not yet deleted.
The *Type* destructor is called for each element in the array.

Thread
******
The :ref:`current thread<omp_get_thread_num-name>` must be the
same as when :ref:`omp_create_array-name` returned the value *array* .
There is an exception to this rule:
when the current execution mode is sequential
(not :ref:`parallel<omp_in_parallel-name>` ) the current thread number does not matter.

Delta
*****
The amount of memory :ref:`omp_inuse-name` will decrease by *delta* ,
and the :ref:`omp_available-name` memory will increase by *delta* ,
where :ref:`omp_create_array@Delta`
is the same as for the corresponding call to ``create_array`` .

{xrst_end omp_delete_array}
*/
   /*!
   Return Memory Used for a Raw Array to the Available Pool.

   \tparam Type
   The type of the elements of the array.

   \param array [in]
   A value returned by create_array that has not yet been deleted.
   The Type destructor is used to destroy each of the elements
   of the array.

   \par
   Durring parallel execution, the current thread must be the same
   as during the corresponding call to create_array.
   */
   template <class Type>
   static void delete_array(Type* array)
   {  thread_alloc::delete_array(array); }
};
/* --------------------------------------------------------------------------
{xrst_begin omp_efficient app}

Check If A Memory Allocation is Efficient for Another Use
#########################################################

Removed
*******
This function has been removed because speed tests seem to indicate
it is just as fast, or faster, to free and then reallocate the memory.

Syntax
******

   # ``include <cppad/utility/omp_alloc.hpp>``

*flag* = ``omp_alloc::efficient`` ( *v_ptr* , *num_bytes* )

Purpose
*******
Check if memory that is currently in use is an efficient
allocation for a specified number of bytes.

v_ptr
*****
This argument has prototype

   ``const void`` * *v_ptr*

.
It must be a pointer to memory that is currently in use; i.e.
obtained by a previous call to :ref:`omp_get_memory-name` and not yet returned.

num_bytes
*********
This argument has prototype

   ``size_t`` *num_bytes*

It specifies the number of bytes of the memory allocated by *v_ptr*
that we want to use.

flag
****
The return value has prototype

   ``bool`` *flag*

It is true,
a call to ``get_memory`` with
:ref:`omp_get_memory@min_bytes`
equal to *num_bytes* would result in a value for
:ref:`omp_get_memory@cap_bytes` that is the same as when ``v_ptr``
was returned by ``get_memory`` ; i.e.,
*v_ptr* is an efficient memory block for *num_bytes*
bytes of information.

Thread
******
Either the :ref:`current thread<omp_get_thread_num-name>` must be the same as during
the corresponding call to :ref:`omp_get_memory-name` ,
or the current execution mode must be sequential
(not :ref:`parallel<omp_in_parallel-name>` ).

NDEBUG
******
If ``NDEBUG`` is defined, *v_ptr* is not checked (this is faster).
Otherwise, a list of in use pointers is searched to make sure
that *v_ptr* is in the list.

{xrst_end omp_efficient}
---------------------------------------------------------------------------
{xrst_begin old_max_num_threads app}
Set Maximum Number of Threads for omp_alloc Allocator
#####################################################

Removed
*******
This function has been removed from the CppAD API.
Use the function :ref:`thread_alloc::parallel_setup<ta_parallel_setup-name>`
in its place.

Syntax
******

   # ``include <cppad/utility/omp_alloc.hpp>``

``omp_alloc::max_num_threads`` ( *number* )

Purpose
*******
By default there is only one thread and all execution is in sequential mode
(not :ref:`parallel<omp_in_parallel-name>` ).

number
******
The argument *number* has prototype

   ``size_t`` *number*

It must be greater than zero and specifies the maximum number of
OpenMP threads that will be active at one time.

Restrictions
************
This function must be called before the program enters
:ref:`parallel<omp_in_parallel-name>` execution mode.

{xrst_end old_max_num_threads}
-------------------------------------------------------------------------------
*/
} // END_CPPAD_NAMESPACE

# endif
