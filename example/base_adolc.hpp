# ifndef CPPAD_BASE_ADOLC_INCLUDED
# define CPPAD_BASE_ADOLC_INCLUDED
/*
$begin base_adolc.hpp$$
$spell
	erf
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

$head Functions Defined by Adolc Package$$
The following required functions are defined by the Adolc package:
$code abs$$,
$code acos$$,
$code asin$$,
$code atan$$,
$code cos$$,
$code cosh$$,
$code erf$$,
$code exp$$,
$code log$$,
$code log10$$, 
$code pow$$,
$code sin$$,
$code sinh$$,
$code sqrt$$,
$code tan$$.

$head CondExpOp$$
This is a simple implementation of CondExpOp. It requires re-taping and 
defeats the purpose of conditional expressions using 
$code AD<adouble>$$. 
A better implementation would use Adolc's $code condassign$$ operation:
$codep */
	inline adouble CondExpOp(
		enum CppAD::CompareOp      cop ,
		const adouble            &left ,
		const adouble           &right ,
		const adouble        &trueCase ,
		const adouble       &falseCase )
	{	return CppAD::CondExpTemplate(
			cop, left, right, trueCase, falseCase
		);
	}
/* $$

$head EqualOpSeq$$
The Adolc user interface does not specify a way to determine if 
two $code adouble$$ variables correspond to the same operations sequence. 
Make $code EqualOpSeq$$ an error if it gets used:
$codep */
	# include <cassert>
	inline bool EqualOpSeq(const adouble &x, const adouble &y)
	{	assert(0);
		return false;
	}
/* $$

$head Identical$$
The Adolc user interface does not specify a way to determine if an 
$code adouble$$ depends on the independent variables. 
To be safe (but slow) return $code false$$ in all the cases below.
$codep */
	inline bool IdenticalPar(const adouble &x)
	{	return false; }

	inline bool IdenticalZero(const adouble &x)
	{	return false; }

	inline bool IdenticalOne(const adouble &x)
	{	return false; }

	inline bool IdenticalEqual(const adouble &x, const adouble &y)
	{	return false; }
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
	{    return static_cast<int>( x.value() ); }

/* $$
$end
*/

# endif
