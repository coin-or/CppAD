# ifndef CPPAD_SPEED_DET_BY_MINOR_HPP
# define CPPAD_SPEED_DET_BY_MINOR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin det_by_minor}

Determinant Using Expansion by Minors
#####################################

Syntax
******
| ``# include <cppad/speed/det_by_minor.hpp>``
| ``det_by_minor`` < *Scalar* > *det* ( *n* )
| *d* = *det* ( *a* )

Inclusion
*********
The template class ``det_by_minor`` is defined in the ``CppAD``
namespace by including the file ``cppad/speed/det_by_minor.hpp`` .

Constructor
***********
The syntax

   ``det_by_minor`` < *Scalar* > *det* ( *n* )

constructs the object *det* which can be used for
evaluating the determinant of *n* by *n* matrices
using expansion by minors.

Scalar
******
The type *Scalar* must satisfy the same conditions
as in the function :ref:`det_of_minor<det_of_minor@Scalar>` .

n
*
The argument *n* has prototype

   ``size_t`` *n*

det
***
The syntax

   *d* = *det* ( *a* )

returns the determinant of the matrix *A* using expansion by minors.

a
=
The argument *a* has prototype

   ``const`` *Vector* & *a*

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

It is equal to the determinant of :math:`A`.

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
   speed/example/det_by_minor.cpp
   xrst/det_by_minor_hpp.xrst
}

Example
*******
The file
:ref:`det_by_minor.cpp-name`
contains an example and test of ``det_by_minor.hpp`` .

Source Code
***********
The file
:ref:`det_by_minor.hpp-name`
contains the source for this template function.

{xrst_end det_by_minor}
---------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/speed/det_of_minor.hpp>
# include <vector>

// BEGIN CppAD namespace
namespace CppAD {

template <class Scalar>
class det_by_minor {
private:
   //
   // m_
   // size for the matrix
   const size_t        m_;
   //
   // r_, c_
   // row and column indices so that minor is entire matrix.
   std::vector<size_t> r_;
   std::vector<size_t> c_;
   //
   // a_
   // temporary vector declared here to avoid reallocation for each use
   std::vector<Scalar> a_;
public:
   det_by_minor(size_t m) : m_(m) , r_(m + 1) , c_(m + 1), a_(m * m)
   {  //
      // r_, c_
      // values that correspond to entire matrix
      for(size_t i = 0; i < m; i++)
      {  r_[i] = i+1;
         c_[i] = i+1;
      }
      r_[m] = 0;
      c_[m] = 0;
   }
   //
   // operator()
   template <class Vector>
   Scalar operator()(const Vector &x)
   {  //
      // a_
      // copy from type Vector to std::vector<Scalar>
      for(size_t i = 0; i < m_ * m_; ++i)
         a_[i] = x[i];
      //
      // det
      // compute determinant of entire matrix
      Scalar det = det_of_minor(a_, m_, m_, r_, c_);
      //
# ifndef NDEBUG
      // r_, c_
      // values that correspond to entire matrix
      // (not const because det_of_minor uses r_, c_ for work space)
      for(size_t i = 0; i < m_; ++i)
      {  assert( r_[i] == i + 1 );
         assert( c_[i] == i + 1 );
      }
      assert( r_[m_] == 0 );
      assert( c_[m_] == 0 );
# endif
      return det;
   }

};

} // END CppAD namespace
// END C++
# endif
