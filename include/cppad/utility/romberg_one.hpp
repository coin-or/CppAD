# ifndef CPPAD_UTILITY_ROMBERG_ONE_HPP
# define CPPAD_UTILITY_ROMBERG_ONE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin RombergOne}
{xrst_spell
   test test
}

One DimensionalRomberg Integration
##################################

Syntax
******
| # ``include <cppad/utility/romberg_one.hpp>``
| *r* = ``RombergOne`` ( *F* , *a* , *b* , *n* , *e* )

Description
***********
Returns the Romberg integration estimate
:math:`r` for a one dimensional integral

.. math::

   r = \int_a^b F(x) {\bf d} x + O \left[ (b - a) / 2^{n-1} \right]^{2(p+1)}

Include
*******
The file ``cppad/utility/romberg_one.hpp``
is included by ``cppad/cppad.hpp``
but it can also be included separately with out the rest of
the ``CppAD`` routines.

r
*
The return value *r* has prototype

   *Float* *r*

It is the estimate computed by ``RombergOne`` for the integral above.

F
*
The object *F* can be of any type, but it must support
the operation

   *F* ( *x* )

The argument *x* to *F* has prototype

   ``const`` *Float* & *x*

The return value of *F* is a *Float* object
(see description of :ref:`RombergOne@Float` below).

a
*
The argument *a* has prototype

   ``const`` *Float* & *a*

It specifies the lower limit for the integration.

b
*
The argument *b* has prototype

   ``const`` *Float* & *b*

It specifies the upper limit for the integration.

n
*
The argument *n* has prototype

   ``size_t`` *n*

A total number of :math:`2^{n-1} + 1` evaluations of *F* ( *x* )
are used to estimate the integral.

p
*
The argument *p* has prototype

   ``size_t`` *p*

It must be less than or equal :math:`n`
and determines the accuracy order in the approximation for the integral
that is returned by ``RombergOne`` .
To be specific

.. math::

   r = \int_a^b F(x) {\bf d} x + O \left[ (b - a) / 2^{n-1} \right]^{2(p+1)}

e
*
The argument *e* has prototype

   *Float* & *e*

The input value of *e* does not matter
and its output value is an approximation for the error in
the integral estimates; i.e.,

.. math::

   e \approx \left| r - \int_a^b F(x) {\bf d} x \right|

Float
*****
The type *Float* must satisfy the conditions
for a :ref:`NumericType-name` .
The routine :ref:`CheckNumericType-name` will generate an error message
if this is not the case.
In addition, if *x* and *y* are *Float* objects,

   *x* < *y*

returns the ``bool`` value true if *x* is less than
*y* and false otherwise.
{xrst_toc_hidden
   example/utility/romberg_one.cpp
}
Example
*******
The file
:ref:`romberg_one.cpp-name`
contains an example and test a test of using this routine.

Source Code
***********
The source code for this routine is in the file
``cppad/romberg_one.hpp`` .

{xrst_end RombergOne}
*/

# include <cppad/utility/check_numeric_type.hpp>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/utility/vector.hpp>

namespace CppAD { // BEGIN CppAD namespace

template <class Fun, class Float>
Float RombergOne(
   Fun           &F ,
   const Float   &a ,
   const Float   &b ,
   size_t         n ,
   size_t         p ,
   Float         &e )
{
   size_t ipow2 = 1;
   size_t k, i;
   Float pow2, sum, x;

   Float  zero  = Float(0);
   Float  two   = Float(2);

   // check specifications for a NumericType
   CheckNumericType<Float>();

   CPPAD_ASSERT_KNOWN(
      n >= 2,
      "RombergOne: n must be greater than or equal 2"
   );
   CppAD::vector<Float> r(n);

   //  set r[i] = trapazoidal rule with 2^i intervals in [a, b]
   r[0]  = ( F(a) + F(b) ) * (b - a) / two;
   for(i = 1; i < n; i++)
   {  ipow2 *= 2;
      // there must be a conversion from int to any numeric type
      pow2   = Float(int(ipow2));
      sum    = zero;
      for(k = 1; k < ipow2; k += 2)
      {  // start = a + (b-a)/pow2, increment = 2*(b-a)/pow2
         x    = ( (pow2 - Float(double(k))) * a + double(k) * b ) / pow2;
         sum  = sum + F(x);
      }
      // combine function evaluations in sum with those in T[i-1]
      r[i] = r[i-1] / two + sum * (b - a) / pow2;
   }

   // now compute the higher order estimates
   size_t ipow4    = 1;   // order of accuract for previous estimate
   Float pow4, pow4minus;
   for(i = 0; i < p; i++)
   {  // compute estimate accurate to O[ step^(2*(i+1)) ]
      // put resutls in r[n-1], r[n-2], ... , r[n-i+1]
      ipow4    *= 4;
      pow4      = Float(int(ipow4));
      pow4minus = Float(ipow4-1);
      for(k = n-1; k > i; k--)
         r[k] = ( pow4 * r[k] - r[k-1] ) / pow4minus;
   }

   // error estimate for r[n]
   e = r[n-1] - r[n-2];
   if( e < zero )
      e = - e;
   return r[n-1];
}

} // END CppAD namespace

# endif
