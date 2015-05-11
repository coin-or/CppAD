/* $Id$ */
# ifndef CPPAD_STD_MATH_AD_INCLUDED
# define CPPAD_STD_MATH_AD_INCLUDED

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
$begin acos$$
$spell
	acos
$$

$section Inverse Sine Function: acos$$

$head Syntax$$
$icode%y% = acos(%x%)%$$

$head x, y$$
See the $cref/possible types/unary_standard_math/Possible Types/$$
for a unary standard math function.

$head Atomic$$
This is an $cref/atomic operation/glossary/Operation/Atomic/$$.

$head Derivative$$
$latex \[
\begin{array}{lcr}
	\R{acos}^{(1)} (x) & = & - (1 - x * x)^{-1/2}
\end{array}
\] $$

$head Example$$
$children%
	example/acos.cpp
%$$
The file
$cref acos.cpp$$
contains an example and test of this function.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
$begin asin$$
$spell
	asin
$$

$section Inverse Sine Function: asin$$

$head Syntax$$
$icode%y% = asin(%x%)%$$

$head x, y$$
See the $cref/possible types/unary_standard_math/Possible Types/$$
for a unary standard math function.

$head Atomic$$
This is an $cref/atomic operation/glossary/Operation/Atomic/$$.

$head Derivative$$
$latex \[
\begin{array}{lcr}
	\R{asin}^{(1)} (x) & = & (1 - x * x)^{-1/2}
\end{array}
\] $$

$head Example$$
$children%
	example/asin.cpp
%$$
The file
$cref asin.cpp$$
contains an example and test of this function.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
$begin atan$$
$spell
	atan
$$

$section Inverse Tangent Function: atan$$

$head Syntax$$
$icode%y% = atan(%x%)%$$

$head x, y$$
See the $cref/possible types/unary_standard_math/Possible Types/$$
for a unary standard math function.

$head Atomic$$
This is an $cref/atomic operation/glossary/Operation/Atomic/$$.

$head Derivative$$
$latex \[
\begin{array}{lcr}
	\R{atan}^{(1)} (x) & = & \frac{1}{1 + x^2}
\end{array}
\] $$

$head Example$$
$children%
	example/atan.cpp
%$$
The file
$cref atan.cpp$$
contains an example and test of this function.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
$begin cos$$
$spell
	cos
$$

$section The Cosine Function: cos$$

$head Syntax$$
$icode%y% = cos(%x%)%$$

$head x, y$$
See the $cref/possible types/unary_standard_math/Possible Types/$$
for a unary standard math function.

$head Atomic$$
This is an $cref/atomic operation/glossary/Operation/Atomic/$$.

$head Derivative$$
$latex \[
\begin{array}{lcr}
	\R{cos}^{(1)} (x) & = & - \sin(x)
\end{array}
\] $$

$head Example$$
$children%
	example/cos.cpp
%$$
The file
$cref cos.cpp$$
contains an example and test of this function.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
$begin cosh$$
$spell
	cosh
$$

$section The Hyperbolic Cosine Function: cosh$$

$head Syntax$$
$icode%y% = cosh(%x%)%$$

$head x, y$$
See the $cref/possible types/unary_standard_math/Possible Types/$$
for a unary standard math function.

$head Atomic$$
This is an $cref/atomic operation/glossary/Operation/Atomic/$$.

$head Derivative$$
$latex \[
\begin{array}{lcr}
	\R{cosh}^{(1)} (x) & = &  \sinh(x)
\end{array}
\] $$

$head Example$$
$children%
	example/cosh.cpp
%$$
The file
$cref cosh.cpp$$
contains an example and test of this function.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
$begin log10$$
$spell
	CppAD
$$

$section The Base 10 Logarithm Function: log10$$

$head Syntax$$
$icode%y% = log10(%x%)%$$

$head x, y$$
See the $cref/possible types/unary_standard_math/Possible Types/$$
for a unary standard math function.

$head Method$$
CppAD uses the representation
$latex \[
\begin{array}{lcr}
        {\rm log10} (x) & = & \log(x) / \log(10)
\end{array}
\] $$

$head Example$$
$children%
	example/log10.cpp
%$$
The file
$cref log10.cpp$$
contains an example and test of this function.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
$begin sin$$
$spell
	sin
$$

$section The Sine Function: sin$$

$head Syntax$$
$icode%y% = sin(%x%)%$$

$head x, y$$
See the $cref/possible types/unary_standard_math/Possible Types/$$
for a unary standard math function.

$head Atomic$$
This is an $cref/atomic operation/glossary/Operation/Atomic/$$.

$head Derivative$$
$latex \[
\begin{array}{lcr}
	\R{sin}^{(1)} (x) & = &  \cos(x)
\end{array}
\] $$

$head Example$$
$children%
	example/sin.cpp
%$$
The file
$cref sin.cpp$$
contains an example and test of this function.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
$begin sinh$$
$spell
	sinh
$$

$section The Hyperbolic Sine Function: sinh$$

$head Syntax$$
$icode%y% = sinh(%x%)%$$

$head x, y$$
See the $cref/possible types/unary_standard_math/Possible Types/$$
for a unary standard math function.

$head Atomic$$
This is an $cref/atomic operation/glossary/Operation/Atomic/$$.

$head Derivative$$
$latex \[
\begin{array}{lcr}
	\R{sinh}^{(1)} (x) & = &  \cosh(x)
\end{array}
\] $$

$head Example$$
$children%
	example/sinh.cpp
%$$
The file
$cref sinh.cpp$$
contains an example and test of this function.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
$begin std_math_ad$$
$spell
	Vec
	std
	const
	exp
	fabs
	sqrt
	CppAD
	namespace
	tanh
$$

$section AD Standard Math Unary Functions$$

$head Syntax$$
$icode%y% = %fun%(%x%)%$$


$head Purpose$$
Evaluates the one argument standard math function
$icode fun$$ where its argument is an
$cref/AD of/glossary/AD of Base/$$ $icode Base$$ object.

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
Most of these functions are AD of $icode Base$$
$cref/atomic operations/glossary/Operation/Atomic/$$.
In all cases,
The AD of $icode Base$$
operation sequence used to calculate $icode y$$ is
$cref/independent/glossary/Operation/Independent/$$
of $icode x$$.

$head fun$$
A definition of $icode fun$$ is included
for each of the following functions:
$code exp$$,
$code fabs$$,
$code log$$,
$code sqrt$$,
$code tan$$,
$code tanh$$.


$head Examples$$
The following files
contain examples and tests of these functions.
Each test returns true if it succeeds and false otherwise.
$children%
	example/exp.cpp%
	example/log.cpp%
	example/sqrt.cpp%
	example/tan.cpp%
	example/tanh.cpp
%$$
$table
$rref abs.cpp$$
$rref Acos.cpp$$
$rref Asin.cpp$$
$rref exp.cpp$$
$rref log.cpp$$
$rref sqrt.cpp$$
$rref tan.cpp$$
$rref tanh.cpp$$
$tend


$head Derivatives$$
Each of these functions satisfy a standard math function differential equation.
Calculating derivatives using this differential equation
is discussed for
both $cref/forward/ForwardTheory/Standard Math Functions/$$
and $cref/reverse/ReverseTheory/Standard Math Functions/$$ mode.
The exact form of the differential equation
for each of these functions is listed below:

$subhead exp$$
$latex \[
\begin{array}{lcr}
        \D{[ \exp (x) ]}{x} & = & \exp (x)
\end{array}
\] $$

$subhead log$$
$latex \[
\begin{array}{lcr}
        \D{[ \log (x) ]}{x} & = & \frac{1}{x}
\end{array}
\] $$


$subhead sqrt$$
$latex \[
\begin{array}{lcr}
        \D{[ {\rm sqrt} (x) ]}{x} & = & \frac{1}{2 {\rm sqrt} (x) }
\end{array}
\] $$

$subhead tan$$
$latex \[
\begin{array}{lcr}
        \D{[ \tan (x) ]}{x} & = & 1 + \tan (x)^2
\end{array}
\] $$

$subhead tanh$$
$latex \[
\begin{array}{lcr}
        \D{[ \tanh (x) ]}{x} & = & 1 - \tanh (x)^2
\end{array}
\] $$

$end
-------------------------------------------------------------------------------
*/

/*!
\file std_math_ad.hpp
Define AD<Base> standard math functions (using their Base versions)
*/

/*!
\def CPPAD_STANDARD_MATH_UNARY_AD(Name, Op)
Defines function Name with argument type AD<Base> and tape operation Op

The macro defines the function x.Name() where x has type AD<Base>.
It then uses this funciton to define Name(x) where x has type
AD<Base> or VecAD_reference<Base>.

If x is a variable, the tape unary operator Op is used
to record the operation and the result is identified as correspoding
to this operation; i.e., Name(x).taddr_ idendifies the operation and
Name(x).tape_id_ identifies the tape.

This macro is used to define AD<Base> versions of
acos, asin, atan, cos, cosh, exp, fabs, log, sin, sinh, sqrt, tan, tanh.
*/

# define CPPAD_STANDARD_MATH_UNARY_AD(Name, Op)                   \
    template <class Base>                                         \
    inline AD<Base> Name(const AD<Base> &x)                       \
    {   return x.Name(); }                                        \
    template <class Base>                                         \
    inline AD<Base> AD<Base>::Name (void) const                   \
    {                                                             \
        AD<Base> result;                                          \
        result.value_ = CppAD::Name(value_);                      \
        CPPAD_ASSERT_UNKNOWN( Parameter(result) );                \
                                                                  \
        if( Variable(*this) )                                     \
        {   CPPAD_ASSERT_UNKNOWN( NumArg(Op) == 1 );              \
            ADTape<Base> *tape = tape_this();                     \
            tape->Rec_.PutArg(taddr_);                            \
            result.taddr_ = tape->Rec_.PutOp(Op);                 \
            result.tape_id_    = tape->id_;                       \
        }                                                         \
        return result;                                            \
    }                                                             \
    template <class Base>                                         \
    inline AD<Base> Name(const VecAD_reference<Base> &x)          \
    {   return Name( x.ADBase() ); }

//  BEGIN CppAD namespace
namespace CppAD {

     CPPAD_STANDARD_MATH_UNARY_AD(acos, AcosOp)
     CPPAD_STANDARD_MATH_UNARY_AD(asin, AsinOp)
     CPPAD_STANDARD_MATH_UNARY_AD(atan, AtanOp)
     CPPAD_STANDARD_MATH_UNARY_AD(cos, CosOp)
     CPPAD_STANDARD_MATH_UNARY_AD(cosh, CoshOp)
     CPPAD_STANDARD_MATH_UNARY_AD(exp, ExpOp)
     CPPAD_STANDARD_MATH_UNARY_AD(fabs, AbsOp)
     CPPAD_STANDARD_MATH_UNARY_AD(log, LogOp)
     CPPAD_STANDARD_MATH_UNARY_AD(sin, SinOp)
     CPPAD_STANDARD_MATH_UNARY_AD(sinh, SinhOp)
     CPPAD_STANDARD_MATH_UNARY_AD(sqrt, SqrtOp)
     CPPAD_STANDARD_MATH_UNARY_AD(tan, TanOp)
     CPPAD_STANDARD_MATH_UNARY_AD(tanh, TanhOp)

# if CPPAD_USE_CPLUSPLUS_2011
     CPPAD_STANDARD_MATH_UNARY_AD(asinh, AsinhOp)
     CPPAD_STANDARD_MATH_UNARY_AD(acosh, AcoshOp)
     CPPAD_STANDARD_MATH_UNARY_AD(atanh, AtanhOp)
     CPPAD_STANDARD_MATH_UNARY_AD(expm1, Expm1Op)
     CPPAD_STANDARD_MATH_UNARY_AD(log1p, Log1pOp)
# endif

# if CPPAD_USE_CPLUSPLUS_2011
	// Error function is a special case
	template <class Base>
	inline AD<Base> erf(const AD<Base> &x)
	{	return x.erf(); }
	template <class Base>
	inline AD<Base> AD<Base>::erf (void) const
	{
		AD<Base> result;
		result.value_ = CppAD::erf(value_);
		CPPAD_ASSERT_UNKNOWN( Parameter(result) );

		if( Variable(*this) )
		{	CPPAD_ASSERT_UNKNOWN( NumArg(ErfOp) == 3 );
			ADTape<Base> *tape = tape_this();
			// arg[0] = argument to erf function
			tape->Rec_.PutArg(taddr_);
			// arg[1] = zero
			addr_t p  = tape->Rec_.PutPar( Base(0) );
			tape->Rec_.PutArg(p);
			// arg[2] = 2 / sqrt(pi)
			p = tape->Rec_.PutPar(Base(
				1.0 / std::sqrt( std::atan(1.0) )
			));
			tape->Rec_.PutArg(p);
			//
			result.taddr_ = tape->Rec_.PutOp(ErfOp);
			result.tape_id_    = tape->id_;
		}
		return result;
	}
	template <class Base>
	inline AD<Base> erf(const VecAD_reference<Base> &x)
	{	return erf( x.ADBase() ); }
# endif

     /*!
	Compute the log of base 10 of x where  has type AD<Base>

	\tparam Base
	is the base type (different from base for log)
	for this AD type, see base_require.

	\param x
	is the argument for the log10 function.

	\result
	if the result is y, then \f$ x = 10^y \f$.
	*/
     template <class Base>
     inline AD<Base> log10(const AD<Base> &x)
     {    return CppAD::log(x) / CppAD::log( Base(10) ); }
     template <class Base>
     inline AD<Base> log10(const VecAD_reference<Base> &x)
     {    return CppAD::log(x.ADBase()) / CppAD::log( Base(10) ); }
}

# undef CPPAD_STANDARD_MATH_UNARY_AD

# endif
