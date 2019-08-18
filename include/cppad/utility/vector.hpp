# ifndef CPPAD_UTILITY_VECTOR_HPP
# define CPPAD_UTILITY_VECTOR_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */


# include <cstddef>
# include <iostream>
# include <limits>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/utility/check_simple_vector.hpp>
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// ==========================================================================
template <class Type> class vector {
// ==========================================================================
/*
$begin cppad_vector_private$$

$section Vector Class: Private Data and Functions$$

$head Type$$
is the type of the elements in the array.

$head capacity_$$
Number of $icode Type$$ elements in $code data_$$ that have been allocated
(and constructor has been called).

$head length_$$
Number of $icode Type$$ elements currently in this vector.

$head data_$$
Pointer to the first element of the vector
(not defined and should not be used when $code capacity_$$ is  0).

$head delete_data$$
Call destructor and free all the allocated elements
(there are $code capacity_$$ such elements).

$srccode%hpp% */
private:
    size_t capacity_;
    size_t length_;
    Type*  data_;
    void delete_data(Type* data_ptr)
    {   thread_alloc::delete_array(data_ptr); }
/* %$$
$end
*/
// ==========================================================================
public:
// ==========================================================================
/*
-----------------------------------------------------------------------------
$begin cppad_vector_typedef$$

$section Vector Class: Public Type definitions$$

$head value_type$$
Type corresponding to an element of the vector.

$srccode%hpp% */
    typedef Type value_type;
/* %$$
$end
-----------------------------------------------------------------------------
$begin cppad_vector_ctor$$
$spell
    vec
$$

$section Vector Class: Public Constructors$$

$head Default$$
The syntax
$codei%
    vector<%Type%> %vec%
%$$
creates an empty vector no elements and no capacity.

$head Sizing$$
The syntax
$codei%
    vector<%Type%> %vec%(%n%)
%$$
where $icode n$$ is a $code size_t$$,
creates the vector $icode vec$$ with $icode n$$ elements and capacity
greater than or equal $icode n$$.

$head Copy$$
The syntax
$codei%
    vector<%Type%> %vec%(%other%)
%$$
where $icode other$$ is a $codei%vector<%Type%>%$$,
creates the vector $icode vec$$
with $icode%n% = other%.size()%$$ elements and capacity
greater than or equal $icode n$$.

$head Destructor$$
If $code capacity_$$ is non-zero, call the destructor
for all the corresponding elements and then frees the corresponding memory.

$srccode%hpp% */
    vector(void) : capacity_(0), length_(0), data_(CPPAD_NULL)
    { }
    vector(size_t n) : capacity_(0), length_(0), data_(CPPAD_NULL)
    {   resize(n); }
    vector(const vector& other) : capacity_(0), length_(0), data_(CPPAD_NULL)
    {   resize(other.length_);
        for(size_t i = 0; i < length_; i++)
            data_[i] = other.data_[i];
    }
    ~vector(void)
    {   if( capacity_ > 0 ) delete_data(data_); }
/* %$$
$end
-----------------------------------------------------------------------------
*/
    /// maximum number of elements current allocation can store
    size_t capacity(void) const
    {   return capacity_; }

    /// number of elements currently in this vector.
    size_t size(void) const
    {   return length_; }

    /// raw pointer to the data
    Type* data(void)
    {   return data_; }

    /// const raw pointer to the data
    const Type* data(void) const
    {   return data_; }

    /// change the number of elements in this vector.
    void resize(
        /// new number of elements for this vector
        size_t n
    )
    {   length_ = n;

        // check if we must allocate new memory
        if( capacity_ < length_ )
        {
            // check if there is old memory to be freed
            if( capacity_ > 0 )
                delete_data(data_);

            // get new memory and set capacity
            data_ = thread_alloc::create_array<Type>(length_, capacity_);
        }
    }

    /// free memory and set number of elements to zero
    void clear(void)
    {   length_ = 0;
        // check if there is old memory to be freed
        if( capacity_ > 0 )
            delete_data(data_);
        capacity_ = 0;
    }

    /// swap
    void swap(vector& x)
    {   // swap with self case
       if( this == &x )
            return;
        std::swap(length_,   x.length_   );
        std::swap(capacity_, x.capacity_ );
        std::swap(data_,     x.data_     );
        return;
    }


    /// vector assignment operator
    vector& operator=(
        /// right hand size of the assingment operation
        const vector& x
    )
    {   size_t i;
        // If original length is zero, then resize it.
        // Otherwise a length mismatch is an error.
        if( length_ == 0 )
            resize( x.length_ );
        CPPAD_ASSERT_KNOWN(
            length_ == x.length_ ,
            "vector: size miss match in assignment operation"
        );
        for(i = 0; i < length_; i++)
            data_[i] = x.data_[i];
        return *this;
    }
# if CPPAD_USE_CPLUSPLUS_2011
    /// vector assignment operator with move semantics
    vector& operator=(
        /// right hand size of the assingment operation
        vector&& x
    )
    {   CPPAD_ASSERT_KNOWN(
            length_ == x.length_ || (length_ == 0),
            "vector: size miss match in assignment operation"
        );
        swap(x);
        return *this;
    }
# endif
    // --------------------------------------------------------------------
    /// non-constant element access; i.e., we can change this element value
    Type& operator[](
        /// element index, must be less than length
        size_t i
    )
    {   CPPAD_ASSERT_KNOWN(
            i < length_,
            "vector: index greater than or equal vector size"
        );
        return data_[i];
    }
    /// non-constant element access; i.e., we can change this element value
    template <class Index>
    Type& operator[](
        /// element index, must be less than length
        Index i
    )
    {   return (*this)[size_t(i)]; }
    // --------------------------------------------------------------------
    /// constant element access; i.e., we cannot change this element value
    const Type& operator[](
        /// element index, must be less than length and convertable to size_t
        size_t i
    ) const
    {   CPPAD_ASSERT_KNOWN(
            i < length_,
            "vector: index greater than or equal vector size"
        );
        return data_[i];
    }
    /// constant element access; i.e., we cannot change this element value
    template <class Index>
    const Type& operator[](
        /// element index, must be less than length and convertable to size_t
        Index i
    ) const
    {   return (*this)[size_t(i)]; }
    // --------------------------------------------------------------------
    /// add an element to the back of this vector
    void push_back(
        /// value of the element
        const Type& s
    )
    {   // case where no allocation is necessary
        if( length_ + 1 <= capacity_ )
        {   data_[length_++] = s;
            return;
        }
        CPPAD_ASSERT_UNKNOWN( length_ == capacity_ );

        // store old length, capacity and data
        size_t old_length   = length_;
        size_t old_capacity = capacity_;
        Type*  old_data     = data_;

        // set the new length, capacity and data
        length_   = 0;
        capacity_ = 0;
        resize(old_length + 1);

        // copy old data values
        for(size_t i = 0; i < old_length; i++)
            data_[i] = old_data[i];

        // put the new element in the vector
        CPPAD_ASSERT_UNKNOWN( old_length + 1 <= capacity_ );
        data_[old_length] = s;

        // free old data
        if( old_capacity > 0 )
            delete_data(old_data);

        CPPAD_ASSERT_UNKNOWN( old_length + 1 == length_ );
        CPPAD_ASSERT_UNKNOWN( length_ <= capacity_ );
    }

    /*! add vector to the back of this vector
    (we could not use push_back because MS V++ 7.1 did not resolve
    to non-template member function when scalar is used.)
    */
    template <class Vector>
    void push_vector(
        /// value of the vector that we are adding
        const Vector& v
    )
    {   CheckSimpleVector<Type, Vector>();
        size_t m = v.size();

        // case where no allcoation is necessary
        if( length_ + m <= capacity_ )
        {   for(size_t i = 0; i < m; i++)
                data_[length_++] = v[i];
            return;
        }

        // store old length, capacity and data
        size_t old_length   = length_;
        size_t old_capacity = capacity_;
        Type*  old_data     = data_;

        // set new length, capacity and data
        length_   = 0;
        capacity_ = 0;
        resize(old_length + m);

        // copy old data values
        for(size_t i = 0; i < old_length; i++)
            data_[i] = old_data[i];

        // put the new elements in the vector
        CPPAD_ASSERT_UNKNOWN( old_length + m <= capacity_ );
        for(size_t i = 0; i < m; i++)
            data_[old_length + i] = v[i];

        // free old data
        if( old_capacity > 0 )
            delete_data(old_data);

        CPPAD_ASSERT_UNKNOWN( old_length + m  == length_ );
        CPPAD_ASSERT_UNKNOWN( length_ <= capacity_ );
    }

// =========================================================================
};  // END_TEMPLATE_CLASS_VECTOR
// =========================================================================

/// output a vector
template <class Type>
std::ostream& operator << (
    /// stream to write the vector to
    std::ostream&              os  ,
    /// vector that is output
    const CppAD::vector<Type>& vec )
{   size_t i = 0;
    size_t n = vec.size();

    os << "{ ";
    while(i < n)
    {   os << vec[i++];
        if( i < n )
            os << ", ";
    }
    os << " }";
    return os;
}

// ---------------------------------------------------------------------------
/*!
Class that is used to hold a non-constant element of a vector.
*/
class vectorBoolElement {
    /// the boolean data is packed with sizeof(UnitType) bits per value
    typedef size_t UnitType;
private:
    /// pointer to the UnitType value holding this eleemnt
    UnitType* unit_;
    /// mask for the bit corresponding to this element
    /// (all zero except for bit that corresponds to this element)
    UnitType mask_;
public:
    /// constructor from member values
    vectorBoolElement(
        /// unit for this element
        UnitType* unit ,
        /// mask for this element
        UnitType mask  )
    : unit_(unit) , mask_(mask)
    { }
    /// constuctor from another element
    vectorBoolElement(
        /// other element
        const vectorBoolElement& e )
    : unit_(e.unit_) , mask_(e.mask_)
    { }
    /// conversion to a boolean value
    operator bool() const
    {   return (*unit_ & mask_) != 0; }
    /// assignment of this element to a bool
    vectorBoolElement& operator=(
        /// right hand side for assignment
        bool bit
    )
    {   if(bit)
            *unit_ |= mask_;
        else
            *unit_ &= ~mask_;
        return *this;
    }
    /// assignment of this element to another element
    vectorBoolElement& operator=(const vectorBoolElement& e)
    {   if( *(e.unit_) & e.mask_ )
            *unit_ |= mask_;
        else
            *unit_ &= ~mask_;
        return *this;
    }
};

class vectorBool {
    /// the boolean data is packed with sizeof(UnitType) bits per value
    typedef size_t UnitType;
private:
    /// number of bits packed into each UnitType value in data_
    static const size_t bit_per_unit_
        = std::numeric_limits<UnitType>::digits;
    /// number of UnitType values in data_
    size_t    n_unit_;
    /// number of bits currently stored in this vector
    size_t    length_;
    /// pointer to where the bits are stored
    UnitType *data_;

    /// minimum number of UnitType values that can store length_ bits
    /// (note that this is really a function of length_)
    size_t unit_min(void)
    {   if( length_ == 0 )
            return 0;
        return (length_ - 1) / bit_per_unit_ + 1;
    }
public:
    /// type corresponding to the elements of this vector
    /// (note that non-const elements actually use vectorBoolElement)
    typedef bool value_type;

    // static member function
    static size_t bit_per_unit(void)
    {   return bit_per_unit_; }

    /// default constructor (sets all member data to zero)
    vectorBool(void) : n_unit_(0), length_(0), data_(CPPAD_NULL)
    { }
    /// sizing constructor
    vectorBool(
        /// number of bits in this vector
        size_t n
    ) : n_unit_(0), length_(n), data_(CPPAD_NULL)
    {   if( length_ > 0 )
        {   // set n_unit and data
            size_t min_unit = unit_min();
            data_ = thread_alloc::create_array<UnitType>(min_unit, n_unit_);
        }
    }
    /// copy constructor
    vectorBool(
        /// the *this vector will be a copy of v
        const vectorBool& v
    ) : n_unit_(0), length_(v.length_), data_(CPPAD_NULL)
    {   if( length_ > 0 )
        {   // set n_unit and data
            size_t min_unit = unit_min();
            data_ = thread_alloc::create_array<UnitType>(min_unit, n_unit_);

            // copy values using UnitType assignment operator
            CPPAD_ASSERT_UNKNOWN( min_unit <= v.n_unit_ );
            size_t i;
            for(i = 0; i < min_unit; i++)
                data_[i] = v.data_[i];
        }
    }
    /// destructor
    ~vectorBool(void)
    {   if( n_unit_ > 0 )
            thread_alloc::delete_array(data_);
    }

    /// number of elements in this vector
    size_t size(void) const
    {   return length_; }

    /// maximum number of elements current allocation can store
    size_t capacity(void) const
    {   return n_unit_ * bit_per_unit_; }

    /// change number of elements in this vector
    void resize(
        /// new number of elements for this vector
        size_t n
    )
    {   length_ = n;
        // check if we can use the current memory
        size_t min_unit = unit_min();
        if( n_unit_ >= min_unit )
            return;
        // check if there is old memory to be freed
        if( n_unit_ > 0 )
            thread_alloc::delete_array(data_);
        // get new memory and set n_unit
        data_ = thread_alloc::create_array<UnitType>(min_unit, n_unit_);
    }

    /// free memory and set number of elements to zero
    void clear(void)
    {   length_ = 0;
        // check if there is old memory to be freed
        if( n_unit_ > 0 )
            thread_alloc::delete_array(data_);
        n_unit_ = 0;
    }

    /// swap
    void swap(vectorBool& x)
    {   // swap with self case
       if( this == &x )
            return;
        std::swap(n_unit_,   x.n_unit_   );
        std::swap(length_,   x.length_   );
        std::swap(data_,     x.data_     );
        return;
    }

    /// vector assignment operator
    vectorBool& operator=(
        /// right hand size of the assingment operation
        const vectorBool& v
    )
    {   size_t i;
        // If original length is zero, then resize it.
        // Otherwise a length mismatch is an error.
        if( length_ == 0 )
            resize( v.length_ );
        CPPAD_ASSERT_KNOWN(
            length_ == v.length_ ,
            "vectorBool: size miss match in assignment operation"
        );
        size_t min_unit = unit_min();
        CPPAD_ASSERT_UNKNOWN( min_unit <= n_unit_ );
        CPPAD_ASSERT_UNKNOWN( min_unit <= v.n_unit_ );
        for(i = 0; i < min_unit; i++)
            data_[i] = v.data_[i];
        return *this;
    }
# if CPPAD_USE_CPLUSPLUS_2011
    /// vector assignment operator with move semantics
    vectorBool& operator=(
        /// right hand size of the assingment operation
        vectorBool&& x
    )
    {   CPPAD_ASSERT_KNOWN(
            length_ == x.length_ || (length_ == 0),
            "vectorBool: size miss match in assignment operation"
        );
        swap(x);
        return *this;
    }
# endif


    /// non-constant element access; i.e., we can change this element value
    vectorBoolElement operator[](
        /// element index, must be less than length
        size_t k
    )
    {   size_t i, j;
        CPPAD_ASSERT_KNOWN(
            k < length_,
            "vectorBool: index greater than or equal vector size"
        );
        i    = k / bit_per_unit_;
        j    = k - i * bit_per_unit_;
        return vectorBoolElement(data_ + i , UnitType(1) << j );
    }
    /// constant element access; i.e., we cannot change this element value
    bool operator[](size_t k) const
    {   size_t i, j;
        UnitType unit, mask;
        CPPAD_ASSERT_KNOWN(
            k < length_,
            "vectorBool: index greater than or equal vector size"
        );
        i    = k / bit_per_unit_;
        j    = k - i * bit_per_unit_;
        unit = data_[i];
        mask = UnitType(1) << j;
        return (unit & mask) != 0;
    }
    /// add an element to the back of this vector
    void push_back(
        /// value of the element
        bool bit
    )
    {   CPPAD_ASSERT_UNKNOWN( unit_min() <= n_unit_ );
        size_t i, j;
        UnitType mask;
        if( length_ + 1 > n_unit_ * bit_per_unit_ )
        {   CPPAD_ASSERT_UNKNOWN( unit_min() == n_unit_ );
            // store old n_unit and data values
            size_t    old_n_unit = n_unit_;
            UnitType* old_data   = data_;
            // set new n_unit and data values
            data_ = thread_alloc::create_array<UnitType>(n_unit_+1, n_unit_);
            // copy old data values
            for(i = 0; i < old_n_unit; i++)
                data_[i] = old_data[i];
            // free old data
            if( old_n_unit > 0 )
                thread_alloc::delete_array(old_data);
        }
        i    = length_ / bit_per_unit_;
        j    = length_ - i * bit_per_unit_;
        mask = UnitType(1) << j;
        if( bit )
            data_[i] |= mask;
        else
            data_[i] &= ~mask;
        length_++;
    }
    /// add vector to the back of this vector
    template <class Vector>
    void push_vector(
        /// value of the vector that we are adding
        const Vector& v
    )
    {    CheckSimpleVector<bool, Vector>();
        size_t min_unit = unit_min();
        CPPAD_ASSERT_UNKNOWN( length_ <= n_unit_ * bit_per_unit_ );
        // some temporaries
        size_t i, j, k, ell;
        UnitType mask;
        bool bit;
        // store old length
        size_t old_length = length_;
        // new length and minium number of units;
        length_    = length_ + v.size();
        min_unit   = unit_min();
        if( length_ >= n_unit_ * bit_per_unit_ )
        {   // store old n_unit and data value
            size_t  old_n_unit = n_unit_;
            UnitType* old_data = data_;
            // set new n_unit and data values
            data_ = thread_alloc::create_array<UnitType>(min_unit, n_unit_);
            // copy old data values
            for(i = 0; i < old_n_unit; i++)
                data_[i] = old_data[i];
            // free old data
            if( old_n_unit > 0 )
                thread_alloc::delete_array(old_data);
        }
        ell = old_length;
        for(k = 0; k < v.size(); k++)
        {
            i    = ell / bit_per_unit_;
            j    = ell - i * bit_per_unit_;
            bit  = v[k];
            mask = UnitType(1) << j;
            if( bit )
                data_[i] |= mask;
            else
                data_[i] &= ~mask;
            ell++;
        }
        CPPAD_ASSERT_UNKNOWN( length_ == ell );
        CPPAD_ASSERT_UNKNOWN( length_ <= n_unit_ * bit_per_unit_ );
    }
};

/// output a vector
inline std::ostream& operator << (
    /// steam to write the vector to
    std::ostream&      os  ,
    /// vector that is output
    const vectorBool&  v   )
{   size_t i = 0;
    size_t n = v.size();

    while(i < n)
        os << v[i++];
    return os;
}

} // END_CPPAD_NAMESPACE
# endif
