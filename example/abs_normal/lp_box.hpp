# ifndef CPPAD_EXAMPLE_ABS_NORMAL_LP_BOX_HPP
# define CPPAD_EXAMPLE_ABS_NORMAL_LP_BOX_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin lp_box}
{xrst_spell
   maxitr
   rl
   xout
}
abs_normal: Solve a Linear Program With Box Constraints
#######################################################

Syntax
******

| *ok* = ``lp_box`` (
| |tab| *level* , *A* , *b* , *c* , *d* , *maxitr* , *xout*
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
:ref:`lp_box.hpp-name` .

Problem
*******
We are given
:math:`A \in \B{R}^{m \times n}`,
:math:`b \in \B{R}^m`,
:math:`c \in \B{R}^n`,
:math:`d \in \B{R}^n`,
This routine solves the problem

.. math::

   \begin{array}{rl}
   \R{minimize} &
   c^T x \; \R{w.r.t} \; x \in \B{R}^n
   \\
   \R{subject \; to} & A x + b \leq 0 \; \R{and} \; - d \leq x \leq d
   \end{array}

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
a trace of the ``lp_box`` operations is printed.
If *level*  >= 2 ,
the objective and primal variables :math:`x` are printed
at each :ref:`simplex_method-name` iteration.
If *level*  == 3 ,
the simplex tableau is printed at each simplex iteration.

A
*
This is a :ref:`row-major<glossary@Row-major Representation>` representation
of the matrix :math:`A` in the problem.

b
*
This is the vector :math:`b` in the problem.

c
*
This is the vector :math:`c` in the problem.

d
*
This is the vector :math:`d` in the problem.
If :math:`d_j` is infinity, there is no limit for the size of
:math:`x_j`.

maxitr
******
This is the maximum number of newton iterations to try before giving up
on convergence.

xout
****
This argument has size is *n* and
the input value of its elements does no matter.
Upon return it is the primal variables
:math:`x` corresponding to the problem solution.

ok
**
If the return value *ok* is true, an optimal solution was found.
{xrst_toc_hidden
   example/abs_normal/lp_box.cpp
   example/abs_normal/lp_box.xrst
}
Example
*******
The file :ref:`lp_box.cpp-name` contains an example and test of
``lp_box`` .

{xrst_end lp_box}
-----------------------------------------------------------------------------
*/
# include "simplex_method.hpp"

// BEGIN C++
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN PROTOTYPE
template <class Vector>
bool lp_box(
   size_t        level   ,
   const Vector& A       ,
   const Vector& b       ,
   const Vector& c       ,
   const Vector& d       ,
   size_t        maxitr  ,
   Vector&       xout    )
// END PROTOTYPE
{  double inf = std::numeric_limits<double>::infinity();
   //
   size_t m = b.size();
   size_t n = c.size();
   //
   CPPAD_ASSERT_KNOWN(
      level <= 3, "lp_box: level is greater than 3");
   CPPAD_ASSERT_KNOWN(
      size_t(A.size()) == m * n, "lp_box: size of A is not m * n"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(d.size()) == n, "lp_box: size of d is not n"
   );
   if( level > 0 )
   {  std::cout << "start lp_box\n";
      CppAD::abs_print_mat("A", m, n, A);
      CppAD::abs_print_mat("b", m, 1, b);
      CppAD::abs_print_mat("c", n, 1, c);
      CppAD::abs_print_mat("d", n, 1, d);
   }
   //
   // count number of limits
   size_t n_limit = 0;
   for(size_t j = 0; j < n; j++)
   {  if( d[j] < inf )
         n_limit += 1;
   }
   //
   // A_simplex and b_simplex define the extended constraints
   Vector A_simplex((m + 2 * n_limit) * (2 * n) ), b_simplex(m + 2 * n_limit);
   for(size_t i = 0; i < size_t(A_simplex.size()); i++)
      A_simplex[i] = 0.0;
   //
   // put A * x + b <= 0 in A_simplex, b_simplex
   for(size_t i = 0; i < m; i++)
   {  b_simplex[i] = b[i];
      for(size_t j = 0; j < n; j++)
      {  // x_j^+ coefficient (positive component)
         A_simplex[i * (2 * n) + 2 * j]     =   A[i * n + j];
         // x_j^- coefficient (negative component)
         A_simplex[i * (2 * n) + 2 * j + 1] = - A[i * n + j];
      }
   }
   //
   // put | x_j | <= d_j in A_simplex, b_simplex
   size_t i_limit = 0;
   for(size_t j = 0; j < n; j++) if( d[j] < inf )
   {
      // x_j^+ <= d_j constraint
      b_simplex[ m + 2 * i_limit]                         = - d[j];
      A_simplex[(m + 2 * i_limit) * (2 * n) + 2 * j]      = 1.0;
      //
      // x_j^- <= d_j constraint
      b_simplex[ m + 2 * i_limit + 1]                         = - d[j];
      A_simplex[(m + 2 * i_limit + 1) * (2 * n) + 2 * j + 1]  = 1.0;
      //
      ++i_limit;
   }
   //
   // c_simples
   Vector c_simplex(2 * n);
   for(size_t j = 0; j < n; j++)
   {  // x_j+ component
      c_simplex[2 * j]     = c[j];
      // x_j^- component
      c_simplex[2 * j + 1] = - c[j];
   }
   size_t level_simplex = 0;
   if( level >= 2 )
      level_simplex = level - 1;
   //
   Vector x_simplex(2 * n);
   bool ok = CppAD::simplex_method(
      level_simplex, A_simplex, b_simplex, c_simplex, maxitr, x_simplex
   );
   for(size_t j = 0; j < n; j++)
      xout[j] = x_simplex[2 * j] - x_simplex[2 * j + 1];
   if( level > 0 )
   {  CppAD::abs_print_mat("xout", n, 1, xout);
      if( ok )
         std::cout << "end lp_box: ok = true\n";
      else
         std::cout << "end lp_box: ok = false\n";
   }
   return ok;
}

} // END_CPPAD_NAMESPACE
// END C++

# endif
