// $Id$
# ifndef CPPAD_BASE_TO_STRING_HPP
# define CPPAD_BASE_TO_STRING_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin base_to_string$$
$spell
	std
	namespace
	CppAD
	struct
	const
	stringstream
	setprecision
	str
$$

$section Base Type Requirements for to_string$$

$head Requirement$$
If the function $cref to_string$$ is used by an
$cref/AD type above Base/glossary/AD Type Above Base/$$,
A specialization for the template structure
$code CppAD::to_string_struct$$ must be defined.

$head CPPAD_TO_STRING$$
In most cases, this macro can be used to define the specialization as follows:
$codei%
	namespace CppAD {
		CPPAD_TO_STRING(%Other%, %Base%)
	}
%$$
Note that this assumes that the
$cref base_limits$$ and $cref base_std_math$$ have already been defined
for this type.
The macro is defined as follows:
$codep */
# define CPPAD_TO_STRING(Base) \
template <> struct to_string_struct<Base>\
{	std::string operator()(const Base& value) \
	{	std::stringstream os;\
		Base epsilon    = CppAD::numeric_limits<Base>::epsilon();\
		Base log10      = CppAD::log( epsilon ) / CppAD::log(Base(10.));\
		size_t n_digits = 1 - Integer( log10 );\
		os << std::setprecision(n_digits);\
		os << value;\
		return os.str();\
	}\
};
/* $$
$end
------------------------------------------------------------------------------
*/
# include <sstream>
# include <iomanip>

namespace CppAD {
	// Default implementation,
	// each Base type must define its own specilization.
	template <class Type>
	struct to_string_struct
	{	std::string operator()(const Type& value)
		{	CPPAD_ASSERT_KNOWN(
				false,
				"to_string is not implemented for this type"
			);
			// return empty string
			return std::string("");
		}
	};
}

# endif
