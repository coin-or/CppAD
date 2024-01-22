# ifndef CPPAD_EXAMPLE_ABS_NORMAL_QP_BOX_HPP
# define CPPAD_EXAMPLE_ABS_NORMAL_QP_BOX_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin qp_box}
{xrst_spell
   maxitr
   rl
   xin
   xout
}
abs_normal: Solve a Quadratic Program With Box Constraints
##########################################################

Syntax
******

| *ok* = ``qp_box`` (
| |tab| *level* , *a* , *b* , *c* , *C* , *g* , *G* , *epsilon* , *maxitr* , *xin* , *xout*
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
:ref:`qp_box.hpp-name` .

Purpose
*******
This routine could be used to create a version of :ref:`abs_min_linear-name`
that solved quadratic programs (instead of linear programs).

Problem
*******
We are given
:math:`a \in \B{R}^n`,
:math:`b \in \B{R}^n`,
:math:`c \in \B{R}^m`,
:math:`C \in \B{R}^{m \times n}`,
:math:`g \in \B{R}^n`,
:math:`G \in \B{R}^{n \times n}`,
where :math:`G` is positive semi-definite.
This routine solves the problem

.. math::

   \begin{array}{rl}
   \R{minimize} &
   \frac{1}{2} x^T G x + g^T x \; \R{w.r.t} \; x \in \B{R}^n
   \\
   \R{subject \; to} & C x + c \leq 0 \; \R{and} \; a \leq x \leq b
   \end{array}

The matrix :math:`G + C^T C` must be positive definite on components
of the vector :math:`x` where the lower limit minus infinity
and the upper limit is plus infinity; see *a* and *b* below.

Vector
******
The type *Vector* is a
simple vector with elements of type ``double`` .

level
*****
This value is less that or equal two.
If *level*  == 0 ,
no tracing is printed.
If *level*  >= 1 ,
a trace of the ``qp_box`` operations is printed.
If *level*  == 2 ,
a trace of the :ref:`qp_interior-name` sub-problem is printed.

a
*
This is the vector of lower limits for :math:`x` in the problem.
If *a* [ *j* ] is minus infinity, there is no lower limit
for :math:`x_j`.

b
*
This is the vector of upper limits for :math:`x` in the problem.
If *a* [ *j* ] is plus infinity, there is no upper limit
for :math:`x_j`.

Lower c
*******
This is the value of the inequality constraint function at :math:`x = 0`.

Upper C
*******
This is a :ref:`row-major<glossary@Row-major Representation>` representation
of thee the inequality constraint matrix :math:`C`.

Lower g
*******
This is the gradient of the objective function.

Upper G
*******
This is a row-major representation of the Hessian of the objective function.
For :math:`j = 0 , \ldots , n-1`,
:math:`- \infty < a_j` or
:math:`b_j < + \infty` or
:math:`G_{j,j} > 0.0`.

epsilon
*******
This argument is the convergence criteria;
see :ref:`qp_box@KKT Conditions` below.
It must be greater than zero.

maxitr
******
This is the maximum number of
:ref:`qp_interior-name` iterations to try before giving up
on convergence.

xin
***
This argument has size *n* and is the initial point for the algorithm.
It must strictly satisfy the constraints; i.e.,

   *a* < *xin* , *xin* < *b* , *C* * *xin* ``-`` *c*  < 0

xout
****
This argument has size is *n* and
the input value of its elements does no matter.
Upon return it is the primal variables
:math:`x` corresponding to the problem solution.

ok
**
If the return value *ok* is true, convergence is obtained; i.e.,

.. math::

   | F ( x , y_a, s_a, y_b, s_b, y_c, s_c ) |_\infty < \varepsilon

where :math:`|v|_\infty` is the infinity norm of the vector :math:`v`,
:math:`\varepsilon` is *epsilon* ,
:math:`x` is equal to *xout* ,
:math:`y_a, s_a \in \B{R}_+^n`,
:math:`y_b, s_b \in \B{R}_+^n` and
:math:`y_c, s_c \in \B{R}_+^m`.

KKT Conditions
**************
Give a vector :math:`v \in \B{R}^m` we define
:math:`D(v) \in \B{R}^{m \times m}` as the corresponding diagonal matrix.
We also define :math:`1_m \in \B{R}^m` as the vector of ones.
We define

.. math::

   F ( x , y_a, s_a, y_b, s_b, y_c, s_c )
   =
   \left(
   \begin{array}{c}
   g + G x - y_a + y_b + y_c^T C         \\
   a + s_a - x                           \\
   x + s_b - b                           \\
   C x + c + s_c                         \\
   D(s_a) D(y_a) 1_m                     \\
   D(s_b) D(y_b) 1_m                     \\
   D(s_c) D(y_c) 1_m
   \end{array}
   \right)

where
:math:`x \in \B{R}^n`,
:math:`y_a, s_a \in \B{R}_+^n`,
:math:`y_b, s_b \in \B{R}_+^n` and
:math:`y_c, s_c \in \B{R}_+^m`.
The KKT conditions for a solution of this problem is

.. math::

   F ( x , y_a, s_a, y_b, s_b, y_c, s_c ) = 0

{xrst_toc_hidden
   example/abs_normal/qp_box.cpp
   example/abs_normal/qp_box.xrst
}
Example
*******
The file :ref:`qp_box.cpp-name` contains an example and test of
``qp_box`` .

{xrst_end qp_box}
-----------------------------------------------------------------------------
*/
# include "qp_interior.hpp"

// BEGIN C++
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN PROTOTYPE
template <class Vector>
bool qp_box(
   size_t        level   ,
   const Vector& a       ,
   const Vector& b       ,
   const Vector& c       ,
   const Vector& C       ,
   const Vector& g       ,
   const Vector& G       ,
   double        epsilon ,
   size_t        maxitr  ,
   const Vector& xin     ,
   Vector&       xout    )
// END PROTOTYPE
{  double inf = std::numeric_limits<double>::infinity();
   //
   size_t n = a.size();
   size_t m = c.size();
   //
   CPPAD_ASSERT_KNOWN(level <= 2, "qp_interior: level is greater than 2");
   CPPAD_ASSERT_KNOWN(
      size_t(b.size()) == n, "qp_box: size of b is not n"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(C.size()) == m * n, "qp_box: size of C is not m * n"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(g.size()) == n, "qp_box: size of g is not n"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(G.size()) == n * n, "qp_box: size of G is not n * n"
   );
   if( level > 0 )
   {  std::cout << "start qp_box\n";
      CppAD::abs_print_mat("a", n, 1, a);
      CppAD::abs_print_mat("b", n, 1, b);
      CppAD::abs_print_mat("c", m, 1, c);
      CppAD::abs_print_mat("C", m, n, C);
      CppAD::abs_print_mat("g", 1, n, g);
      CppAD::abs_print_mat("G", n, n, G);
      CppAD::abs_print_mat("xin", n, 1, xin);
   }
   //
   // count number of lower and upper limits
   size_t n_limit = 0;
   for(size_t j = 0; j < n; j++)
   {  CPPAD_ASSERT_KNOWN(G[j * n + j] >= 0.0, "qp_box: G_{j,j} < 0.0");
      if( -inf < a[j] )
         ++n_limit;
      if( b[j] < inf )
         ++n_limit;
   }
   //
   // C_int and c_int define the extended constraints
   Vector C_int((m + n_limit) * n ), c_int(m + n_limit);
   for(size_t i = 0; i < size_t(C_int.size()); i++)
      C_int[i] = 0.0;
   //
   // put C * x + c <= 0 in C_int, c_int
   for(size_t i = 0; i < m; i++)
   {  c_int[i] = c[i];
      for(size_t j = 0; j < n; j++)
         C_int[i * n + j] = C[i * n + j];
   }
   //
   // put I * x - b <= 0 in C_int, c_int
   size_t i_limit = 0;
   for(size_t j = 0; j < n; j++) if( b[j] < inf )
   {  c_int[m + i_limit]            = - b[j];
      C_int[(m + i_limit) * n + j]  = 1.0;
      ++i_limit;
   }
   //
   // put a - I * x <= 0 in C_int, c_int
   for(size_t j = 0; j < n; j++) if( -inf < a[j] )
   {  c_int[m + i_limit]           = a[j];
      C_int[(m + i_limit) * n + j] = -1.0;
      ++i_limit;
   }
   Vector yout(m + n_limit), sout(m + n_limit);
   size_t level_int = 0;
   if( level == 2 )
      level_int = 1;
   bool ok = qp_interior( level_int,
      c_int, C_int, g, G, epsilon, maxitr, xin, xout, yout, sout
   );
   if( level > 0 )
   {  if( level < 2 )
         CppAD::abs_print_mat("xout", n, 1, xout);
      if( ok )
         std::cout << "end q_box: ok = true\n";
      else
         std::cout << "end q_box: ok = false\n";
   }
   return ok;
}

} // END_CPPAD_NAMESPACE
// END C++

# endif
