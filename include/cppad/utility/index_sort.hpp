# ifndef CPPAD_UTILITY_INDEX_SORT_HPP
# define CPPAD_UTILITY_INDEX_SORT_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin index_sort}

Returns Indices that Sort a Vector
##################################

Syntax
******
| # ``include <cppad/utility/index_sort.hpp>``
| ``index_sort`` ( *keys* , *ind* )

keys
****
The argument *keys* has prototype

   ``const`` *KeyVector* & *keys*

where *KeyVector* is
a :ref:`SimpleVector-name` class with elements that support the ``<``
operation.

ind
***
The argument *ind* has prototype

   *SizeVector* & *ind*

where *SizeVector* is
a :ref:`SimpleVector-name` class with elements of type *Size_t* ,
where *Size_t* is an integral type.
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.

Input
=====
The size of *ind* must be the same as the size of *keys*
and the value of its input elements does not matter.

Return
======
Upon return, *ind* is a permutation of the set of indices
that yields increasing order for *keys* .
In other words, for all *i* != *j* ,

   *ind* [ *i* ] != *ind* [ *j* ]

and for *i* = 0 , ... , *size* ``-2`` ,

   ( *keys* [ *ind* [ *i* +1] ] < *keys* [ *ind* [ *i* ] ] ) == ``false``

Example
*******
{xrst_toc_hidden
   example/utility/index_sort.cpp
}
The file :ref:`index_sort.cpp-name` contains an example
and test of this routine.
It return true if it succeeds and false otherwise.

{xrst_end index_sort}
*/
# include <algorithm>
# include <cppad/utility/thread_alloc.hpp>
# include <cppad/utility/check_simple_vector.hpp>
# include <cppad/local/define.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file index_sort.hpp
File used to implement the CppAD index sort utility
*/

/*!
Helper class used by index_sort
*/
template <class Compare, class Size_t>
class index_sort_element {
private:
   /// key used to determine position of this element
   Compare key_;
   /// index vlaue corresponding to this key
   Size_t  index_;
public:
   /// operator requried by std::sort
   bool operator<(const index_sort_element& other) const
   {  return key_ < other.key_; }
   /// set the key for this element
   void set_key(const Compare& value)
   {  key_ = value; }
   /// set the index for this element
   void set_index(const Size_t& index)
   {  index_ = index; }
   /// get the key for this element
   Compare get_key(void) const
   {  return key_; }
   /// get the index for this element
   Size_t get_index(void) const
   {  return index_; }
};

/*!
Compute the indices that sort a vector of keys

\tparam KeyVector
Simple vector type that deterimene the sorting order by < operator
on its elements.

\tparam SizeVector
Simple vector type with elements of size_t
that is used to return index values.

\param keys [in]
values that determine the sorting order.

\param ind [out]
must have the same size as keys.
The input value of its elements does not matter.
The output value of its elements satisfy
\code
( keys[ ind[i] ] < keys[ ind[i+1] ] ) == false
\endcode
*/
template <class KeyVector, class SizeVector>
void index_sort(const KeyVector& keys, SizeVector& ind)
{  typedef typename KeyVector::value_type      Compare;
   typedef typename SizeVector::value_type     Size_t;
   typedef index_sort_element<Compare, Size_t> Element;
   //
   CheckSimpleVector<Size_t, SizeVector>();
   //
   CPPAD_ASSERT_KNOWN(
      keys.size() == ind.size(),
      "index_sort: vector sizes do not match"
   );

   size_t size_work = keys.size();
   size_t size_out;
   Element* work =
      thread_alloc::create_array<Element>(size_work, size_out);

   // copy initial order into work
   size_t i;
   for(i = 0; i < size_work; i++)
   {  work[i].set_key( keys[i] );
      work[i].set_index( Size_t(i) );
   }

   // sort the work array
   std::sort(work, work+size_work);

   // copy the indices to the output vector
   for(i = 0; i < size_work; i++)
      ind[i] = work[i].get_index();

   // we are done with this work array
   thread_alloc::delete_array(work);

   return;
}

} // END_CPPAD_NAMESPACE

# endif
