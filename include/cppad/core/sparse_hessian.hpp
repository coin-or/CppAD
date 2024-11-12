# ifndef CPPAD_CORE_SPARSE_HESSIAN_HPP
# define CPPAD_CORE_SPARSE_HESSIAN_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin sparse_hessian}
{xrst_spell
   valarray
}

Sparse Hessian
##############

Syntax
******
| *hes* = *f* . ``SparseHessian`` ( *x* , *w* )
| *hes* = *f* . ``SparseHessian`` ( *x* , *w* , *p* )
| *n_sweep* = *f* . ``SparseHessian`` ( *x* , *w* , *p* , *row* , *col* , *hes* , *work* )

Purpose
*******
We use :math:`n` for the :ref:`fun_property@Domain` size,
and :math:`m` for the :ref:`fun_property@Range` size of *f* .
We use :math:`F : \B{R}^n \rightarrow \B{R}^m` do denote the
:ref:`glossary@AD Function`
corresponding to *f* .
The syntax above sets *hes* to the Hessian

.. math::

   H(x) = \dpow{2}{x} \sum_{i=1}^m w_i F_i (x)

This routine takes advantage of the sparsity of the Hessian
in order to reduce the amount of computation necessary.
If *row* and *col* are present, it also takes
advantage of the reduced set of elements of the Hessian that
need to be computed.
One can use speed tests (e.g. :ref:`speed_test-name` )
to verify that results are computed faster
than when using the routine :ref:`Hessian-name` .

f
*
The object *f* has prototype

   ``ADFun`` < *Base* > *f*

Note that the :ref:`ADFun-name` object *f* is not ``const``
(see :ref:`sparse_hessian@Uses Forward` below).

x
*
The argument *x* has prototype

   ``const`` *BaseVector* & *x*

(see :ref:`sparse_hessian@BaseVector` below)
and its size
must be equal to *n* , the dimension of the
:ref:`fun_property@Domain` space for *f* .
It specifies
that point at which to evaluate the Hessian.

w
*
The argument *w* has prototype

   ``const`` *BaseVector* & *w*

and size :math:`m`.
It specifies the value of :math:`w_i` in the expression
for *hes* .
The more components of :math:`w` that are identically zero,
the more sparse the resulting Hessian may be (and hence the more efficient
the calculation of *hes* may be).

p
*
The argument *p* is optional and has prototype

   ``const`` *SetVector* & *p*

(see :ref:`sparse_hessian@SetVector` below)
If it has elements of type ``bool`` ,
its size is :math:`n * n`.
If it has elements of type ``std::set<size_t>`` ,
its size is :math:`n` and all its set elements are between
zero and :math:`n - 1`.
It specifies a
:ref:`glossary@Sparsity Pattern`
for the Hessian :math:`H(x)`.

Purpose
=======
If this sparsity pattern does not change between calls to
``SparseHessian`` , it should be faster to calculate *p* once and
pass this argument to ``SparseHessian`` .
If you specify *p* , CppAD will use the same
type of sparsity representation
(vectors of ``bool`` or vectors of ``std::set<size_t>`` )
for its internal calculations.
Otherwise, the representation
for the internal calculations is unspecified.

work
====
If you specify *work* in the calling sequence,
it is not necessary to keep the sparsity pattern; see the heading
:ref:`sparse_hessian@work@p` under the *work* description.

Column Subset
=============
If the arguments *row* and *col* are present,
and :ref:`sparse_hessian@work@color_method` is
``cppad.general`` or ``cppad.symmetric`` ,
it is not necessary to compute the entire sparsity pattern.
Only the following subset of column values will matter:

   { *col* [ *k* ] : *k* = 0 , ... , *K* ``-1`` }

.

row, col
********
The arguments *row* and *col* are optional and have prototype

| |tab| ``const`` *SizeVector* & *row*
| |tab| ``const`` *SizeVector* & *col*

(see :ref:`sparse_hessian@SizeVector` below).
They specify which rows and columns of :math:`H (x)` are
returned and in what order.
We use :math:`K` to denote the value *hes* . ``size`` ()
which must also equal the size of *row* and *col* .
Furthermore,
for :math:`k = 0 , \ldots , K-1`, it must hold that
:math:`row[k] < n` and :math:`col[k] < n`.
In addition,
all of the :math:`(row[k], col[k])` pairs must correspond to a true value
in the sparsity pattern *p* .

hes
***
The result *hes* has prototype

   *BaseVector* *hes*

In the case where *row* and *col* are not present,
the size of *hes* is :math:`n * n` and
its size is :math:`n * n`.
In this case, for :math:`i = 0 , \ldots , n - 1`
and :math:`ell = 0 , \ldots , n - 1`

.. math::

   hes [ j * n + \ell ] = \DD{ w^{\rm T} F }{ x_j }{ x_\ell } ( x )

In the case where the arguments *row* and *col* are present,
we use :math:`K` to denote the size of *hes* .
The input value of its elements does not matter.
Upon return, for :math:`k = 0 , \ldots , K - 1`,

.. math::

   hes [ k ] = \DD{ w^{\rm T} F }{ x_j }{ x_\ell } (x)
   \; , \;
   \; {\rm where} \;
   j = row[k]
   \; {\rm and } \;
   \ell = col[k]

work
****
If this argument is present, it has prototype

   ``sparse_hessian_work&`` *work*

This object can only be used with the routines ``SparseHessian`` .
During its the first use, information is stored in *work* .
This is used to reduce the work done by future calls to ``SparseHessian``
with the same *f* , *p* , *row* , and *col* .
If a future call is made where any of these values have changed,
you must first call *work* . ``clear`` ()
to inform CppAD that this information needs to be recomputed.

color_method
============
The coloring algorithm determines which rows and columns
can be computed during the same sweep.
This field has prototype

   ``std::string`` *work* . ``color_method``

This value only matters on the first call to ``sparse_hessian`` that
follows the *work* constructor or a call to
*work* . ``clear`` () .

   ``"cppad.symmetric"``

This is the default coloring method (after a constructor or ``clear()`` ).
It takes advantage of the fact that the Hessian matrix
is symmetric to find a coloring that requires fewer
:ref:`sweeps<sparse_hessian@n_sweep>` .

   ``"cppad.general"``

This is the same as the ``"cppad"`` method for the
:ref:`sparse_jacobian<sparse_jacobian@work@color_method>` calculation.

   ``"colpack.symmetric"``

This method requires that
:ref:`colpack_prefix-name` was specified on the
:ref:`cmake@CMake Command` line.
It also takes advantage of the fact that the Hessian matrix is symmetric.

   ``"colpack.general"``

This is the same as the ``"colpack"`` method for the
:ref:`sparse_jacobian<sparse_jacobian@work@color_method>` calculation.

colpack.star Deprecated 2017-06-01
==================================
The ``colpack.star`` method is deprecated.
It is the same as the ``colpack.symmetric``
which should be used instead.

p
=
If *work* is present, and it is not the first call after
its construction or a clear,
the sparsity pattern *p* is not used.
This enables one to free the sparsity pattern
and still compute corresponding sparse Hessians.

n_sweep
*******
The return value *n_sweep* has prototype

   ``size_t`` *n_sweep*

It is the number of first order forward sweeps
used to compute the requested Hessian values.
Each first forward sweep is followed by a second order reverse sweep
so it is also the number of reverse sweeps.
This is proportional to the total work that ``SparseHessian`` does,
not counting the zero order forward sweep,
or the work to combine multiple columns into a single
forward-reverse sweep pair.

BaseVector
**********
The type *BaseVector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
*Base* .
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.

SetVector
*********
The type *SetVector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
``bool`` or ``std::set<size_t>`` ;
see :ref:`glossary@Sparsity Pattern` for a discussion
of the difference.
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.

Restrictions
============
If *SetVector* has elements of ``std::set<size_t>`` ,
then *p* [ *i* ] must return a reference (not a copy) to the
corresponding set.
According to section 26.3.2.3 of the 1998 C++ standard,
``std::valarray< std::set<size_t> >`` does not satisfy
this condition.

SizeVector
**********
The type *SizeVector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
``size_t`` .
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.

Uses Forward
************
After each call to :ref:`Forward-name` ,
the object *f* contains the corresponding
:ref:`Taylor coefficients<glossary@Taylor Coefficient>` .
After a call to any of the sparse Hessian routines,
the zero order Taylor coefficients correspond to
*f* . ``Forward`` (0, *x* )
and the other coefficients are unspecified.
{xrst_toc_hidden
   example/sparse/sparse_hessian.cpp
   example/sparse/sub_sparse_hes.cpp
   example/sparse/sparse_sub_hes.cpp
}

Example
*******
The routine
:ref:`sparse_hessian.cpp-name`
is examples and tests of ``sparse_hessian`` .
It return ``true`` , if it succeeds and ``false`` otherwise.

Subset Hessian
**************
The routine
:ref:`sub_sparse_hes.cpp-name`
is an example and test that compute a sparse Hessian
for a subset of the variables.
It returns ``true`` , for success, and ``false`` otherwise.

{xrst_end sparse_hessian}
-----------------------------------------------------------------------------
*/
# include <cppad/local/std_set.hpp>
# include <cppad/local/color_general.hpp>
# include <cppad/local/color_symmetric.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file sparse_hessian.hpp
Sparse Hessian driver routine and helper functions.
*/
// ===========================================================================
/*!
class used by SparseHessian to hold information
so it does not need to be recomputed.
*/
class sparse_hessian_work {
   public:
      /// Coloring method: "cppad", or "colpack"
      /// (this field is set by user)
      std::string color_method;
      /// row and column indicies for return values
      /// (some may be reflected by star coloring algorithm)
      CppAD::vector<size_t> row;
      CppAD::vector<size_t> col;
      /// indices that sort the user row and col arrays by color
      CppAD::vector<size_t> order;
      /// results of the coloring algorithm
      CppAD::vector<size_t> color;

      /// constructor
      sparse_hessian_work(void) : color_method("cppad.symmetric")
      { }
      /// inform CppAD that this information needs to be recomputed
      void clear(void)
      {  color_method = "cppad.symmetric";
         row.clear();
         col.clear();
         order.clear();
         color.clear();
      }
};
// ===========================================================================
/*!
Private helper function that does computation for all Sparse Hessian cases.

\tparam Base
is the base type for the recording that is stored in this ADFun<Base object.

\tparam BaseVector
is a simple vector class with elements of type Base.

\tparam SetVector
is a simple vector class with elements of type
 bool or std::set<size_t>.

\tparam SizeVector
is sparse_pack or sparse_list.

\param x [in]
is a vector specifing the point at which to compute the Hessian.

\param w [in]
is the weighting vector that defines a scalar valued function by
a weighted sum of the components of the vector valued function
$latex F(x)$$.

\param sparsity [in]
is the sparsity pattern for the Hessian that we are calculating.

\param user_row [in]
is the vector of row indices for the returned Hessian values.

\param user_col [in]
is the vector of columns indices for the returned Hessian values.
It must have the same size as user_row.

\param hes [out]
is the vector of Hessian values.
It must have the same size as user_row.
The return value <code>hes[k]</code> is the second partial of
\f$ w^{\rm T} F(x)\f$ with respect to the
<code>row[k]</code> and <code>col[k]</code> component of \f$ x\f$.

\param work
This structure contains information that is computed by SparseHessianCompute.
If the sparsity pattern, row vector, or col vectors
are not the same between calls to SparseHessianCompute,
 work.clear() must be called to reinitialize work.

\return
Is the number of first order forward sweeps used to compute the
requested Hessian values.
(This is also equal to the number of second order reverse sweeps.)
The total work, not counting the zero order
forward sweep, or the time to combine computations, is proportional to this
return value.
*/
template <class Base, class RecBase>
template <class BaseVector, class SetVector, class SizeVector>
size_t ADFun<Base,RecBase>::SparseHessianCompute(
   const BaseVector&           x           ,
   const BaseVector&           w           ,
          SetVector&            sparsity    ,
   const SizeVector&           user_row    ,
   const SizeVector&           user_col    ,
          BaseVector&           hes         ,
          sparse_hessian_work&  work        )
{
   using   CppAD::vectorBool;
   size_t i, k, ell;

   CppAD::vector<size_t>& row(work.row);
   CppAD::vector<size_t>& col(work.col);
   CppAD::vector<size_t>& color(work.color);
   CppAD::vector<size_t>& order(work.order);

   size_t n = Domain();

   // some values
   const Base zero(0);
   const Base one(1);

   // check BaseVector is Simple Vector class with Base type elements
   CheckSimpleVector<Base, BaseVector>();

   // number of components of Hessian that are required
   size_t K = hes.size();
   CPPAD_ASSERT_UNKNOWN( size_t( user_row.size() ) == K );
   CPPAD_ASSERT_UNKNOWN( size_t( user_col.size() ) == K );

   CPPAD_ASSERT_UNKNOWN( size_t(x.size()) == n );
   CPPAD_ASSERT_UNKNOWN( color.size() == 0 || color.size() == n );
   CPPAD_ASSERT_UNKNOWN( row.size() == 0   || row.size() == K );
   CPPAD_ASSERT_UNKNOWN( col.size() == 0   || col.size() == K );


   // Point at which we are evaluating the Hessian
   Forward(0, x);

   // check for case where nothing (except Forward above) to do
   if( K == 0 )
      return 0;

   // Rows of the Hessian (i below) correspond to the forward mode index
   // and columns (j below) correspond to the reverse mode index.
   if( color.size() == 0 )
   {
      CPPAD_ASSERT_UNKNOWN( sparsity.n_set() ==  n );
      CPPAD_ASSERT_UNKNOWN( sparsity.end() ==  n );

      // copy user rwo and col to work space
      row.resize(K);
      col.resize(K);
      for(k = 0; k < K; k++)
      {  row[k] = user_row[k];
         col[k] = user_col[k];
      }

      // execute coloring algorithm
      color.resize(n);
      if( work.color_method == "cppad.general" )
         local::color_general_cppad(sparsity, row, col, color);
      else if( work.color_method == "cppad.symmetric" )
         local::color_symmetric_cppad(sparsity, row, col, color);
      else if( work.color_method == "colpack.general" )
      {
# if CPPAD_HAS_COLPACK
         local::color_general_colpack(sparsity, row, col, color);
# else
         CPPAD_ASSERT_KNOWN(
            false,
            "SparseHessian: work.color_method = colpack.general "
            "and colpack_prefix missing from cmake command line."
         );
# endif
      }
      else if(
         work.color_method == "colpack.symmetric" ||
         work.color_method == "colpack.star"
      )
      {
# if CPPAD_HAS_COLPACK
         local::color_symmetric_colpack(sparsity, row, col, color);
# else
         CPPAD_ASSERT_KNOWN(
            false,
            "SparseHessian: work.color_method is "
            "colpack.symmetric or colpack.star\n"
            "and colpack_prefix missing from cmake command line."
         );
# endif
      }
      else
      {  CPPAD_ASSERT_KNOWN(
            false,
            "SparseHessian: work.color_method is not valid."
         );
      }

      // put sorting indices in color order
      SizeVector key(K);
      order.resize(K);
      for(k = 0; k < K; k++)
         key[k] = color[ row[k] ];
      index_sort(key, order);

   }
   size_t n_color = 1;
   for(ell = 0; ell < n; ell++) if( color[ell] < n )
      n_color = std::max<size_t>(n_color, color[ell] + 1);

   // direction vector for calls to forward (rows of the Hessian)
   BaseVector u(n);

   // location for return values from reverse (columns of the Hessian)
   BaseVector ddw(2 * n);

   // initialize the return value
   for(k = 0; k < K; k++)
      hes[k] = zero;

   // loop over colors
# ifndef NDEBUG
   const std::string& coloring = work.color_method;
# endif
   k = 0;
   for(ell = 0; ell < n_color; ell++)
   if( k == K )
   {  // kludge because colpack returns colors that are not used
      // (it does not know about the subset corresponding to row, col)
      CPPAD_ASSERT_UNKNOWN(
         coloring == "colpack.general" ||
         coloring == "colpack.symmetic" ||
         coloring == "colpack.star"
      );
   }
   else if( color[ row[ order[k] ] ] != ell )
   {  // kludge because colpack returns colors that are not used
      // (it does not know about the subset corresponding to row, col)
      CPPAD_ASSERT_UNKNOWN(
         coloring == "colpack.general" ||
         coloring == "colpack.symmetic" ||
         coloring == "colpack.star"
      );
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( color[ row[ order[k] ] ] == ell );

      // combine all rows with this color
      for(i = 0; i < n; i++)
      {  u[i] = zero;
         if( color[i] == ell )
            u[i] = one;
      }
      // call forward mode for all these rows at once
      Forward(1, u);

      // evaluate derivative of w^T * F'(x) * u
      ddw = Reverse(2, w);

      // set the corresponding components of the result
      while( k < K && color[ row[ order[k] ] ] == ell )
      {  hes[ order[k] ] = ddw[ col[ order[k] ] * 2 + 1 ];
         k++;
      }
   }
   return n_color;
}
// ===========================================================================
// Public Member Functions
// ===========================================================================
/*!
Compute user specified subset of a sparse Hessian.

The C++ source code corresponding to this operation is
\verbatim
   SparceHessian(x, w, p, row, col, hes, work)
\endverbatim

\tparam Base
is the base type for the recording that is stored in this ADFun<Base object.

\tparam BaseVector
is a simple vector class with elements of type Base.

\tparam SetVector
is a simple vector class with elements of type
 bool or std::set<size_t>.

\tparam SizeVector
is a simple vector class with elements of type size_t.

\param x [in]
is a vector specifing the point at which to compute the Hessian.

\param w [in]
is the weighting vector that defines a scalar valued function by
a weighted sum of the components of the vector valued function
$latex F(x)$$.

\param p [in]
is the sparsity pattern for the Hessian that we are calculating.

\param row [in]
is the vector of row indices for the returned Hessian values.

\param col [in]
is the vector of columns indices for the returned Hessian values.
It must have the same size are r.

\param hes [out]
is the vector of Hessian values.
It must have the same size are r.
The return value <code>hes[k]</code> is the second partial of
\f$ w^{\rm T} F(x)\f$ with respect to the
<code>row[k]</code> and <code>col[k]</code> component of \f$ x\f$.

\param work
This structure contains information that is computed by SparseHessianCompute.
If the sparsity pattern, row vector, or col vectors
are not the same between calls to SparseHessian,
 work.clear() must be called to reinitialize work.

\return
Is the number of first order forward sweeps used to compute the
requested Hessian values.
(This is also equal to the number of second order reverse sweeps.)
The total work, not counting the zero order
forward sweep, or the time to combine computations, is proportional to this
return value.
*/
template <class Base, class RecBase>
template <class BaseVector, class SetVector, class SizeVector>
size_t ADFun<Base,RecBase>::SparseHessian(
   const BaseVector&     x    ,
   const BaseVector&     w    ,
   const SetVector&      p    ,
   const SizeVector&     row  ,
   const SizeVector&     col  ,
   BaseVector&           hes  ,
   sparse_hessian_work&  work )
{
   size_t n    = Domain();
   size_t K    = hes.size();
# ifndef NDEBUG
   size_t k;
   CPPAD_ASSERT_KNOWN(
      size_t(x.size()) == n ,
      "SparseHessian: size of x not equal domain dimension for f."
   );
   CPPAD_ASSERT_KNOWN(
      size_t(row.size()) == K && size_t(col.size()) == K ,
      "SparseHessian: either r or c does not have the same size as ehs."
   );
   CPPAD_ASSERT_KNOWN(
      work.color.size() == 0 || work.color.size() == n,
      "SparseHessian: invalid value in work."
   );
   for(k = 0; k < K; k++)
   {  CPPAD_ASSERT_KNOWN(
         row[k] < n,
         "SparseHessian: invalid value in r."
      );
      CPPAD_ASSERT_KNOWN(
         col[k] < n,
         "SparseHessian: invalid value in c."
      );
   }
   if( work.color.size() != 0 )
      for(size_t j = 0; j < n; j++) CPPAD_ASSERT_KNOWN(
         work.color[j] <= n,
         "SparseHessian: invalid value in work."
   );
# endif
   // check for case where there is nothing to compute
   size_t n_sweep = 0;
   if( K == 0 )
      return n_sweep;

   typedef typename SetVector::value_type Set_type;
   typedef typename local::sparse::internal_pattern<Set_type>::pattern_type Pattern_type;
   Pattern_type s;
   if( work.color.size() == 0 )
   {  bool transpose = false;
      const char* error_msg = "SparseHessian: sparsity pattern"
      " does not have proper row or column dimension";
      sparsity_user2internal(s, p, n, n, transpose, error_msg);
   }
   n_sweep = SparseHessianCompute(x, w, s, row, col, hes, work);
   return n_sweep;
}
/*!
Compute a sparse Hessian.

The C++ source code coresponding to this operation is
\verbatim
   hes = SparseHessian(x, w, p)
\endverbatim


\tparam Base
is the base type for the recording that is stored in this
ADFun<Base object.

\tparam BaseVector
is a simple vector class with elements of the Base.

\tparam SetVector
is a simple vector class with elements of type
 bool or std::set<size_t>.

\param x [in]
is a vector specifing the point at which to compute the Hessian.

\param w [in]
The Hessian is computed for a weighted sum of the components
of the function corresponding to this ADFun<Base> object.
The argument w specifies the weights for each component.
It must have size equal to the range dimension for this ADFun<Base> object.

\param p [in]
is a sparsity pattern for the Hessian.

\return
Will be a vector of size n * n containing the Hessian of
at the point specified by x
(where n is the domain dimension for this ADFun<Base> object).
*/
template <class Base, class RecBase>
template <class BaseVector, class SetVector>
BaseVector ADFun<Base,RecBase>::SparseHessian(
   const BaseVector& x, const BaseVector& w, const SetVector& p
)
{  size_t i, j, k;

   size_t n = Domain();
   BaseVector hes(n * n);

   CPPAD_ASSERT_KNOWN(
      size_t(x.size()) == n,
      "SparseHessian: size of x not equal domain size for f."
   );

   typedef typename SetVector::value_type Set_type;
   typedef typename local::sparse::internal_pattern<Set_type>::pattern_type Pattern_type;

   // initialize the return value as zero
   Base zero(0);
   for(i = 0; i < n; i++)
      for(j = 0; j < n; j++)
         hes[i * n + j] = zero;

   // arguments to SparseHessianCompute
   Pattern_type          s;
   CppAD::vector<size_t> row;
   CppAD::vector<size_t> col;
   sparse_hessian_work   work;
   bool transpose = false;
   const char* error_msg = "SparseHessian: sparsity pattern"
   " does not have proper row or column dimension";
   sparsity_user2internal(s, p, n, n, transpose, error_msg);
   k = 0;
   for(i = 0; i < n; i++)
   {  typename Pattern_type::const_iterator itr(s, i);
      j = *itr;
      while( j != s.end() )
      {  row.push_back(i);
         col.push_back(j);
         k++;
         j = *(++itr);
      }
   }
   size_t K = k;
   BaseVector H(K);

   // now we have folded this into the following case
   SparseHessianCompute(x, w, s, row, col, H, work);

   // now set the non-zero return values
   for(k = 0; k < K; k++)
      hes[ row[k] * n + col[k] ] = H[k];

   return hes;
}
/*!
Compute a sparse Hessian

The C++ source code coresponding to this operation is
\verbatim
   hes = SparseHessian(x, w)
\endverbatim


\tparam Base
is the base type for the recording that is stored in this
ADFun<Base object.

\tparam BaseVector
is a simple vector class with elements of the Base.

\param x [in]
is a vector specifing the point at which to compute the Hessian.

\param w [in]
The Hessian is computed for a weighted sum of the components
of the function corresponding to this ADFun<Base> object.
The argument w specifies the weights for each component.
It must have size equal to the range dimension for this ADFun<Base> object.

\return
Will be a vector of size n * n containing the Hessian of
at the point specified by x
(where n is the domain dimension for this ADFun<Base> object).
*/
template <class Base, class RecBase>
template <class BaseVector>
BaseVector ADFun<Base,RecBase>::SparseHessian(const BaseVector &x, const BaseVector &w)
{  size_t i, j, k;
   typedef CppAD::vectorBool BoolVector;

   size_t m = Range();
   size_t n = Domain();

   // determine the sparsity pattern p for Hessian of w^T F
   BoolVector r(n * n);
   for(j = 0; j < n; j++)
   {  for(k = 0; k < n; k++)
         r[j * n + k] = false;
      r[j * n + j] = true;
   }
   ForSparseJac(n, r);
   //
   BoolVector s(m);
   for(i = 0; i < m; i++)
      s[i] = w[i] != 0;
   BoolVector p = RevSparseHes(n, s);

   // compute sparse Hessian
   return SparseHessian(x, w, p);
}

} // END_CPPAD_NAMESPACE
# endif
