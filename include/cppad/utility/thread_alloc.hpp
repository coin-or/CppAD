# ifndef CPPAD_UTILITY_THREAD_ALLOC_HPP
# define CPPAD_UTILITY_THREAD_ALLOC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <sstream>
# include <limits>
# include <memory>
# include <cstdint>


# ifdef _MSC_VER
// Supress warning that Microsoft compiler changed its behavior and is now
// doing the correct thing at the statement:
//            new(array + i) Type();
# pragma warning(disable:4345)
# endif

# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/define.hpp>
# include <cppad/local/set_get_in_parallel.hpp>
namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file thread_alloc.hpp
File used to define the CppAD multi-threading allocator class
*/

/*!
\def CPPAD_MAX_NUM_CAPACITY
Maximum number of different capacities the allocator will attempt.
This must be larger than the log base two of numeric_limit<size_t>::max().
*/
# define CPPAD_MAX_NUM_CAPACITY 100

/*!
\def CPPAD_MIN_DOUBLE_CAPACITY
Minimum number of double values that will fit in an allocation.
*/
# define CPPAD_MIN_DOUBLE_CAPACITY 16

/*!
\def CPPAD_TRACE_CAPACITY
If NDEBUG is not defined, print all calls to get_memory and return_memory
that correspond to this capacity and thread CPPAD_TRACE_THREAD.
(Note that if CPPAD_TRACE_CAPACITY is zero, or any other value not in the list
of capacities, no tracing will be done.)
*/
# define CPPAD_TRACE_CAPACITY 0

/*!
\def CPPAD_TRACE_THREAD
If NDEBUG is not defined, print all calls to get_memory and return_memory
that correspond to this thead and capacity CPPAD_TRACE_CAPACITY.
*/
# define CPPAD_TRACE_THREAD 0

/*
Note that Section 3.6.2 of ISO/IEC 14882:1998(E) states: "The storage for
objects with static storage duration (3.7.1) shall be zero-initialized
(8.5) before any other initialization takes place."
*/

/*!
Capacity vector for memory allocation block sizes.

Only one of these objects should be created and used as a
static variable inside of the thread_alloc::capacity_info function.
*/

/*!
Allocator class that works well with an multi-threading environment.
*/
class thread_alloc{
// ============================================================================
private:

   class capacity_t {
   public:
      /// number of capacity values actually used
      size_t number;
      /// the different capacity values
      size_t value[CPPAD_MAX_NUM_CAPACITY];
      /// ctor
      capacity_t(void)
      {  // Cannot figure out how to call thread_alloc::in_parallel here.
         // CPPAD_ASSERT_UNKNOWN(
         //    ! thread_alloc::in_parallel() , "thread_alloc: "
         //    "parallel mode and parallel_setup not yet called."
         // );
         number           = 0;
         size_t capacity  = CPPAD_MIN_DOUBLE_CAPACITY * sizeof(double);
         while( capacity < std::numeric_limits<size_t>::max() / 2 )
         {  CPPAD_ASSERT_UNKNOWN( number < CPPAD_MAX_NUM_CAPACITY );
            value[number++] = capacity;
            // next capactiy is 3/2 times the current one
            capacity        = 3 * ( (capacity + 1) / 2 );
         }
         CPPAD_ASSERT_UNKNOWN( number > 0 );
      }
   };

   class block_t {
   public:
      /// extra information (currently used by create and delete array)
      size_t             extra_;
      /// an index that uniquely idenfifies both thread and capacity
      size_t             tc_index_;
      /// pointer to the next memory allocation with the same tc_index_
      void*              next_;
      ///
      /// Calculated by include/cppad/CMakeLists.txt
      CPPAD_PADDING_BLOCK_T
      // -----------------------------------------------------------------
      /// make default constructor private. It is only used by constructor
      /// for `root arrays below.
      block_t(void) : extra_(0), tc_index_(0), next_(nullptr)
      { }
   };

   // ---------------------------------------------------------------------
   /// Vector of fixed capacity values for this allocator
   static const capacity_t* capacity_info(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static const capacity_t capacity;
      return &capacity;
   }
   // ---------------------------------------------------------------------
   /// Structure of information for each thread
   struct thread_alloc_info {
      /// count of available bytes for this thread
      size_t  count_inuse_;
      /// count of inuse bytes for this thread
      size_t  count_available_;
      /// root of available list for this thread and each capacity
      block_t root_available_[CPPAD_MAX_NUM_CAPACITY];
      /*!
      root of inuse list for this thread and each capacity
      If NDEBUG is defined or CPPAD_DEBUG_AND_RELEASE is true,
      this memory is not used, but it still helps to separate
      this structure from the structure for the next thread.
      */
      block_t root_inuse_[CPPAD_MAX_NUM_CAPACITY];
   };
   // ---------------------------------------------------------------------
   /*!
   Set and Get hold available memory flag.

   \param set [in]
   if true, the value returned by this return is changed.

   \param new_value [in]
   if set is true, this is the new value returned by this routine.
   Otherwise, new_value is ignored.

   \return
   the current setting for this routine (which is initially false).
   */
   static bool set_get_hold_memory(bool set, bool new_value = false)
   {  static bool value = false;
      if( set )
         value = new_value;
      return value;
   }
   // ---------------------------------------------------------------------
   /*!
   Get pointer to the information for this thread.

   \param thread [in]
   Is the thread number for this information pointer.

   \param clear
   If clear is true, then the information pointer for this thread
   is deleted and the nullptr pointer is returned.
   There must be no memory currently in either the inuse or avaialble
   lists when this routine is called.

   \return
   is the current information pointer for this thread.
   If clear is false, and the current pointer is nullptr,
   a new infromation record is allocated and its pointer returned.
   In this case, if info is the retured pointer,
   <code>info->count_inuse == 0</code> and
   <code>info->count_available == 0</code>.
   In addition,
   for <code>c = 0 , ... , CPPAD_MAX_NUM_CAPACITY-1</code>
   <code>info->root_inuse_[c].next_ == nullptr</code> and
   <code>info->root_available_[c].next_ == nullptr</code>.
   */
   static thread_alloc_info* thread_info(
      size_t             thread          ,
      bool               clear = false   )
   {  static thread_alloc_info* all_info[CPPAD_MAX_NUM_THREADS];
      static thread_alloc_info  zero_info;

      CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;

      CPPAD_ASSERT_UNKNOWN( thread < CPPAD_MAX_NUM_THREADS );

      thread_alloc_info* info = all_info[thread];
      if( clear )
      {  if( info != nullptr )
         {
# ifndef NDEBUG
            CPPAD_ASSERT_UNKNOWN(
               info->count_inuse_     == 0 &&
               info->count_available_ == 0
            );
            for(size_t c = 0; c < CPPAD_MAX_NUM_CAPACITY; c++)
            {  CPPAD_ASSERT_UNKNOWN(
                  info->root_inuse_[c].next_     == nullptr &&
                  info->root_available_[c].next_ == nullptr
               );
            }
# endif
            if( thread != 0 )
               ::operator delete( reinterpret_cast<void*>(info) );
            info             = nullptr;
            all_info[thread] = info;
         }
      }
      else if( info == nullptr )
      {  if( thread == 0 )
            info = &zero_info;
         else
         {  size_t size = sizeof(thread_alloc_info);
            void* v_ptr = ::operator new(size);
            info        = reinterpret_cast<thread_alloc_info*>(v_ptr);
         }
         all_info[thread] = info;

         // initialize the information record
         for(size_t c = 0; c < CPPAD_MAX_NUM_CAPACITY; c++)
         {  info->root_inuse_[c].next_       = nullptr;
            info->root_available_[c].next_   = nullptr;
         }
         info->count_inuse_     = 0;
         info->count_available_ = 0;
      }
      return info;
   }
   // -----------------------------------------------------------------------
   /*!
   Increase the number of bytes of memory that are currently in use; i.e.,
   that been obtained with get_memory and not yet returned.

   \param inc [in]
   amount to increase memory in use.

   \param thread [in]
   Thread for which we are increasing the number of bytes in use
   (must be less than num_threads).
   Durring parallel execution, this must be the thread
   that is currently executing.
   */
   static void inc_inuse(size_t inc, size_t thread)
   {
      CPPAD_ASSERT_UNKNOWN( thread < num_threads() );
      CPPAD_ASSERT_UNKNOWN(
         thread == thread_num() || (! in_parallel())
      );
      thread_alloc_info* info = thread_info(thread);

      // do the addition
      size_t result = info->count_inuse_ + inc;
      CPPAD_ASSERT_UNKNOWN( result >= info->count_inuse_ );

      info->count_inuse_ = result;
   }
   // -----------------------------------------------------------------------
   /*!
   Increase the number of bytes of memory that are currently avaialble; i.e.,
   have been obtained obtained from the system and are being held future use.

   \copydetails inc_inuse
   */
   static void inc_available(size_t inc, size_t thread)
   {
      CPPAD_ASSERT_UNKNOWN( thread < CPPAD_MAX_NUM_THREADS);
      CPPAD_ASSERT_UNKNOWN(
         thread == thread_num() || (! in_parallel())
      );
      thread_alloc_info* info = thread_info(thread);
      // do the addition
      size_t result = info->count_available_ + inc;
      CPPAD_ASSERT_UNKNOWN( result >= info->count_available_ );

      info->count_available_ = result;
   }
   // -----------------------------------------------------------------------
   /*!
   Decrease the number of bytes of memory that are currently in use; i.e.,
   that been obtained with get_memory and not yet returned.

   \param dec [in]
   amount to decrease number of bytes in use.

   \param thread [in]
   Thread for which we are decreasing the number of bytes in use
   (must be less than num_threads).
   Durring parallel execution, this must be the thread
   that is currently executing.
   */
   static void dec_inuse(size_t dec, size_t thread)
   {
      CPPAD_ASSERT_UNKNOWN(
         thread < num_threads() || (! in_parallel())
      );
      CPPAD_ASSERT_UNKNOWN(
         thread == thread_num() || (! in_parallel())
      );
      thread_alloc_info* info = thread_info(thread);

      // do the subtraction
      CPPAD_ASSERT_UNKNOWN( info->count_inuse_ >= dec );
      info->count_inuse_ = info->count_inuse_ - dec;
   }
   // -----------------------------------------------------------------------
   /*!
   Decrease the number of bytes of memory that are currently avaialble; i.e.,
   have been obtained obtained from the system and are being held future use.

   \copydetails dec_inuse
   */
   static void dec_available(size_t dec, size_t thread)
   {
      CPPAD_ASSERT_UNKNOWN( thread < CPPAD_MAX_NUM_THREADS);
      CPPAD_ASSERT_UNKNOWN(
         thread == thread_num() || (! in_parallel())
      );
      thread_alloc_info* info = thread_info(thread);
      // do the subtraction
      CPPAD_ASSERT_UNKNOWN( info->count_available_ >= dec );
      info->count_available_ =  info->count_available_ - dec;
   }

   // ----------------------------------------------------------------------
   /*!
   Set and get the number of threads that are sharing memory.

   \param number_new
   If number is zero, we are only retreiving the current maximum
   number of threads. Otherwise, we are setting and retreiving
   maximum number of threads.

   \return
   the number of threads that are sharing memory.
   If number_new is non-zero, the return value is equal to
   number_new.
   */
   static size_t set_get_num_threads(size_t number_new)
   {  static size_t number_user = 1;

      CPPAD_ASSERT_UNKNOWN( number_new <= CPPAD_MAX_NUM_THREADS );
      CPPAD_ASSERT_UNKNOWN( ! in_parallel() || (number_new == 0) );

      // case where we are changing the number of threads
      if( number_new != 0 )
         number_user = number_new;

      return number_user;
   }
   /*!
   Set and call the routine that determine the current thread number.

   \return
   returns value for the most recent setting for thread_num_new.
   If set is true,
   or the most recent setting is nullptr (its initial value),
   the return value is zero.
   Otherwise the routine corresponding to the most recent setting
   is called and its value returned by set_get_thread_num.

   \param thread_num_new [in]
   If set is false, thread_num_new it is not used.
   Otherwise, the current value of thread_num_new becomes the
   most recent setting for thread_num.

   \param set
   If set is true, then thread_num_new is becomes the most
   recent setting for this set_get_thread_num.
   */
   static size_t set_get_thread_num(
      size_t (*thread_num_new)(void)  ,
      bool set = false                )
   {  static size_t (*thread_num_user)(void) = nullptr;

      if( set )
      {  thread_num_user = thread_num_new;
         return 0;
      }

      if( thread_num_user == nullptr )
         return 0;

      size_t thread = thread_num_user();
      CPPAD_ASSERT_KNOWN(
         thread < set_get_num_threads(0) ,
         "parallel_setup: thread_num() >= num_threads"
      );
      return thread;
   }
// ============================================================================
public:
/*
{xrst_begin ta_parallel_setup}
Setup thread_alloc For Use in Multi-Threading Environment
#########################################################

Syntax
******
| ``thread_alloc::parallel_setup`` ( *num_threads* , *in_parallel* , *thread_num* )

Purpose
*******
By default there is only one thread and all execution is in sequential mode,
i.e., multiple threads are not sharing the same memory; i.e.
not in parallel mode.

Speed
*****
It should be faster, even when *num_thread* is equal to one,
for ``thread_alloc`` to hold onto memory.
This can be accomplished using the function call

   ``thread_alloc::hold_memory`` ( ``true`` )

see :ref:`hold_memory<ta_hold_memory-name>` .

num_threads
***********
This argument has prototype

   ``size_t`` *num_threads*

and must be greater than zero.
It specifies the number of threads that are sharing memory.
The case *num_threads*  == 1 is a special case that is
used to terminate a multi-threading environment.

in_parallel
***********
This function has prototype

   ``bool`` *in_parallel* ( ``void`` )

It must return ``true`` if there is more than one thread
currently executing.
Otherwise it can return false.

In the special case where *num_threads*  == 1 ,
the routine *in_parallel* is not used.

thread_num
**********
This function has prototype

   ``size_t`` *thread_num* ( ``void`` )

It must return a thread number that uniquely identifies the
currently executing thread.
Furthermore

   0 <= *thread_num* () < *num_threads*

.
In the special case where *num_threads*  == 1 ,
the routine *thread_num* is not used.

Note that this function is called by other routines so,
as soon as a new thread is executing,
one must be certain that *thread_num* () will
work for that thread.

Restrictions
************
The function ``parallel_setup`` must be called before
the program enters :ref:`parallel<ta_in_parallel-name>` execution mode.
In addition, this function cannot be called while in parallel mode.

Example
*******
The files
:ref:`openmp_get_started.cpp-name` ,
:ref:`bthread_get_started.cpp-name` , and
:ref:`pthread_get_started.cpp-name` ,
contain examples and tests that use this function.

{xrst_end ta_parallel_setup}
*/
   /*!
   Set thread_alloc up for parallel mode usage.

   \param num_threads [in]
   Is the number of thread that may be executing at the same time.

   \param in_parallel [in]
   Is the routine that determines if we are in parallel mode or not.

   \param thread_num [in]
   Is the routine that determines the current thread number
   (between zero and num_threads minus one).
   */
   static void parallel_setup(
      size_t num_threads         ,
      bool (*in_parallel)(void)  ,
      size_t (*thread_num)(void) )
   {
      // Special case where we go back to single thread mode right away
      // (previous settings may no longer be valid)
      if( num_threads == 1 )
      {  bool set = true;
         set_get_num_threads(num_threads);
         // emphasize that this routine is outside thread_alloc class
         CppAD::local::set_get_in_parallel(set, nullptr);
         set_get_thread_num(nullptr, set);
         return;
      }

      CPPAD_ASSERT_KNOWN(
         num_threads <= CPPAD_MAX_NUM_THREADS ,
         "parallel_setup: num_threads is too large"
      );
      CPPAD_ASSERT_KNOWN(
         num_threads != 0 ,
         "parallel_setup: num_threads == zero"
      );
      CPPAD_ASSERT_KNOWN(
         in_parallel != nullptr ,
         "parallel_setup: num_threads != 1 and in_parallel == nullptr"
      );
      CPPAD_ASSERT_KNOWN(
         thread_num != nullptr ,
         "parallel_setup: num_threads != 1 and thread_num == nullptr"
      );

      // Make sure that constructors for all static variables in this file
      // are called in sequential mode.
      for(size_t thread = 0; thread < num_threads; thread++)
         thread_info(thread);
      capacity_info();
      size_t cap_bytes;
      void* v_ptr = get_memory(0, cap_bytes);

      // free memory allocated by call to get_memory above
      return_memory(v_ptr);
      free_available( set_get_thread_num(nullptr) );

      // delay this so thread_num() call above is in previous mode
      // (current setings may not yet be valid)
      if( num_threads > 1 )
      {  bool set = true;
         set_get_num_threads(num_threads);
         // emphasize that this routine is outside thread_alloc class
         CppAD::local::set_get_in_parallel(set, in_parallel);
         set_get_thread_num(thread_num, set);
      }
   }
/*
{xrst_begin ta_num_threads}
Get Number of Threads
#####################

Syntax
******
*number* = ``thread_alloc::num_threads`` ()

Purpose
*******
Determine the number of threads as set during :ref:`parallel_setup<ta_parallel_setup-name>` .

number
******
The return value *number* has prototype

   ``size_t`` *number*

and is equal to the value of
:ref:`ta_parallel_setup@num_threads`
in the previous call to *parallel_setup* .
If there was no such previous call, the value one is returned.

Example
*******
The example and test :ref:`thread_alloc.cpp-name` uses this routine.

{xrst_end ta_num_threads}
*/
   /*!
   Get the current number of threads that thread_alloc can use.
   */
   static size_t num_threads(void)
   {  return set_get_num_threads(0); }
/* -----------------------------------------------------------------------
{xrst_begin ta_in_parallel}

Is The Current Execution in Parallel Mode
#########################################

Syntax
******
*flag* = ``thread_alloc::in_parallel`` ()

Purpose
*******
Some of the :ref:`thread_alloc-name` allocation routines have different
specifications for parallel (not sequential) execution mode.
This routine enables you to determine if the current execution mode
is sequential or parallel.

flag
****
The return value has prototype

   ``bool`` *flag*

It is true if the current execution is in parallel mode
(possibly multi-threaded) and false otherwise (sequential mode).

Example
*******
:ref:`thread_alloc.cpp-name`

{xrst_end ta_in_parallel}
*/
   /// Are we in a parallel execution state; i.e., is it possible that
   /// other threads are currently executing.
   static bool in_parallel(void)
   {  // emphasize that this routine is outside thread_alloc class
      return CppAD::local::set_get_in_parallel();
   }
/* -----------------------------------------------------------------------
{xrst_begin ta_thread_num}

Get the Current Thread Number
#############################

Syntax
******
*thread* = ``thread_alloc::thread_num`` ()

Purpose
*******
Some of the :ref:`thread_alloc-name` allocation routines have a thread number.
This routine enables you to determine the current thread.

thread
******
The return value *thread* has prototype

   ``size_t`` *thread*

and is the currently executing thread number.

Example
*******
:ref:`thread_alloc.cpp-name`

{xrst_end ta_thread_num}
*/
   /// Get current thread number
   static size_t thread_num(void)
   {  return set_get_thread_num(nullptr); }
/* -----------------------------------------------------------------------
{xrst_begin ta_get_memory}

Get At Least A Specified Amount of Memory
#########################################

Syntax
******
*v_ptr* = ``thread_alloc::get_memory`` ( *min_bytes* , *cap_bytes* )

Purpose
*******
Use :ref:`thread_alloc-name` to obtain a minimum number of bytes of memory
(for use by the :ref:`current thread<ta_thread_num-name>` ).

min_bytes
*********
This argument has prototype

   ``size_t`` *min_bytes*

It specifies the minimum number of bytes to allocate.
This value must be less than
::

   std::numeric_limits<size_t>::max() / 2

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

Alignment
*********
We call a memory allocation aligned if the address is a multiple
of the number of bytes in a ``size_t`` value.
If the system ``new`` allocator is aligned, then *v_ptr*
pointer is also aligned.

Example
*******
:ref:`thread_alloc.cpp-name`

{xrst_end ta_get_memory}
*/
   /*!
   Use thread_alloc to get a specified amount of memory.

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
   pointer to the beginning of the memory allocated for use.
   */
   static void* get_memory(size_t min_bytes, size_t& cap_bytes)
   {  // see first_trace below
      CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;

      // check that number of requested bytes is not to large
      CPPAD_ASSERT_KNOWN(
         min_bytes < std::numeric_limits<size_t>::max() / 2 ,
         "get_memory(min_bytes, cap_bytes): min_bytes is too large"
      );

      size_t num_cap = capacity_info()->number;
      using std::cout;
      using std::endl;

      // determine the capacity for this request
      size_t c_index   = 0;
      const size_t* capacity_vec = capacity_info()->value;
      while( capacity_vec[c_index] < min_bytes )
      {  ++c_index;
         CPPAD_ASSERT_UNKNOWN(c_index < num_cap );
      }
      cap_bytes = capacity_vec[c_index];

      // determine the thread, capacity, and info for this thread
      size_t thread            = thread_num();
      size_t tc_index          = thread * num_cap + c_index;
      thread_alloc_info* info  = thread_info(thread);

# ifndef NDEBUG
      // trace allocation
      static bool first_trace = true;
      if( cap_bytes == CPPAD_TRACE_CAPACITY &&
             thread    ==  CPPAD_TRACE_THREAD  && first_trace )
      {  cout << endl;
         cout << "thread_alloc: Trace for Thread = " << thread;
         cout << " and capacity = " << cap_bytes << endl;
         if( first_trace )
            first_trace = false;
      }

# if ! CPPAD_DEBUG_AND_RELEASE
      // Root nodes for both lists. Note these are different for different
      // threads because tc_index is different for different threads.
      block_t* inuse_root     = info->root_inuse_ + c_index;
# endif
# endif
      block_t* available_root = info->root_available_ + c_index;

      // check if we already have a node we can use
      void* v_node              = available_root->next_;
      block_t* node             = reinterpret_cast<block_t*>(v_node);
      if( node != nullptr )
      {  CPPAD_ASSERT_UNKNOWN( node->tc_index_ == tc_index );

         // remove node from available list
         available_root->next_ = node->next_;

         // return value for get_memory
         void* v_ptr = reinterpret_cast<void*>(node + 1);
# ifndef NDEBUG
# if ! CPPAD_DEBUG_AND_RELEASE
         // add node to inuse list
         node->next_           = inuse_root->next_;
         inuse_root->next_     = v_node;
# endif

         // trace allocation
         if( cap_bytes == CPPAD_TRACE_CAPACITY &&
                 thread    ==  CPPAD_TRACE_THREAD   )
         {  cout << "get_memory:    v_ptr = " << v_ptr << endl; }
# endif

         // adjust counts
         inc_inuse(cap_bytes, thread);
         dec_available(cap_bytes, thread);

# ifndef NDEBUG
         // check that pointers and doubles are aligned
         std::uintptr_t i_ptr = reinterpret_cast<std::uintptr_t>(v_ptr);
         CPPAD_ASSERT_UNKNOWN( i_ptr % sizeof(v_ptr) == 0 );
         CPPAD_ASSERT_UNKNOWN( i_ptr % sizeof(double) == 0 );
# endif
         // return pointer to memory, do not inclue block_t at begining
         return v_ptr;
      }

      // Create a new node with thread_alloc information at front.
      // This uses the system allocator, which is thread safe, but slower,
      // because the thread might wait for a lock on the allocator.
      v_node          = ::operator new(sizeof(block_t) + cap_bytes);
      CPPAD_ASSERT_UNKNOWN( v_node != nullptr );
      node            = reinterpret_cast<block_t*>(v_node);
      node->tc_index_ = tc_index;
      void* v_ptr     = reinterpret_cast<void*>(node + 1);

# ifndef NDEBUG
# if ! CPPAD_DEBUG_AND_RELEASE
      // add node to inuse list
      node->next_       = inuse_root->next_;
      inuse_root->next_ = v_node;
# endif

      // trace allocation
      if( cap_bytes == CPPAD_TRACE_CAPACITY &&
         thread    == CPPAD_TRACE_THREAD    )
      {  cout << "get_memory:    v_ptr = " << v_ptr << endl; }
# endif

      // adjust counts
      inc_inuse(cap_bytes, thread);

      return v_ptr;
   }

/* -----------------------------------------------------------------------
{xrst_begin ta_return_memory}

Return Memory to thread_alloc
#############################

Syntax
******
``thread_alloc::return_memory`` ( *v_ptr* )

Purpose
*******
If :ref:`hold_memory<ta_hold_memory-name>` is false,
the memory is returned to the system.
Otherwise, the memory is retained by :ref:`thread_alloc-name` for quick future use
by the thread that allocated to memory.

v_ptr
*****
This argument has prototype

   ``void`` * *v_ptr*

.
It must be a pointer to memory that is currently in use; i.e.
obtained by a previous call to
:ref:`get_memory<ta_get_memory-name>` and not yet returned.

Thread
******
Either the :ref:`current thread<ta_thread_num-name>` must be the same as during
the corresponding call to :ref:`get_memory<ta_get_memory-name>` ,
or the current execution mode must be sequential
(not :ref:`parallel<ta_in_parallel-name>` ).

NDEBUG
******
If ``NDEBUG`` is defined, *v_ptr* is not checked (this is faster).
Otherwise, a list of in use pointers is searched to make sure
that *v_ptr* is in the list.

Example
*******
:ref:`thread_alloc.cpp-name`

{xrst_end ta_return_memory}
*/
   /*!
   Return memory that was obtained by get_memory.
   If  <code>num_threads() == 1</code>,
   the memory is returned to the system.
   Otherwise, it is retained by thread_alloc and available for use by
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
   {  size_t num_cap   = capacity_info()->number;

      block_t* node    = reinterpret_cast<block_t*>(v_ptr) - 1;
      size_t tc_index  = node->tc_index_;
      size_t thread    = tc_index / num_cap;
      size_t c_index   = tc_index % num_cap;
      size_t capacity  = capacity_info()->value[c_index];

      CPPAD_ASSERT_UNKNOWN( thread < CPPAD_MAX_NUM_THREADS );
      CPPAD_ASSERT_KNOWN(
         thread == thread_num() || (! in_parallel()),
         "Attempt to return memory for a different thread "
         "while in parallel mode"
      );

      thread_alloc_info* info = thread_info(thread);
# ifndef NDEBUG
# if ! CPPAD_DEBUG_AND_RELEASE
      // remove node from inuse list
      void* v_node         = reinterpret_cast<void*>(node);
      block_t* inuse_root  = info->root_inuse_ + c_index;
      block_t* previous    = inuse_root;
      while( (previous->next_ != nullptr) && (previous->next_ != v_node) )
         previous = reinterpret_cast<block_t*>(previous->next_);

      // check that v_ptr is valid
      if( previous->next_ != v_node )
      {  using std::endl;
         std::ostringstream oss;
         oss << "return_memory: attempt to return memory not in use";
         oss << endl;
         oss << "v_ptr    = " << v_ptr    << endl;
         oss << "thread   = " << thread   << endl;
         oss << "capacity = " << capacity << endl;
         oss << "See CPPAD_TRACE_THREAD & CPPAD_TRACE_CAPACITY in";
         oss << endl << "# include <cppad/utility/thread_alloc.hpp>" << endl;
         // oss.str() returns a string object with a copy of the current
         // contents in the stream buffer.
         std::string msg_str       = oss.str();
         // msg_str.c_str() returns a pointer to the c-string
         // representation of the string object's value.
         const char* msg_char_star = msg_str.c_str();
         CPPAD_ASSERT_KNOWN(false, msg_char_star );
      }
      // remove v_ptr from inuse list
      previous->next_  = node->next_;
# endif
      // trace option
      if( capacity==CPPAD_TRACE_CAPACITY && thread==CPPAD_TRACE_THREAD )
      {  std::cout << "return_memory: v_ptr = " << v_ptr << std::endl; }

# endif
      // capacity bytes are removed from the inuse pool
      dec_inuse(capacity, thread);

      // check for case where we just return the memory to the system
      if( ! set_get_hold_memory(false) )
      {  ::operator delete( reinterpret_cast<void*>(node) );
         return;
      }

      // add this node to available list for this thread and capacity
      block_t* available_root = info->root_available_ + c_index;
      node->next_             = available_root->next_;
      available_root->next_   = reinterpret_cast<void*>(node);

      // capacity bytes are added to the available pool
      inc_available(capacity, thread);
   }
/* -----------------------------------------------------------------------
{xrst_begin ta_free_available}
{xrst_spell
   inuse
}

Free Memory Currently Available for Quick Use by a Thread
#########################################################

Syntax
******
``thread_alloc::free_available`` ( *thread* )

Purpose
*******
Return to the system all the memory that is currently being
:ref:`held<ta_hold_memory-name>` for quick use by the specified thread.

Extra Memory
============
In the case where *thread*  > 0 ,
some extra memory is used to track allocations by the specified thread.
If

   ``thread_alloc::inuse`` ( *thread* ) == 0

the extra memory is also returned to the system.

thread
******
This argument has prototype

   ``size_t`` *thread*

Either :ref:`thread_num<ta_thread_num-name>` must be the same as *thread* ,
or the current execution mode must be sequential
(not :ref:`parallel<ta_in_parallel-name>` ).

Example
*******
:ref:`thread_alloc.cpp-name`

{xrst_end ta_free_available}
*/
   /*!
   Return all the memory being held as available for a thread to the system.

   \param thread [in]
   this thread that will no longer have any available memory after this call.
   This must either be the thread currently executing, or we must be
   in sequential (not parallel) execution mode.
   */
   static void free_available(size_t thread)
   {  CPPAD_ASSERT_KNOWN(
         thread < CPPAD_MAX_NUM_THREADS,
         "Attempt to free memory for a thread >= CPPAD_MAX_NUM_THREADS"
      );
      CPPAD_ASSERT_KNOWN(
         thread == thread_num() || (! in_parallel()),
         "Attempt to free memory for a different thread "
         "while in parallel mode"
      );

      size_t num_cap = capacity_info()->number;
      if( num_cap == 0 )
         return;
      const size_t*     capacity_vec  = capacity_info()->value;
      size_t c_index;
      thread_alloc_info* info = thread_info(thread);
      for(c_index = 0; c_index < num_cap; c_index++)
      {  size_t capacity = capacity_vec[c_index];
         block_t* available_root = info->root_available_ + c_index;
         void* v_ptr             = available_root->next_;
         while( v_ptr != nullptr )
         {  block_t* node = reinterpret_cast<block_t*>(v_ptr);
            void* next    = node->next_;
            ::operator delete(v_ptr);
            v_ptr         = next;

            dec_available(capacity, thread);
         }
         available_root->next_ = nullptr;
      }
      CPPAD_ASSERT_UNKNOWN( available(thread) == 0 );
      if( inuse(thread) == 0 )
      {  // clear the information for this thread
         thread_info(thread, true);
      }
   }
/* -----------------------------------------------------------------------
{xrst_begin ta_hold_memory}

Control When Thread Alloc Retains Memory For Future Use
#######################################################

Syntax
******
``thread_alloc::hold_memory`` ( *value* )

Purpose
*******
It should be faster, even when *num_thread* is equal to one,
for ``thread_alloc`` to hold onto memory.
Calling *hold_memory* with *value* equal to true,
instructs ``thread_alloc`` to hold onto memory,
and put it in the :ref:`available<ta_available-name>` pool,
after each call to :ref:`return_memory<ta_return_memory-name>` .

value
*****
If *value* is true,
``thread_alloc`` with hold onto memory for future quick use.
If it is false, future calls to :ref:`return_memory<ta_return_memory-name>`
will return the corresponding memory to the system.
By default (when ``hold_memory`` has not been called)
``thread_alloc`` does not hold onto memory.

free_available
**************
Memory that is being held by ``thread_alloc`` can be returned
to the system using :ref:`free_available<ta_free_available-name>` .

{xrst_end ta_hold_memory}
*/
   /*!
   Change the thread_alloc hold memory setting.

   \param value [in]
   New value for the thread_alloc hold memory setting.
   */
   static void hold_memory(bool value)
   {  bool set = true;
      set_get_hold_memory(set, value);
   }

/* -----------------------------------------------------------------------
{xrst_begin ta_inuse}

Amount of Memory a Thread is Currently Using
############################################

Syntax
******
*num_bytes* = ``thread_alloc::inuse`` ( *thread* )

Purpose
*******
Memory being managed by :ref:`thread_alloc-name` has two states,
currently in use by the specified thread,
and quickly available for future use by the specified thread.
This function informs the program how much memory is in use.

thread
******
This argument has prototype

   ``size_t`` *thread*

Either :ref:`thread_num<ta_thread_num-name>` must be the same as *thread* ,
or the current execution mode must be sequential
(not :ref:`parallel<ta_in_parallel-name>` ).

num_bytes
*********
The return value has prototype

   ``size_t`` *num_bytes*

It is the number of bytes currently in use by the specified thread.

Example
*******
:ref:`thread_alloc.cpp-name`

{xrst_end ta_inuse}
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
   {
      CPPAD_ASSERT_UNKNOWN( thread < CPPAD_MAX_NUM_THREADS);
      CPPAD_ASSERT_UNKNOWN(
         thread == thread_num() || (! in_parallel())
      );
      thread_alloc_info* info = thread_info(thread);
      return info->count_inuse_;
   }
/* -----------------------------------------------------------------------
{xrst_begin ta_available}

Amount of Memory Available for Quick Use by a Thread
####################################################

Syntax
******
*num_bytes* = ``thread_alloc::available`` ( *thread* )

Purpose
*******
Memory being managed by :ref:`thread_alloc-name` has two states,
currently in use by the specified thread,
and quickly available for future use by the specified thread.
This function informs the program how much memory is available.

thread
******
This argument has prototype

   ``size_t`` *thread*

Either :ref:`thread_num<ta_thread_num-name>` must be the same as *thread* ,
or the current execution mode must be sequential
(not :ref:`parallel<ta_in_parallel-name>` ).

num_bytes
*********
The return value has prototype

   ``size_t`` *num_bytes*

It is the number of bytes currently available for use by the specified thread.

Example
*******
:ref:`thread_alloc.cpp-name`

{xrst_end ta_available}
*/
   /*!
   Determine the amount of memory that is currently available for use.

   \copydetails inuse
   */
   static size_t available(size_t thread)
   {
      CPPAD_ASSERT_UNKNOWN( thread < CPPAD_MAX_NUM_THREADS);
      CPPAD_ASSERT_UNKNOWN(
         thread == thread_num() || (! in_parallel())
      );
      thread_alloc_info* info = thread_info(thread);
      return info->count_available_;
   }
/* -----------------------------------------------------------------------
{xrst_begin ta_create_array}

Allocate An Array and Call Default Constructor for its Elements
###############################################################

Syntax
******
*array* = ``thread_alloc::create_array<`` *Type* >( *size_min* , *size_out* ) .

Purpose
*******
Create a new raw array using :ref:`thread_alloc-name` memory allocator
(works well in a multi-threading environment)
and call default constructor for each element.

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
Note that :ref:`delete_array<ta_delete_array-name>`
should be used to destroy the array when it is no longer needed.

Delta
*****
The amount of memory :ref:`inuse<ta_inuse-name>` by the current thread,
will increase *delta* where

   ``sizeof`` ( *Type* ) * ( *size_out* + 1) > *delta* >= ``sizeof`` ( *Type* ) * *size_out*

The :ref:`available<ta_available-name>` memory will decrease by *delta* ,
(and the allocation will be faster)
if a previous allocation with *size_min* between its current value
and *size_out* is available.

Alignment
*********
We call a memory allocation aligned if the address is a multiple
of the number of bytes in a ``size_t`` value.
If the system ``new`` allocator is aligned, then *array*
pointer is also aligned.

Example
*******
:ref:`thread_alloc.cpp-name`

{xrst_end ta_create_array}
*/
   /*!
   Use thread_alloc to allocate an array, then call default construtor
   for each element.

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
   The extra_ field, in the thread_alloc node before the return value,
   is set to size_out.
   */
   template <class Type>
   static Type* create_array(size_t size_min, size_t& size_out)
   {  // minimum number of bytes to allocate
      size_t min_bytes = size_min * sizeof(Type);
      // do the allocation
      size_t num_bytes;
      void*  v_ptr     = get_memory(min_bytes, num_bytes);
      // This is where the array starts
      Type*  array     = reinterpret_cast<Type*>(v_ptr);
      // number of Type values in the allocation
      size_out         = num_bytes / sizeof(Type);
      // store this number in the extra field
      block_t* node    = reinterpret_cast<block_t*>(v_ptr) - 1;
      node->extra_     = size_out;

      // call default constructor for each element
      size_t i;
      for(i = 0; i < size_out; i++)
         new(array + i) Type();

      return array;
   }
/* -----------------------------------------------------------------------
{xrst_begin ta_delete_array}
{xrst_spell
   deallocate
}

Deallocate An Array and Call Destructor for its Elements
########################################################

Syntax
******
``thread_alloc::delete_array`` ( *array* ) .

Purpose
*******
Returns memory corresponding to an array created by
(create by :ref:`create_array<ta_create_array-name>` ) to the
:ref:`available<ta_available-name>` memory pool for the current thread.

Type
****
The type of the elements of the array.

array
*****
The argument *array* has prototype

   *Type* * *array*

It is a value returned by :ref:`create_array<ta_create_array-name>` and not yet deleted.
The *Type* destructor is called for each element in the array.

Thread
******
The :ref:`current thread<ta_thread_num-name>` must be the
same as when :ref:`create_array<ta_create_array-name>` returned the value *array* .
There is an exception to this rule:
when the current execution mode is sequential
(not :ref:`parallel<ta_in_parallel-name>` ) the current thread number does not matter.

Delta
*****
The amount of memory :ref:`inuse<ta_inuse-name>` will decrease by *delta* ,
and the :ref:`available<ta_available-name>` memory will increase by *delta* ,
where :ref:`ta_create_array@Delta`
is the same as for the corresponding call to ``create_array`` .

Example
*******
:ref:`thread_alloc.cpp-name`

{xrst_end ta_delete_array}
*/
   /*!
   Return Memory Used for an Array to the Available Pool
   (include destructor call for each element).

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
   {  // determine the number of values in the array
      block_t* node = reinterpret_cast<block_t*>(array) - 1;
      size_t size     = node->extra_;

      // call destructor for each element
      size_t i;
      for(i = 0; i < size; i++)
         (array + i)->~Type();

      // return the memory to the available pool for this thread
      thread_alloc::return_memory( reinterpret_cast<void*>(array) );
   }
/* -----------------------------------------------------------------------
{xrst_begin ta_free_all}

Free All Memory That Was Allocated for Use by thread_alloc
##########################################################

Syntax
******
*ok* = ``thread_alloc::free_all`` () .

Purpose
*******
Returns all memory that was used by ``thread_alloc`` to the system.

ok
**
The return value *ok* has prototype

   ``bool`` *ok*

Its value will be ``true`` if all the memory can be freed.
This requires that for all *thread* indices, there is no memory
:ref:`inuse<ta_inuse-name>` ; i.e.,

   0 == ``thread_alloc::inuse`` ( *thread* )

Otherwise, the return value will be false.

Restrictions
************
This function cannot be called while in parallel mode.

Example
*******
:ref:`thread_alloc.cpp-name`

{xrst_end ta_free_all}
*/
   /*!
   Return to the system all thread_alloc memory that is not currently inuse.

   \return
   If no thread_alloc memory is currently inuse,
   all memory is returned to the system and the return value is true.
   Otherwise the return value is false.
   */
   static bool free_all(void)
   {  CPPAD_ASSERT_KNOWN(
         ! in_parallel(),
         "free_all cannot be used while in parallel execution"
      );
      bool ok = true;
      size_t thread = CPPAD_MAX_NUM_THREADS;
      while(thread--)
      {  ok &= inuse(thread) == 0;
         free_available(thread);
      }
      return ok;
   }
};


} // END_CPPAD_NAMESPACE

// preprocessor symbols local to this file
# undef CPPAD_MAX_NUM_CAPACITY
# undef CPPAD_MIN_DOUBLE_CAPACITY
# undef CPPAD_TRACE_CAPACITY
# undef CPPAD_TRACE_THREAD
# endif
