# ifndef CPPAD_CORE_FOR_SPARSE_JAC_HPP
# define CPPAD_CORE_FOR_SPARSE_JAC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin ForSparseJac}

Jacobian Sparsity Pattern: Forward Mode
#######################################

Syntax
******
| *s* = *f* . ``ForSparseJac`` ( *q* , *r* )
| *s* = *f* . ``ForSparseJac`` ( *q* , *r* , *transpose* , *dependency* )

Purpose
*******
We use :math:`F : \B{R}^n \rightarrow \B{R}^m` to denote the
:ref:`glossary@AD Function` corresponding to *f* .
For a fixed :math:`n \times q` matrix :math:`R`,
the Jacobian of :math:`F[ x + R * u ]`
with respect to :math:`u` at :math:`u = 0` is

.. math::

   S(x) = F^{(1)} ( x ) * R

Given a
:ref:`glossary@Sparsity Pattern`
for :math:`R`,
``ForSparseJac`` returns a sparsity pattern for the :math:`S(x)`.

f
*
The object *f* has prototype

   ``ADFun`` < *Base* > *f*

Note that the :ref:`ADFun-name` object *f* is not ``const`` .
After a call to ``ForSparseJac`` , the sparsity pattern
for each of the variables in the operation sequence
is held in *f* (for possible later use by :ref:`RevSparseHes-name` ).
These sparsity patterns are stored with elements of type ``bool``
or elements of type ``std::set<size_t>``
(see :ref:`ForSparseJac@SetVector` below).

size_forward_bool
=================
After ``ForSparseJac`` , if *k* is a ``size_t`` object,

   *k* = *f* . ``size_forward_bool`` ()

sets *k* to the amount of memory (in unsigned character units)
used to store the sparsity pattern with elements of type ``bool``
in the function object *f* .
If the sparsity patterns for the previous ``ForSparseJac`` used
elements of type ``bool`` ,
the return value for ``size_forward_bool`` will be non-zero.
Otherwise, its return value will be zero.
This sparsity pattern is stored for use by :ref:`RevSparseHes-name` and
when it is not longer needed, it can be deleted
(and the corresponding memory freed) using

   *f* . ``size_forward_bool`` (0)

After this call, *f* . ``size_forward_bool`` () will return zero.

size_forward_set
================
After ``ForSparseJac`` , if *k* is a ``size_t`` object,

   *k* = *f* . ``size_forward_set`` ()

sets *k* to the amount of memory (in unsigned character units)
used to store the
:ref:`glossary@Sparsity Pattern@Vector of Sets`
sparsity patterns.
If the sparsity patterns for this operation use elements of type ``bool`` ,
the return value for ``size_forward_set`` will be zero.
Otherwise, its return value will be non-zero.
This sparsity pattern is stored for use by :ref:`RevSparseHes-name` and
when it is not longer needed, it can be deleted
(and the corresponding memory freed) using

   *f* . ``size_forward_set`` (0)

After this call, *f* . ``size_forward_set`` () will return zero.

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

It specifies the number of columns in
:math:`R \in \B{R}^{n \times q}` and the Jacobian
:math:`S(x) \in \B{R}^{m \times q}`.

transpose
*********
The argument *transpose* has prototype

   ``bool`` *transpose*

The default value ``false`` is used when *transpose* is not present.

dependency
**********
The argument *dependency* has prototype

   ``bool`` *dependency*

If *dependency* is true,
the :ref:`dependency.cpp@Dependency Pattern`
(instead of sparsity pattern) is computed.

r
*
The argument *r* has prototype

   ``const`` *SetVector* & *r*

see :ref:`ForSparseJac@SetVector` below.

transpose false
===============
If *r* has elements of type ``bool`` ,
its size is :math:`n * q`.
If it has elements of type ``std::set<size_t>`` ,
its size is :math:`n` and all the set elements must be between
zero and *q* ``-1`` inclusive.
It specifies a
:ref:`glossary@Sparsity Pattern`
for the matrix :math:`R \in \B{R}^{n \times q}`.

transpose true
==============
If *r* has elements of type ``bool`` ,
its size is :math:`q * n`.
If it has elements of type ``std::set<size_t>`` ,
its size is :math:`q` and all the set elements must be between
zero and *n* ``-1`` inclusive.
It specifies a
:ref:`glossary@Sparsity Pattern`
for the matrix :math:`R^\R{T} \in \B{R}^{q \times n}`.

s
*
The return value *s* has prototype

   *SetVector* *s*

see :ref:`ForSparseJac@SetVector` below.

transpose false
===============
If *s* has elements of type ``bool`` ,
its size is :math:`m * q`.
If it has elements of type ``std::set<size_t>`` ,
its size is :math:`m` and all its set elements are between
zero and *q* ``-1`` inclusive.
It specifies a
:ref:`glossary@Sparsity Pattern`
for the matrix :math:`S(x) \in \B{R}^{m \times q}`.

transpose true
==============
If *s* has elements of type ``bool`` ,
its size is :math:`q * m`.
If it has elements of type ``std::set<size_t>`` ,
its size is :math:`q` and all its set elements are between
zero and *m* ``-1`` inclusive.
It specifies a
:ref:`glossary@Sparsity Pattern`
for the matrix :math:`S(x)^\R{T} \in \B{R}^{q \times m}`.

SetVector
*********
The type *SetVector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
``bool`` or ``std::set<size_t>`` ;
see :ref:`glossary@Sparsity Pattern` for a discussion
of the difference.

Entire Sparsity Pattern
***********************
Suppose that :math:`q = n` and
:math:`R` is the :math:`n \times n` identity matrix.
In this case,
the corresponding value for *s* is a
sparsity pattern for the Jacobian :math:`S(x) = F^{(1)} ( x )`.

Example
*******
{xrst_toc_hidden
   example/sparse/for_sparse_jac.cpp
}
The file
:ref:`for_sparse_jac.cpp-name`
contains an example and test of this operation.
The file
:ref:`sparsity_sub.cpp<sparsity_sub.cpp@ForSparseJac>`
contains an example and test of using ``ForSparseJac``
to compute the sparsity pattern for a subset of the Jacobian.

{xrst_end ForSparseJac}
-----------------------------------------------------------------------------
*/

# include <cppad/local/std_set.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file core/for_sparse_jac.hpp
Forward mode Jacobian sparsity patterns.
*/
// ---------------------------------------------------------------------------
/*!
Private helper function for ForSparseJac(q, r) boolean sparsity patterns.

All of the description in the public member function ForSparseJac(q, r)
applies.

\param set_type
is a bool value. This argument is used to dispatch to the proper source
code depending on the value of SetVector::value_type.

\param transpose
See ForSparseJac(q, r, transpose, dependency).

\param dependency
See ForSparseJac(q, r, transpose, dependency).

\param q
See ForSparseJac(q, r, transpose, dependency).

\param r
See ForSparseJac(q, r, transpose, dependency).

\param s
is the return value for the corresponding call to ForSparseJac(q, r).
*/

template <class Base, class RecBase>
template <class SetVector>
void ADFun<Base,RecBase>::ForSparseJacCase(
   bool                set_type      ,
   bool                transpose     ,
   bool                dependency    ,
   size_t              q             ,
   const SetVector&    r             ,
   SetVector&          s             )
{
   // used to identify the RecBase type in calls to sweeps
   RecBase not_used_rec_base(0.0);
   //
   size_t m = Range();
   size_t n = Domain();

   // check SetVector is Simple Vector class with bool elements
   CheckSimpleVector<bool, SetVector>();

   // dimension size of result vector
   s.resize( m * q );

   CPPAD_ASSERT_KNOWN(
      q > 0,
      "ForSparseJac: q is not greater than zero"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(r.size()) == n * q,
      "ForSparseJac: size of r is not equal to\n"
      "q times domain dimension for ADFun object."
   );
   //
   // allocate memory for the requested sparsity calculation result
   for_jac_sparse_pack_.resize(num_var_tape_, q);

   // set values corresponding to independent variables
   for(size_t i = 0; i < n; i++)
   {  CPPAD_ASSERT_UNKNOWN( ind_taddr_[i] < num_var_tape_ );
      // ind_taddr_[i] is operator taddr for i-th independent variable
      CPPAD_ASSERT_UNKNOWN( play_.GetOp( ind_taddr_[i] ) == local::InvOp );

      // set bits that are true
      if( transpose )
      {  for(size_t j = 0; j < q; j++) if( r[ j * n + i ] )
            for_jac_sparse_pack_.post_element( ind_taddr_[i], j);
      }
      else
      {  for(size_t j = 0; j < q; j++) if( r[ i * q + j ] )
            for_jac_sparse_pack_.post_element( ind_taddr_[i], j);
      }
   }
   // process posts
   for(size_t j = 0; j < n; j++)
      for_jac_sparse_pack_.process_post( ind_taddr_[j] );

   // evaluate the sparsity patterns
   local::sweep::for_jac(
      &play_,
      dependency,
      n,
      num_var_tape_,
      for_jac_sparse_pack_,
      not_used_rec_base

   );

   // return values corresponding to dependent variables
   CPPAD_ASSERT_UNKNOWN( size_t(s.size()) == m * q );
   for(size_t i = 0; i < m; i++)
   {  CPPAD_ASSERT_UNKNOWN( dep_taddr_[i] < num_var_tape_ );

      // extract the result from for_jac_sparse_pack_
      if( transpose )
      {  for(size_t j = 0; j < q; j++)
            s[ j * m + i ] = false;
      }
      else
      {  for(size_t j = 0; j < q; j++)
            s[ i * q + j ] = false;
      }
      CPPAD_ASSERT_UNKNOWN( for_jac_sparse_pack_.end() == q );
      local::sparse::pack_setvec::const_iterator
         itr(for_jac_sparse_pack_, dep_taddr_[i] );
      size_t j = *itr;
      while( j < q )
      {  if( transpose )
            s[j * m + i] = true;
         else
            s[i * q + j] = true;
         j = *(++itr);
      }
   }
}
// ---------------------------------------------------------------------------
/*!
Private helper function for ForSparseJac(q, r) set sparsity.

All of the description in the public member function ForSparseJac(q, r)
applies.

\param set_type
is a std::set<size_t> object.
This argument is used to dispatch to the proper source
code depending on the value of SetVector::value_type.

\param transpose
See ForSparseJac(q, r, transpose, dependency).

\param dependency
See ForSparseJac(q, r, transpose, dependency).

\param q
See ForSparseJac(q, r, transpose, dependency).

\param r
See ForSparseJac(q, r, transpose, dependency).

\param s
is the return value for the corresponding call to ForSparseJac(q, r).
*/
template <class Base, class RecBase>
template <class SetVector>
void ADFun<Base,RecBase>::ForSparseJacCase(
   const std::set<size_t>&    set_type      ,
   bool                       transpose     ,
   bool                       dependency    ,
   size_t                     q             ,
   const SetVector&           r             ,
   SetVector&                 s             )
{
   // used to identify the RecBase type in calls to sweeps
   RecBase not_used_rec_base(0.0);
   //
   size_t m = Range();
   size_t n = Domain();

   // check SetVector is Simple Vector class with sets for elements
   CheckSimpleVector<std::set<size_t>, SetVector>(
      local::one_element_std_set<size_t>(), local::two_element_std_set<size_t>()
   );

   // dimension size of result vector
   if( transpose )
      s.resize(q);
   else
      s.resize( m );

   // temporary iterator
   std::set<size_t>::const_iterator itr_1;

   CPPAD_ASSERT_KNOWN(
      q > 0,
      "ForSparseJac: q is not greater than zero"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(r.size()) == n || transpose,
      "ForSparseJac: size of r is not equal to n and transpose is false."
   );
   CPPAD_ASSERT_KNOWN(
      size_t(r.size()) == q || ! transpose,
      "ForSparseJac: size of r is not equal to q and transpose is true."
   );
   //
   // allocate memory for the requested sparsity calculation
   for_jac_sparse_set_.resize(num_var_tape_, q);

   // set values corresponding to independent variables
   if( transpose )
   {  for(size_t i = 0; i < q; i++)
      {  // add the elements that are present
         itr_1 = r[i].begin();
         while( itr_1 != r[i].end() )
         {  size_t j = *itr_1++;
            CPPAD_ASSERT_KNOWN(
            j < n,
            "ForSparseJac: transpose is true and element of the set\n"
            "r[j] has value greater than or equal n."
            );
            CPPAD_ASSERT_UNKNOWN( ind_taddr_[j] < num_var_tape_ );
            // operator for j-th independent variable
            CPPAD_ASSERT_UNKNOWN(
               play_.GetOp( ind_taddr_[j] ) == local::InvOp
            );
            for_jac_sparse_set_.post_element( ind_taddr_[j], i);
         }
      }
   }
   else
   {  for(size_t i = 0; i < n; i++)
      {  CPPAD_ASSERT_UNKNOWN( ind_taddr_[i] < num_var_tape_ );
         // ind_taddr_[i] is operator taddr for i-th independent variable
         CPPAD_ASSERT_UNKNOWN( play_.GetOp( ind_taddr_[i] ) == local::InvOp );

         // add the elements that are present
         itr_1 = r[i].begin();
         while( itr_1 != r[i].end() )
         {  size_t j = *itr_1++;
            CPPAD_ASSERT_KNOWN(
               j < q,
               "ForSparseJac: an element of the set r[i] "
               "has value greater than or equal q."
            );
            for_jac_sparse_set_.post_element( ind_taddr_[i], j);
         }
      }
   }
   // process posts
   for(size_t j = 0; j < n; j++)
      for_jac_sparse_set_.process_post( ind_taddr_[j] );

   // evaluate the sparsity patterns
   local::sweep::for_jac(
      &play_,
      dependency,
      n,
      num_var_tape_,
      for_jac_sparse_set_,
      not_used_rec_base

   );

   // return values corresponding to dependent variables
   CPPAD_ASSERT_UNKNOWN( size_t(s.size()) == m || transpose );
   CPPAD_ASSERT_UNKNOWN( size_t(s.size()) == q || ! transpose );
   for(size_t i = 0; i < m; i++)
   {  CPPAD_ASSERT_UNKNOWN( dep_taddr_[i] < num_var_tape_ );

      // extract results from for_jac_sparse_set_
      // and add corresponding elements to sets in s
      CPPAD_ASSERT_UNKNOWN( for_jac_sparse_set_.end() == q );
      local::sparse::list_setvec::const_iterator
         itr_2(for_jac_sparse_set_, dep_taddr_[i] );
      size_t j = *itr_2;
      while( j < q )
      {  if( transpose )
            s[j].insert(i);
         else
            s[i].insert(j);
         j = *(++itr_2);
      }
   }
}
// ---------------------------------------------------------------------------

/*!
User API for Jacobian sparsity patterns using forward mode.

The C++ source code corresponding to this operation is
\verbatim
   s = f.ForSparseJac(q, r, transpose, dependency)
\endverbatim

\tparam Base
is the base type for this recording.

\tparam SetVector
is a simple vector with elements of type bool
or std::set<size_t>.

\param q
is the number of columns in the matrix \f$ R \f$.

\param r
is a sparsity pattern for the matrix \f$ R \f$.

\param transpose
are sparsity patterns for \f$ R \f$ and \f$ S(x) \f$ transposed.

\param dependency
Are the derivatives with respect to left and right of the expression below
considered to be non-zero:
\code
   CondExpRel(left, right, if_true, if_false)
\endcode
This is used by the optimizer to obtain the correct dependency relations.

\return
The value of transpose is false (true),
the return value is a sparsity pattern for \f$ S(x) \f$ (\f$ S(x)^T \f$) where
\f[
   S(x) = F^{(1)} (x) * R
\f]
where \f$ F \f$ is the function corresponding to the operation sequence
and x is any argument value.
If SetVector::value_type is bool,
the return value has size \f$ m * q \f$ (\f$ q * m \f$).
where m is the number of dependent variables
corresponding to the operation sequence stored in f.
If SetVector::value_type is std::set<size_t>,
the return value has size \f$ m \f$ ( \f$ q \f$ )
and with all its elements between zero and
\f$ q - 1 \f$ ( \f$ m - 1 \f$).

\par Side Effects
If SetVector::value_type is bool,
the forward sparsity pattern for all of the variables on the
tape is stored in for_jac_sparse_pack__.
In this case
\verbatim
   for_jac_sparse_pack_.n_set() == num_var_tape_
   for_jac_sparse_pack_.end() == q
   for_jac_sparse_set_.n_set()  == 0
   for_jac_sparse_set_.end()  == 0
\endverbatim
\n
\n
If SetVector::value_type is std::set<size_t>,
the forward sparsity pattern for all of the variables on the
tape is stored in for_jac_sparse_set__.
In this case
\verbatim
   for_jac_sparse_set_.n_set()   == num_var_tape_
   for_jac_sparse_set_.end()   == q
   for_jac_sparse_pack_.n_set()  == 0
   for_jac_sparse_pack_.end()  == 0
\endverbatim
*/
template <class Base, class RecBase>
template <class SetVector>
SetVector ADFun<Base,RecBase>::ForSparseJac(
   size_t             q             ,
   const SetVector&   r             ,
   bool               transpose     ,
   bool               dependency    )
{
   SetVector s;
   typedef typename SetVector::value_type Set_type;

   // free all memory currently in sparsity patterns
   for_jac_sparse_pack_.resize(0, 0);
   for_jac_sparse_set_.resize(0, 0);

   ForSparseJacCase(
      Set_type()  ,
      transpose   ,
      dependency  ,
      q           ,
      r           ,
      s
   );

   return s;
}
// ===========================================================================
// ForSparseJacCheckpoint
/*!
Forward mode Jacobian sparsity calculation used by checkpoint functions.

\tparam Base
is the base type for this recording.

\param transpose
is true (false) s is equal to \f$ S(x) \f$ (\f$ S(x)^T \f$)
where
\f[
   S(x) = F^{(1)} (x) * R
\f]
where \f$ F \f$ is the function corresponding to the operation sequence
and \f$ x \f$ is any argument value.

\param q
is the number of columns in the matrix \f$ R \f$.

\param r
is a sparsity pattern for the matrix \f$ R \f$.

\param transpose
are the sparsity patterns for \f$ R \f$ and \f$ S(x) \f$ transposed.

\param dependency
Are the derivatives with respect to left and right of the expression below
considered to be non-zero:
\code
   CondExpRel(left, right, if_true, if_false)
\endcode
This is used by the optimizer to obtain the correct dependency relations.

\param s
The input size and elements of s do not matter.
On output, s is the sparsity pattern for the matrix \f$ S(x) \f$
or \f$ S(x)^T \f$ depending on transpose.

\par Side Effects
If SetVector::value_type is bool,
the forward sparsity pattern for all of the variables on the
tape is stored in for_jac_sparse_pack__.
In this case
\verbatim
   for_jac_sparse_pack_.n_set() == num_var_tape_
   for_jac_sparse_pack_.end() == q
   for_jac_sparse_set_.n_set()  == 0
   for_jac_sparse_set_.end()  == 0
\endverbatim
\n
\n
If SetVector::value_type is std::set<size_t>,
the forward sparsity pattern for all of the variables on the
tape is stored in for_jac_sparse_set__.
In this case
\verbatim
   for_jac_sparse_set_.n_set()   == num_var_tape_
   for_jac_sparse_set_.end()   == q
   for_jac_sparse_pack_.n_set()  == 0
   for_jac_sparse_pack_.end()  == 0
\endverbatim
*/
template <class Base, class RecBase>
void ADFun<Base,RecBase>::ForSparseJacCheckpoint(
   size_t                             q          ,
   const local::sparse::list_setvec&  r          ,
   bool                               transpose  ,
   bool                               dependency ,
   local::sparse::list_setvec&        s          )
{
   // used to identify the RecBase type in calls to sweeps
   RecBase not_used_rec_base(0.0);
   //
   size_t n = Domain();
   size_t m = Range();

# ifndef NDEBUG
   if( transpose )
   {  CPPAD_ASSERT_UNKNOWN( r.n_set() == q );
      CPPAD_ASSERT_UNKNOWN( r.end()   == n );
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( r.n_set() == n );
      CPPAD_ASSERT_UNKNOWN( r.end()   == q );
   }
   for(size_t j = 0; j < n; j++)
   {  CPPAD_ASSERT_UNKNOWN( ind_taddr_[j] == (j+1) );
      CPPAD_ASSERT_UNKNOWN( play_.GetOp( ind_taddr_[j] ) == local::InvOp );
   }
# endif

   // free all memory currently in sparsity patterns
   for_jac_sparse_pack_.resize(0, 0);
   for_jac_sparse_set_.resize(0, 0);

   // allocate new sparsity pattern
   for_jac_sparse_set_.resize(num_var_tape_, q);

   // set sparsity pattern for dependent variables
   if( transpose )
   {  for(size_t i = 0; i < q; i++)
      {  local::sparse::list_setvec::const_iterator itr(r, i);
         size_t j = *itr;
         while( j < n )
         {  for_jac_sparse_set_.post_element( ind_taddr_[j], i );
            j = *(++itr);
         }
      }
   }
   else
   {  for(size_t j = 0; j < n; j++)
      {  local::sparse::list_setvec::const_iterator itr(r, j);
         size_t i = *itr;
         while( i < q )
         {  for_jac_sparse_set_.post_element( ind_taddr_[j], i );
            i = *(++itr);
         }
      }
   }
   // process posts
   for(size_t j = 0; j < n; j++)
      for_jac_sparse_set_.process_post( ind_taddr_[j] );

   // evaluate the sparsity pattern for all variables
   local::sweep::for_jac(
      &play_,
      dependency,
      n,
      num_var_tape_,
      for_jac_sparse_set_,
      not_used_rec_base

   );

   // dimension the return value
   if( transpose )
      s.resize(q, m);
   else
      s.resize(m, q);

   // return values corresponding to dependent variables
   for(size_t i = 0; i < m; i++)
   {  CPPAD_ASSERT_UNKNOWN( dep_taddr_[i] < num_var_tape_ );

      // extract the result from for_jac_sparse_set_
      CPPAD_ASSERT_UNKNOWN( for_jac_sparse_set_.end() == q );
      local::sparse::list_setvec::const_iterator
         itr(for_jac_sparse_set_, dep_taddr_[i] );
      size_t j = *itr;
      while( j < q )
      {  if( transpose )
            s.post_element(j, i);
         else
            s.post_element(i, j);
         j  = *(++itr);
      }
   }
   // process posts
   for(size_t i = 0; i < s.n_set(); ++i)
      s.process_post(i);

}


} // END_CPPAD_NAMESPACE
# endif
