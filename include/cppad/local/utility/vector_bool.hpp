# ifndef CPPAD_LOCAL_UTILITY_VECTOR_BOOL_HPP
# define CPPAD_LOCAL_UTILITY_VECTOR_BOOL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cstddef>

// BEGIN_CPPAD_LOCAL_UTILITY_NAMESPACE
namespace CppAD { namespace local { namespace utility {
/*
{xrst_begin vector_bool_element dev}

vectorBoolElement Class
#######################

Syntax
******
| ``local::utility::vectorBoolElement`` *element* ( *unit* , *mask* )
| ``local::utility::vectorBoolElement`` *element* ( *other* )
| *value* = *element*
| *element* = *value*
| *element* = *element*

unit_t
******
Type used to pack multiple boolean (bit) values into one unit.
Logical operations are preformed one unit at a time.

unit\_
******
pointer to the unit that holds the value for this element.

mask\_
******
mask for the bit corresponding to this element; i.e., all the bits
are zero except for bit that corresponds to this element which is one.

value
*****
is a ``bool`` .

Source
******
{xrst_spell_off}
{xrst_code hpp} */
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
   {  return (*unit_ & mask_) != 0; }
   vectorBoolElement& operator=(bool value)
   {  if(value) *unit_ |= mask_;
      else      *unit_ &= ~mask_;
      return *this;
   }
   vectorBoolElement& operator=(const vectorBoolElement& element)
   {  if( *(element.unit_) & element.mask_ ) *unit_ |= mask_;
      else                                   *unit_ &= ~mask_;
      return *this;
   }
};
/* {xrst_code}
{xrst_spell_on}

{xrst_end vector_bool_element}
*/
} } } // END_CPPAD_LOCAL_UTILITY_NAMESPACE
# endif
