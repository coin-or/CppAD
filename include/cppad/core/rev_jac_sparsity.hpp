# ifndef CPPAD_CORE_REV_JAC_SPARSITY_HPP
# define CPPAD_CORE_REV_JAC_SPARSITY_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin rev_jac_sparsity}

Reverse Mode Jacobian Sparsity Patterns
#######################################

Syntax
******
| *f* . ``rev_jac_sparsity`` (
| |tab| *pattern_in* , *transpose* , *dependency* , *internal_bool* , *pattern_out*
| )

Purpose
*******
We use :math:`F : \B{R}^n \rightarrow \B{R}^m` to denote the
:ref:`glossary@AD Function` corresponding to
the operation sequence stored in *f* .
Fix :math:`R \in \B{R}^{\ell \times m}` and define the function

.. math::

   J(x) = R * F^{(1)} ( x )

Given the :ref:`glossary@Sparsity Pattern` for :math:`R`,
``rev_jac_sparsity`` computes a sparsity pattern for :math:`J(x)`.

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

See :ref:`rev_jac_sparsity@pattern_in` above and
:ref:`rev_jac_sparsity@pattern_out` below.

dependency
**********
This argument has prototype

   ``bool`` *dependency*

see :ref:`rev_jac_sparsity@pattern_out` below.

internal_bool
*************
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
:math:`R` is the :math:`m \times m` identity matrix.
In this case, *pattern_out* is a sparsity pattern for
:math:`F^{(1)} ( x )`  ( :math:`F^{(1)} (x)^\R{T}` )
if *transpose* is false (true).

Example
*******
{xrst_toc_hidden
   example/sparse/rev_jac_sparsity.cpp
}
The file
:ref:`rev_jac_sparsity.cpp-name`
contains an example and test of this operation.

{xrst_end rev_jac_sparsity}
-----------------------------------------------------------------------------
*/
# include <cppad/core/ad_fun.hpp>
# include <cppad/local/sparse/internal.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

/*!
Reverse Jacobian sparsity patterns.

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
of boolean values. Otherwise, a vector of standard sets is used.

\param pattern_out
The value of transpose is false (true),
the return value is a sparsity pattern for J(x) ( J(x)^T ) where
\f[
   J(x) = R * F^{(1)} (x)
\f]
Here F is the function corresponding to the operation sequence
and x is any argument value.
*/
template <class Base, class RecBase>
template <class SizeVector>
void ADFun<Base,RecBase>::rev_jac_sparsity(
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
   size_t ell = nr_in;
   size_t m   = nc_in;
   if( transpose )
      std::swap(ell, m);
   //
   CPPAD_ASSERT_KNOWN(
      m == Range() ,
      "rev_jac_sparsity: number columns in R "
      "is not equal number of dependent variables."
   );
   // number of independent variables
   size_t n = Domain();
   //
   bool zero_empty  = true;
   bool input_empty = true;
   if( internal_bool )
   {  // allocate memory for bool sparsity calculation
      // (sparsity pattern is emtpy after a resize)
      local::sparse::pack_setvec internal_jac;
      internal_jac.resize(num_var_tape_, ell);
      //
      // set sparsity patttern for dependent variables
      local::sparse::set_internal_pattern(
         zero_empty            ,
         input_empty           ,
         ! transpose           ,
         dep_taddr_            ,
         internal_jac          ,
         pattern_in
      );

      // compute sparsity for other variables
      local::sweep::rev_jac(
         &play_,
         dependency,
         n,
         num_var_tape_,
         internal_jac,
         not_used_rec_base

      );
      // get sparstiy pattern for independent variables
      local::sparse::get_internal_pattern(
         ! transpose, ind_taddr_, internal_jac, pattern_out
      );
   }
   else
   {  // allocate memory for bool sparsity calculation
      // (sparsity pattern is emtpy after a resize)
      local::sparse::list_setvec internal_jac;
      internal_jac.resize(num_var_tape_, ell);
      //
      // set sparsity patttern for dependent variables
      local::sparse::set_internal_pattern(
         zero_empty            ,
         input_empty           ,
         ! transpose           ,
         dep_taddr_            ,
         internal_jac          ,
         pattern_in
      );

      // compute sparsity for other variables
      local::sweep::rev_jac(
         &play_,
         dependency,
         n,
         num_var_tape_,
         internal_jac,
         not_used_rec_base

      );
      // get sparstiy pattern for independent variables
      local::sparse::get_internal_pattern(
         ! transpose, ind_taddr_, internal_jac, pattern_out
      );
   }
   return;
}
} // END_CPPAD_NAMESPACE
# endif
