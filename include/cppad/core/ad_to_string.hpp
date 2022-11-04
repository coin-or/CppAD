# ifndef CPPAD_CORE_AD_TO_STRING_HPP
# define CPPAD_CORE_AD_TO_STRING_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
$begin ad_to_string$$
$spell
    const
    std
$$

$section Convert An AD or Base Type to String$$

$head Syntax$$
$icode%s% = to_string(%value%)%$$.

$head See Also$$
$cref to_string$$, $cref base_to_string$$

$head value$$
The argument $icode value$$ has prototype
$codei%
    const AD<%Base%>& %value%
    const %Base%&     %value%
%$$
where $icode Base$$ is a type that supports the
$cref base_to_string$$ type requirement.

$head s$$
The return value has prototype
$codei%
    std::string %s%
%$$
and contains a representation of the specified $icode value$$.
If $icode value$$ is an AD type,
the result has the same precision as for the $icode Base$$ type.

$head Example$$
The file $cref to_string.cpp$$
includes an example and test of $code to_string$$ with AD types.

$end
*/
# include <cppad/utility/to_string.hpp>
# include <cppad/core/ad.hpp>

namespace CppAD {

    // Template definition is in cppad/utility/to_string.hpp.
    // Partial specialzation for AD<Base> types
    template<class Base>
    struct to_string_struct< CppAD::AD<Base> >
    {   std::string operator()(const CppAD::AD<Base>& value)
        {   to_string_struct<Base> ts;
            return ts( Value( Var2Par( value ) ) ); }
    };

}

# endif
