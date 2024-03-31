# ifndef CPPAD_EXAMPLE_ABS_NORMAL_ABS_MIN_QUAD_HPP
# define CPPAD_EXAMPLE_ABS_NORMAL_ABS_MIN_QUAD_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin abs_min_quad}
{xrst_spell
   dbl
   lll
   maxitr
}
abs_normal: Minimize a Linear Abs-normal Approximation
######################################################

Syntax
******

| *ok* = ``abs_min_quad`` (
| |tab| *level* , *n* , *m* , *s* ,
| |tab| *g_hat* , *g_jac* , *hessian* , *bound* , *epsilon* , *maxitr* , *delta_x*
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
:ref:`abs_min_quad.hpp-name` .

Purpose
*******
We are given a point :math:`\hat{x} \in \B{R}^n` and
use the notation :math:`\tilde{f} (x)` for the abs-normal
:ref:`approximation for f(x)<abs_normal_fun@Abs-normal Approximation@Approximating f(x)>`
near :math:`\hat{x}`.
We are also given a vector :math:`b \in \B{R}_+^n`
and a positive definite matrix :math:`H \in \B{R}^{n \times n}`.
This routine solves the problem

.. math::

   \begin{array}{lll}
   \R{minimize} &
      \Delta x^T H \Delta x / 2 + \tilde{f}( \hat{x} + \Delta x ) &
      \R{w.r.t} \; \Delta x \in \B{R}^n
   \\
   \R{subject \; to} & | \Delta x_j | \leq b_j & j = 0 , \ldots , n-1
   \end{array}

DblVector
*********
is a :ref:`SimpleVector-name` class with elements of type ``double`` .

SizeVector
**********
is a :ref:`SimpleVector-name` class with elements of type ``size_t`` .

f
*
We use the notation *f* for the original function; see
:ref:`abs_normal_fun@f` .

level
*****
This value is less that or equal 3.
If *level*  == 0 ,
no tracing of the optimization is printed.
If *level*  >= 1 ,
a trace of each iteration of ``abs_min_quad`` is printed.
If *level*  >= 2 ,
a trace of the :ref:`qp_box-name` sub-problem is printed.
If *level*  >= 3 ,
a trace of the :ref:`qp_interior-name` sub-problem is printed.

n
*
This is the dimension of the domain space for *f* ; see
:ref:`abs_normal_fun@f@n` .

m
*
This is the dimension of the range space for *f* ; see
:ref:`abs_normal_fun@f@m` . This must be one so that :math:`f`
is an objective function.

s
*
This is the number of absolute value terms in *f* ; see
:ref:`abs_normal_fun@f@s` .

g
*
We use the notation *g* for the abs-normal representation of *f* ;
see :ref:`abs_normal_fun@g` .

g_hat
*****
This vector has size *m* + *s* and is the value of
*g* ( *x* , *u* ) at :math:`x = \hat{x}` and :math:`u = a( \hat{x} )`.

g_jac
*****
This vector has size ( *m* + *s* ) * ( *n* + *s* ) and is the Jacobian of
:math:`g(x, u)` at :math:`x = \hat{x}` and :math:`u = a( \hat{x} )`.

hessian
*******
This vector has size *n* * *n* .
It is a :ref:`row-major<glossary@Row-major Representation>` representation
of the matrix :math:`H \in \B{R}^{n \times n}`.

bound
*****
This vector has size *n* and is the vector :math:`b \in \B{R}^n`.
The trust region is defined as the set of :math:`\Delta x` such that

.. math::

   | \Delta x | \leq b_j

for :math:`j = 0 , \ldots , n-1`.

epsilon
*******
The value *epsilon* [0] is convergence criteria in terms
of the infinity norm of the difference of *delta_x*
between iterations.
The value *epsilon* [1] is convergence criteria in terms
of the derivative of the objective; i.e.

.. math::

   \Delta x^T H \Delta x / 2 + \tilde{f}( \hat{x} + \Delta x)

maxitr
******
This is a vector with size 2.
The value *maxitr* [0] is the maximum number of
``abs_min_quad`` iterations to try before giving up on convergence.
The value *maxitr* [1] is the maximum number of iterations in
the :ref:`qp_interior<qp_interior@maxitr>` sub-problems.

delta_x
*******
This vector :math:`\Delta x` has size *n* .
The input value of its elements does not matter.
Upon return,
the approximate minimizer of the objective with respect to the trust region.

Method
******

sigma
=====
We use the notation

.. math::

   \sigma (x) = \R{sign} ( z[ x , a(x) ] )

where
:ref:`abs_normal_fun@a@a(x)` and
:ref:`abs_normal_fun@g@z(x, u)`
are as defined in the abs-normal representation of :math:`f(x)`.

Cutting Planes
==============
At each iteration,
we are given affine functions :math:`p_k (x)`
such that :math:`p_k ( x_k ) = \tilde{f}( x_k )`  and
:math:`p_k^{(1)} ( x_k )` is the derivative :math:`\tilde{f}^{(1)} ( x_k )`
corresponding to :math:`\sigma ( x_k )`.

Iteration
=========
At iteration :math:`k`, we solve the problem

.. math::

   \begin{array}{lll}
   \R{minimize}
   & \Delta x^T H \Delta x / 2 +
      \max \{ p_k ( \hat{x} + \Delta x) \W{:} k = 0 , \ldots , K-1 \}
   & \R{w.r.t} \; \Delta x
   \\
   \R{subject \; to} & - b \leq \Delta x \leq + b
   \end{array}

The solution is the new point :math:`x_K`
at which the new affine approximation
:math:`p_K (x)` is constructed.
This process is iterated until the difference
:math:`x_K - x_{K-1}` is small enough.
{xrst_toc_hidden
   example/abs_normal/abs_min_quad.cpp
   example/abs_normal/abs_min_quad.xrst
}
Example
*******
The file :ref:`abs_min_quad.cpp-name` contains an example and test of
``abs_min_quad`` .

{xrst_end abs_min_quad}
-----------------------------------------------------------------------------
*/
# include <cppad/cppad.hpp>
# include "qp_box.hpp"
# include "abs_eval.hpp"

// BEGIN C++
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN PROTOTYPE
template <class DblVector, class SizeVector>
bool abs_min_quad(
   size_t            level   ,
   size_t            n       ,
   size_t            m       ,
   size_t            s       ,
   const DblVector&  g_hat   ,
   const DblVector&  g_jac   ,
   const DblVector&  hessian ,
   const DblVector&  bound   ,
   const DblVector&  epsilon ,
   const SizeVector& maxitr  ,
   DblVector&        delta_x )
// END PROTOTYPE
{  using std::fabs;
   bool ok    = true;
   double inf = std::numeric_limits<double>::infinity();
   //
   CPPAD_ASSERT_KNOWN(
      level <= 4,
      "abs_min_quad: level is not less that or equal 3"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(epsilon.size()) == 2,
      "abs_min_quad: size of epsilon not equal to 2"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(maxitr.size()) == 2,
      "abs_min_quad: size of maxitr not equal to 2"
   );
   CPPAD_ASSERT_KNOWN(
      m == 1,
      "abs_min_quad: m is not equal to 1"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(delta_x.size()) == n,
      "abs_min_quad: size of delta_x not equal to n"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(bound.size()) == n,
      "abs_min_quad: size of bound not equal to n"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(g_hat.size()) == m + s,
      "abs_min_quad: size of g_hat not equal to m + s"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(g_jac.size()) == (m + s) * (n + s),
      "abs_min_quad: size of g_jac not equal to (m + s)*(n + s)"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(hessian.size()) == n * n,
      "abs_min_quad: size of hessian not equal to n * n"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(bound.size()) == n,
      "abs_min_quad: size of bound is not equal to n"
   );
   if( level > 0 )
   {  std::cout << "start abs_min_quad\n";
      CppAD::abs_print_mat("g_hat", m + s, 1, g_hat);
      CppAD::abs_print_mat("g_jac", m + s, n + s, g_jac);
      CppAD::abs_print_mat("hessian", n, n, hessian);
      CppAD::abs_print_mat("bound", n, 1, bound);
   }
   // partial y(x, u) w.r.t x (J in reference)
   DblVector py_px(n);
   for(size_t j = 0; j < n; j++)
      py_px[ j ] = g_jac[ j ];
   //
   // partial y(x, u) w.r.t u (Y in reference)
   DblVector py_pu(s);
   for(size_t j = 0; j < s; j++)
      py_pu[ j ] = g_jac[ n + j ];
   //
   // partial z(x, u) w.r.t x (Z in reference)
   DblVector pz_px(s * n);
   for(size_t i = 0; i < s; i++)
   {  for(size_t j = 0; j < n; j++)
      {  pz_px[ i * n + j ] = g_jac[ (n + s) * (i + m) + j ];
      }
   }
   // partial z(x, u) w.r.t u (L in reference)
   DblVector pz_pu(s * s);
   for(size_t i = 0; i < s; i++)
   {  for(size_t j = 0; j < s; j++)
      {  pz_pu[ i * s + j ] = g_jac[ (n + s) * (i + m) + n + j ];
      }
   }
   // initailize delta_x
   for(size_t j = 0; j < n; j++)
      delta_x[j] = 0.0;
   //
   // current set of cutting planes
   DblVector C(maxitr[0] * n), c(maxitr[0]);
   //
   // value of abs-normal approximation at x_hat + delta_x
   DblVector g_tilde = CppAD::abs_eval(n, m, s, g_hat, g_jac, delta_x);
   //
   // value of sigma at delta_x = 0; i.e., sign( z(x, u) )
   CppAD::vector<double> sigma(s);
   for(size_t i = 0; i < s; i++)
      sigma[i] = CppAD::sign( g_tilde[m + i] );
   //
   // initial value of the objective
   double obj_cur =  g_tilde[0];
   //
   // initial number of cutting planes
   size_t n_plane = 0;
   //
   if( level > 0 )
   {  std::cout << "obj = " << obj_cur << "\n";
      CppAD::abs_print_mat("delta_x", n, 1, delta_x);
   }
   for(size_t itr = 0; itr < maxitr[0]; itr++)
   {
      // Equation (5), Propostion 3.1 of reference
      // dy_dx = py_px + py_pu * Sigma * (I - pz_pu * Sigma)^-1 * pz_px
      //
      // tmp_ss = I - pz_pu * Sigma
      DblVector tmp_ss(s * s);
      for(size_t i = 0; i < s; i++)
      {  for(size_t j = 0; j < s; j++)
            tmp_ss[i * s + j] = - pz_pu[i * s + j] * sigma[j];
         tmp_ss[i * s + i] += 1.0;
      }
      // tmp_sn = (I - pz_pu * Sigma)^-1 * pz_px
      double logdet;
      DblVector tmp_sn(s * n);
      LuSolve(s, n, tmp_ss, pz_px, tmp_sn, logdet);
      //
      // tmp_sn = Sigma * (I - pz_pu * Sigma)^-1 * pz_px
      for(size_t i = 0; i < s; i++)
      {  for(size_t j = 0; j < n; j++)
            tmp_sn[i * n + j] *= sigma[i];
      }
      // dy_dx = py_px + py_pu * Sigma * (I - pz_pu * Sigma)^-1 * pz_px
      DblVector dy_dx(n);
      for(size_t j = 0; j < n; j++)
      {  dy_dx[j] = py_px[j];
         for(size_t k = 0; k < s; k++)
            dy_dx[j] += py_pu[k] * tmp_sn[ k * n + j];
      }
      //
      // compute derivative of the quadratic term
      DblVector dq_dx(n);
      for(size_t j = 0; j < n; j++)
      {  dq_dx[j] = 0.0;
         for(size_t i = 0; i < n; i++)
            dq_dx[j] += delta_x[i] * hessian[i * n + j];
      }
      //
      // check for case where derivative of objective is zero
      // (in convex case, this is the minimizer)
      bool near_zero = true;
      for(size_t j = 0; j < n; j++)
         near_zero &= std::fabs( dq_dx[j] + dy_dx[j] ) < epsilon[1];
      if( near_zero )
      {  if( level > 0 )
            std::cout << "end abs_min_quad: local derivative near zero\n";
         return true;
      }
      // value of hyperplane at delta_x
      double plane_at_zero = g_tilde[0];
      //
      // value of hyperplane at 0
      for(size_t j = 0; j < n; j++)
         plane_at_zero -= dy_dx[j] * delta_x[j];
      //
      // add a cutting plane with value g_tilde[0] at delta_x
      // and derivative dy_dx
      c[n_plane] = plane_at_zero;
      for(size_t j = 0; j < n; j++)
         C[n_plane * n + j] = dy_dx[j];
      ++n_plane;
      //
      // variables for cutting plane problem are (dx, w)
      // c[i] + C[i,:] * dx <= w
      DblVector c_box(n_plane), C_box(n_plane * (n + 1));
      for(size_t i = 0; i < n_plane; i++)
      {  c_box[i] = c[i];
         for(size_t j = 0; j < n; j++)
            C_box[i * (n+1) + j] = C[i * n + j];
         C_box[i * (n+1) + n] = -1.0;
      }
      //
      // w is the objective
      DblVector g_box(n + 1);
      for(size_t i = 0; i < size_t(c_box.size()); i++)
         g_box[i] = 0.0;
      g_box[n] = 1.0;
      //
      // a_box, b_box
      DblVector a_box(n+1), b_box(n+1);
      for(size_t j = 0; j < n; j++)
      {  a_box[j] = - bound[j];
         b_box[j] = + bound[j];
      }
      a_box[n] = - inf;
      b_box[n] = + inf;
      //
      // initial delta_x in qp_box is zero
      DblVector xin_box(n + 1);
      for(size_t j = 0; j < n; j++)
         xin_box[j] = 0.0;
      // initial w in qp_box is 1 + max_i c[i]
      xin_box[n] = 1.0 + c_box[0];
      for(size_t i = 1; i < n_plane; i++)
         xin_box[n] = std::max( xin_box[n], 1.0 + c_box[i] );
      //
      DblVector hessian_box( (n+1) * (n+1) );
      for(size_t i = 0; i < n+1; i++)
      {  for(size_t j = 0; j < n+1; j++)
         {  if( i == n || j == n )
               hessian_box[i * (n+1) + j] = 0.0;
            else
               hessian_box[i * (n+1) + j] = hessian[i * n + j];
         }
      }
      //
      // solve the cutting plane problem
      DblVector xout_box(n + 1);
      size_t level_box = 0;
      if( level > 0 )
         level_box = level - 1;
      ok &= CppAD::qp_box(
         level_box,
         a_box,
         b_box,
         c_box,
         C_box,
         g_box,
         hessian_box,
         epsilon[1],
         maxitr[1],
         xin_box,
         xout_box
      );
      if( ! ok )
      {  if( level > 0 )
         {  CppAD::abs_print_mat("delta_x", n, 1, delta_x);
            std::cout << "end abs_min_quad: qp_box failed\n";
         }
         return false;
      }
      DblVector delta_new(n);
      for(size_t j = 0; j < n; j++)
         delta_new[j] = xout_box[j];
      //
      // check for convergence
      double max_diff = 0.0;
      for(size_t j = 0; j < n; j++)
      {  double diff = delta_x[j] - delta_new[j];
         max_diff    = std::max( max_diff, std::fabs(diff) );
      }
      //
      // new value of the objective
      DblVector g_new   = CppAD::abs_eval(n, m, s, g_hat, g_jac, delta_new);
      double    obj_new = g_new[0];
      for(size_t i = 0; i < n; i++)
      {  for(size_t j = 0; j < n; j++)
            obj_new += delta_new[i] * hessian[i * n + j] * delta_new[j];
      }
      g_tilde = g_new;
      obj_cur = obj_new;
      delta_x = delta_new;
      //
      if( level > 0 )
      {  std::cout << "itr = " << itr << ", max_diff = " << max_diff
            << ", obj_cur = " << obj_cur << "\n";
         CppAD::abs_print_mat("delta_x", n, 1, delta_x);
      }
      //
      // value of sigma at new delta_x; i.e., sign( z(x, u) )
      for(size_t i = 0; i < s; i++)
         sigma[i] = CppAD::sign( g_tilde[m + i] );
      //
      if( max_diff < epsilon[0] )
      {  if( level > 0 )
            std::cout << "end abs_min_quad: change in delta_x near zero\n";
         return true;
      }
   }
   if( level > 0 )
      std::cout << "end abs_min_quad: maximum number of iterations exceeded\n";
   return false;
}
} // END_CPPAD_NAMESPACE
// END C++

# endif
