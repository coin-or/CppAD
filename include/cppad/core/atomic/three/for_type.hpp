# ifndef CPPAD_CORE_ATOMIC_THREE_FOR_TYPE_HPP
# define CPPAD_CORE_ATOMIC_THREE_FOR_TYPE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_three_for_type}

Atomic Function Forward Type Calculation
########################################

Syntax
******
| *ok* = *afun* . ``for_type`` ( *parameter_x* , *type_x* , *type_y* )

Prototype
=========
{xrst_literal
   // BEGIN_PROTOTYPE
   // END_PROTOTYPE
}

Dependency Analysis
*******************
This calculation is sometimes referred to as a forward dependency analysis.

Usage
*****
This syntax and prototype are used by

   *afun* ( *ax* , *ay* )

where :ref:`atomic_three_ctor@atomic_user@afun`
is a user defined atomic function.

Implementation
**************
This virtual function must be defined by the
:ref:`atomic_three_ctor@atomic_user` class.

Base
****
See :ref:`atomic_three_define@Base` .

parameter_x
***********
See :ref:`atomic_three_define@parameter_x` .

type_x
******
See :ref:`atomic_three_define@type_x` .

type_y
******
This vector has size equal to the number of results for this atomic function;
i.e. *m* = *ay* . ``size`` () .
The input values of the elements of *type_y*
are not specified (must not matter).
Upon return, for :math:`i = 0 , \ldots , m-1`,
*type_y* [ *i* ] is set to one of the following values:

#. It is ``constant_enum`` if *ay* [ *i* ] only depends on
   the arguments that are constants.
#. It is ``dynamic_enum`` if *ay* [ *i* ] depends on
   a dynamic parameter and does not depend on any variables.
#. It is ``variable_enum`` if *ay* [ *i* ] depends on
   a variable.

ok
**
If this calculation succeeded, *ok* is true.
Otherwise, it is false.

Example
*******
The following is an example of a atomic function ``for_type`` definition:
:ref:`get_started.cpp<atomic_three_get_started.cpp@for_type>` .

{xrst_end atomic_three_for_type}
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic/three_for_type.hpp
Third generation atomic type computation.
*/
/*!
Link from atomic_three to type calculation

\param parameter_x [in]
is the value of the parameters in the corresponding function call
afun(ax, ay).

\param type_x [in]
specifies which components of x are
constants, dynamics, and variables

\param type_y [out]
specifies which components of y are
constants, dynamics, and variables
*/
// BEGIN_PROTOTYPE
template <class Base>
bool atomic_three<Base>::for_type(
   const vector<Base>&          parameter_x ,
   const vector<ad_type_enum>&  type_x      ,
   vector<ad_type_enum>&        type_y      )
// END_PROTOTYPE
{  return false; }

} // END_CPPAD_NAMESPACE

# endif
