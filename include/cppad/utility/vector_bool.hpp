# ifndef CPPAD_UTILITY_VECTOR_BOOL_HPP
# define CPPAD_UTILITY_VECTOR_BOOL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------


# include <cstddef>
# include <iostream>
# include <limits>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/utility/check_simple_vector.hpp>
# include <cppad/utility/thread_alloc.hpp>
# include <cppad/local/utility/vector_bool.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// ============================================================================
class vectorBool {
// ============================================================================
/*
{xrst_begin vector_bool_member dev}

vectorBool: Member Data
#######################

Syntax
******
| *vec* . ``unit_min`` ()
| *vec* . ``bit_per_unit`` ()

unit_t
******
Type used to pack multiple boolean (bit) values into one unit.
Logical operations are preformed one unit at a time.

bit_per_unit\_
**************
number of bits packed into each unit value in ``data_`` .

n_unit\_
********
Number of unit values in ``data_`` .

length\_
********
number of bits currently stored in this vector.

data\_
******
pointer to where the bits are stored.

unit_min
********
minimum number of ``unit_t`` values that can store ``length_`` bits.
Note that this is really a function of ``length_`` .

size
****
is the number of boolean elements in the vector.

capacity
********
is the maximum number of boolean elements that will fit in the
current allocation for ``data_`` .

Source
******
{xrst_spell_off}
{xrst_code hpp} */
private:
   typedef size_t unit_t;
   static const size_t bit_per_unit_ = std::numeric_limits<unit_t>::digits;
   size_t    n_unit_;
   size_t    length_;
   unit_t    *data_;
   //
   size_t unit_min(void) const
   {  if( length_ == 0 )
         return 0;
      return (length_ - 1) / bit_per_unit_ + 1;
   }
public:
   static size_t bit_per_unit(void)
   {  return bit_per_unit_; }
   size_t size(void) const
   {  return length_; }
   size_t capacity(void) const
   {  return n_unit_ * bit_per_unit_; }
/* {xrst_code}
{xrst_spell_on}

{xrst_end vector_bool_member}
-------------------------------------------------------------------------------
{xrst_begin vector_bool_typedef dev}

vectorBool Type Definitions
###########################

value_type
**********
type corresponding to the elements of this vector
(note that non-const elements actually use
:ref:`vectorBoolElement<vector_bool_element-name>` ).

Source
******
{xrst_spell_off}
{xrst_code hpp} */
public:
   typedef bool value_type;
/* {xrst_code}
{xrst_spell_on}

{xrst_end vector_bool_typedef}
----------------------------------------------------------------------------
{xrst_begin vector_bool_ctor dev}
{xrst_spell
   initializer
}
vectorBool: Constructors and Destructor
#######################################

Default
*******

   ``vectorBool`` *vec*

creates an empty vector with no elements and ``n_unit_`` zero.

Sizing
******

   ``vectorBool`` *vec* ( *n* )

where *n* is a ``size_t`` ,
creates the vector *vec* with *n* elements and ``n_unit_``
greater than or equal ``unit_min()`` .

Initializer
***********

   ``vectorBool`` *vec* ( { *b_1* , ... , *b_n* } )


Copy
****

   ``vector`` < *Type* > *vec* ( *other* )

where *other* is a ``vector`` < *Type* > ,
creates the vector *vec*
with *n* = *other* . ``size`` () elements and ``n_unit_``
greater than or equal ``unit_min()`` .

Destructor
**********
If ``n_unit_`` is non-zero, the memory corresponding to ``data_``
is returned to thread_alloc.

Source
******
{xrst_spell_off}
{xrst_code hpp}:
*/
   // default
   vectorBool(void) : n_unit_(0), length_(0), data_(nullptr)
   { }
   //
   // sizing
   vectorBool(size_t n) : n_unit_(0), length_(0), data_(nullptr)
   {  resize(n); }
   vectorBool(int n) : n_unit_(0), length_(0), data_(nullptr)
   {  resize( size_t(n) ); }
# if ! CPPAD_IS_SAME_UNSIGNED_INT_SIZE_T
   vectorBool(unsigned int n) : n_unit_(0), length_(0), data_(nullptr)
   {  resize( size_t(n) ); }
# endif
   //
   // copy
   vectorBool(const vectorBool& other)
   : n_unit_(0), length_(0), data_(nullptr)
   {  resize(other.length_);
      size_t n_used = unit_min();
      CPPAD_ASSERT_UNKNOWN( n_used <= n_unit_ );
      for(size_t i = 0; i < n_used; ++i)
         data_[i] = other.data_[i];
   }
   vectorBool(std::initializer_list<bool> list)
   : n_unit_(0), length_(0), data_(nullptr)
   {  for(auto itr = list.begin(); itr != list.end(); ++itr)
         this->push_back(*itr);
   }
   // n_unit_ is the only value necessary to make destructor work
   // for other after this move semantics constructor
   vectorBool(vectorBool&& other)
   : n_unit_(0), length_(0), data_(nullptr)
   {  swap(other); }
   ~vectorBool(void)
   {  clear(); }
/* {xrst_code}
{xrst_spell_on}

{xrst_end vector_bool_ctor}
-----------------------------------------------------------------------------
{xrst_begin vector_bool_size dev}

vectorBool: Change Size
#######################

Syntax
******

| |tab| *vec* . ``resize`` ( *n* )
| |tab| *vec* . ``clear`` ()

n
*
is a ``size_t`` or ``int`` specifying
the number of elements in the new version of the vector.

resize
******
If *n* is less than or equal the input value of
*vec* . ``n_unit_`` times *vec* . ``bit_per_unit_`` ,
the only change is that *vec* . ``length_`` is set to *n* .
Otherwise the old elements are freed and a new vector is created
with *vec* . ``length_`` equal to *n* .

clear
*****
the memory allocated for this vector is freed and
*vec.length_* and *vec* . ``n_unit_`` are set to zero.

{xrst_end vector_bool_size}
------------------------------------------------------------------------------
*/
// BEGIN_RESIZE
public:
# if ! CPPAD_IS_SAME_UNSIGNED_INT_SIZE_T
   void resize(unsigned int n)
   {  resize( size_t(n) );
   }
# endif
   void resize(int n)
   {  CPPAD_ASSERT_KNOWN(
         n >= 0,
         "CppAD::vector: attempt to create a vector with a negative size."
      );
      resize( size_t(n) );
   }
   void resize(size_t n)
// END_RESIZE
   {  length_ = n;
      // check if we can use the current memory
      size_t min_unit = unit_min();
      if( n_unit_ >= min_unit )
         return;
      // check if there is old memory to be freed
      if( n_unit_ > 0 )
      {  void* v_ptr = reinterpret_cast<void*>(data_);
         thread_alloc::return_memory(v_ptr);
      }
      // get new memory and set n_unit
      size_t min_bytes = min_unit * sizeof(unit_t);
      size_t cap_bytes;
      void* v_ptr = thread_alloc::get_memory(min_bytes, cap_bytes);
      data_       = reinterpret_cast<unit_t*>(v_ptr);
      n_unit_     = cap_bytes / sizeof(unit_t);
      CPPAD_ASSERT_UNKNOWN( n_unit_ >= min_unit );
   }
// BEGIN_CLEAR
   void clear(void)
// END_CLEAR
   {
      // check if there is old memory to be freed
      if( n_unit_ > 0 )
      {  void* v_ptr = reinterpret_cast<void*>(data_);
         thread_alloc::return_memory(v_ptr);
      }
      length_ = 0;
      n_unit_ = 0;
   }
/*
-------------------------------------------------------------------------------
{xrst_begin vector_bool_assign dev}

vectorBool: Assignment Operators
################################

Syntax
******

| |tab| *vec* . ``swap`` ( *other* )
| |tab| *vec* = *other*

Prototype
*********
{xrst_literal
   // BEGIN_SWAP
   // END_SWAP
}
{xrst_literal
   // BEGIN_ASSIGN
   // END_ASSIGN
}
{xrst_literal
   // BEGIN_MOVE_SEMANTICS
   // END_MOVE_SEMANTICS
}

swap
****
Swaps ``n_unit_`` , ``length_`` and ``data_``
between *vec* and *other* .

Assignment
**********
If the input value of *vec* . ``length_`` is zero,
:ref:`vector_bool_size@resize` is used to change its size to
be the same as other.
The size of *vec* and *other* are then compared and if
different, an assert with a know cause is generated.
The elements of *vec* are then individually assigned
to have the value of the corresponding elements of *other* .

Move Semantics
**************
A move semantics version of the assignment operator, implemented using
``swap`` , is defined.

{xrst_end vector_bool_assign}
-------------------------------------------------------------------------------
*/
// BEGIN_SWAP
   void swap(vectorBool& other)
// END_SWAP
   {  // swap with self case
       if( this == &other )
         return;
      std::swap(n_unit_,   other.n_unit_   );
      std::swap(length_,   other.length_   );
      std::swap(data_,     other.data_     );
      return;
   }
// BEGIN_ASSIGN
   vectorBool& operator=(const vectorBool& other)
// END_ASSIGN
   {  // If original length is zero, then resize it to other.
      // Otherwise a length mismatch is an error.
      if( length_ == 0 )
         resize( other.length_ );
      CPPAD_ASSERT_KNOWN(
         length_ == other.length_ ,
         "vectorBool: size miss match in assignment operation"
      );
      size_t n_used = unit_min();
      CPPAD_ASSERT_UNKNOWN( n_used <= n_unit_ );
      for(size_t i = 0; i < n_used; i++)
         data_[i] = other.data_[i];
      return *this;
   }
// BEGIN_MOVE_SEMANTICS
   vectorBool& operator=(vectorBool&& other)
// END_MOVE_SEMANTICS
   {  CPPAD_ASSERT_KNOWN(
         length_ == other.length_ || (length_ == 0),
         "vectorBool: size miss match in assignment operation"
      );
      swap(other);
      return *this;
   }
/*
-------------------------------------------------------------------------------
{xrst_begin vector_bool_subscript dev}

vectorBool: Subscript Operator
##############################

Syntax
******

| *target* = *vec* [ *i* ]
| *vec* [ *i* ] = *source*

target
******
In this syntax *vec* is ``const``
and the value *vec* [ *i* ] is a ``bool`` .

source
******
In this syntax *vec* is not ``const``
and the value *vec* [ *i* ] is a
:ref:`vectorBoolElement<vector_bool_element-name>` .

Source Code
***********
{xrst_spell_off}
{xrst_code hpp} */
   bool operator[](size_t i) const
   {  CPPAD_ASSERT_KNOWN( i < length_,
         "vectorBool: index greater than or equal vector size"
      );
      size_t unit_index   = i / bit_per_unit_;
      size_t bit_index    = i - unit_index * bit_per_unit_;
      unit_t unit         = data_[unit_index];
      unit_t mask         = unit_t(1) << bit_index;
      return (unit & mask) != 0;
   }
   local::utility::vectorBoolElement operator[](size_t i)
   {  CPPAD_ASSERT_KNOWN( i < length_,
         "vectorBool: index greater than or equal vector size"
      );
      size_t unit_index   = i / bit_per_unit_;
      size_t bit_index    = i - unit_index * bit_per_unit_;
      unit_t mask         = unit_t(1) << bit_index;
      return local::utility::vectorBoolElement(data_ + unit_index , mask);
   }
   template <class Index> bool operator[]( Index i) const
   {  return (*this)[size_t(i)]; }
   template <class Index>
   local::utility::vectorBoolElement operator[](Index i)
   {  return (*this)[size_t(i)]; }
/* {xrst_code}
{xrst_spell_on}

{xrst_end vector_bool_subscript}
-------------------------------------------------------------------------------
{xrst_begin vector_bool_push_back dev}

vectorBool: push_back
#####################

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

{xrst_end vector_bool_push_back}
*/
// BEGIN_PUSH_BACK
   void push_back(bool element)
// END_PUSH_BACK
   {  CPPAD_ASSERT_UNKNOWN( unit_min() <= n_unit_ );
      size_t old_length = length_;
      if( length_ + 1 > n_unit_ * bit_per_unit_ )
      {  CPPAD_ASSERT_UNKNOWN( unit_min() == n_unit_ );

         // create new vector with requuired size
         vectorBool vec(length_ + 1);

         // copy old data values
         size_t n_used = unit_min();
         CPPAD_ASSERT_UNKNOWN( n_used <= n_unit_ );
         for(size_t i = 0; i < n_used; ++i)
            vec.data_[i] = data_[i];

         // swap old and new vectors
         swap(vec);
      }
      else
         ++length_;
      CPPAD_ASSERT_UNKNOWN( length_ <= n_unit_ * bit_per_unit_ )
      size_t   unit_index = old_length / bit_per_unit_;
      size_t   bit_index  = old_length - unit_index * bit_per_unit_;
      unit_t mask         = unit_t(1) << bit_index;
      if( element )
         data_[unit_index] |= mask;
      else
         data_[unit_index] &= ~mask;
   }
/* %$$
-------------------------------------------------------------------------------
{xrst_begin vector_bool_push_vector dev}

vectorBool: push_vector
#######################

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

{xrst_end vector_bool_push_vector}
*/
// BEGIN_PUSH_VECTOR
   template <class Vector> void push_vector(const Vector& other)
// END_PUSH_VECTOR
   {  CPPAD_ASSERT_UNKNOWN( unit_min() <= n_unit_ );
      CheckSimpleVector<bool, Vector>();
      size_t old_length = length_;
      size_t m           = other.size();
      if( length_ + m > n_unit_ * bit_per_unit_ )
      {
         // create new vector with requuired size
         vectorBool vec(length_ + m);

         // copy old data values
         size_t n_used = unit_min();
         CPPAD_ASSERT_UNKNOWN( n_used <= n_unit_ );
         for(size_t i = 0; i < n_used; ++i)
            vec.data_[i] = data_[i];

         // swap old and new vectors
         swap(vec);
      }
      else
         length_ += m;
      //
      // put the new elements in this vector
      CPPAD_ASSERT_UNKNOWN( length_ <= n_unit_ * bit_per_unit_ )
      for(size_t k = 0; k < m; k++)
      {  size_t unit_index = (old_length + k) / bit_per_unit_;
         size_t bit_index  = (old_length + k) - unit_index * bit_per_unit_;
         unit_t mask       = unit_t(1) << bit_index;
         if( other[k] )
            data_[unit_index] |= mask;
         else
            data_[unit_index] &= ~mask;
      }
   }
};

/*
{xrst_begin vector_bool_output dev}

vectorBool: Output
##################

Syntax
******
*os* << ``vec``

Source
******
{xrst_spell_off}
{xrst_code hpp} */
inline std::ostream& operator << (std::ostream&  os , const vectorBool& vec )
{  for(size_t i = 0; i < vec.size(); ++i)
      os << vec[i];
   return os;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end vector_bool_output}
*/

} // END_CPPAD_NAMESPACE
# endif
