# ifndef CppADTrackNewDelIncluded
# define CppADTrackNewDelIncluded

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin TrackNewDel$$
$spell
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
The value of $italic oldptr$$ does not matter for this case.
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

$head Example$$
$children%
	Example/TrackNewDel.cpp
%$$
The file $xref/TrackNewDel.cpp/$$
contains an example and test of these functions.
It returns true, if it succeeds, and false otherwise.

$end
------------------------------------------------------------------------------
*/
# include <CppAD/local/cppad_error.hpp>
# include <sstream>
# include <string>

# define CppADDebugTrack 0

# ifndef CppADNull
# define CppADNull	0
# endif

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
	: file(""), line(0), ptr(CppADNull), next(CppADNull)
	{ }
	
	TrackElement(const char *f, int l, void *p)
	: file(f), line(l), ptr(p), next(CppADNull)
	{	CppADUnknownError( p != CppADNull);
	}

	// There is only one tracking list and it starts it here
	static TrackElement *Root(void)
	{	static TrackElement root;
		return &root;
	}

	// Print the linked list
	static void Print(void)
	{	TrackElement *E = Root();
		std::cout << "Begin Track List" << std::endl;
		while( E->next != CppADNull )
		{	E = E->next;
			std::cout << "next = " << E->next;
			std::cout << ", ptr  = " << E->ptr;
			std::cout << ", line = " << E->line;
			std::cout << ", file = " << E->file;
			std::cout << std::endl;
		}
		std::cout << "End Track List:" << std::endl;
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
	Type *newptr = CppADNull;
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
	while(E != CppADNull && E->ptr != vptr)
	{	P = E;
		E = E->next;
	}

	// check if pointer was not in list
	if( E == CppADNull || E->ptr != vptr ) TrackError(
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
	TrackElement *E = TrackElement::Root();
	size_t count = 0;
	while( E->next != CppADNull ) 
	{
# if CppADTrackDebug
		std::cout << "Before TrackCount:" << std::endl;
		TrackElement::Print();
# endif
		++count;
		E = E->next;
	}
	return count;
}
// ---------------------------------------------------------------------------

} // End CppAD namespace

# undef CppADDebugTrack

# endif
