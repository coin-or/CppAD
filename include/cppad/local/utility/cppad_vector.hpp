# ifndef CPPAD_LOCAL_UTILITY_CPPAD_VECTOR_HPP
# define CPPAD_LOCAL_UTILITY_CPPAD_VECTOR_HPP
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
# include <cppad/core/cppad_assert.hpp>

// BEGIN_CPPAD_LOCAL_UTILITY_NAMESPACE
namespace CppAD { namespace local { namespace utility {


// ==========================================================================
template <class Type> class cppad_vector_itr {
// ==========================================================================
/*
-----------------------------------------------------------------------------
$begin cppad_vector_itr_traits$$
$spell
    Iterator
$$

$section Vector Class Iterator Traits$$

$srccode%hpp% */
public:
    typedef std::forward_iterator_tag          iterator_category;
    typedef Type                               value_type;
    typedef std::ptrdiff_t                     difference_type;
    typedef Type*                              pointer;
    typedef Type&                              reference;
/* %$$
$end
-------------------------------------------------------------------------------
$begin cppad_vector_itr_ctor$$
$spell
    Iterator
    ptr
    cppad
    Namespace
    CppAD
    const
    iterators
    itr
$$

$section Vector Class Iterator Member Data and Constructors$$

$head Constructors$$
$codei%cppad_vector_itr %itr%()
%$$
$codei%cppad_vector_itr %itr%(%data%, %length%, %index%)
%$$
$codei%cppad_vector_itr %itr%(%other%)
%$$

$head Namespace$$
These definitions are in the $code CppAD::local::utility$$ namespace.

$head Indirection$$
We use an extra level of indirection in this routine so that
the iterator has the same values as the vector even if the vector changes.

$head const_data_$$
is a pointer to a constant pointer to constant data for this vector
(used by operations supported by constant iterators).

$head data_$$
is a pointer to a constant pointer to data for this vector
(used by operations that are not supported by constant iterators).

$head length_$$
is a pointer to the length of the corresponding vector.

$head index_$$
is the current vector index corresponding to this iterator.

$head check_element$$
generates an assert with a known cause when the $code index_$$
does not correspond go a valid element and
$code NDEBUG$$ is not defined.

$head Source$$
$srccode%hpp% */
private:
    const Type* const* const_data_;
    Type* const*       data_;
    const size_t*      length_;
    size_t             index_;
    void check_element(void) const
    {   CPPAD_ASSERT_KNOWN( index_ < *length_,
            "CppAD vector iterator: accessing element out of range"
        );
    }
public:
    cppad_vector_itr(void)
    : const_data_(CPPAD_NULL), data_(CPPAD_NULL), length_(CPPAD_NULL), index_(0)
    { }
    cppad_vector_itr(
        const Type* const* data, const size_t* length, size_t index)
    { }
    cppad_vector_itr(Type* const* data, const size_t* length, size_t index)
    : const_data_(data), data_(data), length_(length), index_(index)
    { }
    void operator=(const cppad_vector_itr& other)
    {   const_data_ = other.const_data_;
        data_       = other.data_;
        length_     = other.length_;
        index_      = other.index_;
    }
    cppad_vector_itr(const cppad_vector_itr& other)
    {   *this = other; }
/* %$$
$end
-------------------------------------------------------------------------------
$begin cppad_vector_itr_inc$$
$spell
    Iterator
    itr
$$

$section Vector Class Iterator Increment Operators$$

$head Syntax$$
$codei%++%itr%
%$$
$icode%itr%++
%$$

$head Source$$
$srccode%hpp% */
public:
    cppad_vector_itr& operator++(void)
    {   ++index_;
        return *this;
    }
    cppad_vector_itr operator++(int)
    {   cppad_vector_itr ret(*this);
        ++index_;
        return ret;
    }
/* %$$
$end
-------------------------------------------------------------------------------
$begin cppad_vector_itr_equal$$
$spell
    itr
    Iterator
$$

$section Vector Class Iterator Equality Operators$$

$head Syntax$$
$icode%itr% == %other%
%$$
$icode%itr% != %other%
%$$

$head Source$$
$srccode%hpp% */
public:
    bool operator==(const cppad_vector_itr& other) const
    {   bool ret = const_data_ == other.const_data_;
        CPPAD_ASSERT_UNKNOWN( ret == (length_ == other.length_) );
        ret &= index_ == other.index_;
        return ret;
    }
    bool operator!=(const cppad_vector_itr& other) const
    {   return  ! ( *this == other ); }
/* %$$
$end
-------------------------------------------------------------------------------
$begin cppad_vector_itr_element$$
$spell
    itr
    Iterator
$$

$section Vector Class Iterator Access Elements$$

$head Syntax$$
$icode%element% = *%itr%
%$$
$codei%*%itr% = %element%
%$$

$head Source$$
$srccode%hpp% */
public:
    const Type& operator*(void) const
    {   check_element();
        return (*const_data_)[index_];
    }
    Type& operator*(void)
    {   check_element();
        CPPAD_ASSERT_KNOWN( data_ != CPPAD_NULL,
            "CppAD vector iterator: attempt to modify a constant iterator"
        );
        return (*data_)[index_];
    }
/* %$$
$end
*/
// ==========================================================================
}; // END_TEMPLATE_CLASS_CPPAD_VECTOR_ITR
// ==========================================================================
} } } // END_CPPAD_LOCAL_UTILITY_NAMESPACE
# endif
