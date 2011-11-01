// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/* 
$begin bthread_team.cpp$$
$spell
	bthread
$$

$index bthread, AD team$$
$index AD, bthread team$$
$index team, AD bthread$$

$section Boost Thread Implementation of a Team of AD Threads$$
See $cref thread_team.hpp$$ for this routines specifications.

$code
$verbatim%multi_thread/bthread/bthread_team.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <boost/thread.hpp>
# include <cppad/cppad.hpp>
# include "../thread_team.hpp"
# define MAX_NUMBER_THREADS 48

namespace {
	// number of threads in the team
	size_t num_threads_ = 1; 

	// type of the job currently being done by each thread
	enum thread_job_t { init_enum, work_enum, join_enum } thread_job_;

	// barrier used to wait for other threads to finish work
	boost::barrier* wait_for_work_ = 0;

	// barrier used to wait for master thread to set next job
	boost::barrier* wait_for_job_ = 0;

	// Are we in sequential mode; i.e., other threads are waiting for
	// master thread to set up next job ?
	bool sequential_execution_ = true;

	// structure with information for one thread
	typedef struct {
		boost::thread*       bthread;
		// Boost unique identifier for thread that uses this struct
		boost::thread::id    bthread_id;
		// true if no error for this thread, false otherwise.
		bool                 ok;
	} thread_one_t;

	// vector with information for all threads
	thread_one_t thread_all_[MAX_NUMBER_THREADS];

	// pointer to function that does the work for one thread
	void (* worker_)(void) = 0;

	// ---------------------------------------------------------------------
	// in_parallel()
	bool in_parallel(void)
	{	return ! sequential_execution_; }

	// ---------------------------------------------------------------------
	// thread_number()
	size_t thread_number(void)
	{	using boost::thread;

		// bthread unique identifier for this thread
		thread::id thread_this = boost::this_thread::get_id();

		// convert thread_this to the corresponding thread_num
		size_t thread_num = 0;
		for(thread_num = 0; thread_num < num_threads_; thread_num++)
		{	// Boost unique identifier for this thread_num
			thread::id thread_compare = thread_all_[thread_num].bthread_id;

			// check for a match
			if( thread_this == thread_compare )
				return thread_num;
		}
		// no match error (thread_this is not in thread_all_).
		std::cerr << "thread_number: unknown boost::thread::id" << std::endl;
		exit(1);

		return 0;
	}
	// --------------------------------------------------------------------
	// function that gets called by boost thread constructor
	void thread_work(size_t thread_num)
	{	bool ok = wait_for_work_ != 0;
		ok     &= wait_for_job_  != 0;
		ok     &= thread_num     != 0;

		while( true )
		{
			// Use wait_for_jog_ to give master time in sequential mode
			// (so it can change global information like thread_job_)
			wait_for_job_->wait();

			// case where we are terminating this thread (no more work)
			if( thread_job_ == join_enum)
				break;

			// only other case once wait_for_job_ has been completed (so far)
			ok &= thread_job_ == work_enum;
			worker_();

			// Use wait_for_work_ to inform master that our work is done and
			// that this thread will not use global infromation until
			// passing its barrier wait_for_job_ above.
			wait_for_work_->wait();

		}
		thread_all_[thread_num].ok &= ok;
		return;
	}
}

bool start_team(size_t num_threads)
{	using CppAD::thread_alloc;
	bool ok = true;;

	if( num_threads > MAX_NUMBER_THREADS )
	{	std::cerr << "start_team: num_threads greater than ";
		std::cerr << MAX_NUMBER_THREADS << std::endl;
		exit(1);
	}
	// check that we currently do not have multiple threads running
	ok  = num_threads_ == 1;
	ok &= wait_for_work_ == 0;
	ok &= wait_for_job_  == 0;
	ok &= sequential_execution_;

	// Set the information for this thread so thread_number will work
	// for call to parallel_setup
	thread_all_[0].bthread_id = boost::this_thread::get_id();
	thread_all_[0].bthread    = 0; // not used
	thread_all_[0].ok         = true;

	// Now that thread_number() has necessary information for the case
	// num_threads_ == 1, and while still in sequential mode,
	// call setup for using CppAD::AD<double> in parallel mode.
	thread_alloc::parallel_setup(num_threads, in_parallel, thread_number);
	CppAD::parallel_ad<double>();

	// now change num_threads_ to its final value.
	num_threads_ = num_threads;

	// initialize two barriers, one for work done, one for new job ready
	wait_for_work_ = new boost::barrier(num_threads); 
	wait_for_job_  = new boost::barrier(num_threads); 
	
	// initial job for the threads
	thread_job_           = init_enum;
	if( num_threads > 1 )
		sequential_execution_ = false;

	// This master thread is already running, we need to create
	// num_threads - 1 more threads
	size_t thread_num;
	for(thread_num = 1; thread_num < num_threads; thread_num++)
	{	thread_all_[thread_num].ok         = true;
		// Create the thread with thread number equal to thread_num
		thread_all_[thread_num].bthread = 
			new boost::thread(thread_work, thread_num);
		thread_all_[thread_num].bthread_id = 
			thread_all_[thread_num].bthread->get_id();
	}

	// Current state is other threads are at wait_for_job_.
	// This master thread (thread zero) has not completed wait_for_job_
	sequential_execution_ = true;
	for(thread_num = 0; thread_num < num_threads; thread_num++)
		ok &= thread_all_[thread_num].ok;
	return ok;
}

bool work_team(void worker(void))
{
	// Current state is other threads are at wait_for_job_.
	// This master thread (thread zero) has not completed wait_for_job_
	bool ok = sequential_execution_;
	ok     &= thread_number() == 0;
	ok     &= wait_for_work_  != 0;
	ok     &= wait_for_job_   != 0;

	// set global version of this work routine
	worker_ = worker;

	// set the new job that other threads are waiting for
	thread_job_ = work_enum;

	// Enter parallel exectuion when master thread calls wait_for_job_ 
	if( num_threads_ > 1 )
		sequential_execution_ = false;
	wait_for_job_->wait();

	// Now do the work in this thread and then wait
	// until all threads have completed wait_for_work_
	worker();
	wait_for_work_->wait();

	// Current state is other threads are at wait_for_job_.
	// This master thread (thread zero) has not completed wait_for_job_
	sequential_execution_ = true;

	size_t thread_num;
	for(thread_num = 0; thread_num < num_threads_; thread_num++)
		ok &= thread_all_[thread_num].ok;
	return ok;
}

bool stop_team(void)
{	// Current state is other threads are at wait_for_job_.
	// This master thread (thread zero) has not completed wait_for_job_
	bool ok = sequential_execution_;
	ok     &= thread_number() == 0;
	ok     &= wait_for_work_ != 0;
	ok     &= wait_for_job_  != 0;

	// set the new job that other threads are waiting for
	thread_job_ = join_enum;

	// enter parallel exectuion soon as master thread completes wait_for_job_ 
	if( num_threads_ > 1 )
			sequential_execution_ = false;
	wait_for_job_->wait();

	// now wait for the other threads to be destroyed
	size_t thread_num;
	ok &= thread_all_[0].bthread == 0;
	for(thread_num = 1; thread_num < num_threads_; thread_num++)
	{	thread_all_[thread_num].bthread->join();
		delete thread_all_[thread_num].bthread;
		thread_all_[thread_num].bthread = 0;
	}
	// now we are down to just the master thread (thread zero) 
	sequential_execution_ = true;

	// destroy wait_for_work_
	delete wait_for_work_;
	wait_for_work_ = 0;

	// destroy wait_for_job_
	delete wait_for_job_;
	wait_for_job_ = 0;

	// check ok before changing num_threads_
	for(thread_num = 0; thread_num < num_threads_; thread_num++)
		ok &= thread_all_[thread_num].ok;

	// now inform CppAD that there is only one thread
	num_threads_ = 1;
	using CppAD::thread_alloc;
	thread_alloc::parallel_setup(num_threads_, in_parallel, thread_number);

	return ok;
}
// END PROGRAM
