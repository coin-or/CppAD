// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin to_string.cpp$$

$section to_string: Example and Test$$

$code
$verbatim%example/to_string.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++

// Examples with fundamental types
# include <cppad/utility/to_string.hpp>
namespace {
	template <class Integer>
	Integer string2int(const std::string& s)
	{	Integer result = 0;
		size_t index   = 0;
		if( s[0] == '-' )
			++index;
		while( index < s.size() )
			result = 10 * result + Integer( s[index++] - '0');
		if( s[0] == '-' )
			return - result;
		return result;
	}
	template <class Integer>
	bool integer(void)
	{	bool ok = true;
		//
		Integer max    = std::numeric_limits<Integer>::max();
		std::string s  = CppAD::to_string(max);
		Integer check  = string2int<Integer>(s);
		ok            &= max == check;
		//
		Integer min       = std::numeric_limits<Integer>::min();
		s              = CppAD::to_string(min);
		check          = string2int<Integer>(s);
		ok            &= min == check;
		//
		return ok;
	}
	template <class Float>
	bool floating(void)
	{	bool  ok  = true;
		Float eps = std::numeric_limits<Float>::epsilon();
		Float pi  = 4.0 * std::atan(1.);
		//
		std::string s = CppAD::to_string( pi );
		Float check    = std::atof( s.c_str() );
		ok           &= std::fabs( check - pi ) <= 2.0 * eps;
		//
		return ok;
	}
}

// Examples with AD types
# include <cppad/cppad.hpp>
namespace {
	template <class Base>
	bool ad_floating(void)
	{	bool  ok  = true;
		Base eps = std::numeric_limits<Base>::epsilon();
		Base pi  = 4.0 * std::atan(1.);
		//
		std::string s = CppAD::to_string( CppAD::AD<Base>( pi ) );
		Base check    = std::atof( s.c_str() );
		ok           &= std::fabs( check - pi ) <= 2.0 * eps;
		//
		return ok;
	}
}

// Test driver
bool to_string(void)
{	bool ok = true;

	ok &= integer<unsigned short>();
	ok &= integer<signed int>();
	ok &= integer<unsigned long>();
# if CPPAD_USE_CPLUSPLUS_2011
	ok &= integer<signed long long>();
# endif
	//
	ok &= floating<float>();
	ok &= floating<double>();
	ok &= floating<long double>();
	//
	ok &= ad_floating<float>();
	ok &= ad_floating<double>();
	//
	return ok;
}

// END C++
