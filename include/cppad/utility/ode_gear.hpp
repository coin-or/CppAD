# ifndef CPPAD_UTILITY_ODE_GEAR_HPP
# define CPPAD_UTILITY_ODE_GEAR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin OdeGear}
{xrst_spell
   dep
   jan
   pp
   test test
   vol
}

An Arbitrary Order Gear Method
##############################

Syntax
******
| # ``include <cppad/utility/ode_gear.hpp>``
| ``OdeGear`` ( *F* , *m* , *n* , *T* , *X* , *e* )

Purpose
*******
This routine applies
:ref:`OdeGear@Gear's Method`
to solve an explicit set of ordinary differential equations.
We are given
:math:`f : \B{R} \times \B{R}^n \rightarrow \B{R}^n` be a smooth function.
This routine solves the following initial value problem

.. math::
   :nowrap:

   \begin{eqnarray}
      x( t_{m-1} )  & = & x^0    \\
      x^\prime (t)  & = & f[t , x(t)]
   \end{eqnarray}

for the value of :math:`x( t_m )`.
If your set of  ordinary differential equations are not stiff
an explicit method may be better (perhaps :ref:`Runge45-name` .)

Include
*******
The file ``cppad/utility/ode_gear.hpp``
is included by ``cppad/cppad.hpp``
but it can also be included separately with out the rest of
the ``CppAD`` routines.

Fun
***
The class *Fun*
and the object *F* satisfy the prototype

   *Fun* & *F*

This must support the following set of calls

| |tab| *F* . ``Ode`` ( *t* , *x* , *f* )
| |tab| *F* . ``Ode_dep`` ( *t* , *x* , *f_x* )

t
=
The argument *t* has prototype

   ``const`` *Scalar* & *t*

(see description of :ref:`OdeGear@Scalar` below).

x
=
The argument *x* has prototype

   ``const`` *Vector* & *x*

and has size *n*
(see description of :ref:`OdeGear@Vector` below).

f
=
The argument *f* to *F* . ``Ode`` has prototype

   *Vector* & *f*

On input and output, *f* is a vector of size *n*
and the input values of the elements of *f* do not matter.
On output,
*f* is set equal to :math:`f(t, x)`
(see *f* ( *t* , *x* ) in :ref:`OdeGear@Purpose` ).

f_x
===
The argument *f_x* has prototype

   *Vector* & *f_x*

On input and output, *f_x* is a vector of size :math:`n * n`
and the input values of the elements of *f_x* do not matter.
On output,

.. math::

   f\_x [i * n + j] = \partial_{x(j)} f_i ( t , x )

Warning
=======
The arguments *f* , and *f_x*
must have a call by reference in their prototypes; i.e.,
do not forget the ``&`` in the prototype for
*f* and *f_x* .

m
*
The argument *m* has prototype

   ``size_t`` *m*

It specifies the order (highest power of :math:`t`)
used to represent the function :math:`x(t)` in the multi-step method.
Upon return from ``OdeGear`` ,
the *i*-th component of the polynomial is defined by

.. math::

   p_i ( t_j ) = X[ j * n + i ]

for :math:`j = 0 , \ldots , m` (where :math:`0 \leq i < n`).
The value of :math:`m` must be greater than or equal one.

n
*
The argument *n* has prototype

   ``size_t`` *n*

It specifies the range space dimension of the
vector valued function :math:`x(t)`.

T
*
The argument *T* has prototype

   ``const`` *Vector* & *T*

and size greater than or equal to :math:`m+1`.
For :math:`j = 0 , \ldots m`, :math:`T[j]` is the time
corresponding to time corresponding
to a previous point in the multi-step method.
The value :math:`T[m]` is the time
of the next point in the multi-step method.
The array :math:`T` must be monotone increasing; i.e.,
:math:`T[j] < T[j+1]`.
Above and below we often use the shorthand :math:`t_j` for :math:`T[j]`.

X
*
The argument *X* has the prototype

   *Vector* & *X*

and size greater than or equal to :math:`(m+1) * n`.
On input to ``OdeGear`` ,
for :math:`j = 0 , \ldots , m-1`, and
:math:`i = 0 , \ldots , n-1`

.. math::

   X[ j * n + i ] = x_i ( t_j )

Upon return from ``OdeGear`` ,
for :math:`i = 0 , \ldots , n-1`

.. math::

   X[ m * n + i ] \approx x_i ( t_m )

e
*
The vector *e* is an approximate error bound for the result; i.e.,

.. math::

   e[i] \geq | X[ m * n + i ] - x_i ( t_m ) |

The order of this approximation is one less than the order of
the solution; i.e.,

.. math::

   e = O ( h^m )

where :math:`h` is the maximum of :math:`t_{j+1} - t_j`.

Scalar
******
The type *Scalar* must satisfy the conditions
for a :ref:`NumericType-name` .
The routine :ref:`CheckNumericType-name` will generate an error message
if this is not the case.
In addition, the following operations must be defined for
*Scalar* objects *a* and *b* :

.. list-table::
   :widths: auto

   * - **Operation**
     - **Description**
   * - *a* < *b*
     - less than operator (returns a ``bool`` object)

Vector
******
The type *Vector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type Scalar<SimpleVector@Elements of Specified Type>` .
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.

Example
*******
{xrst_toc_hidden
   example/utility/ode_gear.cpp
}
The file
:ref:`ode_gear.cpp-name`
contains an example and test a test of using this routine.

Source Code
***********
The source code for this routine is in the file
``cppad/ode_gear.hpp`` .

Theory
******
For this discussion we use the shorthand :math:`x_j`
for the value :math:`x ( t_j ) \in \B{R}^n` which is not to be confused
with :math:`x_i (t) \in \B{R}` in the notation above.
The interpolating polynomial :math:`p(t)` is given by

.. math::

   p(t) =
   \sum_{j=0}^m
   x_j
   \frac{
      \prod_{i \neq j} ( t - t_i )
   }{
      \prod_{i \neq j} ( t_j - t_i )
   }

The derivative :math:`p^\prime (t)` is given by

.. math::

   p^\prime (t) =
   \sum_{j=0}^m
   x_j
   \frac{
      \sum_{i \neq j} \prod_{k \neq i,j} ( t - t_k )
   }{
      \prod_{k \neq j} ( t_j - t_k )
   }

Evaluating the derivative at the point :math:`t_\ell` we have

.. math::
   :nowrap:

   \begin{eqnarray}
   p^\prime ( t_\ell ) & = &
   x_\ell
   \frac{
      \sum_{i \neq \ell} \prod_{k \neq i,\ell} ( t_\ell - t_k )
   }{
      \prod_{k \neq \ell} ( t_\ell - t_k )
   }
   +
   \sum_{j \neq \ell}
   x_j
   \frac{
      \sum_{i \neq j} \prod_{k \neq i,j} ( t_\ell - t_k )
   }{
      \prod_{k \neq j} ( t_j - t_k )
   }
   \\
   & = &
   x_\ell
   \sum_{i \neq \ell}
   \frac{ 1 }{ t_\ell - t_i }
   +
   \sum_{j \neq \ell}
   x_j
   \frac{
      \prod_{k \neq \ell,j} ( t_\ell - t_k )
   }{
      \prod_{k \neq j} ( t_j - t_k )
   }
   \\
   & = &
   x_\ell
   \sum_{k \neq \ell} ( t_\ell - t_k )^{-1}
   +
   \sum_{j \neq \ell}
   x_j
   ( t_j - t_\ell )^{-1}
   \prod_{k \neq \ell ,j} ( t_\ell - t_k ) / ( t_j - t_k )
   \end{eqnarray}

We define the vector :math:`\alpha \in \B{R}^{m+1}` by

.. math::

   \alpha_j = \left\{ \begin{array}{ll}
   \sum_{k \neq m} ( t_m - t_k )^{-1}
      & {\rm if} \; j = m
   \\
   ( t_j - t_m )^{-1}
   \prod_{k \neq m,j} ( t_m - t_k ) / ( t_j - t_k )
      & {\rm otherwise}
   \end{array} \right.

It follows that

.. math::

   p^\prime ( t_m ) = \alpha_0 x_0 + \cdots + \alpha_m x_m

Gear's method determines :math:`x_m` by solving the following
nonlinear equation

.. math::

   f( t_m , x_m ) = \alpha_0 x_0 + \cdots + \alpha_m x_m

Newton's method for solving this equation determines iterates,
which we denote by :math:`x_m^k`, by solving the following affine
approximation of the equation above

.. math::
   :nowrap:

   \begin{eqnarray}
   f( t_m , x_m^{k-1} ) + \partial_x f( t_m , x_m^{k-1} ) ( x_m^k - x_m^{k-1} )
   & = &
   \alpha_0 x_0^k + \alpha_1 x_1 + \cdots + \alpha_m x_m
   \\
   \left[ \alpha_m I - \partial_x f( t_m , x_m^{k-1} ) \right]  x_m
   & = &
   \left[
   f( t_m , x_m^{k-1} ) - \partial_x f( t_m , x_m^{k-1} ) x_m^{k-1}
   - \alpha_0 x_0 - \cdots - \alpha_{m-1} x_{m-1}
   \right]
   \end{eqnarray}

In order to initialize Newton's method; i.e. choose :math:`x_m^0`
we define the vector :math:`\beta \in \B{R}^{m+1}` by

.. math::

   \beta_j = \left\{ \begin{array}{ll}
   \sum_{k \neq m-1} ( t_{m-1} - t_k )^{-1}
      & {\rm if} \; j = m-1
   \\
   ( t_j - t_{m-1} )^{-1}
   \prod_{k \neq m-1,j} ( t_{m-1} - t_k ) / ( t_j - t_k )
      & {\rm otherwise}
   \end{array} \right.

It follows that

.. math::

   p^\prime ( t_{m-1} ) = \beta_0 x_0 + \cdots + \beta_m x_m

We solve the following approximation of the equation above to determine
:math:`x_m^0`:

.. math::

   f( t_{m-1} , x_{m-1} ) =
   \beta_0 x_0 + \cdots + \beta_{m-1} x_{m-1} + \beta_m x_m^0

Gear's Method
*************
C. W. Gear,
*Simultaneous Numerical Solution of Differential-Algebraic Equations* ,
IEEE Transactions on Circuit Theory,
vol. 18, no. 1, pp. 89-95, Jan. 1971.

{xrst_end OdeGear}
--------------------------------------------------------------------------
*/

# include <cstddef>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/utility/check_simple_vector.hpp>
# include <cppad/utility/check_numeric_type.hpp>
# include <cppad/utility/vector.hpp>
# include <cppad/utility/lu_factor.hpp>
# include <cppad/utility/lu_invert.hpp>

namespace CppAD { // BEGIN CppAD namespace

template <class Vector, class Fun>
void OdeGear(
   Fun          &F  ,
   size_t        m  ,
   size_t        n  ,
   const Vector &T  ,
   Vector       &X  ,
   Vector       &e  )
{
   // temporary indices
   size_t i, j, k;

   typedef typename Vector::value_type Scalar;

   // check numeric type specifications
   CheckNumericType<Scalar>();

   // check simple vector class specifications
   CheckSimpleVector<Scalar, Vector>();

   CPPAD_ASSERT_KNOWN(
      m >= 1,
      "OdeGear: m is less than one"
   );
   CPPAD_ASSERT_KNOWN(
      n > 0,
      "OdeGear: n is equal to zero"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(T.size()) >= (m+1),
      "OdeGear: size of T is not greater than or equal (m+1)"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(X.size()) >= (m+1) * n,
      "OdeGear: size of X is not greater than or equal (m+1) * n"
   );
   for(j = 0; j < m; j++) CPPAD_ASSERT_KNOWN(
      T[j] < T[j+1],
      "OdeGear: the array T is not monotone increasing"
   );

   // some constants
   Scalar zero(0);
   Scalar one(1);

   // vectors required by method
   Vector alpha(m + 1);
   Vector beta(m + 1);
   Vector f(n);
   Vector f_x(n * n);
   Vector x_m0(n);
   Vector x_m(n);
   Vector b(n);
   Vector A(n * n);

   // compute alpha[m]
   alpha[m] = zero;
   for(k = 0; k < m; k++)
      alpha[m] += one / (T[m] - T[k]);

   // compute beta[m-1]
   beta[m-1] = one / (T[m-1] - T[m]);
   for(k = 0; k < m-1; k++)
      beta[m-1] += one / (T[m-1] - T[k]);


   // compute other components of alpha
   for(j = 0; j < m; j++)
   {  // compute alpha[j]
      alpha[j] = one / (T[j] - T[m]);
      for(k = 0; k < m; k++)
      {  if( k != j )
         {  alpha[j] *= (T[m] - T[k]);
            alpha[j] /= (T[j] - T[k]);
         }
      }
   }

   // compute other components of beta
   for(j = 0; j <= m; j++)
   {  if( j != m-1 )
      {  // compute beta[j]
         beta[j] = one / (T[j] - T[m-1]);
         for(k = 0; k <= m; k++)
         {  if( k != j && k != m-1 )
            {  beta[j] *= (T[m-1] - T[k]);
               beta[j] /= (T[j] - T[k]);
            }
         }
      }
   }

   // evaluate f(T[m-1], x_{m-1} )
   for(i = 0; i < n; i++)
      x_m[i] = X[(m-1) * n + i];
   F.Ode(T[m-1], x_m, f);

   // solve for x_m^0
   for(i = 0; i < n; i++)
   {  x_m[i] =  f[i];
      for(j = 0; j < m; j++)
         x_m[i] -= beta[j] * X[j * n + i];
      x_m[i] /= beta[m];
   }
   x_m0 = x_m;

   // evaluate partial w.r.t x of f(T[m], x_m^0)
   F.Ode_dep(T[m], x_m, f_x);

   // compute the matrix A = ( alpha[m] * I - f_x )
   for(i = 0; i < n; i++)
   {  for(j = 0; j < n; j++)
         A[i * n + j]  = - f_x[i * n + j];
      A[i * n + i] += alpha[m];
   }

   // LU factor (and overwrite) the matrix A
   CppAD::vector<size_t> ip(n) , jp(n);
# ifndef NDEBUG
   int sign =
# endif
   LuFactor(ip, jp, A);
   CPPAD_ASSERT_KNOWN(
      sign != 0,
      "OdeGear: step size is to large"
   );

   // Iterations of Newton's method
   for(k = 0; k < 3; k++)
   {
      // only evaluate f( T[m] , x_m ) keep f_x during iteration
      F.Ode(T[m], x_m, f);

      // b = f + f_x x_m - alpha[0] x_0 - ... - alpha[m-1] x_{m-1}
      for(i = 0; i < n; i++)
      {  b[i]         = f[i];
         for(j = 0; j < n; j++)
            b[i]         -= f_x[i * n + j] * x_m[j];
         for(j = 0; j < m; j++)
            b[i] -= alpha[j] * X[ j * n + i ];
      }
      LuInvert(ip, jp, A, b);
      x_m = b;
   }

   // return estimate for x( t[k] ) and the estimated error bound
   for(i = 0; i < n; i++)
   {  X[m * n + i] = x_m[i];
      e[i]         = x_m[i] - x_m0[i];
      if( e[i] < zero )
         e[i] = - e[i];
   }
}

} // End CppAD namespace

# endif
