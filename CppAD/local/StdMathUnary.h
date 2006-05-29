# ifndef CppADStdMathUnaryIncluded
# define CppADStdMathUnaryIncluded

/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */

/*
-------------------------------------------------------------------------------
$begin StdMathUnary$$
$spell
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
$$

$index standard, AD math unary$$
$index math, AD unary$$
$index unary, AD math$$

$index acos, AD$$
$index asin, AD$$
$index atan, AD$$
$index cos, AD$$
$index cosh, AD$$
$index exp, AD$$
$index log, AD$$
$index log10, AD$$
$index sin, AD$$
$index sinh, AD$$
$index sqrt, AD$$
$index tan, AD$$

$section AD Standard Math Unary Functions$$

$head Syntax$$
$syntax%%y% = %fun%(%x%)%$$


$head Purpose$$
Evaluates the one argument standard math function 
$italic fun$$ where its argument is an 
$xref/glossary/AD of Base/AD of/$$ $italic Base$$ object.

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
Most of these functions are AD of $italic Base$$
$xref/glossary/Operation/Atomic/atomic operations/1/$$.
In all cases,
The AD of $italic Base$$
operation sequence used to calculate $italic y$$ is 
$xref/glossary/Operation/Independent/independent/1/$$
of $italic x$$.

$head fun$$ 
A definition of $italic fun$$ for each of the argument types
list below is included in the $code CppAD$$ namespace
(the corresponding result has the same type as the argument).
These definitions extend to the 
$xref/glossary/AD Type Sequence/AD type sequence/$$
above each of the types list below.

$table
$bold fun$$   
	$cnext $bold Meaning$$
	$cnext $code float$$ 
	$cnext $code double$$
	$cnext $code std::complex<float>$$ 
	$cnext $code std::complex<double>$$
$rnext
$code acos$$  
	$cnext inverse cosine function
	$cnext yes $cnext yes $cnext no  $cnext no  $rnext
$code asin$$  
	$cnext inverse cosine function
	$cnext yes $cnext yes $cnext no  $cnext no  $rnext
$code atan$$  
	$cnext inverse tangent function
	$cnext yes $cnext yes $cnext no  $cnext no  $rnext
$code cos$$   
	$cnext trigonometric cosine function
	$cnext yes $cnext yes $cnext yes $cnext yes $rnext
$code cosh$$  
	$cnext hyperbolic cosine function
	$cnext yes $cnext yes $cnext yes $cnext yes $rnext
$code exp$$   
	$cnext exponential function
	$cnext yes $cnext yes $cnext yes $cnext yes $rnext
$code log$$   
	$cnext logarithm function
	$cnext yes $cnext yes $cnext yes $cnext yes $rnext
$code log10$$ 
	$cnext log base 10 function
	$cnext yes $cnext yes $cnext yes $cnext yes $rnext
$code sin$$   
	$cnext trigonometric sine function
	$cnext yes $cnext yes $cnext yes $cnext yes $rnext
$code sinh$$  
	$cnext hyperbolic sine function
	$cnext yes $cnext yes $cnext yes $cnext yes $rnext
$code sqrt$$  
	$cnext square root function
	$cnext yes $cnext yes $cnext yes $cnext yes $rnext
$code tan$$  
	$cnext square root function
	$cnext yes $cnext yes $cnext yes $cnext yes $rnext
$tend


$head Examples$$
The following files
contain examples and tests of these functions.   
Each test returns true if it succeeds and false otherwise.
$children%
	Example/Acos.cpp%
	Example/Asin.cpp%
	Example/Atan.cpp%
	Example/Cos.cpp%
	Example/Cosh.cpp%
	Example/Exp.cpp%
	Example/Log.cpp%
	Example/Log10.cpp%
	Example/Sin.cpp%
	Example/Sinh.cpp%
	Example/Sqrt.cpp%
	Example/Tan.cpp
%$$
$table
$rref Acos.cpp$$
$rref Asin.cpp$$
$rref Atan.cpp$$
$rref Cos.cpp$$
$rref Cosh.cpp$$
$rref Exp.cpp$$
$rref Log.cpp$$
$rref Log10.cpp$$
$rref Sin.cpp$$
$rref Sinh.cpp$$
$rref Sqrt.cpp$$
$rref Tan.cpp$$
$tend


$head Derivatives$$
Each of these functions satisfy a standard math function differential equation.
Calculating derivatives using this differential equation 
is discussed for 
both $xref/ForwardTheory/Standard Math Functions/forward/$$
and $xref/ReverseTheory/Standard Math Functions/reverse/$$ mode.
The exact form of the differential equation
for each of these functions is listed below:

$subhead acos$$
$latex \[
\begin{array}{lcr}
	\D{[ {\rm acos} (x) ]}{x} & = & - (1 - x * x)^{-1/2}
\end{array}
\] $$

$subhead asin$$
$latex \[
\begin{array}{lcr}
	\D{[ {\rm asin} (x) ]}{x} & = & (1 - x * x)^{-1/2}
\end{array}
\] $$

$subhead atan$$
$latex \[
\begin{array}{lcr}
        \D{[ {\rm atan} (x) ]}{x} & = & \frac{1}{1 + x^2}
\end{array}
\] $$

$subhead cos$$
$latex \[
\begin{array}{lcr}
        \D{[ \cos (x) ]}{x} & = & - \sin (x)  \\
        \D{[ \sin (x) ]}{x} & = & \cos (x)
\end{array}
\] $$

$subhead cosh$$
$latex \[
\begin{array}{lcr}
        \D{[ \cosh (x) ]}{x} & = & \sinh (x)  \\
        \D{[ \sin (x) ]}{x}  & = & \cosh (x)
\end{array}
\] $$

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

$subhead log10$$
This function is special in that it's derivatives are calculated
using the relation
$latex \[
\begin{array}{lcr}
        {\rm log10} (x) & = & \log(x) / \log(10)
\end{array}
\] $$

$subhead sin$$
$latex \[
\begin{array}{lcr}
        \D{[ \sin (x) ]}{x} & = & \cos (x) \\
        \D{[ \cos (x) ]}{x} & = & - \sin (x) 
\end{array}
\] $$

$subhead sinh$$
$latex \[
\begin{array}{lcr}
        \D{[ \sinh (x) ]}{x} & = & \cosh (x)   \\
        \D{[ \cosh (x) ]}{x} & = & \sinh (x)
\end{array}
\] $$

$subhead sqrt$$
$latex \[
\begin{array}{lcr}
        \D{[ {\rm sqrt} (x) ]}{x} & = & \frac{1}{2 {\rm sqrt} (x) }
\end{array}
\] $$

$subhead tan$$
This function is special in that it's derivatives are calculated
using the relation
$latex \[
\begin{array}{lcr}
        \tan (x) & = & \sin(x) / \cos(x)
\end{array}
\] $$

$end
-------------------------------------------------------------------------------
*/

# define CppADStandardMathUnaryBaseAll(Name)                              \
                                                                          \
	inline float Name(float x)                                        \
	{	return std::Name(x); }                                    \
                                                                          \
	inline double Name(double x)                                      \
	{	return std::Name(x); }                                    \
                                                                          \
	inline std::complex<float> Name(std::complex<float> x)            \
	{	return std::Name(x); }                                    \
                                                                          \
	inline std::complex<double> Name(std::complex<double> x)          \
	{	return std::Name(x); }

# define CppADStandardMathUnaryBaseHalf(Name)                             \
                                                                          \
	inline float Name(float x)                                        \
	{	return std::Name(x); }                                    \
                                                                          \
	inline double Name(double x)                                      \
	{	return std::Name(x); }                                    \
                                                                          \
	inline std::complex<float> Name(std::complex<float> x)            \
	{	CppADUsageError(                                          \
			0,                                                \
			#Name ": attempt to use with complex argument"    \
		)                                                         \
		return x;                                                 \
	}                                                                 \
                                                                          \
	inline std::complex<double> Name(std::complex<double> x)          \
	{	CppADUsageError(                                          \
			0,                                                \
			#Name ": attempt to use with complex argument"    \
		)                                                         \
		return x;                                                 \
	}

# define CppADStandardMathUnaryTemplate(Name, Op)                         \
	template <class Base>                                             \
	inline AD<Base> AD<Base>::Name (void) const                       \
        {	using CppAD::Name;                                        \
		AD<Base> result;                                          \
		CppADUnknownError( result.id == 0 );                      \
		result.value = Name(value);                               \
		if( Variable(*this) )                                     \
			Tape()->RecordOp(Op, result, taddr);              \
		return result;                                            \
	}                                                                 \
	template <class Base>                                             \
	inline AD<Base> Name(const AD<Base> &x)                           \
	{	return x.Name(); }                                        \
	template <class Base>                                             \
	inline AD<Base> Name(const VecAD_reference<Base> &x)              \
	{	return Name( x.ADBase() ); }

//  BEGIN CppAD namespace
namespace CppAD {

        // acos
        CppADStandardMathUnaryBaseHalf(acos)
        CppADStandardMathUnaryTemplate(acos, AcosOp)

        // asin
        CppADStandardMathUnaryBaseHalf(asin)
        CppADStandardMathUnaryTemplate(asin, AsinOp)

        // atan
        CppADStandardMathUnaryBaseHalf(atan)
        CppADStandardMathUnaryTemplate(atan, AtanOp)

        // cos
        CppADStandardMathUnaryBaseAll(cos)
        CppADStandardMathUnaryTemplate(cos, CosOp)

        // cosh
        CppADStandardMathUnaryBaseAll(cosh)
        CppADStandardMathUnaryTemplate(cosh, CoshOp)

        // exp
        CppADStandardMathUnaryBaseAll(exp)
        CppADStandardMathUnaryTemplate(exp, ExpOp)

        // log
        CppADStandardMathUnaryBaseAll(log)
        CppADStandardMathUnaryTemplate(log, LogOp)

        // log10
	template <class Base>
	inline AD<Base> log10(const AD<Base> &x)
	{	return CppAD::log(x) / CppAD::log( Base(10) ); }
	template <class Base>
	inline AD<Base> log10(const VecAD_reference<Base> &x)
	{	return CppAD::log(x.ADBase()) / CppAD::log( Base(10) ); }

        // sin
        CppADStandardMathUnaryBaseAll(sin)
        CppADStandardMathUnaryTemplate(sin, SinOp)

        // sinh
        CppADStandardMathUnaryBaseAll(sinh)
        CppADStandardMathUnaryTemplate(sinh, SinhOp)

        // sqrt
        CppADStandardMathUnaryBaseAll(sqrt)
        CppADStandardMathUnaryTemplate(sqrt, SqrtOp)
	
        // tan
	template <class Base>
	inline AD<Base> tan(const AD<Base> &x)
	{	return CppAD::sin(x) / CppAD::cos(x); }
	template <class Base>
	inline AD<Base> tan(const VecAD_reference<Base> &x)
	{	return CppAD::sin(x.ADBase()) / CppAD::cos(x.ADBase()); }
}

# undef CppADStandardMathUnaryBaseAll
# undef CppADStandardMathUnaryBaseHalf
# undef CppADStandardMathUnaryTemplate

# endif 
