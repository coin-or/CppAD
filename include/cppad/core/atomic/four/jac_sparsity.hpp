# ifndef CPPAD_CORE_ATOMIC_FOUR_JAC_SPARSITY_HPP
# define CPPAD_CORE_ATOMIC_FOUR_JAC_SPARSITY_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/core/atomic/four/devel/jac_sparsity.hpp>

/*
{xrst_begin atomic_four_jac_sparsity}

Atomic Function Jacobian Sparsity Patterns
##########################################

Syntax
******
| You can define one or the other of the following callbacks,
| but you should not define both.

Preferred
=========

| *ok* = *afun* . ``jac_sparsity`` ( *call_id* ,
| |tab| *dependency* , *ident_zero_x* , *select_x* , *select_y* , *pattern_out*
| )

Deprecated 2022-05-10
=====================

| *ok* = *afun* . ``jac_sparsity`` (
| |tab| *dependency* , *call_id* , *select_x* , *select_y* , *pattern_out*
| )

Prototype
*********
{xrst_literal
   // BEGIN_PROTOTYPE
   // END_PROTOTYPE
}

Implementation
**************
This function must be defined if
:ref:`atomic_four_ctor@atomic_user@afun` is
used to define an :ref:`ADFun-name` object *f* ,
and Jacobian sparsity patterns are computed for *f* .
(Computing Hessian sparsity patterns
requires Jacobian sparsity patterns.)

Base
****
See :ref:`atomic_four_call@Base` .

vector
******
is the :ref:`CppAD_vector-name` template class.

call_id
*******
See :ref:`atomic_four_call@call_id` .

dependency
**********
If *dependency* is true,
then *pattern_out* is a
:ref:`dependency.cpp@Dependency Pattern`
for this atomic function.
Otherwise it is a
:ref:`glossary@Sparsity Pattern` for the
derivative of the atomic function.

ident_zero_x
************
This can sometimes be used to create more efficient sparsity patterns.
If you do not see a way to do this, you can just ignore it.
This argument has size equal to the number of arguments to this
atomic function; i.e. the size of *ax* .
If *ident_zero_x* [ *j* ] is true, the argument *ax* [ *j* ]
is a constant parameter that is identically zero.
An identically zero value times any other value can be treated
as being identically zero.

select_x
********
This argument has size equal to the number of arguments to this
atomic function; i.e. the size of *ax* .
It specifies which domain components are included in
the calculation of *pattern_out* .
If *select_x* [ *j* ] is false, then there will be no indices
*k* such that

   *pattern_out* . ``col`` ()[ *k* ] == *j*

.
If *select_x* [ *j* ] is true, the argument *ax* [ *j* ]
is a variable and *ident_zero_x* [ *j* ] will be false.

select_y
********
This argument has size equal to the number of results to this
atomic function; i.e. the size of *ay* .
It specifies which range components are included in
the calculation of *pattern_out* .
If *select_y* [ *i* ] is false, then there will be no indices
*k* such that

   *pattern_out* . ``row`` ()[ *k* ] == *i*

.

pattern_out
***********
This input value of *pattern_out* does not matter.
Upon return it is a
dependency or sparsity pattern for the Jacobian of :math:`g(x)`,
the function corresponding to
:ref:`atomic_four_ctor@atomic_user@afun` .
To be specific, there are non-negative indices
*i* , *j* , *k* such that

| |tab| *pattern_out* . ``row`` ()[ *k* ] == *i*
| |tab| *pattern_out* . ``col`` ()[ *k* ] == *j*

if and only if
*select_x* [ *j* ] is true,
*select_y* [ *j* ] is true,
and :math:`g_i(x)` depends on the value of :math:`x_j`
(and the partial of :math:`g_i(x)` with respect to
:math:`x_j` is possibly non-zero).

ok
**
If this calculation succeeded, *ok* is true.
Otherwise it is false.

Example
*******
The following is an example ``jac_sparsity`` definition taken from
:ref:`atomic_four_norm_sq.cpp-name` :
{xrst_literal
   example/atomic_four/norm_sq.cpp
   // BEGIN JAC_SPARSITY
   // END JAC_SPARSITY
}

{xrst_end atomic_four_jac_sparsity}
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// BEGIN_PROTOTYPE
template <class Base>
bool atomic_four<Base>::jac_sparsity(
   size_t                                  call_id      ,
   bool                                    dependency   ,
   const vector<bool>&                     ident_zero_x ,
   const vector<bool>&                     select_x     ,
   const vector<bool>&                     select_y     ,
   sparse_rc< vector<size_t> >&            pattern_out  )
// END_PROTOTYPE
{  return false; }
//
// deprecated version of jac_sparsity callback
template <class Base>
bool atomic_four<Base>::jac_sparsity(
   size_t                                  call_id      ,
   bool                                    dependency   ,
   const vector<bool>&                     select_x     ,
   const vector<bool>&                     select_y     ,
   sparse_rc< vector<size_t> >&            pattern_out  )
{  return false; }
} // END_CPPAD_NAMESPACE

# endif
