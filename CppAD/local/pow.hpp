# ifndef CPPAD_POW_INCLUDED
# define CPPAD_POW_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
-------------------------------------------------------------------------------
$begin pow$$
$spell
	Vec
	std
	namespace
	CppAD
	const
$$

$index pow, AD$$
$index exponent, AD function$$

$section The AD Power Function$$

$head Syntax$$
$syntax%%z% = pow(%x%, %y%)%$$


$head Purpose$$
Determines the value of the power function which is defined by
$latex \[
	{\rm pow} (x, y) = x^y
\] $$.

$head x$$
The argument $italic x$$ has one of the following prototypes
$syntax%
	const AD<%Base%>               &%x%
	const VecAD<%Base%>::reference &%x%
%$$ 

$head y$$
The argument $italic y$$ has one of the following prototypes
$syntax%
	const AD<%Base%>               &%x%
	const VecAD<%Base%>::reference &%x%
%$$ 
(see $xref/PowInt/$$ for the case where $italic y$$ is an $code int$$).

$head z$$
The result $italic z$$ has prototype
$syntax%
	AD<%Base%> %z%
%$$

$head Standard Types$$
A definition for the $code pow$$ function is included
in the CppAD namespace for the case where both $italic x$$
and $italic y$$ have the same type and that type is
$code float$$, $code double$$, 
$code std::complex<float>$$, or
$code std::complex<double>$$.
For these cases, 
the result has the same type as the arguments.

$head Operation Sequence$$
The AD of $italic Base$$
operation sequence used to calculate $italic z$$ is
$xref/glossary/Operation/Independent/independent/1/$$
of $italic x$$ and $italic y$$.

$head Example$$
$children%
	Example/Pow.cpp
%$$
The file
$xref/Pow.cpp/$$
contains an example and test of this function.   
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN CppAD namespace

inline float pow(float x, float y)
{	return std::pow(x, y); }

inline double pow(double x, double y)
{	return std::pow(x, y); }

inline std::complex<float> 
pow(std::complex<float> x, std::complex<float> y)
{	return std::pow(x, y); }

inline std::complex<double> 
pow(std::complex<double> x, std::complex<double> y)
{	return std::pow(x, y); }

template <class Base>
inline AD<Base> pow (const AD<Base> &x, const AD<Base> &y)
{	using CppAD::exp;
	using CppAD::log;

	return exp( y * log(x) );
}

template <class Base>
inline AD<Base> pow (const VecAD_reference<Base> &y, const AD<Base> &x)
{	return pow( y.ADBase() , x ); }

template <class Base>
inline AD<Base> pow (const AD<Base> &y, const VecAD_reference<Base> &x)
{	return pow( y , x.ADBase() ); }

template <class Base>
inline AD<Base> pow 
(const VecAD_reference<Base> &y, const VecAD_reference<Base> &x)
{	return pow( y.ADBase() , x.ADBase() ); }

} // END CppAD namespace

# endif 
