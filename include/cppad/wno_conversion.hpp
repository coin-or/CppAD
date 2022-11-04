# ifndef CPPAD_WNO_CONVERSION_HPP
# define CPPAD_WNO_CONVERSION_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
$begin wno_conversion$$
$spell
    cppad
    wno
    cpp
    hpp
$$

$section Suppress Suspect Implicit Conversion Warnings$$

$head Syntax$$
$codei%# include <cppad/wno_conversion.hpp>%$$

$head Purpose$$
In many cases it is good to have warnings for implicit conversions
that may loose range or precision.
The include command above, before any other includes, suppresses
these warning for a particular compilation unit (which usually corresponds
to a $icode%*%.cpp%$$ file).

$end
*/

# include <cppad/configure.hpp>
# if CPPAD_COMPILER_HAS_CONVERSION_WARN
# pragma GCC diagnostic ignored "-Wfloat-conversion"
# pragma GCC diagnostic ignored "-Wconversion"
# endif

# endif
