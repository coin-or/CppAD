# ifndef CPPAD_EXAMPLE_ABS_NORMAL_ABS_EVAL_HPP
# define CPPAD_EXAMPLE_ABS_NORMAL_ABS_EVAL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin abs_eval}
{xrst_spell
   tilde
}
abs_normal: Evaluate First Order Approximation
##############################################

Syntax
******
*g_tilde* = ``abs_eval`` ( *n* , *m* , *s* , *g_hat* , *g_jac* , *delta_x* )

Prototype
*********
{xrst_literal
   // BEGIN PROTOTYPE
   // END PROTOTYPE
}

Source
******
This following is a link to the source code for this example:
:ref:`abs_eval.hpp-name` .

Purpose
*******
Given a current that abs-normal representation at a point
:math:`\hat{x} \in \B{R}^n`,
and a :math:`\Delta x \in \B{R}^n`,
this routine evaluates the abs-normal
:ref:`approximation for f(x)<abs_normal_fun@Abs-normal Approximation@Approximating f(x)>`
where :math:`x = \hat{x} + \Delta x`.

Vector
******
The type *Vector* is a
simple vector with elements of type ``double`` .

f
*
We use the notation *f* for the original function; see
:ref:`abs_normal_fun@f` .

n
*
This is the dimension of the domain space for *f* ; see
:ref:`abs_normal_fun@f@n` .

m
*
This is the dimension of the range space for *f* ; see
:ref:`abs_normal_fun@f@m` .

s
*
This is the number of absolute value terms in *f* ; see

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

delta_x
*******
This vector has size *n* and is the difference
:math:`\Delta x = x - \hat{x}`,
where :math:`x` is the point that we are approximating :math:`f(x)`.

g_tilde
*******
This vector has size *m* + *s* and is a the
first order approximation for
:ref:`abs_normal_fun@g`
that corresponds to the point
:math:`x = \hat{x} + \Delta x` and :math:`u = a(x)`.
{xrst_toc_hidden
   example/abs_normal/abs_eval.cpp
   example/abs_normal/abs_eval.xrst
}
Example
*******
The file :ref:`abs_eval.cpp-name` contains an example and test of
``abs_eval`` .

{xrst_end abs_eval}
-----------------------------------------------------------------------------
*/

// BEGIN C++
namespace CppAD { // BEGIN_CPPAD_NAMESPACE
// BEGIN PROTOTYPE
template <class Vector>
Vector abs_eval(
   size_t        n       ,
   size_t        m       ,
   size_t        s       ,
   const Vector& g_hat   ,
   const Vector& g_jac   ,
   const Vector& delta_x )
// END PROTOTYPE
{  using std::fabs;
   //
   CPPAD_ASSERT_KNOWN(
      size_t(delta_x.size()) == n,
      "abs_eval: size of delta_x not equal to n"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(g_hat.size()) == m + s,
      "abs_eval: size of g_hat not equal to m + s"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(g_jac.size()) == (m + s) * (n + s),
      "abs_eval: size of g_jac not equal to (m + s)*(n + s)"
   );
# ifndef NDEBUG
   // Check that partial_u z(x, u) is strictly lower triangular
   for(size_t i = 0; i < s; i++)
   {  for(size_t j = i; j < s; j++)
      {  // index in g_jac of partial of z_i w.r.t u_j
         // (note that g_jac has n + s elements in each row)
         size_t index = (m + i) * (n + s) + (n + j);
         CPPAD_ASSERT_KNOWN(
            g_jac[index] == 0.0,
            "abs_eval: partial z_i w.r.t u_j non-zero for i <= j"
         );
      }
   }
# endif
   // return value
   Vector g_tilde(m + s);
   //
   // compute z_tilde, the last s components of g_tilde
   for(size_t i = 0; i < s; i++)
   {  // start at z_hat_i
      g_tilde[m + i] = g_hat[m + i];
      // contribution for change x
      for(size_t j = 0; j < n; j++)
      {  // index in g_jac of partial of z_i w.r.t x_j
         size_t index = (m + i) * (n + s) + j;
         // add contribution for delta_x_j to z_tilde_i
         g_tilde[m + i] += g_jac[index] * delta_x[j];
      }
      // contribution for change in u_j for j < i
      for(size_t j = 0; j < i; j++)
      {  // approixmation for change in absolute value
         double delta_a_j = fabs(g_tilde[m + j]) - fabs(g_hat[m + j]);
         // index in g_jac of partial of z_i w.r.t u_j
         size_t index = (m + i) * (n + s) + n + j;
         // add constribution for delta_a_j to s_tilde_i
         g_tilde[m + i] += g_jac[index] * delta_a_j;
      }
   }
   //
   // compute y_tilde, the first m components of g_tilde
   for(size_t i = 0; i < m; i++)
   {  // start at y_hat_i
      g_tilde[i] = g_hat[i];
      // contribution for change x
      for(size_t j = 0; j < n; j++)
      {  // index in g_jac of partial of y_i w.r.t x_j
         size_t index = i * (n + s) + j;
         // add contribution for delta_x_j to y_tilde_i
         g_tilde[i] += g_jac[index] * delta_x[j];
      }
      // contribution for change in u_j
      for(size_t j = 0; j < s; j++)
      {  // approximation for change in absolute value
         double delta_a_j = fabs(g_tilde[m + j]) - fabs(g_hat[m + j]);
         // index in g_jac of partial of y_i w.r.t u_j
         size_t index = i * (n + s) + n + j;
         // add constribution for delta_a_j to s_tilde_i
         g_tilde[i] += g_jac[index] * delta_a_j;
      }
   }
   return g_tilde;
}
} // END_CPPAD_NAMESPACE
// END C++

# endif
