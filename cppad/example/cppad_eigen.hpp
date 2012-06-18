/* $Id$ */
# ifndef CPPAD_CPPAD_EIGEN_INCLUDED
# define CPPAD_CPPAD_EIGEN_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin cppad_eigen.hpp$$
$spell
	atan
	Num
	acos
	asin
	CppAD
	std::numeric
	enum
	Mul
	Eigen
	cppad.hpp
	namespace
	struct
	typedef
	const
	imag
	sqrt
	exp
	cos
$$
$section Enable Use of Eigen Linear Algebra Package with CppAD$$

$head Syntax$$
$codei%# include <cppad/example/cppad_eigen.hpp>%$$
$children%
	example/eigen_det.cpp
%$$

$head Purpose$$
Enables the use of the 
$href%http://eigen.tuxfamily.org%eigen%$$
linear algebra package with the type $icode%AD<%Base%>%$$.

$head Example$$
THe file $cref eigen_det.cpp$$ contains an example and test of this
include file.
It returns true if it succeeds and false otherwise.

$head Include Files$$
This file $code cppad_eigen.hpp$$ requires the CppAD types to be defined.
In addition, it needs some Eigen definitions to work properly.
It assumes that the corresponding $icode Base$$ type is real; e.g.,
is not $code std::complex<double>$$.
$codep */
# include <cppad/cppad.hpp>
# include <Eigen/Core>
/* $$

$head Traits$$
$codep */
namespace Eigen {
	template <class Base> struct NumTraits< CppAD::AD<Base> >
	{	typedef CppAD::AD<Base>   Real;
		typedef CppAD::AD<Base>   NonInteger;
		typedef CppAD::AD<Base>   Nested;

		enum {
			IsComplex             = 0 ,
			IsInteger             = 0 ,
			IsSigned              = 1 ,
			RequireInitialization = 1 ,
			ReadCost              = 1 ,
			AddCost               = 2 ,
			MulCost               = 2
		};
	};
}
/* $$

$head Internal$$
Eigen using the $code internal$$ sub-namespace to access the following:
$codep */
namespace CppAD {
	namespace internal {
		// standard math functions that get used as is and have prototype
		// 	template <class Base> AD<Base> fun(const AD<Base>& x)
		using CppAD::abs;
		using CppAD::acos;
		using CppAD::asin;
		using CppAD::atan;
		using CppAD::cos;
		using CppAD::cosh;
		using CppAD::exp;
		using CppAD::log;
		using CppAD::log10;
		using CppAD::sin;
		using CppAD::sinh;
		using CppAD::sqrt;
		using CppAD::tan;
		// standard math functions that get used as is and have prototype
		//	template <class Base>
		//	AD<Base> fun(const AD<Base>& x, const AD<Base>& y)
		using CppAD::pow;
		
		// functions that return references
		template <class Base> const AD<Base>& conj(const AD<Base>& x)
		{	return x; }
		template <class Base> const AD<Base>& real(const AD<Base>& x)
		{	return x; }

		// functions that return values
		template <class Base> AD<Base> imag(const AD<Base>& x)
		{	return CppAD::AD<Base>(0.); }
		template <class Base> AD<Base> abs2(const AD<Base>& x)
		{	return x * x; }

		// machine epsilon with type of real part of x
		// (use assumption that Base is not complex)
		template <class Base> AD<Base> epsilon(const AD<Base>& x)
		{	return CppAD::numeric_limits<Base>::epsilon(); }

		// relaxed version of machine epsilon for comparison of different
		// operations that should result in the same value
		template <class Base> AD<Base> dummy_precision(const AD<Base>& x)
		{	return 100. * CppAD::numeric_limits<Base>::epsilon(); }

		// minimum normalized positive value
		template <class Base> AD<Base> lowest(void)
		{	return CppAD::numeric_limits< AD<Base> >::min(); }

		// maximum finite value
		template <class Base> AD<Base> highest(void)
		{	return CppAD::numeric_limits< AD<Base> >::max(); }

	}
}
/* $$
$end
*/
# endif
