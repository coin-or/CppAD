// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin lu_vec_ad.cpp}
{xrst_spell
   logdet
   rhs
   signdet
}

Lu Factor and Solve with Recorded Pivoting
##########################################

Syntax
******

| ``int lu_vec_ad`` (
| |tab| ``size_t`` *n* ,
| |tab| ``size_t`` *m* ,
| |tab| ``VecAD`` < *double* > & *Matrix* ,
| |tab| ``VecAD`` < *double* > & *Rhs* ,
| |tab| ``VecAD`` < *double* > & *Result* ,
| |tab| *AD* < ``double`` > & ``logdet`` )

Purpose
*******
Solves the linear equation

.. math::

   Matrix * Result = Rhs

where *Matrix* is an :math:`n \times n` matrix,
*Rhs* is an :math:`n x m` matrix, and
*Result* is an :math:`n x m` matrix.

The routine :ref:`LuSolve-name` uses an arbitrary vector type,
instead of :ref:`VecAD-name` ,
to hold its elements.
The pivoting operations for a ``ADFun`` object
corresponding to an ``lu_vec_ad`` solution
will change to be optimal for the matrix being factored.

It is often the case that
``LuSolve`` is faster than ``lu_vec_ad`` when ``LuSolve``
uses a simple vector class with
:ref:`elements of type double<SimpleVector@Elements of Specified Type>` ,
but the corresponding :ref:`ADFun-name` objects have a fixed
set of pivoting operations.

Storage Convention
******************
The matrices stored in row major order.
To be specific, if :math:`A` contains the vector storage for an
:math:`n x m` matrix,
:math:`i` is between zero and :math:`n-1`,
and :math:`j` is between zero and :math:`m-1`,

.. math::

   A_{i,j} = A[ i * m + j ]

(The length of :math:`A` must be equal to :math:`n * m`.)

n
*
is the number of rows in
*Matrix* ,
*Rhs* ,
and *Result* .

m
*
is the number of columns in
*Rhs*
and *Result* .
It is ok for *m* to be zero which is reasonable when
you are only interested in the determinant of *Matrix* .

Matrix
******
On input, this is an
:math:`n \times n` matrix containing the variable coefficients for
the equation we wish to solve.
On output, the elements of *Matrix* have been overwritten
and are not specified.

Rhs
***
On input, this is an
:math:`n \times m` matrix containing the right hand side
for the equation we wish to solve.
On output, the elements of *Rhs* have been overwritten
and are not specified.
If *m* is zero, *Rhs* is not used.

Result
******
On input, this is an
:math:`n \times m` matrix and the value of its elements do not matter.
On output, the elements of *Rhs* contain the solution
of the equation we wish to solve
(unless the value returned by ``lu_vec_ad`` is equal to zero).
If *m* is zero, *Result* is not used.

logdet
******
On input, the value of *logdet* does not matter.
On output, it has been set to the
log of the determinant of *Matrix* (but not quite).
To be more specific,
if *signdet* is the value returned by ``lu_vec_ad`` ,
the determinant of *Matrix* is given by the formula

.. math::

   det = signdet \exp( logdet )

This enables ``lu_vec_ad`` to use logs of absolute values.

Example
*******
{xrst_toc_hidden
   example/general/lu_vec_ad_ok.cpp
}
The file
:ref:`lu_vec_ad_ok.cpp-name`
contains an example and test of ``lu_vec_ad`` .

{xrst_end lu_vec_ad.cpp}
------------------------------------------------------------------------------
*/

# include "lu_vec_ad.hpp"
# include <cppad/cppad.hpp>

// BEGIN CppAD namespace
namespace CppAD {

AD<double> lu_vec_ad(
   size_t                           n,
   size_t                           m,
   CppAD::VecAD<double>             &Matrix,
   CppAD::VecAD<double>             &Rhs,
   CppAD::VecAD<double>             &Result,
   CppAD::AD<double>                &logdet)
{
   using namespace CppAD;
   typedef AD<double> Type;

   // temporary index
   Type index;
   Type jndex;

   // index and maximum element value
   Type   imax;
   Type   jmax;
   Type   itmp;
   Type   jtmp;
   Type   emax;

   // some temporary indices
   Type i;
   Type j;
   Type k;

   // count pivots
   Type p;

   // sign of the determinant
   Type  signdet;

   // temporary values
   Type  etmp;
   Type  diff;

   // pivot element
   Type  pivot;

   // singular matrix
   Type  singular = 0.;

   // some constants
   Type M(m);
   Type N(n);
   Type One(1);
   Type Zero(0);

   // pivot row and column order in the matrix
   VecAD<double> ip(n);
   VecAD<double> jp(n);

   // -------------------------------------------------------

   // initialize row and column order in matrix not yet pivoted
   for(i = 0; i < N; i += 1.)
   {  ip[i] = i;
      jp[i] = i;
   }

   // initialize the log determinant
   logdet  = 0.;
   signdet = 1;

   for(p = 0; p < N; p += 1.)
   {
      // determine row and column corresponding to element of
      // maximum absolute value in remaining part of Matrix
      imax = N;
      jmax = N;
      emax = 0.;
      for(i = p; i < N; i += 1.)
      {  itmp = ip[i] * N;
         for(j = p; j < N; j += 1.)
         {  assert(
               (ip[i] < N) & (jp[j] < N)
            );
            index = itmp + jp[j];
            etmp  = Matrix[ index ];

            // compute absolute value of element
            etmp = fabs(etmp);

            // update maximum absolute value so far
            emax = CondExpGe(etmp, emax, etmp, emax);
            imax = CondExpGe(etmp, emax,    i, imax);
            jmax = CondExpGe(etmp, emax,    j, jmax);
         }
      }
      assert( (imax < N) & (jmax < N) );


      // switch rows so max absolute element is in row p
      index    = ip[p];
      ip[p]    = ip[imax];
      ip[imax] = index;

      // if imax != p, switch sign of determinant
      signdet  = CondExpEq(imax, p, signdet,  -signdet);

      // switch columns so max absolute element is in column p
      jndex    = jp[p];
      jp[p]    = jp[jmax];
      jp[jmax] = jndex;

      // if imax != p, switch sign of determinant
      signdet  = CondExpEq(jmax, p, signdet,  -signdet);

      // pivot using the max absolute element
      itmp    = ip[p] * N;
      index   = itmp + jp[p];
      pivot   = Matrix[ index ];

      // update the singular matrix flag
      singular = CondExpEq(pivot, Zero, One, singular);

      // update the log of absolute determinant and its sign
      etmp     = fabs(pivot);
      logdet   = logdet + log( etmp );
      signdet  = CondExpGe(pivot, Zero, signdet, - signdet);

      /*
      Reduce by the elementary transformations that maps
      Matrix( ip[p], jp[p] ) to one and Matrix( ip[i], jp[p] )
      to zero for i = p + 1., ... , n-1
      */

      // divide row number ip[p] by pivot element
      for(j = p + 1.; j < N; j += 1.)
      {
         index           = itmp + jp[j];
         Matrix[ index ] = Matrix[ index ] / pivot;
      }

      // not used anymore so no need to set to 1
      // Matrix[ ip[p] * N + jp[p] ] = Type(1);

      // divide corresponding row of right hand side by pivot element
      itmp = ip[p] * M;
      for(k = 0; k < M; k += 1.)
      {
         index = itmp + k;
         Rhs[ index ] = Rhs[ index ] / pivot;
      }

      for(i = p + 1.; i < N; i += 1. )
      {  itmp  = ip[i] * N;
         jtmp  = ip[p] * N;

         index = itmp + jp[p];
         etmp  = Matrix[ index ];

         for(j = p + 1.; j < N; j += 1.)
         {  index = itmp + jp[j];
            jndex = jtmp + jp[j];
            Matrix[ index ] = Matrix[ index ]
                     - etmp * Matrix[ jndex ];
         }

         itmp = ip[i] * M;
         jtmp = ip[p] * M;
         for(k = 0; k < M; k += 1.)
         {
            index = itmp + k;
            jndex = jtmp + k;
            Rhs[ index ] = Rhs[ index ]
                             - etmp * Rhs[ jndex ];
         }

         // not used any more so no need to set to zero
         // Matrix[ ip[i] * N + jp[p] ] = 0.;
      }

   }

   // loop over equations
   for(k = 0; k < M; k += 1.)
   {  // loop over variables
      p = N;
      while( p > 0. )
      {  p -= 1.;
         index = ip[p] * M + k;
         jndex = jp[p] * M + k;
         etmp            = Rhs[ index ];
         Result[ jndex ] = etmp;
         for(i = 0; i < p; i += 1. )
         {
            index = ip[i] * M + k;
            jndex = ip[i] * N + jp[p];
            Rhs[ index ] = Rhs[ index ]
                             - etmp * Matrix[ jndex ];
         }
      }
   }

   // make sure return zero in the singular case
   return (1. - singular) * signdet;
}

} // END CppAD namespace
