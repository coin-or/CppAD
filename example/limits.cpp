/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin limits.cpp$$
$spell
$$

$section Numeric Limits: Example and Test$$
$index limits$$
$index example, limits$$
$index test, limits$$

$code
$verbatim%example/limits.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++

# ifdef _MSC_VER
// Supress Microsoft compiler warning about possible loss of precision,
// in the constructors (when converting to std::complex<float>)
//	Type one = 1
//	Type two = 2
// 1 and 2 are small enough so no loss of precision when converting to float.
# pragma warning(disable:4244)
# endif

# include <cppad/cppad.hpp>
# include <complex>

namespace {
	template <class Type>
	Type add_one(const Type& value)
	{	return( Type(1) + value ); }
	// -----------------------------------------------------------------
	template <class Type>
	bool check_epsilon(void)
	{	bool ok    = true;
		CppAD::vector<Type> eps(1), one(1), two(1), eps2(1), check(1);
		eps[0]     = CppAD::numeric_limits<Type>::epsilon();
		one[0]     = 1;
		two[0]     = 2;
		eps2[0]    = eps[0] / two[0]; 
		check[0]   = add_one(eps[0]);
		ok        &= one[0] != check[0];
		check[0]   = add_one(eps2[0]);
		ok        &= one[0] == check[0];
		return ok;
	}
	// -----------------------------------------------------------------
	template <class Type>
	bool check_min(void)
	{	bool ok    = true;
		CppAD::vector<Type> min(1), eps(1), one(1), three(1), tmp(1), match(1);
		min[0]     = CppAD::numeric_limits<Type>::min();
		eps[0]     = CppAD::numeric_limits<Type>::epsilon();
		one[0]     = 1;
		three[0]   = 3;
		tmp[0]     = (min[0] * eps[0]) / three[0];
		match[0]   = (tmp[0] * three[0]) / eps[0];
		//        &= abs(match[0] / min[0] - one[0]) > three[0] * eps[0]
		ok        &= CppAD::abs_geq(match[0] / min[0] - one[0], three[0] * eps[0]);
		//
		tmp[0]     = (min[0] / eps[0]) * three[0];
		match[0]   = (tmp[0] / three[0]) * eps[0];
		//        &= abs(match[0] / min[0] - one[0]) < three[0] * eps[0]
		ok        &= CppAD::abs_geq(three[0] * eps[0], match[0] / min[0] - one[0]);
		return ok;
	}
	// -----------------------------------------------------------------
	template <class Type>
	bool check_max(void)
	{	bool ok    = true;
		CppAD::vector<Type> max(1), eps(1), one(1), three(1), tmp(1), match(1);
		max[0]     = CppAD::numeric_limits<Type>::max();
		eps[0]     = CppAD::numeric_limits<Type>::epsilon();
		one[0]     = 1;
		three[0]   = 3;
		tmp[0]     = max[0] *  ( one[0] / eps[0]);
		match[0]   = tmp[0] * eps[0];
		ok        &= CppAD::abs(match[0] / max[0] - one[0]) > three[0] * eps[0];
		return ok;
	}
	//
	template <class Type>
	bool check_max_complex(void)
	{	typedef std::complex<Type> Complex;
		bool ok    = true;
		CppAD::vector<Complex> c_max(1), c_eps(1);
		CppAD::vector<Type> max(1), eps(1), one(1), three(1), tmp(1), match(1);
		c_max[0]     = CppAD::numeric_limits<Complex>::max();
		c_eps[0]     = CppAD::numeric_limits<Type>::epsilon();
		ok &= c_eps[0].imag() == Type(0);
		ok &= c_max[0].imag() == Type(0);
		max[0]     = c_max[0].real();
		eps[0]     = c_eps[0].real();
		one[0]     = 1;
		three[0]   = 3;
		tmp[0]     = max[0] *  ( one[0] / eps[0]);
		match[0]   = tmp[0] * eps[0];
		ok        &= CppAD::abs(match[0] / max[0] - one[0]) > three[0] * eps[0];
		return ok;
	}
	//
	template <class Base>
	bool check_max_ad_complex()
	{	using CppAD::AD;
		bool ok    = true;
		AD<Base> c_max   = CppAD::numeric_limits< AD<Base> >::max();
		ok &= Value(c_max).imag() == Base(0);
		ok &= Value(c_max).real() == CppAD::numeric_limits<Base>::max();

		return ok;
	}
}

bool limits(void)
{	bool ok = true;
	using CppAD::AD;

	// -------------------------------------------------------------------
	// epsilon for Base types defined by CppAD
	ok &= check_epsilon<float>();
	ok &= check_epsilon<double>();
	ok &= check_epsilon< std::complex<float> >();
	ok &= check_epsilon< std::complex<double> >();

	// epsilon for some AD types. 
	ok &= check_epsilon< AD<float> >();
	ok &= check_epsilon< AD<double> >();
	ok &= check_epsilon<  AD<std::complex<float> > >();
	ok &= check_epsilon<  AD<std::complex<double> > >();

	// -------------------------------------------------------------------
	// min for Base types defined by CppAD
	ok &= check_min<float>();
	ok &= check_min<double>();
	ok &= check_min< std::complex<float> >();
	ok &= check_min< std::complex<double> >();

	// min for some AD types. 
	ok &= check_min< AD<float> >();
	ok &= check_min< AD<double> >();
	ok &= check_min<  AD<std::complex<float> > >();
	ok &= check_min<  AD<std::complex<double> > >();

	// -------------------------------------------------------------------
	// max for Base types defined by CppAD
	ok &= check_max<float>();
	ok &= check_max<double>();
	ok &= check_max_complex<float>();
	ok &= check_max_complex<double>();

	// max for some AD types. 
	ok &= check_max< AD<float> >();
	ok &= check_max< AD<double> >();
	ok &= check_max_ad_complex< std::complex<float> >();
	ok &= check_max_ad_complex< std::complex<double> >();

	return ok;
}
// END C++
