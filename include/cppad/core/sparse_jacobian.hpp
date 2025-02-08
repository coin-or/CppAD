# ifndef CPPAD_CORE_SPARSE_JACOBIAN_HPP
# define CPPAD_CORE_SPARSE_JACOBIAN_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

// maximum number of sparse directions to compute at the same time

// # define CPPAD_SPARSE_JACOBIAN_MAX_MULTIPLE_DIRECTION 1
# define CPPAD_SPARSE_JACOBIAN_MAX_MULTIPLE_DIRECTION 64

/*
{xrst_begin sparse_jacobian}
{xrst_spell
   valarray
}

Sparse Jacobian
###############

Syntax
******
| *jac* = *f* . ``SparseJacobian`` ( *x* )
| *jac* = *f* . ``SparseJacobian`` ( *x* , *p* )
| *n_sweep* = *f* . ``SparseJacobianForward`` ( *x* , *p* , *row* , *col* , *jac* , *work* )
| *n_sweep* = *f* . ``SparseJacobianReverse`` ( *x* , *p* , *row* , *col* , *jac* , *work* )

Purpose
*******
We use :math:`n` for the :ref:`fun_property@Domain` size,
and :math:`m` for the :ref:`fun_property@Range` size of *f* .
We use :math:`F : \B{R}^n \rightarrow \B{R}^m` do denote the
:ref:`glossary@AD Function`
corresponding to *f* .
The syntax above sets *jac* to the Jacobian

.. math::

   jac = F^{(1)} (x)

This routine takes advantage of the sparsity of the Jacobian
in order to reduce the amount of computation necessary.
If *row* and *col* are present, it also takes
advantage of the reduced set of elements of the Jacobian that
need to be computed.
One can use speed tests (e.g. :ref:`speed_test-name` )
to verify that results are computed faster
than when using the routine :ref:`Jacobian-name` .

f
*
The object *f* has prototype

   ``ADFun`` < *Base* > *f*

Note that the :ref:`ADFun-name` object *f* is not ``const``
(see :ref:`sparse_jacobian@Uses Forward` below).

x
*
The argument *x* has prototype

   ``const`` *BaseVector* & *x*

(see :ref:`sparse_jacobian@BaseVector` below)
and its size
must be equal to *n* , the dimension of the
:ref:`fun_property@Domain` space for *f* .
It specifies
that point at which to evaluate the Jacobian.

p
*
The argument *p* is optional and has prototype

   ``const`` *SetVector* & *p*

(see :ref:`sparse_jacobian@SetVector` below).
If it has elements of type ``bool`` ,
its size is :math:`m * n`.
If it has elements of type ``std::set<size_t>`` ,
its size is :math:`m` and all its set elements are between
zero and :math:`n - 1`.
It specifies a
:ref:`glossary@Sparsity Pattern`
for the Jacobian :math:`F^{(1)} (x)`.

If this sparsity pattern does not change between calls to
``SparseJacobian`` , it should be faster to calculate *p* once
(using :ref:`ForSparseJac-name` or :ref:`RevSparseJac-name` )
and then pass *p* to ``SparseJacobian`` .
Furthermore, if you specify *work* in the calling sequence,
it is not necessary to keep the sparsity pattern; see the heading
:ref:`sparse_jacobian@work@p` under the *work* description.

In addition,
if you specify *p* , CppAD will use the same
type of sparsity representation
(vectors of ``bool`` or vectors of ``std::set<size_t>`` )
for its internal calculations.
Otherwise, the representation
for the internal calculations is unspecified.

row, col
********
The arguments *row* and *col* are optional and have prototype

| |tab| ``const`` *SizeVector* & *row*
| |tab| ``const`` *SizeVector* & *col*

(see :ref:`sparse_jacobian@SizeVector` below).
They specify which rows and columns of :math:`F^{(1)} (x)` are
computes and in what order.
Not all the non-zero entries in :math:`F^{(1)} (x)` need be computed,
but all the entries specified by *row* and *col*
must be possibly non-zero in the sparsity pattern.
We use :math:`K` to denote the value *jac* . ``size`` ()
which must also equal the size of *row* and *col* .
Furthermore,
for :math:`k = 0 , \ldots , K-1`, it must hold that
:math:`row[k] < m` and :math:`col[k] < n`.

jac
***
The result *jac* has prototype

   *BaseVector* & *jac*

In the case where the arguments *row* and *col* are not present,
the size of *jac* is :math:`m * n` and
for :math:`i = 0 , \ldots , m-1`,
:math:`j = 0 , \ldots , n-1`,

.. math::

   jac [ i * n + j ] = \D{ F_i }{ x_j } (x)

In the case where the arguments *row* and *col* are present,
we use :math:`K` to denote the size of *jac* .
The input value of its elements does not matter.
Upon return, for :math:`k = 0 , \ldots , K - 1`,

.. math::

   jac [ k ] = \D{ F_i }{ x_j } (x)
   \; , \;
   \; {\rm where} \;
   i = row[k]
   \; {\rm and } \;
   j = col[k]

work
****
If this argument is present, it has prototype

   ``sparse_jacobian_work&`` *work*

This object can only be used with the routines
``SparseJacobianForward`` and ``SparseJacobianReverse`` .
During its the first use, information is stored in *work* .
This is used to reduce the work done by future calls to the same mode
(forward or reverse),
the same *f* , *p* , *row* , and *col* .
If a future call is for a different mode,
or any of these values have changed,
you must first call *work* . ``clear`` ()
to inform CppAD that this information needs to be recomputed.

color_method
============
The coloring algorithm determines which columns (forward mode)
or rows (reverse mode) can be computed during the same sweep.
This field has prototype

   ``std::string`` *work* . ``color_method``

and its default value (after a constructor or ``clear()`` )
is ``"cppad"`` .
If :ref:`colpack_prefix-name` is specified on the
:ref:`cmake@CMake Command` line,
you can set this method to ``"colpack"`` .
This value only matters on the first call to ``sparse_jacobian``
that follows the *work* constructor or a call to
*work* . ``clear`` () .

p
=
If *work* is present, and it is not the first call after
its construction or a clear,
the sparsity pattern *p* is not used.
This enables one to free the sparsity pattern
and still compute corresponding sparse Jacobians.

n_sweep
*******
The return value *n_sweep* has prototype

   ``size_t`` *n_sweep*

If ``SparseJacobianForward`` (``SparseJacobianReverse`` ) is used,
*n_sweep* is the number of first order forward (reverse) sweeps
used to compute the requested Jacobian values.
(This is also the number of colors determined by the coloring method
mentioned above).
This is proportional to the total work that ``SparseJacobian`` does,
not counting the zero order forward sweep,
or the work to combine multiple columns (rows) into a single sweep.

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
After a call to any of the sparse Jacobian routines,
the zero order Taylor coefficients correspond to
*f* . ``Forward`` (0, *x* )
and the other coefficients are unspecified.

After ``SparseJacobian`` ,
the previous calls to :ref:`Forward-name` are undefined.

Example
*******
{xrst_toc_hidden
   example/sparse/sparse_jacobian.cpp
}
The routine
:ref:`sparse_jacobian.cpp-name`
is examples and tests of ``sparse_jacobian`` .
It return ``true`` , if it succeeds and ``false`` otherwise.

{xrst_end sparse_jacobian}
==============================================================================
*/
# include <cppad/local/std_set.hpp>
# include <cppad/local/color_general.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file sparse_jacobian.hpp
Sparse Jacobian driver routine and helper functions.
*/
// ===========================================================================
/*!
class used by SparseJacobian to hold information so it does not need to be
recomputed.
*/
class sparse_jacobian_work {
   public:
      /// Coloring method: "cppad", or "colpack"
      /// (this field is set by user)
      std::string color_method;
      /// indices that sort the user row and col arrays by color
      CppAD::vector<size_t> order;
      /// results of the coloring algorithm
      CppAD::vector<size_t> color;

      /// constructor
      sparse_jacobian_work(void) : color_method("cppad")
      { }
      /// reset coloring method to its default and
      /// inform CppAD that color and order need to be recomputed
      void clear(void)
      {  color_method = "cppad";
         order.clear();
         color.clear();
      }
};
// ===========================================================================
/*!
Private helper function forward mode cases

\tparam Base
is the base type for the recording that is stored in this
<code>ADFun<Base></code> object.

\tparam BaseVector
is a simple vector class with elements of type Base.

\tparam SetVector
is either sparse_pack or sparse_list.

\tparam SizeVector
is a simple vector class with elements of type size_t.

\param x [in]
is a vector specifing the point at which to compute the Jacobian.

\param p_transpose [in]
If <code>work.color.size() != 0</code>,
then p_transpose is not used.
Otherwise, it is a
sparsity pattern for the transpose of the Jacobian of this ADFun<Base> object.
Note that we do not change the values in p_transpose,
but is not const because we use its iterator facility.

\param row [in]
is the vector of row indices for the returned Jacobian values.

\param col [in]
is the vector of columns indices for the returned Jacobian values.
It must have the same size as row.

\param jac [out]
is the vector of Jacobian values. We use K to denote the size of jac.
The return value <code>jac[k]</code> is the partial of the
<code>row[k]</code> range component of the function with respect
the the <code>col[k]</code> domain component of its argument.

\param work
<code>work.color_method</code> is an input. The rest of
this structure contains information that is computed by SparseJacobainFor.
If the sparsity pattern, row vector, or col vectors
are not the same between calls to SparseJacobianFor,
 work.clear() must be called to reinitialize work.

\return
Is the number of first order forward sweeps used to compute the
requested Jacobian values. The total work, not counting the zero order
forward sweep, or the time to combine computations, is proportional to this
return value.
*/
template <class Base, class RecBase>
template <class BaseVector, class SetVector, class SizeVector>
size_t ADFun<Base,RecBase>::SparseJacobianFor(
   const BaseVector&            x           ,
          SetVector&             p_transpose ,
   const SizeVector&            row         ,
   const SizeVector&            col         ,
          BaseVector&            jac         ,
           sparse_jacobian_work& work        )
{
   size_t j, k, ell;

   CppAD::vector<size_t>& order(work.order);
   CppAD::vector<size_t>& color(work.color);

   size_t m = Range();
   size_t n = Domain();

   // some values
   const Base zero(0);
   const Base one(1);

   // check BaseVector is Simple Vector class with Base type elements
   CheckSimpleVector<Base, BaseVector>();

   CPPAD_ASSERT_UNKNOWN( size_t(x.size()) == n );
   CPPAD_ASSERT_UNKNOWN( color.size() == 0 || color.size() == n );

   // number of components of Jacobian that are required
   size_t K = size_t(jac.size());
   CPPAD_ASSERT_UNKNOWN( size_t( row.size() ) == K );
   CPPAD_ASSERT_UNKNOWN( size_t( col.size() ) == K );

   // Point at which we are evaluating the Jacobian
   Forward(0, x);

   // check for case where nothing (except Forward above) to do
   if( K == 0 )
      return 0;

   if( color.size() == 0 )
   {
      CPPAD_ASSERT_UNKNOWN( p_transpose.n_set() ==  n );
      CPPAD_ASSERT_UNKNOWN( p_transpose.end() ==  m );

      // execute coloring algorithm
      color.resize(n);
      if( work.color_method == "cppad" )
         local::color_general_cppad(p_transpose, col, row, color);
      else if( work.color_method == "colpack" )
      {
# if CPPAD_HAS_COLPACK
         local::color_general_colpack(p_transpose, col, row, color);
# else
         CPPAD_ASSERT_KNOWN(
            false,
            "SparseJacobianForward: work.color_method = colpack "
            "and colpack_prefix missing from cmake command line."
         );
# endif
      }
      else CPPAD_ASSERT_KNOWN(
         false,
         "SparseJacobianForward: work.color_method is not valid."
      );

      // put sorting indices in color order
      SizeVector key(K);
      order.resize(K);
      for(k = 0; k < K; k++)
         key[k] = color[ col[k] ];
      index_sort(key, order);
   }
   size_t n_color = 1;
   for(j = 0; j < n; j++) if( color[j] < n )
      n_color = std::max<size_t>(n_color, color[j] + 1);

   // initialize the return value
   for(k = 0; k < K; k++)
      jac[k] = zero;

# if CPPAD_SPARSE_JACOBIAN_MAX_MULTIPLE_DIRECTION == 1
   // direction vector and return values for calls to forward
   BaseVector dx(n), dy(m);

   // loop over colors
   k = 0;
   for(ell = 0; ell < n_color; ell++)
   {  CPPAD_ASSERT_UNKNOWN( color[ col[ order[k] ] ] == ell );

      // combine all columns with this color
      for(j = 0; j < n; j++)
      {  dx[j] = zero;
         if( color[j] == ell )
            dx[j] = one;
      }
      // call forward mode for all these columns at once
      dy = Forward(1, dx);

      // set the corresponding components of the result
      while( k < K && color[ col[order[k]] ] == ell )
      {  jac[ order[k] ] = dy[row[order[k]]];
         k++;
      }
   }
# else
   // abbreviation for this value
   size_t max_r = CPPAD_SPARSE_JACOBIAN_MAX_MULTIPLE_DIRECTION;
   CPPAD_ASSERT_UNKNOWN( max_r > 1 );

   // count the number of colors done so far
   size_t count_color = 0;
   // count the sparse matrix entries done so far
   k = 0;
   while( count_color < n_color )
   {  // number of colors we will do this time
      size_t r = std::min<size_t>(max_r , n_color - count_color);
      BaseVector dx(n * r), dy(m * r);

      // loop over colors we will do this tme
      for(ell = 0; ell < r; ell++)
      {  // combine all columns with this color
         for(j = 0; j < n; j++)
         {  dx[j * r + ell] = zero;
            if( color[j] == ell + count_color )
               dx[j * r + ell] = one;
         }
      }
      size_t q           = 1;
      dy = Forward(q, r, dx);

      // store results
      for(ell = 0; ell < r; ell++)
      {  // set the components of the result for this color
         while( k < K && color[ col[order[k]] ] == ell + count_color )
         {  jac[ order[k] ] = dy[ row[order[k]] * r + ell ];
            k++;
         }
      }
      count_color += r;
   }
# endif
   return n_color;
}
/*!
Private helper function for reverse mode cases.

\tparam Base
is the base type for the recording that is stored in this
<code>ADFun<Base></code> object.

\tparam BaseVector
is a simple vector class with elements of type Base.

\tparam SetVector
is either sparse_pack or sparse_list.

\tparam SizeVector
is a simple vector class with elements of type size_t.

\param x [in]
is a vector specifing the point at which to compute the Jacobian.

\param p [in]
If <code>work.color.size() != 0</code>, then p is not used.
Otherwise, it is a
sparsity pattern for the Jacobian of this ADFun<Base> object.
Note that we do not change the values in p,
but is not const because we use its iterator facility.

\param row [in]
is the vector of row indices for the returned Jacobian values.

\param col [in]
is the vector of columns indices for the returned Jacobian values.
It must have the same size as row.

\param jac [out]
is the vector of Jacobian values.
It must have the same size as row.
The return value <code>jac[k]</code> is the partial of the
<code>row[k]</code> range component of the function with respect
the the <code>col[k]</code> domain component of its argument.

\param work
<code>work.color_method</code> is an input. The rest of
This structure contains information that is computed by SparseJacobainRev.
If the sparsity pattern, row vector, or col vectors
are not the same between calls to SparseJacobianRev,
 work.clear() must be called to reinitialize work.

\return
Is the number of first order reverse sweeps used to compute the
reverse Jacobian values. The total work, not counting the zero order
forward sweep, or the time to combine computations, is proportional to this
return value.
*/
template <class Base, class RecBase>
template <class BaseVector, class SetVector, class SizeVector>
size_t ADFun<Base,RecBase>::SparseJacobianRev(
   const BaseVector&           x           ,
          SetVector&            p           ,
   const SizeVector&           row         ,
   const SizeVector&           col         ,
          BaseVector&           jac         ,
          sparse_jacobian_work& work        )
{
   size_t i, k, ell;

   CppAD::vector<size_t>& order(work.order);
   CppAD::vector<size_t>& color(work.color);

   size_t m = Range();
   size_t n = Domain();

   // some values
   const Base zero(0);
   const Base one(1);

   // check BaseVector is Simple Vector class with Base type elements
   CheckSimpleVector<Base, BaseVector>();

   CPPAD_ASSERT_UNKNOWN( size_t(x.size()) == n );
   CPPAD_ASSERT_UNKNOWN (color.size() == m || color.size() == 0 );

   // number of components of Jacobian that are required
   size_t K = size_t(jac.size());
   CPPAD_ASSERT_UNKNOWN( size_t( size_t( row.size() ) ) == K );
   CPPAD_ASSERT_UNKNOWN( size_t( size_t( col.size() ) ) == K );

   // Point at which we are evaluating the Jacobian
   Forward(0, x);

   // check for case where nothing (except Forward above) to do
   if( K == 0 )
      return 0;

   if( color.size() == 0 )
   {
      CPPAD_ASSERT_UNKNOWN( p.n_set() == m );
      CPPAD_ASSERT_UNKNOWN( p.end()   == n );

      // execute the coloring algorithm
      color.resize(m);
      if( work.color_method == "cppad" )
         local::color_general_cppad(p, row, col, color);
      else if( work.color_method == "colpack" )
      {
# if CPPAD_HAS_COLPACK
         local::color_general_colpack(p, row, col, color);
# else
         CPPAD_ASSERT_KNOWN(
            false,
            "SparseJacobianReverse: work.color_method = colpack "
            "and colpack_prefix missing from cmake command line."
         );
# endif
      }
      else CPPAD_ASSERT_KNOWN(
         false,
         "SparseJacobianReverse: work.color_method is not valid."
      );

      // put sorting indices in color order
      SizeVector key(K);
      order.resize(K);
      for(k = 0; k < K; k++)
         key[k] = color[ row[k] ];
      index_sort(key, order);
   }
   size_t n_color = 1;
   for(i = 0; i < m; i++) if( color[i] < m )
      n_color = std::max<size_t>(n_color, color[i] + 1);

   // weighting vector for calls to reverse
   BaseVector w(m);

   // location for return values from Reverse
   BaseVector dw(n);

   // initialize the return value
   for(k = 0; k < K; k++)
      jac[k] = zero;

   // loop over colors
   k = 0;
   for(ell = 0; ell < n_color; ell++)
   {  CPPAD_ASSERT_UNKNOWN( color[ row[ order[k] ] ] == ell );

      // combine all the rows with this color
      for(i = 0; i < m; i++)
      {  w[i] = zero;
         if( color[i] == ell )
            w[i] = one;
      }
      // call reverse mode for all these rows at once
      dw = Reverse(1, w);

      // set the corresponding components of the result
      while( k < K && color[ row[order[k]] ]  == ell )
      {  jac[ order[k] ] = dw[col[order[k]]];
         k++;
      }
   }
   return n_color;
}
// ==========================================================================
// Public Member functions
// ==========================================================================
/*!
Compute user specified subset of a sparse Jacobian using forward mode.

The C++ source code corresponding to this operation is
\verbatim
   SparceJacobianForward(x, p, row, col, jac, work)
\endverbatim

\tparam Base
is the base type for the recording that is stored in this
<code>ADFun<Base></code> object.

\tparam BaseVector
is a simple vector class with elements of type Base.

\tparam SetVector
is a simple vector class with elements of type
 bool or std::set<size_t>.

\tparam SizeVector
is a simple vector class with elements of type size_t.

\param x [in]
is a vector specifing the point at which to compute the Jacobian.

\param p [in]
is the sparsity pattern for the Jacobian that we are calculating.

\param row [in]
is the vector of row indices for the returned Jacobian values.

\param col [in]
is the vector of columns indices for the returned Jacobian values.
It must have the same size as row.

\param jac [out]
is the vector of Jacobian values.
It must have the same size as row.
The return value <code>jac[k]</code> is the partial of the
<code>row[k]</code> range component of the function with respect
the the <code>col[k]</code> domain component of its argument.

\param work [in,out]
this structure contains information that depends on the function object,
sparsity pattern, row vector, and col vector.
If they are not the same between calls to SparseJacobianForward,
 work.clear() must be called to reinitialize them.

\return
Is the number of first order forward sweeps used to compute the
requested Jacobian values. The total work, not counting the zero order
forward sweep, or the time to combine computations, is proportional to this
return value.
*/
template <class Base, class RecBase>
template <class BaseVector, class SetVector, class SizeVector>
size_t ADFun<Base,RecBase>::SparseJacobianForward(
   const BaseVector&     x    ,
   const SetVector&      p    ,
   const SizeVector&     row  ,
   const SizeVector&     col  ,
   BaseVector&           jac  ,
   sparse_jacobian_work& work )
{
   size_t n = Domain();
   size_t m = Range();
   size_t K = jac.size();
# ifndef NDEBUG
   size_t k;
   CPPAD_ASSERT_KNOWN(
      size_t(x.size()) == n ,
      "SparseJacobianForward: size of x not equal domain dimension for f."
   );
   CPPAD_ASSERT_KNOWN(
      size_t(row.size()) == K && size_t(col.size()) == K ,
      "SparseJacobianForward: either r or c does not have "
      "the same size as jac."
   );
   CPPAD_ASSERT_KNOWN(
      work.color.size() == 0 || work.color.size() == n,
      "SparseJacobianForward: invalid value in work."
   );
   for(k = 0; k < K; k++)
   {  CPPAD_ASSERT_KNOWN(
         row[k] < m,
         "SparseJacobianForward: invalid value in r."
      );
      CPPAD_ASSERT_KNOWN(
         col[k] < n,
         "SparseJacobianForward: invalid value in c."
      );
   }
   if( work.color.size() != 0 )
      for(size_t j = 0; j < n; j++) CPPAD_ASSERT_KNOWN(
         work.color[j] <= n,
         "SparseJacobianForward: invalid value in work."
   );
# endif
   // check for case where there is nothing to compute
   size_t n_sweep = 0;
   if( K == 0 )
      return n_sweep;

   typedef typename SetVector::value_type Set_type;
   typedef typename local::sparse::internal_pattern<Set_type>::pattern_type Pattern_type;
   Pattern_type s_transpose;
   if( work.color.size() == 0 )
   {  bool transpose = true;
      const char* error_msg = "SparseJacobianForward: transposed sparsity"
      " pattern does not have proper row or column dimension";
      sparsity_user2internal(s_transpose, p, n, m, transpose, error_msg);
   }
   n_sweep = SparseJacobianFor(x, s_transpose, row, col, jac, work);
   return n_sweep;
}
/*!
Compute user specified subset of a sparse Jacobian using forward mode.

The C++ source code corresponding to this operation is
\verbatim
   SparceJacobianReverse(x, p, row, col, jac, work)
\endverbatim

\tparam Base
is the base type for the recording that is stored in this
<code>ADFun<Base></code> object.

\tparam BaseVector
is a simple vector class with elements of type Base.

\tparam SetVector
is a simple vector class with elements of type
 bool or std::set<size_t>.

\tparam SizeVector
is a simple vector class with elements of type size_t.

\param x [in]
is a vector specifing the point at which to compute the Jacobian.

\param p [in]
is the sparsity pattern for the Jacobian that we are calculating.

\param row [in]
is the vector of row indices for the returned Jacobian values.

\param col [in]
is the vector of columns indices for the returned Jacobian values.
It must have the same size as row.

\param jac [out]
is the vector of Jacobian values.
It must have the same size as row.
The return value <code>jac[k]</code> is the partial of the
<code>row[k]</code> range component of the function with respect
the the <code>col[k]</code> domain component of its argument.

\param work [in,out]
this structure contains information that depends on the function object,
sparsity pattern, row vector, and col vector.
If they are not the same between calls to SparseJacobianReverse,
 work.clear() must be called to reinitialize them.

\return
Is the number of first order reverse sweeps used to compute the
reverse Jacobian values. The total work, not counting the zero order
forward sweep, or the time to combine computations, is proportional to this
return value.
*/
template <class Base, class RecBase>
template <class BaseVector, class SetVector, class SizeVector>
size_t ADFun<Base,RecBase>::SparseJacobianReverse(
   const BaseVector&     x    ,
   const SetVector&      p    ,
   const SizeVector&     row  ,
   const SizeVector&     col  ,
   BaseVector&           jac  ,
   sparse_jacobian_work& work )
{
   size_t m = Range();
   size_t n = Domain();
   size_t K = jac.size();
# ifndef NDEBUG
   size_t k;
   CPPAD_ASSERT_KNOWN(
      size_t(x.size()) == n ,
      "SparseJacobianReverse: size of x not equal domain dimension for f."
   );
   CPPAD_ASSERT_KNOWN(
      size_t(row.size()) == K && size_t(col.size()) == K ,
      "SparseJacobianReverse: either r or c does not have "
      "the same size as jac."
   );
   CPPAD_ASSERT_KNOWN(
      work.color.size() == 0 || work.color.size() == m,
      "SparseJacobianReverse: invalid value in work."
   );
   for(k = 0; k < K; k++)
   {  CPPAD_ASSERT_KNOWN(
         row[k] < m,
         "SparseJacobianReverse: invalid value in r."
      );
      CPPAD_ASSERT_KNOWN(
         col[k] < n,
         "SparseJacobianReverse: invalid value in c."
      );
   }
   if( work.color.size() != 0 )
      for(size_t i = 0; i < m; i++) CPPAD_ASSERT_KNOWN(
         work.color[i] <= m,
         "SparseJacobianReverse: invalid value in work."
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
      const char* error_msg = "SparseJacobianReverse: sparsity"
      " pattern does not have proper row or column dimension";
      sparsity_user2internal(s, p, m, n, transpose, error_msg);
   }
   n_sweep = SparseJacobianRev(x, s, row, col, jac, work);
   return n_sweep;
}
/*!
Compute a sparse Jacobian.

The C++ source code corresponding to this operation is
\verbatim
   jac = SparseJacobian(x, p)
\endverbatim

\tparam Base
is the base type for the recording that is stored in this
<code>ADFun<Base></code> object.

\tparam BaseVector
is a simple vector class with elements of type Base.

\tparam SetVector
is a simple vector class with elements of type
 bool or std::set<size_t>.

\param x [in]
is a vector specifing the point at which to compute the Jacobian.

\param p [in]
is the sparsity pattern for the Jacobian that we are calculating.

\return
Will be a vector if size m * n containing the Jacobian at the
specified point (in row major order).
*/
template <class Base, class RecBase>
template <class BaseVector, class SetVector>
BaseVector ADFun<Base,RecBase>::SparseJacobian(
   const BaseVector& x, const SetVector& p
)
{  size_t i, j, k;

   size_t m = Range();
   size_t n = Domain();
   BaseVector jac(m * n);

   CPPAD_ASSERT_KNOWN(
      size_t(x.size()) == n,
      "SparseJacobian: size of x not equal domain size for f."
   );
   CheckSimpleVector<Base, BaseVector>();

   typedef typename SetVector::value_type Set_type;
   typedef typename local::sparse::internal_pattern<Set_type>::pattern_type Pattern_type;

   // initialize the return value as zero
   Base zero(0);
   for(i = 0; i < m; i++)
      for(j = 0; j < n; j++)
         jac[i * n + j] = zero;

   sparse_jacobian_work work;
   CppAD::vector<size_t> row;
   CppAD::vector<size_t> col;
   if( n <= m )
   {
      // need an internal copy of sparsity pattern
      Pattern_type s_transpose;
      bool transpose = true;
      const char* error_msg = "SparseJacobian: transposed sparsity"
      " pattern does not have proper row or column dimension";
      sparsity_user2internal(s_transpose, p, n, m, transpose, error_msg);

      k = 0;
      for(j = 0; j < n; j++)
      {  typename Pattern_type::const_iterator itr(s_transpose, j);
         i = *itr;
         while( i != s_transpose.end() )
         {  row.push_back(i);
            col.push_back(j);
            k++;
            i = *(++itr);
         }
      }
      size_t K = k;
      BaseVector J(K);

      // now we have folded this into the following case
      SparseJacobianFor(x, s_transpose, row, col, J, work);

      // now set the non-zero return values
      for(k = 0; k < K; k++)
         jac[ row[k] * n + col[k] ] = J[k];
   }
   else
   {
      // need an internal copy of sparsity pattern
      Pattern_type s;
      bool transpose = false;
      const char* error_msg = "SparseJacobian: sparsity"
      " pattern does not have proper row or column dimension";
      sparsity_user2internal(s, p, m, n, transpose, error_msg);

      k = 0;
      for(i = 0; i < m; i++)
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
      BaseVector J(K);

      // now we have folded this into the following case
      SparseJacobianRev(x, s, row, col, J, work);

      // now set the non-zero return values
      for(k = 0; k < K; k++)
         jac[ row[k] * n + col[k] ] = J[k];
   }

   return jac;
}

/*!
Compute a sparse Jacobian.

The C++ source code corresponding to this operation is
\verbatim
   jac = SparseJacobian(x)
\endverbatim

\tparam Base
is the base type for the recording that is stored in this
<code>ADFun<Base></code> object.

\tparam BaseVector
is a simple vector class with elements of the Base.

\param x [in]
is a vector specifing the point at which to compute the Jacobian.

\return
Will be a vector of size m * n containing the Jacobian at the
specified point (in row major order).
*/
template <class Base, class RecBase>
template <class BaseVector>
BaseVector ADFun<Base,RecBase>::SparseJacobian( const BaseVector& x )
{  typedef CppAD::vectorBool   BoolVector;

   size_t m = Range();
   size_t n = Domain();

   // sparsity pattern for Jacobian
   BoolVector p(m * n);

   if( n <= m )
   {  size_t j, k;

      // use forward mode
      BoolVector r(n * n);
      for(j = 0; j < n; j++)
      {  for(k = 0; k < n; k++)
            r[j * n + k] = false;
         r[j * n + j] = true;
      }
      p = ForSparseJac(n, r);
   }
   else
   {  size_t i, k;

      // use reverse mode
      BoolVector s(m * m);
      for(i = 0; i < m; i++)
      {  for(k = 0; k < m; k++)
            s[i * m + k] = false;
         s[i * m + i] = true;
      }
      p = RevSparseJac(m, s);
   }
   return SparseJacobian(x, p);
}

} // END_CPPAD_NAMESPACE
# undef CPPAD_SPARSE_JACOBIAN_MAX_MULTIPLE_DIRECTION
# endif
