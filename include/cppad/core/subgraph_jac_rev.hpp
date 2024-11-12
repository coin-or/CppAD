# ifndef CPPAD_CORE_SUBGRAPH_JAC_REV_HPP
# define CPPAD_CORE_SUBGRAPH_JAC_REV_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin subgraph_jac_rev}
{xrst_spell
   nnz
   nr
   subgraphs
}

Compute Sparse Jacobians Using Subgraphs
########################################

Syntax
******
| *f* . ``subgraph_jac_rev`` ( *x* , *subset* )
| *f* . ``subgraph_jac_rev`` (
| |tab| *select_domain* , *select_range* , *x* , *matrix_out*
| )

See Also
********
:ref:`subgraph_reverse@clear_subgraph` .

Purpose
*******
We use :math:`F : \B{R}^n \rightarrow \B{R}^m` to denote the
function corresponding to *f* .
Here *n* is the :ref:`fun_property@Domain` size,
and *m* is the :ref:`fun_property@Range` size, or *f* .
The syntax above takes advantage of sparsity when computing the Jacobian

.. math::

   J(x) = F^{(1)} (x)

The  first syntax requires one to know what which elements of the Jacobian
they want to compute.
The second syntax computes the sparsity pattern and the value
of the Jacobian at the same time.
If one only wants the sparsity pattern,
it should be faster to use :ref:`subgraph_sparsity-name` .

Method
******
This routine uses a subgraph technique. To be specific,
for each dependent variable,
it creates a subgraph of the operation sequence
containing the variables that affect the dependent variable.
This avoids the overhead of performing set operations
that is inherent in other methods for computing sparsity patterns.

BaseVector
**********
The type *BaseVector* is a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
*Base* .

SizeVector
**********
The type *SizeVector* is a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
``size_t`` .

BoolVector
**********
The type *BoolVector* is a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
``bool`` .

f
*
This object has prototype

   ``ADFun`` < *Base* > *f*

Note that the Taylor coefficients stored in *f* are affected
by this operation; see
:ref:`sparse_jac@Uses Forward` below.

x
*
This argument has prototype

   ``const`` *BaseVector* & *x*

It is the value of *x* at which we are computing the Jacobian.

Uses Forward
************
After each call to :ref:`Forward-name` ,
the object *f* contains the corresponding
:ref:`Taylor coefficients<glossary@Taylor Coefficient>` .
After a call to ``sparse_jac_forward`` or ``sparse_jac_rev`` ,
the zero order coefficients correspond to

   *f* . ``Forward`` (0, *x* )

All the other forward mode coefficients are unspecified.

subset
******
This argument has prototype

   ``sparse_rcv`` < *SizeVector* , *BaseVector* >& *subset*

Its row size is *subset* . ``nr`` () == *m* ,
and its column size is *subset* . ``nc`` () == *n* .
It specifies which elements of the Jacobian are computed.
The input elements in its value vector
*subset* . ``val`` () do not matter.
Upon return it contains the value of the corresponding elements
of the Jacobian.

select_domain
*************
The argument *select_domain* has prototype

   ``const`` *BoolVector* & *select_domain*

It has size :math:`n` and specifies which independent variables
to include.

select_range
************
The argument *select_range* has prototype

   ``const`` *BoolVector* & *select_range*

It has size :math:`m` and specifies which components of the range
to include in the calculation.
A subgraph is built for each dependent variable and the selected set
of independent variables.

matrix_out
**********
This argument has prototype

   ``sparse_rcv`` < *SizeVector* , *BaseVector* >& *matrix_out*

This input value of *matrix_out* does not matter.
Upon return *matrix_out* is
:ref:`sparse matrix<sparse_rcv-name>` representation of :math:`F^{(1)} (x)`.
The matrix has :math:`m` rows, :math:`n` columns.
If *select_domain* [ *j* ] is true,
*select_range* [ *i* ] is true, and
:math:`F_i (x)` depends on :math:`x_j`,
then the pair :math:`(i, j)` is in *matrix_out* .
For each *k* = 0 , ..., *matrix_out* . ``nnz`` () , let

| |tab| *i* = *matrix_out* . ``row`` ()[ *k* ]
| |tab| *j* = *matrix_out* . ``col`` ()[ *k* ]
| |tab| *v* = *matrix_out* . ``val`` ()[ *k* ]

It follows that the partial of :math:`F_i (x)` with respect to
:math:`x_j` is equal to :math:`v`.

Example
*******
{xrst_toc_hidden
   example/sparse/subgraph_jac_rev.cpp
   example/sparse/subgraph_hes2jac.cpp
}
The files :ref:`subgraph_jac_rev.cpp-name` and :ref:`subgraph_hes2jac.cpp-name`
are examples and tests using ``subgraph_jac_rev`` .
They returns ``true`` for success and ``false`` for failure.

{xrst_end subgraph_jac_rev}
-----------------------------------------------------------------------------
*/
# include <cppad/core/ad_fun.hpp>
# include <cppad/local/subgraph/info.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

/*!
Subgraph sparsity patterns.

\tparam Base
is the base type for this recording.

\tparam SizeVector
is the simple vector with elements of type size_t that is used for
row, column index sparsity patterns.

\tparam BaseVector
a simple vector class with elements of type Base.

\param x
a vector of length n, the number of independent variables in f
(this ADFun object).

\param subset
specifices the subset of the sparsity pattern where the Jacobian is evaluated.
subset.nr() == m,
subset.nc() == n.
*/
template <class Base, class RecBase>
template <class SizeVector, class BaseVector>
void ADFun<Base,RecBase>::subgraph_jac_rev(
   const BaseVector&                   x      ,
   sparse_rcv<SizeVector, BaseVector>& subset )
{  size_t m = Range();
   size_t n = Domain();
   //
   CPPAD_ASSERT_KNOWN(
      subset.nr() == m,
      "subgraph_jac_rev: subset.nr() not equal range dimension for f"
   );
   CPPAD_ASSERT_KNOWN(
      subset.nc() == n,
      "subgraph_jac_rev: subset.nc() not equal domain dimension for f"
   );
   //
   // point at which we are evaluating Jacobian
   Forward(0, x);
   //
   // nnz and row, column, and row_major vectors for subset
   size_t nnz = subset.nnz();
   const SizeVector& row( subset.row() );
   const SizeVector& col( subset.col() );
   SizeVector row_major = subset.row_major();
   //
   // determine set of independent variabels
   local::pod_vector<bool> select_domain(n);
   for(size_t j = 0; j < n; j++)
      select_domain[j] = false;
   for(size_t k = 0; k < nnz; k++)
      select_domain[ col[k] ] = true;
   //
   // initialize reverse mode computation on subgraphs
   subgraph_reverse(select_domain);
   //
   // memory used to hold subgraph_reverse results
   BaseVector dw;
   SizeVector dw_col;
   //
   // initialize index in row_major
   size_t k = 0;
   Base zero(0);
   while(k < nnz )
   {  size_t q   = 1;
      size_t i_dep = row[ row_major[k] ];
      size_t i_ind = col[ row_major[k] ];
      size_t ell   = i_dep;
      subgraph_reverse(q, ell, dw_col, dw);
      //
      size_t c = 0;
      while( i_dep == ell )
      {  // row numbers match
         //
         // advance c to possible match with column i_ind
         while( c < size_t( dw_col.size() ) && dw_col[c] < i_ind )
            ++c;
         //
         // check for match with i_ind
         if( i_ind == dw_col[c] )
            subset.set( row_major[k], dw[i_ind] );
         else
            subset.set( row_major[k], zero);
         //
         // advance to next (i_dep, i_ind)
         ++k;
         if( k == nnz )
         {  i_dep = m;
            i_ind = n;
         }
         else
         {  i_dep = row[ row_major[k] ];
            i_ind = col[ row_major[k] ];
         }
      }
   }
   return;
}
template <class Base, class RecBase>
template <class BoolVector, class SizeVector, class BaseVector>
void ADFun<Base,RecBase>::subgraph_jac_rev(
   const BoolVector&                   select_domain  ,
   const BoolVector&                   select_range   ,
   const BaseVector&                   x              ,
   sparse_rcv<SizeVector, BaseVector>& matrix_out     )
{  size_t m = Range();
   size_t n = Domain();
   //
   // point at which we are evaluating Jacobian
   Forward(0, x);
   //
   // nnz and row, column, and row_major vectors for subset
   local::pod_vector<size_t> row_out;
   local::pod_vector<size_t> col_out;
   local::pod_vector_maybe<Base>   val_out;
   //
   // initialize reverse mode computation on subgraphs
   subgraph_reverse(select_domain);
   //
   // memory used to hold subgraph_reverse results
   BaseVector dw;
   SizeVector col;
   //
   // loop through selected independent variables
   for(size_t i = 0; i < m; ++i) if( select_range[i] )
   {  // compute Jacobian and sparsity for this dependent variable
      size_t q   = 1;
      subgraph_reverse(q, i, col, dw);
      CPPAD_ASSERT_UNKNOWN( size_t( dw.size() ) == n );
      //
      // offset for this dependent variable
      size_t index = row_out.size();
      CPPAD_ASSERT_UNKNOWN( col_out.size() == index );
      CPPAD_ASSERT_UNKNOWN( val_out.size() == index );
      //
      // extend vectors to hold results for this dependent variable
      size_t col_size = size_t( col.size() );
      row_out.extend( col_size );
      col_out.extend( col_size );
      val_out.extend( col_size );
      //
      // store results for this dependent variable
      for(size_t c = 0; c < col_size; ++c)
      {  row_out[index + c] = i;
         col_out[index + c] = col[c];
         val_out[index + c] = dw[ col[c] ];
      }
   }
   //
   // create sparsity pattern corresponding to row_out, col_out
   size_t nr  = m;
   size_t nc  = n;
   size_t nnz = row_out.size();
   sparse_rc<SizeVector> pattern(nr, nc, nnz);
   for(size_t k = 0; k < nnz; ++k)
      pattern.set(k, row_out[k], col_out[k]);
   //
   // create sparse matrix
   sparse_rcv<SizeVector, BaseVector> matrix(pattern);
   for(size_t k = 0; k < nnz; ++k)
      matrix.set(k,  val_out[k]);
   //
   // return matrix
   matrix_out = matrix;
   //
   return;
}
} // END_CPPAD_NAMESPACE
# endif
