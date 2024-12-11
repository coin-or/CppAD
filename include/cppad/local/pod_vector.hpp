# ifndef CPPAD_LOCAL_POD_VECTOR_HPP
# define CPPAD_LOCAL_POD_VECTOR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# if CPPAD_CSTDINT_HAS_8_TO_64
# include <cstdint>
# endif
# include <cstring>
# include <algorithm>
# include <cppad/utility/thread_alloc.hpp>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/is_pod.hpp>

/*
{xrst_begin_parent pod_vector dev}

A Template Vector Class That does not construct or destruct Elements
####################################################################

Prototype
*********
{xrst_literal
   // BEGIN_POD_VECTOR_CLASS
   // END_POD_VECTOR_CLASS
}

Member Functions
****************
All the member functions in this class are public.

is_pod
******
The :ref:`is_pod-name` function must return true for this *Type* .

{xrst_end pod_vector}
*/
// BEGIN_POD_VECTOR_CLASS
namespace CppAD { namespace local {
template <class Type> class pod_vector
// END_POD_VECTOR_CLASS
{
/*
{xrst_begin pod_vector_private dev}

pod_vector Private Data
#######################

Prototype
*********
{xrst_literal
   // BEGIN_PRIVATE
   // END_PRIVATE
}

size\_
******
is the number of elements currently in this vector.

capacity\_
**********
is the maximum number of elements that the current allocation can hold.
It is always greater than or equal size\_ .
The only operations that can decrease the capacity are
:ref:`pod_vector_vector@swap` and :ref:`pod_vector_resize@clear` .

data\_
******
is a pointer to the first element of the vector.
This is the null pointer when capacity\_ is zero.

{xrst_end pod_vector_private}
*/
// BEGIN_PRIVATE
private:
   size_t size_;
   size_t capacity_;
   Type   *data_;
// END_PRIVATE
// ---------------------------------------------------------------------------
public:
// ---------------------------------------------------------------------------
/*
{xrst_begin pod_vector_ctor dev}

pod_vector Constructors and Destructor
######################################

Copy
****
{xrst_literal
   // BEGIN_COPY_CTOR
   // END_COPY_CTOR
}
This constructor cannot be used.

Default
*******
{xrst_literal
   // BEGIN_DEFAULT_CTOR
   // END_DEFAULT_CTOR
}
This constructor sets the size and capacity to zero.

Size
****
{xrst_literal
   // BEGIN_SIZE_CTOR
   // END_SIZE_CTOR
}
This constructor sets the size (capacity) equal *n*
( greater that or equal *n* ).

Destructor
**********
{xrst_literal
   // BEGIN_DESTRUCTOR
   // END_DESTRUCTOR
}
The memory is returned using :ref:`ta_return_memory-name` .


{xrst_end pod_vector_ctor}
*/
   // BEGIN_COPY_CTOR
   pod_vector(const pod_vector&) = delete;
   // END_COPY_CTOR

   // BEGIN_DEFAULT_CTOR
   pod_vector(void)
   // END_DEFAULT_CTOR
   : size_(0), capacity_(0), data_(nullptr)
   {  CPPAD_ASSERT_UNKNOWN( is_pod<Type>() );
   }

   // BEGIN_SIZE_CTOR
   pod_vector(size_t n)
   // END_SIZE_CTOR
   : size_(0), capacity_(0), data_(nullptr)
   {  CPPAD_ASSERT_UNKNOWN( is_pod<Type>() );
      extend(n);
   }

   // BEGIN_DESTRUCTOR
   ~pod_vector(void)
   // END_DESTRUCTOR
   {  if( capacity_ > 0 )
      {
         void* v_ptr = reinterpret_cast<void*>( data_ );
         thread_alloc::return_memory(v_ptr);
      }
   }
/*
------------------------------------------------------------------------------
{xrst_begin pod_vector_state dev}

The Current State of a pod_vector
#################################

size
****
{xrst_literal
   // BEGIN_SIZE_STATE
   // END_SIZE_STATE
}

capacity
********
{xrst_literal
   // BEGIN_CAPACITY_STATE
   // END_CAPACITY_STATE
}

data
****
{xrst_literal
   // BEGIN_DATA_STATE
   // END_DATA_STATE
}
This pointer is no longer valid after the following operations:
extend, resize, clear, assignment, destructor.

{xrst_end pod_vector_state}
*/

   // BEGIN_SIZE_STATE
   size_t size(void) const
   {  return size_; }
   // END_SIZE_STATE

   // BEGIN_CAPACITY_STATE
   size_t capacity(void) const
   {  return capacity_; }
   // END_CAPACITY_STATE

   // BEGIN_DATA_STATE
   Type* data(void)
   {  return data_; }
   const Type* data(void) const
   {  return data_; }
   // END_DATA_STATE

/*
------------------------------------------------------------------------------
{xrst_begin pod_vector_element dev}

Access and Change an Element of a pod_vector
############################################

Not Constant
************
{xrst_literal
   // BEGIN_ELEMENT
   // END_ELEMENT
}
This accesses the element at the specified index
(in a way that allows the element's value to be changed).
An assert is generated the index is greater than or equal to
`size_`` for this vector.

Constant
********
{xrst_literal
   // BEGIN_CONST_ELEMENT
   // END_CONST_ELEMENT
}
This accesses the element at the specified index
(in a way that does not allow the element's value to be changed).
An assert is generated the index is greater than or equal to
`size_`` for this vector.

push_back
*********
{xrst_literal
   // BEGIN_PUSH_BACK
   // END_PUSH_BACK
}
This increases the size by one and places the specified element
at the end (highest valid index) of the vector.

{xrst_end pod_vector_element}
*/
   // BEGIN_ELEMENT
   template <class Index>
   Type& operator[](Index index)
   // END_ELEMENT
   {  CPPAD_ASSERT_UNKNOWN( size_t(index)  < size_ );
      return data_[index];
   }
   // BEGIN_CONST_ELEMENT
   template <class Index>
   const Type& operator[](Index index) const
   // END_CONST_ELEMENT
   {  CPPAD_ASSERT_UNKNOWN( size_t(index)  < size_ );
      return data_[index];
   }
   // BEGIN_PUSH_BACK
   void push_back(const Type& element)
   // END_PUSH_BACK
   {  size_t i = extend(1);
      data_[i] = element;
   }
/*
-------------------------------------------------------------------------------
{xrst_begin pod_vector_vector dev}

pod_vector Operations With Vector Arguments
###########################################

swap
****
{xrst_literal
   // BEGIN_SWAP
   // END_SWAP
}
This exchanges this all the information in this vector with another vector.
It is faster that assignment because it does not allocate or free memory and
does not do any element by element operations.

Assignment
**********
{xrst_literal
   // BEGIN_ASSIGNMENT
   // END_ASSIGNMENT
}
This copies all the information from the other vector to this vector.
It copies each element from other and it may free and allocate memory.

{xrst_end pod_vector_vector}
*/
   // BEGIN_SWAP
   void swap(pod_vector& other)
   // END_SWAP
   {  std::swap(capacity_,      other.capacity_);
      std::swap(size_,          other.size_);
      std::swap(data_,          other.data_);
   }
   // BEGIN_ASSIGNMENT
   void operator=(const pod_vector& other)
   // END_ASSIGNMENT
   {  resize( other.size_ );
      if( size_ > 0 )
      {  size_t bytes      = size_ * sizeof(Type);
         void* v_ptr       = reinterpret_cast<void*>( data_ );
         void* v_ptr_other = reinterpret_cast<void*>( other.data_ );
         std::memcpy(v_ptr, v_ptr_other, bytes);
      }
   }
/*
-------------------------------------------------------------------------------
{xrst_begin pod_vector_resize dev}

Changing the Size of a pod_vector
#################################

extent
******
{xrst_literal
   // BEGIN_EXTEND
   // END_EXTEND
}
#. This increases the size of the vector by *n* .
#. It returns the size before the increase
   which is the index of the first new element added to the vector.
#. All of the elements in the vector, before the extension,
   are preserved by this operation.

resize
******
{xrst_literal
   // BEGIN_RESIZE
   // END_RESIZE
}
#. This changes the size of the vector to *n* .
#. If on input, *n* is less that or equal :ref:`pod_vector_private@capacity\_` ,
   all the elements in the vector, before the extension,
   are preserved by this operation.
#. If on input, *n* is greater than capacity\_ ,
   the elements in the vector are lost.

clear
*****
{xrst_literal
   // BEGIN_CLEAR
   // END_CLEAR
}
This sets the size and capacity for the vector to zero
and frees all the memory that it was using.


{xrst_end pod_vector_resize}
*/
   // BEGIN_EXTEND
   size_t extend(size_t n)
   // END_EXTEND
   {  size_t old_size   = size_;
      size_              += n;

      // check if we can use current memory
      if( size_ <= capacity_ )
         return old_size;

      // save more old information
      size_t old_capacity = capacity_;
      void* old_v_ptr     = reinterpret_cast<void*>(data_);

      // get new memory and set capacity
      size_t byte_capacity;
      size_t bytes = size_ * sizeof(Type);
      void* v_ptr  = thread_alloc::get_memory(bytes, byte_capacity);
      capacity_    = byte_capacity / sizeof(Type);
      data_        = reinterpret_cast<Type*>(v_ptr);

      // copy old data to new
      if( old_size >  0 )
         std::memcpy(v_ptr, old_v_ptr, old_size * sizeof(Type));

      // return old memory to available pool
      if( old_capacity > 0 )
         thread_alloc::return_memory(old_v_ptr);

      // return value for extend(n) is the old length
      CPPAD_ASSERT_UNKNOWN( size_ <= capacity_ );
      return old_size;
   }
   // BEGIN_RESIZE
   void resize(size_t n)
   // END_RESIZE
   {  size_      = n;

      // check if we must allocate new memory
      if( capacity_ < size_ )
      {  void* v_ptr;
         //
         if( capacity_ > 0 )
         {  // return old memory to available pool
            v_ptr = reinterpret_cast<void*>( data_ );
            thread_alloc::return_memory(v_ptr);
         }
         //
         // get new memory and set capacity
         size_t byte_capacity;
         size_t bytes = size_ * sizeof(Type);
         v_ptr        = thread_alloc::get_memory(bytes, byte_capacity);
         capacity_    = byte_capacity / sizeof(Type);
         data_        = reinterpret_cast<Type*>(v_ptr);
         //
      }
      CPPAD_ASSERT_UNKNOWN( size_ <= capacity_ );
   }
   // BEGIN_CLEAR
   void clear(void)
   // END_CLEAR
   {  if( capacity_ > 0 )
      {
         void* v_ptr = reinterpret_cast<void*>( data_ );
         thread_alloc::return_memory(v_ptr);
      }
      data_        = nullptr;
      capacity_    = 0;
      size_        = 0;
   }
};
// ---------------------------------------------------------------------------
/*!
A vector class with that does not use element constructors or destructors
when is_pod<Type> is true.
*/
template <class Type>
class pod_vector_maybe {
private:
   /// maximum number of Type elements current allocation can hold
   size_t capacity_;

   /// number of elements currently in this vector
   size_t length_;

   /// pointer to the first type elements
   /// (not defined and should not be used when capacity_ = 0)
   Type   *data_;

   /// do not use the copy constructor
   explicit pod_vector_maybe(const pod_vector_maybe& )
   {  CPPAD_ASSERT_UNKNOWN(false); }
public:
   /// default constructor sets capacity_ = length_ = data_ = 0
   pod_vector_maybe(void)
   : capacity_(0), length_(0), data_(nullptr)
   {  CPPAD_ASSERT_UNKNOWN( is_pod<size_t>() );
   }

   /// sizing constructor
   pod_vector_maybe(
      /// number of elements in this vector
      size_t n )
   : capacity_(0), length_(0), data_(nullptr)
   {  extend(n); }


   /// Destructor: returns allocated memory to thread_alloc;
   /// see extend and resize.  If this is not plain old data,
   /// the destructor for each element is called.
   ~pod_vector_maybe(void)
   {  if( capacity_ > 0 )
      {  if( ! is_pod<Type>() )
         {  // call destructor for each element
            for(size_t i = 0; i < capacity_; i++)
               (data_ + i)->~Type();
         }
         void* v_ptr = reinterpret_cast<void*>( data_ );
         thread_alloc::return_memory(v_ptr);
      }
   }

   /// current number of elements in this vector.
   size_t size(void) const
   {  return length_; }

   /// current capacity (amount of allocated storage) for this vector.
   size_t capacity(void) const
   {  return capacity_; }

   /// current data pointer is no longer valid after any of the following:
   /// extend, resize, erase, clear, assignment, and destructor.
   Type* data(void)
   {  return data_; }

   /// const version of data pointer (see non-const documentation)
   const Type* data(void) const
   {  return data_; }
   // ----------------------------------------------------------------------
   /// non-constant element access; i.e., we can change this element value
   Type& operator[](
      /// element index, must be less than length
      size_t i
   )
   {  CPPAD_ASSERT_UNKNOWN( i < length_ );
      return data_[i];
   }
   /// non-constant element access; i.e., we can change this element value
   template <class Index>
   Type& operator[](
      /// element index, must be less than length and convertable to size_t
      Index i
   )
   {  return (*this)[size_t(i)]; }

   // ----------------------------------------------------------------------
   /// constant element access; i.e., we cannot change this element value
   const Type& operator[](
      /// element index, must be less than length
      size_t i
   ) const
   {  CPPAD_ASSERT_UNKNOWN( i < length_ );
      return data_[i];
   }
   /// constant element access; i.e., we cannot change this element value
   template <class Index>
   const Type& operator[](
      /// element index, must be less than length and convertable to size_t
      Index i
   ) const
   {  return (*this)[size_t(i)]; }

   // ----------------------------------------------------------------------
   /*!
   Add an element to theh back of this vector

   \param e
   is the element we are adding to the back of the vector.
   */
   void push_back(const Type& e)
   {  size_t i = extend(1);
      data_[i] = e;
   }

   /*!
   Swap all properties of this vector with another.
   This is useful when moving a vector that grows after it has reached
   its final size (without copying every element).

   \param other
   is the other vector that we are swapping this vector with.
   */
   void swap(pod_vector_maybe& other)
   {  std::swap(capacity_, other.capacity_);
      std::swap(length_,   other.length_);
      std::swap(data_,     other.data_);
   }
   // ----------------------------------------------------------------------
   /*!
   Increase the number of elements the end of this vector
   (existing elements are always preserved).

   \param n
   is the number of elements to add to end of this vector.

   \return
   is the number of elements in the vector before it was extended.
   This is the index of the first new element added to the vector.

   - If Type is plain old data, new elements are not initialized;
   i.e., their constructor is not called. Otherwise, the constructor
   is called for each new element.

   - This and resize are the only routine that allocate memory for
   pod_vector_maybe. They uses thread_alloc for this allocation.
   */
   size_t extend(size_t n)
   {  size_t old_length   = length_;
      length_            += n;

      // check if we can use current memory
      if( length_ <= capacity_ )
         return old_length;

      // save more old information
      size_t old_capacity = capacity_;
      Type* old_data      = data_;

      // get new memory and set capacity
      size_t length_bytes = length_ * sizeof(Type);
      size_t capacity_bytes;
      void* v_ptr = thread_alloc::get_memory(length_bytes, capacity_bytes);
      capacity_   = capacity_bytes / sizeof(Type);
      data_       = reinterpret_cast<Type*>(v_ptr);

      if( ! is_pod<Type>() )
      {  // call constructor for each new element
         for(size_t i = 0; i < capacity_; i++)
            new(data_ + i) Type();
      }

      // copy old data to new
      for(size_t i = 0; i < old_length; i++)
         data_[i] = old_data[i];

      // return old memory to available pool
      if( old_capacity > 0 )
      {  if( ! is_pod<Type>() )
         {  for(size_t i = 0; i < old_capacity; i++)
               (old_data + i)->~Type();
         }
         v_ptr = reinterpret_cast<void*>( old_data );
         thread_alloc::return_memory(v_ptr);
      }

      // return value for extend(n) is the old length
      CPPAD_ASSERT_UNKNOWN( length_ <= capacity_ );
      return old_length;
   }
   // ----------------------------------------------------------------------
   /*!
   resize the vector (existing elements preserved when n <= capacity_).

   \param n
   is the new size for this vector.

   \par
   if n <= capacity(), no memory is freed or allocated, the capacity
   is not changed, and existing elements are preserved.
   If n > capacity(), new memory is allocates and all the
   data in the vector is lost.

   - If  Type is plain old data, new elements are not initialized;
   i.e., their constructor is not called. Otherwise, the constructor
   is called for each new element.

   - This and extend are the only routine that allocate memory for
   pod_vector_maybe. They uses thread_alloc for this allocation.
   */
   void resize(size_t n)
   {  length_ = n;

      // check if we must allocate new memory
      if( capacity_ < length_ )
      {  void* v_ptr;
         //
         // return old memory to available pool
         if( capacity_ > 0 )
         {  if( ! is_pod<Type>() )
            {  // call destructor for each old element
               for(size_t i = 0; i < capacity_; i++)
                  (data_ + i)->~Type();
            }
            v_ptr = reinterpret_cast<void*>( data_ );
            thread_alloc::return_memory(v_ptr);
         }
         //
         // get new memory and set capacity
         size_t length_bytes = length_ * sizeof(Type);
         size_t capacity_bytes;
         v_ptr     = thread_alloc::get_memory(length_bytes, capacity_bytes);
         capacity_ = capacity_bytes / sizeof(Type);
         data_     = reinterpret_cast<Type*>(v_ptr);
         //
         CPPAD_ASSERT_UNKNOWN( length_ <= capacity_ );
         //
         if( ! is_pod<Type>() )
         {  // call constructor for each new element
            for(size_t i = 0; i < capacity_; i++)
               new(data_ + i) Type();
         }
      }
   }
   // ----------------------------------------------------------------------
   /*!
   Remove all the elements from this vector and free its memory.
   */
   void clear(void)
   {  if( capacity_ > 0 )
      {  if( ! is_pod<Type>() )
         {  // call destructor for each element
            for(size_t i = 0; i < capacity_; i++)
               (data_ + i)->~Type();
         }
         void* v_ptr = reinterpret_cast<void*>( data_ );
         thread_alloc::return_memory(v_ptr);
      }
      data_     = nullptr;
      capacity_ = 0;
      length_   = 0;
   }
   // -----------------------------------------------------------------------
   /// vector assignment operator
   void operator=(
      /// right hand size of the assingment operation
      const pod_vector_maybe& x
   )
   {  resize( x.length_ );
      //
      CPPAD_ASSERT_UNKNOWN( length_   == x.length_ );
      for(size_t i = 0; i < length_; i++)
      {  data_[i] = x.data_[i]; }
   }
};

} } // END_CPPAD_LOCAL_NAMESPACE
# endif
