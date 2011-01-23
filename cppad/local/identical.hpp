/* $Id$ */
# ifndef CPPAD_IDENTICAL_INCLUDED
# define CPPAD_IDENTICAL_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/local/define.hpp>

CPPAD_BEGIN_NAMESPACE
/*!
\file identical.hpp
Check if certain properties is true for any possible AD tape play back.
*/

// Parameter ==============================================================
/*!
return true, used by <tt>IdenticalPar(AD<float>)</tt>.

\param x
not used.

\return
returns true.
*/
inline bool IdenticalPar(const float &x)
{	return true; }
// ---------------------------------------------------------------------------
/*!
return true, used by <tt>IdenticalPar(AD<double>)</tt>.

\param x
not used.

\return
returns true.
*/
inline bool IdenticalPar(const double &x)
{	return true; }
// ---------------------------------------------------------------------------
/*!
Determine if an AD<Base> object is a parameter, and could never have 
a different value during any tape playback.

An AD<Base> object \c x is identically a parameter if and only if
all of the objects in the following chain are parameters:
\code
	x , x.value , x.value.value , ...
\endcode
In such a case, the value of the object will always be the same
no matter what the independent variable values are at any level.

\param x
values that we are checking for identically a pamameter.

\return
returns true iff \c x is identically a parameter.
*/
template <class Base>
CPPAD_INLINE bool IdenticalPar(const AD<Base> &x)
{	return Parameter(x) && IdenticalPar(x.value_); }
// Zero ==============================================================
/*!
Check if a float is equal to zero, used by <tt>IdenticalZero(AD<float>)</tt>.

\param x
value we are checking.

\return
returns true iff the argument equals zero.
*/
inline bool IdenticalZero(const float &x)
{	return (x == 0.); }
// --------------------------------------------------------------------------
/*!
Check if a double is equal to zero, used by <tt>IdenticalZero(AD<double>)</tt>.

\param x
value we are checking.

\return
returns true iff the argument equals zero.
*/
inline bool IdenticalZero(const double &x)
{	return (x == 0.); }
// --------------------------------------------------------------------------
/*!
Determine if an AD<Base> is equal to zero,
and must be equal zero during any tape playback.

\param x
object that we are checking.

\return
returns true if and only if
\c x is equals zero and is identically a parameter \ref IdenticalPar.
*/
template <class Base>
CPPAD_INLINE bool IdenticalZero(const AD<Base> &x)
{	return Parameter(x) && IdenticalZero(x.value_); }
// One ==============================================================
/*!
Check if a float is equal to one, used by <tt>IdenticalOne(AD<float>)</tt>.

\param x
value we are checking.

\return
returns true iff the argument equals one.
*/
inline bool IdenticalOne(const float &x)
{	return (x == 1.); }
// --------------------------------------------------------------------------
/*!
Check if a double is equal to one, used by <tt>IdenticalOne(AD<double>)</tt>.

\param x
value we are checking.

\return
returns true iff the argument equals one.
*/
inline bool IdenticalOne(const double &x)
{	return (x == 1.); }
// --------------------------------------------------------------------------
/*!
Determine if an AD<Base> is equal to one,
and must be equal one during any tape playback.

\param x
object that we are checking.

\return
returns true if and only if
\c x is equals one and is identically a parameter \ref IdenticalPar.
*/
template <class Base>
CPPAD_INLINE bool IdenticalOne(const AD<Base> &x)
{	return Parameter(x) && IdenticalOne(x.value_); }
// Equal ===================================================================
/*!
Check if two floats are equal, 
used by <tt>IdenticalEqualPar(AD<float>, AD<float>)</tt>.

\param x
first float we are checking.

\param y
second float we are checking.

\return
returns true iff the arugments are equal.
*/
inline bool IdenticalEqualPar(const float &x, const float &y)
{	return (x == y); }
// --------------------------------------------------------------------------
/*!
Check if two doubles are equal, 
used by <tt>IdenticalEqualPar(AD<double>, AD<double>)</tt>.

\param x
first double we are checking.

\param y
second double we are checking.

\return
returns true iff the arugments are equal.
*/
inline bool IdenticalEqualPar(const double &x, const double &y)
{	return (x == y); }
// --------------------------------------------------------------------------
/*!
Determine if two AD<Base> objects are equal, 
and must be equal during any tape playback.

\param x
first of two objects we are checking for equal.

\param y
second of two objects we are checking for equal.

\return
returns true if and only if
the arguments are equal and both identically parameters \ref IdenticalPar.
*/
template <class Base>
CPPAD_INLINE bool IdenticalEqualPar
(const AD<Base> &x, const AD<Base> &y)
{	bool parameter;
	parameter = ( Parameter(x) & Parameter(y) );
	return parameter  && IdenticalEqualPar(x.value_, y.value_); 
}
// ==========================================================================

CPPAD_END_NAMESPACE
# endif
