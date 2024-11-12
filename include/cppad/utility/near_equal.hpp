# ifndef CPPAD_UTILITY_NEAR_EQUAL_HPP
# define CPPAD_UTILITY_NEAR_EQUAL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin NearEqual}
{xrst_spell
   cout
   endl
}

Determine if Two Values Are Nearly Equal
########################################

Syntax
******
| # ``include <cppad/utility/near_equal.hpp>``
| *b* = ``NearEqual`` ( *x* , *y* , *r* , *a* )

Purpose
*******
Returns true,
if *x* and *y* are nearly equal,
and false otherwise.

x
*
The argument *x*
has one of the following possible prototypes

| |tab| ``const`` *Type* & *x* ,
| |tab| ``const std::complex<`` *Type* > & *x* ,

y
*
The argument *y*
has one of the following possible prototypes

| |tab| ``const`` *Type* & *y* ,
| |tab| ``const std::complex<`` *Type* > & *y* ,

r
*
The relative error criteria *r* has prototype

   ``const`` *Type* & *r*

It must be greater than or equal to zero.
The relative error condition is defined as:

.. math::

   | x - y | \leq r ( |x| + |y| )

a
*
The absolute error criteria *a* has prototype

   ``const`` *Type* & *a*

It must be greater than or equal to zero.
The absolute error condition is defined as:

.. math::

   | x - y | \leq a

b
*
The return value *b* has prototype

   ``bool`` *b*

If either *x* or *y* is infinite or not a number,
the return value is false.
Otherwise, if either the relative or absolute error
condition (defined above) is satisfied, the return value is true.
Otherwise, the return value is false.

Type
****
The type *Type* must be a
:ref:`NumericType-name` .
The routine :ref:`CheckNumericType-name` will generate
an error message if this is not the case.
In addition, the following operations must be defined objects
*a* and *b* of type *Type* :

.. list-table::
   :widths: auto

   * - **Operation**
     - **Description**
   * - *a* <= *b*
     - less that or equal operator (returns a ``bool`` object)

Include Files
*************
The file ``cppad/utility/near_equal.hpp``
is included by ``cppad/cppad.hpp``
but it can also be included separately with out the rest of
the ``CppAD`` routines.

Example
*******
{xrst_toc_hidden
   example/utility/near_equal.cpp
}
The file :ref:`near_equal.cpp-name` contains an example
and test of ``NearEqual`` .
It return true if it succeeds and false otherwise.

Exercise
********
Create and run a program that contains the following code:
::

   using std::complex;
   using std::cout;
   using std::endl;

   complex<double> one(1., 0), i(0., 1);
   complex<double> x = one / i;
   complex<double> y = - i;
   double          r = 1e-12;
   double          a = 0;
   bool           ok = CppAD::NearEqual(x, y, r, a);
   if( ok )
   cout << "Ok"    << endl;
   else
   cout << "Error" << endl;

{xrst_end NearEqual}

*/

# include <limits>
# include <complex>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/utility/check_numeric_type.hpp>

namespace CppAD { // Begin CppAD namespace

// determine if both x and y are finite values
template <class Type>
bool near_equal_isfinite(const Type &x , const Type &y)
{  Type infinity = Type( std::numeric_limits<double>::infinity() );

   // handle bug where some compilers return true for nan == nan
   bool xNan = x != x;
   bool yNan = y != y;

   // infinite cases
   bool xInf = (x == infinity   || x == - infinity);
   bool yInf = (x == infinity   || x == - infinity);

   return ! (xNan | yNan | xInf | yInf);
}

template <class Type>
bool NearEqual(const Type &x, const Type &y, const Type &r, const Type &a)
{
   CheckNumericType<Type>();
   Type zero(0);

   CPPAD_ASSERT_KNOWN(
      zero <= r,
      "Error in NearEqual: relative error is less than zero"
   );
   CPPAD_ASSERT_KNOWN(
      zero <= a,
      "Error in NearEqual: absolute error is less than zero"
   );

   // check for special cases
   if( ! CppAD::near_equal_isfinite(x, y) )
      return false;

   Type ax = x;
   if( ax <= zero )
      ax = - ax;

   Type ay = y;
   if( ay <= zero )
      ay = - ay;

   Type ad = x - y;
   if( ad <= zero )
      ad = - ad;

   if( ad <= a )
      return true;

   if( ad <= r * (ax + ay) )
      return true;

   return false;
}

template <class Type>
bool NearEqual(
   const std::complex<Type> &x ,
   const std::complex<Type> &y ,
   const              Type  &r ,
   const              Type  & a )
{
   CheckNumericType<Type>();
# ifndef NDEBUG
   Type zero(0);
# endif

   CPPAD_ASSERT_KNOWN(
      zero <= r,
      "Error in NearEqual: relative error is less than zero"
   );
   CPPAD_ASSERT_KNOWN(
      zero <= a,
      "Error in NearEqual: absolute error is less than zero"
   );

   // check for special cases
   if( ! CppAD::near_equal_isfinite(x.real(), x.imag()) )
      return false;
   if( ! CppAD::near_equal_isfinite(y.real(), y.imag()) )
      return false;

   std::complex<Type> d = x - y;

   Type ad = std::abs(d);
   if( ad <= a )
      return true;

   Type ax = std::abs(x);
   Type ay = std::abs(y);
   if( ad <= r * (ax + ay) )
      return true;

   return false;
}

template <class Type>
bool NearEqual(
   const std::complex<Type> &x ,
   const              Type  &y ,
   const              Type  &r ,
   const              Type  & a )
{
   return NearEqual(x, std::complex<Type>(y, Type(0)), r, a);
}

template <class Type>
bool NearEqual(
   const              Type  &x ,
   const std::complex<Type> &y ,
   const              Type  &r ,
   const              Type  & a )
{
   return NearEqual(std::complex<Type>(x, Type(0)), y, r, a);
}

} // END CppAD namespace

# endif
