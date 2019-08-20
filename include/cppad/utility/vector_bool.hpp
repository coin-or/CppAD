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


/*
$begin vector_bool_element$$
$spell
    Bool
$$

$section Vector of Bool Element Class$$

$head Syntax$$
$icode%element% vectorBoolElement(%unit%, %mask%)
%$$
$icode%element% vectorBoolElement(%other%)
%$$
$icode%value% = bool(%element%)
%$$
$icode%element% = %value%
%$$
$icode%element% = %element%
%$$

$head UnitType$$
Type used to pack multiple boolean (bit) values into one unit.
Logical operations are preformed one unit at a time.

$head unit_$$
pointer to the unit that holds the value for this element.

$head mask_$$
mask for the bit corresponding to this element; i.e., all the bits
are zero except for bit that corresponds to this element which is one.

$head Source$$
$srccode%hpp% */
class vectorBoolElement {
private:
    typedef size_t UnitType;
    UnitType* unit_;
    UnitType  mask_;
public:
    vectorBoolElement(UnitType* unit, UnitType mask )
    : unit_(unit) , mask_(mask)
    { }
    vectorBoolElement(const vectorBoolElement& other)
    : unit_(other.unit_) , mask_(other.mask_)
    { }
    operator bool() const
    {   return (*unit_ & mask_) != 0; }
    vectorBoolElement& operator=(bool value)
    {   if(value) *unit_ |= mask_;
        else      *unit_ &= ~mask_;
        return *this;
    }
    vectorBoolElement& operator=(const vectorBoolElement& element)
    {   if( *(element.unit_) & element.mask_ ) *unit_ |= mask_;
        else                                   *unit_ &= ~mask_;
        return *this;
    }
};
/* %$$
$end
*/

// ============================================================================
class vectorBool {
// ============================================================================
/*
$begin vector_bool_member$$
$spell
    Bool
    vec
$$

$section vectorBool: Member Data$$

$head Syntax$$
$icode%vec%.unit_min()
%$$
$icode%vec%.bit_per_unit()
%$$


$head UnitType$$
Type used to pack multiple boolean (bit) values into one unit.
Logical operations are preformed one unit at a time.

$head bit_per_unit_$$
number of bits packed into each unit value in $code data_$$.

$head n_unit_$$
Number of unit values in $code data_$$.

$head length_$$
number of bits currently stored in this vector.

$head data_$$
pointer to where the bits are stored.

$head unit_min$$
minimum number of $code UnitType$$ values that can store $code length_$$ bits.
Note that this is really a function of $code length_$$.

$head Source$$
$srccode%hpp% */
private:
    typedef size_t UnitType;
    static const size_t bit_per_unit_ = std::numeric_limits<UnitType>::digits;
    size_t    n_unit_;
    size_t    length_;
    UnitType  *data_;
    //
    size_t unit_min(void) const
    {   if( length_ == 0 )
            return 0;
        return (length_ - 1) / bit_per_unit_ + 1;
    }
public:
    static size_t bit_per_unit(void)
    {   return bit_per_unit_; }
/* %$$
$end
-------------------------------------------------------------------------------
$begin vector_bool_typedef$$
$spell
    vec
    Bool
    const
$$

$section vectorBool Type Definitions$$

$head value_type$$
type corresponding to the elements of this vector
(note that non-const elements actually use $code vectorBoolElement$$).

$head Source$$
$srccode%hpp% */
public:
    typedef bool value_type;
/* %$$
$end
----------------------------------------------------------------------------
$begin vector_bool_ctor$$
$spell
    Bool
    vec
$$
$section vectorBool: Constructors and Destructor$$

$head Default$$
$codei%vectorBool %vec%
%$$
creates an empty vector with no elements and $code n_unit_$$ zero.

$head Sizing$$
$codei%vectorBool %vec%(%n%)
%$$
where $icode n$$ is a $code size_t$$,
creates the vector $icode vec$$ with $icode n$$ elements and $code n_unit_$$
greater than or equal $code unit_min()$$.

$head Copy$$
$codei%vector<%Type%> %vec%(%other%)
%$$
where $icode other$$ is a $codei%vector<%Type%>%$$,
creates the vector $icode vec$$
with $icode%n% = %other%.size()%$$ elements and $code n_unit_$$
greater than or equal $code unit_min()$$.

$head Source$$
$srccode%hpp%:
*/
    vectorBool(void) : n_unit_(0), length_(0), data_(CPPAD_NULL)
    { }
    vectorBool(size_t n) : n_unit_(0), length_(n), data_(CPPAD_NULL)
    {   resize(n); }
    vectorBool(const vectorBool& other)
    : n_unit_(0), length_(0), data_(CPPAD_NULL)
    {   resize(other.length_);
        CPPAD_ASSERT_UNKNOWN( n_unit_ <= other.n_unit_ );
        for(size_t i = 0; i < n_unit_; ++i)
            data_[i] = other.data_[i];
    }
/* %$$
$end
------------------------------------------------------------------------------
*/
    /// destructor
    ~vectorBool(void)
    {   clear(); }

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
        {   void* v_ptr = reinterpret_cast<void*>(data_);
            thread_alloc::return_memory(v_ptr);
        }
        // get new memory and set n_unit
        size_t min_bytes = n_unit_ * sizeof(UnitType);
        size_t cap_bytes;
        void* v_ptr = thread_alloc::get_memory(min_bytes, cap_bytes);
        data_       = reinterpret_cast<UnitType*>(v_ptr);
        n_unit_     = cap_bytes / sizeof(UnitType);
        CPPAD_ASSERT_UNKNOWN( n_unit_ >= min_unit );
    }

    /// free memory and set number of elements to zero
    void clear(void)
    {   length_ = 0;
        // check if there is old memory to be freed
        if( n_unit_ > 0 )
        {   void* v_ptr = reinterpret_cast<void*>(data_);
            thread_alloc::return_memory(v_ptr);
        }
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
        if( length_ + 1 > n_unit_ * bit_per_unit_ )
        {   CPPAD_ASSERT_UNKNOWN( unit_min() == n_unit_ );

            // create new vector with requuired size
            vectorBool vec(length_ + 1);

            // copy old data values
            for(size_t i = 0; i < n_unit_; ++i)
                vec.data_[i] = data_[i];

            // swap old and new vectors
            swap(vec);
        }
        else
            ++length_;
        CPPAD_ASSERT_UNKNOWN( length_ <= n_unit_ * bit_per_unit_ )
        size_t   unit_index = (length_ - 1) / bit_per_unit_;
        size_t   bit_index  = (length_ - 1) - unit_index * bit_per_unit_;
        UnitType mask       = UnitType(1) << bit_index;
        if( bit )
            data_[unit_index] |= mask;
        else
            data_[unit_index] &= ~mask;
    }
    /// add vector to the back of this vector
    template <class Vector>
    void push_vector(
        /// value of the vector that we are adding
        const Vector& v
    )
    {   CPPAD_ASSERT_UNKNOWN( unit_min() <= n_unit_ );
        CheckSimpleVector<bool, Vector>();
        size_t old_length = length_;
        size_t m           = v.size();
        if( length_ + m > n_unit_ * bit_per_unit_ )
        {
            // create new vector with requuired size
            vectorBool vec(length_ + m);

            // copy old data values
            for(size_t i = 0; i < n_unit_; ++i)
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
        {   size_t unit_index = (old_length + k) / bit_per_unit_;
            size_t bit_index  = (old_length + k) - unit_index * bit_per_unit_;
            UnitType mask     = UnitType(1) << bit_index;
            if( v[k] )
                data_[unit_index] |= mask;
            else
                data_[unit_index] &= ~mask;
        }
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
