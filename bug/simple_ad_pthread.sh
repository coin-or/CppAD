#! /bin/bash -e
# $Id:$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
#!/bin/bash -e
echo "$0"
name=`echo $0 | sed -e 's|.*/||' -e 's|\..*||'`
echo "create $name.cpp"
cat << EOF > $name.cpp
// BEGIN PROGRAM
# include <cppad/cppad.hpp>
# include <pthread.h>
# define NUMBER_THREADS  2

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
		// function object (worker input)
		CppAD::ADFun<double> fun;
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
			assert( rc == 0 );
		}

		// check the value of thread_alloc::thread_num()
		assert( thread_num == thread_alloc::thread_num() );

		CppAD::vector< double > x(1);
		x[0]=1.0;

		CppAD::vector< double > y(1);
		CppAD::ADFun<double>* fun= &(thread_all_[thread_num].fun);

		std::stringstream stream;

		// perform CppAD_FUN_OBJECT FORWARD0
		y=fun->Forward(0,x);

		stream << "thread_num = " << thread_num;
		stream << ", y[0] = " << y[0] << std::endl;
		std::cout << stream.str();

		// no return value
		return CPPAD_NULL;
	}
}

// This test routine is only called by the master thread (thread_num = 0).
bool simple_ad(void)
{	
	// TAPING THE FUNCTION 	
	CppAD::vector< CppAD::AD<double> > x(1), y(1);
	x[0]=3.78;
	CppAD::Independent(x);
	y[0]=sin(x[0]);
  
	CppAD::ADFun<double> fun;
	fun.Dependent(x,y);

	size_t num_threads = NUMBER_THREADS;

	// initialize thread_all_ (execpt for pthread_id)
	size_t thread_num;
	for(thread_num = 0; thread_num < num_threads; thread_num++)
	{	// pointed to by thread specific info for this thread
		thread_all_[thread_num].thread_num = thread_num;
		// make a copy of ADFun object for this thread 
		thread_all_[thread_num].fun        = fun;
	}
	// master pthread_id
	thread_all_[0].pthread_id = pthread_self();

	// error flag for calls to pthread library
	int rc;

	// create a key for thread specific information
	rc = pthread_key_create(&thread_specific_key_,thread_specific_destructor); 
	assert(rc == 0);

	// set thread specific information for this (master thread)
	void* thread_num_vptr = static_cast<void*>(&(thread_all_[0].thread_num));
	rc = pthread_setspecific(thread_specific_key_, thread_num_vptr);
	assert(rc == 0);

	// Now that thread_number() has necessary information for this thread
	// (number zero), and while still in sequential mode,
	// call setup for using CppAD::AD<double> in parallel mode.
	thread_alloc::parallel_setup(num_threads, in_parallel, thread_number);
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
		assert(rc == 0);
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
		assert(rc == 0);
	}


	// Back to sequential execution mode
	sequential_execution_ = true;

	// now inform CppAD that there is only one thread
	thread_alloc::parallel_setup(1, CPPAD_NULL, CPPAD_NULL);

	return true;
}
int main(void)
{	bool ok = simple_ad();
	std::cout << "OK = " << ok << std::endl;
	return int(! ok);
} 
EOF
echo "g++ -g $name.cpp -I$HOME/cppad/trunk -lpthread -o $name"
g++ -g $name.cpp -I$HOME/cppad/trunk -lpthread -o $name
#
echo "./$name"
./$name
