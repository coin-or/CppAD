// $Id:$
# ifndef CPPAD_LOG1P_INCLUDED
# define CPPAD_LOG1P_INCLUDED

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
$begin log1p$$

$section The Logarithm of One Plus Argument$$
$spell
	log1p
	const
	Vec
	std
	cmath
	CppAD
$$

$head Syntax$$
$icode%y% = log1p(%x%)%$$


$head Description$$
Returns the value of the logarithm of one plus argument which is defined
by $icode%y% == log(1 + %x%)%$$.


$head x$$
The argument $icode x$$, and the result $icode y$$
have one of the following paris of prototypes:
$codei%
	const float%%                  &%x%,     float%%    %y%
	const double%%                 &%x%,     double%%   %y%
	const AD<%Base%>               &%x%,     AD<%Base%> %y%
	const VecAD<%Base%>::reference &%x%,     AD<%Base%> %y%
%$$


$head CPPAD_COMPILER_HAS_LOG1P$$
This preprocessor symbol is one if
the function $codei%std::log1p(double %x%)%$$ is defined the in the
include file $code <cmath>$$.
Otherwise this preprocessor symbol is zero.
If this preprocessor symbols is one,
CppAD uses the compiler's version of $code log1p$$
and it corresponds to an $cref/atomic/glossary/Operation/Atomic/$$ operation.

$head Other$$
If the function $codei%std::log1p(double %x%)%$$ is not defined,
CppAD uses the representation
$latex \[
\R{log1p} (x) = log(1 + x)
\] $$
to compute this function.

$head Example$$
$children%
	example/log1p.cpp
%$$
The file
$cref log1p.cpp$$
contains an example and test of this function.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/
# include <cppad/configure.hpp>
# if ! CPPAD_COMPILER_HAS_LOG1P

// BEGIN CppAD namespace
namespace CppAD {

template <class Type>
Type log1p_template(const Type &x)
{	return CppAD::log(Type(1) + x);
}

inline float log1p(const float &x)
{	return log1p_template(x); }

inline double log1p(const double &x)
{	return log1p_template(x); }

template <class Base>
inline AD<Base> log1p(const AD<Base> &x)
{	return log1p_template(x); }

template <class Base>
inline AD<Base> log1p(const VecAD_reference<Base> &x)
{	return log1p_template( x.ADBase() ); }


} // END CppAD namespace

# endif // CPPAD_COMPILER_HAS_LOG1P
# endif // CPPAD_LOG1P_INCLUDED
