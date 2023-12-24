# ifndef CPPAD_LOCAL_SPARSE_PACK_SETVEC_HPP
# define CPPAD_LOCAL_SPARSE_PACK_SETVEC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/pod_vector.hpp>

// BEGIN_CPPAD_LOCAL_SPARSE_NAMESPACE
namespace CppAD { namespace local { namespace sparse {

// forward declaration of iterator class
class pack_setvec_const_iterator;

// ============================================================================
class pack_setvec {
// ============================================================================
/*
{xrst_begin pack_setvec_member_data dev}

class pack_setvec: Private Member Data
######################################

Pack
****
Type used to pack multiple elements of a set (multiple bits) onto one
*Pack* value.

n_bit\_
*******
Number of bits (elements) per *Pack* value.

zero\_
******
The *Pack* value with all bits zero.

one\_
*****
The *Pack* value with all bits zero, except for the lowest order bit.

n_set\_
*******
Number of sets that we are representing.

end\_
*****
The possible elements in each set are ``0`` , ``1`` , ...,
``end_-1`` .

n_pack\_
********
Number of Pack values used to represent one set in the vector; i.e.,
to represent ``end_`` bits.

data\_
******
Data for all of the sets.

Source Code
***********
{xrst_spell_off}
{xrst_code hpp} */
private:
   typedef size_t    Pack;
   const size_t      n_bit_;
   const Pack        zero_;
   const Pack        one_;
   size_t            n_set_;
   size_t            end_;
   size_t            n_pack_;
   pod_vector<Pack>  data_;
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_member_data}
-----------------------------------------------------------------------------
{xrst_begin pack_setvec_vec_memory dev}

class pack_setvec: Approximate Memory Used by Vector
####################################################

Public
******
This function is declared public, but is not part of
:ref:`SetVector-name` concept.

Implementation
**************
{xrst_spell_off}
{xrst_code hpp} */
public:
   size_t memory(void) const
   {  return data_.capacity() * sizeof(Pack); }
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_vec_memory}
-------------------------------------------------------------------------------
{xrst_begin pack_setvec_vec_print dev}

class pack_setvec: Print a Vector of Sets
#########################################

Public
******
This function is declared public, but is not part of
:ref:`SetVector-name` concept.

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void print(void) const;
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_vec_print}
-------------------------------------------------------------------------------
{xrst_begin pack_setvec_iterators dev}
{xrst_spell
   typedef
}

class pack_setvec: Iterators
############################

SetVector Concept
*****************
:ref:`SetVector@const_iterator`

typedef
*******
{xrst_spell_off}
{xrst_code hpp} */
public:
   /// declare a const iterator
   friend class pack_setvec_const_iterator;
   typedef pack_setvec_const_iterator const_iterator;
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_iterators}
-------------------------------------------------------------------------------
{xrst_begin pack_setvec_default_ctor dev}

class pack_setvec: Default Constructor
######################################

SetVector Concept
*****************
:ref:`SetVector@Vector Operations@Constructor`

n_bit\_
*******
This member variable is set to the number of bits in a *Pack* value.

one\_
*****
This member variable has only its lowest order bit non-zero;

data\_
******
This member is initialized as the empty vector; i.e., size zero..

Other
*****
All the other member data are ``size_t`` values
that are initialized as zero.

Implementation
**************
{xrst_spell_off}
{xrst_code hpp} */
public:
   pack_setvec(void) :
   n_bit_( std::numeric_limits<Pack>::digits ),
   zero_(0), one_(1), n_set_(0), end_(0), n_pack_(0), data_(0)
   { }
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_default_ctor}
-------------------------------------------------------------------------------
{xrst_begin pack_setvec_destructor dev}

class pack_setvec: Destructor
#############################

Implementation
**************
{xrst_spell_off}
{xrst_code hpp} */
public:
   ~pack_setvec(void)
   { }
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_destructor}
-------------------------------------------------------------------------------
{xrst_begin pack_setvec_copy_ctor dev}

class pack_setvec: Copy Constructor
###################################

v
*
The vector of sets that we are attempting to make a copy of.

Implementation
**************
Using the copy constructor is probably due to a ``pack_setvec``
being passed by value instead of by reference.
This is a CppAD programing error (not CppAD user error).
{xrst_spell_off}
{xrst_code hpp} */
public:
   pack_setvec(const pack_setvec& v) :
   n_bit_( std::numeric_limits<Pack>::digits ), zero_(0), one_(1)
   {  CPPAD_ASSERT_UNKNOWN(0); }
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_copy_ctor}
-------------------------------------------------------------------------------
{xrst_begin pack_setvec_vec_resize dev}

class pack_setvec: Vector resize
################################

SetVector Concept
*****************
:ref:`vector resize<SetVector@Vector Operations@resize>`

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void resize(size_t n_set, size_t end)
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_vec_resize}
*/
   {  n_set_          = n_set;
      end_            = end;
      if( n_set_ == 0 )
      {  CPPAD_ASSERT_UNKNOWN( end == 0 );
         data_.clear();
         return;
      }
      // now start a new vector with empty sets
      Pack zero(0);
      //
      n_pack_         = ( 1 + (end_ - 1) / n_bit_ );
      size_t i        = n_set_ * n_pack_;
      //
      data_.resize(i);
      while(i--)
         data_[i] = zero;
   }
/* %$$
-------------------------------------------------------------------------------
{xrst_begin pack_setvec_vec_n_set dev}

class pack_setvec: Number of Sets
#################################

SetVector Concept
*****************
:ref:`SetVector@Vector Operations@n_set`

Implementation
**************
{xrst_spell_off}
{xrst_code hpp} */
public:
   size_t n_set(void) const
   {  return n_set_;  }
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_vec_n_set}
-------------------------------------------------------------------------------
{xrst_begin pack_setvec_vec_end dev}

class pack_setvec: End Value
############################

SetVector Concept
*****************
:ref:`SetVector@Vector Operations@end`

Implementation
**************
{xrst_spell_off}
{xrst_code hpp} */
public:
   size_t end(void) const
   {  return end_; }
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_vec_end}
-------------------------------------------------------------------------------
{xrst_begin pack_setvec_vec_assignment dev}

class pack_setvec: Vector Assignment
####################################

SetVector Concept
*****************
:ref:`vector assignment<SetVector@Vector Operations@Assignment>`

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void operator=(const pack_setvec& other)
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_vec_assignment}
*/
   {  CPPAD_ASSERT_UNKNOWN( n_bit_  == other.n_bit_);
      CPPAD_ASSERT_UNKNOWN( zero_   == other.zero_);
      CPPAD_ASSERT_UNKNOWN( one_    == other.one_);
      n_set_  = other.n_set_;
      end_    = other.end_;
      n_pack_ = other.n_pack_;
      data_   = other.data_;
   }
/*
-------------------------------------------------------------------------------
{xrst_begin pack_setvec_vec_swap dev}

class pack_setvec: Vector Swap
##############################

SetVector Concept
*****************
:ref:`vector swap<SetVector@Vector Operations@swap>`

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void swap(pack_setvec& other)
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_vec_swap}
*/
   {  // size_t objects
      CPPAD_ASSERT_UNKNOWN( n_bit_  == other.n_bit_);
      CPPAD_ASSERT_UNKNOWN( zero_   == other.zero_);
      CPPAD_ASSERT_UNKNOWN( one_    == other.one_);
      std::swap(n_set_  , other.n_set_);
      std::swap(end_    , other.end_);
      std::swap(n_pack_ , other.n_pack_);
      //
      // pod_vectors
      data_.swap(other.data_);
   }
/*
-------------------------------------------------------------------------------
{xrst_begin pack_setvec_number_elements dev}

class pack_setvec: Number of Elements in a Set
##############################################

SetVector Concept
*****************
:ref:`SetVector@number_elements`

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   size_t number_elements(size_t i) const
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_number_elements}
*/
   {  CPPAD_ASSERT_UNKNOWN( i < n_set_ );
      //
      // special case where data_[i] is 0 or 1
      if( end_ == 1 )
      {  CPPAD_ASSERT_UNKNOWN( n_pack_ == 1 );
         return size_t( data_[i] );
      }
      //
      // initialize count of non-zero bits in this set
      size_t count = 0;
      //
      // mask corresonding to first bit in Pack
      Pack mask = one_;
      //
      // number of bits in last Packing unit
      size_t n_last = (end_ - 1) % n_bit_ + 1;
      //
      // count bits in last unit
      Pack unit = data_[(i + 1) * n_pack_ - 1];
      for(size_t bit = 0; bit < n_last; ++bit)
      {  CPPAD_ASSERT_UNKNOWN( mask >= one_ );
         if( mask & unit )
            ++count;
         mask = mask << 1;
      }
      if( n_pack_ == 1 )
         return count;
      //
      // count bits in other units
      for(size_t bit = 0; bit < n_bit_; ++bit)
      {  CPPAD_ASSERT_UNKNOWN( mask >= one_ );
         size_t k = n_pack_;
         while(--k)
         {  if( data_[i * n_pack_ + k] & mask )
               ++count;
         }
         mask = mask << 1;
      }
      return count;
   }
/*
-------------------------------------------------------------------------------
{xrst_begin pack_setvec_add_element dev}

class pack_setvec: Add an Elements to a Set
###########################################

SetVector Concept
*****************
:ref:`SetVector@add_element`

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void add_element(size_t i, size_t element)
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_add_element}
*/
   {  CPPAD_ASSERT_UNKNOWN( i   < n_set_ );
      CPPAD_ASSERT_UNKNOWN( element < end_ );
      if( end_ == 1 )
         data_[i] |= one_;
      else
      {  size_t j  = element / n_bit_;
         size_t k  = element - j * n_bit_;
         Pack mask = one_ << k;
         data_[ i * n_pack_ + j] |= mask;
      }
   }
/*
-------------------------------------------------------------------------------
{xrst_begin pack_setvec_post_element dev}

class pack_setvec: Add an Elements to a Set
###########################################

SetVector Concept
*****************
:ref:`SetVector@post_element`

Implementation
**************
{xrst_spell_off}
{xrst_code hpp} */
public:
   void post_element(size_t i, size_t element)
   {  add_element(i, element); }
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_post_element}
*/
/*
-------------------------------------------------------------------------------
{xrst_begin pack_setvec_process_post dev}

class pack_setvec: Add Posted Elements to a Set
###############################################

SetVector Concept
*****************
:ref:`SetVector@process_post`

Implementation
**************
{xrst_spell_off}
{xrst_code hpp} */
public:
   void process_post(size_t i)
   {  return; }
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_process_post}
-------------------------------------------------------------------------------
{xrst_begin pack_setvec_is_element dev}

class pack_setvec: Is an Element in a Set
#########################################

SetVector Concept
*****************
:ref:`SetVector@is_element`

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   bool is_element(size_t i, size_t element) const
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_is_element}
*/
   {  CPPAD_ASSERT_UNKNOWN( i   < n_set_ );
      CPPAD_ASSERT_UNKNOWN( element < end_ );
      if( end_ == 1 )
         return data_[i] != zero_;
      //
      size_t j  = element / n_bit_;
      size_t k  = element - j * n_bit_;
      Pack mask = one_ << k;
      return (data_[i * n_pack_ + j] & mask) != zero_;
   }
/*
-------------------------------------------------------------------------------
{xrst_begin pack_setvec_clear dev}

class pack_setvec: Assign a Set to be Empty
###########################################

SetVector Concept
*****************
:ref:`SetVector@clear`

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void clear(size_t target)
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_clear}
*/
   {  CPPAD_ASSERT_UNKNOWN( target < n_set_ );
      size_t t = target * n_pack_;

      size_t j = n_pack_;
      while(j--)
         data_[t++] = zero_;
   }
/*
-------------------------------------------------------------------------------
{xrst_begin pack_setvec_assignment dev}

class pack_setvec: Assign a Set To Equal Another Set
####################################################

SetVector Concept
*****************
:ref:`SetVector@assignment`

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void assignment(
      size_t               this_target  ,
      size_t               other_value  ,
      const pack_setvec&   other        )
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_assignment}
*/
   {  CPPAD_ASSERT_UNKNOWN( this_target  <   n_set_        );
      CPPAD_ASSERT_UNKNOWN( other_value  <   other.n_set_  );
      CPPAD_ASSERT_UNKNOWN( n_pack_      ==  other.n_pack_ );
      size_t t = this_target * n_pack_;
      size_t v = other_value * n_pack_;

      size_t j = n_pack_;
      while(j--)
         data_[t++] = other.data_[v++];
   }
/*
-------------------------------------------------------------------------------
{xrst_begin pack_setvec_binary_union dev}

class pack_setvec: Assign a Set To Equal Union of Two Sets
##########################################################

SetVector Concept
*****************
:ref:`SetVector@binary_union`

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void binary_union(
      size_t                  this_target  ,
      size_t                  this_left    ,
      size_t                  other_right  ,
      const pack_setvec&      other        )
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_binary_union}
*/
   {  CPPAD_ASSERT_UNKNOWN( this_target < n_set_         );
      CPPAD_ASSERT_UNKNOWN( this_left   < n_set_         );
      CPPAD_ASSERT_UNKNOWN( other_right < other.n_set_   );
      CPPAD_ASSERT_UNKNOWN( n_pack_    ==  other.n_pack_ );

      size_t t  = this_target * n_pack_;
      size_t l  = this_left  * n_pack_;
      size_t r  = other_right * n_pack_;

      size_t j = n_pack_;
      while(j--)
         data_[t++] = ( data_[l++] | other.data_[r++] );
   }
/*
-------------------------------------------------------------------------------
{xrst_begin pack_setvec_binary_intersection dev}

class pack_setvec: Assign a Set To Intersection of Two Sets
###########################################################

SetVector Concept
*****************
:ref:`SetVector@binary_intersection`

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   void binary_intersection(
      size_t                  this_target  ,
      size_t                  this_left    ,
      size_t                  other_right  ,
      const pack_setvec&      other        )
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_binary_intersection}
*/
   {  CPPAD_ASSERT_UNKNOWN( this_target < n_set_         );
      CPPAD_ASSERT_UNKNOWN( this_left   < n_set_         );
      CPPAD_ASSERT_UNKNOWN( other_right < other.n_set_   );
      CPPAD_ASSERT_UNKNOWN( n_pack_    ==  other.n_pack_ );

      size_t t  = this_target * n_pack_;
      size_t l  = this_left  * n_pack_;
      size_t r  = other_right * n_pack_;

      size_t j = n_pack_;
      while(j--)
         data_[t++] = ( data_[l++] & other.data_[r++] );
   }
// ==========================================================================
}; // END_CLASS_PACK_SETVEC
// ==========================================================================


// =========================================================================
class pack_setvec_const_iterator { // BEGIN_CLASS_PACK_SETVEC_CONST_ITERATOR
// =========================================================================

/*
{xrst_begin pack_setvec_const_iterator_member_data dev}

class pack_setvec_const_iterator private: Member Data
#####################################################

Pack
****
This is the same type as
:ref:`pack_setvec Pack<pack_setvec_member_data@Pack>` .

n_bit\_
*******
This is a reference to
:ref:`pack_setvec n_bit_<pack_setvec_member_data@n_bit_>` .

one\_
*****
This is a reference to
:ref:`pack_setvec one_<pack_setvec_member_data@one_>` .

n_pack\_
********
This is a reference to
:ref:`pack_setvec n_pack_<pack_setvec_member_data@n_pack_>` .

end\_
*****
This is a reference to
:ref:`pack_setvec end_<pack_setvec_member_data@end_>` .

data\_
******
This is a reference to
:ref:`pack_setvec data_<pack_setvec_member_data@data_>` .

data_index\_
************
Index in ``data_`` where the next element is located.

next_element
************
Value of the next element in this set
If ``next_element_`` equals ``end_`` ,
no next element exists; i.e., past end of the set.

Source Code
***********
{xrst_spell_off}
{xrst_code hpp} */
private:
   typedef pack_setvec::Pack Pack;
   const size_t&             n_bit_;
   const Pack&               one_;
   const size_t&             n_pack_;
   const size_t&             end_;
   const pod_vector<Pack>&   data_;
   size_t                    data_index_;
   size_t                    next_element_;
public:
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_const_iterator_member_data}
-------------------------------------------------------------------------------
{xrst_begin pack_setvec_const_iterator_ctor dev}

class pack_setvec_const_iterator: Constructor
#############################################

SetVector Concept
*****************
:ref:`iterator constructor<SetVector@const_iterator@Constructor>`

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   pack_setvec_const_iterator (const pack_setvec& pack, size_t set_index)
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_const_iterator_ctor}
*/
   :
   n_bit_         ( pack.n_bit_ )        ,
   one_           ( pack.one_   )        ,
   n_pack_        ( pack.n_pack_ )       ,
   end_           ( pack.end_ )          ,
   data_          ( pack.data_ )         ,
   data_index_    ( set_index * n_pack_ )
   {  CPPAD_ASSERT_UNKNOWN( set_index < pack.n_set_ );
      CPPAD_ASSERT_UNKNOWN( 0 < end_ );
      //
      next_element_ = 0;
      if( data_[data_index_] & one_ )
         return;
      //
      // element with index zero is not in this set of integers,
      // advance to first element or end
      ++(*this);
   }
/*
-------------------------------------------------------------------------------
{xrst_begin pack_setvec_const_iterator_dereference dev}

class pack_setvec_const_iterator: Dereference
#############################################

SetVector Concept
*****************
:ref:`iterator deference<SetVector@const_iterator@Dereference>`

Implementation
**************
{xrst_spell_off}
{xrst_code hpp} */
   size_t operator*(void) const
   {  return next_element_; }
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_const_iterator_dereference}
-------------------------------------------------------------------------------
{xrst_begin pack_setvec_const_iterator_increment dev}

class pack_setvec_const_iterator: Increment
###########################################

SetVector Concept
*****************
:ref:`iterator increment<SetVector@const_iterator@Increment>`

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
public:
   pack_setvec_const_iterator& operator++(void)
/* {xrst_code}
{xrst_spell_on}

{xrst_end pack_setvec_const_iterator_increment}
*/
   {  CPPAD_ASSERT_UNKNOWN( next_element_ <= end_ );
      if( next_element_ == end_ )
         return *this;
      //
      ++next_element_;
      if( next_element_ == end_ )
         return *this;
      //
      // bit index corresponding to next element
      size_t bit = next_element_ % n_bit_;
      //
      // check if we have advanced to the next data index
      if( bit == 0 )
         ++data_index_;
      //
      // initialize mask
      size_t mask = one_ << bit;
      //
      while( next_element_ < end_ )
      {  // check if this element is in the set
         if( data_[data_index_] & mask )
            return *this;
         //
         // try next larger element
         ++next_element_;
         ++bit;
         mask <<= 1;
         //
         // check if we must go to next packed data index
         CPPAD_ASSERT_UNKNOWN( bit <= n_bit_ );
         if( bit == n_bit_ )
         {  // get next packed value
            bit   = 0;
            mask  = one_;
            ++data_index_;
         }
      }
      CPPAD_ASSERT_UNKNOWN( next_element_ == end_ );
      return *this;
   }
// =========================================================================
}; // END_CLASS_PACK_SETVEC_CONST_ITERATOR
// =========================================================================

// Implemented after pack_setvec_const_iterator so can use it
inline void pack_setvec::print(void) const
{  std::cout << "pack_setvec:\n";
   for(size_t i = 0; i < n_set(); i++)
   {  std::cout << "set[" << i << "] = {";
      const_iterator itr(*this, i);
      while( *itr != end() )
      {  std::cout << *itr;
         if( *(++itr) != end() )
            std::cout << ",";
      }
      std::cout << "}\n";
   }
   return;
}

// ----------------------------------------------------------------------------
/*
{xrst_begin sparsity_user2internal_pack_setvec dev}
{xrst_spell
   msg
}

Copy A Boolean Sparsity Pattern To A pack_setvec Object
#######################################################

SetVector
*********
is a :ref:`simple vector<SimpleVector-name>` type with elements of type
``bool`` containing the input sparsity pattern.

internal
********
The input value of this object does not matter.
Upon return it contains the same sparsity pattern as *user*
(or its transpose).

user
****
is the sparsity pattern we are copying to *internal* .

n_set
*****
is the number of sets in the output sparsity pattern *internal* .

end
***
is the end value for the output sparsity pattern *internal* .

transpose
*********
If *transpose* is false,
element *j* is in the *i*-th *internal* set if

   *user* [ *i* * *end* + *j*  ]

Otherwise,
element *j* is in the *i*-th *internal* set if

   *user* [ *i* * *n_set* + *j*  ]

error_msg
*********
is the error message to display if

   *n_set* * *end* != *user* . ``size`` ()

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
template<class SetVector>
void sparsity_user2internal(
   pack_setvec&            internal  ,
   const SetVector&        user      ,
   size_t                  n_set     ,
   size_t                  end       ,
   bool                    transpose ,
   const char*             error_msg )
/* {xrst_code}
{xrst_spell_on}

{xrst_end sparsity_user2internal_pack_setvec}
*/
{  CPPAD_ASSERT_KNOWN(size_t( user.size() ) == n_set * end, error_msg );

   // size of internal sparsity pattern
   internal.resize(n_set, end);

   if( transpose )
   {  // transposed pattern case
      for(size_t j = 0; j < end; j++)
      {  for(size_t i = 0; i < n_set; i++)
         {  // no advantage to using post_element for pack_setvec
            if( user[ j * n_set + i ] )
               internal.add_element(i, j);
         }
      }
      return;
   }
   else
   {  for(size_t i = 0; i < n_set; i++)
      {  for(size_t j = 0; j < end; j++)
         {  // no advantage to using post_element for pack_setvec
            if( user[ i * end + j ] )
               internal.add_element(i, j);
         }
      }
   }
   return;
}

} } } // END_CPPAD_LOCAL_SPARSE_NAMESPACE

# endif
