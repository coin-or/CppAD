# ifndef CPPAD_ABS_INCLUDED
# define CPPAD_ABS_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
-------------------------------------------------------------------------------
$begin abs$$
$spell
	Vec
	std
	faq
	Taylor
	Cpp
	namespace
	const
	abs
$$

$index abs, AD$$
$index absolute, AD value$$
$index value_, AD absolute$$

$section AD Absolute Value Function$$

$head Syntax$$
$syntax%%y% = abs(%x%)%$$


$head Purpose$$
Evaluates the absolute value function.


$head x$$
The argument $italic x$$ has one of the following prototypes
$syntax%
	const AD<%Base%>               &%x%
	const VecAD<%Base%>::reference &%x%
%$$

$head y$$
The result $italic y$$ has prototype
$syntax%
	AD<%Base%> %y%
%$$


$head Operation Sequence$$
This is an AD of $italic Base$$
$xref/glossary/Operation/Atomic/atomic operation/1/$$
and hence is part of the current
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$.

$head Complex Types$$
The function $code abs$$ is not defined for the AD type sequences
above $code std::complex<float>$$ or $code std::complex<double>$$
because the complex $code abs$$ function is not complex differentiable
(see $xref/Faq/Complex Types/complex types faq/$$).

$head Directional Derivative$$
$index directional, derivative abs$$
$index derivative, directional abs$$
The derivative of the absolute value function is one for 
$latex x > 0$$ and minus one for $latex x < 0$$.
The subtitle issue is 
how to compute its directional derivative
what $latex x = 0$$.
$pre

$$
The function corresponding to the argument $italic x$$ 
and the result $italic y$$ are represented
by their Taylor coefficients; i.e.,
$latex \[
\begin{array}{rcl}
	X(t) & = & x^{(0)} (t) + x^{(1)} t + \cdots + x^{(p)} t^p
	\\
	Y(t) & = & y^{(0)} (t) + y^{(1)} t + \cdots + y^{(p)} t^p
\end{array}
\] $$
Note that $latex x^{(0)} = X(0)$$ is the value of $italic x$$ and
$latex y^{(0)} = Y(0)$$ is the value of $italic y$$.
In the equations above, the order $latex p$$ is specified
by a call to $xref/Forward/$$ or $xref/Reverse/$$ as follows:
$syntax%
	%f%.Forward(%p%, %dx%)
	%f%.Reverse(%p%+1, %w%)
%$$
If all of the Taylor coefficients of $latex X(t)$$ are zero,
we define $latex k = p$$.
Otherwise, we define $latex k$$ to be the minimal index such that 
$latex x^{(k)} \neq 0$$.
Note that if $latex x \neq 0$$, $latex k = 0$$.
The Taylor coefficient representation of $latex Y(t)$$
(and hence it's derivatives) are computed as
$latex \[
y^{(\ell)}
=
\left\{ \begin{array}{ll} 
	 x^{(\ell)}   & {\rm if} \; x^{(k)} > 0         \\
	 0                    & {\rm if} \; x^{(k)} = 0 \\
	- x^{(\ell)}  & {\rm if} \; x^{(k)} < 0
\end{array} \right.
\] $$


$head Example$$
$children%
	example/abs.cpp
%$$
The file
$xref/Abs.cpp/$$
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
		CPPAD_ASSERT_UNKNOWN( NumVar(AbsOp) == 1 );
		CPPAD_ASSERT_UNKNOWN( NumInd(AbsOp) == 1 );
		ADTape<Base> *tape = tape_this();

		// corresponding operand address
		tape->Rec_.PutInd(taddr_);
		// put operator in the tape
		result.taddr_ = tape->Rec_.PutOp(AbsOp);
		// make result a variable
		result.id_    = tape->id_;
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
