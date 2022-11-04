# ifndef CPPAD_CORE_USER_AD_HPP
# define CPPAD_CORE_USER_AD_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
---------------------------------------------------------------------------

$begin AD$$
$spell
   std
   bool
   cos
   Cpp
$$

$section AD Objects$$


$head Purpose$$
The sections listed below describe the operations
that are available to $cref/AD of Base/glossary/AD of Base/$$ objects.
These objects are used to $cref/tape/glossary/Tape/$$
an AD of $icode Base$$
$cref/operation sequence/glossary/Operation/Sequence/$$.
This operation sequence can
be transferred to an $cref ADFun$$ object where it
can be used to evaluate the corresponding
function and derivative values.

$head Base Type Requirements$$
The $icode Base$$ requirements are provided by the CppAD package
for the following base types:
$code float$$,
$code double$$,
$code std::complex<float>$$,
$code std::complex<double>$$.
Otherwise, see $cref base_require$$.


$childtable%
   include/cppad/core/ad_ctor.hpp%
   include/cppad/core/ad_assign.hpp%
   include/cppad/core/convert.hpp%
   include/cppad/core/ad_valued.hpp%
   include/cppad/core/bool_valued.hpp%
   include/cppad/core/vec_ad/user.omh%
   include/cppad/base_require.hpp
%$$

$end
---------------------------------------------------------------------------
*/

# include <cppad/core/ad_ctor.hpp>
# include <cppad/core/ad_assign.hpp>
# include <cppad/core/convert.hpp>
# include <cppad/core/vec_ad/vec_ad.hpp>
# include <cppad/core/ad_valued.hpp>
# include <cppad/core/bool_valued.hpp>
# include <cppad/core/zdouble.hpp>

# endif
