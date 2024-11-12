# ifndef CPPAD_CORE_EPSILON_HPP
# define CPPAD_CORE_EPSILON_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
------------------------------------------------------------------------------
{xrst_begin epsilon app}

Machine Epsilon For AD Types
############################

Deprecated 2012-06-17
*********************
This routine has been deprecated.
You should use the :ref:`numeric_limits-name` ``epsilon`` instead.

Syntax
******
| *eps* = ``epsilon`` < *Float* >()

Purpose
*******
Obtain the value of machine epsilon corresponding
to the type *Float* .

Float
*****
this type can either be ``AD`` < *Base* > ,
or it can be *Base* for any ``AD`` < *Base* > type.

eps
***
The result *eps* has prototype

   *Float* ``eps``

{xrst_end epsilon}
------------------------------------------------------------------------------
*/

namespace CppAD {

   template <class Type>
   inline Type epsilon(void)
   {  return Type ( numeric_limits<Type>::epsilon() ); }

}
# endif
