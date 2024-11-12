# ifndef CPPAD_CORE_SPARSE_HES_HPP
# define CPPAD_CORE_SPARSE_HES_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin sparse_hes}
{xrst_spell
   nr
}

Computing Sparse Hessians
#########################

Syntax
******
| *n_sweep* = *f* . ``sparse_hes`` (
| |tab| *x* , *w* , *subset* , *pattern* , *coloring* , *work*
| )

Purpose
*******
We use :math:`F : \B{R}^n \rightarrow \B{R}^m` to denote the
function corresponding to *f* .
Here *n* is the :ref:`fun_property@Domain` size,
and *m* is the :ref:`fun_property@Range` size, or *f* .
The syntax above takes advantage of sparsity when computing the Hessian

.. math::

   H(x) = \dpow{2}{x} \sum_{i=0}^{m-1} w_i F_i (x)

In the sparse case, this should be faster and take less memory than
:ref:`Hessian-name` .
The matrix element :math:`H_{i,j} (x)` is the second partial of
:math:`w^\R{T} F (x)` with respect to :math:`x_i` and :math:`x_j`.

SizeVector
**********
The type *SizeVector* is a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
``size_t`` .

BaseVector
**********
The type *BaseVector* is a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
``size_t`` .

f
*
This object has prototype

   ``ADFun`` < *Base* > *f*

Note that the Taylor coefficients stored in *f* are affected
by this operation; see
:ref:`sparse_hes@Uses Forward` below.

x
*
This argument has prototype

   ``const`` *BaseVector* & *x*

and its size is *n* .
It specifies the point at which to evaluate the Hessian
:math:`H(x)`.

w
*
This argument has prototype

   ``const`` *BaseVector* & *w*

and its size is *m* .
It specifies the weight for each of the components of :math:`F(x)`;
i.e. :math:`w_i` is the weight for :math:`F_i (x)`.

subset
******
This argument has prototype

   ``sparse_rcv`` < *SizeVector* , *BaseVector* >& *subset*

Its row size and column size is *n* ; i.e.,
*subset* . ``nr`` () == *n* and *subset* . ``nc`` () == *n* .
It specifies which elements of the Hessian are computed.

#. The input value of its value vector
   *subset* . ``val`` () does not matter.
   Upon return it contains the value of the corresponding elements
   of the Hessian.
#. All of the row, column pairs in *subset* must also appear in
   *pattern* ; i.e., they must be possibly non-zero.
#. The Hessian is symmetric, so one has a choice as to which off diagonal
   elements to put in *subset* .
   It will probably be more efficient if one makes this choice so that
   the there are more entries in each non-zero column of *subset* ;
   see :ref:`sparse_hes@n_sweep` below.

pattern
*******
This argument has prototype

   ``const sparse_rc`` < *SizeVector* >& *pattern*

Its row size and column size is *n* ; i.e.,
*pattern* . ``nr`` () == *n* and *pattern* . ``nc`` () == *n* .
It is a sparsity pattern for the Hessian :math:`H(x)`.
If the *i*-th row (*j*-th column) does not appear in *subset* ,
the *i*-th row (*j*-th column) of *pattern* does not matter
and need not be computed.
This argument is not used (and need not satisfy any conditions),
when :ref:`sparse_hes@work` is non-empty.

subset
======
If the *i*-th row and *i*-th column do not appear in *subset* ,
the *i*-th row and column of *pattern* do not matter.
In this case the *i-th*-th row and column may have no entries in
*pattern* even though they are possibly non-zero in :math:`H(x)`.
(This can be used to reduce the amount of computation required to find
*pattern* .)

coloring
********
The coloring algorithm determines which rows and columns
can be computed during the same sweep.
This field has prototype

   ``const std::string&`` *coloring*

This value only matters when work is empty; i.e.,
after the *work* constructor or *work* . ``clear`` () .

cppad.symmetric
===============
This coloring takes advantage of the fact that the Hessian matrix
is symmetric when find a coloring that requires fewer
:ref:`sweeps<sparse_hes@n_sweep>` .

cppad.general
=============
This is the same as the sparse Jacobian
:ref:`sparse_jac@coloring@cppad` method
which does not take advantage of symmetry.

colpack.symmetric
=================
If :ref:`colpack_prefix-name` was specified on the
:ref:`cmake@CMake Command` line,
you can set *coloring* to ``colpack.symmetric`` .
This also takes advantage of the fact that the Hessian matrix is symmetric.

colpack.general
===============
If :ref:`colpack_prefix-name` was specified on the
:ref:`cmake@CMake Command` line,
you can set *coloring* to ``colpack.general`` .
This is the same as the sparse Jacobian
:ref:`sparse_jac@coloring@colpack` method
which does not take advantage of symmetry.

colpack.star Deprecated 2017-06-01
==================================
The ``colpack.star`` method is deprecated.
It is the same as the ``colpack.symmetric`` method
which should be used instead.

work
****
This argument has prototype

   ``sparse_hes_work&`` *work*

We refer to its initial value,
and its value after *work* . ``clear`` () , as empty.
If it is empty, information is stored in *work* .
This can be used to reduce computation when
a future call is for the same object *f* ,
and the same subset of the Hessian.
In fact, it can be used with a different *f*
and a different *subset* provided that Hessian sparsity pattern
for *f* and the sparsity pattern in *subset* are the same.
If either of these values change, use *work* . ``clear`` () to
empty this structure.

n_sweep
*******
The return value *n_sweep* has prototype

   ``size_t`` *n_sweep*

It is the number of first order forward sweeps
used to compute the requested Hessian values.
Each first forward sweep is followed by a second order reverse sweep
so it is also the number of reverse sweeps.
It is also the number of colors determined by the coloring method
mentioned above.
This is proportional to the total computational work,
not counting the zero order forward sweep,
or combining multiple columns and rows into a single sweep.

Uses Forward
************
After each call to :ref:`Forward-name` ,
the object *f* contains the corresponding
:ref:`Taylor coefficients<glossary@Taylor Coefficient>` .
After a call to ``sparse_hes``
the zero order coefficients correspond to

   *f* . ``Forward`` (0, *x* )

All the other forward mode coefficients are unspecified.

Example
*******
{xrst_toc_hidden
   example/sparse/sparse_hes.cpp
}
The files :ref:`sparse_hes.cpp-name`
is an example and test of ``sparse_hes`` .
It returns ``true`` , if it succeeds, and ``false`` otherwise.

Subset Hessian
**************
The routine
:ref:`sparse_sub_hes.cpp-name`
is an example and test that compute a subset of a sparse Hessian.
It returns ``true`` , for success, and ``false`` otherwise.

{xrst_end sparse_hes}
*/
# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/sparse/internal.hpp>
# include <cppad/local/color_general.hpp>
# include <cppad/local/color_symmetric.hpp>

/*!
\file sparse_hes.hpp
Sparse Hessian calculation routines.
*/
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

/*!
Class used to hold information used by Sparse Hessian routine in this file,
so it does not need to be recomputed every time.
*/
class sparse_hes_work {
   public:
      /// row and column indicies for return values
      /// (some may be reflected by symmetric coloring algorithms)
      CppAD::vector<size_t> row;
      CppAD::vector<size_t> col;
      /// indices that sort the row and col arrays by color
      CppAD::vector<size_t> order;
      /// results of the coloring algorithm
      CppAD::vector<size_t> color;

      /// constructor
      sparse_hes_work(void)
      { }
      /// inform CppAD that this information needs to be recomputed
      void clear(void)
      {
         row.clear();
         col.clear();
         order.clear();
         color.clear();
      }
};
// ----------------------------------------------------------------------------
/*!
Calculate sparse Hessians using forward mode

\tparam Base
the base type for the recording that is stored in the ADFun object.

\tparam SizeVector
a simple vector class with elements of type size_t.

\tparam BaseVector
a simple vector class with elements of type Base.

\param x
a vector of length n, the number of independent variables in f
(this ADFun object).

\param w
a vector of length m, the number of dependent variables in f
(this ADFun object).

\param subset
specifices the subset of the sparsity pattern where the Hessian is evaluated.
subset.nr() == n,
subset.nc() == n.

\param pattern
is a sparsity pattern for the Hessian of w^T * f;
pattern.nr() == n,
pattern.nc() == n,
where m is number of dependent variables in f.

\param coloring
determines which coloring algorithm is used.
This must be cppad.symmetric, cppad.general, colpack.symmetic,
or colpack.star.

\param work
this structure must be empty, or contain the information stored
by a previous call to sparse_hes.
The previous call must be for the same ADFun object f
and the same subset.

\return
This is the number of first order forward
(and second order reverse) sweeps used to compute thhe Hessian.
*/
template <class Base, class RecBase>
template <class SizeVector, class BaseVector>
size_t ADFun<Base,RecBase>::sparse_hes(
   const BaseVector&                    x        ,
   const BaseVector&                    w        ,
   sparse_rcv<SizeVector , BaseVector>& subset   ,
   const sparse_rc<SizeVector>&         pattern  ,
   const std::string&                   coloring ,
   sparse_hes_work&                     work     )
{  size_t n = Domain();
   //
   CPPAD_ASSERT_KNOWN(
      subset.nr() == n,
      "sparse_hes: subset.nr() not equal domain dimension for f"
   );
   CPPAD_ASSERT_KNOWN(
      subset.nc() == n,
      "sparse_hes: subset.nc() not equal domain dimension for f"
   );
   CPPAD_ASSERT_KNOWN(
      size_t( x.size() ) == n,
      "sparse_hes: x.size() not equal domain dimension for f"
   );
   CPPAD_ASSERT_KNOWN(
      size_t( w.size() ) == Range(),
      "sparse_hes: w.size() not equal range dimension for f"
   );
   //
   // work information
   vector<size_t>& row(work.row);
   vector<size_t>& col(work.col);
   vector<size_t>& color(work.color);
   vector<size_t>& order(work.order);
   //
   // subset information
   const SizeVector& subset_row( subset.row() );
   const SizeVector& subset_col( subset.col() );
   //
   // point at which we are evaluationg the Hessian
   Forward(0, x);
   //
   // number of elements in the subset
   size_t K = subset.nnz();
   //
   // check for case were there is nothing to do
   // (except for call to Forward(0, x)
   if( K == 0 )
      return 0;
   //
# ifndef NDEBUG
   if( color.size() != 0 )
   {  CPPAD_ASSERT_KNOWN(
         color.size() == n,
         "sparse_hes: work is non-empty and conditions have changed"
      );
      CPPAD_ASSERT_KNOWN(
         row.size() == K,
         "sparse_hes: work is non-empty and conditions have changed"
      );
      CPPAD_ASSERT_KNOWN(
         col.size() == K,
         "sparse_hes: work is non-empty and conditions have changed"
      );
      //
      for(size_t k = 0; k < K; k++)
      {  bool ok = row[k] == subset_row[k] && col[k] == subset_col[k];
         ok     |= row[k] == subset_col[k] && col[k] == subset_row[k];
         CPPAD_ASSERT_KNOWN(
            ok,
            "sparse_hes: work is non-empty and conditions have changed"
         );
      }
   }
# endif
   //
   // check for case where input work is empty
   if( color.size() == 0 )
   {  // compute work color and order vectors
      CPPAD_ASSERT_KNOWN(
         pattern.nr() == n,
         "sparse_hes: pattern.nr() not equal domain dimension for f"
      );
      CPPAD_ASSERT_KNOWN(
         pattern.nc() == n,
         "sparse_hes: pattern.nc() not equal domain dimension for f"
      );
      //
      // initialize work row, col to be same as subset row, col
      row.resize(K);
      col.resize(K);
      // cannot assign vectors because may be of different types
      // (SizeVector and CppAD::vector<size_t>)
      for(size_t k = 0; k < K; k++)
      {  row[k] = subset_row[k];
         col[k] = subset_col[k];
      }
      //
      // convert pattern to an internal version of its transpose
      local::pod_vector<size_t> internal_index(n);
      for(size_t j = 0; j < n; j++)
         internal_index[j] = j;
      bool transpose   = true;
      bool zero_empty  = false;
      bool input_empty = true;
      local::sparse::list_setvec internal_pattern;
      internal_pattern.resize(n, n);
      local::sparse::set_internal_pattern(zero_empty, input_empty,
         transpose, internal_index, internal_pattern, pattern
      );
      //
      // execute coloring algorithm
      // (we are using transpose because coloring groups rows, not columns)
      color.resize(n);
      if( coloring == "cppad.general" )
         local::color_general_cppad(internal_pattern, col, row, color);
      else if( coloring == "cppad.symmetric" )
         local::color_symmetric_cppad(internal_pattern, col, row, color);
      else if( coloring == "colpack.general" )
      {
# if CPPAD_HAS_COLPACK
         local::color_general_colpack(internal_pattern, col, row, color);
# else
         CPPAD_ASSERT_KNOWN(
            false,
            "sparse_hes: coloring = colpack.star "
            "and colpack_prefix not in cmake command line."
         );
# endif
      }
      else if(
         coloring == "colpack.symmetric" ||
         coloring == "colpack.star"
      )
      {
# if CPPAD_HAS_COLPACK
         local::color_symmetric_colpack(internal_pattern, col, row, color);
# else
         CPPAD_ASSERT_KNOWN(
            false,
            "sparse_hes: coloring = colpack.symmetic or colpack.star "
            "and colpack_prefix not in cmake command line."
         );
# endif
      }
      else CPPAD_ASSERT_KNOWN(
         false,
         "sparse_hes: coloring is not valid."
      );
      //
      // put sorting indices in color order
      SizeVector key(K);
      order.resize(K);
      for(size_t k = 0; k < K; k++)
         key[k] = color[ col[k] ];
      index_sort(key, order);
   }
   // Base versions of zero and one
   Base one(1.0);
   Base zero(0.0);
   //
   size_t n_color = 1;
   for(size_t j = 0; j < n; j++) if( color[j] < n )
      n_color = std::max<size_t>(n_color, color[j] + 1);
   //
   // initialize the return Hessian values as zero
   for(size_t k = 0; k < K; k++)
      subset.set(k, zero);
   //
   // direction vector for calls to first order forward
   BaseVector dx(n);
   //
   // return values for calls to second order reverse
   BaseVector ddw(2 * n);
   //
   // loop over colors
   size_t k = 0;
   for(size_t ell = 0; ell < n_color; ell++)
   if( k  == K )
   {  // kludge because colpack returns colors that are not used
      // (it does not know about the subset corresponding to row, col)
      CPPAD_ASSERT_UNKNOWN(
         coloring == "colpack.general" ||
         coloring == "colpack.symmetric" ||
         coloring == "colpack.star"
      );
   }
   else if( color[ col[ order[k] ] ] != ell )
   {  // kludge because colpack returns colors that are not used
      // (it does not know about the subset corresponding to row, col)
      CPPAD_ASSERT_UNKNOWN(
         coloring == "colpack.general" ||
         coloring == "colpack.symmetic" ||
         coloring == "colpack.star"
      );
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( color[ col[ order[k] ] ] == ell );
      //
      // combine all columns with this color
      for(size_t j = 0; j < n; j++)
      {  dx[j] = zero;
         if( color[j] == ell )
            dx[j] = one;
      }
      // call forward mode for all these rows at once
      Forward(1, dx);
      //
      // evaluate derivative of w^T * F'(x) * dx
      ddw = Reverse(2, w);
      //
      // set the corresponding components of the result
      while( k < K && color[ col[order[k]] ] == ell )
      {  size_t index = row[ order[k] ] * 2 + 1;
         subset.set(order[k], ddw[index] );
         k++;
      }
   }
   // check that all the required entries have been set
   CPPAD_ASSERT_UNKNOWN( k == K );
   return n_color;
}

} // END_CPPAD_NAMESPACE

# endif
