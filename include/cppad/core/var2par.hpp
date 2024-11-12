# ifndef CPPAD_CORE_VAR2PAR_HPP
# define CPPAD_CORE_VAR2PAR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
------------------------------------------------------------------------------

{xrst_begin Var2Par}

Convert an AD Variable or Dynamic Parameter to a Constant
#########################################################

Syntax
******
| *y* = ``Var2Par`` ( *x* )

See Also
********
:ref:`value-name`

Purpose
*******
Returns a
:ref:`constant parameter<glossary@Parameter@Constant>` *y*
with the same value as *x* .

x
*
The argument *x* has prototype

   ``const AD`` < *Base* > & ``x``

The argument *x* may be a
variable, dynamic parameter, or constant parameter.

y
*
The result *y* has prototype

   ``AD`` < *Base* > & ``y``

and is a constant parameter.

Example
*******
{xrst_toc_hidden
   example/general/var2par.cpp
}
The file
:ref:`var2par.cpp-name`
contains an example and test of this operation.

{xrst_end Var2Par}
------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
AD<Base> Var2Par(const AD<Base> &x)
{  AD<Base> y(x.value_);
   return y;
}


template <class Base>
CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
AD<Base> Var2Par(const VecAD_reference<Base> &x)
{  AD<Base> y(x.ADBase());
   y.id_ = 0;
}


} // END CppAD namespace

# endif
