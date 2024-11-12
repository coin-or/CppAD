# ifndef CPPAD_UTILITY_SPARSE_RCV_HPP
# define CPPAD_UTILITY_SPARSE_RCV_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
------------------------------------------------------------------------------
{xrst_begin sparse_rcv}
{xrst_spell
   nnz
   nr
}
Sparse Matrix Row, Column, Value Representation
###############################################

Syntax
******
| # ``include <cppad/utility/sparse_rcv.hpp>``
| ``sparse_rcv`` < *SizeVector* , *ValueVector* > *empty*
| ``sparse_rcv`` < *SizeVector* , *ValueVector* > *matrix* ( *pattern* )
| *matrix* = *other*
| *matrix* . ``swap`` ( *other*  )
| *matrix* . ``set`` ( *k* , *v* )
| *nr* = *matrix* . ``nr`` ()
| *nc* = *matrix* . ``nc`` ()
| *nnz* = *matrix* . ``nnz`` ()
| ``const`` *SizeVector* & *row* ( *matrix* . ``row`` () )
| ``const`` *SizeVector* & *col* ( *matrix* . ``col`` () )
| ``const`` *ValueVector* & *val* ( *matrix* . ``val`` () )
| ``const sparse_rc`` < *SizeVector* >& *pat* ( *matrix* . ``pat`` () )
| *row_major* = *matrix* . ``row_major`` ()
| *col_major* = *matrix* . ``col_major`` ()

SizeVector
**********
We use :ref:`sparse_rc@SizeVector` to denote the
:ref:`SimpleVector-name` class corresponding to *pattern* .

ValueVector
***********
We use *ValueVector* to denote the
:ref:`SimpleVector-name` class corresponding to *val* .

empty
*****
This is an empty sparse matrix object. To be specific,
the corresponding number of rows *nr* ,
number of columns *nc* ,
and number of possibly non-zero values *nnz* ,
are all zero.

pattern
*******
This constructor argument has prototype

   ``const sparse_rc`` < *SizeVector* >& *pattern*

It specifies the number of rows, number of columns and
the possibly non-zero entries in the *matrix* .

matrix
******
This is a sparse matrix object with the sparsity specified by *pattern* .
Only the *val* vector can be changed. All other values returned by
*matrix* are fixed during the constructor and constant there after.
The *val* vector is only changed by the constructor
and the ``set`` function.
There are two exceptions to this rule, where *other* appears in the
assignment and swap syntax.

other
*****

Assignment and Constructor
==========================
In the assignment and constructor, *other* has prototype

   ``const sparse_rcv`` < *SizeVector* , *ValueVector* >& *other*

After this assignment and constructor, *other* is an independent copy
of *matrix* ; i.e. it has all the same values as *matrix*
and changes to *matrix* do not affect *other* .

Move Semantics Assignment and Constructor
=========================================
In the assignment and constructor, if *other* has prototype

   ``sparse_rcv`` < *SizeVector* , *ValueVector* >&& *other*

A move semantics version of the assignment operator is used; e.g.,
when *other* is a function return value;

swap
====
After the swap operation *other* ( *matrix* ) is equivalent
to *matrix* ( *other* ) before the operation.

nr
**
This return value has prototype

   ``size_t`` *nr*

and is the number of rows in *matrix* .

nc
**
This argument and return value has prototype

   ``size_t`` *nc*

and is the number of columns in *matrix* .

nnz
***
We use the notation *nnz* to denote the number of
possibly non-zero entries in *matrix* .

set
***
This function sets the value

   *val* [ *k* ] = *v*

k
=
This argument has type

   ``size_t`` *k*

and must be less than *nnz* .

v
=
This argument has type

   ``const`` *ValueVector* :: ``value_type&`` *v*

It specifies the value assigned to *val* [ *k* ] .

row
***
This vector has size *nnz* and
*row* [ *k* ]
is the row index of the *k*-th possibly non-zero
element in *matrix* .

col
***
This vector has size *nnz* and
*col* [ ``k`` ] is the column index of the *k*-th possibly non-zero
element in *matrix*

val
***
This vector has size *nnz* and
*val* [ ``k`` ] is value of the *k*-th possibly non-zero entry
in the sparse matrix (the value may be zero).

pat
***
This is equal to the sparsity pattern; i.e.,
*pattern* in the constructor.

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

Eigen Matrix
************
If you have the :ref:`eigen package<eigen-name>` in your include path,
you can use :ref:`sparse2eigen-name` to convert a sparse matrix to eigen format.
{xrst_toc_hidden
   example/utility/sparse_rcv.cpp
}
Example
*******
The file :ref:`sparse_rcv.cpp-name`
contains an example and test of this class.

{xrst_end sparse_rcv}
*/
/*!
\file sparse_rcv.hpp
A sparse matrix class.
*/
# include <cppad/utility/sparse_rc.hpp>

namespace CppAD { // BEGIN CPPAD_NAMESPACE

/// Sparse matrices with elements of type Scalar
template <class SizeVector, class ValueVector>
class sparse_rcv {
private:
   /// sparsity pattern
   sparse_rc<SizeVector> pattern_;
   /// value_type
   typedef typename ValueVector::value_type value_type;
   /// val_[k] is the value for the k-th possibly non-zero entry in the matrix
   ValueVector    val_;
public:
   // ------------------------------------------------------------------------
   /// default constructor
   sparse_rcv(void)
   : pattern_(0, 0, 0)
   { }
   /// copy constructor
   sparse_rcv(const sparse_rcv& other)
   :
   pattern_( other.pat() ) ,
   val_( other.val() )
   { }
   /// move semantics constructor
   /// (none of the default constructor values are used by destructor)
   sparse_rcv(sparse_rcv&& other)
   {  swap(other); }
   /// destructor
   ~sparse_rcv(void)
   { }
   /// constructor
   sparse_rcv(const sparse_rc<SizeVector>& pattern )
   :
   pattern_(pattern)    ,
   val_(pattern_.nnz())
   { }
   /// assignment
   void operator=(const sparse_rcv& other)
   {  pattern_ = other.pattern_;
      // simple vector assignment requires vectors to have same size
      val_.resize( other.nnz() );
      val_ = other.val();
   }
   /// swap
   void swap(sparse_rcv& other)
   {  pattern_.swap( other.pattern_ );
      val_.swap( other.val_ );
   }
   /// move semantics assignment
   void operator=(sparse_rcv&& other)
   {  swap(other); }
   // ------------------------------------------------------------------------
   void set(size_t k, const value_type& v)
   {  CPPAD_ASSERT_KNOWN(
         pattern_.nnz(),
         "The index k is not less than nnz in sparse_rcv::set"
      );
      val_[k] = v;
   }
   /// number of rows in matrix
   size_t nr(void) const
   {  return pattern_.nr(); }
   /// number of columns in matrix
   size_t nc(void) const
   {  return pattern_.nc(); }
   /// number of possibly non-zero elements in matrix
   size_t nnz(void) const
   {  return pattern_.nnz(); }
   /// row indices
   const SizeVector& row(void) const
   {  return pattern_.row(); }
   /// column indices
   const SizeVector& col(void) const
   {  return pattern_.col(); }
   /// value for possibly non-zero elements
   const ValueVector& val(void) const
   {  return val_; }
   /// sparsity pattern
   const sparse_rc<SizeVector>& pat(void) const
   {  return pattern_; }
   /// row-major order
   SizeVector row_major(void) const
   {  return pattern_.row_major(); }
   /// column-major indices
   SizeVector col_major(void) const
   {  return pattern_.col_major(); }
};

} // END_CPPAD_NAMESPACE

# endif
