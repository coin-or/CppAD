# ifndef CPPAD_CORE_ATOMIC_FOUR_REV_DEPEND_HPP
# define CPPAD_CORE_ATOMIC_FOUR_REV_DEPEND_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_rev_depend}

Atomic Function Reverse Dependency
##################################

Syntax
******
You can define one or the other of the following callbacks,
but you should not define both.

Preferred
=========

| *ok* = *afun* . ``rev_depend`` ( *call_id* ,
| |tab| *ident_zero_x* , *depend_x* , *depend_y*
| )

Deprecated 2022-05-10
=====================

| *ok* = *afun* . ``rev_depend`` ( *call_id* ,
| |tab| *depend_x* , *depend_y*
| )

Prototype
=========
{xrst_literal
   // BEGIN_PROTOTYPE
   // END_PROTOTYPE
}

Dependency Analysis
*******************
This calculation is sometimes referred to as a reverse dependency analysis.

Implementation
**************
This function must be defined if
:ref:`atomic_four_ctor@atomic_user@afun` is
used to define an :ref:`ADFun-name` object *f* ,
and :ref:`f.optimize()<optimize-name>` is used.

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
This can sometimes be used to create more efficient dependency
(fewer true values in *depend_x* ).
If you do not see a way to do this, you can just ignore it.
This argument has size equal to the number of arguments to this
atomic function; i.e. the size of *ax* .
If *ident_zero_x* [ *j* ] is true, the argument *ax* [ *j* ]
is a constant parameter that is identically zero.
An identically zero value times any other value can be treated
as being identically zero.

depend_x
********
This vector has size equal to the number of arguments for this atomic function;
i.e. *n* = *ax* . ``size`` () (see :ref:`atomic_four_call@ax` ).
The input values of the elements of *depend_x*
are not specified (must not matter).
Upon return, for :math:`j = 0 , \ldots , n-1`,
*depend_x* [ *j* ] is true if the values of interest depend
on the value of *ax* [ *j* ] in the corresponding atomic function call.

Optimize
========
Parameters and variables,
that the values of interest do not depend on,
may get removed by :ref:`optimization<optimize-name>` .
The corresponding values in
:ref:`atomic_four_forward@taylor_x`
(after optimization has removed them) are currently zero,
but perhaps these should be changed back to nan.

depend_y
********
This vector has size equal to the number of results for this atomic function;
i.e. *m* = *ay* . ``size`` () (see :ref:`atomic_four_call@ay` ).
For :math:`i = 0 , \ldots , m-1`,
*depend_y* [ *i* ] is true if the values of interest depend
on the value of *ay* [ *i* ] in the corresponding atomic function call.

ok
**
If this calculation succeeded, *ok* is true.
Otherwise, it is false.

Example
*******
The following is an example ``rev_depend`` definition taken from
:ref:`atomic_four_norm_sq.cpp-name` :
{xrst_literal
   example/atomic_four/norm_sq.cpp
   // BEGIN REV_DEPEND
   // END REV_DEPEND
}

{xrst_end atomic_four_rev_depend}
-----------------------------------------------------------------------------
*/
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN_PROTOTYPE
template <class Base>
bool atomic_four<Base>::rev_depend(
   size_t                      call_id      ,
   const vector<bool>&         ident_zero_x ,
   vector<bool>&               depend_x     ,
   const vector<bool>&         depend_y     )
// END_PROTOTYPE
{  return false; }

// deprecated version
template <class Base>
bool atomic_four<Base>::rev_depend(
   size_t                      call_id     ,
   vector<bool>&               depend_x    ,
   const vector<bool>&         depend_y    )
// end deprecated version
{  return false; }

} // END_CPPAD_NAMESPACE

# endif
