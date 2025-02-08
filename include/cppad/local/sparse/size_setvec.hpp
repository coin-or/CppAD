# ifndef CPPAD_LOCAL_SPARSE_SIZE_SETVEC_HPP
# define CPPAD_LOCAL_SPARSE_SIZE_SETVEC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/pod_vector.hpp>
/*
{xrst_begin_parent size_setvec dev}

Implement SetVector Using Singly Linked Lists
#############################################

Namespace
*********
This class is in the ``CppAD::local::sparse`` namespace.

Public
******
The public member functions for the ``size_setvec`` class implements the
:ref:`SetVector-name` concept when *s_type* is ``size_t`` .

{xrst_end size_setvec}
*/

// BEGIN_CPPAD_LOCAL_SPARSE_NAMESPACE
namespace CppAD { namespace local { namespace sparse {

// forward declaration of iterator class
template <class s_type> class size_setvec_const_iterator;

// =========================================================================
template <class s_type>
class size_setvec { // BEGIN_CLASS_LIST_SETVEC
// =========================================================================

/*
-------------------------------------------------------------------------------
{xrst_begin size_setvec_member_data dev}
{xrst_spell
   struct
}

class size_setvec: Private Member Data
######################################

pair_s_type
***********
This ``struct`` is local to the ``size_setvec`` class.
It is the type used for each entry in a singly linked list
and has the following fields:

value
=====
The is the value of an entry in the list
(for sets, the first entry in the list is a reference count).

next
====
This is the index in ``data_`` for the next entry in the list.
If there are no more entries in the list, this value is zero; i.e.,
``data_[0]`` is used to represent the end of a list.

end\_
*****
The possible elements in each set are ``0`` , ``1`` , ...,
``end_-1``

number_not_used\_
*****************
Number of elements of the ``data_`` vector that are not being used.

data_not_used\_
***************
Index in ``data_`` for the start of the linked list of elements
that are not being used.

data\_
******
The data for all the singly linked lists.
If *n_set*  > 0 ,
``data`` [0]. ``value`` == ``end_`` and
``data`` [0]. ``next`` == 0 .

start\_
*******
The size of this vector is the number of set; i.e.,
:ref:`SetVector@Vector Operations@n_set` .
The starting index for *i*-th set is ``start_`` [ *i* ] .
If ``start_`` [ *i* ] == 0 , the i-th set has no elements.
Otherwise it is the index of the reference count for the list.

Reference Count
===============
If ``start_`` [ *i* ] != 0 , ``data_`` [ ``start_`` [ *i* ]]. ``value``
is the reference count for the *i*-th list
(not the value of an element in the list).
The reference count must be greater than zero.

First Element
=============
If ``start_`` [ *i* ] != 0 ,

   *first_index* = ``data_`` [ ``start_`` [ *i* ]]. ``next``

is the index of the first element in the list.
This must be non-zero because the list is empty.

Next Element
============
Starting with *index* = *first_index* ,
while *index*  != 0 ,
The value of the corresponding element in the list is
``data_`` [ *index* ]. ``value`` (which must be less than ``end_`` ).
The *index* for the next element of the list is
``data_`` [ *index* ]. ``next`` .

Last Element
============
An index *last* corresponds to the last element of a list if
``data_`` [ *last* ]. ``next`` == 0 .
(Note that ``data_[0].value == end_`` ).

post\_
******
The size of this vector is the number of set; i.e.,
:ref:`SetVector@Vector Operations@n_set` .
Starting with *index* = ``post_`` [ *i* ] ,
while *index*  != 0 ,
The value of the next element posted to the *i*-th list is
``data_`` [ *index* ]. ``value`` (which must be less than ``end_`` ).
The *index* for the next element posted to the *i*-th list is
``data_`` [ *index* ]. ``next`` .

temporary\_
***********
A temporary vector, used by member functions, that keeps its capacity
to avoid re-allocating memory.
If a member function calls another,
no conditions about ``temporary_`` should be assumed during that call.

Source Code
***********
{xrst_spell_off}
{xrst_code hpp} */
private:
   struct pair_s_type {s_type value; s_type next; };
   friend bool CppAD::local::is_pod<pair_s_type>(void);
   //
   s_type                   end_;
   s_type                   number_not_used_;
   s_type                   data_not_used_;
   //
   pod_vector<pair_s_type> data_;
   pod_vector<s_type>      start_;
   pod_vector<s_type>      post_;
   pod_vector<s_type>      temporary_;
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_member_data}
------------------------------------------------------------------------------
{xrst_begin size_setvec_reference_count dev}

class size_setvec private: Number of References to a Set
########################################################

Syntax
******
| *count* = *vec* . ``reference_count`` ( *i* )

vec
***
Is a ``size_setvec`` object and can be ``const`` .

i
*
is the index of the set that we are retrieving the references for.

count
*****
is the reference count.

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
private:
   s_type reference_count(s_type i) const
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_reference_count}
*/
   {  // start data index
      s_type start = start_[i];
      if( start == 0 )
         return 0;
      //
      // reference count
      return data_[start].value;
   }
/*
------------------------------------------------------------------------------
{xrst_begin size_setvec_drop dev}
class size_setvec private: Drop a Set No Longer Being Used
##########################################################

Syntax
******
*not_used* = *vec* . ``drop`` ( *i* )

i
*
is the index of the set that is dropped.

reference_count
***************
if the set is non-empty, the
:ref:`size_setvec_member_data@start_@Reference Count`
is decremented.

start\_
*******
The value ``start_`` [ *i* ] is set to zero; i.e.,
the *i*-th set is empty after the call.

post\_
******
The value ``post_`` [ *i* ] is set to zero; i.e.,
any postings to the list are also dropped.

data_not_used\_
***************
The elements of ``data_`` were information for the *i*-th set,
and are no longer being used, are added to the linked list starting at
``data_not_used`` .
This includes both set elements and postings.

not_used
********
is the number of elements of ``data_`` that were begin used for the
*i*-th set and are no longer being used; i.e., the number of elements
moved to ``data_not_used`` .

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
private:
   s_type drop(s_type i)
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_drop}
*/
   {  // inialize count of addition elements not being used.
      s_type number_drop = 0;

      // the elements in the post list will no longer be used
      s_type post = post_[i];
      if( post != 0 )
      {  // drop this posting
         post_[i]    = 0;
         //
         // count elements in this posting
         ++number_drop;
         s_type previous = post;
         s_type next     = data_[previous].next;
         while( next != 0 )
         {  previous = next;
            next     = data_[previous].next;
            ++number_drop;
         }
         //
         // add the posting elements to data_not_used_
         data_[previous].next = data_not_used_;
         data_not_used_       = post;
      }

      // check for empty set
      s_type start = start_[i];
      if( start == 0 )
         return number_drop;

      // decrement reference counter
      CPPAD_ASSERT_UNKNOWN( data_[start].value > 0 );
      data_[start].value--;

      // set this set to empty
      start_[i] = 0;

      // If new reference count is positive, the list corresponding to
      // start is still being used.
      if( data_[start].value > 0 )
         return number_drop;

      //
      // count elements representing this set
      ++number_drop;
      s_type previous = start;
      s_type next     = data_[previous].next;
      while( next != 0 )
      {  previous = next;
         next     = data_[previous].next;
         ++number_drop;
      }
      //
      // add representing this set to data_not_used_
      data_[previous].next = data_not_used_;
      data_not_used_       = start;
      //
      return number_drop;
   }
/*
------------------------------------------------------------------------------
{xrst_begin size_setvec_get_data_index dev}

class size_setvec private: Get a New List Pair
##############################################

Syntax
******
*index* = *vec* . ``get_data_index`` ()

vec
***
Is a ``size_setvec`` object.

data_not_used\_
***************
If the input value of ``data_not_used_`` is zero,
it is not changed.
Otherwise, the index for the element at the front of that list is returned.
In this case,
``data_not_used`` is advanced to the next element in that list.

number_not_used\_
*****************
If the input value of ``data_not_used_`` is zero,
``number_not_used_`` is not changed.
Otherwise it is decremented by one.

index
*****
If the input value of ``data_not_used_`` is zero,
the size of ``data_`` is increased by one and index corresponding
to the end of ``data_`` is returned.
Otherwise, the input value for ``data_not_used_`` is returned.

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
private:
   s_type get_data_index(void)
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_get_data_index}
*/
   {  s_type index;
      if( data_not_used_ > 0 )
      {  CPPAD_ASSERT_UNKNOWN( number_not_used_ > 0 );
         --number_not_used_;
         index          = data_not_used_;
         data_not_used_ = data_[index].next;
      }
      else
      {  index = s_type( data_.extend(1) );
      }
      return index;
   }
/*
-------------------------------------------------------------------------------
{xrst_begin size_setvec_check_data_structure dev}

class size_setvec private: Check Data Structure
###############################################

Syntax
******
*vec* . ``check_data_structure`` ()

vec
***
Is a ``size_setvec`` object that is effectively const.
It is not declared const because the data structure is modified and
then restored.

NDEBUG
******
If ``NDEBUG`` is defined, the routine does nothing.
Otherwise, if an error is found in the data structure,
a ``CPPAD_ASSERT_UNKNOWN`` is generated.

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
private:
   void check_data_structure(void)
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_check_data_structure}
*/
# ifdef NDEBUG
   {  return; }
# else
   {  // number of sets
      CPPAD_ASSERT_UNKNOWN( s_type( post_.size() ) == s_type( start_.size() ) );
      s_type n_set = s_type( start_.size() );
      if( n_set == 0 )
      {  CPPAD_ASSERT_UNKNOWN( end_ == 0 );
         CPPAD_ASSERT_UNKNOWN( number_not_used_ == 0 );
         CPPAD_ASSERT_UNKNOWN( data_not_used_ == 0 );
         CPPAD_ASSERT_UNKNOWN( s_type( data_.size() ) == 0 );
         CPPAD_ASSERT_UNKNOWN( s_type( start_.size() ) == 0 );
         return;
      }
      // check data index zero
      CPPAD_ASSERT_UNKNOWN( data_[0].value == end_ );
      CPPAD_ASSERT_UNKNOWN( data_[0].next  == 0  );
      // -----------------------------------------------------------
      // save a copy of the reference counters in temporary_
      temporary_.resize( size_t(n_set) );
      for(s_type i = 0; i < n_set; i++)
         temporary_[i] = reference_count(i);
      // -----------------------------------------------------------
      // Initialize number of entries in data used by sets and posts.
      // Start with 1 for data_[0].
      s_type number_used_by_sets = 1;
      // -----------------------------------------------------------
      // count the number of entries in data_ that are used by sets
      for(s_type i = 0; i < n_set; i++)
      {  s_type start = start_[i];
         if( start > 0 )
         {  // check structure for this non-empty set
            s_type reference_count = data_[start].value;
            s_type next            = data_[start].next;
            CPPAD_ASSERT_UNKNOWN( reference_count > 0 );
            CPPAD_ASSERT_UNKNOWN( next != 0 );
            CPPAD_ASSERT_UNKNOWN( data_[next].value < end_ );
            //
            // decrement the reference counter
            data_[start].value--;
            //
            // count the entries when find last reference
            if( data_[start].value == 0 )
            {
               // restore reference count
               data_[start].value = temporary_[i];

               // number of data entries used for this set
               number_used_by_sets += number_elements(i) + 1;
               /*
               number of elements checks that value < end_
               .resizeeach pair in the list except for the start pair
               and the pair with index zero.
               */
            }
         }
      }
      // ------------------------------------------------------------------
      // count the number of entries in data_ that are used by posts
      s_type number_used_by_posts = 0;
      for(s_type i = 0; i < n_set; i++)
      {  s_type post = post_[i];
         if( post > 0 )
         {  s_type value = data_[post].value;
            s_type next  = data_[post].next;
            CPPAD_ASSERT_UNKNOWN( value < end_ );
            //
            while( value < end_ )
            {  ++number_used_by_posts;
               value = data_[next].value;
               next  = data_[next].next;
            }
         }
      }
      // ------------------------------------------------------------------
      // count number of entries in data_not_used_
      s_type count = 0;
      s_type next = data_not_used_;
      while( next != 0 )
      {  ++count;
         next = data_[next].next;
      }
      CPPAD_ASSERT_UNKNOWN( number_not_used_ == count );
      // ------------------------------------------------------------------
      s_type number_used = number_used_by_sets + number_used_by_posts;
      CPPAD_ASSERT_UNKNOWN(
         number_used + number_not_used_ == s_type( data_.size() )
      );
      return;
   }
# endif
/*
-------------------------------------------------------------------------------
{xrst_begin size_setvec_vec_memory dev}

class size_setvec: Approximate Memory Used by Vector
####################################################

Public
******
This function is declared public, but is not part of
:ref:`SetVector-name` concept.

Implementation
**************
{xrst_spell_off}
{xrst_code hpp} */
public:
   s_type memory(void) const
   {  return data_.capacity() * sizeof(pair_s_type); }
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_vec_memory}
-------------------------------------------------------------------------------
{xrst_begin size_setvec_vec_print dev}

class size_setvec: Print a Vector of Sets
#########################################

Public
******
This function is declared public, but is not part of
:ref:`SetVector-name` concept.

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void print(void) const;
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_vec_print}
-------------------------------------------------------------------------------
{xrst_begin size_setvec_iterators dev}
{xrst_spell
   typedef
}

class size_setvec: Iterators
############################

SetVector Concept
*****************
:ref:`SetVector@const_iterator`

typedef
*******
{xrst_spell_off}
{xrst_code hpp} */
public:
   friend class size_setvec_const_iterator<s_type>;
   typedef size_setvec_const_iterator<s_type> const_iterator;
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_iterators}
-------------------------------------------------------------------------------
{xrst_begin size_setvec_default_ctor dev}

class size_setvec: Default Constructor
######################################

SetVector Concept
*****************
:ref:`SetVector@Vector Operations@Constructor`

s_type Members
**************
All of the ``s_type`` member variables are initialized as zero.

pod_vector Members
******************
All of the ``pod_vector`` member variables are initialized
using their default constructors.

Implementation
**************
{xrst_spell_off}
{xrst_code hpp} */
public:
   size_setvec(void)
   : end_(0), number_not_used_(0), data_not_used_(0)
   { }
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_default_ctor}
-------------------------------------------------------------------------------
{xrst_begin size_setvec_destructor dev}

class size_setvec: Destructor
#############################

Implementation
**************
If ``NDEBUG`` is not defined,
:ref:`check data structure<size_setvec_check_data_structure-name>` .
{xrst_spell_off}
{xrst_code hpp} */
public:
   ~size_setvec(void)
   {  check_data_structure(); }
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_destructor}
-------------------------------------------------------------------------------
{xrst_begin size_setvec_copy_ctor dev}

class size_setvec: Copy Constructor
###################################

v
*
The vector of sets that we are attempting to make a copy of.

Implementation
**************
Using the copy constructor is probably due to a ``size_setvec``
being passed by value instead of by reference.
This is a CppAD programing error (not CppAD user error).
{xrst_spell_off}
{xrst_code hpp} */
public:
   size_setvec(const size_setvec& v)
   {  CPPAD_ASSERT_UNKNOWN(false); }
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_copy_ctor}
-------------------------------------------------------------------------------
{xrst_begin size_setvec_vec_resize dev}

class size_setvec: Vector resize
################################

SetVector Concept
*****************
:ref:`vector resize<SetVector@Vector Operations@resize>`

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void resize(s_type n_set, s_type end)
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_vec_resize}
*/
   {  check_data_structure();

      if( n_set == 0 )
      {  CPPAD_ASSERT_UNKNOWN( end == 0 );
         //
         // restore object to start after constructor
         // (no memory allocated for this object)
         data_.clear();
         start_.clear();
         post_.clear();
         number_not_used_  = 0;
         data_not_used_    = 0;
         end_              = 0;
         //
         return;
      }
      end_                   = end;
      //
      start_.resize( size_t(n_set) );
      post_.resize(  size_t(n_set) );
      //
      for(s_type i = 0; i < n_set; i++)
      {  start_[i] = 0;
         post_[i]  = 0;
      }
      //
      // last element, marks the end for all lists
      data_.resize(1);
      data_[0].value    = end_;
      data_[0].next     = 0;
      //
      number_not_used_  = 0;
      data_not_used_    = 0;
   }
/* %$$
-------------------------------------------------------------------------------
{xrst_begin size_setvec_vec_n_set dev}

class size_setvec: Number of Sets
#################################

SetVector Concept
*****************
:ref:`SetVector@Vector Operations@n_set`

Implementation
**************
{xrst_spell_off}
{xrst_code hpp} */
public:
   s_type n_set(void) const
   {  return s_type( start_.size() ); }
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_vec_n_set}
-------------------------------------------------------------------------------
{xrst_begin size_setvec_vec_end dev}

class size_setvec: End Value
############################

SetVector Concept
*****************
:ref:`SetVector@Vector Operations@end`

Implementation
**************
{xrst_spell_off}
{xrst_code hpp} */
public:
   s_type end(void) const
   {  return end_; }
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_vec_end}
-------------------------------------------------------------------------------
{xrst_begin size_setvec_vec_assignment dev}

class size_setvec: Vector Assignment
####################################

SetVector Concept
*****************
:ref:`vector assignment<SetVector@Vector Operations@Assignment>`

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void operator=(const size_setvec& other)
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_vec_assignment}
*/
   {  end_             = other.end_;
      number_not_used_ = other.number_not_used_;
      data_not_used_   = other.data_not_used_;
      data_            = other.data_;
      start_           = other.start_;
      post_            = other.post_;
   }
/*
-------------------------------------------------------------------------------
{xrst_begin size_setvec_vec_swap dev}

class size_setvec: Vector Swap
##############################

SetVector Concept
*****************
:ref:`vector swap<SetVector@Vector Operations@swap>`

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void swap(size_setvec& other)
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_vec_swap}
*/
   {  // s_type objects
      std::swap(end_             , other.end_);
      std::swap(number_not_used_ , other.number_not_used_);
      std::swap(data_not_used_   , other.data_not_used_);

      // pod_vectors
      data_.swap(       other.data_);
      start_.swap(      other.start_);
      post_.swap(       other.post_);
      temporary_.swap(  other.temporary_);
   }
/*
-------------------------------------------------------------------------------
{xrst_begin size_setvec_number_elements dev}

class size_setvec: Number of Elements in a Set
##############################################

SetVector Concept
*****************
:ref:`SetVector@number_elements`

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   s_type number_elements(s_type i) const
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_number_elements}
*/
   {  CPPAD_ASSERT_UNKNOWN( post_[i] == 0 );

      // check if the set is empty
      s_type start   = start_[i];
      if( start == 0 )
         return 0;

      // initialize counter
      s_type count   = 0;

      // advance to the first element in the set
      s_type next    = data_[start].next;
      while( next != 0 )
      {  CPPAD_ASSERT_UNKNOWN( data_[next].value < end_ );
         count++;
         next  = data_[next].next;
      }
      CPPAD_ASSERT_UNKNOWN( count > 0 );
      return count;
   }
/*
-------------------------------------------------------------------------------
{xrst_begin size_setvec_add_element dev}

class size_setvec: Add an Elements to a Set
###########################################

SetVector Concept
*****************
:ref:`SetVector@add_element`

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void add_element(s_type i, s_type element)
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_add_element}
*/
   {  CPPAD_ASSERT_UNKNOWN( i < s_type( start_.size() ) );
      CPPAD_ASSERT_UNKNOWN( element < end_ );

      // check for case where starting set is empty
      s_type start = start_[i];
      if( start == 0 )
      {  start              = get_data_index();
         start_[i]          = start;
         data_[start].value = 1; // reference count
         //
         s_type next        = get_data_index();
         data_[start].next  = next;
         //
         data_[next].value  = element;
         data_[next].next   = 0;
         return;
      }
      //
      // start of set with this index
      s_type previous = start_[i];
      //
      // first entry in this set
      s_type next     = data_[previous].next;
      s_type value    = data_[next].value;
      //
      // locate place to insert this element
      while( value < element )
      {  previous = next;
         next     = data_[next].next;
         value = data_[next].value;
      }
      //
      // check for case where element is in the set
      if( value == element )
         return;
      //
      //
      // check for case where this is the only reference to this set
      CPPAD_ASSERT_UNKNOWN( element < value );
      if( data_[start].value == 1 )
      {  s_type insert         = get_data_index();
         data_[insert].next    = next;
         data_[insert].value   = element;
         data_[previous].next  = insert;
         //
         return;
      }
      //
      // must make a separate copy with new element inserted
      CPPAD_ASSERT_UNKNOWN( data_[start].value > 1 );
      data_[start].value--;   // reverence counter for old list
      //
      s_type start_new       = get_data_index();
      data_[start_new].value = 1;         // reference counter for new list
      s_type previous_new    = start_new;
      //
      // start of old set with this index
      previous  = start_[i];
      //
      // first entry in old set
      next    = data_[previous].next;
      value   = data_[next].value;
      //
      // locate place to insert this element
      while( value < element )
      {  // copy to new list
         s_type next_new          = get_data_index();
         data_[previous_new].next = next_new;
         data_[next_new].value    = value;
         previous_new             = next_new;
         //
         // get next value
         previous = next;
         next     = data_[next].next;
         value = data_[next].value;
      }
      CPPAD_ASSERT_UNKNOWN( element < value );
      //
      // insert the element
      s_type next_new          = get_data_index();
      data_[previous_new].next = next_new;
      data_[next_new].value    = element;
      previous_new             = next_new;
      //
      // copy rest of the old set
      while( value < end_ )
      {  // copy to new list
         next_new                 = get_data_index();
         data_[previous_new].next = next_new;
         data_[next_new].value    = value;
         previous_new             = next_new;
         //
         // get next value
         previous = next;
         next     = data_[next].next;
         value = data_[next].value;
      }
      CPPAD_ASSERT_UNKNOWN( next == 0 );
      data_[previous_new].next = 0;
      //
      // hook up new list
      start_[i] = start_new;
      return;
   }
/*
-------------------------------------------------------------------------------
{xrst_begin size_setvec_post_element dev}

class size_setvec: Post an Elements for Addition to a Set
#########################################################

SetVector Concept
*****************
:ref:`SetVector@post_element`

post\_
******
The element is added at the front of the linked list
that starts at ``post_`` .

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void post_element(s_type i, s_type element)
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_post_element}
*/
   {  CPPAD_ASSERT_UNKNOWN( i < s_type( start_.size() ) );
      CPPAD_ASSERT_UNKNOWN( element < end_ );

      // put element at the front of this list
      s_type next         = post_[i];
      s_type post         = get_data_index();
      post_[i]            = post;
      data_[post].value   = element;
      data_[post].next    = next;

      return;
   }
/*
-------------------------------------------------------------------------------
{xrst_begin size_setvec_process_post dev}

class size_setvec: Add Posted Elements to a Set
###############################################

SetVector Concept
*****************
:ref:`SetVector@process_post`

post\_
******
Upon call, ``post_`` [ *i* ] is the linked list of elements to
be added to the *i*-th set.
Upon return, ``post_`` [ *i* ] is zero; i.e., the list is empty.

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void process_post(s_type i)
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_process_post}
*/
   {  // post
      s_type post = post_[i];
      //
      // check if there are no elements to process
      if( post == 0 )
         return;
      //
      // check if there is only one element to process
      s_type next  = data_[post].next;
      if( next == 0 )
      {  // done with this posting
         s_type value     = data_[post].value;
         post_[i]         = 0;
         data_[post].next = data_not_used_;
         data_not_used_   = post;
         ++number_not_used_;
         //
         add_element(i, value);
         //
         return;
      }
      //
      // copy posting to temporary_
      temporary_.resize(0);
      s_type previous  = post;
      s_type value     = data_[previous].value;
      CPPAD_ASSERT_UNKNOWN( value < end_ );
      temporary_.push_back(value);
      while( next != 0 )
      {  previous = next;
         value    = data_[previous].value;
         CPPAD_ASSERT_UNKNOWN( value < end_ );
         temporary_.push_back(value);
         next     = data_[previous].next;
      }
      s_type number_post = s_type( temporary_.size() );
      //
      // done with this posting
      post_[i]              = 0;
      data_[previous].next  = data_not_used_;
      data_not_used_        = post;
      number_not_used_     += number_post;;
      //
      // sort temporary_
      CPPAD_ASSERT_UNKNOWN( number_post > 1 );
      std::sort( temporary_.data(), temporary_.data() + number_post);
      // posting is the set { temporary_[0], ... , [number_post-1] }
      // -------------------------------------------------------------------
      // put union of posting and set i in
      // temporary_[number_post], ... , temporary_[ temporary_.size()-1 ]
      //
      s_type i_next  = start_[i];
      s_type i_value = end_;
      if( i_next > 0 )
      {  // skip reference count
         i_next  = data_[i_next].next;
         i_value = data_[i_next].value;
      }
      bool   post_is_subset = true;
      s_type previous_post = end_;
      for(s_type j =0; j < number_post; ++j)
      {  s_type post_value = temporary_[j];
         CPPAD_ASSERT_UNKNOWN( post_value < end_ );
         while( i_value < post_value )
         {  // i_value is in union
            temporary_.push_back(i_value);
            i_next  = data_[i_next].next;
            i_value = data_[i_next].value;
         }
         if( i_value == post_value )
         {  i_next  = data_[i_next].next;
            i_value = data_[i_next].value;
         }
         else
            post_is_subset = false;
         //
         if( previous_post != post_value )
         {  // post_value is in union
            temporary_.push_back(post_value);
         }
         previous_post = post_value;
      }
      // check if posting is a subset of set i
      if( post_is_subset )
         return;
      //
      // rest of elements in set i
      while( i_value < end_ )
      {  temporary_.push_back(i_value);
         i_next  = data_[i_next].next;
         i_value = data_[i_next].value;
      }

      // adjust number_not_used_
      s_type number_drop = drop(i);
      number_not_used_  += number_drop;

      // put new set in linked list for set i
      CPPAD_ASSERT_UNKNOWN( s_type( temporary_.size() ) >= number_post + 1 );
      s_type index        = get_data_index();
      start_[i]           = index; // start for the union
      data_[index].value  = 1;    // reference count for the union
      for(s_type j = number_post; j < s_type( temporary_.size() ); ++j)
      {  next              = get_data_index();
         data_[index].next = next;
         data_[next].value = temporary_[j]; // next element in union
         index             = next;
      }
      data_[index].next = 0; // end of union
      //
      return;
   }
/*
-------------------------------------------------------------------------------
{xrst_begin size_setvec_is_element dev}

class size_setvec: Is an Element in a Set
#########################################

SetVector Concept
*****************
:ref:`SetVector@is_element`

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   bool is_element(s_type i, s_type element) const
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_is_element}
*/
   {  CPPAD_ASSERT_UNKNOWN( post_[i] == 0 );
      CPPAD_ASSERT_UNKNOWN( element < end_ );
      //
      s_type start = start_[i];
      if( start == 0 )
         return false;
      //
      s_type next  = data_[start].next;
      s_type value = data_[next].value;
      while( value < element )
      {  next  = data_[next].next;
         value = data_[next].value;
      }
      return element == value;
   }
/*
-------------------------------------------------------------------------------
{xrst_begin size_setvec_clear dev}

class size_setvec: Assign a Set to be Empty
###########################################

SetVector Concept
*****************
:ref:`SetVector@clear`

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void clear(s_type target)
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_clear}
*/
   {  CPPAD_ASSERT_UNKNOWN( target < s_type( start_.size() ) );

      // adjust number_not_used_
      s_type number_drop = drop(target);
      number_not_used_  += number_drop;

      return;
   }
/*
-------------------------------------------------------------------------------
{xrst_begin size_setvec_assignment dev}

class size_setvec: Assign a Set To Equal Another Set
####################################################

SetVector Concept
*****************
:ref:`SetVector@assignment`

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void assignment(
      s_type               this_target  ,
      s_type               other_source ,
      const size_setvec&   other        )
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_assignment}
*/
   {  CPPAD_ASSERT_UNKNOWN( other.post_[ other_source ] == 0 );
      //
      CPPAD_ASSERT_UNKNOWN( this_target < s_type( start_.size() )        );
      CPPAD_ASSERT_UNKNOWN( other_source <   s_type( other.start_.size() )  );
      CPPAD_ASSERT_UNKNOWN( end_        == other.end_   );

      // check if we are assigning a set to itself
      if( (this == &other) && (this_target == other_source) )
         return;

      // set depending on cases below
      s_type this_start;

      // If this and other are the same, use another reference to same list
      s_type other_start = other.start_[other_source];
      if( this == &other )
      {  this_start = other_start;
         if( other_start != 0 )
         {  data_[other_start].value++; // increment reference count
            CPPAD_ASSERT_UNKNOWN( data_[other_start].value > 1 );
         }
      }
      else if( other_start  == 0 )
      {  this_start = 0;
      }
      else
      {  // make a copy of the other list in this size_setvec
         this_start        = get_data_index();
         s_type this_next  = get_data_index();
         data_[this_start].value = 1; // reference count
         data_[this_start].next  = this_next;
         //
         s_type next  = other.data_[other_start].next;
         CPPAD_ASSERT_UNKNOWN( next != 0 );
         while( next != 0 )
         {  data_[this_next].value = other.data_[next].value;
            next                   = other.data_[next].next;
            if( next == 0 )
               data_[this_next].next = 0;
            else
            {  s_type tmp = get_data_index();
               data_[this_next].next = tmp;
               this_next             = tmp;
            }
         }
      }

      // adjust number_not_used_
      s_type number_drop = drop(this_target);
      number_not_used_  += number_drop;

      // set the new start value for this_target
      start_[this_target] = this_start;

      return;
   }
/*
-------------------------------------------------------------------------------
{xrst_begin size_setvec_binary_union dev}

class size_setvec: Assign a Set To Union of Two Sets
####################################################

SetVector Concept
*****************
:ref:`SetVector@binary_union`

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void binary_union(
      s_type                  this_target  ,
      s_type                  this_left    ,
      s_type                  other_right  ,
      const size_setvec&      other        )
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_binary_union}
*/
   {  CPPAD_ASSERT_UNKNOWN( post_[this_left] == 0 );
      CPPAD_ASSERT_UNKNOWN( other.post_[ other_right ] == 0 );
      //
      CPPAD_ASSERT_UNKNOWN( this_target < s_type( start_.size() )         );
      CPPAD_ASSERT_UNKNOWN( this_left < s_type( start_.size() )         );
      CPPAD_ASSERT_UNKNOWN( other_right < s_type( other.start_.size() )   );
      CPPAD_ASSERT_UNKNOWN( end_        == other.end_           );

      // start indices for left and right sets
      s_type start_left    = start_[this_left];
      s_type start_right   = other.start_[other_right];

      // if right is empty, the result is the left set
      if( start_right == 0 )
      {  assignment(this_target, this_left, *this);
         return;
      }
      // if left is empty, the result is the right set
      if( start_left == 0 )
      {  assignment(this_target, other_right, other);
         return;
      }
      // if niether case holds, then both left and right are non-empty
      CPPAD_ASSERT_UNKNOWN( reference_count(this_left) > 0 );
      CPPAD_ASSERT_UNKNOWN( other.reference_count(other_right) > 0 );

      // we will use temparary_ for temporary storage of the union
      temporary_.resize(0);

      // for left next and value
      s_type next_left   = data_[start_left].next;
      s_type value_left  = data_[next_left].value;

      // right next and value
      s_type next_right  = other.data_[start_right].next;
      s_type value_right = other.data_[next_right].value;

      // both left and right set are non-empty
      CPPAD_ASSERT_UNKNOWN( value_left < end_ && value_right < end_ );

      // flag that detects if left is or right is a subset of the other
      bool left_is_subset  = true;
      bool right_is_subset = true;

      while( (value_left < end_) && (value_right < end_) )
      {  if( value_left == value_right )
         {  // value is in both sets
            temporary_.push_back(value_left);
            //
            // advance left
            next_left  = data_[next_left].next;
            value_left = data_[next_left].value;
            //
            // advance right
            next_right  = other.data_[next_right].next;
            value_right = other.data_[next_right].value;
         }
         else if( value_left < value_right )
         {  // need a value from left that is not in right
            left_is_subset = false;
            temporary_.push_back(value_left);
            //
            // advance left to its next element
            next_left  = data_[next_left].next;
            value_left = data_[next_left].value;
         }
         else
         {  CPPAD_ASSERT_UNKNOWN( value_right < value_left )
            // need a value from right that is not in left
            right_is_subset = false;
            temporary_.push_back(value_right);
            //
            // advance right to its next element
            next_right  = other.data_[next_right].next;
            value_right = other.data_[next_right].value;
         }
      }
      right_is_subset &= value_right == end_;
      left_is_subset  &= value_left  == end_;
      //
      // check right first in case they are equal will do this assignment
      if( right_is_subset )
      {  assignment(this_target, this_left, *this);
         return;
      }
      if( left_is_subset )
      {  assignment(this_target, other_right, other);
         return;
      }
      while( value_left < end_ )
      {  CPPAD_ASSERT_UNKNOWN( value_right == end_);
         temporary_.push_back(value_left);
         next_left  = data_[next_left].next;
         value_left = data_[next_left].value;
      }
      while( value_right < end_ )
      {  CPPAD_ASSERT_UNKNOWN( value_left == end_);
         temporary_.push_back(value_right);
         next_right  = other.data_[next_right].next;
         value_right = other.data_[next_right].value;
      }

      // adjust number_not_used_
      s_type number_drop = drop(this_target);
      number_not_used_  += number_drop;

      // put new set in linked for this_target
      CPPAD_ASSERT_UNKNOWN( s_type( temporary_.size() ) >= 2 );
      s_type index        = get_data_index();
      start_[this_target] = index; // start for the union
      data_[index].value  = 1;    // reference count for the union
      for(s_type i = 0; i < s_type( temporary_.size() ); ++i)
      {  s_type next       = get_data_index();
         data_[index].next = next;
         data_[next].value = temporary_[i]; // next element in union
         index             = next;
      }
      data_[index].next = 0; // end of union

      return;
   }
/*
-------------------------------------------------------------------------------
{xrst_begin size_setvec_binary_intersection dev}

class size_setvec: Assign a Set To Equal Another Set
####################################################

SetVector Concept
*****************
:ref:`SetVector@binary_intersection`

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void binary_intersection(
      s_type                  this_target  ,
      s_type                  this_left    ,
      s_type                  other_right  ,
      const size_setvec&      other        )
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_binary_intersection}
*/
   {  CPPAD_ASSERT_UNKNOWN( post_[this_left] == 0 );
      CPPAD_ASSERT_UNKNOWN( other.post_[ other_right ] == 0 );
      //
      CPPAD_ASSERT_UNKNOWN( this_target < s_type( start_.size() )         );
      CPPAD_ASSERT_UNKNOWN( this_left < s_type( start_.size() )         );
      CPPAD_ASSERT_UNKNOWN( other_right < s_type( other.start_.size() )   );
      CPPAD_ASSERT_UNKNOWN( end_        == other.end_           );

      // start indices for left and right sets
      s_type start_left    = start_[this_left];
      s_type start_right   = other.start_[other_right];

      // if left or right is empty, the result is empty
      if( (start_left == 0) || (start_right == 0) )
      {  clear(this_target);
         return;
      }
      // if niether case holds, then both left and right are non-empty
      CPPAD_ASSERT_UNKNOWN( reference_count(this_left) > 0 );
      CPPAD_ASSERT_UNKNOWN( other.reference_count(other_right) > 0 );

      // we will use temparary_ for temporary storage of the intersection
      temporary_.resize(0);

      // left next and value
      s_type next_left   = data_[start_left].next;
      s_type value_left  = data_[next_left].value;

      // right next and value
      s_type next_right  = other.data_[start_right].next;
      s_type value_right = other.data_[next_right].value;

      // both left and right set are non-empty
      CPPAD_ASSERT_UNKNOWN( value_left < end_ && value_right < end_ );

      // flag that detects if left is or right is a subset of the other
      bool left_is_subset  = true;
      bool right_is_subset = true;

      while( (value_left < end_) && (value_right < end_) )
      {  if( value_left == value_right )
         {  // value is in both sets
            temporary_.push_back(value_left);
            //
            // advance left
            next_left  = data_[next_left].next;
            value_left = data_[next_left].value;
            //
            // advance right
            next_right  = other.data_[next_right].next;
            value_right = other.data_[next_right].value;
         }
         else if( value_left < value_right )
         {  // there is a value in left that is not in right
            left_is_subset = false;
            //
            // advance left to its next element
            next_left  = data_[next_left].next;
            value_left = data_[next_left].value;
         }
         else
         {  CPPAD_ASSERT_UNKNOWN( value_right < value_left )
            // there is a value in right that is not in left
            right_is_subset = false;
            //
            // advance right to its next element
            next_right  = other.data_[next_right].next;
            value_right = other.data_[next_right].value;
         }
      }
      right_is_subset &= value_right == end_;
      left_is_subset  &= value_left  == end_;
      //
      // check left first in case they are equal will do this assignment
      if( left_is_subset )
      {  assignment(this_target, this_left, *this);
         return;
      }
      if( right_is_subset )
      {  assignment(this_target, other_right, other);
         return;
      }

      // adjust number_not_used_
      s_type number_drop = drop(this_target);
      number_not_used_  += number_drop;

      // check for empty result
      if( s_type( temporary_.size() ) == 0 )
         return;

      // put new set in linked for this_target
      s_type index        = get_data_index();
      start_[this_target] = index; // start for the union
      data_[index].value  = 1;    // reference count for the union
      for(s_type i = 0; i < s_type( temporary_.size() ); ++i)
      {  s_type next       = get_data_index();
         data_[index].next = next;
         data_[next].value = temporary_[i]; // next element in union
         index             = next;
      }
      data_[index].next = 0; // end of union

      return;
   }
// =========================================================================
}; // END_CLASS_LIST_SETVEC
// =========================================================================

// =========================================================================
template <class s_type>
class size_setvec_const_iterator { // BEGIN_CLASS_SIZE_SETVEC_CONST_ITERATOR
// =========================================================================

/*
{xrst_begin size_setvec_const_iterator_member_data dev}

class size_setvec_const_iterator private: Member Data
#####################################################

pair_s_type
***********
This type is the same as
:ref:`size_setvec pair_s_type<size_setvec_member_data@pair_s_type>` .

end\_
*****
This is
:ref:`size_setvec_member_data@end_`
for the ``size_setvec`` object this iterator refers to.

data\_
******
This is
:ref:`size_setvec_member_data@data_`
for the ``size_setvec`` object this iterator refers to.

next_pair\_
***********
Next element in the set that this iterator refers to.
If ``next_pair_.value == end_`` there are no more elements in the set.

Source Code
***********
{xrst_spell_off}
{xrst_code hpp} */
private:
   typedef typename size_setvec<s_type>::pair_s_type pair_s_type;
   const s_type                     end_;
   const pod_vector<pair_s_type>&   data_;
   pair_s_type                      next_pair_;
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_const_iterator_member_data}
-------------------------------------------------------------------------------
{xrst_begin size_setvec_const_iterator_ctor dev}

class size_setvec_const_iterator: Constructor
#############################################

SetVector Concept
*****************
:ref:`iterator constructor<SetVector@const_iterator@Constructor>`

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   size_setvec_const_iterator (const size_setvec<s_type>& list, s_type i)
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_const_iterator_ctor}
*/
   : end_ ( list.end_ ), data_( list.data_ )
   {  CPPAD_ASSERT_UNKNOWN( list.post_[i] == 0 );
      //
      s_type start = list.start_[i];
      if( start == 0 )
      {  next_pair_.next  = 0;
         next_pair_.value = end_;
      }
      else
      {  // value for this entry is reference count for list
         CPPAD_ASSERT_UNKNOWN( data_[start].value > 0 );

         // data index where list truely starts
         s_type next = data_[start].next;
         CPPAD_ASSERT_UNKNOWN( next != 0 );

         // true first entry in the list
         next_pair_ = data_[next];
         CPPAD_ASSERT_UNKNOWN( next_pair_.value < end_ );
      }
   }
/*
-------------------------------------------------------------------------------
{xrst_begin size_setvec_const_iterator_dereference dev}

class size_setvec_const_iterator: Dereference
#############################################

SetVector Concept
*****************
:ref:`iterator deference<SetVector@const_iterator@Dereference>`

Implementation
**************
{xrst_spell_off}
{xrst_code hpp} */
public:
   s_type operator*(void)
   {  return next_pair_.value; }
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_const_iterator_dereference}
-------------------------------------------------------------------------------
{xrst_begin size_setvec_const_iterator_increment dev}

class size_setvec_const_iterator: Increment
###########################################

SetVector Concept
*****************
:ref:`iterator increment<SetVector@const_iterator@Increment>`

Implementation
**************
{xrst_spell_off}
{xrst_code hpp} */
public:
   size_setvec_const_iterator& operator++(void)
   {  next_pair_  = data_[next_pair_.next];
      return *this;
   }
/* {xrst_code}
{xrst_spell_on}

{xrst_end size_setvec_const_iterator_increment}
*/
// ===========================================================================
}; // END_CLASS_SIZE_SETVEC_CONST_ITERATOR
// ===========================================================================

// Implemented after size_setvec_const_iterator so can use it
template <class s_type>
inline void size_setvec<s_type>::print(void) const
{  std::cout << "size_setvec:\n";
   for(s_type i = 0; i < n_set(); i++)
   {  std::cout << "set[" << i << "] = {";
      const_iterator itr(*this, i);
      while( *itr != end() )
      {  std::cout << *itr;
         if( *(++itr) != end() ) std::cout << ",";
      }
      std::cout << "}\n";
   }
   return;
}
// ----------------------------------------------------------------------------

} } } // END_CPPAD_LOCAL_SPARSE_NAMESPACE

# endif
