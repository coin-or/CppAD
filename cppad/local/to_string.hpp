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

$section Convert Any Type to String$$

$head Syntax$$
$icode%s% = to_string(%value%)%$$.

$head Purpose$$
This routine is similar to the C++11 routine $code std::to_string$$
except that it works for more types and the format may be different.

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
If $icode Type$$ is $code float$$ or $code double$$,
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

	// general implementation
	template <class Type>
	struct to_string_struct
	{	std::string operator()(const Type& value)
		{	std::ostringstream os;
			os << value;
			return os.str();
		}
	};

	// partial specialzation for AD<Base> types
	template<class Base>
	struct to_string_struct< CppAD::AD<Base> >
	{	std::string operator()(const CppAD::AD<Base>& value)
		{	to_string_struct<Base> ts;
			return ts( Value( Var2Par( value ) ) ); }
	};

	// specialization for float
	template <>
	struct to_string_struct<float>
	{	std::string operator()(const float& value)
		{	std::stringstream os;
			float epsilon  = std::numeric_limits<float>::epsilon();
			size_t n_digits = 1 - std::log10( epsilon );
			os << std::setprecision(n_digits);
			os << value;
			return os.str();
		}
	};

	// specialization for double
	template <>
	struct to_string_struct<double>
	{	std::string operator()(const double& value)
		{	std::stringstream os;
			double epsilon  = std::numeric_limits<double>::epsilon();
			size_t n_digits = 1 - std::log10( epsilon );
			os << std::setprecision(n_digits);
			os << value;
			return os.str();
		}
	};

	template<class Type>
	std::string to_string(const Type& value)
	{	to_string_struct<Type> ts;
		return ts(value);
	}
}

# endif
