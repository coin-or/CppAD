// $Id:$
# ifndef CPPAD_ACOSH_INCLUDED
# define CPPAD_ACOSH_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
-------------------------------------------------------------------------------
$begin acosh$$

$section The Inverse Hyperbolic Cosine Function$$
$spell
	acosh
	const
	Vec
	std
	cmath
	CppAD
$$

$head Syntax$$
$icode%y% = acosh(%x%)%$$


$head Description$$
Returns the value of the inverse hyperbolic cosine function which is defined
by $icode%x% == cosh(%y%)%$$.


$head x$$
The argument $icode x$$, and the result $icode y$$
have one of the following paris of prototypes:
$codei%
	const float%%                  &%x%,     float%%    %y%
	const double%%                 &%x%,     double%%   %y%
	const AD<%Base%>               &%x%,     AD<%Base%> %y%
	const VecAD<%Base%>::reference &%x%,     AD<%Base%> %y%
%$$


$head CPPAD_USE_CPLUSPLUS_2011$$
This preprocessor symbol is one if
the function $codei%std::acosh(double %x%)%$$ is defined the in the
include file $code <cmath>$$.
Otherwise this preprocessor symbol is zero.
If this preprocessor symbols is one,
CppAD uses the compiler's version of $code acosh$$
and it corresponds to an $cref/atomic/glossary/Operation/Atomic/$$ operation.

$head Other$$
If the function $codei%std::acosh(double %x%)%$$ is not defined,
CppAD uses the representation
$latex \[
\R{acosh} (x) = \log \left( x + \sqrt{ x^2 - 1 } \right)
\] $$
to compute this function.

$head Example$$
$children%
	example/acosh.cpp
%$$
The file
$cref acosh.cpp$$
contains an example and test of this function.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/
# include <cppad/configure.hpp>
# if ! CPPAD_USE_CPLUSPLUS_2011

// BEGIN CppAD namespace
namespace CppAD {

template <class Type>
Type acosh_template(const Type &x)
{	return CppAD::log( x + CppAD::sqrt( x * x - Type(1) ) );
}

inline float acosh(const float &x)
{	return acosh_template(x); }

inline double acosh(const double &x)
{	return acosh_template(x); }

template <class Base>
inline AD<Base> acosh(const AD<Base> &x)
{	return acosh_template(x); }

template <class Base>
inline AD<Base> acosh(const VecAD_reference<Base> &x)
{	return acosh_template( x.ADBase() ); }


} // END CppAD namespace

# endif // CPPAD_USE_CPLUSPLUS_2011
# endif // CPPAD_ACOSH_INCLUDED
