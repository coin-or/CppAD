/* $Id: */

// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <assert.h>
# include <stdlib.h>
# include <math.h>
# include <stdio.h>
# include <stddef.h>
# include <float.h>

// In the case of plain C, we defined the type bool together with ture, false
# ifndef __cplusplus
# define bool int
# define true 1
# define false 0
# endif

/*
-------------------------------------------------------------------------------
{xrst_begin det_of_minor_c app}
{xrst_spell
   factorial
}

Determinant of a Minor
######################

Syntax
******
*d* = ``det_of_minor`` ( *a* , *m* , *n* , *r* , *c* )

Purpose
*******
returns the determinant of a minor of the matrix :math:`A`
using expansion by minors.
The elements of the :math:`n \times n` minor :math:`M`
of the matrix :math:`A` are defined,
for :math:`i = 0 , \ldots , n-1` and :math:`j = 0 , \ldots , n-1`, by

.. math::

   M_{i,j} = A_{R(i), C(j)}

where the functions
:math:`R(i)` is defined by the :ref:`argument r<det_of_minor@r>` and
:math:`C(j)` is defined by the :ref:`argument c<det_of_minor@c>` .

This function
is for example and testing purposes only.
Expansion by minors is chosen as an example because it uses
a lot of floating point operations yet does not require much source code
(on the order of *m* factorial floating point operations and
about 70 lines of source code including comments).
This is not an efficient method for computing a determinant;
for example, using an LU factorization would be better.

Determinant of A
****************
If the following conditions hold, the minor is the
entire matrix :math:`A` and hence ``det_of_minor``
will return the determinant of :math:`A`:

#. :math:`n = m`.
#. for :math:`i = 0 , \ldots , m-1`, :math:`r[i] = i+1`,
   and :math:`r[m] = 0`.
#. for :math:`j = 0 , \ldots , m-1`, :math:`c[j] = j+1`,
   and :math:`c[m] = 0`.

a
*
The argument *a* has prototype

   ``const double`` * *a*

and is a vector with size :math:`m * m`.
The elements of the :math:`m \times m` matrix :math:`A` are defined,
for :math:`i = 0 , \ldots , m-1` and :math:`j = 0 , \ldots , m-1`, by

.. math::

   A_{i,j} = a[ i * m + j]

m
*
The argument *m* has prototype

   ``size_t`` *m*

and is the size of the square matrix :math:`A`.

n
*
The argument *n* has prototype

   ``size_t`` *n*

and is the size of the square minor :math:`M`.

r
*
The argument *r* has prototype

   ``size_t`` * *r*

and is a vector with :math:`m + 1` elements.
This vector defines the function :math:`R(i)`
which specifies the rows of the minor :math:`M`.
To be specific, the function :math:`R(i)`
for :math:`i = 0, \ldots , n-1` is defined by

.. math::
   :nowrap:

   \begin{eqnarray}
      R(0)   & = & r[m]
      \\
      R(i+1) & = & r[ R(i) ]
   \end{eqnarray}

All the elements of *r* must have value
less than or equal *m* .
The elements of vector *r* are modified during the computation,
and restored to their original value before the return from
``det_of_minor`` .

c
*
The argument *c* has prototype

   ``size_t`` * *c*

and is a vector with :math:`m + 1` elements
This vector defines the function :math:`C(i)`
which specifies the rows of the minor :math:`M`.
To be specific, the function :math:`C(i)`
for :math:`j = 0, \ldots , n-1` is defined by

.. math::
   :nowrap:

   \begin{eqnarray}
      C(0)   & = & c[m]
      \\
      C(j+1) & = & c[ C(j) ]
   \end{eqnarray}

All the elements of *c* must have value
less than or equal *m* .
The elements of vector *c* are modified during the computation,
and restored to their original value before the return from
``det_of_minor`` .

d
*
The result *d* has prototype

   ``double`` *d*

and is equal to the determinant of the minor :math:`M`.

Source Code
***********
{xrst_spell_off}
{xrst_code cpp} */
double det_of_minor(
   const double*        a  ,
   size_t               m  ,
   size_t               n  ,
   size_t*              r  ,
   size_t*              c  )
{  size_t R0, Cj, Cj1, j;
   double detM, M0j, detS;
   int s;

   R0 = r[m]; /* R(0) */
   Cj = c[m]; /* C(j)    (case j = 0) */
   Cj1 = m;   /* C(j-1)  (case j = 0) */

   /* check for 1 by 1 case */
   if( n == 1 ) return a[ R0 * m + Cj ];

   /* initialize determinant of the minor M */
   detM = 0.;

   /* initialize sign of factor for neat sub-minor */
   s = 1;

   /* remove row with index 0 in M from all the sub-minors of M */
   r[m] = r[R0];

   /* for each column of M */
   for(j = 0; j < n; j++)
   {  /* element with index (0,j) in the minor M */
      M0j = a[ R0 * m + Cj ];

      /* remove column with index j in M to form next sub-minor S of M */
      c[Cj1] = c[Cj];

      /* compute determinant of the current sub-minor S */
      detS = det_of_minor(a, m, n - 1, r, c);

      /* restore column Cj to representation of M as a minor of A */
      c[Cj1] = Cj;

      /* include this sub-minor term in the summation */
      if( s > 0 )
         detM = detM + M0j * detS;
      else
         detM = detM - M0j * detS;

      /* advance to neat column of M */
      Cj1 = Cj;
      Cj  = c[Cj];
      s   = - s;
   }

   /* restore row zero to the minor representation for M */
   r[m] = R0;

   /* return the determinant of the minor M */
   return detM;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end det_of_minor_c}
-------------------------------------------------------------------------------
{xrst_begin det_by_minor_c app}

Compute Determinant using Expansion by Minors
#############################################

Syntax
******
*d* = ``det_by_minor`` ( *a* , *n* )

Purpose
*******
returns the determinant of the matrix :math:`A`
using expansion by minors.
The elements of the :math:`n \times n` minor :math:`M`
of the matrix :math:`A` are defined,
for :math:`i = 0 , \ldots , n-1` and :math:`j = 0 , \ldots , n-1`, by

.. math::

   M_{i,j} = A_{i, j}

a
*
The argument *a* has prototype

   ``const double`` * *a*

and is a vector with size :math:`m * m`.
The elements of the :math:`m \times m` matrix :math:`A` are defined,
for :math:`i = 0 , \ldots , m-1` and :math:`j = 0 , \ldots , m-1`, by

.. math::

   A_{i,j} = a[ i * m + j]

m
*
The argument *m* has prototype

   ``size_t`` *m*

and is the number of rows (and columns) in the square matrix :math:`A`.

Source Code
***********
{xrst_spell_off}
{xrst_code cpp} */
double det_by_minor(double* a, size_t m)
{  size_t *r, *c, i;
   double value;

   r = (size_t*) malloc( (m+1) * sizeof(size_t) );
   c = (size_t*) malloc( (m+1) * sizeof(size_t) );

   assert(m <= 100);
   for(i = 0; i < m; i++)
   {  r[i] = i+1;
      c[i] = i+1;
   }
   r[m] = 0;
   c[m] = 0;

   value = det_of_minor(a, m, m, r, c);

   free(r);
   free(c);
   return value;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end det_by_minor_c}
--------------------------------------------------------------------------
{xrst_begin uniform_01_c app}
{xrst_spell
   variate
}

Simulate a [0,1] Uniform Random Variate
#######################################

Syntax
******

   ``random_seed`` ( *seed* )

``uniform_01`` ( *n* , *a* )

Purpose
*******
This routine is used to create random values for speed testing purposes.

seed
****
The argument *seed* has prototype

   ``size_t`` *seed*

It specifies a seed
for the uniform random number generator.

n
*
The argument *n* has prototype

   ``size_t`` *n*

It specifies the number of elements in the random vector *a* .

a
*
The argument *a* has prototype

   ``double`` * *a*

.
The input value of the elements of *a* does not matter.
Upon return, the elements of *a* are set to values
randomly sampled over the interval [0,1].

Source Code
***********
{xrst_spell_off}
{xrst_code cpp} */
void random_seed(size_t seed)
{  srand( (unsigned int) seed );
}
void uniform_01(size_t n, double* a)
{  static double factor = 1. / (double) RAND_MAX;
   while(n--)
      a[n] = rand() * factor;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end uniform_01_c}
------------------------------------------------------------------------------
{xrst_begin correct_det_by_minor_c app}

Correctness Test of det_by_minor Routine
########################################

Syntax
******
*flag* = ``correct_det_by_minor`` ()

flag
****
The return value has prototype

   ``bool`` *flag*

It value is ``1`` if the test passes and ``0`` otherwise.

Source Code
***********
{xrst_spell_off}
{xrst_code cpp} */
bool correct_det_by_minor(void)
{  double a[9], det, check;
   double eps99 = 99.0 * DBL_EPSILON;

   random_seed(123);
   uniform_01(9, a);

   /* compute determinant using expansion by minors */
   det = det_by_minor(a, 3);

   /* use expansion by minors to hand code the determinant  */
   check = 0.;
   check += a[0] * ( a[4] * a[8] - a[5] * a[7] );
   check -= a[1] * ( a[3] * a[8] - a[5] * a[6] );
   check += a[2] * ( a[3] * a[7] - a[4] * a[6] );

   if( fabs(det / check - 1.0) < eps99 )
      return true;
   return false;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end correct_det_by_minor_c}
------------------------------------------------------------------------------
{xrst_begin repeat_det_by_minor_c app}

Repeat det_by_minor Routine A Specified Number of Times
#######################################################

Syntax
******
``repeat_det_by_minor`` ( *repeat* , *size* )

repeat
******
The argument has prototype

   ``size_t`` *repeat*

It specifies the number of times to repeat the calculation.

size
****
The argument has prototype

   ``size_t`` *size*

It specifies the number of rows (and columns) in the square
matrix we are computing the determinant of.

Source Code
***********
{xrst_spell_off}
{xrst_code cpp} */
void repeat_det_by_minor(size_t repeat, size_t size)
{  double *a;
   a = (double*) malloc( (size * size) * sizeof(double) );

   while(repeat--)
   {  uniform_01(size * size, a);
      det_by_minor(a, size);
   }

   free(a);
   return;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end repeat_det_by_minor_c}
------------------------------------------------------------------------------
{xrst_begin elapsed_seconds_c app}

Returns Elapsed Number of Seconds
#################################

Syntax
******
*s* = ``elapsed_seconds`` ()

Purpose
*******
This routine is accurate to within .02 seconds
It does not necessary work for time intervals that are greater than a day.

s
*
is a ``double`` equal to the
number of seconds since the first call to ``elapsed_seconds`` .

Source Code
***********
{xrst_spell_off}
{xrst_code cpp} */
# if _MSC_VER
// ---------------------------------------------------------------------------
// Microsoft version of timer
# ifndef NOMINMAX
# define NOMINMAX  // so windows.h does not define min and max as macros
# endif
# include <windows.h>
# include <cassert>
double elapsed_seconds(void)
{  static bool       first_  = true;
   static SYSTEMTIME st_;
   double hour, minute, second, milli, diff;
   SYSTEMTIME st;

   if( first_ )
   {  GetSystemTime(&st_);
      first_ = false;
      return 0.;
   }
   GetSystemTime(&st);

   hour   = (double) st.wHour         - (double) st_.wHour;
   minute = (double) st.wMinute       - (double) st_.wMinute;
   second = (double) st.wSecond       - (double) st_.wSecond;
   milli  = (double) st.wMilliseconds - (double) st_.wMilliseconds;

   diff   = 1e-3*milli + second + 60.*minute + 3600.*hour;
   if( diff < 0. )
      diff += 3600.*24.;
   assert( 0 <= diff && diff < 3600.*24. );

   return diff;
}
# else
// ---------------------------------------------------------------------------
// Unix version of timer
# include <sys/time.h>
double elapsed_seconds(void)
{  double sec, usec, diff;

   static bool first_ = true;
   static struct timeval tv_first;
   struct timeval        tv;
   if( first_ )
   {  gettimeofday(&tv_first, NULL);
      first_ = false;
      return 0.;
   }
   gettimeofday(&tv, NULL);
   assert( tv.tv_sec >= tv_first.tv_sec );

   sec  = (double)(tv.tv_sec -  tv_first.tv_sec);
   usec = (double)tv.tv_usec - (double)tv_first.tv_usec;
   diff = sec + 1e-6*usec;

   return diff;
}
# endif
/* {xrst_code}
{xrst_spell_on}

{xrst_end elapsed_seconds_c}
-----------------------------------------------------------------------------
{xrst_begin time_det_by_minor_c app}

Determine Amount of Time to Execute det_by_minor
################################################

Syntax
******
*time* = ``time_test`` ( *size* , *time_min* )

Purpose
*******
reports the amount of wall clock time for ``det_by_minor``
to compute the determinant of a square matrix.

The *size* has prototype

   ``size_t`` *size*

It specifies the number of rows (and columns) in the square matrix
that the determinant is being calculated for.

time_min
********
The argument *time_min* has prototype

   ``double`` *time_min*

It specifies the minimum amount of time in seconds
that the *test* routine should take.
The calculations is repeated the necessary number of times so that
this amount of execution time (or more) is reached.

time
****
The return value *time* has prototype

   ``double`` *time*

and is the number of wall clock seconds that it took for
``det_by_minor`` to compute its determinant
(plus overhead which includes choosing a random matrix).

Source Code
***********
{xrst_spell_off}
{xrst_code cpp} */
double time_det_by_minor(size_t size, double time_min)
{  size_t repeat;
   double s0, s1, time;
   repeat = 0;
   s0     = elapsed_seconds();
   s1     = s0;
   while( s1 - s0 < time_min )
   {  if( repeat == 0 )
         repeat = 1;
      else
         repeat = 2 * repeat;
      s0     = elapsed_seconds();
      repeat_det_by_minor(repeat, size);
      s1     = elapsed_seconds();
   }
   time = (s1 - s0) / (double) repeat;
   return time;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end time_det_by_minor_c}
------------------------------------------------------------------------------
{xrst_begin main_compare_c app}

Main Program For Comparing C and C++ Speed
##########################################

Source Code
***********
{xrst_spell_off}
{xrst_code cpp} */
int main(void)
{  bool flag;
   size_t i;

   random_seed(123);

   printf("correct_det_by_minor: ");
   flag = correct_det_by_minor();
   if( flag )
      printf("OK\n");
   else
      printf("Error\n");

   for(i = 0; i < 5; i++)
   {  double time_min = 1.0;
      size_t size     = 2 + i * 2;
      int   i_size    = (int) size;
      printf("time_det_minor for %d x %d matrix = ", i_size, i_size);
      printf("%g\n", time_det_by_minor(size, time_min) );
   }

   if( flag )
      return 0;
   return 1;
}
/* {xrst_code}
{xrst_spell_on}

{xrst_end main_compare_c}
*/
