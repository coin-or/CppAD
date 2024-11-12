# ifndef CPPAD_CORE_ATOMIC_THREE_REV_DEPEND_HPP
# define CPPAD_CORE_ATOMIC_THREE_REV_DEPEND_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_three_rev_depend}

Atomic Function Reverse Dependency Calculation
##############################################

Syntax
******
| *ok* = *afun* . ``rev_depend`` (
| |tab| *parameter_x* , *type_x* , *depend_x* , *depend_y*
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
:ref:`atomic_three_ctor@atomic_user@afun` is
used to define an :ref:`ADFun-name` object *f* ,
and :ref:`f.optimize()<optimize-name>` is used.

Base
****
See :ref:`atomic_three_afun@Base` .

parameter_x
***********
See :ref:`atomic_three_define@parameter_x` .

type_x
******
See :ref:`atomic_three_define@type_x` .

depend_x
********
This vector has size equal to the number of arguments for this atomic function;
i.e. *n* = *ax* . ``size`` () .
The input values of the elements of *depend_x*
are not specified (must not matter).
Upon return, for :math:`j = 0 , \ldots , n-1`,
*depend_x* [ *j* ] is true if the values of interest depend
on the value of :ref:`ax[j]<atomic_three_afun@ax>` in the corresponding
*afun* ( *ax* , *ay* ) call.

Optimize
========
Parameters and variables,
that the values of interest do not depend on,
may get removed by :ref:`optimization<optimize-name>` .
The corresponding values in :ref:`atomic_three_define@parameter_x` ,
and :ref:`atomic_three_forward@taylor_x`
(after optimization has removed them) are not specified.

depend_y
********
This vector has size equal to the number of results for this atomic function;
i.e. *m* = *ay* . ``size`` () .
For :math:`i = 0 , \ldots , m-1`,
*depend_y* [ *i* ] is true if the values of interest depend
on the value of :ref:`ay[i]<atomic_three_afun@ay>` in the corresponding
*afun* ( *ax* , *ay* ) call.

ok
**
If this calculation succeeded, *ok* is true.
Otherwise, it is false.

Contents
********
{xrst_toc_table
   example/atomic_three/rev_depend.cpp
}
Example
*******
The following is an example of a atomic function ``rev_depend`` definition:
:ref:`atomic_three_rev_depend.cpp-name` .

{xrst_end atomic_three_rev_depend}
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic/three_rev_depend.hpp
Third generation atomic type computation.
*/
/*!
Link from atomic_three to reverse dependency calculation

\param parameter_x [in]
is the value of the parameters in the corresponding function call
afun(ax, ay).

\param type_x [in]
is the value for each of the components of x.

\param depend_x [out]
specifies which components of x affect values of interest.

\param depend_y [in]
specifies which components of y affect values of interest.
*/
// BEGIN_PROTOTYPE
template <class Base>
bool atomic_three<Base>::rev_depend(
   const vector<Base>&         parameter_x ,
   const vector<ad_type_enum>& type_x      ,
   vector<bool>&               depend_x    ,
   const vector<bool>&         depend_y    )
// END_PROTOTYPE
{  return false; }

} // END_CPPAD_NAMESPACE

# endif
