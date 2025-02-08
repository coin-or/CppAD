# ifndef CPPAD_UTILITY_LU_SOLVE_HPP
# define CPPAD_UTILITY_LU_SOLVE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin LuSolve}
{xrst_spell
   geq
   leq
   logdet
   signdet
}

Compute Determinant and Solve Linear Equations
##############################################

Syntax
******
| # ``include <cppad/utility/lu_solve.hpp>``
| *signdet* = ``LuSolve`` ( *n* , *m* , *A* , *B* , *X* , *logdet* )

Description
***********
Use an LU factorization of the matrix *A* to
compute its determinant
and solve for *X* in the linear of equation

.. math::

   A * X = B

where *A* is an
*n* by *n* matrix,
*X* is an
*n* by *m* matrix, and
*B* is an :math:`n x m` matrix.

Include
*******
The file ``cppad/utility/lu_solve.hpp``
is included by ``cppad/cppad.hpp``
but it can also be included separately with out the rest of
the ``CppAD`` routines.

Factor and Invert
*****************
This routine is an easy to user interface to
:ref:`LuFactor-name` and :ref:`LuInvert-name` for computing determinants and
solutions of linear equations.
These separate routines should be used if
one right hand side *B*
depends on the solution corresponding to another
right hand side (with the same value of *A* ).
In this case only one call to ``LuFactor`` is required
but there will be multiple calls to ``LuInvert`` .

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

signdet
*******
The return value *signdet* is a ``int`` value
that specifies the sign factor for the determinant of *A* .
This determinant of *A* is zero if and only if *signdet*
is zero.

n
*
The argument *n* has type ``size_t``
and specifies the number of rows in the matrices
*A* ,
*X* ,
and *B* .
The number of columns in *A* is also equal to *n* .

m
*
The argument *m* has type ``size_t``
and specifies the number of columns in the matrices
*X*
and *B* .
If *m* is zero,
only the determinant of *A* is computed and
the matrices *X* and *B* are not used.

A
*
The argument *A* has the prototype

   ``const`` *FloatVector* & *A*

and the size of *A* must equal :math:`n * n`
(see description of :ref:`LuSolve@FloatVector` below).
This is the :math:`n` by *n* matrix that
we are computing the determinant of
and that defines the linear equation.

B
*
The argument *B* has the prototype

   ``const`` *FloatVector* & *B*

and the size of *B* must equal :math:`n * m`
(see description of :ref:`LuSolve@FloatVector` below).
This is the :math:`n` by *m* matrix that
defines the right hand side of the linear equations.
If *m* is zero, *B* is not used.

X
*
The argument *X* has the prototype

   *FloatVector* & *X*

and the size of *X* must equal :math:`n * m`
(see description of :ref:`LuSolve@FloatVector` below).
The input value of *X* does not matter.
On output, the elements of *X* contain the solution
of the equation we wish to solve
(unless *signdet* is equal to zero).
If *m* is zero, *X* is not used.

logdet
******
The argument *logdet* has prototype

   *Float* & *logdet*

On input, the value of *logdet* does not matter.
On output, it has been set to the
log of the determinant of *A*
(but not quite).
To be more specific,
the determinant of *A* is given by the formula

   *det* = *signdet* * ``exp`` ( *logdet*  )

This enables ``LuSolve`` to use logs of absolute values
in the case where *Float* corresponds to a real number.

Float
*****
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

FloatVector
***********
The type *FloatVector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type Float<SimpleVector@Elements of Specified Type>` .
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.

LeqZero
*******
Including the file ``lu_solve.hpp`` defines the template function

| |tab| ``template <class`` *Float* >
| |tab| ``bool LeqZero`` < *Float* >( ``const`` *Float* & *x* )

in the ``CppAD`` namespace.
This function returns true if *x* is less than or equal to zero
and false otherwise.
It is used by ``LuSolve`` to avoid taking the log of
zero (or a negative number if *Float* corresponds to real numbers).
This template function definition assumes that the operator
``<=`` is defined for *Float* objects.
If this operator is not defined for your use of *Float* ,
you will need to specialize this template so that it works for your
use of ``LuSolve`` .

Complex numbers do not have the operation or ``<=`` defined.
In addition, in the complex case,
one can take the log of a negative number.
The specializations

| |tab| ``bool LeqZero< std::complex<float> >`` ( ``const std::complex<float> &`` *x* )
| |tab| ``bool LeqZero< std::complex<double> >`` ( ``const std::complex<double> &`` *x* )

are defined by including ``lu_solve.hpp`` .
These return true if *x* is zero and false otherwise.

AbsGeq
******
Including the file ``lu_solve.hpp`` defines the template function

| |tab| ``template <class`` *Float* >
| |tab| ``bool AbsGeq`` < *Float* >( ``const`` *Float* & *x* , ``const`` *Float* & *y* )

If the type *Float* does not support the ``<=`` operation
and it is not ``std::complex<float>`` or ``std::complex<double>`` ,
see the documentation for ``AbsGeq`` in :ref:`LuFactor<LuFactor@AbsGeq>` .
{xrst_toc_hidden
   example/utility/lu_solve.cpp
   xrst/lu_solve_hpp.xrst
}
Example
*******
The file
:ref:`lu_solve.cpp-name`
contains an example and test of using this routine.

Source
******
The file :ref:`lu_solve.hpp-name` contains the
current source code that implements these specifications.

{xrst_end LuSolve}
--------------------------------------------------------------------------
*/
// BEGIN C++
# include <complex>
# include <vector>

// link exp for float and double cases
# include <cppad/base_require.hpp>

# include <cppad/core/cppad_assert.hpp>
# include <cppad/utility/check_simple_vector.hpp>
# include <cppad/utility/check_numeric_type.hpp>
# include <cppad/utility/lu_factor.hpp>
# include <cppad/utility/lu_invert.hpp>

namespace CppAD { // BEGIN CppAD namespace

// LeqZero
template <class Float>
bool LeqZero(const Float &x)
{  return x <= Float(0); }
inline bool LeqZero( const std::complex<double> &x )
{  return x == std::complex<double>(0); }
inline bool LeqZero( const std::complex<float> &x )
{  return x == std::complex<float>(0); }

// LuSolve
template <class Float, class FloatVector>
int LuSolve(
   size_t             n      ,
   size_t             m      ,
   const FloatVector &A      ,
   const FloatVector &B      ,
   FloatVector       &X      ,
   Float        &logdet      )
{
   // check numeric type specifications
   CheckNumericType<Float>();

   // check simple vector class specifications
   CheckSimpleVector<Float, FloatVector>();

   size_t        p;       // index of pivot element (diagonal of L)
   int     signdet;       // sign of the determinant
   Float     pivot;       // pivot element

   // the value zero
   const Float zero(0);

   // pivot row and column order in the matrix
   std::vector<size_t> ip(n);
   std::vector<size_t> jp(n);

   // -------------------------------------------------------
   CPPAD_ASSERT_KNOWN(
      size_t(A.size()) == n * n,
      "Error in LuSolve: A must have size equal to n * n"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(B.size()) == n * m,
      "Error in LuSolve: B must have size equal to n * m"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(X.size()) == n * m,
      "Error in LuSolve: X must have size equal to n * m"
   );
   // -------------------------------------------------------

   // copy A so that it does not change
   FloatVector Lu(A);

   // copy B so that it does not change
   X = B;

   // Lu factor the matrix A
   signdet = LuFactor(ip, jp, Lu);

   // compute the log of the determinant
   logdet  = Float(0);
   for(p = 0; p < n; p++)
   {  // pivot using the max absolute element
      pivot   = Lu[ ip[p] * n + jp[p] ];

      // check for determinant equal to zero
      if( pivot == zero )
      {  // abort the mission
         logdet = Float(0);
         return   0;
      }

      // update the determinant
      if( LeqZero ( pivot ) )
      {  logdet += log( - pivot );
         signdet = - signdet;
      }
      else
         logdet += log( pivot );

   }

   // solve the linear equations
   LuInvert(ip, jp, Lu, X);

   // return the sign factor for the determinant
   return signdet;
}
} // END CppAD namespace
// END C++
# endif
