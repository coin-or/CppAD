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
	// =====================================================================
	// General purpose code you can copy to your application
	// =====================================================================
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
		// false if an error occurs, true otherwise
		bool            ok;
		// pointer to problem specific information
		void*           info;
	} thread_one_t;
	// vector with information for all threads
	thread_one_t thread_all_[NUMBER_THREADS];
	// --------------------------------------------------------------------
	// function that initializes the thread and then calls the actual worker
	bool worker(size_t thread_num);
	void* run_worker(void* thread_num_vptr)
	{	bool ok = true;

		// thread_num for this thread
		size_t thread_num = *static_cast<size_t*>(thread_num_vptr);

		// The master thread should call worker directly
		ok &= thread_num != 0;

		// This is not the master thread, so thread specific infromation
		// has not yet been set. We use it to inform other routines 
		// of this threads number.
		// We must do this before calling thread_alloc::thread_num().
		int rc = pthread_setspecific(
			thread_specific_key_,
			thread_num_vptr
		);
		ok &= rc == 0;

		// check the value of thread_alloc::thread_num().
		ok = thread_num == thread_alloc::thread_num();

		// Now do the work
		ok &= worker(thread_num);

		// pass back ok information for this thread
		thread_all_[thread_num].ok = ok;

		// no return value
		return CPPAD_NULL;
	}
	// =====================================================================
	// End of General purpose code 
	// =====================================================================
	// structure with problem specific information
	typedef struct {
		// function argument (worker input)
		double          x;
		// This structure would also have return information in it,
		// but this example only returns the ok flag
	} problem_specific;
	// ---------------------------------------------------------------------
	// function that does the work for one thread
	bool worker(size_t thread_num)
	{	bool ok = true;

		// get pointer to problem specific information for this thread
		problem_specific* info = static_cast<problem_specific*>(
			thread_all_[thread_num].info
		);
	
		// CppAD::vector uses the CppAD fast multi-threading allocator
		CppAD::vector< CppAD::AD<double> > ax(1), ay(1);
		ax[0] = info->x;
		Independent(ax);
		ay[0] = sqrt( ax[0] * ax[0] );
		CppAD::ADFun<double> f(ax, ay); 

		// Check function value corresponds to the identity 
		double eps = 10. * CppAD::epsilon<double>();
		ok        &= CppAD::NearEqual(ay[0], ax[0], eps, eps);

		// Check derivative value corresponds to the identity.
		CppAD::vector<double> d_x(1), d_y(1);
		d_x[0] = 1.;
		d_y    = f.Forward(1, d_x);
		ok    &= CppAD::NearEqual(d_x[0], 1., eps, eps);

		return ok;
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
	problem_specific* info;
	for(thread_num = 0; thread_num < num_threads; thread_num++)
	{	// pointed to by thread specific info for this thread
		thread_all_[thread_num].thread_num = thread_num;
		// initialize as false to make sure worker gets called
		thread_all_[thread_num].ok         = false;
		// problem specific information
		size_t min_bytes(sizeof(info)), cap_bytes;
		void*  v_ptr = thread_alloc::get_memory(min_bytes, cap_bytes);
		thread_all_[thread_num].info = v_ptr;
		info         = static_cast<problem_specific*>(v_ptr);
		info->x      = double(thread_num) + 1.;
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
	// inform CppAD that we now may be in parallel execution mode
	sequential_execution_ = false;
	
	// structure used to create the threads
	pthread_t       pthread_id;
	// default for pthread_attr_setdetachstate is PTHREAD_CREATE_JOINABLE
	pthread_attr_t* no_attr= CPPAD_NULL;

	// This master thread is already running, we need to create
	// num_threads - 1 more threads
	for(thread_num = 1; thread_num < num_threads; thread_num++)
	{	// Create the thread with thread number equal to thread_num
		thread_num_vptr = static_cast<void*> (
			&(thread_all_[thread_num].thread_num)
		);
		rc = pthread_create(
				&pthread_id ,
				no_attr     ,
				run_worker,
				thread_num_vptr
		);
		thread_all_[thread_num].pthread_id = pthread_id;
		ok &= (rc == 0);
	}

	// now call worker for the master thread
	thread_num = thread_alloc::thread_num();
	ok &= thread_num == 0;
	ok &= worker(thread_num);
	// just to make the loop over all threads simpler
	thread_all_[thread_num].ok = ok;

	// now wait for the other threads to finish 
	for(thread_num = 1; thread_num < num_threads; thread_num++)
	{	void* no_status = CPPAD_NULL;
		rc      = pthread_join(
			thread_all_[thread_num].pthread_id, &no_status
		);
		ok &= (rc == 0);
	}

	// Inform CppAD that we now are definately back to sequential execution
	sequential_execution_ = true;

	// now inform CppAD that there is only one thread
	thread_alloc::parallel_setup(1, CPPAD_NULL, CPPAD_NULL);
	thread_alloc::hold_memory(false);

	// destroy the key for thread specific data
	pthread_key_delete(thread_specific_key_);

	// go down so that free memory for other threads before memory for master
	thread_num = num_threads;
	while(thread_num--)
	{	// check that this thread was called and successful
		ok &= thread_all_[thread_num].ok;
		// delete problem specific information
		thread_alloc::return_memory( thread_all_[thread_num].info );
		// check that there is no longer any memory inuse by this thread
		ok &= thread_alloc::inuse(thread_num) == 0; 
		// return all memory being held for future use by this thread
		thread_alloc::free_available(thread_num); 
	}

	return ok;
}
// END PROGRAM
