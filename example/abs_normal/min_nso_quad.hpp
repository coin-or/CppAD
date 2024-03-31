# ifndef CPPAD_EXAMPLE_ABS_NORMAL_MIN_NSO_QUAD_HPP
# define CPPAD_EXAMPLE_ABS_NORMAL_MIN_NSO_QUAD_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin min_nso_quad}
{xrst_spell
   dbl
   maxitr
}
Non-Smooth Optimization Using Abs-normal Quadratic Approximations
#################################################################

Syntax
******

| *ok* = ``min_nso_quad`` (
| |tab| *level* , *f* , *g* , *a* , *epsilon* , *maxitr* , *b_in* , *x_in* , *x_out*
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
:ref:`min_nso_quad.hpp-name` .

Purpose
*******
Given a current that abs-normal representation
:ref:`abs_normal_fun@g` , :ref:`abs_normal_fun@a` ,
for a function :math:`f(x)`,
this routine minimizes :math:`f(x)`.

DblVector
*********
is a :ref:`SimpleVector-name` class with elements of type ``double`` .

SizeVector
**********
is a :ref:`SimpleVector-name` class with elements of type ``size_t`` .

level
*****
This value is less that or equal 5.
If *level*  == 0 ,
no tracing of the optimization is printed.
If *level*  >= 1 ,
a trace of each iteration of ``min_nso_quad`` is printed.
If *level*  >= 2 ,
a trace of each iteration of the ``abs_min_quad`` sub-problem is printed.
If *level*  >= 3 ,
a trace of the :ref:`qp_box-name` sub-problem is printed.
If *level*  >= 4 ,
a trace of the :ref:`qp_interior-name` sub-problem is printed.

f
*
This is the original function for the abs-normal form; see
:ref:`abs_normal_fun@f` .

n
=
We use *n* to denote the dimension of the domain for *f* ; i.e.,
*f* . ``Domain`` () .

m
=
We use *m* to denote the dimension of the range for *f* ; i.e.,
*f* . ``Range`` () .
This must be equal to one.

s
=
We use
:ref:`abs_normal_fun@f@s` to denote the number absolute terms in *f* .

g
*
This is the function :ref:`abs_normal_fun@g`
in the abs-normal representation of *f* .

a
*
This is the function :ref:`abs_normal_fun@a`
in the abs-normal representation of *f* .

epsilon
*******
This is a vector with size 2.
The value *epsilon* [0] is convergence criteria in terms
of the infinity norm of the difference of *x_out*
between iterations.
The value *epsilon* [1] is convergence criteria in terms
of the derivative of :math:`f(x)`.
This derivative is actually the average of the directional derivative
in the direction of the sub-problem minimizer.

maxitr
******
This is a vector with size 3.
The value *maxitr* [0] is the maximum number of
``min_nso_quad`` iterations to try before giving up on convergence.
The value *maxitr* [1] is the maximum number of iterations in the
``abs_min_quad`` sub-problem.
The value *maxitr* [2] is the maximum number of iterations in
the :ref:`qp_interior<qp_interior@maxitr>` sub-problems.

b_in
****
This the initial bound on the trust region size.
To be specific, if :math:`b` is the current trust region size,
at each iteration affine approximation is minimized with respect to
:math:`\Delta x` and subject to

.. math::

   -b \leq \Delta x_j \leq b

for *j* = 0 , ..., *n* ``-1`` .
It must hold that *b_in* > *epsilon* [0] .

x_in
****
This vector *x_out* has size *n* .
It is the starting point for the optimization procedure; i.e.,
the ``min_nso_quad`` iterations.

x_out
*****
This vector *x_out* has size *n* .
The input value of its elements does not matter.
Upon return,
it is the approximate minimizer
of the abs-normal approximation for :math:`f(x)` over the trust region
is :math:`x = \hat{x} + \Delta x`.
{xrst_toc_hidden
   example/abs_normal/min_nso_quad.cpp
   example/abs_normal/min_nso_quad.xrst
}
Example
*******
The file :ref:`min_nso_quad.cpp-name` contains an example and test of
``min_nso_quad`` .

{xrst_end min_nso_quad}
-----------------------------------------------------------------------------
*/
# include <cppad/cppad.hpp>
# include "abs_min_quad.hpp"
# include "abs_eval.hpp"

// BEGIN C++
namespace {
   CPPAD_TESTVECTOR(double) min_nso_quad_join(
      const CPPAD_TESTVECTOR(double)& x ,
      const CPPAD_TESTVECTOR(double)& u )
   {  size_t n = x.size();
      size_t s = u.size();
      CPPAD_TESTVECTOR(double) xu(n + s);
      for(size_t j = 0; j < n; j++)
         xu[j] = x[j];
      for(size_t j = 0; j < s; j++)
         xu[n + j] = u[j];
      return xu;
   }
}

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN PROTOTYPE
template <class DblVector, class SizeVector>
bool min_nso_quad(
   size_t           level     ,
   ADFun<double>&   f         ,
   ADFun<double>&   g         ,
   ADFun<double>&   a         ,
   const DblVector& epsilon   ,
   SizeVector       maxitr    ,
   double           b_in      ,
   const DblVector& x_in      ,
   DblVector&       x_out     )
// END PROTOTYPE
{
   using std::fabs;
   //
   // number of absolute value terms
   size_t s  = a.Range();
   //
   // size of domain for f
   size_t n  = f.Domain();
   //
   // size of range space for f
   size_t m = f.Range();
   //
   CPPAD_ASSERT_KNOWN( g.Domain() == n + s,
      "min_nso_quad: (g, a) is not an abs-normal for for f"
   );
   CPPAD_ASSERT_KNOWN( g.Range() == m + s,
      "min_nso_quad: (g, a) is not an abs-normal for for f"
   );
   CPPAD_ASSERT_KNOWN(
      level <= 5,
      "min_nso_quad: level is not less that or equal 5"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(epsilon.size()) == 2,
      "min_nso_quad: size of epsilon not equal to 2"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(maxitr.size()) == 3,
      "min_nso_quad: size of maxitr not equal to 3"
   );
   CPPAD_ASSERT_KNOWN(
      g.Domain() > s && g.Range() > s,
      "min_nso_quad: g, a is not an abs-normal representation"
   );
   CPPAD_ASSERT_KNOWN(
      m == 1,
      "min_nso_quad: m is not equal to 1"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(x_in.size()) == n,
      "min_nso_quad: size of x_in not equal to n"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(x_out.size()) == n,
      "min_nso_quad: size of x_out not equal to n"
   );
   CPPAD_ASSERT_KNOWN(
      epsilon[0] < b_in,
      "min_nso_quad: b_in <= epsilon[0]"
   );
   if( level > 0 )
   {  std::cout << "start min_nso_quad\n";
      std::cout << "b_in = " << b_in << "\n";
      CppAD::abs_print_mat("x_in", n, 1, x_in);
   }
   // level in abs_min_quad sub-problem
   size_t level_tilde = 0;
   if( level > 0 )
      level_tilde = level - 1;
   //
   // maxitr in abs_min_quad sub-problem
   SizeVector maxitr_tilde(2);
   maxitr_tilde[0] = maxitr[1];
   maxitr_tilde[1] = maxitr[2];
   //
   // epsilon in abs_min_quad sub-problem
   DblVector eps_tilde(2);
   eps_tilde[0] = epsilon[0] / 10.;
   eps_tilde[1] = epsilon[1] / 10.;
   //
   // current bound
   double b_cur = b_in;
   //
   // initilaize the current x
   x_out = x_in;
   //
   // value of a(x) at current x
   DblVector a_cur = a.Forward(0, x_out);
   //
   // (x_out, a_cur)
   DblVector xu_cur = min_nso_quad_join(x_out, a_cur);
   //
   // value of g[ x_cur, a_cur ]
   DblVector g_cur = g.Forward(0, xu_cur);
   //
   for(size_t itr = 0; itr < maxitr[0]; itr++)
   {
      // Jacobian of g[ x_cur, a_cur ]
      DblVector g_jac = g.Jacobian(xu_cur);
      //
      // Hessian at x_cur
      DblVector f_hes = f.Hessian(x_out, 0);
      //
      // bound in abs_min_quad sub-problem
      DblVector bound_tilde(n);
      for(size_t j = 0; j < n; j++)
         bound_tilde[j] = b_cur;
      //
      DblVector delta_x(n);
      bool ok = abs_min_quad(
         level_tilde, n, m, s,
         g_cur, g_jac, f_hes, bound_tilde, eps_tilde, maxitr_tilde, delta_x
      );
      if( ! ok )
      {  if( level > 0 )
            std::cout << "end min_nso_quad: abs_min_quad failed\n";
         return false;
      }
      //
      // new candidate value for x
      DblVector x_new(n);
      double max_delta_x = 0.0;
      for(size_t j = 0; j < n; j++)
      {  x_new[j] = x_out[j] + delta_x[j];
         max_delta_x = std::max(max_delta_x, std::fabs( delta_x[j] ) );
      }
      //
      if( max_delta_x < 0.75 * b_cur && max_delta_x < epsilon[0] )
      {  if( level > 0 )
            std::cout << "end min_nso_quad: delta_x is near zero\n";
         return true;
      }
      // value of abs-normal approximation at minimizer
      DblVector g_tilde = CppAD::abs_eval(n, m, s, g_cur, g_jac, delta_x);
      //
      double derivative = (g_tilde[0] - g_cur[0]) / max_delta_x;
      CPPAD_ASSERT_UNKNOWN( derivative <= 0.0 )
      if( - epsilon[1] < derivative )
      {  if( level > 0 )
            std::cout << "end min_nso_quad: derivative near zero\n";
         return true;
      }
      //
      // value of a(x) at new x
      DblVector a_new = a.Forward(0, x_new);
      //
      // (x_new, a_new)
      DblVector xu_new = min_nso_quad_join(x_new, a_new);
      //
      // value of g[ x_new, a_new ]
      DblVector g_new = g.Forward(0, xu_new);
      //
      // check for descent of objective
      double rate_new = (g_new[0] - g_cur[0]) / max_delta_x;
      if( - epsilon[1] < rate_new )
      {  // did not get sufficient descent
         b_cur /= 2.0;
         if( level > 0 )
            std::cout << "itr = " << itr
            << ", rate_new = " << rate_new
            << ", b_cur = " << b_cur << "\n";
         //
      }
      else
      {  // got sufficient descent so accept candidate for x
         x_out  = x_new;
         a_cur  = a_new;
         g_cur  = g_new;
         xu_cur = xu_new;
         //
         if( level >  0 )
         {  std::cout << "itr = " << itr
            << ", derivative = "<< derivative
            << ", max_delta_x = "<< max_delta_x
            << ", objective = " << g_cur[0] << "\n";
            abs_print_mat("x_out", n, 1, x_out);
         }
      }
   }
   if( level > 0 )
      std::cout << "end min_nso_quad: maximum number of iterations exceeded\n";
   return false;
}
} // END_CPPAD_NAMESPACE
// END C++

# endif
