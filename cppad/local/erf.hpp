# ifndef CPPAD_ERF_INCLUDED
# define CPPAD_ERF_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
-------------------------------------------------------------------------------
$begin erf$$

$section The AD Error Function$$
$spell
	Cpp
	namespace
	Vec
	erf
	const
$$

$index erf, AD function$$
$index error, AD function$$
$index function, AD error$$

$head Syntax$$
$syntax%%y% = erf(%x%)%$$


$head Description$$
Returns the value of the error function which is defined by
$latex \[
{\rm erf} (x) = \frac{2}{ \sqrt{\pi} } \int_0^x \exp( - t * t ) \; {\bf d} t
\] $$

$head Base$$ 
A definition of $code erf$$ for arguments of type
$code float$$ and $code double$$ 
is included in the $code CppAD$$ namespace
(the corresponding results has the same type as the arguments).
The type $italic Base$$ can be any type in the 
$cref/AD levels above/glossary/AD Levels Above Base/$$
above $code float$$ or $code double$$.

$head x$$
The argument $italic x$$ has prototype
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
The AD of $italic Base$$
operation sequence used to calculate $italic y$$ is
$xref/glossary/Operation/Independent/independent/1/$$
of $italic x$$.

$head Example$$
$children%
	example/erf.cpp
%$$
The file
$xref/Erf.cpp/$$
contains an example and test of this function.   
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice 
 * is preserved.
 * ====================================================
 *
 *
 * double erf(double x)
 * double erfc(double x)
 *			     x
 *		      2      |\
 *     erf(x)  =  ---------  | exp(-t*t)dt
 *	 	   sqrt(pi) \| 
 *			     0
 *
 *     erfc(x) =  1-erf(x)
 *  Note that 
 *		erf(-x) = -erf(x)
 *		erfc(-x) = 2 - erfc(x)
 *
 * Method:
 *	1. For |x| in [0, 0.84375]
 *	    erf(x)  = x + x*R(x^2)
 *          erfc(x) = 1 - erf(x)           if x in [-.84375,0.25]
 *                  = 0.5 + ((0.5-x)-x*R)  if x in [0.25,0.84375]
 *	   where R = P/Q where P is an odd poly of degree 8 and
 *	   Q is an odd poly of degree 10.
 *						 -57.90
 *			| R - (erf(x)-x)/x | <= 2
 *	
 *
 *	   Remark. The formula is derived by noting
 *          erf(x) = (2/sqrt(pi))*(x - x^3/3 + x^5/10 - x^7/42 + ....)
 *	   and that
 *          2/sqrt(pi) = 1.128379167095512573896158903121545171688
 *	   is close to one. The interval is chosen because the fix
 *	   point of erf(x) is near 0.6174 (i.e., erf(x)=x when x is
 *	   near 0.6174), and by some experiment, 0.84375 is chosen to
 * 	   guarantee the error is less than one ulp for erf.
 *
 *      2. For |x| in [0.84375,1.25], let s = |x| - 1, and
 *         c = 0.84506291151 rounded to single (24 bits)
 *         	erf(x)  = sign(x) * (c  + P1(s)/Q1(s))
 *         	erfc(x) = (1-c)  - P1(s)/Q1(s) if x > 0
 *			  1+(c+P1(s)/Q1(s))    if x < 0
 *         	|P1/Q1 - (erf(|x|)-c)| <= 2**-59.06
 *	   Remark: here we use the taylor series expansion at x=1.
 *		erf(1+s) = erf(1) + s*Poly(s)
 *			 = 0.845.. + P1(s)/Q1(s)
 *	   That is, we use rational approximation to approximate
 *			erf(1+s) - (c = (single)0.84506291151)
 *	   Note that |P1/Q1|< 0.078 for x in [0.84375,1.25]
 *	   where 
 *		P1(s) = degree 6 poly in s
 *		Q1(s) = degree 6 poly in s
 *
 *      3. For x in [1.25,1/0.35(~2.857143)], 
 *         	erfc(x) = (1/x)*exp(-x*x-0.5625+R1/S1)
 *         	erf(x)  = 1 - erfc(x)
 *	   where 
 *		R1(z) = degree 7 poly in z, (z=1/x^2)
 *		S1(z) = degree 8 poly in z
 *
 *      4. For x in [1/0.35,28]
 *         	erfc(x) = (1/x)*exp(-x*x-0.5625+R2/S2) if x > 0
 *			= 2.0 - (1/x)*exp(-x*x-0.5625+R2/S2) if -6<x<0
 *			= 2.0 - tiny		(if x <= -6)
 *         	erf(x)  = sign(x)*(1.0 - erfc(x)) if x < 6, else
 *         	erf(x)  = sign(x)*(1.0 - tiny)
 *	   where
 *		R2(z) = degree 6 poly in z, (z=1/x^2)
 *		S2(z) = degree 7 poly in z
 *
 *      Note1:
 *	   To compute exp(-x*x-0.5625+R/S), let s be a single
 *	   precision number and s := x; then
 *		-x*x = -s*s + (s-x)*(s+x)
 *	        exp(-x*x-0.5626+R/S) = 
 *			exp(-s*s-0.5625)*exp((s-x)*(s+x)+R/S);
 *      Note2:
 *	   Here 4 and 5 make use of the asymptotic series
 *			  exp(-x*x)
 *		erfc(x) ~ ---------- * ( 1 + Poly(1/x^2) )
 *			  x*sqrt(pi)
 *	   We use rational approximation to approximate
 *      	g(s)=f(1/x^2) = log(erfc(x)*x) - x*x + 0.5625
 *	   Here is the error bound for R1/S1 and R2/S2
 *      	|R1/S1 - f(x)|  < 2**(-62.57)
 *      	|R2/S2 - f(x)|  < 2**(-61.52)
 *
 *      5. For inf > x >= 28
 *         	erf(x)  = sign(x) *(1 - tiny)  (raise inexact)
 *         	erfc(x) = tiny*tiny (raise underflow) if x > 0
 *			= 2 - tiny if x<0
 *
 *      7. Special case:
 *         	erf(0)  = 0, erf(inf)  = 1, erf(-inf) = -1,
 *         	erfc(0) = 1, erfc(inf) = 0, erfc(-inf) = 2, 
 *	   	erfc/erf(NaN) is NaN
 */


# include <cmath>
# include <cppad/cppad.hpp>

// BEGIN CppAD namespace
namespace CppAD {   

template <class Type>
Type erf_template(const Type &x)
{	using CppAD::exp;

	static const Type
		tiny = static_cast<Type>(  1e-300                     ),
		one  = static_cast<Type>(  1.00000000000000000000e+00 ),
			
		erx  = static_cast<Type>(  8.45062911510467529297e-01 ),
		/*
		 * Coefficients for approximation to  erf on [0,0.84375]
		 */
		pp0  = static_cast<Type>(  1.28379167095512558561e-01 ),
		pp1  = static_cast<Type>( -3.25042107247001499370e-01 ),
		pp2  = static_cast<Type>( -2.84817495755985104766e-02 ),
		pp3  = static_cast<Type>( -5.77027029648944159157e-03 ),
		pp4  = static_cast<Type>( -2.37630166566501626084e-05 ),
		qq1  = static_cast<Type>(  3.97917223959155352819e-01 ),
		qq2  = static_cast<Type>(  6.50222499887672944485e-02 ),
		qq3  = static_cast<Type>(  5.08130628187576562776e-03 ),
		qq4  = static_cast<Type>(  1.32494738004321644526e-04 ),
		qq5  = static_cast<Type>( -3.96022827877536812320e-06 ),
		/*
		 * Coefficients for approximation to  erf  in [0.84375,1.25] 
		 */
		pa0  = static_cast<Type>( -2.36211856075265944077e-03 ),
		pa1  = static_cast<Type>(  4.14856118683748331666e-01 ),
		pa2  = static_cast<Type>( -3.72207876035701323847e-01 ),
		pa3  = static_cast<Type>(  3.18346619901161753674e-01 ),
		pa4  = static_cast<Type>( -1.10894694282396677476e-01 ),
		pa5  = static_cast<Type>(  3.54783043256182359371e-02 ),
		pa6  = static_cast<Type>( -2.16637559486879084300e-03 ),
		qa1  = static_cast<Type>(  1.06420880400844228286e-01 ),
		qa2  = static_cast<Type>(  5.40397917702171048937e-01 ),
		qa3  = static_cast<Type>(  7.18286544141962662868e-02 ),
		qa4  = static_cast<Type>(  1.26171219808761642112e-01 ),
		qa5  = static_cast<Type>(  1.36370839120290507362e-02 ),
		qa6  = static_cast<Type>(  1.19844998467991074170e-02 ),
		/*
		 * Coefficients for approximation to  erfc in [1.25,1/0.35]
		 */
		ra0  = static_cast<Type>( -9.86494403484714822705e-03 ),
		ra1  = static_cast<Type>( -6.93858572707181764372e-01 ),
		ra2  = static_cast<Type>( -1.05586262253232909814e+01 ),
		ra3  = static_cast<Type>( -6.23753324503260060396e+01 ),
		ra4  = static_cast<Type>( -1.62396669462573470355e+02 ),
		ra5  = static_cast<Type>( -1.84605092906711035994e+02 ),
		ra6  = static_cast<Type>( -8.12874355063065934246e+01 ),
		ra7  = static_cast<Type>( -9.81432934416914548592e+00 ),
		sa1  = static_cast<Type>(  1.96512716674392571292e+01 ),
		sa2  = static_cast<Type>(  1.37657754143519042600e+02 ),
		sa3  = static_cast<Type>(  4.34565877475229228821e+02 ),
		sa4  = static_cast<Type>(  6.45387271733267880336e+02 ),
		sa5  = static_cast<Type>(  4.29008140027567833386e+02 ),
		sa6  = static_cast<Type>(  1.08635005541779435134e+02 ),
		sa7  = static_cast<Type>(  6.57024977031928170135e+00 ),
		sa8  = static_cast<Type>( -6.04244152148580987438e-02 ),
		/*
		 * Coefficients for approximation to  erfc in [1/.35,28]
		 */
		rb0  = static_cast<Type>( -9.86494292470009928597e-03 ),
		rb1  = static_cast<Type>( -7.99283237680523006574e-01 ),
		rb2  = static_cast<Type>( -1.77579549177547519889e+01 ),
		rb3  = static_cast<Type>( -1.60636384855821916062e+02 ),
		rb4  = static_cast<Type>( -6.37566443368389627722e+02 ),
		rb5  = static_cast<Type>( -1.02509513161107724954e+03 ),
		rb6  = static_cast<Type>( -4.83519191608651397019e+02 ),
		sb1  = static_cast<Type>(  3.03380607434824582924e+01 ),
		sb2  = static_cast<Type>(  3.25792512996573918826e+02 ),
		sb3  = static_cast<Type>(  1.53672958608443695994e+03 ),
		sb4  = static_cast<Type>(  3.19985821950859553908e+03 ),
		sb5  = static_cast<Type>(  2.55305040643316442583e+03 ),
		sb6  = static_cast<Type>(  4.74528541206955367215e+02 ),
		sb7  = static_cast<Type>( -2.24409524465858183362e+01 );

	Type R,S,P,Q,s,y,z,r;

	Type ax, value;

	Type zero(0);

	// |x|
	ax = CondExpGe(x, zero, x, -x);

	// erf(|x|) when |x| <= 0.84375
	z = ax*ax;
	r = pp0+z*(pp1+z*(pp2+z*(pp3+z*pp4)));
	s = one+z*(qq1+z*(qq2+z*(qq3+z*(qq4+z*qq5))));
	y = r/s;
	Type Le0Dot84375 = ax + ax*y;

	// erf(|x|) when 0.84375 < |x| <= 1.25 
	s = ax-one;
	P = pa0+s*(pa1+s*(pa2+s*(pa3+s*(pa4+s*(pa5+s*pa6)))));
	Q = one+s*(qa1+s*(qa2+s*(qa3+s*(qa4+s*(qa5+s*qa6)))));
	value = erx + P/Q; 

	// erf(|x|) when |x| <= 1.25
	Type Le1Dot25 = CondExpGt(ax, Type(0.84375), value, Le0Dot84375);

	// erf(|x|) when 1.25 < |x| <= 1 / 0.35
	s = one/(ax*ax);
	R=ra0+s*(ra1+s*(ra2+s*(ra3+s*(ra4+s*(
		ra5+s*(ra6+s*ra7))))));
	S=one+s*(sa1+s*(sa2+s*(sa3+s*(sa4+s*(
		sa5+s*(sa6+s*(sa7+s*sa8)))))));
	r  =  exp(Type(-0.5625)) * exp(-ax*ax+R/S);
	value = one-r/ax;

	// erf(|x|) when |x| <= 1 / 0.35
	Type Le1OverDot35 = CondExpGt(ax, Type(1.25), value, Le1Dot25);

	// erf(|x|) when 1 / 0.35 < |x| <= 6
	R=rb0+s*(rb1+s*(rb2+s*(rb3+s*(rb4+s*(
		rb5+s*rb6)))));
	S=one+s*(sb1+s*(sb2+s*(sb3+s*(sb4+s*(
		sb5+s*(sb6+s*sb7))))));
	r  =  exp(Type(-0.5625)) * exp(-ax*ax+R/S);
	value = one-r/ax;

	// erf(|x|) when |x| <= 6
	Type Le6 = CondExpGt(ax, Type(1/.35), value, Le1OverDot35); 

	// erf(|x|) when |x| > 6
	Type Gt6 = one - tiny;

	// erf(|x|) 
	Type Gt0 = CondExpGt(ax, Type(6.), Gt6, Le6);

	// Gt0 = erf( CondExpGe(x, zero, x, -x) )
	// so must switch sign in case where x < 0
	return CondExpGe(x, zero, Gt0, -Gt0);
}

inline float erf(float x)
{	return erf_template(x); }

inline double erf(double x)
{	return erf_template(x); }

inline std::complex<float>  erf(std::complex<float> x)
{	CppADUsageError(
		0,
		"CppAD::erf: attempt to use with std::complex<float> argument"
	);
	return std::complex<float>(0);
}

inline std::complex<double>  erf(std::complex<double> x)
{	CppADUsageError(
		0,
		"CppAD::erf: attempt to use with std::complex<double> argument"
	);
	return std::complex<double>(0);
}

template <class Base>
inline AD<Base> erf(const AD<Base> &x)
{	return erf_template(x); }

template <class Base>
inline AD<Base> erf(const VecAD_reference<Base> &x)
{	return erf_template( x.ADBase() ); }


} // END CppAD namespace

# endif
