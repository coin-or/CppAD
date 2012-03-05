// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin simple_ad_pthread.cpp$$
$spell
	pthread
	CppAD
$$

$section A Simple pthread AD: Example and Test$$

$index pthread, simple AD$$
$index AD, simple pthread$$
$index simple, pthread AD$$

$head Purpose$$
This example demonstrates how CppAD can be used in a 
pthread multi-threading environment.

$head Source Code$$
$code
$verbatim%multi_thread/pthread/simple_ad_pthread.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
------------------------------------------------------------------------------
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>
# include <pthread.h>
# define NUMBER_THREADS  4

namespace {
	using CppAD::thread_alloc;
	// ------------------------------------------------------------------
	// key for accessing thread specific information 
	pthread_key_t thread_specific_key_;

	// no need to destroy thread specific information
	void thread_specific_destructor(void* thread_num_vptr)
	{	return; }

	// Are we in sequential mode; i.e., other threads are waiting for
	// master thread to set up next job ?
	bool sequential_execution_ = true;

	// used to inform CppAD when we are in parallel execution mode
	bool in_parallel(void)
	{	return ! sequential_execution_; }

	// used to inform CppAD of current thread number thread_number()
	size_t thread_number(void)
	{	// get thread specific information
		void*   thread_num_vptr = pthread_getspecific(thread_specific_key_);	
		size_t* thread_num_ptr  = static_cast<size_t*>(thread_num_vptr);
		size_t  thread_num      = *thread_num_ptr;
		return thread_num;
	}
	// ---------------------------------------------------------------------
	// structure with information for one thread
	typedef struct {
		// number for this thread (thread specific points here)
		size_t          thread_num;
		// pthread unique identifier for this thread
		pthread_t       pthread_id;
		// function argument (worker input)
		double          x;
		// false if an error occurs, true otherwise (worker output)
		bool            ok;
	} thread_one_t;
	// vector with information for all threads
	thread_one_t thread_all_[NUMBER_THREADS];
	// --------------------------------------------------------------------
	// function that does the work for one thread
	void* worker(void* thread_num_vptr)
	{	using CppAD::NearEqual;
		using CppAD::AD;
		bool ok = true;

		// thread_num for this thread
		size_t thread_num = *static_cast<size_t*>(thread_num_vptr);
		if( thread_num > 0 )
		{	// This is not the master thread, so thread specific infromation
			// has not yet been set so other routines know thread number.
			int rc = pthread_setspecific(
				thread_specific_key_,
				thread_num_vptr
			);
			ok &= rc == 0;
		}

		// check the value of thread_alloc::thread_num()
		ok = thread_num == thread_alloc::thread_num();

		// CppAD::vector uses the CppAD fast multi-threading allocator
		CppAD::vector< AD<double> > ax(1), ay(1);
		ax[0] = thread_all_[thread_num].x;
		Independent(ax);
		ay[0] = sqrt( ax[0] * ax[0] );
		CppAD::ADFun<double> f(ax, ay); 

		// Check function value corresponds to the identity 
		double eps = 10. * CppAD::epsilon<double>();
		ok        &= NearEqual(ay[0], ax[0], eps, eps);

		// Check derivative value corresponds to the identity.
		CppAD::vector<double> d_x(1), d_y(1);
		d_x[0] = 1.;
		d_y    = f.Forward(1, d_x);
		ok    &= NearEqual(d_x[0], 1., eps, eps);

		// pass back ok information for this thread
		thread_all_[thread_num].ok = ok;

		// no return value
		return CPPAD_NULL;
	}
}

// This test routine is only called by the master thread (thread_num = 0).
bool simple_ad(void)
{	bool ok = true;

	size_t num_threads = NUMBER_THREADS;

	// Check that no memory is in use or avialable at start
	// (using thread_alloc in sequential mode)
	size_t thread_num;
	for(thread_num = 0; thread_num < num_threads; thread_num++)
	{	ok &= thread_alloc::inuse(thread_num) == 0; 
		ok &= thread_alloc::available(thread_num) == 0; 
	}

	// initialize thread_all_ (execpt for pthread_id)
	for(thread_num = 0; thread_num < num_threads; thread_num++)
	{	// pointed to by thread specific info for this thread
		thread_all_[thread_num].thread_num = thread_num;
		// argument value for this thread 
		thread_all_[thread_num].x          = double(thread_num) + 1.;
		// initialize as false to make sure worker gets called
		thread_all_[thread_num].ok         = false;
	}
	// master pthread_id
	thread_all_[0].pthread_id = pthread_self();

	// error flag for calls to pthread library
	int rc;

	// create a key for thread specific information
	rc = pthread_key_create(&thread_specific_key_,thread_specific_destructor); 
	ok &= (rc == 0);

	// set thread specific information for this (master thread)
	void* thread_num_vptr = static_cast<void*>(&(thread_all_[0].thread_num));
	rc = pthread_setspecific(thread_specific_key_, thread_num_vptr);
	ok &= (rc == 0);

	// Now that thread_number() has necessary information for this thread
	// (number zero), and while still in sequential mode,
	// call setup for using CppAD::AD<double> in parallel mode.
	thread_alloc::parallel_setup(num_threads, in_parallel, thread_number);
	thread_alloc::hold_memory(true);
	CppAD::parallel_ad<double>();
	
	// structure used to create the threads
	pthread_t       pthread_id;
	// default for pthread_attr_setdetachstate is PTHREAD_CREATE_JOINABLE
	pthread_attr_t* no_attr= CPPAD_NULL;

	// This master thread is already running, we need to create
	// num_threads - 1 more threads
	for(thread_num = 1; thread_num < num_threads; thread_num++)
	{	// inform CppAD that we are now in parallel execution mode
		sequential_execution_ = false;
	
		// Create the thread with thread number equal to thread_num
		thread_num_vptr = static_cast<void*> (
			&(thread_all_[thread_num].thread_num)
		);
		rc = pthread_create(
				&pthread_id ,
				no_attr     ,
				worker,
				thread_num_vptr
		);
		thread_all_[thread_num].pthread_id = pthread_id;
		ok &= (rc == 0);
	}

	// now call worker for the master thread
	thread_num_vptr = static_cast<void*>(&(thread_all_[0].thread_num));
	worker(thread_num_vptr);

	// now wait for the other threads to finish 
	for(thread_num = 1; thread_num < num_threads; thread_num++)
	{	void* no_status = CPPAD_NULL;
		rc      = pthread_join(
			thread_all_[thread_num].pthread_id, &no_status
		);
		ok &= (rc == 0);
	}


	// Back to sequential execution mode
	sequential_execution_ = true;

	// destroy the key for thread specific data
	pthread_key_delete(thread_specific_key_);

	// Check that all the threads were called and succeeded
	for(thread_num = 0; thread_num < num_threads; thread_num++)
		ok &= thread_all_[thread_num].ok;

	// Check that no memory currently in use, and free avialable memory.
	for(thread_num = 0; thread_num < num_threads; thread_num++)
	{	ok &= thread_alloc::inuse(thread_num) == 0; 
		thread_alloc::free_available(thread_num); 
	}

	// now inform CppAD that there is only one thread
	thread_alloc::parallel_setup(1, CPPAD_NULL, CPPAD_NULL);
	thread_alloc::hold_memory(false);

	return ok;
}
// END PROGRAM
