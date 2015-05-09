// $Id:$
# ifndef CPPAD_EXPM1_INCLUDED
# define CPPAD_EXPM1_INCLUDED

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
$begin expm1$$

$section The Inverse Hyperbolic Sine Function$$
$spell
	exp
	expm1
	expm1
	const
	Vec
	std
	cmath
	CppAD
$$

$head Syntax$$
$icode%y% = expm1(%x%)%$$


$head Description$$
Returns the value of the inverse hyperbolic sine function which is defined
by $icode%y% == exp(%x%) - 1%$$.


$head x$$
The argument $icode x$$, and the result $icode y$$
have one of the following paris of prototypes:
$codei%
	const float%%                  &%x%,     float%%    %y%
	const double%%                 &%x%,     double%%   %y%
	const AD<%Base%>               &%x%,     AD<%Base%> %y%
	const VecAD<%Base%>::reference &%x%,     AD<%Base%> %y%
%$$


$head CPPAD_COMPILER_HAS_EXPM1$$
This preprocessor symbol is one if
the function $codei%std::expm1(double %x%)%$$ is defined the in the
include file $code <cmath>$$.
Otherwise this preprocessor symbol is zero.
If this preprocessor symbols is one,
CppAD uses the compiler's version of $code expm1$$
and it corresponds to an $cref/atomic/glossary/Operation/Atomic/$$ operation.

$head Other$$
If the function $codei%std::expm1(double %x%)%$$ is not defined,
CppAD uses the representation
$latex \[
\R{expm1} (x) = exp(x) - 1
\] $$
to compute this function.

$head Example$$
$children%
	example/expm1.cpp
%$$
The file
$cref expm1.cpp$$
contains an example and test of this function.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/
# include <cppad/configure.hpp>
# if ! CPPAD_COMPILER_HAS_EXPM1

// BEGIN CppAD namespace
namespace CppAD {

template <class Type>
Type expm1_template(const Type &x)
{	return CppAD::exp(x) - Type(1);
}

inline float expm1(const float &x)
{	return expm1_template(x); }

inline double expm1(const double &x)
{	return expm1_template(x); }

template <class Base>
inline AD<Base> expm1(const AD<Base> &x)
{	return expm1_template(x); }

template <class Base>
inline AD<Base> expm1(const VecAD_reference<Base> &x)
{	return expm1_template( x.ADBase() ); }


} // END CppAD namespace

# endif // CPPAD_COMPILER_HAS_EXPM1
# endif // CPPAD_EXPM1_INCLUDED
