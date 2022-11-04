# ifndef CPPAD_LOCAL_UTILITY_VECTOR_BOOL_HPP
# define CPPAD_LOCAL_UTILITY_VECTOR_BOOL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cstddef>

// BEGIN_CPPAD_LOCAL_UTILITY_NAMESPACE
namespace CppAD { namespace local { namespace utility {
/*
$begin vector_bool_element$$
$spell
    Bool
$$

$section vectorBoolElement Class$$

$head Syntax$$
$codei%local::utility::vectorBoolElement %element%(%unit%, %mask%)
%$$
$codei%local::utility::vectorBoolElement %element%(%other%)
%$$
$icode%value% = %element%
%$$
$icode%element% = %value%
%$$
$icode%element% = %element%
%$$

$head unit_t$$
Type used to pack multiple boolean (bit) values into one unit.
Logical operations are preformed one unit at a time.

$head unit_$$
pointer to the unit that holds the value for this element.

$head mask_$$
mask for the bit corresponding to this element; i.e., all the bits
are zero except for bit that corresponds to this element which is one.

$head value$$
is a $code bool$$.

$head Source$$
$srccode%hpp% */
class vectorBoolElement {
private:
    typedef size_t unit_t;
    unit_t* unit_;
    unit_t  mask_;
public:
    vectorBoolElement(unit_t* unit, unit_t mask )
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
} } } // END_CPPAD_LOCAL_UTILITY_NAMESPACE
# endif
