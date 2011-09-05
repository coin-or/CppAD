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
$begin pthread_ad.cpp$$
$spell
	Cygwin
	pthread
	pthreads
	CppAD
$$

$section Simple Pthread Parallel AD: Example and Test$$

$index pthread_ad, example$$
$index AD, parallel pthread$$
$index parallel, AD pthread$$

$head Purpose$$
This example demonstrates how CppAD can be used with multiple pthreads.

$head Discussion$$
The function $code arc_tan$$ below
is an implementation of the inverse tangent function where the
$cref/operation sequence/glossary/Operation/Sequence/$$ depends on the
argument values. 
Hence the function needs to be re-taped 
for different argument values.
The $cref/atan2/$$ function uses $cref/CondExp/$$ operations
to avoid the need to re-tape.

$head pthread_exit Bug in Cygwin$$
$index bug, cygwin pthread_exit$$
$index cygwin, bug in pthread_exit$$
$index pthread_exit, bug in cygwin$$ 
There is a bug in $code pthread_exit$$,
using cygwin 5.1 and g++ version 4.3.4,
whereby calling $code pthread_exit$$ is not the same as returning from
the corresponding routine.
To be specific, destructors for the vectors are not called
and a memory leaks result.
Search for $code pthread_exit$$ in the source code below to
see how to demonstrate this bug.

$head Source Code$$
$code
$verbatim%example/pthread_ad.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
------------------------------------------------------------------------------
*/
// BEGIN PROGRAM
# include <pthread.h>
# include <cppad/cppad.hpp>

# define NUMBER_THREADS            4
# define DEMONSTRATE_BUG_IN_CYGWIN 0

namespace {
	// ===================================================================
	// General purpose code for linking CppAD to pthreads
	// -------------------------------------------------------------------
	// alternative name for NUMBER_THREADS
	size_t number_threads = NUMBER_THREADS; 

	// The master thread switches the value of this variable
	static bool multiple_threads_active = false;

	// general purpose vector with information for each thread
	typedef struct {
		// Mutex used to synchronize access to this struct.
		pthread_mutex_t mutex;
		// pthread unique identifier for thread that uses this struct
		pthread_t       thread_id;
		// cppad unique identifier for thread that uses this struct
		size_t          thread_index;
		// False if error related to thread corresponding to this struct
		// occurred, true otherwise.
		bool            ok;
	} thread_info_t;
	thread_info_t thread_vector[NUMBER_THREADS];

	// ----------------------------------------------------------------------
	// This function sets and gets the unique index for each thread
	// 	0 <= index < number_threads
	// if index < number_threads, then we first set the value for this thread
	// and wait for other threads to do the same
	size_t set_get_thread_num(size_t index_this)
	{	size_t index;
		int rc;

		// Check for case where we know it this is the master thread
		if( ! multiple_threads_active )
			return 0;

		// pthread system unique identifier for this thread
		pthread_t thread_this = pthread_self();

		// If index_this is a valid index, set this thread_id and
		// wait until all the thread_id values have been set.
		bool wait = (index_this < number_threads);
		while( wait )
		{	bool all_set = true;
			for(index = 0; index < number_threads; index++)
			{	// get lock on thread_vector[index].thread_id	
				rc  = pthread_mutex_lock( &(thread_vector[index].mutex) );

				// no need for a lock on thread_vector[index_this].ok
				thread_vector[index_this].ok &= (rc == 0);

				pthread_t thread_zero;
				if( index == 0 )
				{	// pthread identity corresponding to master
					thread_zero = thread_vector[index].thread_id;
					// master has been before this call
				}
				else if( index == index_this )
				{	// pthread identity corresponding to this thread
					thread_vector[index].thread_id = thread_this;
					// this thread_it has just been set
				}
				else
				{	// check if this thread_id has been set yet.
					all_set &= ! pthread_equal(
						thread_vector[index].thread_id, thread_zero
					);
				}

				// free lock on thread_vector[index].thread_id
				rc = pthread_mutex_unlock( &(thread_vector[index].mutex));

				// no need for a lock on thread_vector.index_this].ok
				thread_vector[index_this].ok &= (rc == 0);
			}
			if( all_set )
				wait = false;
		}

		// convert thread_this to the corresponding thread index
		index = 0;
		for(index = 0; index < number_threads; index++)
		{	// pthread system unique identifier for this index
			pthread_t thread_compare = thread_vector[index].thread_id;

			// check for a match
			if( pthread_equal(thread_this, thread_compare) )
				return index;
		}
		// no match error (thread_this is not in thread_vector).
		assert(false);
		return index;
	}

	// ---------------------------------------------------------------------
	// in_parallel()
	bool in_parallel(void)
	{	return multiple_threads_active; }

	// ---------------------------------------------------------------------
	// thread_num()
	size_t thread_num(void)
	{	// the get call to set_get_thread_num	
		return set_get_thread_num(NUMBER_THREADS);
	}
	// ====================================================================
	// code for specific problem we are solving
	// --------------------------------------------------------------------
	using CppAD::AD;
	using CppAD::NearEqual;

	// vector with specific information for each thread
	typedef struct {
		// angle for this work 
		double          theta;
		// False if error related to this work,
		// true otherwise.
		bool            ok;
	} work_info_t;
	work_info_t work_vector[NUMBER_THREADS];
	// --------------------------------------------------------------------
	// function that we are computing the derivative of
	AD<double> arc_tan(const AD<double>& x, const AD<double>& y)
	{	double pi  = 4. * atan(1.);
		AD<double> theta;

		// valid for first quadrant 
		if( abs(x) > abs(y) )
			theta = atan(abs(y) / abs(x));
		else	theta = pi / 2. - atan(abs(x) / abs(y) ) ;

		// valid for first or second quadrant
		if( x < 0. )
			theta = pi - theta;

		// valid for any quadrant
		if( y < 0. )
			theta = - theta;

		return theta;
	}
	// --------------------------------------------------------------------
	// function that does the work for each thread
	void* thread_work(void* thread_info_vptr)
	{	bool ok = true;

		// ----------------------------------------------------------------
		// Setup for this thread

		// general purpose information for this thread
		thread_info_t* thread_info = 
			static_cast<thread_info_t*>(thread_info_vptr);

		// index to problem specific information for this thread
		size_t index = thread_info->thread_index;

		// Set put pthread unique identifier for this thread in
		// thread_vector[index].thread_id
		// and wait for other threads to do the same.
		set_get_thread_num(index);
		// ----------------------------------------------------------------
		// Work for this thread

		// CppAD::vector uses the CppAD fast multi-threading allocator
		CppAD::vector< AD<double> > Theta(1), Z(1);
		Theta[0] = work_vector[index].theta;
		Independent(Theta);
		AD<double> x = cos(Theta[0]);
		AD<double> y = sin(Theta[0]);
		Z[0]  = arc_tan( x, y );
		CppAD::ADFun<double> f(Theta, Z); 

		// Check function value corresponds to the identity 
		// (must get a lock before changing thread_info).
		double eps = 10. * CppAD::epsilon<double>();
		ok        &= NearEqual(Z[0], Theta[0], eps, eps);

		// Check derivative value corresponds to the identity.
		CppAD::vector<double> d_theta(1), d_z(1);
		d_theta[0] = 1.;
		d_z        = f.Forward(1, d_theta);
		ok        &= NearEqual(d_z[0], 1., eps, eps);

		// pass back ok information for this thread
		work_vector[index].ok &= ok;

		// It this is not the master thread, then terminate it.
# if DEMONSTRATE_BUG_IN_CYGWIN
		if( ! pthread_equal(
			thread_info->thread_id, thread_vector[0].thread_id) )
		{	void* no_status = 0;
			pthread_exit(no_status);
		}
# endif
	
		// return null pointer
		return 0;
	}
}

// This test routine is only called by the master thread (index = 0).
bool pthread_ad(void)
{	bool all_ok = true;
	using CppAD::AD;
	using CppAD::thread_alloc;

	// Check that no memory is in use or avialable at start
	// (using thread_alloc in sequential mode)
	size_t index;
	for(index = 0; index < number_threads; index++)
	{	all_ok &= thread_alloc::inuse(index) == 0; 
		all_ok &= thread_alloc::available(index) == 0; 
	}

	// initialize the thread_vector and work_vector
	int    rc;
 	double pi = 4. * atan(1.);
	for(index = 0; index < number_threads; index++)
	{	// mutex
		pthread_mutexattr_t* no_mutexattr = 0;
		rc = pthread_mutex_init(&(thread_vector[index].mutex), no_mutexattr);
		all_ok &= (rc == 0);
		// thread_id (initialze all as same as master thread)
		thread_vector[index].thread_id    = pthread_self();
		// thread_index
		thread_vector[index].thread_index = index;
		// ok
		thread_vector[index].ok           = true;
		work_vector[index].ok             = true;
		// theta 
		work_vector[index].theta          = index * pi / number_threads;
	}

	// Setup for using AD<double> in parallel mode
	thread_alloc::parallel_setup(number_threads, in_parallel, thread_num);
	CppAD::parallel_ad<double>();

	// Now change in_parallel() to return true.
	multiple_threads_active = true;
	
	// Data structure used by pthreads library.
	pthread_t      thread[NUMBER_THREADS];
	pthread_attr_t attr;
	void*          thread_info_vptr;
	//
	rc      = pthread_attr_init(&attr);
	all_ok &= (rc == 0);
	rc      = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	all_ok &= (rc == 0);

	// This master thread is already running, we need to create
	// number_threads - 1 more threads
	for(index = 1; index < number_threads; index++)
	{
		// Create the thread with thread_num equal to index
		thread_info_vptr = static_cast<void*> ( &(thread_vector[index]) );
		rc = pthread_create(
				&thread[index] , 	
				&attr              ,
				thread_work        ,
				thread_info_vptr
		);
		all_ok &= (rc == 0);
	}

	// Now, while other threads are working, do work in master thread also
	thread_info_vptr = static_cast<void*> ( &(thread_vector[0]) );
	thread_work(thread_info_vptr);

	// now wait for the other threads to finish
	for(index = 1; index < number_threads; index++)
	{	void* no_status = 0;
		rc      = pthread_join(thread[index], &no_status);
		all_ok &= (rc == 0);
	}

	// Summarize results.
	for(index = 0; index < number_threads; index++)
	{	all_ok &= thread_vector[index].ok;
		all_ok &= work_vector[index].ok;
	}

	// Free up the pthread resources that are no longer in use.
	rc      = pthread_attr_destroy(&attr);
	all_ok &= (rc == 0);
	for(index = 0; index < number_threads; index++)
	{	rc      = pthread_mutex_destroy(&(thread_vector[index].mutex));
		all_ok &= (rc == 0);
	}

	// Tell thread_alloc that we are in sequential execution mode.
	multiple_threads_active = false;

	// Check that no memory currently in use, and free avialable memory.
	for(index = 0; index < number_threads; index++)
	{	all_ok &= thread_alloc::inuse(index) == 0; 
		thread_alloc::free_available(index); 
	}

	// return memory allocator to single threading mode
	number_threads = 1;
	thread_alloc::parallel_setup(number_threads, in_parallel, thread_num);

	return all_ok;
}
// END PROGRAM
