# ifndef CPPAD_CORE_ATOMIC_FOUR_FOR_TYPE_HPP
# define CPPAD_CORE_ATOMIC_FOUR_FOR_TYPE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_for_type}

Atomic Function Forward Type Calculation
########################################

Syntax
******
| *ok* = *afun* . ``for_type`` ( *call_id* , *type_x* , *type_y* )

Prototype
*********
{xrst_literal
   // BEGIN_PROTOTYPE
   // END_PROTOTYPE
}

Dependency Analysis
*******************
This calculation is sometimes referred to as a forward dependency analysis.

Usage
*****
This syntax and prototype are used a
:ref:`call<atomic_four_call-name>` to an atomic function.

Implementation
**************
This virtual function must be defined by the
:ref:`atomic_four_ctor@atomic_user` derived class.

vector
******
is the :ref:`CppAD_vector-name` template class.

Base
****
See :ref:`atomic_four_call@Base` .

call_id
*******
See :ref:`atomic_four_call@call_id` .

ad_type
*******
The type ``CppAD::ad_type_enum``
is used to specify if an AD object is a
:ref:`constant parameter<glossary@Parameter@Constant>`
:ref:`dynamic parameter<glossary@Parameter@Dynamic>`
or :ref:`glossary@Variable` .
It has the following possible values:

.. csv-table::
   :widths: auto

   *ad_type_enum*,Meaning
   ``identical_zero_enum``,identically zero
   ``constant_enum``,constant parameter
   ``dynamic_enum``,dynamic parameter
   ``variable_enum``,variable

In addition,

   ``identical_zero_enum < constant_enum < dynamic_enum < variable_enum``

A value that is identically zero is also a constant parameter.
In CppAD, multiplication of a variable by a value that is identically zero
is sometimes treated like :ref:`azmul-title`.
This avoids having to record the operation.

type_x
******
This vector has size equal to the number of arguments in the
atomic function call; i.e., the size of
:ref:`atomic_four_call@ax` which we denote by *n* .
For *j* =0,..., *n* ``-1`` , *type_x* [ *j* ]
is the type of *ax* [ *j* ] .

type_y
******
This vector has size equal to the number of results in the
atomic function call; i.e., the size of
:ref:`atomic_four_call@ay` which we denote by *m* .
The input values of the elements of *type_y*
are not specified (must not matter).
Upon return, for :math:`i = 0 , \ldots , m-1`,
*type_y* [ *i* ] is set to one of the following values:

#. It is ``identical_zero_enum`` if *ay* [ *i* ] is
   :ref:`identically zero<base_identical@Identical>` .
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
The following is an example ``for_type`` definition taken from
:ref:`atomic_four_norm_sq.cpp-name` :
{xrst_literal
   example/atomic_four/norm_sq.cpp
   // BEGIN FOR_TYPE
   // END FOR_TYPE
}

{xrst_end atomic_four_for_type}
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN_PROTOTYPE
template <class Base>
bool atomic_four<Base>::for_type(
   size_t                       call_id     ,
   const vector<ad_type_enum>&  type_x      ,
   vector<ad_type_enum>&        type_y      )
// END_PROTOTYPE
{  return false; }

} // END_CPPAD_NAMESPACE

# endif
