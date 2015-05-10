// $Id:$
# ifndef CPPAD_ASINH_INCLUDED
# define CPPAD_ASINH_INCLUDED

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
$begin asinh$$

$section The Inverse Hyperbolic Sine Function$$
$spell
	asinh
	const
	Vec
	std
	cmath
	CppAD
$$

$head Syntax$$
$icode%y% = asinh(%x%)%$$


$head Description$$
Returns the value of the inverse hyperbolic sine function which is defined
by $icode%x% == sinh(%y%)%$$.


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
the function $codei%std::asinh(double %x%)%$$ is defined the in the
include file $code <cmath>$$.
Otherwise this preprocessor symbol is zero.
If this preprocessor symbols is one,
CppAD uses the compiler's version of $code asinh$$
and it corresponds to an $cref/atomic/glossary/Operation/Atomic/$$ operation.

$head Other$$
If the function $codei%std::asinh(double %x%)%$$ is not defined,
CppAD uses the representation
$latex \[
\R{asinh} (x) = \log \left( x + \sqrt{ 1 + x^2 } \right)
\] $$
to compute this function.

$head Example$$
$children%
	example/asinh.cpp
%$$
The file
$cref asinh.cpp$$
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
Type asinh_template(const Type &x)
{	return CppAD::log( x + CppAD::sqrt( Type(1) + x * x ) );
}

inline float asinh(const float &x)
{	return asinh_template(x); }

inline double asinh(const double &x)
{	return asinh_template(x); }

template <class Base>
inline AD<Base> asinh(const AD<Base> &x)
{	return asinh_template(x); }

template <class Base>
inline AD<Base> asinh(const VecAD_reference<Base> &x)
{	return asinh_template( x.ADBase() ); }


} // END CppAD namespace

# endif // CPPAD_USE_CPLUSPLUS_2011
# endif // CPPAD_ASINH_INCLUDED
