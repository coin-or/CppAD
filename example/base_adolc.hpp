# ifndef CPPAD_BASE_ADOLC_INCLUDED
# define CPPAD_BASE_ADOLC_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin base_adolc.hpp$$
$spell
	cppad.hpp
	undef
	Lt
	Le
	Eq
	Ge
	Gt
	namespace
	cassert
	condassign
	hpp
	bool
	const
	Adolc
	adouble
	CondExpOp
	inline
	enum
	CppAD
	pow
	acos
	asin
	atan
	cos
	cosh
	exp
	sqrt
$$

$index Adolc, adouble as Base$$
$index Base, Adolc's adouble$$
$index adouble, as Base$$ 

$section Enable use of AD<Base> where Base is Adolc's adouble Type$$

$head Syntax$$
This file in located in the $code example$$ directory. 
It can be copied into the current working directory and
included with the command:
$syntax%
	# include "base_adolc.hpp"
%$$

$children%
	example/mul_level_adolc.cpp
%$$

$head Example$$
The file $cref/mul_level_adolc.cpp/$$ contains an example use of
Adolc's $code adouble$$ type for a CppAD $italic Base$$ type.
It returns true if it succeeds and false otherwise.
The file $cref/ode_taylor_adolc.cpp/$$ contains a more realistic
(and complex) example.


$head Include File$$
This file is included before $code <cppad/cppad.hpp>$$
so it is necessary to define the error handler
in addition to including
$cref/declare.hpp/base_require/declare.hpp/$$
$codep */
# include <cppad/declare.hpp>
# include <cppad/error_handler.hpp>

/* $$

$head Standard Math Functions Defined by Adolc Package$$
The following $cref/required/base_require/$$ 
functions are defined by the Adolc package:
$pre
$$
$code acos$$,
$code asin$$,
$code atan$$,
$code cos$$,
$code cosh$$,
$code exp$$,
$code log$$,
$code pow$$,
$code sin$$,
$code sinh$$,
$code sqrt$$,
$code tan$$.

$head CondExpOp$$
The type $code adouble$$ supports a conditional assignment function
with the syntax
$syntax%
	condassign(%a%, %b%, %c%, %d%)
%$$
which evaluates to
$syntax%
	%a% = (%b% > 0) ? %c% : %d%;
%$$
This enables one to include conditionals in the recording of
$code adouble$$ operations and later evaluation for different
values of the independent variables 
(in the same spirit as the CppAD $cref/CondExp/$$ function).
$codep */
namespace CppAD {
	inline adouble CondExpOp(
		enum  CppAD::CompareOp     cop ,
		const adouble            &left ,
		const adouble           &right ,
		const adouble        &trueCase ,
		const adouble       &falseCase )
	{	adouble result;
		switch( cop )
		{
			case CompareLt: // left < right
			condassign(result, right - left, trueCase, falseCase);
			break;

			case CompareLe: // left <= right
			condassign(result, left - right, falseCase, trueCase);
			break;

			case CompareEq: // left == right
			condassign(result, left - right, falseCase, trueCase);
			condassign(result, right - left, falseCase, result);
			break;

			case CompareGe: // left >= right
			condassign(result, right - left, falseCase, trueCase);
			break;

			case CompareGt: // left > right
			condassign(result, left - right, trueCase, falseCase);
			break;

			default:
			CppAD::ErrorHandler::Call(
				true     , __LINE__ , __FILE__ ,
				"CppAD::CondExp",
				"Error: for unknown reason."
			);
			result = trueCase;
		}
		return result;
	}
}
/* $$

$head EqualOpSeq$$
The Adolc user interface does not specify a way to determine if 
two $code adouble$$ variables correspond to the same operations sequence. 
Make $code EqualOpSeq$$ an error if it gets used:
$codep */
namespace CppAD {
	inline bool EqualOpSeq(const adouble &x, const adouble &y)
	{	CppAD::ErrorHandler::Call(
			true     , __LINE__ , __FILE__ ,
			"CppAD::EqualOpSeq(x, y)",
			"Error: adouble does not support EqualOpSeq."
		);
		return false;
	}
}
/* $$

$head Identical$$
The Adolc user interface does not specify a way to determine if an 
$code adouble$$ depends on the independent variables. 
To be safe (but slow) return $code false$$ in all the cases below.
$codep */
namespace CppAD {
	inline bool IdenticalPar(const adouble &x)
	{	return false; }
	inline bool IdenticalZero(const adouble &x)
	{	return false; }
	inline bool IdenticalOne(const adouble &x)
	{	return false; }
	inline bool IdenticalEqualPar(const adouble &x, const adouble &y)
	{	return false; }
}
/* $$

$head Ordered$$
$codep */
	inline bool GreaterThanZero(const adouble &x)
	{    return (x > 0); }
	inline bool GreaterThanOrZero(const adouble &x)
	{    return (x >= 0); }
	inline bool LessThanZero(const adouble &x)
	{    return (x < 0); }
	inline bool LessThanOrZero(const adouble &x)
	{    return (x <= 0); }
/* $$

$head Integer$$
$codep */
	inline int Integer(const adouble &x)
	{    return static_cast<int>( x.getValue() ); }
/* $$
$end
*/
# endif
