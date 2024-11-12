# ifndef CPPAD_UTILITY_LU_FACTOR_HPP
# define CPPAD_UTILITY_LU_FACTOR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin LuFactor}
{xrst_spell
   geq
   ip
   jp
}

LU Factorization of A Square Matrix
###################################

Syntax
******
| # ``include <cppad/utility/lu_factor.hpp>``
| *sign* = ``LuFactor`` ( *ip* , *jp* , *LU* )

Description
***********
Computes an LU factorization of the matrix *A*
where *A* is a square matrix.

Include
*******
The file ``cppad/utility/lu_factor.hpp``
is included by ``cppad/cppad.hpp``
but it can also be included separately with out the rest of
the ``CppAD`` routines.

Matrix Storage
**************
All matrices are stored in row major order.
To be specific, if :math:`Y` is a vector
that contains a :math:`p` by :math:`q` matrix,
the size of :math:`Y` must be equal to :math:`p * q` and for
:math:`i = 0 , \ldots , p-1`,
:math:`j = 0 , \ldots , q-1`,

.. math::

   Y_{i,j} = Y[ i * q + j ]

sign
****
The return value *sign* has prototype

   ``int`` *sign*

If *A* is invertible, *sign* is plus or minus one
and is the sign of the permutation corresponding to the row ordering
*ip* and column ordering *jp* .
If *A* is not invertible, *sign* is zero.

ip
**
The argument *ip* has prototype

   *SizeVector* & *ip*

(see description of :ref:`LuFactor@SizeVector` below).
The size of *ip* is referred to as *n* in the
specifications below.
The input value of the elements of *ip* does not matter.
The output value of the elements of *ip* determine
the order of the rows in the permuted matrix.

jp
**
The argument *jp* has prototype

   *SizeVector* & *jp*

(see description of :ref:`LuFactor@SizeVector` below).
The size of *jp* must be equal to *n* .
The input value of the elements of *jp* does not matter.
The output value of the elements of *jp* determine
the order of the columns in the permuted matrix.

LU
**
The argument *LU* has the prototype

   *FloatVector* & *LU*

and the size of *LU* must equal :math:`n * n`
(see description of :ref:`LuFactor@FloatVector` below).

A
=
We define *A* as the matrix corresponding to the input
value of *LU* .

P
=
We define the permuted matrix *P* in terms of *A* by

   *P* ( *i* , *j* ) = *A* [ *ip* [ *i* ] * *n* + *jp* [ *j* ] ]

L
=
We define the lower triangular matrix *L* in terms of the
output value of *LU* .
The matrix *L* is zero above the diagonal
and the rest of the elements are defined by

   *L* ( *i* , *j* ) = *LU* [ *ip* [ *i* ] * *n* + *jp* [ *j* ] ]

for :math:`i = 0 , \ldots , n-1` and :math:`j = 0 , \ldots , i`.

U
=
We define the upper triangular matrix *U* in terms of the
output value of *LU* .
The matrix *U* is zero below the diagonal,
one on the diagonal,
and the rest of the elements are defined by

   *U* ( *i* , *j* ) = *LU* [ *ip* [ *i* ] * *n* + *jp* [ *j* ] ]

for :math:`i = 0 , \ldots , n-2` and :math:`j = i+1 , \ldots , n-1`.

Factor
======
If the return value *sign* is non-zero,

   *L* * *U* = *P*

If the return value of *sign* is zero,
the contents of *L* and *U* are not defined.

Determinant
===========
If the return value *sign* is zero,
the determinant of *A* is zero.
If *sign* is non-zero,
using the output value of *LU*
the determinant of the matrix *A* is equal to

   *sign* * *LU* [ *ip* [0], *jp* [0]] * ... * *LU* [ *ip* [ *n* ``-1`` ], *jp* [ *n* ``-1`` ]]

SizeVector
**********
The type *SizeVector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type size_t<SimpleVector@Elements of Specified Type>` .
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.

FloatVector
***********
The type *FloatVector* must be a
:ref:`simple vector class<SimpleVector-name>` .
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.

Float
*****
This notation is used to denote the type corresponding
to the elements of a *FloatVector* .
The type *Float* must satisfy the conditions
for a :ref:`NumericType-name` .
The routine :ref:`CheckNumericType-name` will generate an error message
if this is not the case.
In addition, the following operations must be defined for any pair
of *Float* objects *x* and *y* :

.. list-table::
   :widths: auto

   * - **Operation**
     - **Description**
   * - ``log`` ( *x* )
     - returns the logarithm of *x* as a *Float* object

AbsGeq
******
Including the file ``lu_factor.hpp`` defines the template function

| |tab| ``template <class`` *Float* >
| |tab| ``bool AbsGeq`` < *Float* >( ``const`` *Float* & *x* , ``const`` *Float* & *y* )

in the ``CppAD`` namespace.
This function returns true if the absolute value of
*x* is greater than or equal the absolute value of *y* .
It is used by ``LuFactor`` to choose the pivot elements.
This template function definition uses the operator
``<=`` to obtain the absolute value for *Float* objects.
If this operator is not defined for your use of *Float* ,
you will need to specialize this template so that it works for your
use of ``LuFactor`` .

Complex numbers do not have the operation ``<=`` defined.
The specializations

| ``bool AbsGeq< std::complex<float> >``
| |tab| ( ``const std::complex<float> &`` *x* , ``const std::complex<float> &`` *y* )
| ``bool AbsGeq< std::complex<double> >``
| |tab| ( ``const std::complex<double> &`` *x* , ``const std::complex<double> &`` *y* )

are define by including ``lu_factor.hpp``
These return true if the sum of the square of the real and imaginary parts
of *x* is greater than or equal the
sum of the square of the real and imaginary parts of *y* .
{xrst_toc_hidden
   example/utility/lu_factor.cpp
   xrst/lu_factor_hpp.xrst
}
Example
*******
The file
:ref:`lu_factor.cpp-name`
contains an example and test of using ``LuFactor`` by itself.

The file :ref:`lu_solve.hpp-name` provides a useful example usage of
``LuFactor`` with ``LuInvert`` .

Source
******
The file :ref:`lu_factor.hpp-name` contains the
current source code that implements these specifications.

{xrst_end LuFactor}
--------------------------------------------------------------------------
*/
// BEGIN C++

# include <complex>
# include <vector>

# include <cppad/core/cppad_assert.hpp>
# include <cppad/utility/check_simple_vector.hpp>
# include <cppad/utility/check_numeric_type.hpp>

namespace CppAD { // BEGIN CppAD namespace

// AbsGeq
template <class Float>
bool AbsGeq(const Float &x, const Float &y)
{  Float xabs = x;
   if( xabs <= Float(0) )
      xabs = - xabs;
   Float yabs = y;
   if( yabs <= Float(0) )
      yabs = - yabs;
   return xabs >= yabs;
}
inline bool AbsGeq(
   const std::complex<double> &x,
   const std::complex<double> &y)
{  double xsq = x.real() * x.real() + x.imag() * x.imag();
   double ysq = y.real() * y.real() + y.imag() * y.imag();

   return xsq >= ysq;
}
inline bool AbsGeq(
   const std::complex<float> &x,
   const std::complex<float> &y)
{  float xsq = x.real() * x.real() + x.imag() * x.imag();
   float ysq = y.real() * y.real() + y.imag() * y.imag();

   return xsq >= ysq;
}

// Lines that are different from code in cppad/core/lu_ratio.hpp end with //
template <class SizeVector, class FloatVector>                          //
int LuFactor(SizeVector &ip, SizeVector &jp, FloatVector &LU)           //
{
   // type of the elements of LU                                   //
   typedef typename FloatVector::value_type Float;                 //

   // check numeric type specifications
   CheckNumericType<Float>();

   // check simple vector class specifications
   CheckSimpleVector<Float, FloatVector>();
   CheckSimpleVector<size_t, SizeVector>();

   size_t  i, j;          // some temporary indices
   const Float zero( 0 ); // the value zero as a Float object
   size_t  imax;          // row index of maximum element
   size_t  jmax;          // column indx of maximum element
   Float    emax;         // maximum absolute value
   size_t  p;             // count pivots
   int     sign;          // sign of the permutation
   Float   etmp;          // temporary element
   Float   pivot;         // pivot element

   // -------------------------------------------------------
   size_t n = ip.size();
   CPPAD_ASSERT_KNOWN(
      size_t(jp.size()) == n,
      "Error in LuFactor: jp must have size equal to n"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(LU.size()) == n * n,
      "Error in LuFactor: LU must have size equal to n * m"
   );
   // -------------------------------------------------------

   // initialize row and column order in matrix not yet pivoted
   for(i = 0; i < n; i++)
   {  ip[i] = i;
      jp[i] = i;
   }
   // initialize the sign of the permutation
   sign = 1;
   // ---------------------------------------------------------

   // Reduce the matrix P to L * U using n pivots
   for(p = 0; p < n; p++)
   {  // determine row and column corresponding to element of
      // maximum absolute value in remaining part of P
      imax = jmax = n;
      emax = zero;
      for(i = p; i < n; i++)
      {  for(j = p; j < n; j++)
         {  CPPAD_ASSERT_UNKNOWN(
               (ip[i] < n) && (jp[j] < n)
            );
            etmp = LU[ ip[i] * n + jp[j] ];

            // check if maximum absolute value so far
            if( AbsGeq (etmp, emax) )
            {  imax = i;
               jmax = j;
               emax = etmp;
            }
         }
      }
      CPPAD_ASSERT_KNOWN(
      (imax < n) && (jmax < n) ,
      "LuFactor can't determine an element with "
      "maximum absolute value.\n"
      "Perhaps original matrix contains not a number or infinity.\n"
      "Perhaps your specialization of AbsGeq is not correct."
      );
      if( imax != p )
      {  // switch rows so max absolute element is in row p
         i        = ip[p];
         ip[p]    = ip[imax];
         ip[imax] = i;
         sign     = -sign;
      }
      if( jmax != p )
      {  // switch columns so max absolute element is in column p
         j        = jp[p];
         jp[p]    = jp[jmax];
         jp[jmax] = j;
         sign     = -sign;
      }
      // pivot using the max absolute element
      pivot   = LU[ ip[p] * n + jp[p] ];

      // check for determinant equal to zero
      if( pivot == zero )
      {  // abort the mission
         return   0;
      }

      // Reduce U by the elementary transformations that maps
      // LU( ip[p], jp[p] ) to one.  Only need transform elements
      // above the diagonal in U and LU( ip[p] , jp[p] ) is
      // corresponding value below diagonal in L.
      for(j = p+1; j < n; j++)
         LU[ ip[p] * n + jp[j] ] /= pivot;

      // Reduce U by the elementary transformations that maps
      // LU( ip[i], jp[p] ) to zero. Only need transform elements
      // above the diagonal in U and LU( ip[i], jp[p] ) is
      // corresponding value below diagonal in L.
      for(i = p+1; i < n; i++ )
      {  etmp = LU[ ip[i] * n + jp[p] ];
         for(j = p+1; j < n; j++)
         {  LU[ ip[i] * n + jp[j] ] -=
               etmp * LU[ ip[p] * n + jp[j] ];
         }
      }
   }
   return sign;
}
} // END CppAD namespace
// END C++
# endif
