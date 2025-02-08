# ifndef CPPAD_CORE_LU_RATIO_HPP
# define CPPAD_CORE_LU_RATIO_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin LuRatio app}
{xrst_spell
   ip
   jp
   xk
}

LU Factorization of A Square Matrix and Stability Calculation
#############################################################

Syntax
******
| ``# include <cppad/cppad.hpp>``
| *sign* = ``LuRatio`` ( *ip* , *jp* , *LU* , *ratio* )

Description
***********
Computes an LU factorization of the matrix *A*
where *A* is a square matrix.
A measure of the numerical stability called *ratio* is calculated.
This ratio is useful when the results of ``LuRatio`` are
used as part of an :ref:`ADFun-name` object.

Include
*******
This routine is designed to be used with AD objects and
requires the ``cppad/cppad.hpp`` file to be included.

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

   *ADvector* & *LU*

and the size of *LU* must equal :math:`n * n`
(see description of :ref:`LuRatio@ADvector` below).

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

ratio
*****
The argument *ratio* has prototype

   ``AD`` < *Base* > & *ratio*

On input, the value of *ratio* does not matter.
On output it is a measure of how good the choice of pivots is.
For :math:`p = 0 , \ldots , n-1`,
the *p*-th pivot element is the element of maximum absolute value of a
:math:`(n-p) \times (n-p)` sub-matrix.
The ratio of each element of sub-matrix divided by the pivot element
is computed.
The return value of *ratio* is the maximum absolute value of
such ratios over with respect to all elements and all the pivots.

Purpose
=======
Suppose that the execution of a call to ``LuRatio``
is recorded in the ``ADFun`` < *Base* > object *F* .
Then a call to :ref:`Forward-name` of the form

   *F* . ``Forward`` ( *k* , *xk* )

with *k* equal to zero will revaluate this Lu factorization
with the same pivots and a new value for *A* .
In this case, the resulting *ratio* may not be one.
If *ratio* is too large (the meaning of too large is up to you),
the current pivots do not yield a stable LU factorization of *A* .
A better choice for the pivots (for this value of *A* )
will be made if you recreate the ``ADFun`` object
starting with the :ref:`Independent-name` variable values
that correspond to the vector *xk* .

SizeVector
**********
The type *SizeVector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type size_t<SimpleVector@Elements of Specified Type>` .
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.

ADvector
********
The type *ADvector* must be a
:ref:`simple vector class<SimpleVector-name>` with elements of type
``AD`` < *Base* > .
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.

Example
*******
{xrst_toc_hidden
   example/general/lu_ratio.cpp
}
The file :ref:`lu_ratio.cpp-name`
contains an example and test of using ``LuRatio`` .

{xrst_end LuRatio}
--------------------------------------------------------------------------
*/
namespace CppAD { // BEGIN CppAD namespace

// Lines different from the code in cppad/lu_factor.hpp end with           //
template <class SizeVector, class ADvector, class Base>                    //
int LuRatio(SizeVector &ip, SizeVector &jp, ADvector &LU, AD<Base> &ratio) //
{
   typedef ADvector FloatVector;                                       //
   typedef AD<Base>       Float;                                       //

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
   size_t n = size_t(ip.size());
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
   // initialize the ratio                                             //
   ratio = Float(1);                                                   //
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
      for(i = p; i < n; i++)                                       //
      {  for(j = p; j < n; j++)                               //
         {  etmp  = fabs(LU[ ip[i] * n + jp[j] ] / emax); //
            ratio =                                      //
            CondExpGt(etmp, ratio, etmp, ratio);         //
         }                                                    //
      }                                                            //
      CPPAD_ASSERT_KNOWN(
         (imax < n) && (jmax < n) ,
         "AbsGeq must return true when second argument is zero"
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

# endif
