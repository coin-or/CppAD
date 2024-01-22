# ifndef CPPAD_EXAMPLE_ABS_NORMAL_QP_INTERIOR_HPP
# define CPPAD_EXAMPLE_ABS_NORMAL_QP_INTERIOR_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin qp_interior}
{xrst_spell
   maxitr
   rl
   sout
   xin
   xout
   yout
}

Solve a Quadratic Program Using Interior Point Method
#####################################################

Syntax
******

| *ok* = ``qp_interior`` (
| *level* , *c* , *C* , *g* , *G* , *epsilon* , *maxitr* , *xin* , *xout* , *yout* , *sout*
| )

Prototype
*********
{xrst_literal
   // BEGIN PROTOTYPE
   // END PROTOTYPE
}

Source
******
This following is a link to the source code for this example:
:ref:`qp_interior.hpp-name` .

Purpose
*******
This routine could be used to create a version of :ref:`abs_min_linear-name`
that solved Quadratic programs (instead of linear programs).

Problem
*******
We are given
:math:`C \in \B{R}^{m \times n}`,
:math:`c \in \B{R}^m`,
:math:`G \in \B{R}^{n \times n}`,
:math:`g \in \B{R}^n`,
where :math:`G` is positive semi-definite
and :math:`G + C^T C` is positive definite.
This routine solves the problem

.. math::

   \begin{array}{rl}
   \R{minimize} &
   \frac{1}{2} x^T G x + g^T x \; \R{w.r.t} \; x \in \B{R}^n
   \\
   \R{subject \; to} &
   C x + c \leq 0
   \end{array}

Vector
******
The type *Vector* is a
simple vector with elements of type ``double`` .

level
*****
This value is zero or one.
If *level*  == 0 ,
no tracing is printed.
If *level*  == 1 ,
a trace of the ``qp_interior`` optimization is printed.

Lower c
*******
This is the vector :math:`c` in the problem.

Upper C
*******
This is a :ref:`row-major<glossary@Row-major Representation>`
of the matrix :math:`C` in the problem.

Lower g
*******
This is the vector :math:`g` in the problem.

Upper G
*******
This is a :ref:`row-major<glossary@Row-major Representation>`
of the matrix :math:`G` in the problem.

epsilon
*******
This argument is the convergence criteria;
see :ref:`qp_interior@KKT Conditions` below.
It must be greater than zero.

maxitr
******
This is the maximum number of newton iterations to try before giving up
on convergence.

xin
***
This argument has size *n* and is the initial point for the algorithm.
It must strictly satisfy the constraints; i.e.,
:math:`C x - c < 0`  for *x* = *xin* .

xout
****
This argument has size is *n* and
the input value of its elements does no matter.
Upon return it is the primal variables corresponding to the problem solution.

yout
****
This argument has size is *m* and
the input value of its elements does no matter.
Upon return it the components of *yout* are all positive
and it is the dual variables corresponding to the program solution.

sout
****
This argument has size is *m* and
the input value of its elements does no matter.
Upon return it the components of *sout* are all positive
and it is the slack variables corresponding to the program solution.

ok
**
If the return value *ok* is true, convergence is obtained; i.e.,

.. math::

   | F_0 (xout , yout, sout) |_\infty \leq epsilon

where :math:`| v |_\infty` is the maximum absolute element
for the vector :math:`v` and :math:`F_\mu (x, y, s)` is defined below.

KKT Conditions
**************
Give a vector :math:`v \in \B{R}^m` we define
:math:`D(v) \in \B{R}^{m \times m}` as the corresponding diagonal matrix.
We also define :math:`1_m \in \B{R}^m` as the vector of ones.
We define
:math:`F_\mu : \B{R}^{n + m + m } \rightarrow \B{R}^{n + m + m}`
by

.. math::

   F_\mu ( x , y , s )
   =
   \left(
   \begin{array}{c}
   g + G x + y^T C             \\
   C x + c + s                           \\
   D(s) D(y) 1_m - \mu 1_m
   \end{array}
   \right)

The KKT conditions for a solution of this problem is
:math:`0 \leq y`,
:math:`0 \leq s`, and
:math:`F_0 (x , y, s) = 0`.

Newton Step
***********
The derivative of :math:`F_\mu` is given by

.. math::

   F_\mu^{(1)} (x, y, s)  =
   \left( \begin{array}{ccc}
   G       & C^T  & 0_{n,m} \\
   C       & 0    & I_{m,m} \\
   0_{m,m} & D(s) & D(y)
   \end{array} \right)

The Newton step solves the following equation for
:math:`\Delta x`, :math:`\Delta y`, and :math:`\Delta z`

.. math::

   F_\mu^{(1)} (x, y, s)
   \left( \begin{array}{c} \Delta x \\ \Delta y \\ \Delta s \end{array} \right)
   =
   - F_\mu (x, y, s)

To simplify notation, we define

.. math::
   :nowrap:

   \begin{eqnarray}
   r_x (x, y, s) & = & g + G x + y^T C \\
   r_y (x, y, s) & = & C x + c + s          \\
   r_s (x, y, s) & = & D(s) D(y) 1_m - \mu 1_m
   \end{eqnarray}

It follows that

.. math::

   \left( \begin{array}{ccc}
   G       & C^T  & 0_{n,m} \\
   C       & 0    & I_{m,m} \\
   0_{m,m} & D(s) & D(y)
   \end{array} \right)
   \left( \begin{array}{c} \Delta x \\ \Delta y \\ \Delta s \end{array} \right)
   =
   -
   \left( \begin{array}{c}
      r_x (x, y, s) \\
      r_y (x, y, s) \\
      r_s (x, y, s)
   \end{array} \right)

Elementary Row Reduction
========================
Subtracting :math:`D(y)` times the second row from the third row
we obtain:

.. math::

   \left( \begin{array}{ccc}
   G        & C^T  & 0_{n,m} \\
   C        & 0    & I_{m,m} \\
   - D(y) C & D(s) & 0_{m,m}
   \end{array} \right)
   \left( \begin{array}{c} \Delta x \\ \Delta y \\ \Delta s \end{array} \right)
   =
   -
   \left( \begin{array}{c}
      r_x (x, y, s) \\
      r_y (x, y, s) \\
      r_s (x, y, s) - D(y) r_y(x, y, s)
   \end{array} \right)

Multiplying the third row by :math:`D(s)^{-1}` we obtain:

.. math::

   \left( \begin{array}{ccc}
   G          & C^T     & 0_{n,m} \\
   C          & 0       & I_{m,m} \\
   - D(y/s) C & I_{m,m} & 0_{m,m}
   \end{array} \right)
   \left( \begin{array}{c} \Delta x \\ \Delta y \\ \Delta s \end{array} \right)
   =
   -
   \left( \begin{array}{c}
      r_x (x, y, s) \\
      r_y (x, y, s) \\
      D(s)^{-1} r_s (x, y, s) - D(y/s) r_y(x, y, s)
   \end{array} \right)

where :math:`y/s` is the vector in :math:`\B{R}^m` defined by
:math:`(y/s)_i = y_i / s_i`.
Subtracting :math:`C^T` times the third row from the first row we obtain:

.. math::

   \left( \begin{array}{ccc}
   G + C^T D(y/s) C & 0_{n,m} & 0_{n,m} \\
   C                & 0       & I_{m,m} \\
   - D(y/s) C       & I_{m,m} & 0_{m,m}
   \end{array} \right)
   \left( \begin{array}{c} \Delta x \\ \Delta y \\ \Delta s \end{array} \right)
   =
   -
   \left( \begin{array}{c}
      r_x (x, y, s)
         - C^T D(s)^{-1} \left[ r_s (x, y, s) - D(y) r_y(x, y, s) \right] \\
      r_y (x, y, s) \\
      D(s)^{-1} r_s (x, y, s) - D(y/s) r_y(x, y, s)
   \end{array} \right)

Solution
********
It follows that :math:`G + C^T D(y/s) C` is invertible and
we can determine :math:`\Delta x` by solving the equation

.. math::

   [ G + C^T D(y/s) C ] \Delta x
   =
   C^T D(s)^{-1} \left[ r_s (x, y, s) - D(y) r_y(x, y, s) \right] - r_x (x, y, s)

Given :math:`\Delta x` we have that

.. math::

   \Delta s = - r_y (x, y, s) - C \Delta x

.. math::

   \Delta y =
   D(s)^{-1}[ D(y) r_y(x, y, s) - r_s (x, y, s) + D(y) C \Delta x ]

{xrst_toc_hidden
   example/abs_normal/qp_interior.cpp
   example/abs_normal/qp_interior.xrst
}
Example
*******
The file :ref:`qp_interior.cpp-name` contains an example and test of
``qp_interior`` .

{xrst_end qp_interior}
-----------------------------------------------------------------------------
*/
# include <cmath>
# include <cppad/utility/lu_solve.hpp>
# include "abs_print_mat.hpp"

// BEGIN C++
namespace {
   // ------------------------------------------------------------------------
   template <class Vector>
   double qp_interior_max_abs(const Vector& v)
   {  double max_abs = 0.0;
      for(size_t j = 0; j < size_t(v.size()); j++)
         max_abs = std::max( max_abs, std::fabs(v[j]) );
      return max_abs;
   }
   // ------------------------------------------------------------------------
   template <class Vector>
   void qp_interior_split(
      const Vector& v, Vector& v_x, Vector& v_y, Vector& v_s
   )
   {  size_t n = v_x.size();
      size_t m = v_y.size();
      CPPAD_ASSERT_UNKNOWN( size_t(v_s.size()) == m );
      CPPAD_ASSERT_UNKNOWN( size_t(v.size()) == n + m + m );
      for(size_t i = 0; i < n; i++)
         v_x[i] = v[i];
      for(size_t i = 0; i < m; i++)
      {  v_y[i] = v[n + i];
         v_s[i] = v[n + m + i];
      }
      return;
   }
   // ------------------------------------------------------------------------
   template <class Vector>
   void qp_interior_join(
      Vector& v, const Vector& v_x, const Vector& v_y, const Vector& v_s
   )
   {  size_t n = v_x.size();
      size_t m = v_y.size();
      CPPAD_ASSERT_UNKNOWN( size_t(v_s.size()) == m );
      CPPAD_ASSERT_UNKNOWN( size_t(v.size()) == n + m + m );
      for(size_t i = 0; i < n; i++)
         v[i] = v_x[i];
      for(size_t i = 0; i < m; i++)
         v[n + i] = v_y[i];
      for(size_t i = 0; i < m; i++)
         v[n + m + i] = v_s[i];
      return;
   }
   // ------------------------------------------------------------------------
   template <class Vector>
   Vector qp_interior_F_0(
      const Vector& c       ,
      const Vector& C       ,
      const Vector& g       ,
      const Vector& G       ,
      const Vector& x       ,
      const Vector& y       ,
      const Vector& s       )
   {  size_t n = g.size();
      size_t m = c.size();
      // compute r_x(x, y, s) = g + G x + y^T C
      Vector r_x(n);
      for(size_t j = 0; j < n; j++)
      {  r_x[j] = g[j];
         for(size_t i = 0; i < n; i++)
            r_x[j] += G[j * n + i] * x[i];
         for(size_t i = 0; i < m; i++)
            r_x[j] += y[i] * C[i * n + j];
      }
      // compute r_y(x, y, s) = C x + c + s
      Vector r_y(m);
      for(size_t i = 0; i < m; i++)
      {  r_y[i] = c[i] + s[i];
         for(size_t j = 0; j < n; j++)
            r_y[i] += C[i * n + j] * x[j];
      }
      // compute r_s(x, y, s) = D(s) * D(y) * 1_m - mu * 1_m
      // where mu = 0
      Vector r_s(m);
      for(size_t i = 0; i < m; i++)
         r_s[i] = s[i] * y[i];
      //
      // combine into one vector
      Vector F_0(n + m + m);
      qp_interior_join(F_0, r_x, r_y, r_s);
      //
      return F_0;
   }
}
//
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN PROTOTYPE
template <class Vector>
bool qp_interior(
   size_t        level   ,
   const Vector& c       ,
   const Vector& C       ,
   const Vector& g       ,
   const Vector& G       ,
   double        epsilon ,
   size_t        maxitr  ,
   const Vector& xin     ,
   Vector&       xout    ,
   Vector&       yout    ,
   Vector&       sout    )
// END PROTOTYPE
{  size_t m = c.size();
   size_t n = g.size();
   CPPAD_ASSERT_KNOWN(
      level <= 1,
      "qp_interior: level is greater than one"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(C.size()) == m * n,
      "qp_interior: size of C is not m * n"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(G.size()) == n * n,
      "qp_interior: size of G is not n * n"
   );
   if( level > 0 )
   {  std::cout << "start qp_interior\n";
      CppAD::abs_print_mat("c", m, 1, c);
      CppAD::abs_print_mat("C", m, n, C);
      CppAD::abs_print_mat("g", n, 1, g);
      CppAD::abs_print_mat("G", n, n, G);
      CppAD::abs_print_mat("xin", n, 1, xin);
   }
   //
   // compute the maximum absolute element of the problem vectors and matrices
   double max_element = 0.0;
   for(size_t i = 0; i < size_t(C.size()); i++)
      max_element = std::max(max_element , std::fabs(C[i]) );
   for(size_t i = 0; i < size_t(c.size()); i++)
      max_element = std::max(max_element , std::fabs(c[i]) );
   for(size_t i = 0; i < size_t(G.size()); i++)
      max_element = std::max(max_element , std::fabs(G[i]) );
   for(size_t i = 0; i < size_t(g.size()); i++)
      max_element = std::max(max_element , std::fabs(g[i]) );
   //
   double mu = 1e-1 * max_element;
   //
   if( max_element == 0.0 )
   {  if( level > 0 )
         std::cout << "end qp_interior: line_search failed\n";
      return false;
   }
   //
   // initialize x, y, s
   xout = xin;
   for(size_t i = 0; i < m; i++)
   {  double sum = c[i];
      for(size_t j = 0; j < n; j++)
         sum += C[ i * n + j ] * xout[j];
      if( sum > 0.0 )
      {  if( level > 0 ) std::cout <<
            "end qp_interior: xin is not in interior of feasible set\n";
         return false;
      }
      //
      sout[i] = std::sqrt(mu);
      yout[i] = std::sqrt(mu);
   }
   // ----------------------------------------------------------------------
   // initialie F_0(xout, yout, sout)
   Vector F_0       = qp_interior_F_0(c, C, g, G, xout, yout, sout);
   double F_max_abs = qp_interior_max_abs( F_0 );
   for(size_t itr = 0; itr <= maxitr; itr++)
   {
      // check for convergence
      if( F_max_abs <= epsilon )
      {  if( level > 0 )
            std::cout << "end qp_interior: ok = true\n";
         return true;
      }
      if( itr == maxitr )
      {  if( level > 0 ) std::cout <<
            "end qp_interior: max # iterations without convergence\n";
         return false;
      }
      //
      // compute F_mu(xout, yout, sout)
      Vector F_mu  = F_0;
      for(size_t i = 0; i < m; i++)
         F_mu[n + m + i] -= mu;
      //
      // r_x, r_y, r_s (xout, yout, sout)
      Vector r_x(n), r_y(m), r_s(m);
      qp_interior_split(F_mu, r_x, r_y, r_s);
      //
      // tmp_m = D(s)^{-1} * [ r_s - D(y) r_y ]
      Vector tmp_m(m);
      for(size_t i = 0; i < m; i++)
         tmp_m[i]  = ( r_s[i] - yout[i] * r_y[i] ) / sout[i];
      //
      // right_x = C^T * D(s)^{-1} * [ r_s - D(y) r_y ] - r_x
      Vector right_x(n);
      for(size_t j = 0; j < n; j++)
      {  right_x[j] = 0.0;
         for(size_t i = 0; i < m; i++)
            right_x[j] += C[ i * n + j ] * tmp_m[i];
         right_x[j] -= r_x[j];
      }
      //
      // Left_x = G + C^T * D(y / s) * C
      Vector Left_x = G;
      for(size_t i = 0; i < n; i++)
      {  for(size_t j = 0; j < n; j++)
         {  for(size_t k = 0; k < m; k++)
            {  double y_s = yout[k] / sout[k];
               Left_x[ i * n + j] += C[k * n + j] * y_s * C[k * n + i];
            }
         }
      }
      // delta_x
      Vector delta_x(n);
      double logdet;
      LuSolve(n, 1, Left_x, right_x, delta_x, logdet);
      //
      // C_delta_x = C * delta_x
      Vector C_delta_x(m);
      for(size_t i = 0; i < m; i++)
      {  C_delta_x[i] = 0.0;
         for(size_t j = 0; j < n; j++)
            C_delta_x[i] += C[ i * n + j ] * delta_x[j];
      }
      //
      // delta_y = D(s)^-1 * [D(y) * r_y - r_s + D(y) * C * delta_x]
      Vector delta_y(m);
      for(size_t i = 0; i < m; i++)
      {  delta_y[i] = yout[i] * r_y[i] - r_s[i] + yout[i] * C_delta_x[i];
         delta_y[i] /= sout[i];
      }
      // delta_s = - r_y - C * delta_x
      Vector delta_s(m);
      for(size_t i = 0; i < m; i++)
         delta_s[i] = - r_y[i] - C_delta_x[i];
      //
      // delta_xys
      Vector delta_xys(n + m + m);
      qp_interior_join(delta_xys, delta_x, delta_y, delta_s);
      // -------------------------------------------------------------------
      //
      // The initial derivative in direction  Delta_xys is equal to
      // the negative of the norm square of F_mu
      //
      // line search parameter lam
      Vector x(n), y(m), s(m);
      double  lam = 2.0;
      bool lam_ok = false;
      while( ! lam_ok && lam > 1e-5 )
      {  lam = lam / 2.0;
         for(size_t j = 0; j < n; j++)
            x[j] = xout[j] + lam * delta_xys[j];
         lam_ok = true;
         for(size_t i = 0; i < m; i++)
         {  y[i] = yout[i] + lam * delta_xys[n + i];
            s[i] = sout[i] + lam * delta_xys[n + m + i];
            lam_ok &= s[i] > 0.0 && y[i] > 0.0;
         }
         if( lam_ok )
         {  Vector F_mu_tmp = qp_interior_F_0(c, C, g, G, x, y, s);
            for(size_t i = 0; i < m; i++)
               F_mu_tmp[n + m + i] -= mu;
            // avoid cancellation roundoff in difference of norm squared
            // |v + dv|^2         = v^T * v + 2 * v^T * dv + dv^T * dv
            // |v + dv|^2 - |v|^2 =           2 * v^T * dv + dv^T * dv
            double F_norm_sq    = 0.0;
            double diff_norm_sq = 0.0;
            for(size_t i = 0; i < n + m + m; i++)
            {  double dv     = F_mu_tmp[i] - F_mu[i];
               F_norm_sq    += F_mu[i] * F_mu[i];
               diff_norm_sq += 2.0 * F_mu[i] * dv + dv * dv;
            }
            lam_ok &= diff_norm_sq < - lam * F_norm_sq / 4.0;
         }
      }
      if( ! lam_ok )
      {  if( level > 0 )
            std::cout << "end qp_interior: line search failed\n";
         return false;
      }
      //
      // update current solution
      xout = x;
      yout = y;
      sout = s;
      //
      // updage F_0
      F_0       = qp_interior_F_0(c, C, g, G, xout, yout, sout);
      F_max_abs = qp_interior_max_abs( F_0 );
      //
      // update mu
      if( F_max_abs <= 1e1 *  mu )
         mu = mu / 1e2;
      if( level > 0 )
      {  std::cout << "itr = " << itr
            << ", mu = " << mu
            << ", lam = " << lam
            << ", F_max_abs = " << F_max_abs << "\n";
         abs_print_mat("xout", 1, n, xout);
      }
   }
   if( level > 0 )
      std::cout << "end qp_interior: progam error\n";
   return false;
}
} // END_CPPAD_NAMESPACE
// END C++

# endif
