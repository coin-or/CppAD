# ifndef CPPAD_LOCAL_UTILITY_CPPAD_VECTOR_ITR_HPP
# define CPPAD_LOCAL_UTILITY_CPPAD_VECTOR_ITR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cstddef>
# include <cppad/core/cppad_assert.hpp>
/*
------------------------------------------------------------------------------
{xrst_begin cppad_vector_itr_define dev}
{xrst_spell
   undef
}

Vector Class Iterator Preprocessor Definitions
##############################################

Syntax
******
{xrst_code cpp}
# define CPPAD_CONST 0
# include <cppad/local/utility/cppad_vector_itr.hpp>
# undef CPPAD_LOCAL_UTILITY_CPPAD_VECTOR_ITR_HPP
# define CPPAD_CONST 1
# include <cppad/local/utility/cppad_vector_itr.hpp>
{xrst_code}

Beginning of cppad_vector_itr.hpp
*********************************
The following preprocessor definition appears at the beginning of
``cppad_vector_itr.hpp`` and is used for the class definition in this file:
::

   # if CPPAD_CONST
   # define CPPAD_VECTOR_ITR const_cppad_vector_itr
   # else
   # define CPPAD_VECTOR_ITR cppad_vector_itr
   # endif

End of cppad_vector_itr.hpp
***************************
The following preprocessor definition appears at the end of
``cppad_vector_itr.hpp`` so that it can be included with a different
value for ``CPPAD_CONST`` :
::

   # undef CPPAD_CONST
   # undef CPPAD_VECTOR_ITR

{xrst_end cppad_vector_itr_define}
*/
# if CPPAD_CONST
# define CPPAD_VECTOR_ITR const_cppad_vector_itr
# else
# define CPPAD_VECTOR_ITR cppad_vector_itr
# endif


// BEGIN_CPPAD_LOCAL_UTILITY_NAMESPACE
namespace CppAD { namespace local { namespace utility {

// const_cppad_vector_itr
// declare here can be make it a friend in cppad_vector_itr<Type>
template <class Type> class const_cppad_vector_itr;

// ==========================================================================
template <class Type> class CPPAD_VECTOR_ITR {
// ==========================================================================
/*
-----------------------------------------------------------------------------
{xrst_begin cppad_vector_itr_traits dev}

Vector Class Iterator Traits and Friends
########################################

{xrst_spell_off}
{xrst_code hpp} */
# if ! CPPAD_CONST
   friend class const_cppad_vector_itr<Type>;
# endif
public:
   typedef std::random_access_iterator_tag    iterator_category;
   typedef Type                               value_type;
   typedef std::ptrdiff_t                     difference_type;
   typedef Type*                              pointer;
   typedef Type&                              reference;
/* {xrst_code}
{xrst_spell_on}

{xrst_end cppad_vector_itr_traits}
-------------------------------------------------------------------------------
{xrst_begin cppad_vector_itr_ctor dev}

Vector Class Iterator Member Data and Constructors
##################################################

Constructors
************

Constant
========

| ``const_cppad_vector_itr`` *itr* ()
| ``const_cppad_vector_itr`` *itr* ( *data* , *length* , *index* )
| ``const_cppad_vector_itr`` *itr* ( *other* )
| ``const_cppad_vector_itr`` *itr* ( *non_const_other* )

Not Constant
============

| ``cppad_vector_itr`` *itr* ()
| ``cppad_vector_itr`` *itr* ( *data* , *length* , *index* )
| ``cppad_vector_itr`` *itr* ( *other* )

Namespace
*********
These definitions are in the ``CppAD::local::utility`` namespace.

Indirection
***********
We use an extra level of indirection in this routine so that
the iterator has the same values as the vector even if the vector changes.

data\_
******
is a pointer to a constant pointer to data for this vector
(used by operations that are not supported by constant iterators).

length\_
********
is a pointer to the length of the corresponding vector.

index\_
*******
is the current vector index corresponding to this iterator.

check_element
*************
generates an assert with a known cause when the ``index_``
does not correspond go a valid element and
``NDEBUG`` is not defined.

check_cop
*********
Generates an assert with a known cause when the ``data_``
for this vector is different from the other vector and
``NDEBUG`` is not defined.
This should be used by operators that compare iterators.

Source
******
{xrst_spell_off}
{xrst_code hpp} */
private:
# if CPPAD_CONST
   const Type* const* data_;
# else
   Type* const*       data_;
# endif
   const size_t*      length_;
   difference_type    index_;
   void check_element(void) const CPPAD_NDEBUG_NOEXCEPT
   {  CPPAD_ASSERT_KNOWN( 0 <= index_ && size_t(index_) < *length_,
         "CppAD vector iterator: accessing element out of range"
      );
   }
   void check_cop(const CPPAD_VECTOR_ITR& other) const CPPAD_NDEBUG_NOEXCEPT
   {  CPPAD_ASSERT_KNOWN( data_ == other.data_,
         "CppAD vector iterator: comparing indices from different vectors"
      );
   }
public:
   CPPAD_VECTOR_ITR(void) noexcept
   : data_(nullptr), length_(nullptr), index_(0)
   { }
# if CPPAD_CONST
   // ctor
   const_cppad_vector_itr(
      const Type* const* data, const size_t* length, difference_type index
   ) noexcept
   : data_(data), length_(length), index_( difference_type(index) )
   { }
   // ctor a const_iterator from an iterator. Here is were we need
   //  const_cppad_vector_itr to be friend of cppad_vector_itr
   const_cppad_vector_itr(
      const cppad_vector_itr<Type>& non_const_other
   ) noexcept
   {  data_       = non_const_other.data_;
      length_     = non_const_other.length_;
      index_      = non_const_other.index_;
   }
# else
   // ctor
   cppad_vector_itr(
      Type* const* data, const size_t* length, difference_type index
   ) noexcept
   : data_(data), length_(length), index_( difference_type(index) )
   { }
# endif
   void operator=(const CPPAD_VECTOR_ITR& other) noexcept
   {  data_       = other.data_;
      length_     = other.length_;
      index_      = other.index_;
   }
   CPPAD_VECTOR_ITR(const CPPAD_VECTOR_ITR& other) noexcept
   {  *this = other; }
/* {xrst_code}
{xrst_spell_on}

{xrst_end cppad_vector_itr_ctor}
-------------------------------------------------------------------------------
{xrst_begin cppad_vector_itr_inc dev}

Vector Class Iterator Increment Operators
#########################################

Syntax
******

| ++ *itr*
| ``--`` *itr*
| *itr* ++
| *itr* ``--``

Source
******
{xrst_spell_off}
{xrst_code hpp} */
public:
   CPPAD_VECTOR_ITR& operator++(void) noexcept
   {  ++index_;
      return *this;
   }
   CPPAD_VECTOR_ITR& operator--(void) noexcept
   {  --index_;
      return *this;
   }
   CPPAD_VECTOR_ITR operator++(int) noexcept
   {  CPPAD_VECTOR_ITR ret(*this);
      ++index_;
      return ret;
   }
   CPPAD_VECTOR_ITR operator--(int) noexcept
   {  CPPAD_VECTOR_ITR ret(*this);
      --index_;
      return ret;
   }
/* {xrst_code}
{xrst_spell_on}

{xrst_end cppad_vector_itr_inc}
-------------------------------------------------------------------------------
{xrst_begin cppad_vector_itr_equal dev}

Vector Class Iterator Equality Operators
########################################

Syntax
******

| *itr* == *other*
| *itr* != *other*

Restrictions
************
It is an error to compare iterators corresponding to different
``data_`` vectors

Source
******
{xrst_spell_off}
{xrst_code hpp} */
public:
   bool operator==(const CPPAD_VECTOR_ITR& other) const CPPAD_NDEBUG_NOEXCEPT
   {  check_cop(other);
      return index_ == other.index_;
   }
   bool operator!=(const CPPAD_VECTOR_ITR& other) const CPPAD_NDEBUG_NOEXCEPT
   {  check_cop(other);
      return index_ != other.index_;
   }
/* {xrst_code}
{xrst_spell_on}

{xrst_end cppad_vector_itr_equal}
-------------------------------------------------------------------------------
{xrst_begin cppad_vector_itr_element dev}

Vector Class Iterator Access Elements
#####################################

Syntax
******

| *element* = * *itr*
| * *itr* = *element*
| *element* = *itr* [ *n* ]
| *itr* [ *n* ] = *element*

Source
******
{xrst_spell_off}
{xrst_code hpp} */
public:
# if CPPAD_CONST
   const Type& operator*(void) const
   {  check_element();
      return (*data_)[index_];
   }
   const Type& operator[](difference_type n) const
   {  return *(*this + n);
   }
   const Type& operator[](size_t n) const
   {  return *( *this + difference_type(n) );
   }
# else
   Type& operator*(void) const
   {  check_element();
      return (*data_)[index_];
   }
   Type& operator[](difference_type n) const
   {  return *(*this + n);
   }
   Type& operator[](size_t n) const
   {  return *( *this + difference_type(n) );
   }
# endif
/* {xrst_code}
{xrst_spell_on}

{xrst_end cppad_vector_itr_element}
-------------------------------------------------------------------------------
{xrst_begin cppad_vector_itr_random dev}

Vector Class Iterator Random Access
###################################

Syntax
******

| *itr* + *-* = *n*
| *itr* = *other* + *-* *n*
| *itr* = *n* + *-* *other*
| *n* = *itr* ``-`` *other*
| *b* = *itr* *cop* *other*

Plus or Minus
*************
The notation + *-* above is either ``+`` or ``-`` .

cop
***
is one of the following:
``<`` , ``<=`` ,
``>`` , ``>=`` .

itr, other
**********
are iterators of the same type.

n
*
is a ``difference_type`` object.

b
*
is a ``bool`` .

Restrictions
************
It is an error to use a *cop* with iterators corresponding to different
``data_`` vectors

Source
******
{xrst_spell_off}
{xrst_code hpp} */
public:
   // sum and difference operators
   CPPAD_VECTOR_ITR& operator+=(difference_type n) noexcept
   {  index_ += n;
      return *this;
   }
   CPPAD_VECTOR_ITR& operator-=(difference_type n) noexcept
   {  index_ -= n;
      return *this;
   }
   CPPAD_VECTOR_ITR operator+(difference_type n) const noexcept
   {  return CPPAD_VECTOR_ITR(data_, length_, index_ + n);
   }
   CPPAD_VECTOR_ITR operator-(difference_type n) const noexcept
   {  return CPPAD_VECTOR_ITR(data_, length_, index_ - n);
   }
   difference_type  operator-(const CPPAD_VECTOR_ITR& other) const
   noexcept
   {  return index_ - other.index_;
   }
   // comparison operators
   bool operator<(const CPPAD_VECTOR_ITR& other) const CPPAD_NDEBUG_NOEXCEPT
   {  check_cop(other);
      return index_ < other.index_;
   }
   bool operator<=(const CPPAD_VECTOR_ITR& other) const CPPAD_NDEBUG_NOEXCEPT
   {  check_cop(other);
      return index_ <= other.index_;
   }
   bool operator>(const CPPAD_VECTOR_ITR& other) const CPPAD_NDEBUG_NOEXCEPT
   {  check_cop(other);
      return index_ > other.index_;
   }
   bool operator>=(const CPPAD_VECTOR_ITR& other) const CPPAD_NDEBUG_NOEXCEPT
   {  check_cop(other);
      return index_ >= other.index_;
   }
/* {xrst_code}
{xrst_spell_on}
{xrst_literal
   // BEGIN_BINARY_OP
   // END_BINARY_OP
}

{xrst_end cppad_vector_itr_random}
*/
// ==========================================================================
}; // END_TEMPLATE_CLASS_CPPAD_VECTOR_ITR
// ==========================================================================

// BEGIN_BINARY_OP
template <class Type> CPPAD_VECTOR_ITR<Type> operator+(
   typename CPPAD_VECTOR_ITR<Type>::difference_type n  ,
   const CPPAD_VECTOR_ITR<Type>&               other   ) noexcept
{  return other + n;
}
// END_BINARY_OP

} } } // END_CPPAD_LOCAL_UTILITY_NAMESPACE

# undef CPPAD_CONST
# undef CPPAD_VECTOR_ITR
# endif
