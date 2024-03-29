// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin team_openmp.cpp}

OpenMP Implementation of a Team of AD Threads
#############################################
See :ref:`team_thread.hpp-name` for this routines specifications.

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end team_openmp.cpp}
*/
// BEGIN C++
# include <omp.h>
# include <cppad/cppad.hpp>
# include "../team_thread.hpp"

namespace {
   using CppAD::thread_alloc;

   // number of threads in this team
   size_t num_threads_;

   // used to inform CppAD when we are in parallel execution mode
   bool in_parallel(void)
   {  return omp_in_parallel() != 0; }

   // used to inform CppAD of the current thread number
   size_t thread_num(void)
   {  return static_cast<size_t>( omp_get_thread_num() ); }
}

bool team_create(size_t num_threads)
{
   bool ok = ! in_parallel();
   ok     &= thread_num() == 0;;
   ok     &= num_threads > 0;

   // Turn off dynamic thread adjustment
   omp_set_dynamic(0);

   // Set the number of OpenMP threads
   omp_set_num_threads( int(num_threads) );

   // setup for using CppAD::AD<double> in parallel
   thread_alloc::parallel_setup(num_threads, in_parallel, thread_num);
   thread_alloc::hold_memory(true);
   CppAD::parallel_ad<double>();

   // inform team_work of number of threads
   num_threads_ = num_threads;

   return ok;
}

bool team_work(void worker(void))
{  bool ok = ! in_parallel();
   ok     &= thread_num() == 0;;
   ok     &= num_threads_ > 0;

   int number_threads = int(num_threads_);
   int thread_num;
# pragma omp parallel for
   for(thread_num = 0; thread_num < number_threads; thread_num++)
      worker();
// end omp parallel for

   return ok;
}

bool team_destroy(void)
{  bool ok = ! in_parallel();
   ok     &= thread_num() == 0;;
   ok     &= num_threads_ > 0;

   // inform team_work of number of threads
   num_threads_ = 1;

   // Set the number of OpenMP threads to one
   omp_set_num_threads( int(num_threads_) );

   // inform CppAD no longer in multi-threading mode
   thread_alloc::parallel_setup(num_threads_, nullptr, nullptr);
   thread_alloc::hold_memory(false);
   CppAD::parallel_ad<double>();

   return ok;
}

const char* team_name(void)
{  return "openmp"; }
// END C++
