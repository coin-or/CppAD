# ifndef CPPAD_CORE_STANDARD_MATH_HPP
# define CPPAD_CORE_STANDARD_MATH_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
$begin unary_standard_math$$
$spell
   const
   VecAD
   fabs
$$

$section The Unary Standard Math Functions$$

$head Syntax$$
$icode%y% = %fun%(%x%)%$$

$head Purpose$$
Evaluates the standard math function $icode fun$$.

$head Possible Types$$

$subhead Base$$
If $icode Base$$ satisfies the
$cref/base type requirements/base_require/$$
and argument $icode x$$ has prototype
$codei%
   const %Base%& %x%
%$$
then the result $icode y$$ has prototype
$codei%
   %Base% %y%
%$$

$subhead AD<Base>$$
If the argument $icode x$$ has prototype
$codei%
   const AD<%Base%>& %x%
%$$
then the result $icode y$$ has prototype
$codei%
   AD<%Base%> %y%
%$$

$subhead VecAD<Base>$$
If the argument $icode x$$ has prototype
$codei%
   const VecAD<%Base%>::reference& %x%
%$$
then the result $icode y$$ has prototype
$codei%
   AD<%Base%> %y%
%$$

$children%include/cppad/core/std_math_11.hpp
   %include/cppad/core/abs.hpp
   %include/cppad/core/sign.hpp
%$$

$head fun$$
The possible values for $icode fun$$ are
$table
$icode  fun$$ $pre  $$ $cnext Description        $rnext
$rref abs$$
$rref acos$$
$rref acosh$$
$rref asin$$
$rref asinh$$
$rref atan$$
$rref atanh$$
$rref cos$$
$rref cosh$$
$rref erf$$
$rref exp$$
$rref expm1$$
$cref/fabs/abs/$$ $tref abs$$ $rnext
$rref log10$$
$rref log1p$$
$rref log$$
$rref sign$$
$rref sin$$
$rref sinh$$
$rref sqrt$$
$rref tan$$
$rref tanh$$
$tend

$end
*/
# include <cppad/core/abs.hpp>
# include <cppad/core/sign.hpp>

/*
$begin binary_math$$

$section The Binary Math Functions$$

$childtable%include/cppad/core/atan2.hpp
   %include/cppad/core/pow.hpp
   %include/cppad/core/azmul.hpp
%$$

$end
*/
# include <cppad/core/atan2.hpp>
# include <cppad/core/pow.hpp>

# endif
