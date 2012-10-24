/* $Id$ */
# ifndef CPPAD_CPPAD_EIGEN_INCLUDED
# define CPPAD_CPPAD_EIGEN_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin cppad_eigen.hpp$$
$spell
	plugin
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
	cppad/example/eigen_plugin.hpp%
	example/eigen_array.cpp%
	example/eigen_det.cpp
%$$

$head Purpose$$
Enables the use of the 
$href%http://eigen.tuxfamily.org%eigen%$$
linear algebra package with the type $icode%AD<%Base%>%$$.

$head Example$$
THe files $cref eigen_array.cpp$$ and $cref eigen_det.cpp$$ 
contain an example and test of this include file.
It returns true if it succeeds and false otherwise.

$head eigen_plugin.hpp$$
Here is the source code for $cref eigen_plugin.hpp$$:

$head Include Files$$
This file $code cppad_eigen.hpp$$ requires the CppAD types to be defined.
The file $code eigen_plugin$$ defines $code value_type$$
in the Eigen matrix class definition so its vectors are 
$cref/simple vectors/SimpleVector/$$.
$codep */
# include <cppad/local/limits.hpp>
# define EIGEN_MATRIXBASE_PLUGIN <cppad/example/eigen_plugin.hpp>
# include <Eigen/Core>
/* $$
$head Eigen NumTraits$$
Eigen needs the following definitions to work properly
with $codei%AD<%Base%>%$$ scalars:
$codep */
namespace Eigen {
	template <class Base> struct NumTraits< CppAD::AD<Base> >
	{	// type that corresponds to the real part of an AD<Base> value
		typedef CppAD::AD<Base>   Real;
		// type for AD<Base> operations that result in non-integer values
		typedef CppAD::AD<Base>   NonInteger;
		// type for nested value inside an AD<Base> expression tree
		typedef CppAD::AD<Base>   Nested;

		enum {
			// does not support complex Base types
			IsComplex             = 0 ,
			// does not support integer Base types
			IsInteger             = 0 ,
			// only support signed Base types
			IsSigned              = 1 ,
			// must initialize an AD<Base> object
			RequireInitialization = 1 ,
			// computational cost of the corresponding operations
			ReadCost              = 1 ,
			AddCost               = 2 ,
			MulCost               = 2
		};

		// machine epsilon with type of real part of x
		// (use assumption that Base is not complex)
		static CppAD::AD<Base> epsilon(void)
		{	return CppAD::numeric_limits< CppAD::AD<Base> >::epsilon(); }

		// relaxed version of machine epsilon for comparison of different
		// operations that should result in the same value
		static CppAD::AD<Base> dummy_epsilon(void)
		{	return 100. * 
				CppAD::numeric_limits< CppAD::AD<Base> >::epsilon(); 
		}

		// minimum normalized positive value
		static CppAD::AD<Base> lowest(void)
		{	return CppAD::numeric_limits< CppAD::AD<Base> >::min(); }

		// maximum finite value
		static CppAD::AD<Base> highest(void)
		{	return CppAD::numeric_limits< CppAD::AD<Base> >::max(); }

	};
}
/* $$
$head CppAD Namespace$$
Eigen also needs the following definitions to work properly
with $codei%AD<%Base%>%$$ scalars:
$codep */
namespace CppAD {
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
}
/* $$
$end
*/
# endif
