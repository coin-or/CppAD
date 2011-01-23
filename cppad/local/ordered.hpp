/* $Id$ */
# ifndef CPPAD_ORDERED_INCLUDED
# define CPPAD_ORDERED_INCLUDED

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
\file ordered.hpp
Check and AD values ordering properties relative to zero.
*/

// GreaterThanZero ============================================================
/*!
Check if a float is greater than zero, 
used by <tt>GreaterThanZero(AD<float>)</tt>.

\param x
value we are checking.

\return
returns true iff the \c x is greater than zero.
*/
inline bool GreaterThanZero(const float &x)
{	return x > 0.; }
// ---------------------------------------------------------------------------
/*!
Check if a double is greater than zero, 
used by <tt>GreaterThanZero(AD<double>)</tt>.

\param x
value we are checking.

\return
returns true iff the \c x is greater than zero.
*/
inline bool GreaterThanZero(const double &x)
{	return x > 0.; }
// ---------------------------------------------------------------------------
/*!
Check if an AD<Base> is greater than zero.

\param x
value we are checking.

\return
returns true iff the \c x is greater than zero.
*/
template <class Base>
CPPAD_INLINE bool GreaterThanZero(const AD<Base> &x)
{	return GreaterThanZero(x.value_); }
// GreaterThanOrZero =========================================================
/*!
Check if a float is greater than or equal zero, 
used by <tt>GreaterThanOrZero(AD<float>)</tt>.

\param x
value we are checking.

\return
returns true iff the \c x is greater than or equal zero.
*/
inline bool GreaterThanOrZero(const float &x)
{	return x >= 0.; }
// ---------------------------------------------------------------------------
/*!
Check if a double is greater than or equal zero, 
used by <tt>GreaterThanOrZero(AD<double>)</tt>.

\param x
value we are checking.

\return
returns true iff the \c x is greater than or equal zero.
*/
inline bool GreaterThanOrZero(const double &x)
{	return x >= 0.; }
// ---------------------------------------------------------------------------
/*!
Check if an AD<Base> is greater than or equal zero.

\param x
value we are checking.

\return
returns true iff the \c x is greater than or equal zero.
*/
template <class Base>
CPPAD_INLINE bool GreaterThanOrZero(const AD<Base> &x)
{	return GreaterThanOrZero(x.value_); }
// LessThanZero ============================================================
/*!
Check if a float is less than zero, 
used by <tt>LessThanZero(AD<float>)</tt>.

\param x
value we are checking.

\return
returns true iff the \c x is less than zero.
*/
inline bool LessThanZero(const float &x)
{	return x < 0.; }
// ---------------------------------------------------------------------------
/*!
Check if a double is less than zero, 
used by <tt>LessThanZero(AD<double>)</tt>.

\param x
value we are checking.

\return
returns true iff the \c x is less than zero.
*/
inline bool LessThanZero(const double &x)
{	return x < 0.; }
// ---------------------------------------------------------------------------
/*!
Check if an AD<Base> is less than zero.

\param x
value we are checking.

\return
returns true iff the \c x is less than zero.
*/
template <class Base>
CPPAD_INLINE bool LessThanZero(const AD<Base> &x)
{	return LessThanZero(x.value_); }
// LessThanOrZero =========================================================
/*!
Check if a float is less than or equal zero, 
used by <tt>LessThanOrZero(AD<float>)</tt>.

\param x
value we are checking.

\return
returns true iff the \c x is less than or equal zero.
*/
inline bool LessThanOrZero(const float &x)
{	return x <= 0.; }
// ---------------------------------------------------------------------------
/*!
Check if a double is less than or equal zero, 
used by <tt>LessThanOrZero(AD<double>)</tt>.

\param x
value we are checking.

\return
returns true iff the \c x is less than or equal zero.
*/
inline bool LessThanOrZero(const double &x)
{	return x <= 0.; }
// ---------------------------------------------------------------------------
/*!
Check if an AD<Base> is less than or equal zero.

\param x
value we are checking.

\return
returns true iff the \c x is less than or equal zero.
*/
template <class Base>
CPPAD_INLINE bool LessThanOrZero(const AD<Base> &x)
{	return LessThanOrZero(x.value_); }
// ============================================================================
CPPAD_END_NAMESPACE
# endif

