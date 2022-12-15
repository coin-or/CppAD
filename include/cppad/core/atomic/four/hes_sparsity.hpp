# ifndef CPPAD_CORE_ATOMIC_FOUR_HES_SPARSITY_HPP
# define CPPAD_CORE_ATOMIC_FOUR_HES_SPARSITY_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/core/atomic/four/devel/hes_sparsity.hpp>
/*
{xrst_begin atomic_four_hes_sparsity}

Atomic Function Hessian Sparsity Patterns
#########################################

Syntax
******

Preferred
=========

| *ok* = *afun* . ``hes_sparsity`` ( *call_id* ,
| |tab| *ident_zero_x* , *select_x* , *select_y* , *pattern_out*
| )

Deprecated 2022-05-16
=====================

| *ok* = *afun* . ``hes_sparsity`` ( *call_id* ,
| |tab| *select_x* , *select_y* , *pattern_out*
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
and Hessian sparsity patterns are computed for *f* .

Base
****
See :ref:`atomic_four_call@Base` .

vector
******
is the :ref:`CppAD_vector-name` template class.

call_id
*******
See :ref:`atomic_four_call@call_id` .

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
*k* such that either of the following hold:

| |tab| *pattern_out* . ``row`` ()[ *k* ] == *j*
| |tab| *pattern_out* . ``col`` ()[ *k* ] == *j*

.

select_y
********
This argument has size equal to the number of results to this
atomic function; i.e. the size of *ay* .
It specifies which range component functions :math:`g_i (x)` are included in
of *pattern_out* .

pattern_out
***********
This input value of *pattern_out* does not matter.
Upon return it is the union,
with respect to *i* such that *select_y* [ *i* ] is true,
of the sparsity pattern for Hessian of :math:`g_i (x)`.
To be specific, there are non-negative indices
*r* , *c* , and *k* such that

| |tab| *pattern_out* . ``row`` ()[ *k* ] == *r*
| |tab| *pattern_out* . ``col`` ()[ *k* ] == *c*

if and only if
there exists an index *i* such that,
*select_y* [ *i* ] is true,
*select_x* [ *r* ] is true,
*select_x* [ *c* ] is true,
and

.. math::

   \partial_{x(r)} \partial_{x(c)} g_i(x)

is possibly non-zero.
Note that the sparsity pattern should be symmetric.

ok
**
If this calculation succeeded, *ok* is true.
Otherwise it is false.

Example
*******
The following is an example ``hes_sparsity`` definition taken from
:ref:`atomic_four_norm_sq.cpp-name` :
{xrst_literal
   example/atomic_four/norm_sq.cpp
   // BEGIN HES_SPARSITY
   // END HES_SPARSITY
}

{xrst_end atomic_four_hes_sparsity}
-----------------------------------------------------------------------------
*/
namespace CppAD { // BEGIN_CPPAD_NAMESPACE
// BEGIN_PROTOTYPE
template <class Base>
bool atomic_four<Base>::hes_sparsity(
   size_t                                  call_id      ,
   const vector<bool>&                     ident_zero_x ,
   const vector<bool>&                     select_x     ,
   const vector<bool>&                     select_y     ,
   sparse_rc< vector<size_t> >&            pattern_out  )
// END_PROTOTYPE
{  return false; }
//
// deprecated version
template <class Base>
bool atomic_four<Base>::hes_sparsity(
   size_t                                  call_id      ,
   const vector<bool>&                     select_x     ,
   const vector<bool>&                     select_y     ,
   sparse_rc< vector<size_t> >&            pattern_out  )
{  return false; }

} // END_CPPAD_NAMESPACE

# endif
