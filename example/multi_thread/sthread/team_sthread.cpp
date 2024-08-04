// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin team_sthread.cpp}

Standard Thread Implementation of a Team of AD Threads
######################################################
See :ref:`team_thread.hpp-name` for this routines specifications.

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end team_sthread.cpp}
*/
// BEGIN C++
# include <thread>
# include <mutex>
# include <map>
# include <cppad/cppad.hpp>
# include "../team_thread.hpp"

namespace {
   using CppAD::thread_alloc;
   //
   // begin_work_mutex_;
   std::mutex begin_work_mutex_;
   //
   // thread_id2num_
   std::map< std::thread::id, size_t > thread_id2num_;
   //
   // num_threads_
   // number of threads in this team
   size_t num_threads_;
   //
   // sequential_execution_
   bool sequential_execution_ = true;
   //
   // in_parallel
   // used to inform CppAD when we are in parallel execution mode
   bool in_parallel(void)
   {  return ! sequential_execution_; }
   //
   // thread_number
   // used to inform CppAD of the current thread number
   size_t thread_number(void)
   {  return thread_id2num_.at( std::this_thread::get_id() ); }
}
// team_create
bool team_create(size_t num_threads)
{
   bool ok = ! in_parallel();
   ok     &= num_threads > 0;
   //
   // thread_id2num_
   // must setup for this thread before calling parallel_setup
   thread_id2num_.clear();
   std::thread::id thread_id  = std::this_thread::get_id();
   size_t                       thread_num = 0;
   thread_id2num_[thread_id]  = thread_num;
   //
   // setup for using CppAD::AD<double> in parallel
   thread_alloc::parallel_setup(num_threads, in_parallel, thread_number);
   thread_alloc::hold_memory(true);
   CppAD::parallel_ad<double>();
   //
   // num_thread_
   num_threads_ = num_threads;
   //
   return ok;
}
// work_wrapper
void work_wrapper(void worker(void))
{  // begin_work_mutex_
   // wait here while thread_id2num_ is changing
   begin_work_mutex_.lock();
   begin_work_mutex_.unlock();
   //
   // now go to work
   worker();
}
// team_work
bool team_work( void worker(void) )
{  bool ok = sequential_execution_;
   ok     &= num_threads_ > 0;
   //
   // begin_work_mutex_
   // stop all threads at the beginning of the work routine
   begin_work_mutex_.lock();
   //
   // sequential_execution
   sequential_execution_ = false;
   //
   // thread_ptr
   CppAD::vector<std::thread*> thread_ptr(num_threads_ - 1);
   //
   // thread_num
   for(size_t thread_num = 1; thread_num < num_threads_; ++thread_num)
   {  //
      // thread_ptr
      thread_ptr[thread_num - 1] = new std::thread(work_wrapper, worker);
      //
      // thread_id
      std::thread::id thread_id = thread_ptr[thread_num-1]->get_id();
      //
      // thread_id2num_
      thread_id2num_[thread_id] = thread_num;
   }
   //
   // begin_work_mutex_
   // Let the threads go
   begin_work_mutex_.unlock();
   //
   // put this thread to work
   worker();
   //
   // join
   for(size_t thread_num = 1; thread_num < num_threads_; ++thread_num)
   {  thread_ptr[thread_num-1]->join();
      delete thread_ptr[thread_num-1];
   }
   //
   // sequential execution
   sequential_execution_ = true;
   //
   return ok;
}

bool team_destroy(void)
{  bool ok = ! in_parallel();
   ok     &= thread_number() == 0;;
   ok     &= num_threads_ > 0;

   // inform team_work of number of threads
   num_threads_ = 1;
   //
   // inform CppAD no longer in multi-threading mode
   thread_alloc::parallel_setup(num_threads_, nullptr, nullptr);
   thread_alloc::hold_memory(false);
   CppAD::parallel_ad<double>();
   //
   return ok;
}

const char* team_name(void)
{  return "sthread"; }
// END C++
