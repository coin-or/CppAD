// $Id$
# ifndef CPPAD_TO_STRING_HPP
# define CPPAD_TO_STRING_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin to_string$$
$spell
	std
	const
	ostringstream
$$

$section Convert An AD or Base Type to String$$

$head Syntax$$
$icode%s% = to_string(%value%)%$$.

$head Purpose$$
This routine is similar to the C++11 routine $code std::to_string$$
except that it works for AD and Base types and the format may be different.

$head value$$
The argument $icode value$$ has prototype
$codei%
	const %Type%& %value%
%$$
where $icode value$$ is the value being converted to a string.
The corresponding $icode Type$$ must support the operation
$codei%
	%os% << %value%
%$$
where $icode os$$ in a $code std::ostringstream$$.

$head s$$
The return value has prototype
$codei%
	std::string %s%
%$$
and contains a representation of the specified $icode value$$.

$head Format$$

$head Floating Point Types$$
If $icode Type$$ is
$code float$$,
$code double$$,
$code std::complex<float>$$, or
$code std::complex<double>$$,
enough digits are used in the representation so that
the result is accurate to withing round off error.

$head AD Types$$
If $icode Type$$ is $codei%AD<%Base%>%$$,
the representation used is the same as for $icode Base$$.

$children%
	example/to_string.cpp
%$$
$head Example$$
The file $cref to_string.cpp$$
contains an example and test of this routine.
It returns true if it succeeds and false otherwise.

$end
*/

# include <cppad/local/ad.hpp>

namespace CppAD {

	// Default implementation is in base_to_string.hpp.
	// Partial specialzation for AD<Base> types
	template<class Base>
	struct to_string_struct< CppAD::AD<Base> >
	{	std::string operator()(const CppAD::AD<Base>& value)
		{	to_string_struct<Base> ts;
			return ts( Value( Var2Par( value ) ) ); }
	};

	// link from function to function object in structure
	template<class Type>
	std::string to_string(const Type& value)
	{	to_string_struct<Type> to_s;
		return to_s(value);
	}
}

# endif
