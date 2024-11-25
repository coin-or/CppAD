# ifndef CPPAD_CORE_FOR_JAC_SPARSITY_HPP
# define CPPAD_CORE_FOR_JAC_SPARSITY_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin for_jac_sparsity}

Forward Mode Jacobian Sparsity Patterns
#######################################

Syntax
******
| *f* . ``for_jac_sparsity`` (
| |tab| *pattern_in* , *transpose* , *dependency* , *internal_bool* , *pattern_out*
| )

Purpose
*******
We use :math:`F : \B{R}^n \rightarrow \B{R}^m` to denote the
:ref:`glossary@AD Function` corresponding to
the operation sequence stored in *f* .
Fix :math:`R \in \B{R}^{n \times \ell}` and define the function

.. math::

   J(x) = F^{(1)} ( x ) * R

Given the :ref:`glossary@Sparsity Pattern` for :math:`R`,
``for_jac_sparsity`` computes a sparsity pattern for :math:`J(x)`.

x
*
Note that the sparsity pattern :math:`J(x)` corresponds to the
operation sequence stored in *f* and does not depend on
the argument *x* .
(The operation sequence may contain
:ref:`CondExp-name` and  :ref:`VecAD-name` operations.)

SizeVector
**********
The type *SizeVector* is a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
``size_t`` .

f
*
The object *f* has prototype

   ``ADFun`` < *Base* > *f*

The :ref:`ADFun-name` object *f* is not ``const`` .
After a call to ``for_jac_sparsity`` , a sparsity pattern
for each of the variables in the operation sequence
is held in *f* for possible later use during
reverse Hessian sparsity calculations.

size_forward_bool
=================
After ``for_jac_sparsity`` , if *k* is a ``size_t`` object,

   *k* = *f* . ``size_forward_bool`` ()

sets *k* to the amount of memory (in unsigned character units)
used to store the
:ref:`glossary@Sparsity Pattern@Boolean Vector`
sparsity patterns.
If *internal_bool* if false, *k* will be zero.
Otherwise it will be non-zero.
If you do not need this information for :ref:`RevSparseHes-name`
calculations, it can be deleted
(and the corresponding memory freed) using

   *f* . ``size_forward_bool`` (0)

after which *f* . ``size_forward_bool`` () will return zero.

size_forward_set
================
After ``for_jac_sparsity`` , if *k* is a ``size_t`` object,

   *k* = *f* . ``size_forward_set`` ()

sets *k* to the amount of memory (in unsigned character units)
used to store the
:ref:`glossary@Sparsity Pattern@Vector of Sets`
sparsity patterns.
If *internal_bool* if true, *k* will be zero.
Otherwise it will be non-zero.
If you do not need this information for future :ref:`rev_hes_sparsity-name`
calculations, it can be deleted
(and the corresponding memory freed) using

   *f* . ``size_forward_set`` (0)

after which *f* . ``size_forward_set`` () will return zero.

pattern_in
**********
The argument *pattern_in* has prototype

   ``const sparse_rc`` < *SizeVector* >& *pattern_in*

see :ref:`sparse_rc-name` .
If *transpose* it is false (true),
*pattern_in* is a sparsity pattern for :math:`R` (:math:`R^\R{T}`).

transpose
*********
This argument has prototype

   ``bool`` *transpose*

See :ref:`for_jac_sparsity@pattern_in` above and
:ref:`for_jac_sparsity@pattern_out` below.

dependency
**********
This argument has prototype

   ``bool`` *dependency*

see :ref:`for_jac_sparsity@pattern_out` below.

internal_bool
*************
This argument has prototype

   ``bool`` *internal_bool*

If this is true, calculations are done with sets represented by a vector
of boolean values. Otherwise, a vector of sets of integers is used.

pattern_out
***********
This argument has prototype

   ``sparse_rc`` < *SizeVector* >& *pattern_out*

This input value of *pattern_out* does not matter.
If *transpose* it is false (true),
upon return *pattern_out* is a sparsity pattern for
:math:`J(x)` (:math:`J(x)^\R{T}`).
If *dependency* is true, *pattern_out* is a
:ref:`dependency.cpp@Dependency Pattern`
instead of sparsity pattern.

Sparsity for Entire Jacobian
****************************
Suppose that
:math:`R` is the :math:`n \times n` identity matrix.
In this case, *pattern_out* is a sparsity pattern for
:math:`F^{(1)} ( x )`  ( :math:`F^{(1)} (x)^\R{T}` )
if *transpose* is false (true).

Example
*******
{xrst_toc_hidden
   example/sparse/for_jac_sparsity.cpp
}
The file
:ref:`for_jac_sparsity.cpp-name`
contains an example and test of this operation.

{xrst_end for_jac_sparsity}
-----------------------------------------------------------------------------
*/
# include <cppad/core/ad_fun.hpp>
# include <cppad/local/sparse/internal.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

/*!
Forward Jacobian sparsity patterns.

\tparam Base
is the base type for this recording.

\tparam SizeVector
is the simple vector with elements of type size_t that is used for
row, column index sparsity patterns.

\param pattern_in
is the sparsity pattern for for R or R^T depending on transpose.

\param transpose
Is the input and returned sparsity pattern transposed.

\param dependency
Are the derivatives with respect to left and right of the expression below
considered to be non-zero:
\code
   CondExpRel(left, right, if_true, if_false)
\endcode
This is used by the optimizer to obtain the correct dependency relations.

\param internal_bool
If this is true, calculations are done with sets represented by a vector
of boolean values. Othewise, a vector of standard sets is used.

\param pattern_out
The value of transpose is false (true),
the return value is a sparsity pattern for J(x) ( J(x)^T ) where
\f[
   J(x) = F^{(1)} (x) * R
\f]
Here F is the function corresponding to the operation sequence
and x is any argument value.
*/
template <class Base, class RecBase>
template <class SizeVector>
void ADFun<Base,RecBase>::for_jac_sparsity(
   const sparse_rc<SizeVector>& pattern_in       ,
   bool                         transpose        ,
   bool                         dependency       ,
   bool                         internal_bool    ,
   sparse_rc<SizeVector>&       pattern_out      )
{
   // used to identify the RecBase type in calls to sweeps
   RecBase not_used_rec_base(0.0);
   //
   // number or rows, columns, and non-zeros in pattern_in
   size_t nr_in  = pattern_in.nr();
   size_t nc_in  = pattern_in.nc();
   //
   size_t n   = nr_in;
   size_t ell = nc_in;
   if( transpose )
      std::swap(n, ell);
   //
   CPPAD_ASSERT_KNOWN(
      n == Domain() ,
      "for_jac_sparsity: number rows in R "
      "is not equal number of independent variables."
   );
   bool zero_empty  = true;
   bool input_empty = true;
   if( internal_bool )
   {  // allocate memory for bool sparsity calculation
      // (sparsity pattern is emtpy after a resize)
      for_jac_sparse_pack_.resize(num_var_tape_, ell);
      for_jac_sparse_set_.resize(0, 0);
      //
      // set sparsity patttern for independent variables
      local::sparse::set_internal_pattern(
         zero_empty            ,
         input_empty           ,
         transpose             ,
         ind_taddr_            ,
         for_jac_sparse_pack_  ,
         pattern_in
      );

      // compute sparsity for other variables
      local::sweep::for_jac(
         &play_,
         dependency,
         n,
         num_var_tape_,
         for_jac_sparse_pack_,
         not_used_rec_base

      );
      // set the output pattern
      local::sparse::get_internal_pattern(
         transpose, dep_taddr_, for_jac_sparse_pack_, pattern_out
      );
   }
   else
   {
      // allocate memory for set sparsity calculation
      // (sparsity pattern is emtpy after a resize)
      for_jac_sparse_set_.resize(num_var_tape_, ell);
      for_jac_sparse_pack_.resize(0, 0);
      //
      // set sparsity patttern for independent variables
      local::sparse::set_internal_pattern(
         zero_empty            ,
         input_empty           ,
         transpose             ,
         ind_taddr_            ,
         for_jac_sparse_set_   ,
         pattern_in
      );

      // compute sparsity for other variables
      local::sweep::for_jac(
         &play_,
         dependency,
         n,
         num_var_tape_,
         for_jac_sparse_set_,
         not_used_rec_base

      );
      // get the ouput pattern
      local::sparse::get_internal_pattern(
         transpose, dep_taddr_, for_jac_sparse_set_, pattern_out
      );
   }
   return;
}


} // END_CPPAD_NAMESPACE
# endif
