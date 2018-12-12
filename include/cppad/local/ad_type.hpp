# ifndef CPPAD_LOCAL_AD_TYPE_HPP
# define CPPAD_LOCAL_AD_TYPE_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*!
\file ad_type.hpp
Define the enum type ad_type_enum
*/

/// The ad_type_enum type
typedef enum {
    no_ad_type,
    dyn_ad_type,
    var_ad_type
}
ad_type_enum;

} } // END_CPPAD_LOCAL_NAMESPACE
# endif
