# ifndef CPPAD_UTILITY_VECTOR_HPP
# define CPPAD_UTILITY_VECTOR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------


# include <cstddef>
# include <iostream>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/utility/check_simple_vector.hpp>
# include <cppad/utility/thread_alloc.hpp>

// Note that CPPAD_CONST is undefined by cppad_vector_itr.hpp
# define CPPAD_CONST 0
# include <cppad/local/utility/cppad_vector_itr.hpp>
# undef  CPPAD_LOCAL_UTILITY_CPPAD_VECTOR_ITR_HPP
# define CPPAD_CONST 1
# include <cppad/local/utility/cppad_vector_itr.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// ==========================================================================
template <class Type> class vector {
// ==========================================================================
/*
{xrst_begin cppad_vector_member dev}

Vector Class: Member Data
#########################

Syntax
******
| *vec* . ``capacity`` ()
| *vec* . ``size`` ()
| *vec* . ``data`` ()

Type
****
is the type of the elements in the array.

capacity\_
**********
Number of *Type* elements in ``data_`` that have been allocated
(and constructor has been called).

length\_
********
Number of *Type* elements currently in this vector.

data\_
******
Pointer to the first element of the vector
(not defined and should not be used when ``capacity_`` is  0).

Source
******
{xrst_spell_off}
{xrst_code hpp} */
private:
   size_t capacity_;
   size_t length_;
   Type*  data_;
public:
   size_t capacity(void) const noexcept
   {  return capacity_; }
   size_t size(void) const noexcept
   {  return length_; }
   const Type* data(void) const noexcept
   {  return data_; }
   Type* data(void) noexcept
   {  return data_; }
/* {xrst_code}
{xrst_spell_on}

{xrst_end cppad_vector_member}
-----------------------------------------------------------------------------
{xrst_begin cppad_vector_typedef dev}

Vector Class: Type Definitions
##############################

value_type
**********
type corresponding to an element of a vector.

iterator
********
type corresponding to an iterator for a vector.

const_iterator
**************
type corresponding to an iterator for a vector when
the vector is ``const`` .

{xrst_spell_off}
{xrst_code hpp} */
public:
   typedef Type                                         value_type;
   typedef local::utility::cppad_vector_itr<Type>       iterator;
   typedef local::utility::const_cppad_vector_itr<Type> const_iterator;
/* {xrst_code}
{xrst_spell_on}

{xrst_end cppad_vector_typedef}
-----------------------------------------------------------------------------
{xrst_begin cppad_vector_ctor dev}
{xrst_spell
   initializer
}

Vector Class: Constructors and Destructor
#########################################

Default
*******

   ``vector`` < *Type* > *vec*

creates an empty vector no elements and capacity zero.

Sizing
******

|  ``vector`` < *Type* > *vec* ( *n* )
|  ``vector`` < *Type* > *vec* ( *n* , *value* )

where *n* is a ``size_t`` , ``unsigned int`` , or ``int`` .
This creates the vector *vec* with *n* elements and capacity
greater than or equal *n* .

Value
=====
If *value* is present, it is a *Type* object and all of the
elements of the vector are given this value.

Initializer List
****************

   ``vector`` < *Type > *vec* ( { *e_1* , ... , *e_n* } )

Copy
****

   ``vector`` < *Type* > *vec* ( *other* )

where *other* is a ``vector`` < *Type* > ,
creates the vector *vec*
with *n* = *other* . ``size`` () elements and capacity
greater than or equal *n* .

Move Semantics
**************
A move semantics version of the copy operator
is implemented using ``swap`` .

Destructor
**********
If ``capacity_`` is non-zero, call the destructor
for all the corresponding elements and then frees the corresponding memory.

delete_data
***********
Call destructor and free all the allocated elements
(there are ``capacity_`` such elements).

Source
******
{xrst_spell_off}
{xrst_code hpp} */
public:
   // default
   vector(void) noexcept
   : capacity_(0), length_(0), data_(nullptr)
   { }
   //
   // sizing
   vector(size_t n) : capacity_(0), length_(0), data_(nullptr)
   {  resize(n); }
# if ! CPPAD_IS_SAME_UNSIGNED_INT_SIZE_T
   vector(unsigned int n) : capacity_(0), length_(0), data_(nullptr)
   {  resize(n); }
# endif
   vector(int n) : capacity_(0), length_(0), data_(nullptr)
   {  resize(n); }
   //
   // sizing with value
   vector(size_t n, const Type& value)
   : capacity_(0), length_(0), data_(nullptr)
   {  resize(n);
      for(size_t i = 0; i < length_; ++i)
         data_[i] = value;
   }
# if ! CPPAD_IS_SAME_UNSIGNED_INT_SIZE_T
   vector(unsigned int n, const Type& value)
   : capacity_(0), length_(0), data_(nullptr)
   {  resize(n);
      for(size_t i = 0; i < length_; ++i)
         data_[i] = value;
   }
# endif
   vector(int n, const Type& value)
   : capacity_(0), length_(0), data_(nullptr)
   {  resize(n);
      for(size_t i = 0; i < length_; ++i)
         data_[i] = value;
   }
   //
   // initializer list
   vector(std::initializer_list<Type> list) :
   capacity_(0), length_(0), data_(nullptr)
   {  for(auto itr = list.begin(); itr != list.end(); ++itr)
         this->push_back(*itr);
   }
   //
   // copy
   vector(const vector& other) : capacity_(0), length_(0), data_(nullptr)
   {  resize(other.length_);
      for(size_t i = 0; i < length_; i++)
         data_[i] = other.data_[i];
   }
   // capacity_ is only value required to make destructor work for other
   // after this move semantics constuctor
   vector(vector&& other) : capacity_(0), length_(0), data_(nullptr)
   {  swap(other); }
   ~vector(void)
   {  if( capacity_ > 0 ) delete_data(data_); }
private:
   void delete_data(Type* data_ptr)
   {  thread_alloc::delete_array(data_ptr); }
/* {xrst_code}
{xrst_spell_on}

{xrst_end cppad_vector_ctor}
-----------------------------------------------------------------------------
{xrst_begin cppad_vector_size dev}

Vector Class: Change Size
#########################

Syntax
******

| |tab|  *vec* . ``resize`` ( *n* )
| |tab| *vec* . ``clear`` ()

n
*
is a ``size_t`` , ``unsigned int`` , or ``int`` specifying
the number of elements in the new version of the vector.

resize
******
If *n* is less than or equal the input value of
*vec* . ``capacity_`` ,
the only change is that *vec* . ``length_`` is set to *n* .
Otherwise, new memory is allocated for the vector and
*vec* . ``length_`` elements are copied from the old vector
to the new one. I you do not need the old elements, you can first resize
to zero and then the *n* to avoid copying the elements.

clear
*****
The destructor is called for all the elements of *vec*
and then *vec.length_* and *vec* . ``capacity_`` are set to zero.

{xrst_end cppad_vector_size}
------------------------------------------------------------------------------
*/
public:
# if ! CPPAD_IS_SAME_UNSIGNED_INT_SIZE_T
   void resize(unsigned int n)
   {  resize( size_t(n) ); }
# endif
   void resize(int n)
   {  CPPAD_ASSERT_KNOWN(
         n >= 0,
         "CppAD::vector: attempt to create a vector with a negative size."
      );
      resize( size_t(n) );
   }
   void resize(size_t n)
   {  if( capacity_ < n )
      {  if( capacity_ == 0 )
         {  // get new memory and set capacity
            data_ = thread_alloc::create_array<Type>(n, capacity_);
         }
         else
         {  // save old information
            Type*  old_data     = data_;

            // get new memory and set capacity
            data_ = thread_alloc::create_array<Type>(n, capacity_);

            // copy old data
            for(size_t i = 0; i < length_; ++i)
               data_[i] = old_data[i];

            // free old memory
            thread_alloc::delete_array(old_data);
         }
      }
      length_ = n;
   }
   void clear(void)
   {  length_ = 0;
      // check if there is old memory to be freed
      if( capacity_ > 0 )
         delete_data(data_);
      capacity_ = 0;
   }
/*
-------------------------------------------------------------------------------
{xrst_begin cppad_vector_assign dev}

Vector Class: Assignment Operators
##################################

Syntax
******

   *vec* . ``swap`` ( *other* )

*vec* = *other*

Prototype
*********
{xrst_literal
   // BEGIN_SWAP
   // END_SWAP
}
{xrst_literal
   // BEGIN_MOVE_ASSIGN
   // END_MOVE_ASSIGN
}
{xrst_literal
   // BEGIN_ASSIGN
   // END_ASSIGN
}

swap
****
Swaps ``length_`` , ``capacity_`` and ``data_``
between *vec* and *other* .

Assignment
**********
see :ref:`user API assignment<CppAD_vector@Assignment>`

Move Semantics
**************
The move semantics version of the assignment operator
is implemented using ``swap`` .

{xrst_end cppad_vector_assign}
-------------------------------------------------------------------------------
*/
// BEGIN_SWAP
public:
   // swap does not do any allocation and hence is declared noexcept
   void swap(vector& other) noexcept
// END_SWAP
   {  // special case where vec and other are the same vector
       if( this == &other )
         return;
      //
      std::swap(length_,   other.length_   );
      std::swap(capacity_, other.capacity_ );
      std::swap(data_,     other.data_     );
      return;
   }

// BEGIN_MOVE_ASSIGN
   // move assingment does not doe any allocation and hence is declared noexcept
   vector& operator=(vector&& other) noexcept
// END_MOVE_ASSIGN
   {  swap(other);
      return *this;
   }

// BEGIN_ASSIGN
   vector& operator=(const vector& other)
// END_ASSIGN
   {  // avoid copying old elements
      resize(0);
      // new size for this vector
      resize( other.length_ );
      // copy elements from other
      for(size_t i = 0; i < length_; i++)
         data_[i] = other.data_[i];
      return *this;
   }
/*
-------------------------------------------------------------------------------
{xrst_begin cppad_vector_compare dev}
{xrst_spell
   ture
}

Vector Class: Comparison Operators
##################################

Syntax
******

|  *result* = *vec* *op* *other*

where *op* is one of the following: == , != , < , <= , > , >= .

Prototype
*********
{xrst_literal ,
   // BEGIN_EQUAL, // END_EQUAL
   // BEGIN_NOT_EQUAL, // END_NOT_EQUAL
   // BEGIN_LESS_THAN, // END_LESS_THAN
   // BEGIN_LESS_OR_EQUAL, // END_LESS_OR_EQUAL
   // BEGIN_GREATER_THAN, // END_GREATER_THAN
   // BEGIN_GREATER_OR_EQUAL, // END_GREATER_OR_EQUAL
}

vec
***
is the left side for the comparison operation.

other
*****
is the right side for the comparison operation.
This vector must have the same size as *vec* .

result
******
The *result* is true if the comparison

|  *vec* [ *i* ] *op* *other [ *i* ]

ture for all valid indices *i* .  Otherwise, the *result* is false

{xrst_end cppad_vector_compare}
-------------------------------------------------------------------------------
*/
// BEGIN_EQUAL
public:
   bool operator==(const vector& other) const
// END_EQUAL
   {  bool result = true;
      CPPAD_ASSERT_KNOWN(
         length_ == other.length_ ,
         "CppAD::vector: vec == other: size of vectors is different"
      );
      for(size_t i = 0; i < length_; ++i)
         result &= data_[i] == other.data_[i];
      return result;
   }
// BEGIN_NOT_EQUAL
   bool operator!=(const vector& other) const
// END_NOT_EQUAL
   {  bool result = true;
      CPPAD_ASSERT_KNOWN(
         length_ == other.length_ ,
         "CppAD::vector: vec == other: size of vectors is different"
      );
      for(size_t i = 0; i < length_; ++i)
         result &= data_[i] != other.data_[i];
      return result;
   }
// BEGIN_LESS_THAN
   bool operator<(const vector& other) const
// END_LESS_THAN
   {  bool result = true;
      CPPAD_ASSERT_KNOWN(
         length_ == other.length_ ,
         "CppAD::vector: vec == other: size of vectors is different"
      );
      for(size_t i = 0; i < length_; ++i)
         result &= data_[i] < other.data_[i];
      return result;
   }
// BEGIN_LESS_OR_EQUAL
   bool operator<=(const vector& other) const
// END_LESS_OR_EQUAL
   {  bool result = true;
      CPPAD_ASSERT_KNOWN(
         length_ == other.length_ ,
         "CppAD::vector: vec == other: size of vectors is different"
      );
      for(size_t i = 0; i < length_; ++i)
         result &= data_[i] <= other.data_[i];
      return result;
   }
// BEGIN_GREATER_THAN
   bool operator>(const vector& other) const
// END_GREATER_THAN
   {  bool result = true;
      CPPAD_ASSERT_KNOWN(
         length_ == other.length_ ,
         "CppAD::vector: vec == other: size of vectors is different"
      );
      for(size_t i = 0; i < length_; ++i)
         result &= data_[i] > other.data_[i];
      return result;
   }
// BEGIN_GREATER_OR_EQUAL
   bool operator>=(const vector& other) const
// END_GREATER_OR_EQUAL
   {  bool result = true;
      CPPAD_ASSERT_KNOWN(
         length_ == other.length_ ,
         "CppAD::vector: vec == other: size of vectors is different"
      );
      for(size_t i = 0; i < length_; ++i)
         result &= data_[i] >= other.data_[i];
      return result;
   }
/*
-------------------------------------------------------------------------------
{xrst_begin cppad_vector_subscript dev}

Vector Class: Subscript Operator
################################

Syntax
******

| *element* = *vec* [ *i* ]
| *vec* [ *i* ] = *element*

Source
******
{xrst_spell_off}
{xrst_code hpp} */
   const Type& operator[]( size_t i) const
   {  CPPAD_ASSERT_KNOWN( i < length_,
         "vector: index greater than or equal vector size"
      );
      return data_[i];
   }
   Type& operator[](size_t i)
   {  CPPAD_ASSERT_KNOWN(i < length_,
         "vector: index greater than or equal vector size"
      );
      return data_[i];
   }
   template <class Index> const Type& operator[]( Index i) const
   {  return (*this)[size_t(i)]; }
   template <class Index> Type& operator[](Index i)
   {  return (*this)[size_t(i)]; }
/* {xrst_code}
{xrst_spell_on}

{xrst_end cppad_vector_subscript}
-------------------------------------------------------------------------------
{xrst_begin cppad_vector_push_back dev}

Vector Class: push_back
#######################

Syntax
******
*vec* . ``push_back`` ( *element* )

Prototype
*********
{xrst_literal
   // BEGIN_PUSH_BACK
   // END_PUSH_BACK
}

Documentation
*************
see :ref:`use API push_back<cppad_vector_push_back-name>`

{xrst_end cppad_vector_push_back}
*/
// BEGIN_PUSH_BACK
   void push_back(const Type& element)
// END_PUSH_BACK
   {  // case where no allocation is necessary
      if( length_ < capacity_ )
      {  data_[length_++] = element;
         return;
      }
      CPPAD_ASSERT_UNKNOWN( length_ == capacity_ );

      // create new vector with required size
      vector vec(length_ + 1);

      // copy old data values
      for(size_t i = 0; i < length_; ++i)
         vec.data_[i] = data_[i];

      // put the new element in the new vector
      CPPAD_ASSERT_UNKNOWN( vec.length_ == length_ + 1);
      vec.data_[length_] = element;

      // swap old and new vectors
      swap(vec);
   }
/* %$$
-------------------------------------------------------------------------------
{xrst_begin cppad_vector_push_vector dev}

Vector Class: push_vector
#########################

Syntax
******
*vec* . ``push_vector`` ( *other* )

Prototype
*********
{xrst_literal
   // BEGIN_PUSH_VECTOR
   // END_PUSH_VECTOR
}

Documentation
*************
see :ref:`use API push_vector<cppad_vector_push_vector-name>`

{xrst_end cppad_vector_push_vector}
*/
// BEGIN_PUSH_VECTOR
   template <class Vector> void push_vector(const Vector& other)
// END_PUSH_VECTOR
   {  // can not use push_back because MS V++ 7.1 did not resolve
      // to non-template member function when scalar is used.
      //
      CheckSimpleVector<Type, Vector>();
      size_t m = other.size();

      // case where no allcoation is necessary
      if( length_ + m <= capacity_ )
      {  for(size_t i = 0; i < m; i++)
            data_[length_++] = other[i];
         return;
      }

      // create new vector with required size
      vector vec(length_ + m);

      // copy old data values
      for(size_t i = 0; i < length_; ++i)
         vec.data_[i] = data_[i];

      // put the new elements in the new vector
      CPPAD_ASSERT_UNKNOWN( vec.length_ == length_ + m );
      for(size_t i = 0; i < m; i++)
         vec.data_[length_ + i] = other[i];

      // swap old and new vectors
      swap(vec);
   }
/*
------------------------------------------------------------------------------
{xrst_begin cppad_vector_itr_fun dev}

Vector Class: Iterator Functions
################################

Syntax
******

| ``os`` *vec* . ``begin`` ()
| *os* ``vec`` . *end* ()

Source
******
{xrst_spell_off}
{xrst_code hpp} */
   const_iterator begin(void) const noexcept
   {    return const_iterator(&data_, &length_, 0); }
   const_iterator end(void) const noexcept
   {  typedef typename const_iterator::difference_type difference_type;
      difference_type index = static_cast<difference_type>(length_);
      return const_iterator(&data_, &length_, index);
   }
   //
   iterator begin(void) noexcept
   {    return iterator(&data_, &length_, 0); }
   iterator end(void) noexcept
   {  typedef typename iterator::difference_type difference_type;
      difference_type index = static_cast<difference_type>(length_);
      return iterator(&data_, &length_, index);
   }
/* {xrst_code}
{xrst_spell_on}

{xrst_end cppad_vector_itr_fun}
*/

// =========================================================================
};  // END_TEMPLATE_CLASS_VECTOR
// =========================================================================

/*
{xrst_begin cppad_vector_output dev}

Vector Class: Output
####################

Syntax
******
*os* << ``vec``

Source
******
{xrst_spell_off}
{xrst_code hpp} */
template <class Type>
std::ostream& operator << (std::ostream&  os , const CppAD::vector<Type>& vec )
{  os << "{ ";
   for(size_t i = 0; i < vec.size(); ++i)
   {  os << vec[i];
      if( i + 1 < vec.size() )
         os << ", ";
   }
   os << " }";
   return os;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end cppad_vector_output}
*/

} // END_CPPAD_NAMESPACE

// user API specifies that vector_bool.hpp is included by vector.hpp
# include <cppad/utility/vector_bool.hpp>

# endif
