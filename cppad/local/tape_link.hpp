/* $Id$ */
# ifndef CPPAD_TAPE_LINK_INCLUDED
# define CPPAD_TAPE_LINK_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/local/define.hpp>
# include <cppad/thread_alloc.hpp>
# include <cppad/local/cppad_assert.hpp>

// needed before one can use CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/thread_alloc.hpp>

CPPAD_BEGIN_NAMESPACE
/*!
\file tape_link.hpp
Routines that Link AD<Base> and ADTape<Base> Objects \ref tape_link_hpp.

The routines that connect the AD<Base> class to the corresponding tapes
(one for each thread).

\defgroup tape_link_hpp  tape_link.hpp
*/
/* \{ */


/*!
Pointer to the tape identifier for this AD<Base> class and the specific thread.

\tparam Base
is the base type for this AD<Base> class.

\param thread
is the thread number
\code
0 <= thread == thread_alloc::thread_num()
\endcode

\return
is a pointer to the tape identifier for this thread
and AD<Base> class.
*/
template <class Base>
inline size_t* AD<Base>::tape_id_ptr(size_t thread)
{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
	// Section 3.6.2 of the 1998 C++ standard specifies that storage for
	// static objects will be zero initalized.
	static size_t tape_id_table[CPPAD_MAX_NUM_THREADS];
	CPPAD_ASSERT_UNKNOWN( thread == thread_alloc::thread_num() );

	return tape_id_table + thread;
}

/*!
Handle for the tape for this AD<Base> class and the specific thread.

\tparam Base
is the base type for this AD<Base> class.

\param thread
is the thread number
\code
0 <= thread == thread_alloc::thread_num()
\endcode

\return
is a handle for the  AD<Base> class and the specified thread.
*/
template <class Base>
inline ADTape<Base>** AD<Base>::tape_handle(size_t thread)
{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
	static ADTape<Base>* tape_table[CPPAD_MAX_NUM_THREADS];
	CPPAD_ASSERT_UNKNOWN( thread < thread_alloc::num_threads() );
	return tape_table + thread;
}

/*!
Pointer for the tape for this AD<Base> class and the current thread.

\code
thread == thread_alloc::thread_num()
\endcode

\tparam Base
is the base type corresponding to AD<Base> operations.

\return
is a pointer to the tape that is currently recording AD<Base> operations
for the current thread.
If this value is \c CPPAD_NULL, there is no tape currently
recording AD<Base> operations for this thread.
*/
template <class Base>
inline ADTape<Base>* AD<Base>::tape_ptr(void)
{	size_t thread = thread_alloc::thread_num();
	return *tape_handle(thread); 
}

/*!
Pointer for the tape for this AD<Base> class and the specified tape 
identifier.

\tparam Base
is the base type corresponding to AD<Base> operations.

\param tape_id
is the identifier for the tape that is currently recording
AD<Base> operations for the current thread.
It must hold that the current thread is
\code
	thread = tape_id % CPPAD_MAX_NUM_THREADS
\endcode
and that there is a tape recording AD<Base> operations 
for this thread.
If this is not the currently executing thread, 
a variable from a different thread is being recorded on the
tape for this thread which is a user error.

\return
is a pointer to the tape that is currently recording AD<Base> operations
for the current thread (and it is not \c CPPAD_NULL).
*/
template <class Base>
inline ADTape<Base>* AD<Base>::tape_ptr(size_t tape_id)
{	size_t thread = tape_id % CPPAD_MAX_NUM_THREADS;
	CPPAD_ASSERT_KNOWN(
		thread == thread_alloc::thread_num(),
		"Attempt to use an AD variable with two different threads."
	);
	CPPAD_ASSERT_UNKNOWN( tape_id == *tape_id_ptr(thread) );
	CPPAD_ASSERT_UNKNOWN( *tape_handle(thread) != CPPAD_NULL );
	return *tape_handle(thread); 
}

/*!
Create a new tape that records AD<Base> operations for current thread

\par thread
the current thread is given by
\code
thread = thread_alloc::thread_num()
\endcode

\tparam Base
is the base type corresponding to AD<Base> operations.

\par tape_handle
It is assumed that
\code
*tape_handle(thread) == CPPAD_NULL
\endcode
when \c tape_new is called; i.e., there is no tape currently recording
AD<Base> operations for the current thread.

\par tape_id_ptr
The value of \c tape_id_ptr is modified in the following way:
\code
*tape_id_ptr(thread) = *tape_id_ptr(thread) + CPPAD_MAX_NUM_THREADS
\endcode
The new tape identifier is the identifier for the newly created tape.

\return
the return value <tt>*tape_handle(thread)</tt>
is a ponter to a newly created tape for recording AD<Base> operations 
by the current thread. 
The corresponding tape identifier is <tt>*tape_id_ptr(thread)</tt>
If the input value of this identifier is zero,
it is set to <tt>thread + 2 * CPPAD_MAX_NUM_THREADS</tt>.
Otherwise, this value is not modified by \c tape_new.
*/
template <class Base>
ADTape<Base>*  AD<Base>::tape_new(void)
{
	size_t thread       = thread_alloc::thread_num();
	size_t* tape_id     = tape_id_ptr(thread);
	ADTape<Base>** tape = tape_handle(thread);
	CPPAD_ASSERT_KNOWN(
		thread < thread_alloc::num_threads(),
		"Independent: current thread_num is greater than num_threads;\n"
		"See parallel_setup for setting these values."
	);

	// initialize so that id > 1 and thread == id % CPPAD_MAX_NUM_THREADS
	if( *tape_id == 0 )
		*tape_id = thread + 2 * CPPAD_MAX_NUM_THREADS;
	// else *tape_id has been set to its new value by tape_delete

	// tape for this thread must be null at the start
	CPPAD_ASSERT_UNKNOWN( *tape  == CPPAD_NULL );
	*tape = new ADTape<Base>( *tape_id );

	return *tape;
}

/*!
Delete the new tape that was recording AD<Base> operations for current thread

\par thread
the current thread must be
\code
	thread = tape_id % CPPAD_MAX_NUM_THREADS
\endcode

\tparam Base
is the base type corresponding to AD<Base> operations.

\par tape_handle
It is assumed that
\code
*tape_handle(thread) != CPPAD_NULL
\endcode
when \c tape_new is called; i.e., there is a tape currently recording
AD<Base> operations for the current thread.

\param tape_id
this must be the identifier for the tape that is recording 
AD<Base> operations for the current thread
(and it is also the value of <tt>*tape_id_ptr(thread)</tt>
when this routine is called.
The new value for this threads tape identifier and pointer are 
\code
*tape_id_ptr(thread) = *tape_id_ptr(thread) + CPPAD_MAX_NUM_THREADS
*tape_handle(thread) = CPPAD_NULL
\endcode
*/
template <class Base>
void  AD<Base>::tape_delete(size_t tape_id)
{
	size_t thread        = tape_id % CPPAD_MAX_NUM_THREADS;
	ADTape<Base>** tape  = tape_handle(thread);
	CPPAD_ASSERT_KNOWN(
		thread == thread_alloc::thread_num(),
		"AD tape recording must stop in same thread as it started in."
	);
	CPPAD_ASSERT_UNKNOWN( *tape != CPPAD_NULL );
	CPPAD_ASSERT_UNKNOWN( tape_id == *tape_id_ptr(thread)  );
	CPPAD_ASSERT_UNKNOWN( tape_id == (*tape)->id_ );

	CPPAD_ASSERT_KNOWN(size_t(std::numeric_limits<CPPAD_TAPE_ID_TYPE>::max())
		- CPPAD_MAX_NUM_THREADS > tape_id,
		"To many different tapes given the type used for CPPAD_TAPE_ID_TYPE"
	);
	// advance tape identifier
	*tape_id_ptr(thread)  += CPPAD_MAX_NUM_THREADS;
	// delete the old tape for this thread
	delete ( *tape );
	*tape = CPPAD_NULL;

	return;
}

/*!
Get a pointer to tape that records AD<Base> operations for the current thread.

\tparam Base
is the base type corresponding to AD<Base> operations.

\par thread
The current thread must be given by
\code
	thread = this->tape_id_ % CPPAD_MAX_NUM_THREADS
\endcode

\return
is a pointer to the tape that is currently recording AD<Base> operations
for the current thread.
This value must not be \c CPPAD_NULL; i.e., there must be a tape currently
recording AD<Base> operations for this thread.
*/

template <class Base>
inline ADTape<Base> *AD<Base>::tape_this(void) const
{	
	size_t thread = tape_id_ % CPPAD_MAX_NUM_THREADS;
	CPPAD_ASSERT_UNKNOWN( tape_id_ == *tape_id_ptr(thread) );
	CPPAD_ASSERT_UNKNOWN( *tape_handle(thread) != CPPAD_NULL );
	return *tape_handle(thread);
}

/* \} */
CPPAD_END_NAMESPACE
# endif
