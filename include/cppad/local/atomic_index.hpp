# ifndef CPPAD_LOCAL_ATOMIC_INDEX_HPP
# define CPPAD_LOCAL_ATOMIC_INDEX_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*!
{xrst_begin atomic_index dev}

Store and Retrieve Atomic Function Information by Index
#######################################################

Syntax
******
| *index_out* = ``local::atomic_index<`` *Base* >(
| |tab| *set_null* , *index_in* , *type* , *name* , *ptr*
| )

Prototype
*********
{xrst_literal
   // BEGIN_ATOMIC_INDEX
   // END_PROTOTYPE
}

Base
****
Is the base type for the tape for the atomic functions
that we are using an index to identify.

Get Number Case
***************
The get number case is defined by
*set_null* is true and *index_in* is zero.
For this case, *index_out* is set to
the number of atomic functions stored in ``atomic_index`` < *Base* >
and no information is stored or changed.
In this case, the atomic functions correspond to *index_in* from
one to *index_out* inclusive.

set_null
********
If *set_null* is true and *index_in* is zero,
this argument is just used to signal the get number case.
Otherwise, *set_null*
should only be true during a call to an atomic function destructor.
In this case, the *ptr* corresponding to *index_in*
is set to null
(so that CppAD knows the corresponding atomic function no longer works).

index_in
********
If *index_in* is zero and *set_null* is true,
this argument is just used to signal the get number case.
Otherwise, see below:

zero
====
The value *index_in* should only be zero
during a call to an atomic function constructor.
In this case, a copy of the input value of
*type* , * *name* , and *ptr* are stored.
The value *index_out*
is the *index_in* value corresponding to these input values.

non-zero
========
If *index_in* is non-zero,
the information corresponding to this index is returned.

type
****
This argument is not used in the get number case.
Otherwise if *index_in* is zero, *type* is an input.
Otherwise it is set to the value corresponding to *index_in* .
The type corresponding to an index is intended to be
2 for :ref:`atomic_two-name` functions,
3 for :ref:`atomic_three-name` functions, and
4 for :ref:`atomic_four-name` functions,

name
****
This argument is not used in the get number case.
Otherwise if *index_in* is zero,
*name* is an input and must not be null.
Otherwise, if *name* is not null, * *name*
is set to the name corresponding to *index_in* .
Allowing for *name* to be null avoids
a string copy when it is not needed.

ptr
***
This argument is not used in the get number case.
Otherwise if *index_in* is zero, *ptr* is an input.
Otherwise it is set to the value corresponding to *index_in* .
In the special case where *set_null* is true,
*ptr* is set to the null pointer and this is the *ptr* value
corresponding to *index_in* for future calls to ``atomic_index`` .

index_out
*********
In the get number case, this is the number of atomic functions.
Otherwise if *index_in* is zero,
*index_out* is non-zero and is the *index_in* value
corresponding to the input values for
*type* , * *name* , and *ptr* .
Otherwise, *index_out* is zero.

{xrst_end atomic_index}
*/
# include <vector>
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE

struct atomic_index_info {
   size_t      type;
   std::string name;
   void*       ptr;
};

// BEGIN_ATOMIC_INDEX
template <class Base>
size_t atomic_index(
   bool               set_null      ,
   const size_t&      index_in      ,
   size_t&            type          ,
   std::string*       name          ,
   void*&             ptr           )
// END_PROTOTYPE
{  //
   // information for each index
   static std::vector<atomic_index_info> vec;
# ifndef NDEBUG
   if( index_in == 0 || set_null )
   {  CPPAD_ASSERT_KNOWN( ! thread_alloc::in_parallel(),
      "calling atomic function constructor or destructor in parallel mode"
      );
   }
# endif
   if( set_null & (index_in == 0) )
      return vec.size();
   //
   // case were we are retreving information for an atomic function
   if( 0 < index_in )
   {  CPPAD_ASSERT_UNKNOWN( index_in <= vec.size() )
      //
      // case where we are setting the pointer to null
      if( set_null )
         vec[index_in-1].ptr = nullptr;
      //
      atomic_index_info& entry = vec[index_in - 1];
      type = entry.type;
      ptr  = entry.ptr;
      if( name != nullptr )
         *name  = entry.name;
      return 0;
   }
   //
   // case where we are storing information for an atomic function
   atomic_index_info entry;
   entry.type = type;
   entry.name = *name;
   entry.ptr  = ptr;
   vec.push_back(entry);
   //
   return vec.size();
}

} } // END_CPPAD_LOCAL_NAMESPACE

# endif
