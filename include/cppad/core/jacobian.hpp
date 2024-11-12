# ifndef CPPAD_CORE_JACOBIAN_HPP
# define CPPAD_CORE_JACOBIAN_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin Jacobian}

Jacobian: Driver Routine
########################

Syntax
******
| *jac* = *f* . ``Jacobian`` ( *x* )

Purpose
*******
We use :math:`F : \B{R}^n \rightarrow \B{R}^m` to denote the
:ref:`glossary@AD Function` corresponding to *f* .
The syntax above sets *jac* to the
Jacobian of *F* evaluated at *x* ; i.e.,

.. math::

   jac = F^{(1)} (x)

f
*
The object *f* has prototype

   ``ADFun`` < *Base* > *f*

Note that the :ref:`ADFun-name` object *f* is not ``const``
(see :ref:`Jacobian@Forward or Reverse` below).

x
*
The argument *x* has prototype

   ``const`` *Vector* & *x*

(see :ref:`Jacobian@Vector` below)
and its size
must be equal to *n* , the dimension of the
:ref:`fun_property@Domain` space for *f* .
It specifies
that point at which to evaluate the Jacobian.

jac
***
The result *jac* has prototype

   *Vector* *jac*

(see :ref:`Jacobian@Vector` below)
and its size is :math:`m * n`; i.e., the product of the
:ref:`fun_property@Domain`
and
:ref:`fun_property@Range`
dimensions for *f* .
For :math:`i = 0 , \ldots , m - 1`
and :math:`j = 0 , \ldots , n - 1`

.. math::

   jac[ i * n + j ] = \D{ F_i }{ x_j } ( x )

Vector
******
The type *Vector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
*Base* .
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.

Forward or Reverse
******************
This will use order zero Forward mode and either
order one Forward or order one Reverse to compute the Jacobian
(depending on which it estimates will require less work).
After each call to :ref:`Forward-name` ,
the object *f* contains the corresponding
:ref:`Taylor coefficients<glossary@Taylor Coefficient>` .
After a call to ``Jacobian`` ,
the zero order Taylor coefficients correspond to
*f* . ``Forward`` (0, *x* )
and the other coefficients are unspecified.

Example
*******
{xrst_toc_hidden
   example/general/jacobian.cpp
}
The routine
:ref:`Jacobian<jacobian.cpp-name>` is both an example and test.
It returns ``true`` , if it succeeds and ``false`` otherwise.

{xrst_end Jacobian}
-----------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base, class RecBase, class Vector>
void JacobianFor(ADFun<Base, RecBase> &f, const Vector &x, Vector &jac)
{  size_t i;
   size_t j;

   size_t n = f.Domain();
   size_t m = f.Range();

   // check Vector is Simple Vector class with Base type elements
   CheckSimpleVector<Base, Vector>();

   CPPAD_ASSERT_UNKNOWN( size_t(x.size())   == f.Domain() );
   CPPAD_ASSERT_UNKNOWN( size_t(jac.size()) == f.Range() * f.Domain() );

   // argument and result for forward mode calculations
   Vector u(n);
   Vector v(m);

   // initialize all the components
   for(j = 0; j < n; j++)
      u[j] = Base(0.0);

   // loop through the different coordinate directions
   for(j = 0; j < n; j++)
   {  // set u to the j-th coordinate direction
      u[j] = Base(1.0);

      // compute the partial of f w.r.t. this coordinate direction
      v = f.Forward(1, u);

      // reset u to vector of all zeros
      u[j] = Base(0.0);

      // return the result
      for(i = 0; i < m; i++)
         jac[ i * n + j ] = v[i];
   }
}
template <class Base, class RecBase, class Vector>
void JacobianRev(ADFun<Base, RecBase> &f, const Vector &x, Vector &jac)
{  size_t i;
   size_t j;

   size_t n = f.Domain();
   size_t m = f.Range();

   CPPAD_ASSERT_UNKNOWN( size_t(x.size())   == f.Domain() );
   CPPAD_ASSERT_UNKNOWN( size_t(jac.size()) == f.Range() * f.Domain() );

   // argument and result for reverse mode calculations
   Vector u(n);
   Vector v(m);

   // initialize all the components
   for(i = 0; i < m; i++)
      v[i] = Base(0.0);

   // loop through the different coordinate directions
   for(i = 0; i < m; i++)
   {  if( f.Parameter(i) )
      {  // return zero for this component of f
         for(j = 0; j < n; j++)
            jac[ i * n + j ] = Base(0.0);
      }
      else
      {
         // set v to the i-th coordinate direction
         v[i] = Base(1.0);

         // compute the derivative of this component of f
         u = f.Reverse(1, v);

         // reset v to vector of all zeros
         v[i] = Base(0.0);

         // return the result
         for(j = 0; j < n; j++)
            jac[ i * n + j ] = u[j];
      }
   }
}

template <class Base, class RecBase>
template <class Vector>
Vector ADFun<Base,RecBase>::Jacobian(const Vector &x)
{  size_t i;
   size_t n = Domain();
   size_t m = Range();

   CPPAD_ASSERT_KNOWN(
      size_t(x.size()) == n,
      "Jacobian: length of x not equal domain dimension for F"
   );

   // point at which we are evaluating the Jacobian
   Forward(0, x);

   // work factor for forward mode
   size_t workForward = n;

   // work factor for reverse mode
   size_t workReverse = 0;
   for(i = 0; i < m; i++)
   {  if( ! Parameter(i) )
         ++workReverse;
   }

   // choose the method with the least work
   Vector jac( n * m );
# ifdef CPPAD_FOR_TMB
   if( workForward < workReverse )
# else
   if( workForward <= workReverse )
# endif
      JacobianFor(*this, x, jac);
   else
      JacobianRev(*this, x, jac);

   return jac;
}

} // END CppAD namespace

# endif
