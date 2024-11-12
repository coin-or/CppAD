# ifndef CPPAD_CORE_AD_ASSIGN_HPP
# define CPPAD_CORE_AD_ASSIGN_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
------------------------------------------------------------------------------

{xrst_begin ad_assign}

AD Assignment Operator
######################

Syntax
******
| *y* = *x*

Purpose
*******
Assigns the value in *x* to the object *y* .
In either case,

x
*
The argument *x* has prototype

   ``const`` *Type* & *x*

where *Type* is
``VecAD`` < *Base* >:: ``reference`` ,
``AD`` < *Base* > ,
*Base* ,
or any type that has an implicit constructor of the form
*Base* ( *x* ) .

y
*
The target *y* has prototype

   ``AD`` < *Base* > *y*

Example
*******
{xrst_toc_hidden
   example/general/ad_assign.cpp
}
The file :ref:`ad_assign.cpp-name` contain examples and tests of these operations.
It test returns true if it succeeds and false otherwise.

{xrst_end ad_assign}
------------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

/*!
\file ad_assign.hpp
AD<Base> constructors and and copy operations.
*/

/*!
\page AD_default_assign
Use default assignment operator
because they may be optimized better than the code below:
\code
template <class Base>
AD<Base>& AD<Base>::operator=(const AD<Base> &right)
{  value_    = right.value_;
   tape_id_  = right.tape_id_;
   taddr_    = right.taddr_;
   ad_type_  = right.ad_type_;

   return *this;
}
\endcode
*/

/*!
Assignment to Base type value.

\tparam Base
Base type for this AD object.

\param b
is the Base type value being assignment to this AD object.
The tape identifier will be an invalid tape identifier,
so this object is initially a parameter.
*/
template <class Base>
AD<Base>& AD<Base>::operator=(const Base &b)
{  value_   = b;
   tape_id_ = 0;
   //
   CPPAD_ASSERT_UNKNOWN( ! ( Variable(*this) || Dynamic(*this) ) );
   return *this;
}

/*!
Assignment to an ADVec<Base> element drops the vector information.

\tparam Base
Base type for this AD object.
*/
template <class Base>
AD<Base>& AD<Base>::operator=(const VecAD_reference<Base> &x)
{  *this = x.ADBase();
   CPPAD_ASSERT_UNKNOWN( ! Dynamic(*this) );
   return *this;
}

/*!
Assignment from any other type, converts to Base type, and then uses assignment
from Base type.

\tparam Base
Base type for this AD object.

\tparam T
is the the type that is being assigned to AD<Base>.
There must be an assignment for Base from Type.

\param t
is the object that is being assigned to an AD<Base> object.
*/
template <class Base>
template <class T>
AD<Base>& AD<Base>::operator=(const T &t)
{  *this = Base(t);
   CPPAD_ASSERT_UNKNOWN( ! ( Variable(*this) || Dynamic(*this) ) );
   return *this;
}


} // END_CPPAD_NAMESPACE
# endif
