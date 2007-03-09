# ifndef CPPAD_TRACK_NEW_DEL_INCLUDED
# define CPPAD_TRACK_NEW_DEL_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin TrackNewDel$$
$spell
	cppad.hpp
	Cpp
	newptr
	Vec
	oldptr
	newlen
	ncopy
	const
$$

$section Routines That Track Use of New and Delete$$
$index new, track$$
$index delete, track$$
$index track, new and delete$$
$index memory, track$$

$head Syntax$$
$syntax%# include <cppad/track_new_del.hpp>
%$$
$syntax%%newptr% = TrackNewVec(%file%, %line%, %newlen%, %oldptr%)%$$
$pre
$$
$syntax%TrackDelVec(%file%, %line%, %oldptr%)%$$
$pre
$$
$syntax%%newptr% = TrackExtend(%file%, %line%, %newlen%, %ncopy%, %oldptr%)%$$
$pre
$$
$syntax%%count% = TrackCount(%file%, %line%)%$$


$head Purpose$$
These routines 
aid in the use of $code new[]$$ and  $code delete[]$$
during the execution of a C++ program.

$head Include$$
The file $code cppad/track_new_del.hpp$$ is included by 
$code cppad/cppad.hpp$$
but it can also be included separately with out the rest of the 
CppAD include files.


$head file$$
The argument $italic file$$ has prototype
$syntax%
	const char *%file%
%$$
It should be the source code file name 
where the call to $code TrackNew$$ is located.
The best way to accomplish this is the use the preprocessor symbol
$code __FILE__$$ for this argument.

$head line$$
The argument $italic line$$ has prototype
$syntax%
	int %line%
%$$
It should be the source code file line number 
where the call to $code TrackNew$$ is located.
The best way to accomplish this is the use the preprocessor symbol
$code __LINE__$$ for this argument.

$head oldptr$$
The argument $italic oldptr$$ has prototype
$syntax%
	%Type% *%oldptr%
%$$
This argument is used to identify the type $italic Type$$.

$subhead OpenMP$$
$index OpenMP, TrackNewDel$$
$index TrackNewDel, OpenMP$$ 
In the case of multi-threading with OpenMP,
calls with the argument $italic oldptr$$ must be made with the
same thread as when $italic oldptr$$ was created
(except for $code TrackNewVec$$ where the value of $italic oldptr$$
does not matter).


$head newlen$$
The argument $italic newlen$$ has prototype
$syntax%
	size_t %newlen%
%$$

$head head newptr$$
The return value $italic newptr$$ has prototype
$syntax%
	%Type% *%newptr%
%$$
It points to the newly allocated vector of objects
that were allocated using
$syntax%
	new Type[%newlen%]
%$$

$head ncopy$$
The argument $italic ncopy$$ has prototype
$syntax%
        size_t %ncopy%
%$$
This specifies the number of elements that are copied from
the old array to the new array.
The value of $italic ncopy$$ 
must be less than or equal $italic newlen$$.

$head TrackNewVec$$
$index TrackNewVec$$
$index NDEBUG$$
This routine is used to start the tracking of memory allocation 
using $code new[]$$.
The value of $italic oldptr$$ does not matter for this case
(except that it is used to identify $italic Type$$).
If $code NDEBUG$$ is not defined and the memory cannot be allocated,
$xref/ErrorHandler/$$ is used to generate a message
stating that there was not sufficient memory.
The preprocessor macro call
$syntax%
	CppADTrackNewVec(%newlen%, %oldptr%)
%$$
expands to
$syntax%
	CppAD::TrackNewVec(__FILE__, __LINE__, %newlen%, %oldptr%)
%$$


$head TrackDelVec$$
$index TrackDelVec$$
This routine is used to a vector of objects 
that have been allocated using $code TrackNew$$ or $code TrackExtend$$.
If $code NDEBUG$$ is not defined, $code TrackDelete$$ check that
$italic oldptr$$ was allocated by $code TrackNew$$ or $code TrackExtend$$
and has not yet been freed.
If this is not the case,
$xref/ErrorHandler/$$ is used to generate an error message.
The preprocessor macro call
$syntax%
	CppADTrackDelVec(%oldptr%)
%$$
expands to
$syntax%
	CppAD::TrackDelVec(__FILE__, __LINE__, %oldptr%)
%$$

$head TrackExtend$$
$index TrackExtend$$
This routine is used to 
allocate a new vector (using $code TrackNewVec$$),
and copy $italic ncopy$$ elements from the old vector to the new vector.
The preprocessor macro call
$syntax%
	CppADTrackExtend(%newlen%, %ncopy%, %oldptr%)
%$$
expands to
$syntax%
	CppAD::TrackExtend(__FILE__, __LINE__, %newlen%, %ncopy%, %oldptr%)
%$$
If $italic ncopy$$ is greater than zero, $italic oldptr$$ 
must have been allocated using $code TrackNewVec$$ or $code TrackExtend$$.
In this case, the vector pointed to by $italic oldptr$$ 
must be have at least $italic ncopy$$ elements
and it will be freed (using $code TrackDelVec$$).

$head TrackCount$$
$index TrackCount$$
The return value $italic count$$ has prototype
$syntax%
	size_t %count%
%$$
If $code NDEBUG$$ is defined, $italic count$$ will be zero.
Otherwise, it will be
the number of vectors that 
have been allocated
(by $code TrackNewVec$$ or $code TrackExtend$$)
and not yet freed
(by $code TrackDelete$$).
$syntax%
	CppADTrackCount()
%$$
expands to
$syntax%
	CppAD::TrackCount(__FILE__, __LINE__)
%$$

$subhead OpenMP$$
$index OpenMP, TrackCount$$
$index TrackCount, OpenMP$$
In the case of multi-threading with OpenMP,
the information for all of the threads is checked 
so only one thread can be running
when this routine is called.


$head Example$$
$children%
	example/track_new_del.cpp
%$$
The file $xref/TrackNewDel.cpp/$$
contains an example and test of these functions.
It returns true, if it succeeds, and false otherwise.

$end
------------------------------------------------------------------------------
*/
# include <cppad/local/cppad_error.hpp>
# include <sstream>
# include <string>

# ifdef _OPENMP
# include <omp.h>
# endif


# ifndef CPPAD_NULL
# define CPPAD_NULL	0
# endif

# ifndef CPPAD_MAX_NUM_THREADS
# ifdef _OPENMP
# define CPPAD_MAX_NUM_THREADS 32
# else
# define CPPAD_MAX_NUM_THREADS 1
# endif
# endif

# define CPPAD_TRACK_DEBUG 0

# define CppADTrackNewVec(newlen, oldptr) \
	CppAD::TrackNewVec(__FILE__, __LINE__, newlen, oldptr)

# define CppADTrackDelVec(oldptr) \
	CppAD::TrackDelVec(__FILE__, __LINE__, oldptr)

# define CppADTrackExtend(newlen, ncopy, oldptr) \
	CppAD::TrackExtend(__FILE__, __LINE__, newlen, ncopy, oldptr)

# define CppADTrackCount() \
	CppAD::TrackCount(__FILE__, __LINE__)

namespace CppAD { // Begin CppAD namespace

// TrackElement ------------------------------------------------------------
class TrackElement {
	
public:
	std::string   file;   // corresponding file name
	int           line;   // corresponding line number
	void          *ptr;   // value returned by TrackNew
	TrackElement *next;   // next element in linked list

	// default contructor (used to initialize root)
	TrackElement(void)
	: file(""), line(0), ptr(CPPAD_NULL), next(CPPAD_NULL)
	{ }
	
	TrackElement(const char *f, int l, void *p)
	: file(f), line(l), ptr(p), next(CPPAD_NULL)
	{	CppADUnknownError( p != CPPAD_NULL);
	}

	// There is only one tracking list and it starts it here
	static TrackElement *root_for(size_t thread)
	{	static TrackElement root[CPPAD_MAX_NUM_THREADS];
		CppADUnknownError( thread < CPPAD_MAX_NUM_THREADS );
		return root + thread;
	}

	// There is only one tracking list and it starts it here
	static TrackElement *Root(void)
	{
# ifdef _OPENMP
		size_t thread = static_cast<size_t> ( omp_get_thread_num() );
# else
		size_t thread = 0;
# endif
		CppADUsageError(
			thread < CPPAD_MAX_NUM_THREADS,
			"TrackNewDel: too many OpenMP threads are active."
		);
		return root_for(thread); 
	}

	// Print the linked list
	static void Print(size_t thread)
	{	using std::cout;
		using std::endl;
		TrackElement *E = Root();
		cout << "Begin Track List for thread " << thread << endl;
		while( E->next != CPPAD_NULL )
		{	E = E->next;
			cout << "next = " << E->next;
			cout << ", ptr  = " << E->ptr;
			cout << ", line = " << E->line;
			cout << ", file = " << E->file;
			cout << endl;
		}
		cout << "End Track List for thread " << thread << endl;
		cout << endl;
	}
}; 


// TrackError ----------------------------------------------------------------
inline void TrackError(
	const char *routine,
	const char *file,
	int         line,
	const char *msg )
{
	std::ostringstream buf;
	buf << routine
	    << ": at line "
	    << line
	    << " in file "
	    << file
	    << std::endl
	    << msg; 
	std::string str = buf.str();
	size_t      n   = str.size();
	size_t i;
	char *message = new char[n + 1];
	for(i = 0; i < n; i++)
		message[i] = str[i];
	message[n] = '\0';
	CppADUsageError( false , message);
}

// TrackNewVec ---------------------------------------------------------------
# ifdef NDEBUG
template <class Type>
inline Type *TrackNewVec(
	const char *file, int line, size_t len, Type * /* oldptr */ )
{	return (new Type[len]); 
}

# else

template <class Type>
Type *TrackNewVec(
	const char *file          , 
	int         line          , 
	size_t      len           ,
	Type       * /* oldptr */ )
{
	// try to allocate the new memrory
	Type *newptr = CPPAD_NULL;
	try
	{	newptr = new Type[len];
	}
	catch(...)
	{	TrackError("TrackNewVec", file, line, 
			"Cannot allocate sufficient memory"
		);
	}
	// create tracking element
	void *vptr = static_cast<void *>(newptr);
	TrackElement *E = new TrackElement(file, line, vptr);

	// get the root
	TrackElement *root = TrackElement::Root();

	// put this elemenent at the front of linked list
	E->next    = root->next;  
	root->next = E;

	return newptr;
}

# endif

// TrackDelVec --------------------------------------------------------------
# ifdef NDEBUG
template <class Type>
inline void TrackDelVec(const char *file, int line, Type *oldptr)
{	 delete [] oldptr; 
}

# else

template <class Type>
void TrackDelVec(
	const char *file    ,
	int         line    ,
	Type       *oldptr  )
{
	TrackElement        *P;
	TrackElement        *E;

	// search list for pointer
	P          = TrackElement::Root();
	E          = P->next;
	void *vptr = static_cast<void *>(oldptr);
	while(E != CPPAD_NULL && E->ptr != vptr)
	{	P = E;
		E = E->next;
	}

	// check if pointer was not in list
	if( E == CPPAD_NULL || E->ptr != vptr ) TrackError(
		"TrackDelVec", file, line, 
		"Invalid value for the argument oldptr"
	); 

	// remove tracking element from list
	P->next = E->next;

	// delete allocated pointer
	delete [] oldptr;

	// delete tracking element
	delete E;

	return;
}

# endif

// TrackExtend --------------------------------------------------------------
template <class Type>
Type *TrackExtend(
	const char *file    , 
	int         line    , 
	size_t      newlen  , 
	size_t      ncopy   ,
	Type       *oldptr  ) 
{	// check size of ncopy
	CppADUsageError( 
		ncopy <= newlen,
		"TrackExtend: ncopy is greater than newlen."
	);

	// allocate the new memrory
	Type *newptr = TrackNewVec(file, line, newlen, oldptr);

	// copy the data
	size_t i = ncopy;
	while(i)
	{	--i;
		newptr[i] = oldptr[i];
	}

	// delete the old vector 
	if( ncopy > 0 )
		TrackDelVec(file, line, oldptr);

	return newptr;
}

// TrackCount --------------------------------------------------------------
inline size_t TrackCount(const char *file, int line)
{
	size_t count = 0;
	size_t thread;
	for(thread = 0; thread < CPPAD_MAX_NUM_THREADS; thread++)
	{
		TrackElement *E = TrackElement::root_for(thread);
# if CPPAD_TRACK_DEBUG
		if( E->next != CPPAD_NULL )
			TrackElement::Print(thread);
# endif

		while( E->next != CPPAD_NULL ) 
		{	++count;
			E = E->next;
		}
	}
	return count;
}
// ---------------------------------------------------------------------------

} // End CppAD namespace

# undef CppADDebugTrack

# endif
