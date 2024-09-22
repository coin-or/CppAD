# ifndef CPPAD_CORE_IDENTICAL_HPP
# define CPPAD_CORE_IDENTICAL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/local/define.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file identical.hpp
Check if certain properties is true for any possible AD tape play back.
*/

// ---------------------------------------------------------------------------
/*!
Determine if an AD<Base> object is a parameter, and could never have
a different value during any tape playback.

An AD<Base> object x is identically a parameter if and only if
all of the objects in the following chain are parameters:
\code
   x , x.value , x.value.value , ...
\endcode
In such a case, the value of the object will always be the same
no matter what the independent variable values are at any level.

\param x
values that we are checking for identically a pamameter.

\return
returns true iff x is identically a parameter.
*/
template <class Base>
bool IdenticalCon(const AD<Base> &x)
{  return Constant(x) && IdenticalCon(x.value_); }
// Zero ==============================================================
/*!
Determine if an AD<Base> is equal to zero,
and must be equal zero during any tape playback.

\param x
object that we are checking.

\return
returns true if and only if
 x is equals zero and is identically a parameter \ref CppAD::IdenticalCon.
*/
template <class Base>
bool IdenticalZero(const AD<Base> &x)
{  return Constant(x) && IdenticalZero(x.value_); }
// One ==============================================================
/*!
Determine if an AD<Base> is equal to one,
and must be equal one during any tape playback.

\param x
object that we are checking.

\return
returns true if and only if
 x is equals one and is identically a parameter \ref CppAD::IdenticalCon.
*/
template <class Base>
bool IdenticalOne(const AD<Base> &x)
{  return Constant(x) && IdenticalOne(x.value_); }
// Equal ===================================================================
/*!
Determine if two AD<Base> objects are equal,
and must be equal during any tape playback.

\param x
first of two objects we are checking for equal.

\param y
second of two objects we are checking for equal.

\return
returns true if and only if
the arguments are equal and both identically parameters \ref CppAD::IdenticalCon.
*/
template <class Base>
bool IdenticalEqualCon
(const AD<Base> &x, const AD<Base> &y)
{  bool constant;
   constant  = Constant(x) && Constant(y);
   return constant  & IdenticalEqualCon(x.value_, y.value_);
}
// ==========================================================================

} // END_CPPAD_NAMESPACE
# endif
