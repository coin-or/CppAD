# ifndef CPPAD_TAPE_LINK_INCLUDED
# define CPPAD_TAPE_LINK_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin tape_link$$ $comment CppAD Developer Documentation$$

$section Routines that Link AD<Base> and ADTape<Base> Objects$$
$spell
	omp
	bool
	ptr
	const
	tbl
	atexit
$$

$head tape_this$$
$index tape_this$$
The syntax
$syntax%
	%ptr% = %x%.tape_this()
%$$
returns the tape corresponding to $italic x$$
where $italic x$$ and $italic ptr$$ have the prototypes
$syntax%
	const AD<%Base%> &%x%
	ADTape<%Base%>  *%ptr%
%$$
The argument $italic x$$ must a variable.

$head id_handle$$
$index id_handle$$

$subhead _OPENMP Defined$$
In the case where the preprocessor symbol $code _OPENMP$$ is defined,
the syntax
$syntax%
	%id% = AD<%Base%>::id_handle(%thread%)
%$$
returns a pointer to the currently active tape identifier
for the specified OpenMP thread.
The initial value, for the tape identifier is zero.
This way it is not equal to the default $code id_ = 1$$ value
in each $syntax%AD<%Base%>%$$ object.
The argument $italic thread$$ and the return value $italic id$$ have prototype
$syntax%
	size_t  %thread%
	size_t *%id%
%$$
Either $syntax%*id_handle(%thread%)%$$ is zero (its initial value) or
the current thread number satisfies the equation
$syntax/
	/thread/ == *id_handle(/thread/) % CPPAD_MAX_NUM_THREADS
/$$
(This is a restriction on how $syntax%*id_handle(%thread%)%$$ may be changed.)

$subhead _OPENMP Not Defined$$
In the case where the preprocessor symbol $code _OPENMP$$ is not defined,
the syntax
$syntax%
	%id% = AD<%Base%>::id_handle()
%$$
returns a pointer to the currently active tape identifier.
The initial value, for the tape identifier is zero.
This way it is not equal to the default $code id_ = 1$$ value
in each $syntax%AD<%Base%>%$$ object.
The return value $italic id$$ has prototype
$syntax%
	size_t *%id%
%$$


$head tape_handle$$
$index tape_handle$$

$head _OPENMP Defined$$
In the case where the preprocessor symbol $code _OPENMP$$ is defined,
the syntax
$syntax%
	%tape% = AD<%Base%>::tape_handle(%thread%)
%$$
returns a pointer to a pointer to the current tape
for the specified OpenMP thread.
The argument $italic thread$$ and 
the return value $italic type$$ have prototype
$syntax%%
	size_t           thread
	ADTape<%Base%> **tape
%$$.
If the tape is currently active,
$syntax%
	*%tape% != CPPAD_NULL
%$$

$head _OPENMP Not Defined$$
In the case where the preprocessor symbol $code _OPENMP$$ is not defined,
the syntax
$syntax%
	%tape% = AD<%Base%>::tape_handle()
%$$
returns a pointer to a pointer to the current tape.
The return value $italic tape$$ has prototype
$syntax%%
	ADTape<%Base%> **tape
%$$.
If the tape is currently active,
$syntax%
	*%tape% != CPPAD_NULL
%$$

$head tape_new$$
$index tape_new$$
The syntax
$syntax%
	%id% = AD<%Base%>::tape_new()
%$$
creates a new tape and returns the corresponding tape identifier.
The resulting tape identifier is not equal to zero, one, or to any
of the previous values return by $code tape_new$$.
In addition, the current thread number is given by
$syntax/
	/thread/ = /id/ % CPPAD_MAX_NUM_THREADS
/$$
The return value $italic id$$ has prototype
$syntax%
	size_t %id%
%$$
This function is only called by the user $code Independent$$ routine
and hence usage errors can be reported as coming from that routine.

$head tape_delete$$
$index tape_delete$$
The syntax
$syntax%
	AD<%Base%>::tape_delete(%id%)
%$$
deletes the tape corresponding to the tape identifier $italic id$$.
The value returned by $code *id_handle()$$ for this thread
is set to a value larger than any previous value returned by
$code tape_new()$$.


$head tape_ptr$$
$index tape_ptr$$
The syntax
$syntax%
	%ptr% = AD<%Base%>::tape_ptr()
%$$
returns the a pointer to the tape corresponding to the current thread.
The corresponding tape is active if and only if $syntax%%ptr% == CPPAD_NULL%$$.
The syntax
$syntax%
	%ptr% = AD<%Base%>::tape_ptr(%id%)
%$$
does the same thing but if NDEBUG is not defined, it also check that
the $italic id$$ is the corresponding tape identifier and that $italic ptr$$
is not equal to $code CPPAD_NULL$$.
The argument $italic id$$ has prototype
$syntax%
	size_t %id%
%$$


$end
----------------------------------------------------------------------------
*/

# ifdef _OPENMP
# include <omp.h>
# endif

// BEGIN CppAD namespace
namespace CppAD { 

// ----------------------------------------------------------------------
template <class Base>
inline ADTape<Base> *AD<Base>::tape_this(void) const
{	
# ifdef _OPENMP
	size_t thread = id_ % CPPAD_MAX_NUM_THREADS;
	CppADUnknownError( id_ == *id_handle(thread) );
	CppADUnknownError( *tape_handle(thread) != CPPAD_NULL );
	return *tape_handle(thread);
# else
	CppADUnknownError( id_ == *id_handle() );
	CppADUnknownError( *tape_handle() != CPPAD_NULL );
	return *tape_handle();
# endif

}
// ----------------------------------------------------------------------
// Static functions
//
# ifdef _OPENMP
template <class Base>
inline size_t * AD<Base>::id_handle(size_t thread)
{	static size_t id_table[CPPAD_MAX_NUM_THREADS];
	CppADUnknownError( 
		(id_table[thread] == 0)
		| (id_table[thread] % CPPAD_MAX_NUM_THREADS == thread)
	); 
	return id_table + thread;
}
template <class Base>
inline ADTape<Base> ** AD<Base>::tape_handle(size_t thread)
{	static ADTape<Base> *tape_table[CPPAD_MAX_NUM_THREADS];
	return tape_table + thread;
}
# else
template <class Base>
inline size_t * AD<Base>::id_handle(void)
{	static size_t id;
	return &id;
}
template <class Base>
inline ADTape<Base> ** AD<Base>::tape_handle(void)
{	static ADTape<Base> *tape;
	return &tape;
}
# endif

template <class Base>
size_t  AD<Base>::tape_new(void)
{
# ifdef _OPENMP
	size_t thread       = static_cast<size_t> ( omp_get_thread_num() );
	size_t *id          = id_handle(thread);
	ADTape<Base> **tape = tape_handle(thread);
# else
	size_t       thread = 0;
	size_t          *id = id_handle();
	ADTape<Base> **tape = tape_handle();
# endif
	CppADUsageError(
	thread < omp_max_thread(0),
	"Independent: OpenMP thread number is >= omp_max_thread setting"
	);

	// initialize so that id > 1 and thread == id % CPPAD_MAX_NUM_THREADS
	if( *id == 0 )
		*id = thread + 2 * CPPAD_MAX_NUM_THREADS;

	// tape for this thread must be null at the start
	CppADUnknownError( *tape  == CPPAD_NULL );
	*tape = new ADTape<Base>( *id );

	return *id;
}

template <class Base>
void  AD<Base>::tape_delete(size_t id_old)
{
# ifdef _OPENMP
	size_t thread = id_old % CPPAD_MAX_NUM_THREADS;
	CppADUsageError(
	thread == static_cast<size_t> ( omp_get_thread_num() ),
	"AD tape recording must stop in same thread as it started in."
	);
	size_t        *id   = id_handle(thread);
	ADTape<Base> **tape = tape_handle(thread);
# else
	size_t        *id   = id_handle();
	ADTape<Base> **tape = tape_handle();
# endif
	CppADUnknownError( *id   == id_old     );
	CppADUnknownError( *tape != CPPAD_NULL );

	// increase the id for this thread in a way such that 
	// thread = id % CPPAD_MAX_NUM_THREADS
	*id  += CPPAD_MAX_NUM_THREADS;

	// delete the old tape for this thread
	delete ( *tape );
	*tape = CPPAD_NULL;

	return;
}
template <class Base>
inline ADTape<Base> *AD<Base>::tape_ptr(void)
{
# ifdef _OPENMP
	size_t thread = static_cast<size_t> ( omp_get_thread_num() );
	return *tape_handle(thread); 
# else
	return *tape_handle();
# endif
}

template <class Base>
inline ADTape<Base> *AD<Base>::tape_ptr(size_t id)
{
# ifdef _OPENMP
	size_t thread = id % CPPAD_MAX_NUM_THREADS;
	CppADUsageError(
	thread == static_cast<size_t> ( omp_get_thread_num() ),
	"AD tape recording must stop in same thread as it started in."
	);
	CppADUnknownError( id == *id_handle(thread) );
	CppADUnknownError( *tape_handle(thread) != CPPAD_NULL );
	return *tape_handle(thread); 
# else
	CppADUnknownError( id == *id_handle() );
	CppADUnknownError( *tape_handle() != CPPAD_NULL );
	return *tape_handle(); 
# endif
}

} // END CppAD namespace

# endif
