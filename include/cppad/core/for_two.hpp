# ifndef CPPAD_CORE_FOR_TWO_HPP
# define CPPAD_CORE_FOR_TWO_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin ForTwo}
{xrst_spell
   ddy
}

Forward Mode Second Partial Derivative Driver
#############################################

Syntax
******
| *ddy* = *f* . ``ForTwo`` ( *x* , *j* , *k* )

Purpose
*******
We use :math:`F : \B{R}^n \rightarrow \B{R}^m` to denote the
:ref:`glossary@AD Function` corresponding to *f* .
The syntax above sets

.. math::

   ddy [ i * p + \ell ]
   =
   \DD{ F_i }{ x_{j[ \ell ]} }{ x_{k[ \ell ]} } (x)

for :math:`i = 0 , \ldots , m-1`
and :math:`\ell = 0 , \ldots , p`,
where :math:`p` is the size of the vectors *j* and *k* .

f
*
The object *f* has prototype

   ``ADFun`` < *Base* > *f*

Note that the :ref:`ADFun-name` object *f* is not ``const``
(see :ref:`ForTwo@ForTwo Uses Forward` below).

x
*
The argument *x* has prototype

   ``const`` *BaseVector* & *x*

(see :ref:`ForTwo@BaseVector` below)
and its size
must be equal to *n* , the dimension of the
:ref:`fun_property@Domain` space for *f* .
It specifies
that point at which to evaluate the partial derivatives listed above.

j
*
The argument *j* has prototype

   ``const`` *SizeVector_t* & *j*

(see :ref:`ForTwo@SizeVector_t` below)
We use *p* to denote the size of the vector *j* .
All of the indices in *j*
must be less than *n* ; i.e.,
for :math:`\ell = 0 , \ldots , p-1`, :math:`j[ \ell ]  < n`.

k
*
The argument *k* has prototype

   ``const`` *SizeVector_t* & *k*

(see :ref:`ForTwo@SizeVector_t` below)
and its size must be equal to *p* ,
the size of the vector *j* .
All of the indices in *k*
must be less than *n* ; i.e.,
for :math:`\ell = 0 , \ldots , p-1`, :math:`k[ \ell ]  < n`.

ddy
***
The result *ddy* has prototype

   *BaseVector* *ddy*

(see :ref:`ForTwo@BaseVector` below)
and its size is :math:`m * p`.
It contains the requested partial derivatives; to be specific,
for :math:`i = 0 , \ldots , m - 1`
and :math:`\ell = 0 , \ldots , p - 1`

.. math::

   ddy [ i * p + \ell ]
   =
   \DD{ F_i }{ x_{j[ \ell ]} }{ x_{k[ \ell ]} } (x)

BaseVector
**********
The type *BaseVector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type Base<SimpleVector@Elements of Specified Type>` .
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.

SizeVector_t
************
The type *SizeVector_t* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type size_t<SimpleVector@Elements of Specified Type>` .
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.

ForTwo Uses Forward
*******************
After each call to :ref:`Forward-name` ,
the object *f* contains the corresponding
:ref:`Taylor coefficients<glossary@Taylor Coefficient>` .
After a call to ``ForTwo`` ,
the zero order Taylor coefficients correspond to
*f* . ``Forward`` (0, *x* )
and the other coefficients are unspecified.

Examples
********
{xrst_toc_hidden
   example/general/for_two.cpp
}
The routine
:ref:`ForTwo<for_two.cpp-name>` is both an example and test.
It returns ``true`` , if it succeeds and ``false`` otherwise.

{xrst_end ForTwo}
-----------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base, class RecBase>
template <class BaseVector, class SizeVector_t>
BaseVector ADFun<Base,RecBase>::ForTwo(
   const BaseVector   &x,
   const SizeVector_t &j,
   const SizeVector_t &k)
{  size_t i;
   size_t j1;
   size_t k1;
   size_t l;

   size_t n = Domain();
   size_t m = Range();
   size_t p = j.size();

   // check BaseVector is Simple Vector class with Base type elements
   CheckSimpleVector<Base, BaseVector>();

   // check SizeVector_t is Simple Vector class with size_t elements
   CheckSimpleVector<size_t, SizeVector_t>();

   CPPAD_ASSERT_KNOWN(
      x.size() == n,
      "ForTwo: Length of x not equal domain dimension for f."
   );
   CPPAD_ASSERT_KNOWN(
      j.size() == k.size(),
      "ForTwo: Lenght of the j and k vectors are not equal."
   );
   // point at which we are evaluating the second partials
   Forward(0, x);


   // dimension the return value
   BaseVector ddy(m * p);

   // allocate memory to hold all possible diagonal Taylor coefficients
   // (for large sparse cases, this is not efficient)
   BaseVector D(m * n);

   // boolean flag for which diagonal coefficients are computed
   CppAD::vector<bool> c(n);
   for(j1 = 0; j1 < n; j1++)
      c[j1] = false;

   // direction vector in argument space
   BaseVector dx(n);
   for(j1 = 0; j1 < n; j1++)
      dx[j1] = Base(0.0);

   // result vector in range space
   BaseVector dy(m);

   // compute the diagonal coefficients that are needed
   for(l = 0; l < p; l++)
   {  j1 = j[l];
      k1 = k[l];
      CPPAD_ASSERT_KNOWN(
      j1 < n,
      "ForTwo: an element of j not less than domain dimension for f."
      );
      CPPAD_ASSERT_KNOWN(
      k1 < n,
      "ForTwo: an element of k not less than domain dimension for f."
      );
      size_t count = 2;
      while(count)
      {  count--;
         if( ! c[j1] )
         {  // diagonal term in j1 direction
            c[j1]  = true;
            dx[j1] = Base(1.0);
            Forward(1, dx);

            dx[j1] = Base(0.0);
            dy     = Forward(2, dx);
            for(i = 0; i < m; i++)
               D[i * n + j1 ] = dy[i];
         }
         j1 = k1;
      }
   }
   // compute all the requested cross partials
   for(l = 0; l < p; l++)
   {  j1 = j[l];
      k1 = k[l];
      if( j1 == k1 )
      {  for(i = 0; i < m; i++)
            ddy[i * p + l] = Base(2.0) * D[i * n + j1];
      }
      else
      {
         // cross term in j1 and k1 directions
         dx[j1] = Base(1.0);
         dx[k1] = Base(1.0);
         Forward(1, dx);

         dx[j1] = Base(0.0);
         dx[k1] = Base(0.0);
         dy = Forward(2, dx);

         // place result in return value
         for(i = 0; i < m; i++)
            ddy[i * p + l] = dy[i] - D[i*n+j1] - D[i*n+k1];

      }
   }
   return ddy;
}

} // END CppAD namespace

# endif
