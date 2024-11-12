# ifndef CPPAD_CORE_SUBGRAPH_SPARSITY_HPP
# define CPPAD_CORE_SUBGRAPH_SPARSITY_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin subgraph_sparsity}
{xrst_spell
   subgraphs
}

Subgraph Dependency Sparsity Patterns
#####################################

Syntax
******
| *f* . ``subgraph_sparsity`` (
| |tab| *select_domain* , *select_range* , *transpose* , *pattern_out*
| )

See Also
********
:ref:`subgraph_reverse@clear_subgraph` .

Notation
********
We use :math:`F : \B{R}^n \rightarrow \B{R}^m` to denote the
:ref:`glossary@AD Function` corresponding to
the operation sequence stored in *f* .

Method
******
This routine uses a subgraph technique. To be specific,
for each dependent variable,
it creates a subgraph of the operation sequence
containing the variables that affect the dependent variable.
This avoids the overhead of performing set operations
that is inherent in other methods for computing sparsity patterns.

Atomic Function
***************
The sparsity calculation for
:ref:`atomic functions<atomic_two_afun-name>` in the *f* operation sequence
are not efficient. To be specific, each atomic function is treated as if
all of its outputs depend on all of its inputs.
This may be improved upon in the future; see the
:ref:`subgraph sparsity<wish_list@Subgraph@Sparsity>`
wish list item.

BoolVector
**********
The type *BoolVector* is a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
``bool`` .

SizeVector
**********
The type *SizeVector* is a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
``size_t`` .

f
*
The object *f* has prototype

   ``ADFun`` < *Base* > *f*

select_domain
*************
The argument *select_domain* has prototype

   ``const`` *BoolVector* & *select_domain*

It has size :math:`n` and specifies which independent variables
to include in the calculation.
If not all the independent variables are included in the calculation,
a forward pass on the operation sequence is used to determine which
nodes may be in the subgraphs.

select_range
************
The argument *select_range* has prototype

   ``const`` *BoolVector* & *select_range*

It has size :math:`m` and specifies which components of the range
to include in the calculation.
A subgraph is built for each dependent variable
and the selected set of independent variables.

transpose
*********
This argument has prototype

   ``bool`` *transpose*

If *transpose* it is false (true),
upon return *pattern_out* is a sparsity pattern for
:math:`J(x)` (:math:`J(x)^\R{T}`) defined below.

pattern_out
***********
This argument has prototype

   ``sparse_rc`` < *SizeVector* >& *pattern_out*

This input value of *pattern_out* does not matter.
Upon return *pattern_out* is a
:ref:`dependency.cpp@Dependency Pattern`
for :math:`F(x)`.
The pattern has :math:`m` rows, :math:`n` columns.
If *select_domain* [ *j* ] is true,
*select_range* [ *i* ] is true, and
:math:`F_i (x)` depends on :math:`x_j`,
then the pair :math:`(i, j)` is in *pattern_out* .
Not that this is also a sparsity pattern for the Jacobian

.. math::

   J(x) = R F^{(1)} (x) D

where :math:`D` (:math:`R`) is the diagonal matrix corresponding
to *select_domain* ( *select_range* ).

Example
*******
{xrst_toc_hidden
   example/sparse/subgraph_sparsity.cpp
}
The file
:ref:`subgraph_sparsity.cpp-name`
contains an example and test of this operation.

{xrst_end subgraph_sparsity}
-----------------------------------------------------------------------------
*/
# include <cppad/core/ad_fun.hpp>
# include <cppad/local/subgraph/sparsity.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

/*!
Subgraph sparsity patterns.

\tparam Base
is the base type for this recording.

\tparam SizeVector
is the simple vector with elements of type size_t that is used for
row, column index sparsity patterns.

\param select_domain
sparsity pattern for the diagonal of the square matrix D.

\param select_range
sparsity pattern for the diagnoal of the square matrix R

\param transpose
If true, the return is a dependency sparsity pattern for
\f$ D F^{(1)} (x)^T R \f$

\param pattern_out
The input value does not matter.
The return value is a dependency sparsity pattern for  \f$ R F^{(1)} (x) D \f$
where F is the function corresponding to the operation sequence
and x is any argument value.
is the sparsity pattern transposed.
*/
template <class Base, class RecBase>
template <class BoolVector, class SizeVector>
void ADFun<Base,RecBase>::subgraph_sparsity(
   const BoolVector&            select_domain    ,
   const BoolVector&            select_range     ,
   bool                         transpose        ,
   sparse_rc<SizeVector>&       pattern_out      )
{
   // compute the sparsity pattern in row, col
   local::pod_vector<size_t> row;
   local::pod_vector<size_t> col;

   // create the optimized recording
   switch( play_.address_type() )
   {
      case local::play::unsigned_short_enum:
      local::subgraph::subgraph_sparsity<unsigned short>(
         &play_,
         subgraph_info_,
         dep_taddr_,
         select_domain,
         select_range,
         row,
         col
      );
      break;

      case local::play::unsigned_int_enum:
      local::subgraph::subgraph_sparsity<unsigned int>(
         &play_,
         subgraph_info_,
         dep_taddr_,
         select_domain,
         select_range,
         row,
         col
      );
      break;

      case local::play::size_t_enum:
      local::subgraph::subgraph_sparsity<size_t>(
         &play_,
         subgraph_info_,
         dep_taddr_,
         select_domain,
         select_range,
         row,
         col
      );
      break;

      default:
      CPPAD_ASSERT_UNKNOWN(false);
   }

   CPPAD_ASSERT_UNKNOWN( row.size() == col.size() );

   // return the sparsity pattern
   size_t nr  = dep_taddr_.size();
   size_t nc  = ind_taddr_.size();
   size_t nnz = row.size();
   if( transpose )
   {  pattern_out.resize(nc, nr, nnz);
      for(size_t k = 0; k < nnz; k++)
         pattern_out.set(k, col[k], row[k]);
   }
   else
   {  pattern_out.resize(nr, nc, nnz);
      for(size_t k = 0; k < nnz; k++)
         pattern_out.set(k, row[k], col[k]);
   }
   return;
}
} // END_CPPAD_NAMESPACE
# endif
