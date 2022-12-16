# ifndef CPPAD_CORE_USER_AD_HPP
# define CPPAD_CORE_USER_AD_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
---------------------------------------------------------------------------

{xrst_begin AD}

AD Objects
##########

Purpose
*******
The sections listed below describe the operations
that are available to :ref:`glossary@AD of Base` objects.
These objects are used to :ref:`glossary@Tape`
an AD of *Base*
:ref:`operation sequence<glossary@Operation@Sequence>` .
This operation sequence can
be transferred to an :ref:`ADFun-name` object where it
can be used to evaluate the corresponding
function and derivative values.

Base Type Requirements
**********************
The *Base* requirements are provided by the CppAD package
for the following base types:
``float`` ,
``double`` ,
``std::complex<float>`` ,
``std::complex<double>`` .
Otherwise, see :ref:`base_require-name` .

Contents
********
{xrst_toc_table
   include/cppad/core/ad_ctor.hpp
   include/cppad/core/ad_assign.hpp
   include/cppad/core/convert.hpp
   include/cppad/core/ad_valued.hpp
   include/cppad/core/bool_valued.hpp
   include/cppad/core/vec_ad/user.xrst
   include/cppad/base_require.hpp
}

{xrst_end AD}
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
