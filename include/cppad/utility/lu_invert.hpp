# ifndef CPPAD_UTILITY_LU_INVERT_HPP
# define CPPAD_UTILITY_LU_INVERT_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin LuInvert}
{xrst_spell
   ip
   jp
}

Invert an LU Factored Equation
##############################

Syntax
******
| # ``include <cppad/utility/lu_invert.hpp>``
| ``LuInvert`` ( *ip* , *jp* , *LU* , *X* )

Description
***********
Solves the matrix equation *A* * *X* = *B*
using an LU factorization computed by :ref:`LuFactor-name` .

Include
*******
The file ``cppad/utility/lu_invert.hpp``
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

ip
**
The argument *ip* has prototype

   ``const`` *SizeVector* & *ip*

(see description for *SizeVector* in
:ref:`LuFactor<LuFactor@SizeVector>` specifications).
The size of *ip* is referred to as *n* in the
specifications below.
The elements of *ip* determine
the order of the rows in the permuted matrix.

jp
**
The argument *jp* has prototype

   ``const`` *SizeVector* & *jp*

(see description for *SizeVector* in
:ref:`LuFactor<LuFactor@SizeVector>` specifications).
The size of *jp* must be equal to *n* .
The elements of *jp* determine
the order of the columns in the permuted matrix.

LU
**
The argument *LU* has the prototype

   ``const`` *FloatVector* & *LU*

and the size of *LU* must equal :math:`n * n`
(see description for *FloatVector* in
:ref:`LuFactor<LuFactor@FloatVector>` specifications).

L
=
We define the lower triangular matrix *L* in terms of *LU* .
The matrix *L* is zero above the diagonal
and the rest of the elements are defined by

   *L* ( *i* , *j* ) = *LU* [ *ip* [ *i* ] * *n* + *jp* [ *j* ] ]

for :math:`i = 0 , \ldots , n-1` and :math:`j = 0 , \ldots , i`.

U
=
We define the upper triangular matrix *U* in terms of *LU* .
The matrix *U* is zero below the diagonal,
one on the diagonal,
and the rest of the elements are defined by

   *U* ( *i* , *j* ) = *LU* [ *ip* [ *i* ] * *n* + *jp* [ *j* ] ]

for :math:`i = 0 , \ldots , n-2` and :math:`j = i+1 , \ldots , n-1`.

P
=
We define the permuted matrix *P* in terms of
the matrix *L* and the matrix *U*
by *P* = *L* * *U* .

A
=
The matrix *A* ,
which defines the linear equations that we are solving, is given by

   *P* ( *i* , *j* ) = *A* [ *ip* [ *i* ] * *n* + *jp* [ *j* ] ]

(Hence
*LU* contains a permuted factorization of the matrix *A* .)

X
*
The argument *X* has prototype

   *FloatVector* & *X*

(see description for *FloatVector* in
:ref:`LuFactor<LuFactor@FloatVector>` specifications).
The matrix *X*
must have the same number of rows as the matrix *A* .
The input value of *X* is the matrix *B* and the
output value solves the matrix equation *A* * *X* = *B* .
{xrst_toc_hidden
   example/utility/lu_invert.cpp
   xrst/lu_invert_hpp.xrst
}
Example
*******
The file :ref:`lu_solve.hpp-name` is a good example usage of
``LuFactor`` with ``LuInvert`` .
The file
:ref:`lu_invert.cpp-name`
contains an example and test of using ``LuInvert`` by itself.

Source
******
The file :ref:`lu_invert.hpp-name` contains the
current source code that implements these specifications.

{xrst_end LuInvert}
--------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/core/cppad_assert.hpp>
# include <cppad/utility/check_simple_vector.hpp>
# include <cppad/utility/check_numeric_type.hpp>

namespace CppAD { // BEGIN CppAD namespace

// LuInvert
template <class SizeVector, class FloatVector>
void LuInvert(
   const SizeVector  &ip,
   const SizeVector  &jp,
   const FloatVector &LU,
   FloatVector       &B )
{  size_t k; // column index in X
   size_t p; // index along diagonal in LU
   size_t i; // row index in LU and X

   typedef typename FloatVector::value_type Float;

   // check numeric type specifications
   CheckNumericType<Float>();

   // check simple vector class specifications
   CheckSimpleVector<Float, FloatVector>();
   CheckSimpleVector<size_t, SizeVector>();

   Float etmp;

   size_t n = ip.size();
   CPPAD_ASSERT_KNOWN(
      size_t(jp.size()) == n,
      "Error in LuInvert: jp must have size equal to n * n"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(LU.size()) == n * n,
      "Error in LuInvert: Lu must have size equal to n * m"
   );
   size_t m = size_t(B.size()) / n;
   CPPAD_ASSERT_KNOWN(
      size_t(B.size()) == n * m,
      "Error in LuSolve: B must have size equal to a multiple of n"
   );

   // temporary storage for reordered solution
   FloatVector x(n);

   // loop over equations
   for(k = 0; k < m; k++)
   {  // invert the equation c = L * b
      for(p = 0; p < n; p++)
      {  // solve for c[p]
         etmp = B[ ip[p] * m + k ] / LU[ ip[p] * n + jp[p] ];
         B[ ip[p] * m + k ] = etmp;
         // subtract off effect on other variables
         for(i = p+1; i < n; i++)
            B[ ip[i] * m + k ] -=
               etmp * LU[ ip[i] * n + jp[p] ];
      }

      // invert the equation x = U * c
      p = n;
      while( p > 0 )
      {  --p;
         etmp       = B[ ip[p] * m + k ];
         x[ jp[p] ] = etmp;
         for(i = 0; i < p; i++ )
            B[ ip[i] * m + k ] -=
               etmp * LU[ ip[i] * n + jp[p] ];
      }

      // copy reordered solution into B
      for(i = 0; i < n; i++)
         B[i * m + k] = x[i];
   }
   return;
}
} // END CppAD namespace
// END C++
# endif
