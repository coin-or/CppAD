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
\] $$.
using logarithms and exponentiation to compute the value.
If $italic y$$ has an integer value_,
The other CppAD $cref/pow/PowInt/$$ function can compute the same value 
using only multiplication and possibly division. 

$head x$$
The argument $italic x$$ has one of the following prototypes
$syntax%
	int   %%                        %x%
	const %Base%                   &%x%
	const AD<%Base%>               &%x%
	const VecAD<%Base%>::reference &%x%
%$$ 

$head y$$
The argument $italic y$$ has one of the following prototypes
$syntax%
	int   %%                        %y%
	const %Base%                   &%y%
	const AD<%Base%>               &%y%
	const VecAD<%Base%>::reference &%y%
%$$ 

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

// case where x and y are AD<Base> -----------------------------------------
template <class Base> AD<Base> 
pow(const AD<Base> &x, const AD<Base> &y)
{	AD<Base> p;
	CppADUnknownError( Parameter(p) );

	// base type result
	p.value_  = pow(x.value_, y.value_);

	if( Variable(x) )
	{	if( Variable(y) )
		{	// result = variable + variable
			CppADUsageError(
				x.id_ == y.id_,
				"pow: arguments are AD objects that are"
				" variables on different tapes."
			);
			x.tape_this()-> 
				RecordOp(PowvvOp, p, x.taddr_, y.taddr_
			);
		}
		// if IdenticalZero(y.value_), p = variable^0 = 1
		else if( ! IdenticalZero(y.value_) )
			x.tape_this()->
				RecordOp(PowvpOp, p, x.taddr_, y.value_);
	}
	else if( Variable(y) )
	{	// if IdenticalZero(x.value_), p = 0^variable = 0 
		if( ! IdenticalZero(x.value_) )
			y.tape_this()->
				RecordOp(PowpvOp, p, x.value_, y.taddr_);
	}

	return p;
}
// -------------------------------------------------------------------------

// Fold operations with VecAD_reference<Base> and AD<Base>
template <class Base> AD<Base>
pow(const AD<Base> &x, const VecAD_reference<Base> &y)
{	return pow(x, y.ADBase()); }

template <class Base> AD<Base>
pow(const VecAD_reference<Base> &x, const AD<Base> &y)
{	return pow(x.ADBase(), y); }

template <class Base> AD<Base> 
pow(const VecAD_reference<Base> &x, const VecAD_reference<Base> &y) 
{	return pow(x.ADBase(), y.ADBase()); }

// Fold operations with Base
template <class Base> AD<Base>
pow(const Base &x, const VecAD_reference<Base> &y)
{	return pow(AD<Base>(x), y.ADBase()); }

template <class Base> AD<Base>
pow(const VecAD_reference<Base> &x, const Base &y)
{	return pow(x.ADBase(), AD<Base>(y)); }

template <class Base> AD<Base>
pow(const Base &x, const AD<Base> &y)
{	return pow(AD<Base>(x), y); }

template <class Base> AD<Base>
pow(const AD<Base> &x, const Base &y)
{	return pow(x, AD<Base>(y)); }

// Flod operations with int
template <class Base> AD<Base> pow
(int x, const VecAD_reference<Base> &y)
{	return pow(AD<Base>(x), y.ADBase()); }

template <class Base> AD<Base> pow
(const VecAD_reference<Base> &x, int y)
{	return pow(x.ADBase(), AD<Base>(y)); }

template <class Base> AD<Base> pow
(int x, const AD<Base> &y)
{	return pow(AD<Base>(x), y); }

template <class Base> AD<Base> pow
(const AD<Base> &x, int y)
{	return pow(x, AD<Base>(y)); }

} // END CppAD namespace

# endif 
