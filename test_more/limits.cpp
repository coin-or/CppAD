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

$head Assumption$$
This code assumes that the decimal point is infront of the mantissa.
Hence dividing the minimum normalized value looses precision,
while multiplying the maximum normalized value results in infinity. 

$head Externals$$
This example using external routines to get and set values
so that the complier does not set the correspdong code and optimize
it out.

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
# include "extern_value.hpp"

namespace {
	using CppAD::vector;
	using CppAD::abs_geq;

	template <class Type>
	Type add_one(const Type& value)
	{	return( Type(1) + value ); }
	// -----------------------------------------------------------------
	template <class Type>
	bool check_epsilon(void)
	{	bool ok    = true;
		typedef extern_value<Type> value;
		value eps( CppAD::numeric_limits<Type>::epsilon() );
		value one( Type(1) );
		value two( Type(2) );
		value tmp( Type(0) );
		//
		tmp.set( add_one( eps.get() / two.get() ) ); 
		ok        &= one.get() == tmp.get();
		//
		tmp.set( add_one( eps.get() ) ); 
		ok        &= one.get() != tmp.get();
		return ok;
	}
	// -----------------------------------------------------------------
	template <class Type>
	bool check_min(void)
	{	bool ok    = true;
		typedef extern_value<Type> value;
		value min( CppAD::numeric_limits<Type>::min() );
		value eps3( Type(3) * CppAD::numeric_limits<Type>::epsilon() );
		value one( Type(1) );
		value hun( Type(100) );
		value tmp( Type(0) );
		//
		tmp.set( min.get() / hun.get() );
		tmp.set( tmp.get() * hun.get() );
		ok        &= abs_geq(tmp.get()/min.get() - one.get(), eps3.get());
		//
		tmp.set( min.get() * hun.get() );
		tmp.set( tmp.get() / hun.get() );
		ok        &= ! abs_geq(tmp.get()/min.get() - one.get(), eps3.get());
		return ok;
	}

	// -----------------------------------------------------------------
	template <class Type>
	bool check_max(void)
	{	bool ok    = true;
		typedef extern_value<Type> value;
		value max2( CppAD::numeric_limits<Type>::max() / Type(2) );
		value eps3( Type(3) * CppAD::numeric_limits<Type>::epsilon() );
		value one( Type(1) );
		value hun( Type(100) );
		value tmp( Type(0) );
		//
		tmp.set( max2.get() * hun.get() );
		tmp.set( tmp.get() / hun.get() );
		// tmp is infinite
		ok        &= abs_geq(tmp.get() / max2.get() - one.get(), eps3.get() );
		//
		tmp.set( max2.get() / hun.get() );
		tmp.set( tmp.get() * hun.get() );
		ok        &= ! abs_geq(tmp.get() / max2.get() - one.get(), eps3.get() );
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
	ok &= check_max< std::complex<float> >();
	ok &= check_max< std::complex<double> >();

	// max for some AD types. 
	ok &= check_max< AD<float> >();
	ok &= check_max< AD<double> >();
	ok &= check_max< AD< std::complex<float> > >();
	ok &= check_max< AD< std::complex<double> > >();

	return ok;
}
// END C++
