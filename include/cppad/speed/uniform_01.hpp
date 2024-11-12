# ifndef CPPAD_SPEED_UNIFORM_01_HPP
# define CPPAD_SPEED_UNIFORM_01_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin uniform_01}

Simulate a [0,1] Uniform Random Variate
#######################################

Syntax
******
| # ``include <cppad/speed/uniform_01.hpp>``
| ``uniform_01`` ( *seed* )
| ``uniform_01`` ( *n* , *x* )

Purpose
*******
This routine is used to create random values for speed testing purposes.

Inclusion
*********
The template function ``uniform_01`` is defined in the ``CppAD``
namespace by including
the file ``cppad/speed/uniform_01.hpp``
(relative to the CppAD distribution directory).

seed
****
The argument *seed* has prototype

   ``size_t`` *seed*

It specifies a seed
for the uniform random number generator.

n
*
The argument *n* has prototype

   ``size_t`` *n*

It specifies the number of elements in the random vector *x* .

x
*
The argument *x* has prototype

   *Vector* & *x*

.
The input value of the elements of *x* does not matter.
Upon return, the elements of *x* are set to values
randomly sampled over the interval [0,1].

Vector
******
If *y* is a ``double`` value,
the object *x* must support the syntax

   *x* [ *i* ] = *y*

where *i* has type ``size_t`` with value less than
or equal :math:`n-1`.
This is the only requirement of the type *Vector* .
{xrst_toc_hidden
   xrst/uniform_01_hpp.xrst
}

Source Code
***********
The file
:ref:`uniform_01.hpp-name`
constraints the source code for this template function.

{xrst_end uniform_01}
------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cstdlib>

namespace CppAD {
   inline void uniform_01(size_t seed)
   {  std::srand( (unsigned int) seed); }

   template <class Vector>
   void uniform_01(size_t n, Vector &x)
   {  static double factor = 1. / double(RAND_MAX);
      while(n--)
         x[n] = std::rand() * factor;
   }
}
// END C++
# endif
