# ifndef CPPAD_SPEED_DET_BY_LU_HPP
# define CPPAD_SPEED_DET_BY_LU_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin det_by_lu}

Determinant Using Expansion by Lu Factorization
###############################################

Syntax
******
| # ``include <cppad/speed/det_by_lu.hpp>``
| ``det_by_lu`` < *Scalar* > *det* ( *n* )
| *d* = *det* ( *a* )

Inclusion
*********
The template class ``det_by_lu`` is defined in the ``CppAD``
namespace by including
the file ``cppad/speed/det_by_lu.hpp``
(relative to the CppAD distribution directory).

Constructor
***********
The syntax

   ``det_by_lu`` < *Scalar* > *det* ( *n* )

constructs the object *det* which can be used for
evaluating the determinant of *n* by *n* matrices
using LU factorization.

Scalar
******
The type *Scalar* can be any
:ref:`NumericType-name`

n
*
The argument *n* has prototype

   ``size_t`` *n*

det
***
The syntax

   *d* = *det* ( *a* )

returns the determinant of the matrix :math:`A` using LU factorization.

a
=
The argument *a* has prototype

   ``const`` *Vector* & *a*

It must be a *Vector* with length :math:`n * n` and with
It must be a *Vector* with length :math:`n * n` and with
elements of type *Scalar* .
The elements of the :math:`n \times n` matrix :math:`A` are defined,
for :math:`i = 0 , \ldots , n-1` and :math:`j = 0 , \ldots , n-1`, by

.. math::

   A_{i,j} = a[ i * m + j]

d
=
The return value *d* has prototype

   *Scalar* *d*

Vector
******
If *y* is a *Vector* object,
it must support the syntax

   *y* [ *i* ]

where *i* has type ``size_t`` with value less than :math:`n * n`.
This must return a *Scalar* value corresponding to the *i*-th
element of the vector *y* .
This is the only requirement of the type *Vector* .
{xrst_toc_hidden
   speed/example/det_by_lu.cpp
   xrst/det_by_lu_hpp.xrst
}

Example
*******
The file
:ref:`det_by_lu.cpp-name`
contains an example and test of ``det_by_lu.hpp`` .

Source Code
***********
The file
:ref:`det_by_lu.hpp-name`
contains the source for this template function.

{xrst_end det_by_lu}
---------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/utility/vector.hpp>
# include <cppad/utility/lu_solve.hpp>

// BEGIN CppAD namespace
namespace CppAD {

template <class Scalar>
class det_by_lu {
private:
   const size_t m_;
   const size_t n_;
   CppAD::vector<Scalar> A_;
   CppAD::vector<Scalar> B_;
   CppAD::vector<Scalar> X_;
public:
   det_by_lu(size_t n) : m_(0), n_(n), A_(n * n)
   {  }

   template <class Vector>
   Scalar operator()(const Vector &x)
   {

      Scalar       logdet;
      Scalar       det;
      int          signdet;
      size_t       i;

      // copy matrix so it is not overwritten
      for(i = 0; i < n_ * n_; i++)
         A_[i] = x[i];

      // comput log determinant
      signdet = CppAD::LuSolve(
         n_, m_, A_, B_, X_, logdet);

/*
      // Do not do this for speed test because it makes floating
      // point operation sequence very simple.
      if( signdet == 0 )
         det = 0;
      else
         det =  Scalar( signdet ) * exp( logdet );
*/

      // convert to determinant
      det     = Scalar( signdet ) * exp( logdet );

# ifdef FADBAD
      // Fadbad requires tempories to be set to constants
      for(i = 0; i < n_ * n_; i++)
         A_[i] = 0;
# endif

      return det;
   }
};
} // END CppAD namespace
// END C++
# endif
