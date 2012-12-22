/* $Id$ */
# ifndef CPPAD_LIMITS_INCLUDED
# define CPPAD_LIMITS_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
------------------------------------------------------------------------------
$begin limits$$
$spell
	std
	eps
	CppAD
	namespace
	const
$$
$index limits, AD$$
$index AD, limits$$
$index epsilon, AD$$
$index limit, max$$
$index limit, min$$
$index limit, epsilon$$

$section Numeric Limits For an AD Type$$

$head Syntax$$
$icode%eps% = numeric_limits::epsilon<%Float%>()
%$$
$icode%min% = numeric_limits::min<%Float%>()
%$$
$icode%max% = numeric_limits::min<%Float%>()
%$$

$head Purpose$$
Obtain the value of some of the C++ standard numeric limits
using the CppAD namespace version.
These are all functions and have the prototype
$codei%
	%Type% numeric_limits<%Type%>::%fun%(%void%)
%$$
where $icode fun$$ is $code epsilon$$, $code min$$, or $code max$$.
Note that this is different than the standard namespace prototype
for some types; e.g., $code std::complex<double>$$.

$head Type$$
These functions are defined for all $codei%AD<%Base%>%$$,
and for all corresponding $icode Base$$ types.

$head eps$$
The result $icode eps$$ is equal to machine epsilon and has prototype
$codei%
	%Type% eps
%$$

$head min$$
The result $icode min$$ is equal to 
the minimum positive normalized value and has prototype
$codei%
	%Type% min
%$$

$head max$$
The result $icode max$$ is equal to 
the maximum finite value and has prototype
$codei%
	%Type% max
%$$

$head Example$$
$children%
	example/limits.cpp
%$$
The file
$cref limits.cpp$$
contains an example and test of these functions.

$end 
------------------------------------------------------------------------------
*/
# include <iostream>

# include <cppad/configure.hpp>
# include <cppad/local/define.hpp>
# include <cppad/local/cppad_assert.hpp>
# include <cppad/local/declare_ad.hpp>

CPPAD_BEGIN_NAMESPACE
/*!
\defgroup limits_hpp limits.hpp
\{
\file limits.hpp
File that defines CppAD numeric_limits for AD types
*/

/// Default value for all undefined numeric_limits types
template <class Type>
class numeric_limits {
public:
	/// machine epsilon
	static Type epsilon(void)
	{	CPPAD_ASSERT_KNOWN(
		false,
		"numeric_limits<Type>::epsilon() is not specialized for this Type"
		);
		return Type(0);
	}
	/// minimum positive normalized value
	static Type min(void)
	{	CPPAD_ASSERT_KNOWN(
		false,
		"numeric_limits<Type>::min() is not specialized for this Type"
		);
		return Type(0);
	}
	/// maximum finite value
	static Type max(void)
	{	CPPAD_ASSERT_KNOWN(
		false,
		"numeric_limits<Type>::max() is not specialized for this Type"
		);
		return Type(0);
	}
};

/// Partial specialization that defines limits for for all AD types
template <class Base>
class numeric_limits< AD<Base> > {
public:
	/// machine epsilon
	static AD<Base> epsilon(void)
	{	return AD<Base>( numeric_limits<Base>::epsilon() ); }
	/// minimum positive normalized value
	static AD<Base> min(void)
	{	return AD<Base>( numeric_limits<Base>::min() ); }
	/// maximum finite value
	static AD<Base> max(void)
	{	return AD<Base>( numeric_limits<Base>::max() ); }
};

/* \} */
CPPAD_END_NAMESPACE
# endif
