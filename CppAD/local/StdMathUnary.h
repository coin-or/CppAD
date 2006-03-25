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

$table
$bold Syntax$$ $cnext 
$syntax% %y% = %fun%(%x%)%$$
$tend

$fend 20$$

$head Purpose$$
Returns an
$syntax%AD<%Base%>%$$ object that is equal to
$syntax%
	%fun%(%x%)
%$$
where $italic fun$$ has the same interpretation as for the
$italic Base$$ type. 

$head x$$
The argument $italic x$$ has prototype
$syntax%
	const AD<%Base%> &%x%
%$$

$head y$$
The result $italic y$$ has prototype
$syntax%
	AD<%Base%> %y%
%$$

$head fun$$ 
A definition of $italic fun$$ for a $syntax%AD<%Base%>%$$ argument
(for each of the cases listed below)
is automatically included in the $code CppAD$$ namespace.
This definition depends on the function $italic fun$$ being
defined for a $italic Base$$ argument.
Each "yes" below corresponds to a function $italic fun$$
and an argument type $italic Base$$ for which a definition is also included
in the $code CppAD$$ namespace.

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
		if( (Tape()->State() == Recording) & Variable(*this) )    \
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
	template <class Type>
	inline Type log10(const Type &x)
	{	return CppAD::log(x) / CppAD::log( Type(10) ); }

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
	template <class Type>
	inline Type tan(const Type &x)
	{	return CppAD::sin(x) / CppAD::cos(x); }
}

# undef CppADStandardMathUnaryBaseAll
# undef CppADStandardMathUnaryBaseHalf
# undef CppADStandardMathUnaryTemplate

# endif 
