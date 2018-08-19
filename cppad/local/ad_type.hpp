# ifndef CPPAD_LOCAL_AD_TYPE_HPP
# define CPPAD_LOCAL_AD_TYPE_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*!
\file ad_type.hpp
Define the enum type ad_type_enum
*/

/// The ad_type_enum type
typedef enum {
	no_ad_type,
	con_ad_type,
	dyn_ad_type,
	var_ad_type
}
ad_type_enum;

} } // END_CPPAD_LOCAL_NAMESPACE
# endif
