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
is the thread number; i.e.,
\code
thread == thread_alloc::thread_num()
\endcode
If this condition is not satisfied, and \c NDEBUG is not defined,
a CPPAD_ASSERT_UNKNOWN is generated.

\return
is a pointer to the tape identifier for this thread
and AD<Base> class.
*/
template <class Base>
inline size_t* AD<Base>::tape_id_ptr(size_t thread)
{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
	static size_t tape_id_table[CPPAD_MAX_NUM_THREADS];
	CPPAD_ASSERT_UNKNOWN( thread == thread_alloc::thread_num() );

	return tape_id_table + thread;
}

/*!
Handle for the tape for this AD<Base> class and the specific thread.

\tparam Base
is the base type for this AD<Base> class.


\param thread
is the thread number; i.e.,
\code
thread == thread_alloc::thread_num()
\endcode
If this condition is not satisfied, and \c NDEBUG is not defined,
a CPPAD_ASSERT_UNKNOWN is generated.

\return
is a handle for the  AD<Base> class and the specified thread.
*/
template <class Base>
inline ADTape<Base>** AD<Base>::tape_handle(size_t thread)
{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
	static ADTape<Base>* tape_table[CPPAD_MAX_NUM_THREADS];
	CPPAD_ASSERT_UNKNOWN( thread == thread_alloc::thread_num() );

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
Create and delete tapes that record AD<Base> operations for current thread.

\par thread
the current thread is given by
\code
thread = thread_alloc::thread_num()
\endcode

\tparam Base
is the base type corresponding to AD<Base> operations.

\param job
This argument determines if we are creating a new tape, or deleting an
old one.
- \c tape_manage_new :
Creates and a new tape. 
It is assumed that there is no tape recording AD<Base> operations
for this thread when \c tape_manate is called.
It the input value of <tt>*tape_id_ptr(thread)</tt> is zero,
it will be changed to <tt>thread + CPPAD_MAX_NUM_THREADS</tt>.
The id for the new tape will be <tt>*tape_id_ptr(thread)</tt>.
- \c tape_manage_delete :
It is assumed that there is a tape recording AD<Base> operations
for this thread when \c tape_manage is called.
The value of <tt>*tape_id_ptr(thread)</tt> will be advanced by
\c CPPAD_MAX_NUM_THREADS.


\return
- <tt>job == tape_manage_new</tt>: a pointer to the new tape is returned.
- <tt>job == tape_manage_delete</tt>: the value \c CPPAD_NULL is returned.
*/
template <class Base>
ADTape<Base>*  AD<Base>::tape_manage(tape_manage_job job)
{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
	static ADTape<Base> tape_table[CPPAD_MAX_NUM_THREADS];

	size_t thread       = thread_alloc::thread_num();
	size_t* tape_id     = tape_id_ptr(thread);
	ADTape<Base>** tape = tape_handle(thread);

	// initialize so that id > 0 and thread == id % CPPAD_MAX_NUM_THREADS
	if( *tape_id == 0 )
		*tape_id = thread + CPPAD_MAX_NUM_THREADS;
	CPPAD_ASSERT_UNKNOWN( *tape_id % CPPAD_MAX_NUM_THREADS == thread );

	switch(job)
	{	case tape_manage_new:
		// tape for this thread must be null at the start
		CPPAD_ASSERT_UNKNOWN( *tape  == CPPAD_NULL );
		tape_table[thread].id_ = *tape_id;
		*tape = tape_table + thread;
		break;

		case tape_manage_delete:
		CPPAD_ASSERT_UNKNOWN( *tape  == tape_table + thread );
		CPPAD_ASSERT_UNKNOWN( *tape_id == (*tape)->id_ );
		CPPAD_ASSERT_KNOWN(
			size_t( std::numeric_limits<CPPAD_TAPE_ID_TYPE>::max() )
			- CPPAD_MAX_NUM_THREADS > *tape_id,
			"To many different tapes given the type used for "
			"CPPAD_TAPE_ID_TYPE"
		);
		// advance tape identfier so all AD<Base> variables become parameters
		*tape_id  += CPPAD_MAX_NUM_THREADS;
		// free memory corresponding to recording in the old tape 
		tape_table[thread].Rec_.free();
		// inform rest of CppAD that no tape recording for this thread
		*tape = CPPAD_NULL;
	}
	return *tape;
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
