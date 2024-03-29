// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin taylor_ode.cpp}

Taylor's Ode Solver: An Example and Test
########################################

Purpose
*******
This example uses the method described in :ref:`taylor_ode-name`
to solve an ODE.

ODE
***
The ODE is defined by
:math:`y(0) = 0` and :math:`y^1 (t) = g[ y(t) ]`
where the function
:math:`g : \B{R}^n \rightarrow \B{R}^n` is defined by

.. math::

   g(y)
   =
   \left( \begin{array}{c}
         1                       \\
         y_1                     \\
         \vdots                  \\
         y_{n-1}
   \end{array} \right)

and the initial condition is :math:`z(0) = 0`.

ODE Solution
************
The solution for this example can be calculated by
starting with the first row and then using the solution
for the first row to solve the second and so on.
Doing this we obtain

.. math::

   y(t) =
   \left( \begin{array}{c}
      t           \\
      t^2 / 2     \\
      \vdots      \\
      t^n / n !
   \end{array} \right)

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end taylor_ode.cpp}
--------------------------------------------------------------------------
*/
// BEGIN C++

# include <cppad/cppad.hpp>

// =========================================================================
// define types for each level
namespace { // BEGIN empty namespace

   typedef CppAD::AD<double>          a_double;
   typedef CPPAD_TESTVECTOR(double)   d_vector;
   typedef CPPAD_TESTVECTOR(a_double) a_vector;

   a_vector ode(const a_vector y)
   {  size_t n = y.size();
      a_vector g(n);
      g[0] = 1;
      for(size_t k = 1; k < n; k++)
         g[k] = y[k-1];
      return g;
   }

}

// -------------------------------------------------------------------------
// use Taylor's method to solve this ordinary differential equaiton
bool taylor_ode(void)
{  // initialize the return value as true
   bool ok = true;

   // The ODE does not depend on the arugment values
   // so only tape once, also note that ode does not depend on t
   size_t n = 5;    // number of independent and dependent variables
   a_vector ay(n), ag(n);
   CppAD::Independent( ay );
   ag = ode(ay);
   CppAD::ADFun<double> g(ay, ag);

   // initialize the solution vector at time zero
   d_vector y(n);
   for(size_t j = 0; j < n; j++)
      y[j] = 0.0;

   size_t order   = n;   // order of the Taylor method
   size_t n_step  = 4;   // number of time steps
   double dt      = 0.5; // step size in time

   // Taylor coefficients of order k
   d_vector yk(n), zk(n);

   // loop with respect to each step of Taylor's method
   for(size_t i_step = 0; i_step < n_step; i_step++)
   {  // Use Taylor's method to take a step
      yk           = y;     // initialize y^{(k)}  for k = 0
      double dt_kp = dt;    // initialize dt^(k+1) for k = 0
      for(size_t k = 0; k < order; k++)
      {  // evaluate k-th order Taylor coefficient of z(t) = g(y(t))
         zk = g.Forward(k, yk);

         for(size_t j = 0; j < n; j++)
         {  // convert to (k+1)-Taylor coefficient for y
            yk[j] = zk[j] / double(k + 1);

            // add term for to this Taylor coefficient
            // to solution for y(t, x)
            y[j] += yk[j] * dt_kp;
         }
         // next power of t
         dt_kp *= dt;
      }
   }

   // check solution of the ODE,
   // Taylor's method should have no truncation error for this case
   double eps   = 100. * std::numeric_limits<double>::epsilon();
   double check = 1.;
   double t     = double(n_step) * dt;
   for(size_t i = 0; i < n; i++)
   {  check *= t / double(i + 1);
      ok &= CppAD::NearEqual(y[i], check, eps, eps);
   }

   return ok;
}

// END C++
