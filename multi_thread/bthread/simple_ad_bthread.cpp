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
$begin simple_ad_bthread.cpp$$
$spell
	bthread
	CppAD
$$

$section A Simple Boost Threading AD: Example and Test$$

$index boost, simple thread AD$$
$index AD, simple boost thread$$
$index simple, boost thread AD$$
$index thread, simple boost AD$$

$head Purpose$$
This example demonstrates how CppAD can be used in a 
boost multi-threading environment.

$head Source Code$$
$code
$verbatim%multi_thread/bthread/simple_ad_bthread.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
------------------------------------------------------------------------------
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>
# include <boost/thread.hpp>
# define NUMBER_THREADS  4

namespace {
	using CppAD::thread_alloc;
	// ------------------------------------------------------------------
	// thread specific point to the thread number (initialize as null)
	void cleanup(size_t*)
	{	return; }
	boost::thread_specific_ptr<size_t> thread_num_ptr_(cleanup);

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
	{	// return thread_all_[thread_num].thread_num
		return *thread_num_ptr_.get();
	}
	// ---------------------------------------------------------------------
	// structure with information for one thread
	typedef struct {
		// The thread
		boost::thread*  bthread;
		// number for this thread (thread specific points here)
		size_t          thread_num;
		// function argument (worker input)
		double          x;
		// false if an error occurs, true otherwise (worker output)
		bool            ok;
	} thread_one_t;
	// vector with information for all threads
	thread_one_t thread_all_[NUMBER_THREADS];
	// --------------------------------------------------------------------
	// function that does the work for one thread
	void worker(size_t thread_num)
	{	using CppAD::NearEqual;
		using CppAD::AD;
		bool ok = true;

		// thread_num for this thread
		if( thread_num > 0 )
		{	// This is not the master thread, so thread specific infromation
			// has not yet been set so other routines know thread number.
			thread_num_ptr_.reset(& thread_all_[thread_num].thread_num);
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
		return;
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
	// master bthread number
	thread_num_ptr_.reset(& thread_all_[0].thread_num);

	// Now that thread_number() has necessary information for this thread
	// (number zero), and while still in sequential mode,
	// call setup for using CppAD::AD<double> in parallel mode.
	thread_alloc::parallel_setup(num_threads, in_parallel, thread_number);
	thread_alloc::hold_memory(true);
	CppAD::parallel_ad<double>();
	
	// This master thread is already running, we need to create
	// num_threads - 1 more threads
	thread_all_[0].bthread = CPPAD_NULL;
	for(thread_num = 1; thread_num < num_threads; thread_num++)
	{	// inform CppAD that we are now in parallel execution mode
		sequential_execution_ = false;
	
		// Create the thread with thread number equal to thread_num
		thread_all_[thread_num].bthread = 
			new boost::thread(worker, thread_num);
	}

	// now call worker for the master thread
	worker(thread_all_[0].thread_num);

	// now wait for the other threads to finish 
	for(thread_num = 1; thread_num < num_threads; thread_num++)
	{	thread_all_[thread_num].bthread->join();
		delete thread_all_[thread_num].bthread;
		thread_all_[thread_num].bthread = CPPAD_NULL;
	}


	// Back to sequential execution mode
	sequential_execution_ = true;


	// Check that all the threads were called and succeeded
	for(thread_num = 0; thread_num < num_threads; thread_num++)
		ok &= thread_all_[thread_num].ok;

	// Check that no memory currently in use, and free avialable memory.
	for(thread_num = 0; thread_num < num_threads; thread_num++)
	{	ok &= thread_alloc::inuse(thread_num) == 0; 
		thread_alloc::free_available(thread_num); 
	}

	// now inform CppAD that there is only one thread
	using CppAD::thread_alloc;
	thread_alloc::parallel_setup(1, CPPAD_NULL, CPPAD_NULL);
	thread_alloc::hold_memory(false);

	return ok;
}
// END PROGRAM
