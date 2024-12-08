# ifndef CPPAD_CORE_REV_HES_SPARSITY_HPP
# define CPPAD_CORE_REV_HES_SPARSITY_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin rev_hes_sparsity}

Reverse Mode Hessian Sparsity Patterns
######################################

Syntax
******
| *f* . ``rev_hes_sparsity`` (
| |tab| *select_range* , *transpose* , *internal_bool* , *pattern_out*
| )

Purpose
*******
We use :math:`F : \B{R}^n \rightarrow \B{R}^m` to denote the
:ref:`glossary@AD Function` corresponding to
the operation sequence stored in *f* .
Fix :math:`R \in \B{R}^{n \times \ell}`, :math:`s \in \B{R}^m`
and define the function

.. math::

   H(x) = ( s^\R{T} F )^{(2)} ( x ) R

Given a :ref:`glossary@Sparsity Pattern` for :math:`R`
and for the vector :math:`s`,
``rev_hes_sparsity`` computes a sparsity pattern for :math:`H(x)`.

x
*
Note that the sparsity pattern :math:`H(x)` corresponds to the
operation sequence stored in *f* and does not depend on
the argument *x* .

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

R
*
The sparsity pattern for the matrix :math:`R` is specified by
:ref:`for_jac_sparsity@pattern_in` in the previous call

| |tab| *f* . ``for_jac_sparsity`` (
| |tab| |tab| *pattern_in* , *transpose* , *dependency* , *internal_bool* , *pattern_out*
| )

select_range
************
The argument *select_range* has prototype

   ``const`` *BoolVector* & *select_range*

It has size :math:`m` and specifies which components of the vector
:math:`s` are non-zero; i.e., *select_range* [ *i* ] is true
if and only if :math:`s_i` is possibly non-zero.

transpose
*********
This argument has prototype

   ``bool`` *transpose*

See :ref:`rev_hes_sparsity@pattern_out` below.

internal_bool
*************
If this is true, calculations are done with sets represented by a vector
of boolean values. Otherwise, a vector of sets of integers is used.
This must be the same as in the previous call to
*f* . ``for_jac_sparsity`` .

pattern_out
***********
This argument has prototype

   ``sparse_rc`` < *SizeVector* >& *pattern_out*

This input value of *pattern_out* does not matter.
If *transpose* it is false (true),
upon return *pattern_out* is a sparsity pattern for
:math:`H(x)` (:math:`H(x)^\R{T}`).

Sparsity for Entire Hessian
***************************
Suppose that :math:`R` is the :math:`n \times n` identity matrix.
In this case, *pattern_out* is a sparsity pattern for
:math:`(s^\R{T} F)^{(2)} ( x )`.

Example
*******
{xrst_toc_hidden
   example/sparse/rev_hes_sparsity.cpp
}
The file
:ref:`rev_hes_sparsity.cpp-name`
contains an example and test of this operation.

{xrst_end rev_hes_sparsity}
-----------------------------------------------------------------------------
*/
# include <cppad/core/ad_fun.hpp>
# include <cppad/local/sparse/internal.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

/*!
Reverse Hessian sparsity patterns.

\tparam Base
is the base type for this recording.

\tparam BoolVector
is the simple vector with elements of type bool that is used for
sparsity for the vector s.

\tparam SizeVector
is the simple vector with elements of type size_t that is used for
row, column index sparsity patterns.

\param select_range
is a sparsity pattern for for s.

\param transpose
Is the returned sparsity pattern transposed.

\param internal_bool
If this is true, calculations are done with sets represented by a vector
of boolean values. Otherwise, a vector of standard sets is used.

\param pattern_out
The value of transpose is false (true),
the return value is a sparsity pattern for H(x) ( H(x)^T ) where
\f[
   H(x) = R * F^{(1)} (x)
\f]
Here F is the function corresponding to the operation sequence
and x is any argument value.
*/
template <class Base, class RecBase>
template <class BoolVector, class SizeVector>
void ADFun<Base,RecBase>::rev_hes_sparsity(
   const BoolVector&            select_range     ,
   bool                         transpose        ,
   bool                         internal_bool    ,
   sparse_rc<SizeVector>&       pattern_out      )
{
   // used to identify the RecBase type in calls to sweeps
   RecBase not_used_rec_base(0.0);
   //
   size_t m  = Range();
   //
   CPPAD_ASSERT_KNOWN(
      size_t( select_range.size() ) == m,
      "rev_hes_sparsity: size of select_range is not equal to "
      "number of dependent variables"
   );
   //
   // vector that holds reverse Jacobian sparsity flag
   local::pod_vector<bool> rev_jac_pattern(num_var_tape_);
   for(size_t i = 0; i < num_var_tape_; i++)
      rev_jac_pattern[i] = false;
   //
   // initialize rev_jac_pattern for dependent variables
   for(size_t i = 0; i < m; i++)
      rev_jac_pattern[ dep_taddr_[i] ] = select_range[i];
   //
   //
   if( internal_bool )
   {  CPPAD_ASSERT_KNOWN(
         for_jac_sparse_pack_.n_set() > 0,
         "rev_hes_sparsity: previous call to for_jac_sparsity did not "
         "use bool for interanl sparsity patterns."
      );
      // column dimension of internal sparstiy pattern
      size_t ell = for_jac_sparse_pack_.end();
      //
      // allocate memory for bool sparsity calculation
      // (sparsity pattern is emtpy after a resize)
      local::sparse::pack_setvec internal_hes;
      internal_hes.resize(num_var_tape_, ell);
      //
      // compute the Hessian sparsity pattern
      local::sweep::rev_hes(
         &play_,
         num_var_tape_,
         for_jac_sparse_pack_,
         rev_jac_pattern.data(),
         internal_hes,
         not_used_rec_base
      );
      // get sparstiy pattern for independent variables
      local::sparse::get_internal_pattern(
         transpose, ind_taddr_, internal_hes, pattern_out
      );
   }
   else
   {  CPPAD_ASSERT_KNOWN(
         for_jac_sparse_set_.n_set() > 0,
         "rev_hes_sparsity: previous call to for_jac_sparsity did not "
         "use bool for interanl sparsity patterns."
      );
      // column dimension of internal sparstiy pattern
      size_t ell = for_jac_sparse_set_.end();
      //
      // allocate memory for bool sparsity calculation
      // (sparsity pattern is emtpy after a resize)
      local::sparse::list_setvec internal_hes;
      internal_hes.resize(num_var_tape_, ell);
      //
      // compute the Hessian sparsity pattern
      local::sweep::rev_hes(
         &play_,
         num_var_tape_,
         for_jac_sparse_set_,
         rev_jac_pattern.data(),
         internal_hes,
         not_used_rec_base
      );
      // get sparstiy pattern for independent variables
      local::sparse::get_internal_pattern(
         transpose, ind_taddr_, internal_hes, pattern_out
      );
   }
   return;
}
} // END_CPPAD_NAMESPACE
# endif
