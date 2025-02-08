# ifndef CPPAD_UTILITY_ROMBERG_MUL_HPP
# define CPPAD_UTILITY_ROMBERG_MUL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin RombergMul}
{xrst_spell
   test test
}

Multi-dimensional Romberg Integration
#####################################

Syntax
******
| # ``include <cppad/utility/romberg_mul.hpp>``
| ``RombergMul`` < *Fun* , *SizeVector* , *FloatVector* , *m* > *R*
| *r* = *R* ( *F* , *a* , *b* , *n* , *p* , *e* )

Description
***********
Returns the Romberg integration estimate
:math:`r` for the multi-dimensional integral

.. math::

   r =
   \int_{a[0]}^{b[0]} \cdots \int_{a[m-1]}^{b[m-1]}
   \; F(x) \;
   {\bf d} x_0 \cdots {\bf d} x_{m-1}
   \; + \;
   \sum_{i=0}^{m-1}
   O \left[ ( b[i] - a[i] ) / 2^{n[i]-1} \right]^{2(p[i]+1)}

Include
*******
The file ``cppad/utility/romberg_mul.hpp``
is included by ``cppad/cppad.hpp``
but it can also be included separately with out the rest of
the ``CppAD`` routines.

m
*
The template parameter *m* must be convertible to a ``size_t``
object with a value that can be determined at compile time; for example
``2`` .
It determines the dimension of the domain space for the integration.

r
*
The return value *r* has prototype

   *Float* *r*

It is the estimate computed by ``RombergMul`` for the integral above
(see description of :ref:`RombergMul@Float` below).

F
*
The object *F* has the prototype

   *Fun* & *F*

It must support the operation

   *F* ( *x* )

The argument *x* to *F* has prototype

   ``const`` *Float* & *x*

The return value of *F* is a *Float* object

a
*
The argument *a* has prototype

   ``const`` *FloatVector* & *a*

It specifies the lower limit for the integration
(see description of :ref:`RombergMul@FloatVector` below).

b
*
The argument *b* has prototype

   ``const`` *FloatVector* & *b*

It specifies the upper limit for the integration.

n
*
The argument *n* has prototype

   ``const`` *SizeVector* & *n*

A total number of :math:`2^{n[i]-1} + 1`
evaluations of *F* ( *x* ) are used to estimate the integral
with respect to :math:`{\bf d} x_i`.

p
*
The argument *p* has prototype

   ``const`` *SizeVector* & *p*

For :math:`i = 0 , \ldots , m-1`,
:math:`n[i]` determines the accuracy order in the
approximation for the integral
that is returned by ``RombergMul`` .
The values in *p* must be less than or equal *n* ; i.e.,
*p* [ *i* ] <= *n* [ *i* ] .

e
*
The argument *e* has prototype

   *Float* & *e*

The input value of *e* does not matter
and its output value is an approximation for the absolute error in
the integral estimate.

Float
*****
The type *Float* is defined as the type of the elements of
:ref:`RombergMul@FloatVector` .
The type *Float* must satisfy the conditions
for a :ref:`NumericType-name` .
The routine :ref:`CheckNumericType-name` will generate an error message
if this is not the case.
In addition, if *x* and *y* are *Float* objects,

   *x* < *y*

returns the ``bool`` value true if *x* is less than
*y* and false otherwise.

FloatVector
***********
The type *FloatVector* must be a :ref:`SimpleVector-name` class.
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.
{xrst_toc_hidden
   example/utility/romberg_mul.cpp
}
Example
*******
The file :ref:`romberg_mul.cpp-name`
contains an example and test a test of using this routine.

Source Code
***********
The source code for this routine is in the file
``cppad/romberg_mul.hpp`` .

{xrst_end RombergMul}
*/

# include <cppad/utility/romberg_one.hpp>
# include <cppad/utility/check_numeric_type.hpp>
# include <cppad/utility/check_simple_vector.hpp>

namespace CppAD { // BEGIN CppAD namespace

template <class Fun, class FloatVector>
class SliceLast {
   typedef typename FloatVector::value_type Float;
private:
   Fun        *F;
   size_t      last;
   FloatVector x;
public:
   SliceLast( Fun *F_, size_t last_, const FloatVector &x_ )
   : F(F_) , last(last_), x(last + 1)
   {  size_t i;
      for(i = 0; i < last; i++)
         x[i] = x_[i];
   }
   double operator()(const Float &xlast)
   {  x[last] = xlast;
      return (*F)(x);
   }
};

template <class Fun, class SizeVector, class FloatVector, class Float>
class IntegrateLast {
private:
   Fun                 *F;
   const size_t        last;
   const FloatVector   a;
   const FloatVector   b;
   const SizeVector    n;
   const SizeVector    p;
   Float               esum;
   size_t              ecount;

public:
   IntegrateLast(
      Fun                *F_    ,
      size_t              last_ ,
      const FloatVector  &a_    ,
      const FloatVector  &b_    ,
      const SizeVector   &n_    ,
      const SizeVector   &p_    )
   : F(F_) , last(last_), a(a_) , b(b_) , n(n_) , p(p_)
   { }
   Float operator()(const FloatVector           &x)
   {  Float r, e;
      SliceLast<Fun, FloatVector           > S(F, last, x);
      r     = CppAD::RombergOne(
         S, a[last], b[last], n[last], p[last], e
      );
      esum = esum + e;
      ecount++;
      return r;
   }
   void ClearEsum(void)
   {  esum   = 0.; }
   Float GetEsum(void)
   {  return esum; }

   void ClearEcount(void)
   {  ecount   = 0; }
   size_t GetEcount(void)
   {  return ecount; }
};

template <class Fun, class SizeVector, class FloatVector, size_t m>
class RombergMul {
   typedef typename FloatVector::value_type Float;
public:
   RombergMul(void)
   {  }
   Float operator() (
      Fun                 &F  ,
      const FloatVector   &a  ,
      const FloatVector   &b  ,
      const SizeVector    &n  ,
      const SizeVector    &p  ,
      Float               &e  )
   {  Float r;

      typedef IntegrateLast<
         Fun         ,
         SizeVector  ,
         FloatVector ,
         Float       > IntegrateOne;

      IntegrateOne Fm1(&F, m-1, a, b, n, p);
      RombergMul<
         IntegrateOne,
         SizeVector  ,
         FloatVector ,
         m-1         > RombergMulM1;

      Fm1.ClearEsum();
      Fm1.ClearEcount();

      r  = RombergMulM1(Fm1, a, b, n, p, e);

      Float prod = 1;
      for(size_t i = 0; i < m-1; i++)
         prod *= (b[i] - a[i]);

# ifndef NDEBUG
      size_t pow2 = 1;
      for(size_t i = 0; i < m-1; i++)
         for(size_t j = 0; j < (n[i] - 1); j++)
            pow2 *= 2;
      assert( Fm1.GetEcount() == (pow2+1) );
# endif

      e = e + Fm1.GetEsum() * prod / Float( double(Fm1.GetEcount()) );

      return r;
   }
};

template <class Fun, class SizeVector, class FloatVector>
class RombergMul <Fun, SizeVector, FloatVector, 1> {
   typedef typename FloatVector::value_type Float;
public:
   Float operator() (
      Fun                 &F  ,
      const FloatVector   &a  ,
      const FloatVector   &b  ,
      const SizeVector    &n  ,
      const SizeVector    &p  ,
      Float               &e  )
   {  Float r;
      typedef IntegrateLast<
         Fun         ,
         SizeVector  ,
         FloatVector ,
         Float       > IntegrateOne;

      // check simple vector class specifications
      CheckSimpleVector<Float, FloatVector>();

      // check numeric type specifications
      CheckNumericType<Float>();

      IntegrateOne F0(&F, 0, a, b, n, p);

      F0.ClearEsum();
      F0.ClearEcount();

      r  = F0(a);

      assert( F0.GetEcount() == 1 );
      e = F0.GetEsum();

      return r;
   }
};

} // END CppAD namespace

# endif
