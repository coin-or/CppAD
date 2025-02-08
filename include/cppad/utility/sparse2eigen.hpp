# ifndef CPPAD_UTILITY_SPARSE2EIGEN_HPP
# define CPPAD_UTILITY_SPARSE2EIGEN_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin sparse2eigen}
{xrst_spell
   nnz
}

Convert A CppAD Sparse Matrix to an Eigen Sparse Matrix
#######################################################

Syntax
******
| # ``include <cppad/utility/sparse2eigen.hpp>``
| ``sparse2eigen`` ( *source* , *destination* )

Prototype
*********
{xrst_literal
   // BEGIN_PROTOTYPE
   // END_PROTOTYPE
}

Include
*******
If :ref:`cmake@Eigen` is found (and c++14 is supported),
the file ``cppad/utility/sparse2eigen.hpp``
is included by ``cppad/cppad.hpp`` .
In any case,
it can also be included separately with out the rest of
the ``CppAD`` routines.
Including this file defines
this version of the ``sparse2eigen`` within the ``CppAD`` namespace.

SizeVector
**********
We use :ref:`sparse_rc@SizeVector` to denote a
:ref:`SimpleVector-name` class with elements of ``size_t`` .

ValueVector
***********
We use *ValueVector* to denote a
:ref:`SimpleVector-name` class with elements of type *value_type* .

Options
*******
We use *Options* to denote either
``Eigen::RowMajor`` of ``Eigen::ColMajor`` .

value_type
**********
The type of elements of elements in *source* and *destination*
must be the same. We use *value_type* to denote this type.

source
******
This is the CppAD sparse matrix that is being converted to eigen format.

destination
***********
This is the Eigen sparse matrix that is the result of the conversion.

Compressed
**********
The result matrix *destination*
is in compressed format. For example, let

| |tab| ``size_t`` *nnz* = *source* . ``nnz`` ();
| |tab| ``const`` *s_vector* & *s_value* = *source* . ``val`` ();
| |tab| ``const`` *value_type* * *d_value* = *destination* . ``valuePtr`` ();
| |tab| ``const`` *s_vector* & *row_major* = *source* . ``row_major`` ();
| |tab| ``const`` *s_vector* & *col_major* = *source* . ``col_major`` ();

It follows that, for *k* = 0 , ..., *nnz* :
If *Options* is ``Eigen::RowMajor`` ,

   *d_value* [ *k* ] == *s_value* [ *row_major* [ *k* ] ]

If *Options* is ``Eigen::ColMajor`` ,

   *d_value* [ *k* ] == *s_value* [ *col_major* [ *k* ] ]

{xrst_toc_hidden
   example/sparse/sparse2eigen.cpp
}

Example
*******
The file :ref:`sparse2eigen.cpp-name` contains an example and test
of ``sparse2eigen.cpp`` It return true if the test passes
and false otherwise.

{xrst_end sparse2eigen}
*/
# include <cppad/configure.hpp>
# include <Eigen/Sparse>
# include <cppad/utility/sparse_rcv.hpp>
# include <cppad/utility/vector.hpp>

namespace CppAD { // BEGIN CPPAD_NAMESPACE

// BEGIN_PROTOTYPE
template <class SizeVector, class ValueVector, int Options>
void sparse2eigen(
const CppAD::sparse_rcv<SizeVector, ValueVector>&               source       ,
Eigen::SparseMatrix<typename ValueVector::value_type, Options>& destination  )
// END_PROTOTYPE
{  using Eigen::Index;
   typedef typename ValueVector::value_type value_type;
   typedef Eigen::Triplet<value_type>       triplet;
   std::vector<triplet> vec( source.nnz() );
   //
   const SizeVector&  row = source.row();
   const SizeVector&  col = source.col();
   const ValueVector& val = source.val();
   //
   for(size_t k = 0; k < source.nnz(); k++)
      vec[k] = triplet( int(row[k]), int(col[k]), val[k] );
   //
   size_t nr = source.nr();
   size_t nc = source.nc();
   destination.resize( Index(nr), Index(nc) );
   destination.setFromTriplets(vec.begin(), vec.end());
   //
   CPPAD_ASSERT_UNKNOWN( destination.isCompressed() );
   //
   return;
}

} // END_CPPAD_NAMESPACE
# endif
