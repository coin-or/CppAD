# ifndef CPPAD_CORE_BOOL_FUN_HPP
# define CPPAD_CORE_BOOL_FUN_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin bool_fun}

AD Boolean Functions
####################

Syntax
******
| ``CPPAD_BOOL_UNARY`` ( *Base* , *unary_name* )
| *b* = *unary_name* ( *u* )
| *b* = *unary_name* ( *x* )
| ``CPPAD_BOOL_BINARY`` ( *Base* , *binary_name* )
| *b* = *binary_name* ( *u* , *v* )
| *b* = *binary_name* ( *x* , *y* )

Purpose
*******
Create a ``bool`` valued function that has ``AD`` < *Base* > arguments.

unary_name
**********
This is the name of the ``bool`` valued function with one argument
(as it is used in the source code).
The user must provide a version of *unary_name* where
the argument has type *Base* .
CppAD uses this to create a version of *unary_name* where the
argument has type ``AD`` < *Base* > .

u
*
The argument *u* has prototype

   ``const`` *Base* & *u*

It is the value at which the user provided version of *unary_name*
is to be evaluated.
It is also used for the first argument to the
user provided version of *binary_name* .

x
*
The argument *x* has prototype

   ``const AD`` < *Base* > & *x*

It is the value at which the CppAD provided version of *unary_name*
is to be evaluated.
It is also used for the first argument to the
CppAD provided version of *binary_name* .

b
*
The result *b* has prototype

   ``bool`` *b*

Create Unary
************
The preprocessor macro invocation

   ``CPPAD_BOOL_UNARY`` ( *Base* , *unary_name* )

defines the version of *unary_name* with a ``AD`` < *Base* >
argument.
This can with in a namespace
(not the ``CppAD`` namespace)
but must be outside of any routine.

binary_name
***********
This is the name of the ``bool`` valued function with two arguments
(as it is used in the source code).
The user must provide a version of *binary_name* where
the arguments have type *Base* .
CppAD uses this to create a version of *binary_name* where the
arguments have type ``AD`` < *Base* > .

v
*
The argument *v* has prototype

   ``const`` *Base* & *v*

It is the second argument to
the user provided version of *binary_name* .

y
*
The argument *x* has prototype

   ``const AD`` < *Base* > & *y*

It is the second argument to
the CppAD provided version of *binary_name* .

Create Binary
*************
The preprocessor macro invocation

   ``CPPAD_BOOL_BINARY`` ( *Base* , *binary_name* )

defines the version of *binary_name* with ``AD`` < *Base* >
arguments.
This can with in a namespace
(not the ``CppAD`` namespace)
but must be outside of any routine.

Operation Sequence
******************
The result of this operation is not an
:ref:`glossary@AD of Base` object.
Thus it will not be recorded as part of an
AD of *Base*
:ref:`operation sequence<glossary@Operation@Sequence>` .

Example
*******
{xrst_toc_hidden
   example/general/bool_fun.cpp
}
The file
:ref:`bool_fun.cpp-name`
contains an example and test of these operations.

Deprecated 2007-07-31
*********************
The preprocessor symbols ``CppADCreateUnaryBool``
and ``CppADCreateBinaryBool`` are defined to be the same as
``CPPAD_BOOL_UNARY`` and ``CPPAD_BOOL_BINARY`` respectively
(but their use is deprecated).

{xrst_end bool_fun}
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file bool_fun.hpp
Routines and macros that implement functions from AD<Base> to bool.
*/

/*!
Macro that defines a unary function <tt>bool F(AD<Base> x)</tt>
using <tt>bool F(Base x)</tt>.

\param Base
base for the AD type of arguments to this unary bool valued function.

\param unary_name
name of this unary function; i.e., F.
*/
# define CPPAD_BOOL_UNARY(Base, unary_name)                        \
     inline bool unary_name (const CppAD::AD<Base> &x)             \
     {                                                             \
          return CppAD::AD<Base>::UnaryBool(unary_name, x);        \
     }

/*!
Deprecated name for CPPAD_BOOL_UNARY
*/
# define CppADCreateUnaryBool  CPPAD_BOOL_UNARY

/*!
Link a function name, and AD value pair to function call with base argument
and bool retrun value.

\param FunName
is the name of the function that we are linking.

\param x
is the argument where we are evaluating the function.
*/
template <class Base>
bool AD<Base>::UnaryBool(
   bool FunName(const Base &x),
   const AD<Base> &x
)
{
   return FunName(x.value_);
}

/*!
Macro that defines a binary function <tt>bool F(AD<Base> x, AD<Base> y)</tt>
using <tt>bool F(Base x, Base y)</tt>.

\param Base
base for the AD type of arguments to this binary bool valued function.

\param binary_name
name of this binary function; i.e., F.
*/

# define CPPAD_BOOL_BINARY(Base, binary_name)                      \
     inline bool binary_name (                                     \
          const CppAD::AD<Base> &x, const CppAD::AD<Base> &y)      \
     {                                                             \
          return CppAD::AD<Base>::BinaryBool(binary_name, x, y);   \
     }
/*!
Deprecated name for CPPAD_BOOL_BINARY
*/
# define CppADCreateBinaryBool CPPAD_BOOL_BINARY


/*!
Link a function name, and two AD values to function call with base arguments
and bool retrun value.

\param FunName
is the name of the function that we are linking.

\param x
is the first argument where we are evaluating the function at.

\param y
is the second argument where we are evaluating the function at.
*/
template <class Base>
bool AD<Base>::BinaryBool(
   bool FunName(const Base &x, const Base &y),
   const AD<Base> &x, const AD<Base> &y
)
{
   return FunName(x.value_, y.value_);
}

} // END_CPPAD_NAMESPACE
# endif
