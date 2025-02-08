# ifndef CPPAD_CORE_REV_TWO_HPP
# define CPPAD_CORE_REV_TWO_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin RevTwo}
{xrst_spell
   ddw
}

Reverse Mode Second Partial Derivative Driver
#############################################

Syntax
******
| *ddw* = *f* . ``RevTwo`` ( *x* , *i* , *j* )

Purpose
*******
We use :math:`F : \B{R}^n \rightarrow \B{R}^m` to denote the
:ref:`glossary@AD Function` corresponding to *f* .
The syntax above sets

.. math::

   ddw [ k * p + \ell ]
   =
   \DD{ F_{i[ \ell ]} }{ x_{j[ \ell ]} }{ x_k } (x)

for :math:`k = 0 , \ldots , n-1`
and :math:`\ell = 0 , \ldots , p`,
where :math:`p` is the size of the vectors *i* and *j* .

f
*
The object *f* has prototype

   ``ADFun`` < *Base* > *f*

Note that the :ref:`ADFun-name` object *f* is not ``const``
(see :ref:`RevTwo@RevTwo Uses Forward` below).

x
*
The argument *x* has prototype

   ``const`` *BaseVector* & *x*

(see :ref:`RevTwo@BaseVector` below)
and its size
must be equal to *n* , the dimension of the
:ref:`fun_property@Domain` space for *f* .
It specifies
that point at which to evaluate the partial derivatives listed above.

i
*
The argument *i* has prototype

   ``const`` *SizeVector_t* & *i*

(see :ref:`RevTwo@SizeVector_t` below)
We use *p* to denote the size of the vector *i* .
All of the indices in *i*
must be less than *m* , the dimension of the
:ref:`fun_property@Range` space for *f* ; i.e.,
for :math:`\ell = 0 , \ldots , p-1`, :math:`i[ \ell ]  < m`.

j
*
The argument *j* has prototype

   ``const`` *SizeVector_t* & *j*

(see :ref:`RevTwo@SizeVector_t` below)
and its size must be equal to *p* ,
the size of the vector *i* .
All of the indices in *j*
must be less than *n* ; i.e.,
for :math:`\ell = 0 , \ldots , p-1`, :math:`j[ \ell ]  < n`.

ddw
***
The result *ddw* has prototype

   *BaseVector* *ddw*

(see :ref:`RevTwo@BaseVector` below)
and its size is :math:`n * p`.
It contains the requested partial derivatives; to be specific,
for :math:`k = 0 , \ldots , n - 1`
and :math:`\ell = 0 , \ldots , p - 1`

.. math::

   ddw [ k * p + \ell ]
   =
   \DD{ F_{i[ \ell ]} }{ x_{j[ \ell ]} }{ x_k } (x)

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

RevTwo Uses Forward
*******************
After each call to :ref:`Forward-name` ,
the object *f* contains the corresponding
:ref:`Taylor coefficients<glossary@Taylor Coefficient>` .
After a call to ``RevTwo`` ,
the zero order Taylor coefficients correspond to
*f* . ``Forward`` (0, *x* )
and the other coefficients are unspecified.

Examples
********
{xrst_toc_hidden
   example/general/rev_two.cpp
}
The routine
:ref:`RevTwo<rev_two.cpp-name>` is both an example and test.
It returns ``true`` , if it succeeds and ``false`` otherwise.

{xrst_end RevTwo}
-----------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base, class RecBase>
template <class BaseVector, class SizeVector_t>
BaseVector ADFun<Base,RecBase>::RevTwo(
   const BaseVector   &x,
   const SizeVector_t &i,
   const SizeVector_t &j)
{  size_t i1;
   size_t j1;
   size_t k;
   size_t l;

   size_t n = Domain();
   size_t m = Range();
   size_t p = i.size();

   // check BaseVector is Simple Vector class with Base elements
   CheckSimpleVector<Base, BaseVector>();

   // check SizeVector_t is Simple Vector class with size_t elements
   CheckSimpleVector<size_t, SizeVector_t>();

   CPPAD_ASSERT_KNOWN(
      x.size() == n,
      "RevTwo: Length of x not equal domain dimension for f."
   );
   CPPAD_ASSERT_KNOWN(
      i.size() == j.size(),
      "RevTwo: Lenght of the i and j vectors are not equal."
   );
   // point at which we are evaluating the second partials
   Forward(0, x);

   // dimension the return value
   BaseVector ddw(n * p);

   // direction vector in argument space
   BaseVector dx(n);
   for(j1 = 0; j1 < n; j1++)
      dx[j1] = Base(0.0);

   // direction vector in range space
   BaseVector w(m);
   for(i1 = 0; i1 < m; i1++)
      w[i1] = Base(0.0);

   // place to hold the results of a reverse calculation
   BaseVector r(n * 2);

   // check the indices in i and j
   for(l = 0; l < p; l++)
   {  i1 = i[l];
      j1 = j[l];
      CPPAD_ASSERT_KNOWN(
      i1 < m,
      "RevTwo: an eleemnt of i not less than range dimension for f."
      );
      CPPAD_ASSERT_KNOWN(
      j1 < n,
      "RevTwo: an element of j not less than domain dimension for f."
      );
   }

   // loop over all forward directions
   for(j1 = 0; j1 < n; j1++)
   {  // first order forward mode calculation done
      bool first_done = false;
      for(l = 0; l < p; l++) if( j[l] == j1 )
      {  if( ! first_done )
         {  first_done = true;

            // first order forward mode in j1 direction
            dx[j1] = Base(1.0);
            Forward(1, dx);
            dx[j1] = Base(0.0);
         }
         // execute a reverse in this component direction
         i1    = i[l];
         w[i1] = Base(1.0);
         r     = Reverse(2, w);
         w[i1] = Base(0.0);

         // place the reverse result in return value
         for(k = 0; k < n; k++)
            ddw[k * p + l] = r[k * 2 + 1];
      }
   }
   return ddw;
}

} // END CppAD namespace

# endif
