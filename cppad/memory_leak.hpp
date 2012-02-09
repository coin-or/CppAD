/* $Id$ */
# ifndef CPPAD_MEMORY_LEAK_INCLUDED
# define CPPAD_MEMORY_LEAK_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin memory_leak$$
$spell
	num
	alloc
	hpp
	bool
	inuse
$$

$section Memory Leak Detection$$
$index memory_leak$$
$index leak, memory$$
$index check, memory leak$$
$index static, memory leak check$$

$head Syntax$$
$icode%flag% = %memory_leak()
%$$
$icode%flag% = %memory_leak(%set_static%)%$$

$head Purpose$$
This routine checks that the are no memory leaks 
caused by improper use of $cref thread_alloc$$ memory allocator.
The deprecated memory allocator $cref/TrackNewDel/$$ is also checked.
Memory errors in the deprecated $cref omp_alloc$$ allocator are
reported as being in $code thread_alloc$$.

$head set_static$$
Static variables hold onto memory forever.
If the argument $icode set_static$$ is present (and true),
$code memory_leak$$ sets the amount of memory that
is $cref/inuse/ta_inuse/$$ and corresponds to static variables in the program.
It therefore necessary to call to all the routines that
have static variables and that allocate memory using
$cref/get_memory/ta_get_memory/$$ before making a call
with $icode set_static$$ true.
It is also necessary to have no other $code inuse$$ memory when
$icode set_static$$ is called.
Since multiple statics may be allocated in different places in the program,
it is OK to use this option more that once.

$head flag$$
The return value $icode flag$$ has prototype
$codei%
	bool %flag%
%$$
If $icode set_static$$ is true,
the return value for $code memory_leak$$ is false.
Otherwise, the return value for $code memory_leak$$ should be false
(indicating that the only allocated memory corresponds to static variables).

$head inuse$$
It is assumed that, when $code memory_leak$$ is called,
there should not be any memory
$cref/inuse/ta_inuse/$$ or $cref omp_inuse$$ for any thread
(except for inuse memory corresponding to static variables).
If there is, a message is printed and $code memory_leak$$ returns false.

$head available$$
It is assumed that, when $code memory_leak$$ is called,
there should not be any memory
$cref/available/ta_available/$$ or $cref omp_available$$ for any thread;
i.e., it all has been returned to the system.
If there is memory still available for a thread,
$code memory_leak$$ returns false. 

$head TRACK_COUNT$$
It is assumed that, when $code memory_leak$$ is called,
$cref/TrackCount/TrackNewDel/TrackCount/$$ will return a zero value.
If it returns a non-zero value, 
$code memory_leak$$ returns false.

$head Error Message$$
If this is the first call to $code memory_leak$$, no message is printed.
Otherwise, if it returns true, an error message is printed
to standard output describing the memory leak that was detected.

$head Multi-Threading$$
This routine can only be called when the number of threads
$cref/num_threads/ta_num_threads/$$ is one.

$end
*/
# include <iostream>
# include <cppad/local/define.hpp>
# include <cppad/omp_alloc.hpp>
# include <cppad/thread_alloc.hpp>
# include <cppad/track_new_del.hpp>

CPPAD_BEGIN_NAMESPACE
/*!
\file memory_leak.hpp
File that implements a memory check at end of a CppAD program
*/

/*!
Function that checks 
allocator \c thread_alloc for misuse that results in memory leaks.
Deprecated routines in track_new_del.hpp and omp_alloc.hpp are also checked.

\return
If this is the first call to \c memory_leak, the return value is \c true
and the amount of <code>thread_alloc::inuse()</code> memory corresponding
to thread zero is set.

If this not the first call, if the return value is \c true,
if thread zero does not have the expected amount of inuse memory,
if one of the other threads has any inuse memory,
or any thread has available memory.

returns \c true, if no error is detected and \c false otherwise.

\par
If an error is detected, diagnostic information is printed to standard
output.
*/
inline bool memory_leak(bool set_static = false)
{	// CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL not necessary given asserts below
	static size_t thread_zero_static_inuse     = 0;
	using std::cout;
	using std::endl;
	using CppAD::thread_alloc;
	using CppAD::omp_alloc;
	// --------------------------------------------------------------------
	// check thead_alloc
	CPPAD_ASSERT_KNOWN(
		! thread_alloc::in_parallel(),
		"attempt to use thread_leak in parallel execution mode."
	);
	CPPAD_ASSERT_KNOWN(
		! thread_alloc::in_parallel(),
		"attempt to use thread_leak in parallel execution mode."
	);
	CPPAD_ASSERT_KNOWN(
		thread_alloc::num_threads() == 1,
		"attempt to use thread_leak while num_threads > 1."
	);
	if( set_static )
	{	thread_zero_static_inuse  = thread_alloc::inuse(0);
		return true;
	}
	bool leak     = false;
	size_t thread = 0;

	// check that memory in use for thread zero corresponds to statics
	size_t num_bytes = thread_alloc::inuse(thread);
	if( num_bytes != thread_zero_static_inuse )
	{	leak = true;
		cout << "thread zero: static inuse = " << thread_zero_static_inuse;
		cout << "current inuse(thread)     = " << num_bytes << endl;
	}
	// check that no memory is currently available for this thread
	num_bytes = thread_alloc::available(thread);
	if( num_bytes != 0 )
	{	leak = true;
		cout << "thread zero: available    = ";
		cout << num_bytes << endl;
	}
	for(thread = 1; thread < CPPAD_MAX_NUM_THREADS; thread++)
	{
		// check that no memory is currently in use for this thread
		num_bytes = thread_alloc::inuse(thread);
		if( num_bytes != 0 )
		{	leak = true;
			cout << "thread " << thread << ": inuse(thread) = ";
			cout << num_bytes << endl;
		}
		// check that no memory is currently available for this thread
		num_bytes = thread_alloc::available(thread);
		if( num_bytes != 0 )
		{	leak = true;
			cout << "thread " << thread << ": available(thread) = ";
			cout << num_bytes << endl;
		}
	}
	// ----------------------------------------------------------------------
	// check track_new_del
	if( CPPAD_TRACK_COUNT() != 0 )
	{	leak = true;
		CppAD::TrackElement::Print();
	}
	return leak;
}

CPPAD_END_NAMESPACE
# endif
