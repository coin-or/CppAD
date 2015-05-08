// $Id:$
# ifndef CPPAD_ATANH_INCLUDED
# define CPPAD_ATANH_INCLUDED

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
$begin atanh$$

$section The Inverse Hyperbolic Tangent Function$$
$spell
	tanh
	atanh
	const
	Vec
	std
	cmath
	CppAD
$$

$head Syntax$$
$icode%y% = atanh(%x%)%$$


$head Description$$
Returns the value of the inverse hyperbolic tangent function which is defined
by $icode%x% == tanh(%y%)%$$.


$head x$$
The argument $icode x$$, and the result $icode y$$
have one of the following paris of prototypes:
$codei%
	const float%%                  &%x%,     float%%    %y%
	const double%%                 &%x%,     double%%   %y%
	const AD<%Base%>               &%x%,     AD<%Base%> %y%
	const VecAD<%Base%>::reference &%x%,     AD<%Base%> %y%
%$$


$head CPPAD_COMPILER_HAS_ATANH$$
This preprocessor symbol is one if
the function $codei%std::atanh(double %x%)%$$ is defined the in the
include file $code <cmath>$$.
Otherwise this preprocessor symbol is zero.
If this preprocessor symbols is one,
CppAD uses the compiler's version of $code atanh$$
and it corresponds to an $cref/atomic/glossary/Operation/Atomic/$$ operation.

$head Other$$
If the function $codei%std::atanh(double %x%)%$$ is not defined,
CppAD uses the representation
$latex \[
\R{atanh} (x) = \frac{1}{2} \log \left( \frac{1 + x}{1 - x} \right)
\] $$
to compute this function.

$head Example$$
$children%
	example/atanh.cpp
%$$
The file
$cref atanh.cpp$$
contains an example and test of this function.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/
# include <cppad/configure.hpp>
# if ! CPPAD_COMPILER_HAS_ATANH

// BEGIN CppAD namespace
namespace CppAD {

template <class Type>
Type atanh_template(const Type &x)
{	return CppAD::log( (1 + x) / (1 - x) ) / 2.0;
}

inline float atanh(const float &x)
{	return atanh_template(x); }

inline double atanh(const double &x)
{	return atanh_template(x); }

template <class Base>
inline AD<Base> atanh(const AD<Base> &x)
{	return atanh_template(x); }

template <class Base>
inline AD<Base> atanh(const VecAD_reference<Base> &x)
{	return atanh_template( x.ADBase() ); }


} // END CppAD namespace

# endif // CPPAD_COMPILER_HAS_ATANH
# endif // CPPAD_ATANH_INCLUDED
