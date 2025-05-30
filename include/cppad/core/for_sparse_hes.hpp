# ifndef CPPAD_CORE_FOR_SPARSE_HES_HPP
# define CPPAD_CORE_FOR_SPARSE_HES_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin ForSparseHes}
{xrst_spell
   walther
}

Hessian Sparsity Pattern: Forward Mode
######################################

Syntax
******
| *h* = *f* . ``ForSparseHes`` ( *r* , *s* )

Purpose
*******
We use :math:`F : \B{R}^n \rightarrow \B{R}^m` to denote the
:ref:`glossary@AD Function` corresponding to *f* .
we define

.. math::
   :nowrap:

   \begin{eqnarray}
   H(x)
   & = & \partial_x \left[ \partial_u S \cdot F[ x + R \cdot u ] \right]_{u=0}
   \\
   & = & R^\R{T} \cdot (S \cdot F)^{(2)} ( x ) \cdot R
   \end{eqnarray}

Where :math:`R \in \B{R}^{n \times n}` is a diagonal matrix
and :math:`S \in \B{R}^{1 \times m}` is a row vector.
Given a
:ref:`glossary@Sparsity Pattern`
for the diagonal of :math:`R` and the vector :math:`S`,
``ForSparseHes`` returns a sparsity pattern for the :math:`H(x)`.

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

r
*
The argument *r* has prototype

   ``const`` *SetVector* & *r*

(see :ref:`ForSparseHes@SetVector` below)
If it has elements of type ``bool`` ,
its size is :math:`n`.
If it has elements of type ``std::set<size_t>`` ,
its size is one and all the elements of *s* [0]
are between zero and :math:`n - 1`.
It specifies a
:ref:`glossary@Sparsity Pattern`
for the diagonal of :math:`R`.
The fewer non-zero elements in this sparsity pattern,
the faster the calculation should be and the more sparse
:math:`H(x)` should be.

s
*
The argument *s* has prototype

   ``const`` *SetVector* & *s*

(see :ref:`ForSparseHes@SetVector` below)
If it has elements of type ``bool`` ,
its size is :math:`m`.
If it has elements of type ``std::set<size_t>`` ,
its size is one and all the elements of *s* [0]
are between zero and :math:`m - 1`.
It specifies a
:ref:`glossary@Sparsity Pattern`
for the vector *S* .
The fewer non-zero elements in this sparsity pattern,
the faster the calculation should be and the more sparse
:math:`H(x)` should be.

h
*
The result *h* has prototype

   *SetVector* & *h*

(see :ref:`ForSparseHes@SetVector` below).
If *h* has elements of type ``bool`` ,
its size is :math:`n * n`.
If it has elements of type ``std::set<size_t>`` ,
its size is :math:`n` and all the set elements are between
zero and *n* ``-1`` inclusive.
It specifies a
:ref:`glossary@Sparsity Pattern`
for the matrix :math:`H(x)`.

SetVector
*********
The type *SetVector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
``bool`` or ``std::set<size_t>`` ;
see :ref:`glossary@Sparsity Pattern` for a discussion
of the difference.
The type of the elements of
:ref:`ForSparseHes@SetVector` must be the
same as the type of the elements of *r* .

Algorithm
*********
See Algorithm II in
*Computing sparse Hessians with automatic differentiation*
by Andrea Walther.
Note that *s* provides the information so that
'dead ends' are not included in the sparsity pattern.

Example
*******
{xrst_toc_hidden
   example/sparse/for_sparse_hes.cpp
}
The file
:ref:`for_sparse_hes.cpp-name`
contains an example and test of this operation.

{xrst_end ForSparseHes}
-----------------------------------------------------------------------------
*/
# include <algorithm>
# include <cppad/local/pod_vector.hpp>
# include <cppad/local/std_set.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file core/for_sparse_hes.hpp
Forward mode Hessian sparsity patterns.
*/
// ===========================================================================
// ForSparseHesCase
/*!
Private helper function for ForSparseHes(q, s) bool sparsity.

All of the description in the public member function ForSparseHes(q, s)
applies.

\param set_type
is a bool value. This argument is used to dispatch to the proper source
code depending on the vlaue of SetVector::value_type.

\param r
See ForSparseHes(r, s).

\param s
See ForSparseHes(r, s).

\param h
is the return value for the corresponging call to ForSparseJac(q, s).
*/
template <class Base, class RecBase>
template <class SetVector>
void ADFun<Base,RecBase>::ForSparseHesCase(
   bool              set_type         ,
   const SetVector&  r                ,
   const SetVector&  s                ,
   SetVector&        h                )
{
   // used to identify the RecBase type in calls to sweeps
   RecBase not_used_rec_base(0.0);
   //
   size_t n = Domain();
   size_t m = Range();
   //
   // check Vector is Simple SetVector class with bool elements
   CheckSimpleVector<bool, SetVector>();
   //
   CPPAD_ASSERT_KNOWN(
      size_t(r.size()) == n,
      "ForSparseHes: size of r is not equal to\n"
      "domain dimension for ADFun object."
   );
   CPPAD_ASSERT_KNOWN(
      size_t(s.size()) == m,
      "ForSparseHes: size of s is not equal to\n"
      "range dimension for ADFun object."
   );
   //
   // select_domain corresponding to r
   local::pod_vector<bool> select_domain(n);
   for(size_t j = 0; j < n; ++j)
   {  select_domain[j] = r[j];
      CPPAD_ASSERT_UNKNOWN( ind_taddr_[j]  == j + 1);
      CPPAD_ASSERT_UNKNOWN( play_.GetOp(j + 1) == local::InvOp );
   }
   // sparsity pattern correspnding to s
   local::sparse::pack_setvec rev_jac_pattern;
   rev_jac_pattern.resize(num_var_tape_, 1);
   for(size_t i = 0; i < m; i++)
   {  CPPAD_ASSERT_UNKNOWN( dep_taddr_[i] < num_var_tape_ );
      //
      // Not using post_element because only adding one element per set
      if( s[i] )
         rev_jac_pattern.add_element( dep_taddr_[i], 0);
   }
   // compute reverse sparsity pattern for dependency analysis
   // (note that we are only want non-zero derivatives not true dependency)
   bool dependency = false;
   local::sweep::rev_jac(
      &play_,
      dependency,
      n,
      num_var_tape_,
      rev_jac_pattern,
      not_used_rec_base
   );
   // vector of sets that will hold the forward Hessain values
   local::sparse::pack_setvec for_hes_pattern;
   for_hes_pattern.resize(n+1+num_var_tape_, n+1);
   //
   // compute the Hessian sparsity patterns
   local::sweep::for_hes(
      &play_,
      n,
      num_var_tape_,
      select_domain,
      rev_jac_pattern,
      for_hes_pattern,
      not_used_rec_base

   );
   // initialize return values corresponding to independent variables
   h.resize(n * n);
   for(size_t i = 0; i < n; i++)
   {  for(size_t j = 0; j < n; j++)
         h[ i * n + j ] = false;
   }
   // copy to result pattern
   CPPAD_ASSERT_UNKNOWN( for_hes_pattern.end() == n+1 );
   for(size_t i = 0; i < n; i++)
   {  // ind_taddr_[i] is operator taddr for i-th independent variable
      CPPAD_ASSERT_UNKNOWN( ind_taddr_[i] == i + 1 );
      CPPAD_ASSERT_UNKNOWN( play_.GetOp( ind_taddr_[i] ) == local::InvOp );

      // extract the result from for_hes_pattern
      local::sparse::pack_setvec::const_iterator itr(for_hes_pattern, ind_taddr_[i] );
      size_t j = *itr;
      while( j < for_hes_pattern.end() )
      {  CPPAD_ASSERT_UNKNOWN( 0 < j )
         h[ i * n + (j-1) ] = true;
         j = *(++itr);
      }
   }
}
/*!
Private helper function for ForSparseHes(q, s) set sparsity.

All of the description in the public member function ForSparseHes(q, s)
applies.

\param set_type
is a std::set<size_t> value.
This argument is used to dispatch to the proper source
code depending on the vlaue of SetVector::value_type.

\param r
See ForSparseHes(r, s).

\param s
See ForSparseHes(q, s).

\param h
is the return value for the corresponging call to ForSparseJac(q, s).
*/
template <class Base, class RecBase>
template <class SetVector>
void ADFun<Base,RecBase>::ForSparseHesCase(
   const std::set<size_t>&   set_type         ,
   const SetVector&          r                ,
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
   std::set<size_t>::const_iterator itr_1;
   //
   // check SetVector is Simple Vector class with sets for elements
   CheckSimpleVector<std::set<size_t>, SetVector>(
      local::one_element_std_set<size_t>(), local::two_element_std_set<size_t>()
   );
   CPPAD_ASSERT_KNOWN(
      r.size() == 1,
      "ForSparseHes: size of s is not equal to one."
   );
   CPPAD_ASSERT_KNOWN(
      s.size() == 1,
      "ForSparseHes: size of s is not equal to one."
   );
   //
   // select_domain corresponding to r
   local::pod_vector<bool> select_domain(n);
   for(size_t j = 0; j < n; ++j)
   {  select_domain[j] = false;
      CPPAD_ASSERT_UNKNOWN( ind_taddr_[j]  == j + 1);
      CPPAD_ASSERT_UNKNOWN( play_.GetOp(j + 1) == local::InvOp );
   }
   itr_1 = r[0].begin();
   while( itr_1 != r[0].end() )
   {  size_t j = *itr_1++;
      select_domain[j] = true;
   }
   // sparsity pattern correspnding to s
   local::sparse::list_setvec rev_jac_pattern;
   rev_jac_pattern.resize(num_var_tape_, 1);
   itr_1 = s[0].begin();
   while( itr_1 != s[0].end() )
   {  size_t i = *itr_1++;
      CPPAD_ASSERT_KNOWN(
         i < m,
         "ForSparseHes: an element of the set s[0] has value "
         "greater than or equal m"
      );
      CPPAD_ASSERT_UNKNOWN( dep_taddr_[i] < num_var_tape_ );
      //
      // Not using post_element because only adding one element per set
      rev_jac_pattern.add_element( dep_taddr_[i], 0);
   }
   //
   // compute reverse sparsity pattern for dependency analysis
   // (note that we are only want non-zero derivatives not true dependency)
   bool dependency = false;
   local::sweep::rev_jac(
      &play_,
      dependency,
      n,
      num_var_tape_,
      rev_jac_pattern,
      not_used_rec_base
   );
   //
   // vector of sets that will hold reverse Hessain values
   local::sparse::list_setvec for_hes_pattern;
   for_hes_pattern.resize(n+1+num_var_tape_, n+1);
   //
   // compute the Hessian sparsity patterns
   local::sweep::for_hes(
      &play_,
      n,
      num_var_tape_,
      select_domain,
      rev_jac_pattern,
      for_hes_pattern,
      not_used_rec_base

   );
   // return values corresponding to independent variables
   // j is index corresponding to reverse mode partial
   h.resize(n);
   CPPAD_ASSERT_UNKNOWN( for_hes_pattern.end() == n+1 );
   for(size_t i = 0; i < n; i++)
   {  CPPAD_ASSERT_UNKNOWN( ind_taddr_[i] == i + 1 );
      CPPAD_ASSERT_UNKNOWN( play_.GetOp( ind_taddr_[i] ) == local::InvOp );

      // extract the result from for_hes_pattern
      local::sparse::list_setvec::const_iterator itr_2(for_hes_pattern, ind_taddr_[i] );
      size_t j = *itr_2;
      while( j < for_hes_pattern.end() )
      {  CPPAD_ASSERT_UNKNOWN( 0 < j )
            h[i].insert(j-1);
         j = *(++itr_2);
      }
   }
}

// ===========================================================================
// ForSparseHes

/*!
User API for Hessian sparsity patterns using reverse mode.

The C++ source code corresponding to this operation is
\verbatim
   h = f.ForSparseHes(q, r)
\endverbatim

\tparam Base
is the base type for this recording.

\tparam SetVector
is a simple vector with elements of type bool
or std::set<size_t>.

\param r
is a vector with size n that specifies the sparsity pattern
for the diagonal of the matrix \f$ R \f$,
where n is the number of independent variables
corresponding to the operation sequence stored in play.

\param s
is a vector with size m that specifies the sparsity pattern
for the vector \f$ S \f$,
where m is the number of dependent variables
corresponding to the operation sequence stored in play.

\return
The return vector is a sparsity pattern for \f$ H(x) \f$
\f[
   H(x) = R^T ( S * F)^{(2)} (x) R
\f]
where \f$ F \f$ is the function corresponding to the operation sequence
and x is any argument value.
*/

template <class Base, class RecBase>
template <class SetVector>
SetVector ADFun<Base,RecBase>::ForSparseHes(
   const SetVector& r, const SetVector& s
)
{
   SetVector h;
   typedef typename SetVector::value_type Set_type;

   // Should check to make sure q is same as in previous call to
   // forward sparse Jacobian.
   ForSparseHesCase(
      Set_type()    ,
      r             ,
      s             ,
      h
   );

   return h;
}
// ===========================================================================
// ForSparseHesCheckpoint
/*!
Hessian sparsity patterns calculation used by checkpoint functions.

\tparam Base
is the base type for this recording.

\param r
is a vector with size n that specifies the sparsity pattern
for the diagonal of \f$ R \f$,
where n is the number of independent variables
corresponding to the operation sequence stored in play_.

\param s
is a vector with size m that specifies the sparsity pattern
for the vector \f$ S \f$,
where m is the number of dependent variables
corresponding to the operation sequence stored in play_.

\param h
The input size and elements of h do not matter.
On output, h is the sparsity pattern for the matrix \f$ H(x) R \f$.

\par Assumptions
The forward jacobian sparsity pattern must be currently stored
in this ADFUN object.
*/

// The checkpoint class is not yet using forward sparse Hessians.
# ifdef CPPAD_NOT_DEFINED
template <class Base, class RecBase>
void ADFun<Base,RecBase>::ForSparseHesCheckpoint(
   vector<bool>&                 r         ,
   vector<bool>&                 s         ,
   local::sparse::list_setvec&   h         )
{
   // used to identify the RecBase type in calls to sweeps
   RecBase not_used_rec_base(0.0);
   //

   size_t n = Domain();
   size_t m = Range();

   // checkpoint functions should get this right
   CPPAD_ASSERT_UNKNOWN( for_jac_sparse_pack_.n_set() == 0 );
   CPPAD_ASSERT_UNKNOWN( for_jac_sparse_set_.n_set() == 0   );
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

   // holds forward Hessian sparsity pattern for all variables
   local::sparse::list_setvec for_hes_pattern;
   for_hes_pattern.resize(n+1+num_var_tape_, n+1);

   // compute Hessian sparsity pattern for all variables
   local::sweep::for_hes(
      &play_,
      n,
      num_var_tape_,
      for_jac_sparse_set_,
      RevJac.data(),
      for_hes_pattern,
      not_used_rec_base

   );

   // dimension the return value
   if( transpose )
      h.resize(n, n);
   else
      h.resize(n, n);

   // j is index corresponding to reverse mode partial
   for(size_t j = 0; j < n; j++)
   {  CPPAD_ASSERT_UNKNOWN( ind_taddr_[j] < num_var_tape_ );

      // ind_taddr_[j] is operator taddr for j-th independent variable
      CPPAD_ASSERT_UNKNOWN( ind_taddr_[j] == j + 1 );
      CPPAD_ASSERT_UNKNOWN( play_.GetOp( ind_taddr_[j] ) == local::InvOp );

      // extract the result from for_hes_pattern
      CPPAD_ASSERT_UNKNOWN( for_hes_pattern.end() == q );
      local::sparse::list_setvec::const_iterator itr(for_hes_pattern, .j + 1);
      size_t i = *itr;
      while( i < q )
      {  if( transpose )
            h.post_element(j,  i);
         else
            h.post_element(i, j);
         i = *(++itr);
      }
   }
   // process posts
   for(size_t i = 0; i < n; ++i)
      h.process_post(i);
}
# endif

} // END_CPPAD_NAMESPACE
# endif
