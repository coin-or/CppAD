// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin lp_box.cpp}
{xrst_spell
  rl
}

abs_normal lp_box: Example and Test
###################################

Problem
*******
Our original problem is

.. math::

   \begin{array}{rl}
   \R{minimize}      & x_0 - x_1 \; \R{w.r.t} \; x \in \B{R}^2 \\
   \R{subject \; to} & -2 \leq x_0 \leq +2 \; \R{and} \; -2 \leq x_1 \leq +2
   \end{array}

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end lp_box.cpp}
*/
// BEGIN C++
# include <limits>
# include <cppad/utility/vector.hpp>
# include "lp_box.hpp"

bool lp_box(void)
{  bool ok = true;
   typedef CppAD::vector<double> vector;
   double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
   //
   size_t n = 2;
   size_t m = 0;
   vector A(m), b(m), c(n), d(n), xout(n);
   c[0] = +1.0;
   c[1] = -1.0;
   //
   d[0] = +2.0;
   d[1] = +2.0;
   //
   size_t level   = 0;
   size_t maxitr  = 20;
   //
   ok &= CppAD::lp_box(level, A, b, c, d, maxitr, xout);
   //
   // check optimal value for x
   ok &= std::fabs( xout[0] + 2.0 ) < eps99;
   ok &= std::fabs( xout[1] - 2.0 ) < eps99;
   //
   return ok;
}
// END C++
