/* $Id$ */
# ifndef CPPAD_ABS_INCLUDED
# define CPPAD_ABS_INCLUDED

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
$begin abs$$
$spell
	fabs
	Vec
	std
	faq
	Taylor
	Cpp
	namespace
	const
	abs
$$

$section AD Absolute Value Functions: abs, fabs$$

$head Syntax$$
$icode%y% = abs(%x%)
%$$
$icode%y% = fabs(%x%)%$$


$head Purpose$$
Evaluates the absolute value function.

$head x$$
The argument $icode x$$ has one of the following prototypes
$codei%
	const AD<%Base%>               &%x%
	const VecAD<%Base%>::reference &%x%
%$$

$head y$$
The result $icode y$$ has prototype
$codei%
	AD<%Base%> %y%
%$$


$head Operation Sequence$$
This is an AD of $icode Base$$
$cref/atomic operation/glossary/Operation/Atomic/$$
and hence is part of the current
AD of $icode Base$$
$cref/operation sequence/glossary/Operation/Sequence/$$.

$head Complex Types$$
The function $code abs$$ is not defined for the AD type sequences
above $code std::complex<float>$$ or $code std::complex<double>$$
because the complex $code abs$$ function is not complex differentiable
(see $cref/complex types faq/Faq/Complex Types/$$).

$head Derivative$$
CppAD defines the derivative of the $code abs$$ function is
the $cref sign$$ function; i.e.,
$latex \[
{\rm abs}^{(1)} ( x ) = {\rm sign} (x ) =
\left\{ \begin{array}{rl}
	+1 & {\rm if} \; x > 0 \\
	0  & {\rm if} \; x = 0 \\
	-1 & {\rm if} \; x < 0
\end{array} \right.
\] $$
The result for $icode%x% == 0%$$ used to be a directional derivative.

$head Example$$
$children%
	example/abs.cpp
%$$
The file
$cref abs.cpp$$
contains an example and test of this function.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
AD<Base> AD<Base>::Abs (void) const
{
	AD<Base> result;
	result.value_ = abs(value_);
	CPPAD_ASSERT_UNKNOWN( Parameter(result) );

	if( Variable(*this) )
	{	// add this operation to the tape
		CPPAD_ASSERT_UNKNOWN( NumRes(AbsOp) == 1 );
		CPPAD_ASSERT_UNKNOWN( NumArg(AbsOp) == 1 );
		ADTape<Base> *tape = tape_this();

		// corresponding operand address
		tape->Rec_.PutArg(taddr_);
		// put operator in the tape
		result.taddr_ = tape->Rec_.PutOp(AbsOp);
		// make result a variable
		result.tape_id_    = tape->id_;
	}
	return result;
}

template <class Base>
inline AD<Base> abs(const AD<Base> &x)
{	return x.Abs(); }

template <class Base>
inline AD<Base> abs(const VecAD_reference<Base> &x)
{	return abs( x.ADBase() ); }

} // END CppAD namespace

# endif
