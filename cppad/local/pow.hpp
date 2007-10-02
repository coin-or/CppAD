# ifndef CPPAD_POW_INCLUDED
# define CPPAD_POW_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
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
\] $$
This version of the $code pow$$ function may use
logarithms and exponentiation to compute derivatives.
This will not work if $italic x$$ is less than or equal zero.
If the value of $italic y$$ is an integer, 
the $cref/pow_int/$$ function is used to compute this value 
using only multiplication (and division if $italic y$$ is negative). 
(This will work even if $italic x$$ is less than or equal zero.)

$head x$$
The argument $italic x$$ has the following prototype
$syntax%
	const %Type% &%x%
%$$
where $italic Type$$ is
$syntax%VecAD<%Base%>::reference%$$,
$syntax%AD<%Base%>%$$,
$syntax%%Base%$$,
$syntax%double%$$,
or
$syntax%int%$$.

$head y$$
The argument $italic y$$ has the following prototype
$syntax%
	const %Type% &%y%
%$$
where $italic Type$$ is
$syntax%VecAD<%Base%>::reference%$$,
$syntax%AD<%Base%>%$$,
$syntax%%Base%$$,
$syntax%double%$$,
or
$syntax%int%$$.

$head z$$
The result $italic z$$ has prototype
$syntax%
	AD<%Base%> %z%
%$$

$head Standard Types$$
A definition for the $code pow$$ function is included
in the CppAD namespace for the case where both $italic x$$
and $italic y$$ have the same type and that type is
$code float$$ or $code double$$.
 
$head Operation Sequence$$
This is an AD of $italic Base$$
$xref/glossary/Operation/Atomic/atomic operation/1/$$
and hence is part of the current
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$.

$head Example$$
$children%
	example/pow.cpp%
	example/pow_int.cpp
%$$
The files
$xref/Pow.cpp/$$, $cref/pow_int.cpp/$$
contain an examples and tests of this function.   
They returns true if they succeed and false otherwise.

$end
-------------------------------------------------------------------------------
*/
//  BEGIN CppAD namespace
namespace CppAD {
 
// copy of standard functions in CppAD namespace
inline float pow(const float &x, const float &y)
{ return std::pow(x, y); }

inline double pow(const double &x, const double &y)
{ return std::pow(x, y); }

// case where x and y are AD<Base> -----------------------------------------
template <class Base> AD<Base> 
pow(const AD<Base> &x, const AD<Base> &y)
{	AD<Base> p;
	CPPAD_ASSERT_UNKNOWN( Parameter(p) );

	// base type result
	p.value_  = pow(x.value_, y.value_);

	if( Variable(x) )
	{	if( Variable(y) )
		{	// result = variable + variable
			CPPAD_ASSERT_KNOWN(
				x.id_ == y.id_,
				"pow: arguments are AD objects that are"
				" variables on different tapes."
			);
			x.tape_this()-> 
				RecordOp(PowvvOp, p, x.taddr_, y.taddr_
			);
		}
		// if IdenticalZero(y.value_), variable^0 is a parameter
		else if( ! IdenticalZero(y.value_) )
			x.tape_this()->
				RecordOp(PowvpOp, p, x.taddr_, y.value_);
	}
	else if( Variable(y) )
	{	// if IdenticalZero(x.value_), 0^variable is a parameter
		if( ! IdenticalZero(x.value_) )
			y.tape_this()->
				RecordOp(PowpvOp, p, x.value_, y.taddr_);
	}

	return p;
}
// =========================================================================
// Fold operations in same way as CPPAD_FOLD_AD_VALUED_BINARY_OPERATION(Op)
// -------------------------------------------------------------------------
// Operations with VecAD_reference<Base> and AD<Base> only

template <class Base> AD<Base>
pow(const AD<Base> &x, const VecAD_reference<Base> &y)
{	return pow(x, y.ADBase()); }

template <class Base> AD<Base> 
pow(const VecAD_reference<Base> &x, const VecAD_reference<Base> &y) 
{	return pow(x.ADBase(), y.ADBase()); }

template <class Base> AD<Base>
pow(const VecAD_reference<Base> &x, const AD<Base> &y)
{	return pow(x.ADBase(), y); }
// -------------------------------------------------------------------------
// Operations with Base

template <class Base> AD<Base>
pow(const Base &x, const AD<Base> &y)
{	return pow(AD<Base>(x), y); }

template <class Base> AD<Base>
pow(const Base &x, const VecAD_reference<Base> &y)
{	return pow(AD<Base>(x), y.ADBase()); }

template <class Base> AD<Base>
pow(const AD<Base> &x, const Base &y)
{	return pow(x, AD<Base>(y)); }

template <class Base> AD<Base>
pow(const VecAD_reference<Base> &x, const Base &y)
{	return pow(x.ADBase(), AD<Base>(y)); }
// -------------------------------------------------------------------------
// Operations with double

template <class Base> AD<Base>
pow(const double &x, const AD<Base> &y)
{	return pow(AD<Base>(x), y); }

template <class Base> AD<Base>
pow(const double &x, const VecAD_reference<Base> &y)
{	return pow(AD<Base>(x), y.ADBase()); }

template <class Base> AD<Base>
pow(const AD<Base> &x, const double &y)
{	return pow(x, AD<Base>(y)); }

template <class Base> AD<Base>
pow(const VecAD_reference<Base> &x, const double &y)
{	return pow(x.ADBase(), AD<Base>(y)); }
// -------------------------------------------------------------------------
// Special case to avoid ambuigity when Base is double

inline AD<double>
pow(const double &x, const AD<double> &y)
{	return pow(AD<double>(x), y); }

inline AD<double>
pow(const double &x, const VecAD_reference<double> &y)
{	return pow(AD<double>(x), y.ADBase()); }

inline AD<double>
pow(const AD<double> &x, const double &y)
{	return pow(x, AD<double>(y)); }

inline AD<double>
pow(const VecAD_reference<double> &x, const double &y)
{	return pow(x.ADBase(), AD<double>(y)); }

// =========================================================================
// Fold operations for the cases where x is an int, 
// but let cppad/pow_int.hpp handle the cases where y is an int.
// -------------------------------------------------------------------------
template <class Base> AD<Base> pow
(int x, const VecAD_reference<Base> &y)
{	return pow(AD<Base>(x), y.ADBase()); }

template <class Base> AD<Base> pow
(int x, const AD<Base> &y)
{	return pow(AD<Base>(x), y); }

} // END CppAD namespace

# endif 
