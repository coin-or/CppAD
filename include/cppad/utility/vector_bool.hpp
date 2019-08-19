# ifndef CPPAD_UTILITY_VECTOR_BOOL_HPP
# define CPPAD_UTILITY_VECTOR_BOOL_HPP
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
