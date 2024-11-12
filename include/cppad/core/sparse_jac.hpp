# ifndef CPPAD_CORE_SPARSE_JAC_HPP
# define CPPAD_CORE_SPARSE_JAC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin sparse_jac}
{xrst_spell
   nr
}

Computing Sparse Jacobians
##########################

Syntax
******
| *n_color* = *f* . ``sparse_jac_for`` (
| |tab| *group_max* , *x* , *subset* , *pattern* , *coloring* , *work*
| )
| *n_color* = *f* . ``sparse_jac_rev`` (
| |tab| *x* , *subset* , *pattern* , *coloring* , *work*
| )

Purpose
*******
We use :math:`F : \B{R}^n \rightarrow \B{R}^m` to denote the
function corresponding to *f* .
Here *n* is the :ref:`fun_property@Domain` size,
and *m* is the :ref:`fun_property@Range` size, or *f* .
The syntax above takes advantage of sparsity when computing the Jacobian

.. math::

   J(x) = F^{(1)} (x)

In the sparse case, this should be faster and take less memory than
:ref:`Jacobian-name` .
We use the notation :math:`J_{i,j} (x)` to denote the partial of
:math:`F_i (x)` with respect to :math:`x_j`.

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

sparse_jac_for
**************
This function uses first order forward mode sweeps :ref:`forward_one-name`
to compute multiple columns of the Jacobian at the same time.

sparse_jac_rev
**************
This uses function first order reverse mode sweeps :ref:`reverse_one-name`
to compute multiple rows of the Jacobian at the same time.

f
*
This object has prototype

   ``ADFun`` < *Base* > *f*

Note that the Taylor coefficients stored in *f* are affected
by this operation; see
:ref:`sparse_jac@Uses Forward` below.

group_max
*********
This argument has prototype

   ``size_t`` *group_max*

and must be greater than zero.
It specifies the maximum number of colors to group during
a single forward sweep.
If a single color is in a group,
a single direction for of first order forward mode
:ref:`forward_one-name` is used for each color.
If multiple colors are in a group,
the multiple direction for of first order forward mode
:ref:`forward_dir-name` is used with one direction for each color.
This uses separate memory for each direction (more memory),
but my be significantly faster.

x
*
This argument has prototype

   ``const`` *BaseVector* & *x*

and its size is *n* .
It specifies the point at which to evaluate the Jacobian
:math:`J(x)`.

subset
******
This argument has prototype

   ``sparse_rcv`` < *SizeVector* , *BaseVector* >& *subset*

Its row size is *subset* . ``nr`` () == *m* ,
and its column size is *subset* . ``nc`` () == *n* .
It specifies which elements of the Jacobian are computed.
The input value of its value vector
*subset* . ``val`` () does not matter.
Upon return it contains the value of the corresponding elements
of the Jacobian.
All of the row, column pairs in *subset* must also appear in
*pattern* ; i.e., they must be possibly non-zero.

pattern
*******
This argument has prototype

   ``const sparse_rc`` < *SizeVector* >& *pattern*

Its row size is *pattern* . ``nr`` () == *m* ,
and its column size is *pattern* . ``nc`` () == *n* .
It is a sparsity pattern for the Jacobian :math:`J(x)`.
This argument is not used (and need not satisfy any conditions),
when :ref:`sparse_jac@work` is non-empty.

coloring
********
The coloring algorithm determines which rows (reverse) or columns (forward)
can be computed during the same sweep.
This field has prototype

   ``const std::string&`` *coloring*

This value only matters when work is empty; i.e.,
after the *work* constructor or *work* . ``clear`` () .

cppad
=====
This uses a general purpose coloring algorithm written for Cppad.

colpack
=======
If :ref:`colpack_prefix-name` is specified on the
:ref:`cmake@CMake Command` line,
you can set *coloring* to ``colpack`` .
This uses a general purpose coloring algorithm that is part of Colpack.

work
****
This argument has prototype

   ``sparse_jac_work&`` *work*

We refer to its initial value,
and its value after *work* . ``clear`` () , as empty.
If it is empty, information is stored in *work* .
This can be used to reduce computation when
a future call is for the same object *f* ,
the same member function ``sparse_jac_for`` or ``sparse_jac_rev`` ,
and the same subset of the Jacobian.
In fact, it can be used with a different *f*
and a different *subset* provided that Jacobian sparsity pattern
for *f* and the sparsity pattern in *subset* are the same.
If any of these values change, use *work* . ``clear`` () to
empty this structure.

n_color
*******
The return value *n_color* has prototype

   ``size_t`` *n_color*

If ``sparse_jac_for`` (``sparse_jac_rev`` ) is used,
*n_color* is the number of first order forward directions
used to compute the requested Jacobian values.
It is also the number of colors determined by the coloring method
mentioned above.
This is proportional to the total computational work,
not counting the zero order forward sweep,
or combining multiple columns (rows) into a single sweep.
Note that if *group_max*  == 1 ,
or if we are using ``sparse_jac_rev`` ,
*n_color* is equal to the number of sweeps.

Uses Forward
************
After each call to :ref:`Forward-name` ,
the object *f* contains the corresponding
:ref:`Taylor coefficients<glossary@Taylor Coefficient>` .
After a call to ``sparse_jac_forward`` or ``sparse_jac_rev`` ,
the zero order coefficients correspond to

   *f* . ``Forward`` (0, *x* )

All the other forward mode coefficients are unspecified.

Example
*******
{xrst_toc_hidden
   example/sparse/sparse_jac_for.cpp
   example/sparse/sparse_jac_rev.cpp
}
The files :ref:`sparse_jac_for.cpp-name` and :ref:`sparse_jac_rev.cpp-name`
are examples and tests of ``sparse_jac_for`` and ``sparse_jac_rev`` .
They return ``true`` , if they succeed, and ``false`` otherwise.

{xrst_end sparse_jac}
*/
# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/sparse/internal.hpp>
# include <cppad/local/color_general.hpp>
# include <cppad/utility/vector.hpp>

/*!
\file sparse_jac.hpp
Sparse Jacobian calculation routines.
*/
namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
Class used to hold information used by Sparse Jacobian routines in this file,
so they do not need to be recomputed every time.
*/
class sparse_jac_work {
   public:
      /// indices that sort the user row and col arrays by color
      CppAD::vector<size_t> order;
      /// results of the coloring algorithm
      CppAD::vector<size_t> color;
      //
      /// constructor
      sparse_jac_work(void)
      { }
      /// reset work to empty.
      /// This informs CppAD that color and order need to be recomputed
      void clear(void)
      {  order.clear();
         color.clear();
      }
};
// ----------------------------------------------------------------------------
/*!
Calculate sparse Jacobains using forward mode

\tparam Base
the base type for the recording that is stored in the ADFun object.

\tparam SizeVector
a simple vector class with elements of type size_t.

\tparam BaseVector
a simple vector class with elements of type Base.

\param group_max
specifies the maximum number of colors to group during a single forward sweep.
This must be greater than zero and group_max = 1 minimizes memory usage.

\param x
a vector of length n, the number of independent variables in f
(this ADFun object).

\param subset
specifices the subset of the sparsity pattern where the Jacobian is evaluated.
subset.nr() == m,
subset.nc() == n.

\param pattern
is a sparsity pattern for the Jacobian of f;
pattern.nr() == m,
pattern.nc() == n,
where m is number of dependent variables in f.

\param coloring
determines which coloring algorithm is used.
This must be cppad or colpack.

\param work
this structure must be empty, or contain the information stored
by a previous call to sparse_jac_for.
The previous call must be for the same ADFun object f
and the same subset.

\return
This is the number of first order forward sweeps used to compute
the Jacobian.
*/
template <class Base, class RecBase>
template <class SizeVector, class BaseVector>
size_t ADFun<Base,RecBase>::sparse_jac_for(
   size_t                               group_max  ,
   const BaseVector&                    x          ,
   sparse_rcv<SizeVector, BaseVector>&  subset     ,
   const sparse_rc<SizeVector>&         pattern    ,
   const std::string&                   coloring   ,
   sparse_jac_work&                     work       )
{  size_t m = Range();
   size_t n = Domain();
   //
   CPPAD_ASSERT_KNOWN(
      subset.nr() == m,
      "sparse_jac_for: subset.nr() not equal range dimension for f"
   );
   CPPAD_ASSERT_KNOWN(
      subset.nc() == n,
      "sparse_jac_for: subset.nc() not equal domain dimension for f"
   );
   //
   // row and column vectors in subset
   const SizeVector& row( subset.row() );
   const SizeVector& col( subset.col() );
   //
   vector<size_t>& color(work.color);
   vector<size_t>& order(work.order);
   CPPAD_ASSERT_KNOWN(
      color.size() == 0 || color.size() == n,
      "sparse_jac_for: work is non-empty and conditions have changed"
   );
   //
   // point at which we are evaluationg the Jacobian
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
   // check for case where input work is empty
   if( color.size() == 0 )
   {  // compute work color and order vectors
      CPPAD_ASSERT_KNOWN(
         pattern.nr() == m,
         "sparse_jac_for: pattern.nr() not equal range dimension for f"
      );
      CPPAD_ASSERT_KNOWN(
         pattern.nc() == n,
         "sparse_jac_for: pattern.nc() not equal domain dimension for f"
      );
      //
      // convert pattern to an internal version of its transpose
      local::pod_vector<size_t> internal_index(n);
      for(size_t j = 0; j < n; j++)
         internal_index[j] = j;
      bool transpose   = true;
      bool zero_empty  = false;
      bool input_empty = true;
      local::sparse::list_setvec pattern_transpose;
      pattern_transpose.resize(n, m);
      local::sparse::set_internal_pattern(zero_empty, input_empty,
         transpose, internal_index, pattern_transpose, pattern
      );
      //
      // execute coloring algorithm
      // (we are using transpose because coloring groups rows, not columns).
      color.resize(n);
      if( coloring == "cppad" )
         local::color_general_cppad(pattern_transpose, col, row, color);
      else if( coloring == "colpack" )
      {
# if CPPAD_HAS_COLPACK
         local::color_general_colpack(pattern_transpose, col, row, color);
# else
         CPPAD_ASSERT_KNOWN(
            false,
            "sparse_jac_for: coloring = colpack "
            "and colpack_prefix missing from cmake command line."
         );
# endif
      }
      else CPPAD_ASSERT_KNOWN(
         false,
         "sparse_jac_for: coloring is not valid."
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
   // initialize the return Jacobian values as zero
   for(size_t k = 0; k < K; k++)
      subset.set(k, zero);
   //
   // index in subset
   size_t k = 0;
   // number of colors computed so far
   size_t color_count = 0;
   //
   while( color_count < n_color )
   {  // number of colors that will be in this group
      size_t group_size = std::min<size_t>(group_max, n_color - color_count);
      //
      // forward mode values for independent and dependent variables
      BaseVector dx(n * group_size), dy(m * group_size);
      //
      // set dx
      for(size_t ell = 0; ell < group_size; ell++)
      {  // combine all columns with this color
         for(size_t j = 0; j < n; j++)
         {  dx[j * group_size + ell] = zero;
            if( color[j] == ell + color_count )
               dx[j * group_size + ell] = one;
         }
      }
      if( group_size == 1 )
         dy = Forward(1, dx);
      else
         dy = Forward(1, group_size, dx);
      //
      // store results in subset
      for(size_t ell = 0; ell < group_size; ell++)
      {  // color with index ell + color_count is in this group
         while(k < K && color[ col[ order[k] ] ] == ell + color_count )
         {  // subset element with index order[k] is included in this color
            size_t r = row[ order[k] ];
            subset.set( order[k], dy[ r * group_size + ell ] );
            ++k;
         }
      }
      // advance color count
      color_count += group_size;
   }
   CPPAD_ASSERT_UNKNOWN( color_count == n_color );
   //
   return n_color;
}
// ----------------------------------------------------------------------------
/*!
Calculate sparse Jacobains using reverse mode

\tparam Base
the base type for the recording that is stored in the ADFun object.

\tparam SizeVector
a simple vector class with elements of type size_t.

\tparam BaseVector
a simple vector class with elements of type Base.

\param x
a vector of length n, the number of independent variables in f
(this ADFun object).

\param subset
specifices the subset of the sparsity pattern where the Jacobian is evaluated.
subset.nr() == m,
subset.nc() == n.

\param pattern
is a sparsity pattern for the Jacobian of f;
pattern.nr() == m,
pattern.nc() == n,
where m is number of dependent variables in f.

\param coloring
determines which coloring algorithm is used.
This must be cppad or colpack.

\param work
this structure must be empty, or contain the information stored
by a previous call to sparse_jac_rev.
The previous call must be for the same ADFun object f
and the same subset.

\return
This is the number of first order reverse sweeps used to compute
the Jacobian.
*/
template <class Base, class RecBase>
template <class SizeVector, class BaseVector>
size_t ADFun<Base,RecBase>::sparse_jac_rev(
   const BaseVector&                    x        ,
   sparse_rcv<SizeVector, BaseVector>&  subset   ,
   const sparse_rc<SizeVector>&         pattern  ,
   const std::string&                   coloring ,
   sparse_jac_work&                     work     )
{  size_t m = Range();
   size_t n = Domain();
   //
   CPPAD_ASSERT_KNOWN(
      subset.nr() == m,
      "sparse_jac_rev: subset.nr() not equal range dimension for f"
   );
   CPPAD_ASSERT_KNOWN(
      subset.nc() == n,
      "sparse_jac_rev: subset.nc() not equal domain dimension for f"
   );
   //
   // row and column vectors in subset
   const SizeVector& row( subset.row() );
   const SizeVector& col( subset.col() );
   //
   vector<size_t>& color(work.color);
   vector<size_t>& order(work.order);
   CPPAD_ASSERT_KNOWN(
      color.size() == 0 || color.size() == m,
      "sparse_jac_rev: work is non-empty and conditions have changed"
   );
   //
   // point at which we are evaluationg the Jacobian
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
   // check for case where input work is empty
   if( color.size() == 0 )
   {  // compute work color and order vectors
      CPPAD_ASSERT_KNOWN(
         pattern.nr() == m,
         "sparse_jac_rev: pattern.nr() not equal range dimension for f"
      );
      CPPAD_ASSERT_KNOWN(
         pattern.nc() == n,
         "sparse_jac_rev: pattern.nc() not equal domain dimension for f"
      );
      //
      // convert pattern to an internal version
      local::pod_vector<size_t> internal_index(m);
      for(size_t i = 0; i < m; i++)
         internal_index[i] = i;
      bool transpose   = false;
      bool zero_empty  = false;
      bool input_empty = true;
      local::sparse::list_setvec internal_pattern;
      internal_pattern.resize(m, n);
      local::sparse::set_internal_pattern(zero_empty, input_empty,
         transpose, internal_index, internal_pattern, pattern
      );
      //
      // execute coloring algorithm
      color.resize(m);
      if( coloring == "cppad" )
         local::color_general_cppad(internal_pattern, row, col, color);
      else if( coloring == "colpack" )
      {
# if CPPAD_HAS_COLPACK
         local::color_general_colpack(internal_pattern, row, col, color);
# else
         CPPAD_ASSERT_KNOWN(
            false,
            "sparse_jac_rev: coloring = colpack "
            "and colpack_prefix missing from cmake command line."
         );
# endif
      }
      else CPPAD_ASSERT_KNOWN(
         false,
         "sparse_jac_rev: coloring is not valid."
      );
      //
      // put sorting indices in color order
      SizeVector key(K);
      order.resize(K);
      for(size_t k = 0; k < K; k++)
         key[k] = color[ row[k] ];
      index_sort(key, order);
   }
   // Base versions of zero and one
   Base one(1.0);
   Base zero(0.0);
   //
   size_t n_color = 1;
   for(size_t i = 0; i < m; i++) if( color[i] < m )
      n_color = std::max<size_t>(n_color, color[i] + 1);
   //
   // initialize the return Jacobian values as zero
   for(size_t k = 0; k < K; k++)
      subset.set(k, zero);
   //
   // weighting vector and return values for calls to Reverse
   BaseVector w(m), dw(n);
   //
   // loop over colors
   size_t k = 0;
   for(size_t ell = 0; ell < n_color; ell++)
   if( k  == K )
   {  // kludge because colpack returns colors that are not used
      // (it does not know about the subset corresponding to row, col)
      CPPAD_ASSERT_UNKNOWN( coloring == "colpack" );
   }
   else if( color[ row[ order[k] ] ] != ell )
   {  // kludge because colpack returns colors that are not used
      // (it does not know about the subset corresponding to row, col)
      CPPAD_ASSERT_UNKNOWN( coloring == "colpack" );
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( color[ row[ order[k] ] ] == ell );
      //
      // combine all rows with this color
      for(size_t i = 0; i < m; i++)
      {  w[i] = zero;
         if( color[i] == ell )
            w[i] = one;
      }
      // call reverse mode for all these rows at once
      dw = Reverse(1, w);
      //
      // set the corresponding components of the result
      while( k < K && color[ row[order[k]] ] == ell )
      {  subset.set(order[k], dw[col[order[k]]] );
         k++;
      }
   }
   return n_color;
}

} // END_CPPAD_NAMESPACE
# endif
