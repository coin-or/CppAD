# ifndef  CPPAD_LOCAL_NEW_OPTIMIZE_SUBVECTOR_HPP
# define  CPPAD_LOCAL_NEW_OPTIMIZE_SUBVECTOR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2025 Bradley M. Bell
// --------------------------------------------------------------------------
/*
------------------------------------------------------------------------------
{xrst_begin mutable_subvector dev}
{xrst_spell
   subvectors
}

Mutable Subvectors With addr_t Elements
#######################################

Syntax
******
| |tab| ``mutable_subvector_t`` *mutable_subvector* ( )
| |tab| *mutable_subvector* . ``size`` ( )
| |tab| *mutable_subvector* . ``set``   ( *data*, *size* )
| |tab| *mutable_subvector* [ *index* ]

mutable_subvector
*****************
This constructs *mutable_subvector* with size zero:
{xrst_literal ,
   // BEGIN_MUTTABLE_CLASS , // END_MUTTABLE_CLASS
   // BEGIN_MUTTABLE_CTOR , // END_MUTTABLE_CTOR
}

size
****
This returns the size of elements in the subvector:
{xrst_literal ,
   // BEGIN_MUTTABLE_SIZE
   // END_MUTTABLE_SIZE
}

set
***
This changes *mutable_subvector* to a new subvector
{xrst_literal ,
   // BEGIN_MUTTABLE_SET
   // END_MUTTABLE_SET
}

data
====
is a raw pointer to the memory where the subvector starts

size
====
is the number of elements in the subvector.

Index Operator
**************
This returns a reference to the value at the specified index in the subvector:
{xrst_literal ,
   // BEGIN_MUTTABLE_INDEX , // END_MUTTABLE_INDEX
}

index
=====
in the subvector index,
the corresponding index in *vector* is *start* + *index* .

element
=======
is a reference to the element at the specified index in the subvector.


{xrst_end mutable_subvector}
*/

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
// BEGIN_MUTTABLE_CLASS
namespace CppAD { namespace local { namespace optimize  {
class mutable_subvector_t {
   // END_MUTTABLE_CLASS
   //
   // data_
   addr_t* data_;
   //
   // size_
   size_t size_;
public:
   //
   // BEGIN_MUTTABLE_CTOR
   // mutable_subvector_t mutable_subvector()
   mutable_subvector_t(void)
   // END_MUTTABLE_CTOR
   : data_(nullptr)
   , size_(0)
   { };
   //
   // destructor
   ~mutable_subvector_t(void)
   { };
   //
   // BEGIN_MUTTABLE_SIZE
   // size = mutable_subvector.size()
   size_t size(void) const
   // END_MUTTABLE_SIZE
   {  return size_; }
   //
   // BEGIN_MUTTABLE_SET
   // mutable_subvector.set(data, size)
   void set(addr_t *data, size_t size)
   // END_MUTTABLE_SET
   {  data_   = data;
      size_   = size;
   }
   //
   // BEGIN_MUTTABLE_INDEX
   // element = mutable_subvector[index]
   addr_t& operator[](size_t index) const
   // END_MUTTABLE_INDEX
   {  CPPAD_ASSERT_UNKNOWN( index < size_ );
      return data_[index];
   }
};
} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
/*
------------------------------------------------------------------------------
{xrst_begin const_subvector dev}
{xrst_spell
   subvectors
}

Constant Subvectors With addr_t Elements
########################################

Syntax
******
| |tab| ``const_subvector_t`` *const_subvector* ( )
| |tab| *const_subvector* . ``size`` ( )
| |tab| *const_subvector* . ``set``   ( *data*, *size* )
| |tab| *const_subvector* = *mutable_subvector*
| |tab| *const_subvector* [ *index* ]

const_subvector
***************
This constructs *const_subvector* with size zero:
{xrst_literal ,
   // BEGIN_CONST_CLASS , // END_CONST_CLASS
   // BEGIN_CONST_CTOR , // END_CONST_CTOR
}

size
****
This returns the size of elements in the subvector:
{xrst_literal ,
   // BEGIN_CONST_SIZE
   // END_CONST_SIZE
}

set
***
This changes *const_subvector* to a new subvector
{xrst_literal ,
   // BEGIN_CONST_SET
   // END_CONST_SET
}

Assignment Operator
*******************
This converts a :ref:`mutable_subvector-name` to a ``const_subvector`` :
{xrst_literal ,
   // BEGIN_CONST_ASSIGN , // END_CONST_ASSIGN
}


Index Operator
**************
This returns a reference to the value at the specified index in the subvector:
{xrst_literal ,
   // BEGIN_CONST_INDEX , // END_CONST_INDEX
}

index
=====
in the subvector index,
the corresponding index in *vector* is *start* + *index* .

element
=======
is a reference to the element at the specified index in the subvector.


{xrst_end const_subvector}
*/

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
// BEGIN_CONST_CLASS
namespace CppAD { namespace local { namespace optimize  {
class const_subvector_t {
   // END_CONST_CLASS
   //
   // data_
   const addr_t* data_;
   //
   // size_
   size_t size_;
public:
   //
   // BEGIN_CONST_CTOR
   // const_subvector_t const_subvector()
   const_subvector_t(void)
   // END_CONST_CTOR
   : data_(nullptr)
   , size_(0)
   { };
   //
   // destructor
   ~const_subvector_t(void)
   { };
   //
   // BEGIN_CONST_SIZE
   // size = const_subvector.size()
   size_t size(void) const
   // END_CONST_SIZE
   {  return size_; }
   //
   // BEGIN_CONST_SET
   // const_subvector.set(data, size)
   void set(const addr_t *data, size_t size)
   // END_CONST_SET
   {  data_   = data;
      size_   = size;
   }
   //
   // BEGIN_CONST_ASSIGN
   // const_subvector = mutable_subvector
   void operator=(const mutable_subvector_t& mutable_subvector)
   // END_CONST_ASSIGN
   {  data_ = &( mutable_subvector[0] );
      size_ = mutable_subvector.size();
   }
   //
   // BEGIN_CONST_INDEX
   // element = const_subvector[index]
   const addr_t& operator[](size_t index) const
   // END_CONST_INDEX
   {  CPPAD_ASSERT_UNKNOWN( index < size_ );
      return data_[index];
   }
};
} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

// --------------------------------------------------------------------------
# endif
