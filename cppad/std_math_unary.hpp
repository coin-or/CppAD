# ifndef CPPAD_STD_MATH_UNARY_INCLUDED
# define CPPAD_STD_MATH_UNARY_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
-------------------------------------------------------------------------------
$begin std_math_unary$$
$spell
	cppad.hpp
	Vec
	std
	atan
	const
	acos
	asin
	atan
	cos
	exp
	sqrt
	CppAD
	namespace
	tanh
$$

$index standard, math unary$$
$index math, unary$$
$index unary, math$$

$index abs, float and double$$
$index acos, float and double$$
$index asin, float and double$$
$index atan, float and double$$
$index cos, float and double$$
$index cosh, float and double$$
$index exp, float and double$$
$index log, float and double$$
$index log10, float and double$$
$index sin, float and double$$
$index sinh, float and double$$
$index sqrt, float and double$$
$index tan, float and double$$
$index tanh, float and double$$

$section Float and Double Standard Math Unary Functions$$

$head Syntax$$
$syntax%# include <cppad/std_math_unary.hpp>%$$
$syntax%%y% = %fun%(%x%)%$$


$head Purpose$$
Places a copy of the standard math unary functions in the CppAD namespace.
This is included with $code <cppad/cppad.hpp>$$ and can
be included separately.

$head Type$$
The $italic Type$$ is determined by the argument $italic x$$
and is either $code float$$ or $code double$$.

$head x$$
The argument $italic x$$ has the following prototype
$syntax%
	const %Type%  &%x%
%$$

$head y$$
The result $italic y$$ has prototype
$syntax%
	%Type% %y%
%$$

$head fun$$ 
A definition of $italic fun$$ is included for each of the 
following functions:
$code abs$$, 
$code acos$$, 
$code asin$$, 
$code atan$$, 
$code cos$$,  
$code cosh$$  
$code exp$$, 
$code log$$, 
$code log10$$, 
$code sin$$, 
$code sinh$$, 
$code sqrt$$, 
$code tan$$, 
$code tanh$$, 

$end
-------------------------------------------------------------------------------
*/

# include <cmath>

# define CPPAD_STANDARD_MATH_UNARY(Name)                             \
                                                                     \
	inline float Name(const float &x)                            \
	{	return std::Name(x); }                               \
                                                                     \
	inline double Name(const double &x)                          \
	{	return std::Name(x); }

namespace CppAD {

        CPPAD_STANDARD_MATH_UNARY(abs)
        CPPAD_STANDARD_MATH_UNARY(acos)
        CPPAD_STANDARD_MATH_UNARY(asin)
        CPPAD_STANDARD_MATH_UNARY(atan)
        CPPAD_STANDARD_MATH_UNARY(cos)
        CPPAD_STANDARD_MATH_UNARY(cosh)
        CPPAD_STANDARD_MATH_UNARY(exp)
        CPPAD_STANDARD_MATH_UNARY(log)
        CPPAD_STANDARD_MATH_UNARY(log10)
        CPPAD_STANDARD_MATH_UNARY(sin)
        CPPAD_STANDARD_MATH_UNARY(sinh)
        CPPAD_STANDARD_MATH_UNARY(sqrt)
        CPPAD_STANDARD_MATH_UNARY(tan)
        CPPAD_STANDARD_MATH_UNARY(tanh)
}

# undef CPPAD_STANDARD_MATH_UNARY

# endif 
