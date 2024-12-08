# ifndef CPPAD_CORE_REV_SPARSE_HES_HPP
# define CPPAD_CORE_REV_SPARSE_HES_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin RevSparseHes}

Hessian Sparsity Pattern: Reverse Mode
######################################

Syntax
******
| *h* = *f* . ``RevSparseHes`` ( *q* , *s* )
| *h* = *f* . ``RevSparseHes`` ( *q* , *s* , *transpose* )

Purpose
*******
We use :math:`F : \B{R}^n \rightarrow \B{R}^m` to denote the
:ref:`glossary@AD Function` corresponding to *f* .
For a fixed matrix :math:`R \in \B{R}^{n \times q}`
and a fixed vector :math:`S \in \B{R}^{1 \times m}`,
we define

.. math::
   :nowrap:

   \begin{eqnarray}
   H(x)
   & = & \partial_x \left[ \partial_u S * F[ x + R * u ] \right]_{u=0}
   \\
   & = & R^\R{T} * (S * F)^{(2)} ( x )
   \\
   H(x)^\R{T}
   & = & (S * F)^{(2)} ( x ) * R
   \end{eqnarray}

Given a
:ref:`glossary@Sparsity Pattern`
for the matrix :math:`R` and the vector :math:`S`,
``RevSparseHes`` returns a sparsity pattern for the :math:`H(x)`.

f
*
The object *f* has prototype

   ``const ADFun`` < *Base* > *f*

x
*
If the operation sequence in *f* is
:ref:`glossary@Operation@Independent` of
the independent variables in :math:`x \in \B{R}^n`,
the sparsity pattern is valid for all values of
(even if it has :ref:`CondExp-name` or :ref:`VecAD-name` operations).

q
*
The argument *q* has prototype

   ``size_t`` *q*

It specifies the number of columns in :math:`R \in \B{R}^{n \times q}`
and the number of rows in :math:`H(x) \in \B{R}^{q \times n}`.
It must be the same value as in the previous :ref:`ForSparseJac-name` call

   *f* . ``ForSparseJac`` ( *q* , *r* , *r_transpose* )

Note that if *r_transpose* is true, *r* in the call above
corresponding to :math:`R^\R{T} \in \B{R}^{q \times n}`

transpose
*********
The argument *transpose* has prototype

   ``bool`` *transpose*

The default value ``false`` is used when *transpose* is not present.

r
*
The matrix :math:`R` is specified by the previous call

   *f* . ``ForSparseJac`` ( *q* , *r* , *transpose* )

see :ref:`ForSparseJac@r` .
The type of the elements of
:ref:`RevSparseHes@SetVector` must be the
same as the type of the elements of *r* .

s
*
The argument *s* has prototype

   ``const`` *SetVector* & *s*

(see :ref:`RevSparseHes@SetVector` below)
If it has elements of type ``bool`` ,
its size is :math:`m`.
If it has elements of type ``std::set<size_t>`` ,
its size is one and all the elements of *s* [0]
are between zero and :math:`m - 1`.
It specifies a
:ref:`glossary@Sparsity Pattern`
for the vector *S* .

h
*
The result *h* has prototype

   *SetVector* & *h*

(see :ref:`RevSparseHes@SetVector` below).

transpose false
===============
If *h* has elements of type ``bool`` ,
its size is :math:`q * n`.
If it has elements of type ``std::set<size_t>`` ,
its size is :math:`q` and all the set elements are between
zero and *n* ``-1`` inclusive.
It specifies a
:ref:`glossary@Sparsity Pattern`
for the matrix :math:`H(x)`.

transpose true
==============
If *h* has elements of type ``bool`` ,
its size is :math:`n * q`.
If it has elements of type ``std::set<size_t>`` ,
its size is :math:`n` and all the set elements are between
zero and *q* ``-1`` inclusive.
It specifies a
:ref:`glossary@Sparsity Pattern`
for the matrix :math:`H(x)^\R{T}`.

SetVector
*********
The type *SetVector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
``bool`` or ``std::set<size_t>`` ;
see :ref:`glossary@Sparsity Pattern` for a discussion
of the difference.
The type of the elements of
:ref:`RevSparseHes@SetVector` must be the
same as the type of the elements of *r* .

Entire Sparsity Pattern
***********************
Suppose that :math:`q = n` and
:math:`R \in \B{R}^{n \times n}` is the :math:`n \times n` identity matrix.
Further suppose that the :math:`S` is the *k*-th
:ref:`glossary@Elementary Vector` ; i.e.

.. math::

   S_j = \left\{ \begin{array}{ll}
      1  & {\rm if} \; j = k
      \\
      0  & {\rm otherwise}
   \end{array} \right.

In this case,
the corresponding value *h* is a
sparsity pattern for the Hessian matrix
:math:`F_k^{(2)} (x) \in \B{R}^{n \times n}`.

Example
*******
{xrst_toc_hidden
   example/sparse/rev_sparse_hes.cpp
   example/sparse/sparsity_sub.cpp
}
The file
:ref:`rev_sparse_hes.cpp-name`
contains an example and test of this operation.
The file
:ref:`sparsity_sub.cpp<sparsity_sub.cpp@RevSparseHes>`
contains an example and test of using ``RevSparseHes``
to compute the sparsity pattern for a subset of the Hessian.

{xrst_end RevSparseHes}
-----------------------------------------------------------------------------
*/
# include <algorithm>
# include <cppad/local/pod_vector.hpp>
# include <cppad/local/std_set.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file core/rev_sparse_hes.hpp
Reverse mode Hessian sparsity patterns.
*/
// ===========================================================================
// RevSparseHesCase
/*!
Private helper function for RevSparseHes(q, s) bool sparsity.

All of the description in the public member function RevSparseHes(q, s)
applies.

\param set_type
is a bool value. This argument is used to dispatch to the proper source
code depending on the vlaue of SetVector::value_type.

\param transpose
See RevSparseHes(q, s).

\param q
See RevSparseHes(q, s).

\param s
See RevSparseHes(q, s).

\param h
is the return value for the corresponging call to RevSparseJac(q, s).
*/
template <class Base, class RecBase>
template <class SetVector>
void ADFun<Base,RecBase>::RevSparseHesCase(
   bool              set_type         ,
   bool              transpose        ,
   size_t            q                ,
   const SetVector&  s                ,
   SetVector&        h                )
{
   // used to identify the RecBase type in calls to sweeps
   RecBase not_used_rec_base(0.0);
   //
   size_t n = Domain();
   size_t m = Range();
   //
   h.resize(q * n );

   CPPAD_ASSERT_KNOWN(
      for_jac_sparse_pack_.n_set() > 0,
      "RevSparseHes: previous stored call to ForSparseJac did not "
      "use bool for the elements of r."
   );
   CPPAD_ASSERT_UNKNOWN( for_jac_sparse_set_.n_set() == 0 );
   CPPAD_ASSERT_UNKNOWN( for_jac_sparse_pack_.n_set() == num_var_tape_  );
   //
   // temporary indices
   size_t i, j;

   // check Vector is Simple SetVector class with bool elements
   CheckSimpleVector<bool, SetVector>();

   CPPAD_ASSERT_KNOWN(
      q == for_jac_sparse_pack_.end(),
      "RevSparseHes: q is not equal to its value\n"
      "in the previous call to ForSparseJac with this ADFun object."
   );
   CPPAD_ASSERT_KNOWN(
      size_t(s.size()) == m,
      "RevSparseHes: size of s is not equal to\n"
      "range dimension for ADFun object."
   );

   // Array that will hold reverse Jacobian dependency flag.
   // Initialize as true for the dependent variables.
   local::pod_vector<bool> RevJac(num_var_tape_);
   for(i = 0; i < num_var_tape_; i++)
      RevJac[i] = false;
   for(i = 0; i < m; i++)
   {  CPPAD_ASSERT_UNKNOWN( dep_taddr_[i] < num_var_tape_ );
      RevJac[ dep_taddr_[i] ] = s[i];
   }

   // vector of sets that will hold reverse Hessain values
   local::sparse::pack_setvec rev_hes_pattern;
   rev_hes_pattern.resize(num_var_tape_, q);

   // compute the Hessian sparsity patterns
   local::sweep::rev_hes(
      &play_,
      num_var_tape_,
      for_jac_sparse_pack_,
      RevJac.data(),
      rev_hes_pattern,
      not_used_rec_base

   );

   // return values corresponding to independent variables
   CPPAD_ASSERT_UNKNOWN( size_t(h.size()) == n * q );
   for(j = 0; j < n; j++)
   {  for(i = 0; i < q; i++)
      {  if( transpose )
            h[ j * q + i ] = false;
         else
            h[ i * n + j ] = false;
      }
   }

   // j is index corresponding to reverse mode partial
   for(j = 0; j < n; j++)
   {  CPPAD_ASSERT_UNKNOWN( ind_taddr_[j] < num_var_tape_ );

      // ind_taddr_[j] is operator taddr for j-th independent variable
      CPPAD_ASSERT_UNKNOWN( ind_taddr_[j] == j + 1 );
      CPPAD_ASSERT_UNKNOWN( play_.GetOp( ind_taddr_[j] ) == local::InvOp );

      // extract the result from rev_hes_pattern
      CPPAD_ASSERT_UNKNOWN( rev_hes_pattern.end() == q );
      local::sparse::pack_setvec::const_iterator itr(rev_hes_pattern, j + 1);
      i = *itr;
      while( i < q )
      {  if( transpose )
            h[ j * q + i ] = true;
         else
            h[ i * n + j ] = true;
         i = *(++itr);
      }
   }
}
/*!
Private helper function for RevSparseHes(q, s) set sparsity.

All of the description in the public member function RevSparseHes(q, s)
applies.

\param set_type
is a std::set<size_t> value.
This argument is used to dispatch to the proper source
code depending on the vlaue of SetVector::value_type.

\param transpose
See RevSparseHes(q, s).

\param q
See RevSparseHes(q, s).

\param s
See RevSparseHes(q, s).

\param h
is the return value for the corresponging call to RevSparseJac(q, s).
*/
template <class Base, class RecBase>
template <class SetVector>
void ADFun<Base,RecBase>::RevSparseHesCase(
   const std::set<size_t>&   set_type         ,
   bool                      transpose        ,
   size_t                    q                ,
   const SetVector&          s                ,
   SetVector&                h                )
{
   // used to identify the RecBase type in calls to sweeps
   RecBase not_used_rec_base(0.0);
   //
   size_t n = Domain();
# ifndef NDEBUG
   size_t m = Range();
# endif
   //
   if( transpose )
      h.resize(n);
   else
      h.resize(q);

   CPPAD_ASSERT_KNOWN(
      for_jac_sparse_set_.n_set() > 0,
      "RevSparseHes: previous stored call to ForSparseJac did not "
      "use std::set<size_t> for the elements of r."
   );
   CPPAD_ASSERT_UNKNOWN( for_jac_sparse_pack_.n_set() == 0 );
   CPPAD_ASSERT_UNKNOWN( for_jac_sparse_set_.n_set() == num_var_tape_  );
   //
   // temporary indices
   size_t i, j;
   std::set<size_t>::const_iterator itr_1;

   // check SetVector is Simple Vector class with sets for elements
   CheckSimpleVector<std::set<size_t>, SetVector>(
      local::one_element_std_set<size_t>(), local::two_element_std_set<size_t>()
   );

   CPPAD_ASSERT_KNOWN(
      q == for_jac_sparse_set_.end(),
      "RevSparseHes: q is not equal to its value\n"
      "in the previous call to ForSparseJac with this ADFun object."
   );
   CPPAD_ASSERT_KNOWN(
      s.size() == 1,
      "RevSparseHes: size of s is not equal to one."
   );

   // Array that will hold reverse Jacobian dependency flag.
   // Initialize as true for the dependent variables.
   local::pod_vector<bool> RevJac(num_var_tape_);
   for(i = 0; i < num_var_tape_; i++)
      RevJac[i] = false;
   itr_1 = s[0].begin();
   while( itr_1 != s[0].end() )
   {  i = *itr_1++;
      CPPAD_ASSERT_KNOWN(
         i < m,
         "RevSparseHes: an element of the set s[0] has value "
         "greater than or equal m"
      );
      CPPAD_ASSERT_UNKNOWN( dep_taddr_[i] < num_var_tape_ );
      RevJac[ dep_taddr_[i] ] = true;
   }


   // vector of sets that will hold reverse Hessain values
   local::sparse::list_setvec rev_hes_pattern;
   rev_hes_pattern.resize(num_var_tape_, q);

   // compute the Hessian sparsity patterns
   local::sweep::rev_hes(
      &play_,
      num_var_tape_,
      for_jac_sparse_set_,
      RevJac.data(),
      rev_hes_pattern,
      not_used_rec_base

   );

   // return values corresponding to independent variables
   // j is index corresponding to reverse mode partial
   CPPAD_ASSERT_UNKNOWN( size_t(h.size()) == q || transpose );
   CPPAD_ASSERT_UNKNOWN( size_t(h.size()) == n || ! transpose );
   for(j = 0; j < n; j++)
   {  CPPAD_ASSERT_UNKNOWN( ind_taddr_[j] < num_var_tape_ );
      CPPAD_ASSERT_UNKNOWN( ind_taddr_[j] == j + 1 );
      CPPAD_ASSERT_UNKNOWN( play_.GetOp( ind_taddr_[j] ) == local::InvOp );

      // extract the result from rev_hes_pattern
      // and add corresponding elements to result sets in h
      CPPAD_ASSERT_UNKNOWN( rev_hes_pattern.end() == q );
      local::sparse::list_setvec::const_iterator itr_2(rev_hes_pattern, j+1);
      i = *itr_2;
      while( i < q )
      {  if( transpose )
            h[j].insert(i);
         else
            h[i].insert(j);
         i = *(++itr_2);
      }
   }
}

// ===========================================================================
// RevSparseHes

/*!
User API for Hessian sparsity patterns using reverse mode.

The C++ source code corresponding to this operation is
\verbatim
   h = f.RevSparseHes(q, r)
\endverbatim

\tparam Base
is the base type for this recording.

\tparam SetVector
is a simple vector with elements of type bool
or std::set<size_t>.

\param transpose
is true (false) if is is equal to \f$ H(x) \f$ (\f$ H(x)^T \f$)
where
\f[
   H(x) = R^T (S * F)^{(2)} (x)
\f]
where \f$ F \f$ is the function corresponding to the operation sequence
and x is any argument value.

\param q
is the value of q in the
by the previous call of the form
\verbatim
   f.ForSparseJac(q, r, packed)
\endverbatim
The value r in this call is a sparsity pattern for the matrix \f$ R \f$.
The type of the element of r for the previous call to ForSparseJac
must be the same as the type of the elements of s.

\param s
is a vector with size m that specifies the sparsity pattern
for the vector \f$ S \f$,
where m is the number of dependent variables
corresponding to the operation sequence stored in play.

\return
If transpose is false (true),
the return vector is a sparsity pattern for \f$ H(x) \f$ (\f$ H(x)^T \f$).
\f[
   H(x) = R^T ( S * F)^{(2)} (x)
\f]
where \f$ F \f$ is the function corresponding to the operation sequence
and x is any argument value.
*/

template <class Base, class RecBase>
template <class SetVector>
SetVector ADFun<Base,RecBase>::RevSparseHes(
   size_t q,  const SetVector& s, bool transpose
)
{
   SetVector h;
   typedef typename SetVector::value_type Set_type;

   // Should check to make sure q is same as in previous call to
   // forward sparse Jacobian.
   RevSparseHesCase(
      Set_type()    ,
      transpose     ,
      q             ,
      s             ,
      h
   );

   return h;
}
// ===========================================================================
// RevSparseHesCheckpoint
/*!
Hessian sparsity patterns calculation used by checkpoint functions.

\tparam Base
is the base type for this recording.

\param transpose
is true (false) h is equal to \f$ H(x) \f$ (\f$ H(x)^T \f$)
where
\f[
   H(x) = R^T (S * F)^{(2)} (x)
\f]
where \f$ F \f$ is the function corresponding to the operation sequence
and \f$ x \f$ is any argument value.

\param q
is the value of q in the by the previous call of the form
\verbatim
   f.ForSparseJac(q, r)
\endverbatim
The value r in this call is a sparsity pattern for the matrix \f$ R \f$.

\param s
is a vector with size m that specifies the sparsity pattern
for the vector \f$ S \f$,
where m is the number of dependent variables
corresponding to the operation sequence stored in play_.

\param h
The input size and elements of h do not matter.
On output, h is the sparsity pattern for the matrix \f$ H(x) \f$
or \f$ H(x)^T \f$ depending on transpose.

\par Assumptions
The forward jacobian sparsity pattern must be currently stored
in this ADFUN object.
*/
template <class Base, class RecBase>
void ADFun<Base,RecBase>::RevSparseHesCheckpoint(
   size_t                        q         ,
   vector<bool>&                 s         ,
   bool                          transpose ,
   local::sparse::list_setvec&   h         )
{
   // used to identify the RecBase type in calls to sweeps
   RecBase not_used_rec_base(0.0);
   //
   size_t n = Domain();
   size_t m = Range();

   // checkpoint functions should get this right
   CPPAD_ASSERT_UNKNOWN( for_jac_sparse_pack_.n_set() == 0 );
   CPPAD_ASSERT_UNKNOWN( for_jac_sparse_set_.n_set() == num_var_tape_ );
   CPPAD_ASSERT_UNKNOWN( for_jac_sparse_set_.end()   == q );
   CPPAD_ASSERT_UNKNOWN( s.size()                    == m );

   // Array that holds the reverse Jacobiain dependcy flags.
   // Initialize as true for dependent variables, flase for others.
   local::pod_vector<bool> RevJac(num_var_tape_);
   for(size_t i = 0; i < num_var_tape_; i++)
      RevJac[i] = false;
   for(size_t i = 0; i < m; i++)
   {  CPPAD_ASSERT_UNKNOWN( dep_taddr_[i] < num_var_tape_ )
      RevJac[ dep_taddr_[i] ] = s[i];
   }

   // holds reverse Hessian sparsity pattern for all variables
   local::sparse::list_setvec rev_hes_pattern;
   rev_hes_pattern.resize(num_var_tape_, q);

   // compute Hessian sparsity pattern for all variables
   local::sweep::rev_hes(
      &play_,
      num_var_tape_,
      for_jac_sparse_set_,
      RevJac.data(),
      rev_hes_pattern,
      not_used_rec_base

   );

   // dimension the return value
   if( transpose )
      h.resize(n, q);
   else
      h.resize(q, n);

   // j is index corresponding to reverse mode partial
   for(size_t j = 0; j < n; j++)
   {  CPPAD_ASSERT_UNKNOWN( ind_taddr_[j] < num_var_tape_ );

      // ind_taddr_[j] is operator taddr for j-th independent variable
      CPPAD_ASSERT_UNKNOWN( ind_taddr_[j] == j + 1 );
      CPPAD_ASSERT_UNKNOWN( play_.GetOp( ind_taddr_[j] ) == local::InvOp );

      // extract the result from rev_hes_pattern
      CPPAD_ASSERT_UNKNOWN( rev_hes_pattern.end() == q );
      local::sparse::list_setvec::const_iterator itr(rev_hes_pattern, j + 1);
      size_t i = *itr;
      while( i < q )
      {  if( transpose )
            h.post_element(j,  i);
         else
            h.post_element(i, j);
         i = *(++itr);
      }
   }
   for(size_t i = 0; i < h.n_set(); ++i)
      h.process_post(i);
}

} // END_CPPAD_NAMESPACE
# endif
