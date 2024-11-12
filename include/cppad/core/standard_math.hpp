# ifndef CPPAD_CORE_STANDARD_MATH_HPP
# define CPPAD_CORE_STANDARD_MATH_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin unary_standard_math}
{xrst_spell
   expm
}

The Unary Standard Math Functions
#################################

Syntax
******
| *y* = *fun* ( *x* )

Purpose
*******
Evaluates the standard math function *fun* .

Possible Types
**************

Base
====
If *Base* satisfies the
:ref:`base type requirements<base_require-name>`
and argument *x* has prototype

   ``const`` *Base* & *x*

then the result *y* has prototype

   *Base* *y*

AD<Base>
========
If the argument *x* has prototype

   ``const AD`` < *Base* >& *x*

then the result *y* has prototype

   ``AD`` < *Base* > *y*

VecAD<Base>
===========
If the argument *x* has prototype

   ``const VecAD`` < *Base* >:: ``reference&`` *x*

then the result *y* has prototype

   ``AD`` < *Base* > *y*

{xrst_toc_hidden
   include/cppad/core/std_math_11.hpp
   include/cppad/core/abs.hpp
   include/cppad/core/sign.hpp
}

fun
***
The possible values for *fun* are

.. csv-table::
   :widths: auto

   *fun*,Description
   abs,:ref:`abs-title`
   acos,:ref:`acos-title`
   acosh,:ref:`acosh-title`
   asin,:ref:`asin-title`
   asinh,:ref:`asinh-title`
   atan,:ref:`atan-title`
   atanh,:ref:`atanh-title`
   cos,:ref:`cos-title`
   cosh,:ref:`cosh-title`
   erf,:ref:`erf-title`
   exp,:ref:`exp-title`
   expm1,:ref:`expm1-title`
   :ref:`fabs<abs-name>` :ref:`abs-title`
   log10,:ref:`log10-title`
   log1p,:ref:`log1p-title`
   log,:ref:`log-title`
   sign,:ref:`sign-title`
   sin,:ref:`sin-title`
   sinh,:ref:`sinh-title`
   sqrt,:ref:`sqrt-title`
   tan,:ref:`tan-title`
   tanh,:ref:`tanh-title`

{xrst_end unary_standard_math}
*/
# include <cppad/core/abs.hpp>
# include <cppad/core/sign.hpp>

/*
{xrst_begin binary_math}

The Binary Math Functions
#########################

Contents
********
{xrst_toc_table
   include/cppad/core/atan2.hpp
   include/cppad/core/pow.hpp
   include/cppad/core/azmul.hpp
}

{xrst_end binary_math}
*/
# include <cppad/core/atan2.hpp>
# include <cppad/core/pow.hpp>

# endif
