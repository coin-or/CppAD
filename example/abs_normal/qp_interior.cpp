// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin qp_interior.cpp}
{xrst_spell
  rlr
}

abs_normal qp_interior: Example and Test
########################################

Problem
*******
Our original problem is

.. math::

   \R{minimize} \; | u - 1| \; \R{w.r.t} \; u \in \B{R}

We reformulate this as the following problem

.. math::

   \begin{array}{rlr}
      \R{minimize}      & v             & \R{w.r.t} \; (u,v) \in \B{R}^2 \\
      \R{subject \; to} &  u - 1 \leq v \\
                         &  1 - u \leq v
   \end{array}

This is equivalent to

.. math::

   \begin{array}{rlr}
      \R{minimize}
      & (0, 1) \cdot (u, v)^T  & \R{w.r.t} \; (u,v) \in \B{R}^2 \\
      \R{subject \; to}
      &
      \left( \begin{array}{cc} 1 & -1 \\ -1 & -1 \end{array} \right)
      \left( \begin{array}{c} u \\ v \end{array} \right)
      +
      \left( \begin{array}{c} -1 \\ 1 \end{array} \right)
      \leq
      0
   \end{array}

which is in the form expected by :ref:`qp_interior-name` .

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end qp_interior.cpp}
*/
// BEGIN C++
# include <limits>
# include <cppad/utility/vector.hpp>
# include "qp_interior.hpp"

bool qp_interior(void)
{  bool ok = true;
   typedef CppAD::vector<double> vector;
   //
   size_t n = 2;
   size_t m = 2;
   vector C(m*n), c(m), G(n*n), g(n), xin(n), xout(n), yout(m), sout(m);
   C[ 0 * n + 0 ] =  1.0; // C(0,0)
   C[ 0 * n + 1 ] = -1.0; // C(0,1)
   C[ 1 * n + 0 ] = -1.0; // C(1,0)
   C[ 1 * n + 1 ] = -1.0; // C(1,1)
   //
   c[0]           = -1.0;
   c[1]           =  1.0;
   //
   g[0]           =  0.0;
   g[1]           =  1.0;
   //
   // G = 0
   for(size_t i = 0; i < n * n; i++)
      G[i] = 0.0;
   //
   // If (u, v) = (0,2), C * (u, v) + c = (-2,-2)^T + (1,-1)^T < 0
   // Hence (0, 2) is feasible.
   xin[0] = 0.0;
   xin[1] = 2.0;
   //
   double epsilon = 99.0 * std::numeric_limits<double>::epsilon();
   size_t maxitr  = 10;
   size_t level   = 0;
   //
   ok &= CppAD::qp_interior(
      level, c, C, g, G, epsilon, maxitr, xin, xout, yout, sout
   );
   //
   // check optimal value for u
   ok &= std::fabs( xout[0] - 1.0 ) < epsilon;
   // check optimal value for v
   ok &= std::fabs( xout[1] ) < epsilon;
   //
   return ok;
}
// END C++
