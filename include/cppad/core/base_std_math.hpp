# ifndef CPPAD_CORE_BASE_STD_MATH_HPP
# define CPPAD_CORE_BASE_STD_MATH_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin base_std_math}
{xrst_spell
   erfc
   expm
   isnan
}

Base Type Requirements for Standard Math Functions
##################################################

Purpose
*******
These definitions are required for the user's code to use the type
``AD`` < *Base* > :

Unary Standard Math
*******************
The type *Base* must support the following functions
unary standard math functions (in the CppAD namespace):

.. csv-table::
   :widths: auto

   **Syntax**,**Result**
   *y* = ``abs`` ( *x* ),absolute value
   *y* = ``acos`` ( *x* ),inverse cosine
   *y* = ``acosh`` ( *x* ),inverse hyperbolic cosine
   *y* = ``asin`` ( *x* ),inverse sine
   *y* = ``asinh`` ( *x* ),inverse hyperbolic sin
   *y* = ``atan`` ( *x* ),inverse tangent
   *y* = ``atanh`` ( *x* ),inverse hyperbolic tangent
   *y* = ``cos`` ( *x* ),cosine
   *y* = ``cosh`` ( *x* ),hyperbolic cosine
   *y* = ``erf`` ( *x* ),error function
   *y* = ``erfc`` ( *x* ),complementary error function
   *y* = ``exp`` ( *x* ),exponential
   *y* = ``expm1`` ( *x* ),exponential of x minus one
   *y* = ``fabs`` ( *x* ),absolute value
   *y* = ``log`` ( *x* ),natural logarithm
   *y* = ``log1p`` ( *x* ),logarithm of one plus x
   *y* = ``sin`` ( *x* ),sine
   *y* = ``sinh`` ( *x* ),hyperbolic sine
   *y* = ``sqrt`` ( *x* ),square root
   *y* = ``tan`` ( *x* ),tangent

where the arguments and return value have the prototypes

| |tab| ``const`` *Base* & *x*
| |tab| *Base* *y*

For example,
:ref:`base_alloc<base_alloc.hpp@Unary Standard Math>` ,

CPPAD_STANDARD_MATH_UNARY
*************************
The macro invocation, within the CppAD namespace,

   ``CPPAD_STANDARD_MATH_UNARY`` ( *Base* , *Fun* )

defines the syntax

   *y* = ``CppAD::`` *Fun* ( *x* )

This macro uses the functions ``std::`` *Fun* which
must be defined and have the same prototype as ``CppAD::`` *Fun* .
For example,
:ref:`float<base_float.hpp@Unary Standard Math>` .

sign
****
The type *Base* must support the syntax

   *y* = ``CppAD::sign`` ( *x* )

which computes

.. math::

   y = \left\{ \begin{array}{ll}
      +1 & {\rm if} \; x > 0 \\
        0 & {\rm if} \; x = 0 \\
      -1 & {\rm if} \; x < 0
   \end{array} \right.

where *x* and *y* have the same prototype as above.
For example, see
:ref:`base_alloc<base_alloc.hpp@sign>` .
Note that, if ordered comparisons are not defined for the type *Base* ,
the ``code sign`` function should generate an assert if it is used; see
:ref:`complex invalid unary math<base_complex.hpp@Invalid Unary Math>` .

pow
***
The type *Base* must support the syntax

   *z* = ``CppAD::pow`` ( *x* , *y* )

which computes :math:`z = x^y`.
The arguments *x* and *y* have prototypes

| |tab| ``const`` *Base* & *x*
| |tab| ``const`` *Base* & *y*

and the return value *z* has prototype

   *Base* *z*

For example, see
:ref:`base_alloc<base_alloc.hpp@pow>` .

isnan
*****
If *Base* defines the ``isnan`` function,
you may also have to provide a definition in the CppAD namespace
(to avoid a function ambiguity).
For example, see
:ref:`base_complex<base_complex.hpp@isnan>` .

{xrst_end base_std_math}
-------------------------------------------------------------------------------
*/

# include <cmath>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

/*!
\file base_std_math.hpp
Defintions that aid meeting Base type requirements for standard math functions.
*/

/*!
\def CPPAD_STANDARD_MATH_UNARY(Type, Fun)
This macro defines the function
\verbatim
   y = CppAD:Fun(x)
\endverbatim
where the argument x and return value y have type Type
using the corresponding function <code>std::Fun</code>.
*/
# define CPPAD_STANDARD_MATH_UNARY(Type, Fun) \
   inline Type Fun(const Type& x)            \
   {  return std::Fun(x); }

} // END_CPPAD_NAMESPACE

# endif
