# ifndef CPPAD_UTILITY_SPARSE_RC_HPP
# define CPPAD_UTILITY_SPARSE_RC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin sparse_rc}
{xrst_spell
   nnz
   nr
   ostream
}
Row and Column Index Sparsity Patterns
######################################

Syntax
******

include
=======

   # ``include <cppad/utility/sparse_rc.hpp>``

Constructor
===========

| ``sparse_rc`` < *SizeVector* > *empty*
| ``sparse_rc`` < *SizeVector* > *pattern* ( *nr* , *nc* , *nnz* )
| ``sparse_rc`` < *SizeVector* > *pattern* ( *other* )

Assignment
==========

| *pattern* = *other*
| *pattern* . ``swap`` ( *other* )

Equality
========

   *equal* = *pattern* == *other*

.

Setting
=======

| *resize* ( *nr* , *nc* , *nnz* )
| *pattern* . ``set`` ( *k* , *r* , *c* )
| *pattern* . ``push_back`` ( *r* , *c* )
| *pattern* . ``set_row_major`` ()
| *pattern* . ``set_col_major`` ()

Scalars
=======

| *pattern* . ``nr`` ()
| *pattern* . ``nc`` ()
| *pattern* . ``nnz`` ()

Vectors
=======

| ``const`` *SizeVector* & *row* ( *pattern* . ``row`` () )
| ``const`` *SizeVector* & *col* ( *pattern* . ``col`` () )
| ``const`` *SizeVector* & *row_major* ( *pattern* . ``get_row_major`` () )
| ``const`` *SizeVector* & *col_major* ( *pattern* . ``get_col_major`` () )
| *row_major* = *pattern* . ``row_major`` ()
| *col_major* = *pattern* . ``col_major`` ()

Output
======

   *os* << *pattern*

SizeVector
**********
We use *SizeVector* to denote :ref:`SimpleVector-name` class
:ref:`with elements of type<SimpleVector@Elements of Specified Type>`
``size_t`` .
In addition, *SimpleVector* must support the ``swap`` operation
between two of its vectors.

empty
*****
This is an empty sparsity pattern. To be specific,
the corresponding number of rows *nr* ,
number of columns *nc* ,
and number of possibly non-zero values *nnz* ,
are all zero.

pattern
*******
This object is used to hold a sparsity pattern for a matrix.
The sparsity *pattern* is ``const``
except during its constructor, ``resize`` , and ``set`` .

other
*****

Assignment and Constructor
==========================
In the assignment and constructor, *other* has prototype

   ``const sparse_rc`` < *SizeVector* >& *other*

After the assignment and constructor, *pattern* is an independent copy
of *other* ; i.e. it has all the same values as *other*
and changes to *pattern* do not affect *other* .

Move Semantics Assignment and Constructor
=========================================
In the assignment and constructor, if *other* has prototype

   ``sparse_rc`` < *SizeVector* >&& *other*

A move semantics version of the assignment or constructor is used; e.g.,
when *other* is a function return value.

swap
====
In the swap operation, *other* has prototype

   ``sparse_rc`` < *SizeVector* >& *other*

After the swap operation *other* ( *pattern* ) is equivalent
to *pattern* ( *other* ) before the operation.

Equality
========
In the equality operation, *other* has prototype

   ``const sparse_rc`` < *SizeVector* >& *other*

The two sparsity patterns are equal if the following conditions hold:

#. The number of rows
   *pattern* . ``nr`` () and *other* . ``nr`` () are equal.
#. The number of columns
   *pattern* . ``nc`` () and *other* . ``nc`` () are equal.
#. The number of non-zero values
   *pattern* . ``nnz`` () and *other* . ``nnz`` () are equal.
#. The set of (row, column) pairs corresponding to
   *pattern* and *other* , are equal.

Determining equality requires sorting both patterns

nr
**
This argument has prototype

   ``size_t`` *nr*

It specifies the number of rows in the sparsity pattern.
The function call ``nr()`` returns the value of *nr* .

nc
**
This argument has prototype

   ``size_t`` *nc*

It specifies the number of columns in the sparsity pattern.
The function call ``nc()`` returns the value of *nc* .

nnz
***
This argument has prototype

   ``size_t`` *nnz*

It specifies the number of possibly non-zero
index pairs in the sparsity pattern.
The function call ``nnz()`` returns the value of *nnz* .

resize
******
The current sparsity pattern is lost and a new one is started
with the specified parameters. The elements in the *row*
and *col* vectors should be assigned using ``set`` .

set
***
This function sets the values

| |tab| *row* [ *k* ] = *r*
| |tab| *col* [ *k* ] = *c*

push_back
*********
This function  the value *r* to the back of *row* ,
the value *c* to the back of *col* ,
and increases *nnz* by one.
This operation requires *SizeVector* to support the
``push_back`` operation
(which is not part of the SimpleVector requirements).

k
=
This argument has type

   ``size_t`` *k*

and must be less than *nnz* .

r
=
This argument has type

   ``size_t`` *r*

It specifies the value assigned to *row* [ *k* ] and must
be less than *nr* .

c
=
This argument has type

   ``size_t`` *c*

It specifies the value assigned to *col* [ *k* ] and must
be less than *nc* .

row
***
This vector has size *nnz* and
*row* [ *k* ]
is the row index of the *k*-th possibly non-zero
index pair in the sparsity pattern.

col
***
This vector has size *nnz* and
*col* [ *k* ] is the column index of the *k*-th possibly non-zero
index pair in the sparsity pattern.

row_major
*********
This vector has prototype

   *SizeVector* *row_major*

and its size *nnz* .
It sorts the sparsity pattern in row-major order.
To be specific,

   *col* [ *row_major* [ *k* ] ] <= *col* [ *row_major* [ *k* +1] ]

and if *col* [ *row_major* [ *k* ] ] == *col* [ *row_major* [ *k* +1] ] ,

   *row* [ *row_major* [ *k* ] ] < *row* [ *row_major* [ *k* +1] ]

This routine generates an assert if there are two entries with the same
row and column values (if ``NDEBUG`` is not defined).

set_row_major
*************
Store the current row major order in *pattern* .
This can be used by the row_major function and the equality function
to avoid re-sorting the pattern each time.

get_row_major
*************
Retrieve the row major order stored in *pattern*
by the previous ``set_row_major`` .
If this order is no longer valid, the return value
*row_major* has size zero.

col_major
*********
This vector has prototype

   *SizeVector* *col_major*

and its size *nnz* .
It sorts the sparsity pattern in column-major order.
To be specific,

   *row* [ *col_major* [ *k* ] ] <= *row* [ *col_major* [ *k* +1] ]

and if *row* [ *col_major* [ *k* ] ] == *row* [ *col_major* [ *k* +1] ] ,

   *col* [ *col_major* [ *k* ] ] < *col* [ *col_major* [ *k* +1] ]

This routine generates an assert if there are two entries with the same
row and column values (if ``NDEBUG`` is not defined).

set_col_major
*************
Store the current row major order in *pattern* .
This can be used by the col_major function and the equality function
to avoid re-sorting the pattern each time.

get_col_major
*************
Retrieve the row major order stored in *pattern*
by the previous ``set_col_major`` .
If this order is no longer valid, the return value
*col_major* has size zero.
{xrst_toc_hidden
   example/utility/sparse_rc.cpp
}
Example
*******
The file :ref:`sparse_rc.cpp-name`
contains an example and test of this class.

os
**
If *os* is an ``std::ostream`` , the operation

   *os* << *pattern*

outputs *pattern* to the *os* stream.
The output begins with a left brace ``{``
and ends with a right brace ``}`` .
The output is in row major order and has one line for each row.
The row index is output at the beginning of a line
and the column indices follow.

{xrst_end sparse_rc}
*/
# include <cstddef> // for size_t
# include <cppad/core/cppad_assert.hpp>  // for CPPAD_ASSERT
# include <cppad/utility/index_sort.hpp> // for row and column major ordering

namespace CppAD { // BEGIN CPPAD_NAMESPACE

// sparsity pattern for a matrix with indices of type size_t
template <class SizeVector>
class sparse_rc {
private:
   // number of rows in the sparsity pattern
   size_t nr_;
   //
   // number of columns in the sparsity pattern
   size_t nc_;
   //
   // number of possibly non-zero index pairs
   size_t nnz_;
   //
   // row_[k] is the row index for the k-th possibly non-zero entry
   SizeVector row_;
   //
   // col_[k] is the column index for the k-th possibly non-zero entry
   SizeVector col_;
   //
   // if row_major_.size() != 0, row_major_[k] is index of k-th non-zero
   // entry in row major order.
   SizeVector row_major_;
   //
   // if col_major_.size() != 0, col_major_[k] is index of k-th non-zero
   // entry in column major order.
   SizeVector col_major_;
   //
   // simple_vector_assign
   static void simple_vector_assign(
      SizeVector& destination, const SizeVector& source
   )
   {  // resize to zero first so do not copy any values
      destination.resize(0);
      // size agreement required for simple vector
      destination.resize( source.size() );
      // assignment
      destination = source;
   }
public:
   // default constructor
   // Eigen vector is ambiguous for row_(0), col_(0) so use default ctor
   sparse_rc(void)
   : nr_(0), nc_(0), nnz_(0)
   { }
   //
   // move semantics constructor
   // (none of the default constructor values are used by destructor)
   sparse_rc(sparse_rc&& other)
   {  swap(other); }
   //
   // destructor
   ~sparse_rc(void)
   { }
   //
   // sizing constructor
   // Eigen vector is ambiguous for row_(0), col_(0) so use default ctor
   sparse_rc(size_t nr, size_t nc, size_t nnz)
   : nr_(nr), nc_(nc), nnz_(nnz)
   {  row_.resize(nnz);
      col_.resize(nnz);
   }
   //
   // copy constructor
   sparse_rc(const sparse_rc& other)
   :
   nr_(other.nr_)   ,
   nc_(other.nc_)   ,
   nnz_(other.nnz_) ,
   row_(other.row_) ,
   col_(other.col_) ,
   row_major_(other.row_major_) ,
   col_major_(other.col_major_)
   { }
   //
   // assignment
   void operator=(const sparse_rc& other)
   {  nr_  = other.nr_;
      nc_  = other.nc_;
      nnz_ = other.nnz_;
      //
      simple_vector_assign(row_, other.row_);
      simple_vector_assign(col_, other.col_);
      simple_vector_assign(row_major_, other.row_major_);
      simple_vector_assign(col_major_, other.col_major_);
   }
   //
   // swap
   void swap(sparse_rc& other)
   {  std::swap( nr_ , other.nr_ );
      std::swap( nc_ , other.nc_ );
      std::swap( nnz_ , other.nnz_ );
      //
      row_.swap( other.row_ );
      col_.swap( other.col_ );
      row_major_.swap( other.row_major_ );
      col_major_.swap( other.col_major_ );
   }
   //
   // move semantics assignment
   void operator=(sparse_rc&& other)
   {  swap(other); }
   //
   // equality
   bool operator==(const sparse_rc& other) const
   {  // result
      bool result = true;
      result &= nr_  == other.nr_;
      result &= nc_  == other.nc_;
      result &= nnz_ == other.nnz_;
      if( ! result )
         return result;
      //
      // this_order, other_order, this_order_ptr, other_order_ptr
      SizeVector        this_order;
      SizeVector        other_order;
      const SizeVector* this_order_ptr = &this_order;
      const SizeVector* other_order_ptr = &other_order;
      bool this_row_ok  = row_major_.size() > 0;
      bool this_col_ok  = col_major_.size() > 0;
      bool other_row_ok = other.row_major_.size() > 0;
      bool other_col_ok = other.col_major_.size() > 0;
      if( this_row_ok && this_row_ok )
      {  this_order_ptr  = &row_major_;
         other_order_ptr = &(other.row_major_);
      }
      else if( this_col_ok && this_col_ok )
      {  this_order_ptr  = &col_major_;
         other_order_ptr = &(other.col_major_);
      }
      else if( this_row_ok )
      {  this_order_ptr = &row_major_;
         other_order    = other.row_major();
      }
      else if( this_col_ok )
      {  this_order_ptr = &col_major_;
         other_order    = other.col_major();
      }
      else if( other_row_ok )
      {  other_order_ptr = &(other.row_major_);
         this_order      = row_major();
      }
      else if( other_col_ok )
      {  other_order_ptr = &(other.col_major_);
         this_order      = col_major();
      }
      else
      {  this_order  = row_major();
         other_order = other.row_major();
      }
      //
      // result
      for(size_t k = 0; k < nnz_; ++k)
      {  size_t this_k  = (*this_order_ptr)[k];
         size_t other_k = (*other_order_ptr)[k];
         result &= row_[this_k] == other.row_[other_k];
         result &= col_[this_k] == other.col_[other_k];
      }
      return result;
   }
   //
   // resize
   void resize(size_t nr, size_t nc, size_t nnz)
   {  nr_ = nr;
      nc_ = nc;
      nnz_ = nnz;
      row_.resize(nnz);
      col_.resize(nnz);
      row_major_.resize(0);
      col_major_.resize(0);
   }
   //
   // set row and column for a possibly non-zero element
   void set(size_t k, size_t r, size_t c)
   {  CPPAD_ASSERT_KNOWN(
         k < nnz_,
         "The index k is not less than nnz in sparse_rc::set"
      );
      CPPAD_ASSERT_KNOWN(
         r < nr_,
         "The index r is not less than nr in sparse_rc::set"
      );
      CPPAD_ASSERT_KNOWN(
         c < nc_,
         "The index c is to not less than nc in sparse_rc::set"
      );
      row_[k] = r;
      col_[k] = c;
      //
      row_major_.resize(0);
      col_major_.resize(0);
   }
   //
   // push_back
   void push_back(size_t r, size_t c)
   {  CPPAD_ASSERT_KNOWN(
         r < nr_,
         "The index r is not less than nr in sparse_rc::push_back"
      );
      CPPAD_ASSERT_KNOWN(
         c < nc_,
         "The index c is to not less than nc in sparse_rc::push_back"
      );
      row_.push_back(r);
      col_.push_back(c);
      ++nnz_;
      CPPAD_ASSERT_UNKNOWN( row_.size() == nnz_ );
      CPPAD_ASSERT_UNKNOWN( col_.size() == nnz_ );
      //
      row_major_.resize(0);
      col_major_.resize(0);
   }
   //
   // number of rows in matrix
   size_t nr(void) const
   {  return nr_; }
   //
   // number of columns in matrix
   size_t nc(void) const
   {  return nc_; }
   //
   // number of possibly non-zero elements in matrix
   size_t nnz(void) const
   {  return nnz_; }
   //
   // row indices
   const SizeVector& row(void) const
   {  return row_; }
   //
   // column indices
   const SizeVector& col(void) const
   {  return col_; }
   //
   // row-major order
   SizeVector row_major(void) const
   {  if( row_major_.size() > 0 )
         return row_major_;
      //
      SizeVector keys(nnz_), row_major(nnz_);
      for(size_t k = 0; k < nnz_; k++)
      {  CPPAD_ASSERT_UNKNOWN( row_[k] < nr_ );
         keys[k] = row_[k] * nc_ + col_[k];
      }
      index_sort(keys, row_major);
# ifndef NDEBUG
      for(size_t ell = 0; ell + 1 < nnz_; ell++)
      {  size_t k  = row_major[ ell ];
         size_t kp = row_major[ ell + 1 ];
         CPPAD_ASSERT_KNOWN(
            row_[k] != row_[kp] || col_[k] != col_[kp],
            "sparse_rc: row_major: duplicate entry in this pattern"
         );
         CPPAD_ASSERT_UNKNOWN(
            row_[k]<row_[kp] || (row_[k]==row_[kp] && col_[k]<col_[kp])
         );
      }
# endif
      return row_major;
   }
   //
   // column-major indices
   SizeVector col_major(void) const
   {  if( col_major_.size() > 0 )
         return col_major_;
      SizeVector keys(nnz_), col_major(nnz_);
      for(size_t k = 0; k < nnz_; k++)
      {  CPPAD_ASSERT_UNKNOWN( col_[k] < nc_ );
         keys[k] = col_[k] * nr_ + row_[k];
      }
      index_sort(keys, col_major);
# ifndef NDEBUG
      for(size_t ell = 0; ell + 1 < nnz_; ell++)
      {  size_t k  = col_major[ ell ];
         size_t kp = col_major[ ell + 1 ];
         CPPAD_ASSERT_KNOWN(
            col_[k] != col_[kp] || row_[k] != row_[kp],
            "sparse_rc: col_major: duplicate entry in this pattern"
         );
         CPPAD_ASSERT_UNKNOWN(
            col_[k]<col_[kp] || (col_[k]==col_[kp] && row_[k]<row_[kp])
         );
      }
# endif
      return col_major;
   }
   //
   void set_row_major(void)
   {  row_major_ = row_major();
   }
   const SizeVector& get_row_major(void) const
   {  return row_major_;
   }
   //
   void set_col_major(void)
   {  col_major_ = col_major();
   }
   const SizeVector& get_col_major(void) const
   {  return col_major_;
   }
};
//
// output
template <class SizeVector>
std::ostream& operator << (
   std::ostream&                       os      ,
   const CppAD::sparse_rc<SizeVector>& pattern )
{  size_t nnz = pattern.nnz();
   if( nnz == 0 )
   {  os << "{ }";
      return os;
   }
   const SizeVector& row       = pattern.row();
   const SizeVector& col       = pattern.col();
   SizeVector        row_major = pattern.row_major();
   //
   // k, r, c
   size_t k = 0;
   size_t r = row[ row_major[k] ];
   size_t c = col[ row_major[k] ];
   //
   // os
   os << "{\nrow = " << r << ", col = " << c;
   while(++k < nnz )
   {  bool new_row = r != row[ row_major[k] ];
      r = row[ row_major[k] ];
      c = col[ row_major[k] ];
      if( new_row )
         os << "\nrow = " << r << ", col = " << c;
      else
         os << ", " << c;
   }
   os << "\n}";
   //
   return os;
}
} // END_CPPAD_NAMESPACE

# endif
