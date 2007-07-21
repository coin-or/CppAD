# ifndef CPPAD_BASE_COMPLEX_INCLUDED
# define CPPAD_BASE_COMPLEX_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin base_complex.hpp$$
$spell
	sqrt
	exp
	cos
	std
	const
	CppAD
	Op
	inline
	enum
	undef
	acos
	asin
	atan
	erf
	Cond
	namespace
	bool
$$

$index complex, double Base$$
$index Base, double complex$$
$index double, complex Base$$

$section Enable use of AD<Base> where Base is std::complex<double>$$

$children%
	example/complex_poly.cpp
%$$

$head Example$$
The file $cref/ComplexPoly.cpp/$$ contains an example use of
$code std::complex<double>$$ type for a CppAD $italic Base$$ type.
It returns true if it succeeds and false otherwise.


$head CondExpOp$$
The conditional expressions $cref/CondExp/$$ 
requires ordered comparisons (e.g., $code <$$)
and the C++ standard complex types do not allow for ordered comparisons.
Thus, we make it an error to use the conditional comparisons 
with complex types:
$codep */
namespace CppAD {
	inline std::complex<double> CondExpOp(
		enum CppAD::CompareOp      cop        ,
		const std::complex<double> &left      ,
		const std::complex<double> &right     ,
		const std::complex<double> &trueCase  ,
		const std::complex<double> &falseCase )
	{	CppAD::ErrorHandler::Call(
			true     , __LINE__ , __FILE__ ,
			"std::complex<float> CondExpOp(...)",
			"Error: cannot use CondExp with a complex type"
		);
		return std::complex<double>(0);
	}
}
/* $$

$head EqualOpSeq$$
Complex numbers do not carry operation sequence information. 
Thus they are equal in this sense if and only if there values are equal.  
$codep */
namespace CppAD {
	inline bool EqualOpSeq(
		const std::complex<double> &x , 
		const std::complex<double> &y )
	{	return x == y; 
	}
}
/* $$

$head Identical$$
Complex numbers do not carry operation sequence information. 
Thus they are all parameters so the identical functions just check values.
$codep */
namespace CppAD {
	inline bool IdenticalPar(const std::complex<double> &x)
	{	return true; }
	inline bool IdenticalZero(const std::complex<double> &x)
	{	return (x == std::complex<double>(0., 0.) ); }
	inline bool IdenticalOne(const std::complex<double> &x)
	{	return (x == std::complex<double>(1., 0.) ); }
	inline bool IdenticalEqualPar(
		const std::complex<double> &x, const std::complex<double> &y)
	{	return (x == y); }
}
/* $$

$head Ordered$$

$codep */
namespace CppAD {
	inline bool GreaterThanZero(const std::complex<double> &x)
	{	CppAD::ErrorHandler::Call(
			true     , __LINE__ , __FILE__ ,
			"GreaterThanZero(x)",
			"Error: cannot use GreaterThanZero with complex"
		);
		return false;
	}
	inline bool GreaterThanOrZero(const std::complex<double> &x)
	{	CppAD::ErrorHandler::Call(
			true     , __LINE__ , __FILE__ ,
			"GreaterThanZero(x)",
			"Error: cannot use GreaterThanZero with complex"
		);
		return false;
	}
	inline bool LessThanZero(const std::complex<double> &x)
	{	CppAD::ErrorHandler::Call(
			true     , __LINE__ , __FILE__ ,
			"LessThanZero(x)",
			"Error: cannot use LessThanZero with complex"
		);
		return false;
	}
	inline bool LessThanOrZero(const std::complex<double> &x)
	{	CppAD::ErrorHandler::Call(
			true     , __LINE__ , __FILE__ ,
			"LessThanZero(x)",
			"Error: cannot use LessThanZero with complex"
		);
		return false;
	}
}
/* $$

$head Integer$$
The implementation of this function must agree
with the CppAD user specifications for complex arguments to the
$cref/Integer/Integer/x/Complex Types/$$ function:
$codep */
namespace CppAD {
	inline int Integer(const std::complex<double> &x)
	{	return static_cast<int>( x.real() ); }
}
/* $$
The complex absolute value function is nowhere complex differentiable.

$head Standard Functions$$
The following standard math functions,
that are required by $cref/base_require/$$,
are defined by 
$code std:;complex$$:
$code cos$$,
$code cosh$$,
$code exp$$,
$code log$$,
$code pow$$,
$code sin$$,
$code sinh$$,
$code sqrt$$.
The other standard math functions,
(and $code abs$$) required by $cref/base_require/$$
are not defined for complex types
(see $cref/abs/abs/Complex Types/$$ and $cref/StdMathUnary/StdMathUnary/fun/$$).
Hence we make it an error to use them.
$codep */

# define CPPAD_INVALID_COMPLEX_CASE(function)                                 \
inline std::complex<double> function(const std::complex<double> &x)           \
{	CppAD::ErrorHandler::Call(                                            \
		true     , __LINE__ , __FILE__ ,                              \
		"std::complex<double>",                                       \
			"Error: cannot use " #function " with a complex type" \
	);                                                                    \
	return std::complex<double>(0);                                       \
}

namespace CppAD {
	CPPAD_INVALID_COMPLEX_CASE(abs)
	CPPAD_INVALID_COMPLEX_CASE(acos)
	CPPAD_INVALID_COMPLEX_CASE(asin)
	CPPAD_INVALID_COMPLEX_CASE(atan)
	CPPAD_INVALID_COMPLEX_CASE(erf)
}

# undef CPPAD_INVALID_COMPLEX_CASE
/* $$
$end
*/
# define CPPAD_INVALID_COMPLEX_CASE(function)                                 \
inline std::complex<float> function(const std::complex<float> &x)             \
{	CppAD::ErrorHandler::Call(                                            \
		true     , __LINE__ , __FILE__ ,                              \
		"std::complex<float>",                                        \
			"Error: cannot use " #function " with a complex type" \
	);                                                                    \
	return std::complex<float>(0);                                        \
}
namespace CppAD {
	// CondExpOp ------------------------------------------------------
	inline std::complex<float> CondExpOp(
		enum CppAD::CompareOp      cop       ,
		const std::complex<float> &left      ,
		const std::complex<float> &right     ,
		const std::complex<float> &trueCase  ,
		const std::complex<float> &falseCase )
	{	CppAD::ErrorHandler::Call(
			true     , __LINE__ , __FILE__ ,
			"std::complex<float> CondExpOp(...)",
			"Error: cannot use CondExp with a complex type"
		);
		return std::complex<float>(0);
	}
	// EqualOpSeq -----------------------------------------------------
	inline bool EqualOpSeq(
		const std::complex<float> &x , 
		const std::complex<float> &y )
	{	return x == y; 
	}
	// Identical ------------------------------------------------------
	inline bool IdenticalPar(const std::complex<float> &x)
	{	return true; }
	inline bool IdenticalZero(const std::complex<float> &x)
	{	return (x == std::complex<float>(0., 0.) ); }
	inline bool IdenticalOne(const std::complex<float> &x)
	{	return (x == std::complex<float>(1., 0.) ); }
	inline bool IdenticalEqualPar(
		const std::complex<float> &x, const std::complex<float> &y)
	{	return (x == y); }
	// Ordered --------------------------------------------------------
	inline bool GreaterThanZero(const std::complex<float> &x)
	{	CppAD::ErrorHandler::Call(
			true     , __LINE__ , __FILE__ ,
			"GreaterThanZero(x)",
			"Error: cannot use GreaterThanZero with complex"
		);
		return false;
	}
	inline bool GreaterThanOrZero(const std::complex<float> &x)
	{	CppAD::ErrorHandler::Call(
			true     , __LINE__ , __FILE__ ,
			"GreaterThanOrZero(x)",
			"Error: cannot use GreaterThanOrZero with complex"
		);
		return false;
	}
	inline bool LessThanZero(const std::complex<float> &x)
	{	CppAD::ErrorHandler::Call(
			true     , __LINE__ , __FILE__ ,
			"LessThanZero(x)",
			"Error: cannot use LessThanZero with complex"
		);
		return false;
	}
	inline bool LessThanOrZero(const std::complex<float> &x)
	{	CppAD::ErrorHandler::Call(
			true     , __LINE__ , __FILE__ ,
			"LessThanOrZero(x)",
			"Error: cannot use LessThanOrZero with complex"
		);
		return false;
	}
	// Integer ------------------------------------------------------
	inline int Integer(const std::complex<float> &x)
	{	return static_cast<int>( x.real() ); }
	// Invalid standrd math functions -------------------------------
	CPPAD_INVALID_COMPLEX_CASE(abs)
	CPPAD_INVALID_COMPLEX_CASE(acos)
	CPPAD_INVALID_COMPLEX_CASE(asin)
	CPPAD_INVALID_COMPLEX_CASE(atan)
	CPPAD_INVALID_COMPLEX_CASE(erf)
}

# undef CPPAD_INVALID_COMPLEX_CASE
/* $$
$end
*/

# endif
