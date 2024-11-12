# ifndef CPPAD_UTILITY_TRACK_NEW_DEL_HPP
# define CPPAD_UTILITY_TRACK_NEW_DEL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin track_new_del app}
{xrst_spell
   ncopy
   newlen
   newptr
   oldptr
}

Routines That Track Use of New and Delete
#########################################

Deprecated 2007-07-23
*********************
All these routines have been deprecated.
You should use the :ref:`thread_alloc-name` memory allocator instead
(which works better in both a single thread and
properly in multi-threading environment).

Syntax
******
| # ``include <cppad/utility/track_new_del.hpp>``
| *newptr* = ``TrackNewVec`` ( *file* , *line* , *newlen* , *oldptr* )
| ``TrackDelVec`` ( *file* , *line* , *oldptr* )
| *newptr* = ``TrackExtend`` ( *file* , *line* , *newlen* , *ncopy* , *oldptr* )
| *count* = ``TrackCount`` ( *file* , *line* )

Purpose
*******
These routines
aid in the use of ``new[]`` and  ``delete[]``
during the execution of a C++ program.

Include
*******
The file ``cppad/utility/track_new_del.hpp`` is included by
``cppad/cppad.hpp``
but it can also be included separately with out the rest of the
CppAD include files.

file
****
The argument *file* has prototype

   ``const char`` * *file*

It should be the source code file name
where the call to ``TrackNew`` is located.
The best way to accomplish this is the use the preprocessor symbol
``__FILE__`` for this argument.

line
****
The argument *line* has prototype

   ``int`` *line*

It should be the source code file line number
where the call to ``TrackNew`` is located.
The best way to accomplish this is the use the preprocessor symbol
``__LINE__`` for this argument.

oldptr
******
The argument *oldptr* has prototype

   *Type* * *oldptr*

This argument is used to identify the type *Type* .

newlen
******
The argument *newlen* has prototype

   ``size_t`` *newlen*

head newptr
***********
The return value *newptr* has prototype

   *Type* * *newptr*

It points to the newly allocated vector of objects
that were allocated using

   ``new Type`` [ *newlen* ]

ncopy
*****
The argument *ncopy* has prototype

   ``size_t`` *ncopy*

This specifies the number of elements that are copied from
the old array to the new array.
The value of *ncopy*
must be less than or equal *newlen* .

TrackNewVec
***********
If ``NDEBUG`` is defined, this routine only sets

   *newptr* = *Type* ``new`` [ *newlen* ]

The value of *oldptr* does not matter
(except that it is used to identify *Type* ).
If ``NDEBUG`` is not defined, ``TrackNewVec`` also
tracks the this memory allocation.
In this case, if memory cannot be allocated
:ref:`ErrorHandler-name` is used to generate a message
stating that there was not sufficient memory.

Macro
=====
The preprocessor macro call

   ``CPPAD_TRACK_NEW_VEC`` ( *newlen* , *oldptr* )

expands to

   ``CppAD::TrackNewVec`` (__ ``FILE__`` , __ ``LINE__`` , *newlen* , *oldptr* )

Previously Deprecated
=====================
The preprocessor macro ``CppADTrackNewVec`` is the
same as ``CPPAD_TRACK_NEW_VEC`` and was previously deprecated.

TrackDelVec
***********
This routine is used to a vector of objects
that have been allocated using ``TrackNew`` or ``TrackExtend`` .
If ``NDEBUG`` is defined, this routine only frees memory with

   ``delete`` [] *oldptr*

If ``NDEBUG`` is not defined, ``TrackDelete`` also checks that
*oldptr* was allocated by ``TrackNew`` or ``TrackExtend``
and has not yet been freed.
If this is not the case,
:ref:`ErrorHandler-name` is used to generate an error message.

Macro
=====
The preprocessor macro call

   ``CPPAD_TRACK_DEL_VEC`` ( *oldptr* )

expands to

   ``CppAD::TrackDelVec`` (__ ``FILE__`` , __ ``LINE__`` , *oldptr* )

Previously Deprecated
=====================
The preprocessor macro ``CppADTrackDelVec`` is the
same as ``CPPAD_TRACK_DEL_VEC`` was previously deprecated.

TrackExtend
***********
This routine is used to
allocate a new vector (using ``TrackNewVec`` ),
copy *ncopy* elements from the old vector to the new vector.
If *ncopy* is greater than zero, *oldptr*
must have been allocated using ``TrackNewVec`` or ``TrackExtend`` .
In this case, the vector pointed to by *oldptr*
must be have at least *ncopy* elements
and it will be deleted (using ``TrackDelVec`` ).
Note that the dependence of ``TrackExtend`` on ``NDEBUG``
is indirectly through the routines ``TrackNewVec`` and
``TrackDelVec`` .

Macro
=====
The preprocessor macro call

   ``CPPAD_TRACK_EXTEND`` ( *newlen* , *ncopy* , *oldptr* )

expands to

   ``CppAD::TrackExtend`` (__ ``FILE__`` , __ ``LINE__`` , *newlen* , *ncopy* , *oldptr* )

Previously Deprecated
=====================
The preprocessor macro ``CppADTrackExtend`` is the
same as ``CPPAD_TRACK_EXTEND`` and was previously deprecated.

TrackCount
**********
The return value *count* has prototype

   ``size_t`` *count*

If ``NDEBUG`` is defined, *count* will be zero.
Otherwise, it will be
the number of vectors that
have been allocated
(by ``TrackNewVec`` or ``TrackExtend`` )
and not yet freed
(by ``TrackDelete`` ).

Macro
=====
The preprocessor macro call

   ``CPPAD_TRACK_COUNT`` ()

expands to

   ``CppAD::TrackCount`` (__ ``FILE__`` , __ ``LINE__`` )

Previously Deprecated
=====================
The preprocessor macro ``CppADTrackCount`` is the
same as ``CPPAD_TRACK_COUNT`` and was previously deprecated.

Multi-Threading
***************
These routines cannot be used :ref:`in_parallel<ta_in_parallel-name>`
execution mode.
Use the :ref:`thread_alloc-name` routines instead.

{xrst_end track_new_del}
------------------------------------------------------------------------------
*/
# include <cppad/local/define.hpp>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/utility/thread_alloc.hpp>
# include <sstream>
# include <string>

# ifndef CPPAD_TRACK_DEBUG
# define CPPAD_TRACK_DEBUG 0
# endif

// -------------------------------------------------------------------------
# define CPPAD_TRACK_NEW_VEC(newlen, oldptr) \
   CppAD::TrackNewVec(__FILE__, __LINE__, newlen, oldptr)

# define CPPAD_TRACK_DEL_VEC(oldptr) \
   CppAD::TrackDelVec(__FILE__, __LINE__, oldptr)

# define CPPAD_TRACK_EXTEND(newlen, ncopy, oldptr) \
   CppAD::TrackExtend(__FILE__, __LINE__, newlen, ncopy, oldptr)

# define CPPAD_TRACK_COUNT() \
   CppAD::TrackCount(__FILE__, __LINE__)
// -------------------------------------------------------------------------
# define CppADTrackNewVec CPPAD_TRACK_NEW_VEC
# define CppADTrackDelVec CPPAD_TRACK_DEL_VEC
# define CppADTrackExtend CPPAD_TRACK_EXTEND
# define CppADTrackCount  CPPAD_TRACK_COUNT
// -------------------------------------------------------------------------
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
   : file(""), line(0), ptr(nullptr), next(nullptr)
   { }

   TrackElement(const char *f, int l, void *p)
   : file(f), line(l), ptr(p), next(nullptr)
   {  CPPAD_ASSERT_UNKNOWN( p != nullptr);
   }

   // There is only one tracking list and it starts it here
   static TrackElement *Root(void)
   {  CPPAD_ASSERT_UNKNOWN( ! thread_alloc::in_parallel() );
      static TrackElement root;
      return &root;
   }

   // Print one tracking element
   static void Print(TrackElement* E)
   {
      CPPAD_ASSERT_UNKNOWN( ! thread_alloc::in_parallel() );
      using std::cout;
      cout << "E = "         << E;
      cout << ", E->next = " << E->next;
      cout << ", E->ptr  = " << E->ptr;
      cout << ", E->line = " << E->line;
      cout << ", E->file = " << E->file;
      cout << std::endl;
   }

   // Print the linked list for a thread
   static void Print(void)
   {
      CPPAD_ASSERT_UNKNOWN( ! thread_alloc::in_parallel() );
      using std::cout;
      using std::endl;
      TrackElement *E = Root();
      // convert int(size_t) to avoid warning on _MSC_VER systems
      cout << "Begin Track List" << endl;
      while( E->next != nullptr )
      {  E = E->next;
         Print(E);
      }
      cout << "End Track List:" << endl;
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
   CPPAD_ASSERT_UNKNOWN( ! thread_alloc::in_parallel() );
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
   CPPAD_ASSERT_KNOWN( false , message);
}

// TrackNewVec ---------------------------------------------------------------
# ifdef NDEBUG
template <class Type>
Type *TrackNewVec(
   const char *file, int line, size_t len, Type * /* oldptr */ )
{
# if CPPAD_TRACK_DEBUG
   static bool first = true;
   if( first )
   {  std::cout << "NDEBUG is defined for TrackNewVec" << std::endl;
      first = false;
   }
# endif
   return (new Type[len]);
}

# else

template <class Type>
Type *TrackNewVec(
   const char *file          ,
   int         line          ,
   size_t      len           ,
   Type       * /* oldptr */ )
{
   CPPAD_ASSERT_KNOWN(
      ! thread_alloc::in_parallel() ,
      "attempt to use TrackNewVec in parallel execution mode."
   );
   // try to allocate the new memrory
   Type *newptr = nullptr;
   try
   {  newptr = new Type[len];
   }
   catch(...)
   {  TrackError("TrackNewVec", file, line,
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

# if CPPAD_TRACK_DEBUG
   std::cout << "TrackNewVec: ";
   TrackElement::Print(E);
# endif

   return newptr;
}

# endif

// TrackDelVec --------------------------------------------------------------
# ifdef NDEBUG
template <class Type>
void TrackDelVec(const char *file, int line, Type *oldptr)
{
# if CPPAD_TRACK_DEBUG
   static bool first = true;
   if( first )
   {  std::cout << "NDEBUG is defined in TrackDelVec" << std::endl;
      first = false;
   }
# endif
     delete [] oldptr;
}

# else

template <class Type>
void TrackDelVec(
   const char *file    ,
   int         line    ,
   Type       *oldptr  )
{
   CPPAD_ASSERT_KNOWN(
      ! thread_alloc::in_parallel() ,
      "attempt to use TrackDelVec in parallel execution mode."
   );
   TrackElement        *P;
   TrackElement        *E;

   // search list for pointer
   P          = TrackElement::Root();
   E          = P->next;
   void *vptr = static_cast<void *>(oldptr);
   while(E != nullptr && E->ptr != vptr)
   {  P = E;
      E = E->next;
   }

   // check if pointer was not in list
   if( E == nullptr || E->ptr != vptr ) TrackError(
      "TrackDelVec", file, line,
      "Invalid value for the argument oldptr.\n"
      "Possible linking of debug and NDEBUG compilations of CppAD."
   );

# if CPPAD_TRACK_DEBUG
   std::cout << "TrackDelVec: ";
   TrackElement::Print(E);
# endif

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
{
   CPPAD_ASSERT_KNOWN(
      ! thread_alloc::in_parallel() ,
      "attempt to use TrackExtend in parallel execution mode."
   );

# if CPPAD_TRACK_DEBUG
   using std::cout;
   cout << "TrackExtend: file = " << file;
   cout << ", line = " << line;
   cout << ", newlen = " << newlen;
   cout << ", ncopy = " << ncopy;
   cout << ", oldptr = " << oldptr;
   cout << std::endl;
# endif
   CPPAD_ASSERT_KNOWN(
      ncopy <= newlen,
      "TrackExtend: ncopy is greater than newlen."
   );

   // allocate the new memrory
   Type *newptr = TrackNewVec(file, line, newlen, oldptr);

   // copy the data
   size_t i;
   for(i = 0; i < ncopy; i++)
      newptr[i] = oldptr[i];

   // delete the old vector
   if( ncopy > 0 )
      TrackDelVec(file, line, oldptr);

   return newptr;
}

// TrackCount --------------------------------------------------------------
inline size_t TrackCount(const char *file, int line)
{
   CPPAD_ASSERT_KNOWN(
      ! thread_alloc::in_parallel() ,
      "attempt to use TrackCount in parallel execution mode."
   );
   size_t count = 0;
   TrackElement *E = TrackElement::Root();
   while( E->next != nullptr )
   {  ++count;
      E = E->next;
   }
   return count;
}
// ---------------------------------------------------------------------------

} // End CppAD namespace

// preprocessor symbols local to this file
# undef CPPAD_TRACK_DEBUG

# endif
